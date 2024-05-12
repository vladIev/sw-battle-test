#pragma once
#include "types.hpp"

#include <IO/System/EventLog.hpp>

#include <memory>

namespace sw {
class Unit;
class UnitsFactory;
class Map;
class TickExecutor;

class ScenarioExecutor {
private:
  Scenario d_scenario;
  EventLog d_eventLog;
  std::shared_ptr<Map> d_map;
  std::vector<Unit> d_units;

  void execute(const io::CreateMap &command);
  void execute(const io::March &command);
  void execute(const io::SpawnArcher &command);
  void execute(const io::SpawnWarrior &command);
  void execute(const io::Wait &command);
  void execute(const Command &command);

public:
  ScenarioExecutor(Scenario &&scenario, EventLog &&eventLog);
  void run();
};
} // namespace sw
