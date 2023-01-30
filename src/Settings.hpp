#pragma once

#include "SimpleIni.hpp"
#include <latch>

struct Settings {
private:
  Settings() = default;

  double pos_regen_health_x_{210.};
  double pos_regen_health_y_{600.};

  double pos_regen_stamina_x_{210.};
  double pos_regen_stamina_y_{660.};

  double pos_regen_magicka_x_{210.};
  double pos_regen_magicka_y_{630.};

  double scale_{100.};

  double pos_value_health_x_{110.};
  double pos_value_health_y_{600.};

  double pos_value_stamina_x_{110.};
  double pos_value_stamina_y_{660.};

  double pos_value_magicka_x_{110.};
  double pos_value_magicka_y_{630.};

public:
  [[nodiscard]] auto pos_regen_health_x() const -> double { return pos_regen_health_x_; }
  [[nodiscard]] auto pos_regen_health_y() const -> double { return pos_regen_health_y_; }

  [[nodiscard]] auto pos_regen_stamina_x() const -> double { return pos_regen_stamina_x_; }
  [[nodiscard]] auto pos_regen_stamina_y() const -> double { return pos_regen_stamina_y_; }

  [[nodiscard]] auto pos_regen_magicka_x() const -> double { return pos_regen_magicka_x_; }
  [[nodiscard]] auto pos_regen_magicka_y() const -> double { return pos_regen_magicka_y_; }

  [[nodiscard]] auto scale() const -> double { return scale_; }

  [[nodiscard]] auto pos_value_health_x() const -> double { return pos_value_health_x_; }
  [[nodiscard]] auto pos_value_health_y() const -> double { return pos_value_health_y_; }

  [[nodiscard]] auto pos_value_stamina_x() const -> double { return pos_value_stamina_x_; }
  [[nodiscard]] auto pos_value_stamina_y() const -> double { return pos_value_stamina_y_; }

  [[nodiscard]] auto pos_value_magicka_x() const -> double { return pos_value_magicka_x_; }
  [[nodiscard]] auto pos_value_magicka_y() const -> double { return pos_value_magicka_y_; }

  [[nodiscard]] static auto get_singleton() noexcept -> Settings& {
    static Settings         instance;
    static std::atomic_bool initialized;
    static std::latch       latch{1};

    if (!initialized.exchange(true)) {
      logger::info("Settings created");
      latch.count_down();
    }
    latch.wait();
    return instance;
  }

  auto load() -> void {

    constexpr auto path_to_ini         = L"Data/MCM/Settings/Reflyem - Widget.ini";
    constexpr auto path_to_ini_default = L"Data/MCM/Config/Reflyem - Widget/settings.ini";

    constexpr auto section = L"Main";
    constexpr auto f_pos_regen_health_x = L"fPosRegenHealthX";
    constexpr auto f_pos_regen_health_y = L"fPosRegenHealthY";
    constexpr auto f_pos_regen_stamina_x = L"fPosRegenStaminaX";
    constexpr auto f_pos_regen_stamina_y = L"fPosRegenStaminaY";
    constexpr auto f_pos_regen_magicka_x = L"fPosRegenMagickaX";
    constexpr auto f_pos_regen_magicka_y = L"fPosRegenMagickaY";
    constexpr auto f_pos_value_health_x = L"fPosValueHealthX";
    constexpr auto f_pos_value_health_y = L"fPosValueHealthY";
    constexpr auto f_pos_value_stamina_x = L"fPosValueStaminaX";
    constexpr auto f_pos_value_stamina_y = L"fPosValueStaminaY";
    constexpr auto f_pos_value_magicka_x = L"fPosValueMagickaX";
    constexpr auto f_pos_value_magicka_y = L"fPosValueMagickaY";
    constexpr auto f_scale_widget = L"fScaleWidget";

    const auto read_double = [this](const CSimpleIni& ini, const wchar_t* key, double& value) -> void {
      if (ini.GetValue(section, key)) {
        value = ini.GetDoubleValue(section, key);
      }
    };

    const auto read_settings = [this, read_double](CSimpleIni& ini, const wchar_t* path) -> void {
      ini.LoadFile(path);

      read_double(ini, f_pos_regen_health_x, pos_regen_health_x_);
      read_double(ini, f_pos_regen_health_y, pos_regen_health_y_);
      
      read_double(ini, f_pos_regen_stamina_x, pos_regen_stamina_x_);
      read_double(ini, f_pos_regen_stamina_y, pos_regen_stamina_y_);
      
      read_double(ini, f_pos_regen_magicka_x, pos_regen_magicka_x_);
      read_double(ini, f_pos_regen_magicka_y, pos_regen_magicka_y_);

      read_double(ini, f_pos_value_health_x, pos_value_health_x_);
      read_double(ini, f_pos_value_health_y, pos_value_health_y_);
      
      read_double(ini, f_pos_value_stamina_x, pos_value_stamina_x_);
      read_double(ini, f_pos_value_stamina_y, pos_value_stamina_y_);
      
      read_double(ini, f_pos_value_magicka_x, pos_value_magicka_x_);
      read_double(ini, f_pos_value_magicka_y, pos_value_magicka_y_);
      
      read_double(ini, f_scale_widget, scale_);
      
    };

    CSimpleIni ini;

    ini.SetUnicode();
    read_settings(ini, path_to_ini_default);
    read_settings(ini, path_to_ini);

  }
};
