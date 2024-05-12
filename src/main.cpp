#include "Scenario.hpp"
#include "types.hpp"

#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Commands/SpawnArcher.hpp>
#include <IO/Commands/SpawnWarrior.hpp>
#include <IO/Commands/Wait.hpp>
#include <IO/EventLogs/MapCreated.hpp>
#include <IO/EventLogs/MarchEnded.hpp>
#include <IO/EventLogs/MarchStarted.hpp>
#include <IO/EventLogs/UnitAttacked.hpp>
#include <IO/EventLogs/UnitDied.hpp>
#include <IO/EventLogs/UnitMoved.hpp>
#include <IO/EventLogs/UnitSpawned.hpp>
#include <IO/System/CommandParser.hpp>
#include <IO/System/EventLog.hpp>
#include <IO/System/PrintDebug.hpp>
#include <fstream>
#include <iostream>

using namespace sw;

namespace details {

template <typename CommandT> auto generateCommandHandler(Scenario &scenario) {
  return [&scenario](CommandT command) {
    printDebug(std::cout, command);
    scenario.emplace_back(std::move(command));
  };
}

auto parseScenario(std::ifstream &file) -> Scenario {
  Scenario scenario;

  io::CommandParser parser;
  parser.add<io::CreateMap>(generateCommandHandler<io::CreateMap>(scenario))
      .add<io::SpawnWarrior>(generateCommandHandler<io::SpawnWarrior>(scenario))
      .add<io::SpawnArcher>(generateCommandHandler<io::SpawnArcher>(scenario))
      .add<io::March>(generateCommandHandler<io::March>(scenario))
      .add<io::Wait>(generateCommandHandler<io::Wait>(scenario));

  parser.parse(file);

  return scenario;
}

template <typename Event> auto generateEventHandler() {
  return [](auto &event) { printDebug(std::cout, event); };
}

auto makeEventLog() -> EventLog {
  EventLog eventLog;

  eventLog.listen<io::MapCreated>(generateEventHandler<io::MapCreated>());
  eventLog.listen<io::UnitSpawned>(generateEventHandler<io::UnitSpawned>());
  eventLog.listen<io::MarchStarted>(generateEventHandler<io::MarchStarted>());
  eventLog.listen<io::UnitMoved>(generateEventHandler<io::UnitMoved>());
  eventLog.listen<io::MarchEnded>(generateEventHandler<io::MarchEnded>());
  eventLog.listen<io::UnitAttacked>(generateEventHandler<io::UnitAttacked>());
  eventLog.listen<io::UnitDied>(generateEventHandler<io::UnitDied>());

  return eventLog;
}
} // namespace details

int main(int argc, char **argv) {
  if (argc != 2) {
    throw std::runtime_error(
        "Error: No file specified in command line argument");
  }

  std::ifstream file(argv[1]);
  if (!file) {
    throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
  }

  auto scenario = details::parseScenario(file);
  auto eventLog = details::makeEventLog();

  eventLog.log(io::MapCreated{10, 10});
  eventLog.log(io::UnitSpawned{1, "Archer", 5, 3});
  eventLog.log(io::UnitSpawned{2, "Warrior", 5, 3});
  eventLog.log(io::MarchStarted{1, 5, 3, 7, 9});
  eventLog.log(io::UnitMoved{1, 6, 4});
  eventLog.log(io::UnitAttacked{1, 2, 5, 0});
  eventLog.log(io::MarchEnded{1, 7, 9});
  eventLog.log(io::UnitDied{1});

  return 0;
}