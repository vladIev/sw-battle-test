#pragma once

#include <IO/EventLogs/MarchEnded.hpp>
#include <IO/EventLogs/UnitAttacked.hpp>
#include <IO/EventLogs/UnitMoved.hpp>
#include <IO/System/EventLog.hpp>

#include <Units/Units.hpp>
#include <types.hpp>

namespace sw
{
class Map;
class MeleeAttack;
class RangeAttack;

class TickExecutor
{
	EventLog* d_eventLog;
	std::unordered_map<UnitId, UnitsVariant>* d_units;
	uint64_t d_ticksCntr;

	auto executeMeleeAttack(MeleeAttack& unit,
							const Map& map,
							UnitId attackerId) -> std::optional<io::UnitAttacked>;

	auto executeRangeAttack(RangeAttack& unit,
							const Map& map,
							UnitId attackerId) -> std::optional<io::UnitAttacked>;

	auto executeMove(Unit& unit, const Map& map) -> std::optional<io::UnitMoved>;

	void executeUnitAction(UnitsVariant& unit, const Map& map);

  public:
	TickExecutor(EventLog* eventLog, std::unordered_map<UnitId, UnitsVariant>* units);
	void run(Map& map, uint32_t numOfTicks);
	void runWhileHaveMarchingUnits(Map& map);
};

} // namespace sw