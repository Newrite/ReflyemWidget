#pragma once

struct ReflyemWidget final : RE::IMenu {
public:
  static constexpr const char* MENU_PATH = "ReflyemWidget";
  static constexpr const char* MENU_NAME = "ReflyemWidget";

  ReflyemWidget();

  static auto register_() -> void;
  static auto show() -> void;
  static auto hide() -> void;
  static auto update() -> void;
  static auto apply_layout(const RE::GPtr<RE::IMenu>& reflyem_widget) -> void;
  static auto toggle_visibility(bool mode) -> void;

  static auto actor_value_regeneration_value(const RE::ActorValue av, const RE::ActorValue av_rate,
                                             const RE::ActorValue av_rate_mult) -> float;

  static auto creator() -> RE::stl::owner<RE::IMenu*> { return new ReflyemWidget(); }

  auto AdvanceMovie(float interval, uint32_t current_time) -> void override;

private:
  class ReflyemLogger final : public RE::GFxLog {
  public:
    void LogMessageVarg(LogMessageType, const char* fmt, const std::va_list arg_list) override {
      std::string format(fmt ? fmt : "");
      while (!format.empty() && format.back() == '\n') {
        format.pop_back();
      }

      std::va_list args;
      va_copy(args, arg_list);
      std::vector<char> buf(
          static_cast<std::size_t>(std::vsnprintf(0, 0, format.c_str(), arg_list) + 1));
      std::vsnprintf(buf.data(), buf.size(), format.c_str(), args);
      va_end(args);

      logger::info("{}"sv, buf.data());
    }
  };
};