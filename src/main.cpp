#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

#include "MenuHandler.hpp"
#include "Settings.hpp"
#include "Widget.hpp"

auto init_logger() -> void {
  // ReSharper disable once CppLocalVariableMayBeConst
  auto path = logger::log_directory();
  if (!path) return;

  const auto plugin = SKSE::PluginDeclaration::GetSingleton();
  *path /= fmt::format(FMT_STRING("{}.log"), plugin->GetName());

  std::shared_ptr<spdlog::sinks::sink> sink;
  if (WinAPI::IsDebuggerPresent()) {
    sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
  } else {
    sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
  }

  auto log = std::make_shared<spdlog::logger>("global log"s, sink);
  log->set_level(spdlog::level::info);
  log->flush_on(spdlog::level::info);

  set_default_logger(std::move(log));
  spdlog::set_pattern("%s(%#): [%^%l%$] %v"s);
}

auto initialize_messaging() -> void {
  // ReSharper disable once CppParameterMayBeConstPtrOrRef
  if (!SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message*
                                                              message) {
        switch (message->type) {
        // Skyrim lifecycle events.
        case SKSE::MessagingInterface::kPostLoad: // Called after all plugins have finished running
        // SKSEPlugin_Load. It is now safe to do
        // multithreaded operations, or operations against
        // other plugins.
        case SKSE::MessagingInterface::kPostPostLoad: // Called after all kPostLoad message handlers
        // have run.
        case SKSE::MessagingInterface::kInputLoaded: // Called when all game data has been found.
          break;
        case SKSE::MessagingInterface::kDataLoaded: // All ESM/ESL/ESP plugins have loaded, main
                                                    // menu is now active.
        {

          // It is now safe to access form data.
          Settings::get_singleton().load();
          ReflyemWidget::register_();
          MenuHandler::register_();
          ReflyemWidget::show();
          logger::info("Get data loaded message"sv);
          break;
        }
        // Skyrim game events.
        case SKSE::MessagingInterface::kNewGame: // Player starts a new game from main menu.
        {
          Settings::get_singleton().load();
          ReflyemWidget::show();
          break;
        }
        case SKSE::MessagingInterface::kPreLoadGame: // Player selected a game to load, but it
                                                     // hasn't loaded yet. Data will be the name of
                                                     // the loaded save.
          break;
        case SKSE::MessagingInterface::kPostLoadGame: // Player's selected save game has finished
        {
          // loading. Data will be a boolean indicating
          // whether the load was successful.
          Settings::get_singleton().load();
          ReflyemWidget::show();
          break;
        }
        case SKSE::MessagingInterface::kSaveGame: // The player has saved a game.
        // Data will be the save name.
        case SKSE::MessagingInterface::kDeleteGame: // The player deleted a saved game from within
          // the load menu.
          break;
        default: {
          break;
        }
        }
      })) {
    stl::report_and_fail("Unable to register message listener."sv);
  }
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse) {
  init_logger();

  const auto plugin = SKSE::PluginDeclaration::GetSingleton();
  logger::info("{} v{}"sv, plugin->GetName(), plugin->GetVersion());

  Init(a_skse);
  initialize_messaging();

  logger::info("{} loaded"sv, plugin->GetName());

  return true;
}
