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
    logger::debug("Menu: {}  is {}", event->menuName, event->opening ? "open" : "close");
    if (event->menuName == RE::RaceSexMenu::MENU_NAME) {
      event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    }
    if (event->menuName == RE::LoadingMenu::MENU_NAME) {
      event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    }
    if (event->menuName == RE::JournalMenu::MENU_NAME) {
      Settings::get_singleton().load();
      event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    }
    if (event->menuName == RE::MapMenu::MENU_NAME) {
      event->opening ? ReflyemWidget::toggle_visibility(false) : ReflyemWidget::toggle_visibility(true);
    }
    if (event->menuName == RE::TweenMenu::MENU_NAME) {
      event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    }
    if (event->menuName == RE::MagicMenu::MENU_NAME) {
      event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    }
    if (event->menuName == RE::InventoryMenu::MENU_NAME) {
      event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    }
    if (event->menuName == RE::FaderMenu::MENU_NAME) {
      event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    }
  }
  logger::debug("Menu event continue");
  return RE::BSEventNotifyControl::kContinue;
}