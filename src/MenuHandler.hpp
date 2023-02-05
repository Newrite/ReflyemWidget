#pragma once

struct MenuHandler final : public RE::BSTEventSink<RE::MenuOpenCloseEvent> {
public:
  [[nodiscard]] static auto get_singleton() noexcept -> MenuHandler*;
  static auto               register_() -> void;

  auto ProcessEvent(const RE::MenuOpenCloseEvent*               event,
                    RE::BSTEventSource<RE::MenuOpenCloseEvent>* event_source)
      -> RE::BSEventNotifyControl override;
};
