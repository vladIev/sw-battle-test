#pragma once
#include <IO/EventLogs/UnitSpawned.hpp>
#include <IO/System/EventLog.hpp>
#include <Simulation/Map.hpp>
#include <Units/Units.hpp>
#include <types.hpp>
#include <utils.hpp>

#include <memory>
#include <unordered_map>

namespace sw
{
class Unit;
class UnitsFactory;
class TickExecutor;

class Simulation
{
  private:
	Scenario d_scenario;
	EventLog d_eventLog;
	std::unique_ptr<Map> d_map;
	std::unordered_map<UnitId, UnitsVariant> d_units;

	void execute(const io::CreateMap& command);
	void execute(const io::March& command);
	void execute(const io::SpawnArcher& command);
	void execute(const io::SpawnWarrior& command);
	void execute(const io::Wait& command);
	void execute(const Command& command);

	template <typename T>
	auto addUnit(T&& unit, std::string_view unitType) -> io::UnitSpawned
	{
		const UnitId id = unit.id();
		const Position pos = unit.position();
		d_map->spawnUnit(id, pos);
		d_units.emplace(id, std::forward<T>(unit));
		return io::UnitSpawned{id, std::string(unitType), pos.x, pos.y};
	}

	auto getUnitPtr(UnitId unitId) -> Unit*;

	friend struct CommandsValidator;

  public:
	Simulation(Scenario&& scenario, EventLog&& eventLog);
	~Simulation();
	void run();
	void finish();
};
} // namespace sw
