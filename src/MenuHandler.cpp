#include "MenuHandler.hpp"
#include "Settings.hpp"

auto MenuHandler::get_singleton() noexcept -> MenuHandler* {
  static MenuHandler instance;
  return std::addressof(instance);
}

auto             MenuHandler::register_() -> void {
  if (const auto ui = RE::UI::GetSingleton()) {
    ui->AddEventSink(get_singleton());
  }
}

auto MenuHandler::ProcessEvent(const RE::MenuOpenCloseEvent* event,
                               RE::BSTEventSource<RE::MenuOpenCloseEvent>*) ->
  RE::BSEventNotifyControl {
  if (event) {
    if (event->menuName == RE::JournalMenu::MENU_NAME) {
      Settings::get_singleton().load();
    }
  }
  return RE::BSEventNotifyControl::kContinue;
}
