#include "Widget.hpp"
#include "Settings.hpp"

ReflyemWidget::ReflyemWidget() {
  const auto scale_form_manager = RE::BSScaleformManager::GetSingleton();

  inputContext  = Context::kNone;
  depthPriority = 0;

  menuFlags.set(RE::UI_MENU_FLAGS::kAlwaysOpen);
  menuFlags.set(RE::UI_MENU_FLAGS::kRequiresUpdate);
  menuFlags.set(RE::UI_MENU_FLAGS::kAllowSaving);

  if (uiMovie) {
    uiMovie->SetMouseCursorCount(0);
  }

  scale_form_manager->LoadMovieEx(this, MENU_PATH, [](RE::GFxMovieDef* def) -> void {
    def->SetState(RE::GFxState::StateType::kLog, RE::make_gptr<ReflyemLogger>().get());
  });
}

auto             ReflyemWidget::register_() -> void {
  if (const auto ui = RE::UI::GetSingleton()) {
    ui->Register(MENU_NAME, creator);

    show();
  }
}

auto             ReflyemWidget::show() -> void {
  if (const auto message_queue = RE::UIMessageQueue::GetSingleton()) {
    message_queue->AddMessage(MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
  }
}

auto             ReflyemWidget::hide() -> void {
  if (const auto message_queue = RE::UIMessageQueue::GetSingleton()) {
    message_queue->AddMessage(MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
  }
}

auto ReflyemWidget::actor_value_regeneration_value(const RE::ActorValue av,
                                                   const RE::ActorValue av_rate,
                                                   const RE::ActorValue av_rate_mult) -> float {

  const auto player = RE::PlayerCharacter::GetSingleton();
  if (!player) {
    return 0.f;
  }

  const auto active_effects = player->GetActiveEffectList();
  if (!active_effects) {
    return 0.f;
  }

  const auto validate_active_effect = [](const RE::ActiveEffect* active_effect) -> bool {
    return active_effect && !active_effect->flags.any(RE::ActiveEffect::Flag::kInactive) &&
           active_effect->effect && active_effect->effect->baseEffect && !active_effect->effect->
           baseEffect->data.flags.any(RE::EffectSetting::EffectSettingData::Flag::kRecover) && !
           active_effect->effect->baseEffect->IsDetrimental();
  };

  const auto                    value_peak_mod_or_value_mod_is_av = [av
      ](const RE::ActiveEffect* active_effect) -> bool {
    return (active_effect->effect->baseEffect->HasArchetype(
                RE::EffectSetting::Archetype::kPeakValueModifier) ||
            active_effect->effect->baseEffect->
                           HasArchetype(RE::EffectSetting::Archetype::kValueModifier)) &&
           active_effect->effect->baseEffect->data.primaryAV == av;
  };

  auto restore_value_counter = 0.f;

  for (const auto active_effect : *active_effects) {

    if (!validate_active_effect(active_effect)) {
      continue;
    }

    if (value_peak_mod_or_value_mod_is_av(active_effect)) {
      restore_value_counter += active_effect->magnitude;
      continue;
    }

    if (active_effect->effect->baseEffect->HasArchetype(
        RE::EffectSetting::Archetype::kDualValueModifier)) {

      const auto base_effect = active_effect->effect->baseEffect;

      if (base_effect->data.primaryAV == av && base_effect->data.secondaryAV != av) {
        restore_value_counter += active_effect->magnitude;
      } else if (base_effect->data.primaryAV == av && base_effect->data.secondaryAV == av) {
        restore_value_counter += (active_effect->magnitude * base_effect->data.secondAVWeight);
        restore_value_counter += active_effect->magnitude;
      } else if (base_effect->data.secondaryAV == av) {
        restore_value_counter += (active_effect->magnitude * base_effect->data.secondAVWeight);
      }

    }

  }

  auto regeneration = 0.f;

  const auto rate      = player->GetActorValue(av_rate);
  const auto mult_rate = player->GetActorValue(av_rate_mult);

  if (rate <= 0.f || mult_rate <= -100.f) {
    regeneration = 0.f;
  } else {
    const auto max_value = player->GetPermanentActorValue(av);
    const auto percent   = rate * (1.f + (mult_rate / 100.f));
    regeneration         = (max_value / 100.f) * percent;
  }

  return restore_value_counter + regeneration;
}

auto ReflyemWidget::update() -> void {
  const auto reflyem_widget = RE::UI::GetSingleton()->GetMenu(MENU_NAME);
  if (!reflyem_widget || !reflyem_widget->uiMovie) {
    return;
  }

  const auto player = RE::PlayerCharacter::GetSingleton();
  if (!player) {
    return;
  }

  const auto heath_regen_string =
      fmt::format("{:.{}f}"sv, actor_value_regeneration_value(
                      RE::ActorValue::kHealth,
                      RE::ActorValue::kHealRate,
                      RE::ActorValue::kHealRateMult), 2);

  const auto stamina_regen_string =
      fmt::format("{:.{}f}"sv, actor_value_regeneration_value(
                      RE::ActorValue::kStamina,
                      RE::ActorValue::kStaminaRate,
                      RE::ActorValue::kStaminaRateMult), 2);

  const auto magicka_regen_string =
      fmt::format("{:.{}f}"sv, actor_value_regeneration_value(
                      RE::ActorValue::kMagicka,
                      RE::ActorValue::kMagickaRate,
                      RE::ActorValue::kMagickaRateMult), 2);

  const auto value_health_string =
      fmt::format("{} / {}"sv, static_cast<int32_t>(player->GetActorValue(RE::ActorValue::kHealth)),
                  static_cast<int32_t>(player->GetPermanentActorValue(RE::ActorValue::kHealth)));

  const auto value_stamina_string =
      fmt::format("{} / {}"sv, static_cast<int32_t>(player->GetActorValue(RE::ActorValue::kStamina)),
                  static_cast<int32_t>(player->GetPermanentActorValue(RE::ActorValue::kStamina)));

  const auto value_magicka_string =
      fmt::format("{} / {}"sv, static_cast<int32_t>(player->GetActorValue(RE::ActorValue::kMagicka)),
                  static_cast<int32_t>(player->GetPermanentActorValue(RE::ActorValue::kMagicka)));

  logger::debug("HR: {} SR: {} MR: {} | VH: {} VS: {} VM: {}"sv, heath_regen_string,
                stamina_regen_string, magicka_regen_string, value_health_string,
                value_stamina_string, value_magicka_string);

  const RE::GFxValue health_regen{heath_regen_string};
  const RE::GFxValue stamina_regen{stamina_regen_string};
  const RE::GFxValue magicka_regen{magicka_regen_string};

  const RE::GFxValue value_health{value_health_string};
  const RE::GFxValue value_stamina{value_stamina_string};
  const RE::GFxValue value_magicka{value_magicka_string};

  reflyem_widget
      ->uiMovie
      ->Invoke("mrlreflyemwidget.setHealthText", nullptr, &health_regen, 1);
  reflyem_widget
      ->uiMovie
      ->Invoke("mrlreflyemwidget.setStaminaText", nullptr, &stamina_regen, 1);
  reflyem_widget
      ->uiMovie
      ->Invoke("mrlreflyemwidget.setMagickaText", nullptr, &magicka_regen, 1);

  reflyem_widget
      ->uiMovie
      ->Invoke("mrlreflyemwidget.setValueHealthText", nullptr, &value_health, 1);
  reflyem_widget
      ->uiMovie
      ->Invoke("mrlreflyemwidget.setValueStaminaText", nullptr, &value_stamina,
               1);
  reflyem_widget
      ->uiMovie
      ->Invoke("mrlreflyemwidget.setValueMagickaText", nullptr, &value_magicka,
               1);

  apply_layout(reflyem_widget);
}

void ReflyemWidget::AdvanceMovie(const float interval, const uint32_t current_time) {
  update();
  IMenu::AdvanceMovie(interval, current_time);
}

auto ReflyemWidget::toggle_visibility(const bool mode) -> void {
  const auto ui = RE::UI::GetSingleton();
  if (!ui) { return; }

  const auto overlay_menu = ui->GetMenu(MENU_NAME);
  if (!overlay_menu || !overlay_menu->uiMovie) {
    return;
  }

  overlay_menu->uiMovie->SetVisible(mode);
}

auto ReflyemWidget::apply_layout(const RE::GPtr<RE::IMenu>& reflyem_widget) -> void {
  if (!reflyem_widget || !reflyem_widget->uiMovie)
    return;

  const auto settings = Settings::get_singleton();
  
  const RE::GFxValue pos_health_x = settings.pos_regen_health_x();
  const RE::GFxValue pos_health_y = settings.pos_regen_health_y();

  const RE::GFxValue pos_stamina_x = settings.pos_regen_stamina_x();
  const RE::GFxValue pos_stamina_y = settings.pos_regen_stamina_y();

  const RE::GFxValue pos_magicka_x = settings.pos_regen_magicka_x();
  const RE::GFxValue pos_magicka_y = settings.pos_regen_magicka_y();

  const RE::GFxValue scale = settings.scale();

  const RE::GFxValue pos_value_health_x = settings.pos_value_health_x();
  const RE::GFxValue pos_value_health_y = settings.pos_value_health_y();

  const RE::GFxValue pos_value_stamina_x = settings.pos_value_stamina_x();
  const RE::GFxValue pos_value_stamina_y = settings.pos_value_stamina_y();

  const RE::GFxValue pos_value_magicka_x = settings.pos_value_magicka_x();
  const RE::GFxValue pos_value_magicka_y = settings.pos_value_magicka_y();

  reflyem_widget
      ->uiMovie
      ->Invoke("mrlreflyemwidget.setHealthY", nullptr, &pos_health_y, 1);
  reflyem_widget
      ->uiMovie
      ->Invoke("mrlreflyemwidget.setHealthX", nullptr, &pos_health_x, 1);

  reflyem_widget
      ->uiMovie
      ->Invoke("mrlreflyemwidget.setStaminaY", nullptr, &pos_stamina_y, 1);
  reflyem_widget
      ->uiMovie
      ->Invoke("mrlreflyemwidget.setStaminaX", nullptr, &pos_stamina_x, 1);

  reflyem_widget
      ->uiMovie
      ->Invoke("mrlreflyemwidget.setMagickaY", nullptr, &pos_magicka_y, 1);
  reflyem_widget
      ->uiMovie
      ->Invoke("mrlreflyemwidget.setMagickaX", nullptr, &pos_magicka_x, 1);

  reflyem_widget
      ->uiMovie
      ->Invoke("mrlreflyemwidget.setValueHealthY", nullptr, &pos_value_health_y,
               1);
  reflyem_widget
      ->uiMovie
      ->Invoke("mrlreflyemwidget.setValueHealthX", nullptr, &pos_value_health_x,
               1);

  reflyem_widget
      ->uiMovie
      ->Invoke("mrlreflyemwidget.setValueStaminaY", nullptr,
               &pos_value_stamina_y, 1);
  reflyem_widget
      ->uiMovie
      ->Invoke("mrlreflyemwidget.setValueStaminaX", nullptr,
               &pos_value_stamina_x, 1);

  reflyem_widget
      ->uiMovie
      ->Invoke("mrlreflyemwidget.setValueMagickaY", nullptr,
               &pos_value_magicka_y, 1);
  reflyem_widget
      ->uiMovie
      ->Invoke("mrlreflyemwidget.setValueMagickaX", nullptr,
               &pos_value_magicka_x, 1);

  reflyem_widget
      ->uiMovie
      ->SetVariable("mrlreflyemwidget._xscale", scale);
  reflyem_widget
      ->uiMovie
      ->SetVariable("mrlreflyemwidget._yscale", scale);

}
