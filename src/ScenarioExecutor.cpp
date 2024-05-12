#include "ScenarioExecutor.hpp"

namespace details {
template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};
} // namespace details

namespace sw {
void ScenarioExecutor::execute(const io::CreateMap &command) {}
void ScenarioExecutor::execute(const io::March &command) {}
void ScenarioExecutor::execute(const io::SpawnArcher &command) {}
void ScenarioExecutor::execute(const io::SpawnWarrior &command) {}
void ScenarioExecutor::execute(const io::Wait &command) {}
void ScenarioExecutor::execute(const Command &command) {

  std::visit(details::overloaded{
                 [](const io::CreateMap &command) { execute(command); },
                 [](const io::March &command) { execute(command); },
                 [](const io::SpawnArcher &command) { execute(command); },
                 [](const io::SpawnWarrior &command) { execute(command); },
                 [](const io::Wait &command) { execute(command); }},
             command);
}

ScenarioExecutor::ScenarioExecutor(Scenario &&scenario, EventLog &&eventLog)
    : d_eventLog(std::move(eventLog)), d_scenario(std::move(scenario)) {}

void ScenarioExecutor::run() {
  for (const auto &command : d_scenario) {
    execute(command);
  }
}

} // namespace sw
