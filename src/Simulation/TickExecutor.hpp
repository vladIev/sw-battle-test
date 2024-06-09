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

	std::optional<io::UnitAttacked>
	executeMeleeAttack(MeleeAttack& unit, const Map& map, UnitId attackerId);

	std::optional<io::UnitAttacked>
	executeRangeAttack(RangeAttack& unit, const Map& map, UnitId attackerId);

	std::optional<io::UnitMoved> executeMove(Unit& unit, const Map& map);
	void action(UnitsVariant& unit, const Map& map);

  public:
	TickExecutor(EventLog* eventLog, std::unordered_map<UnitId, UnitsVariant>* units);
	void run(Map& map, uint32_t numOfTicks);
	void runUntilFinish(Map& map);
};

} // namespace sw