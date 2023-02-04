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
    if (event->menuName == RE::JournalMenu::MENU_NAME) {
      Settings::get_singleton().load();
      event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    }
    if (event->menuName == RE::MapMenu::MENU_NAME) {
      event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    }
    if (event->menuName == RE::CraftingMenu::MENU_NAME) {
      event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    }
    if (event->menuName == RE::MagicMenu::MENU_NAME) {
      event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    }
    if (event->menuName == RE::InventoryMenu::MENU_NAME) {
      event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    }
    if (event->menuName == RE::BookMenu::MENU_NAME) {
      event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    }
    if (event->menuName == RE::BarterMenu::MENU_NAME) {
      event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    }
    if (event->menuName == RE::FavoritesMenu::MENU_NAME) {
      event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    }
    if (event->menuName == RE::StatsMenu::MENU_NAME) {
      event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    }
    if (event->menuName == RE::MainMenu::MENU_NAME) {
      event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    }
    if (event->menuName == RE::TweenMenu::MENU_NAME) {
      event->opening ? ReflyemWidget::hide() : ReflyemWidget::show();
    }
  }
  return RE::BSEventNotifyControl::kContinue;
}