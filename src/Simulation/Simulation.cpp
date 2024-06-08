#include <Simulation/Simulation.hpp>

#include <IO/System/EventLog.hpp>
#include <Simulation/CommandsValidator.hpp>
#include <Simulation/TickExecutor.hpp>
#include <Units/UnitsFactory.hpp>

#include <IO/EventLogs/MapCreated.hpp>
#include <IO/EventLogs/MarchEnded.hpp>
#include <IO/EventLogs/MarchStarted.hpp>
#include <IO/EventLogs/UnitAttacked.hpp>
#include <IO/EventLogs/UnitDied.hpp>
#include <IO/EventLogs/UnitMoved.hpp>
#include <IO/EventLogs/UnitSpawned.hpp>

#include <cassert>

namespace sw
{
namespace details
{
template <class... Ts>
struct overloaded : Ts...
{
	using Ts::operator()...;
};

auto validateScenario(const Scenario& scenario) -> std::optional<std::string>
{
	if(scenario.size() == 0)
	{
		return {"Scenario is empty"};
	}

	if(!std::holds_alternative<io::CreateMap>(scenario[0]))
	{
		return {"Scenario should start with CREATE_MAP command"};
	}

	return std::nullopt;
}

} // namespace details

void Simulation::execute(const io::CreateMap& command)
{
	d_map = std::make_unique<Map>(command.width, command.height);
	d_eventLog.log(io::MapCreated{.width = command.width, .height = command.height});
}

void Simulation::execute(const io::SpawnArcher& command)
{
	auto unit = UnitsFactory::spawnArcher(command);
	auto event = addUnit(std::move(unit), "Archer");
	d_eventLog.log(std::move(event));
}

void Simulation::execute(const io::SpawnWarrior& command)
{
	auto unit = UnitsFactory::spawnWarrior(command);
	auto event = addUnit(std::move(unit), "Warrior");
	d_eventLog.log(std::move(event));
}

void Simulation::execute(const io::March& command)
{
	auto unit = getUnitPtr(command.unitId);
	const auto [currentX, currentY] = unit->march({command.targetX, command.targetY});
	d_eventLog.log(
		io::MarchStarted{unit->id(), currentX, currentY, command.targetX, command.targetY});
}

void Simulation::execute(const io::Wait& command)
{
	TickExecutor executor(&d_eventLog, &d_units);
	executor.run(*d_map, command.ticks);
}

void Simulation::execute(const Command& command)
{
	std::visit(
		[this](const auto& command) {
			const auto validationError = CommandsValidator::validate(command, *this);
			if(validationError)
			{
				throw std::runtime_error(*validationError);
			}
			execute(command);
		},
		command);
}

auto Simulation::getUnitPtr(UnitId unitId) -> Unit*
{
	auto unit = d_units.find(unitId);
	return utils::getUnitPtr(unit->second);
}

Simulation::Simulation(Scenario&& scenario, EventLog&& eventLog)
	: d_eventLog(std::move(eventLog))
	, d_scenario(std::move(scenario))
{ }

Simulation::~Simulation()
{
	finish();
}

void Simulation::run()
{
	const auto error = details::validateScenario(d_scenario);
	if(error)
	{
		throw std::runtime_error("Error: Scenario validation failed. " + *error);
	}

	for(const auto& command : d_scenario)
	{
		execute(command);
	}
}

void Simulation::finish()
{
	auto executor = TickExecutor(&d_eventLog, &d_units);
	executor.runUntilFinish(*d_map);
}

} // namespace sw
