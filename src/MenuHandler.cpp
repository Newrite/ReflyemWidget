#include "MenuHandler.hpp"
#include "Settings.hpp"
#include "Widget.hpp"

auto MenuHandler::get_singleton() noexcept -> MenuHandler* {
  static MenuHandler instance;
  return std::addressof(instance);
}

auto MenuHandler::register_() -> void {
  if (const auto ui = RE::UI::GetSingleton()) {
    ui->AddEventSink(get_singleton());
  }
}

auto MenuHandler::ProcessEvent(const RE::MenuOpenCloseEvent* event,
                               RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
    -> RE::BSEventNotifyControl {

  if (event) {
    logger::debug("Menu: {}  is {}", event->menuName.c_str(), event->opening ? "open" : "close");
    if (event->menuName == RE::HUDMenu::MENU_NAME) {
      event->opening ? ReflyemWidget::show() : ReflyemWidget::hide();
    }
    if (event->menuName == ReflyemWidget::TRUE_HUD) {
      event->opening ? ReflyemWidget::show() : ReflyemWidget::hide();
    }
    // if (event->menuName == RE::LoadingMenu::MENU_NAME) {
    //   event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    // }
    if (event->menuName == RE::JournalMenu::MENU_NAME) {
      Settings::get_singleton().load();
      // event->opening ? ReflyemWidget::toggle_visibility(false) :
      // ReflyemWidget::toggle_visibility(true);
    }
    // if (event->menuName == RE::MapMenu::MENU_NAME) {
    //   event->opening ? ReflyemWidget::toggle_visibility(false) :
    //   ReflyemWidget::toggle_visibility(true);
    // }
    // if (event->menuName == RE::TweenMenu::MENU_NAME) {
    //   event->opening ? ReflyemWidget::toggle_visibility(false) :
    //   ReflyemWidget::toggle_visibility(true);
    // }
    // if (event->menuName == RE::MagicMenu::MENU_NAME) {
    //   event->opening ? ReflyemWidget::toggle_visibility(false) :
    //   ReflyemWidget::toggle_visibility(true);
    // }
    // if (event->menuName == RE::InventoryMenu::MENU_NAME) {
    //   event->opening ? ReflyemWidget::toggle_visibility(false) :
    //   ReflyemWidget::toggle_visibility(true);
    // }
    // if (event->menuName == RE::FaderMenu::MENU_NAME) {
    //   event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    // }

    // copypast from true hud
    
    using ContextID = RE::UserEvents::INPUT_CONTEXT_ID;
    // Hide the widgets when a menu is open
    auto control_map = RE::ControlMap::GetSingleton();
    if (control_map) {
      auto& priorityStack = control_map->contextPriorityStack;
      if (priorityStack.empty()) {
        ReflyemWidget::toggle_visibility(false);
      } else if (priorityStack.back() == ContextID::kGameplay ||
                 priorityStack.back() == ContextID::kFavorites ||
                 priorityStack.back() == ContextID::kConsole) {
        ReflyemWidget::toggle_visibility(true);
      } else if ((priorityStack.back() == ContextID::kCursor ||
                  priorityStack.back() == ContextID::kItemMenu ||
                  priorityStack.back() == ContextID::kMenuMode ||
                  priorityStack.back() == ContextID::kInventory) &&
                 (RE::UI::GetSingleton()->IsMenuOpen(RE::DialogueMenu::MENU_NAME) ||
                  RE::UI::GetSingleton()->IsMenuOpen(RE::CraftingMenu::MENU_NAME) ||
                  (RE::UI::GetSingleton()->IsMenuOpen(RE::BarterMenu::MENU_NAME) ||
                   RE::UI::GetSingleton()->IsMenuOpen(RE::ContainerMenu::MENU_NAME) ||
                   RE::UI::GetSingleton()->IsMenuOpen(RE::GiftMenu::MENU_NAME) ||
                   RE::UI::GetSingleton()->IsMenuOpen(RE::InventoryMenu::MENU_NAME)))) {
        ReflyemWidget::toggle_visibility(false);
      } else {
        ReflyemWidget::toggle_visibility(false);
      }
    }
  }
  logger::debug("Menu event continue");
  return RE::BSEventNotifyControl::kContinue;
}