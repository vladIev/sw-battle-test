#include <Simulation/TickExecutor.hpp>

#include <IO/EventLogs/UnitDied.hpp>
#include <IO/System/EventLog.hpp>
#include <Simulation/Map.hpp>
#include <Units/Abilities.hpp>
#include <limits>
#include <utils.hpp>

#include <ranges>

namespace sw
{
namespace details
{

auto chooseNextPosition(Position current, Position destination) -> Position
{
	const std::vector<Position> directions = {
		{current.x + 1, current.y + 1},
		{current.x + 1, current.y - 1},
		{current.x - 1, current.y + 1},
		{current.x - 1, current.y - 1},
		{current.x + 1, current.y},
		{current.x - 1, current.y},
		{current.x, current.y - 1},
		{current.x, current.y + 1},
	};

	Position nextPosition;
	double minDistance = std::numeric_limits<double>::max();

	for(const auto& pos : directions)
	{
		double distance = utils::calculateDistance(pos, destination);
		if(distance < minDistance)
		{
			minDistance = distance;
			nextPosition = pos;
		}
	}

	return nextPosition;
}

auto splitOnDeadAndAlive(std::ranges::view auto unitsView)
{
	const auto predicate = [](const auto& unit) -> bool {
		return std::visit([](const auto& unit) { return unit.isAlive(); }, unit.second);
	};

	auto dead =
		unitsView | std::views::filter([&predicate](const auto& val) { return !predicate(val); });

	auto alive = unitsView | std::views::filter(predicate);

	return std::make_pair(dead, alive);
}

auto marchingUnits(std::ranges::view auto unitsView)
{
	return unitsView | std::views::filter([](const auto& val) -> bool {
			   return std::visit([](const auto& unit) -> bool { return unit.isOnMarch(); },
								 val.second);
		   });
}

void handleDeadUnits(std::ranges::view auto deadUnits,
					 std::unordered_map<UnitId, UnitsVariant>& units,
					 Map& map,
					 EventLog& eventLog)
{
	for(const auto unit : deadUnits)
	{
		std::visit(
			[&](const auto& unit) {
				units.erase(unit.id());
				map.removeUnit(unit.id());
				eventLog.log(io::UnitDied{unit.id()});
			},
			unit);
	}
}

void handleAliveUnits(std::ranges::view auto aliveUnits, Map& map)
{
	for(const auto& unit : aliveUnits)
	{
		std::visit([&map](const auto& unit) { map.moveUnit(unit.id(), unit.position()); }, unit);
	}
}

auto unitsById(std::unordered_map<UnitId, UnitsVariant>* units,
			   std::span<const UnitId> targetsIds) -> std::vector<Unit*>
{
	std::vector<Unit*> targets;
	targets.reserve(targetsIds.size());
	for(const auto id : targetsIds)
	{
		auto target = std::visit([](auto& val) -> Unit* { return &val; }, units->at(id));
		targets.push_back(target);
	}
	return targets;
}

} // namespace details

auto TickExecutor::executeMeleeAttack(MeleeAttack& unit,
									  const Map& map,
									  UnitId attackerId) -> std::optional<io::UnitAttacked>
{
	const auto targetsId = map.getUnitsInRange(attackerId, 1);
	if(targetsId.empty()) return std::nullopt;

	std::vector<Unit*> targets = details::unitsById(d_units, targetsId);
	auto target = unit.selectTarget(targets);
	if(!target) return std::nullopt;

	const auto dmg = unit.meleeAttack(target);

	return io::UnitAttacked{attackerId, target->id(), dmg, target->hp()};
}

auto TickExecutor::executeRangeAttack(RangeAttack& unit,
									  const Map& map,
									  UnitId attackerId) -> std::optional<io::UnitAttacked>
{
	const auto targetsId = map.getUnitsInRange(attackerId, unit.rangeAttackDistance(), 2);
	if(targetsId.empty()) return std::nullopt;

	std::vector<Unit*> targets = details::unitsById(d_units, targetsId);

	auto target = unit.selectTarget(targets, map.unitPosition(attackerId));
	if(!target) return std::nullopt;

	const auto dmg = unit.rangeAttack(target);

	return io::UnitAttacked{attackerId, target->id(), dmg, target->hp()};
}

auto TickExecutor::executeMove(Unit& unit, const Map& map) -> std::optional<io::UnitMoved>
{
	const auto destination = unit.marchDestination();
	if(!destination || unit.position() == destination) return std::nullopt;

	const auto nextPosition = details::chooseNextPosition(unit.position(), *destination);
	unit.move(nextPosition);

	return io::UnitMoved{unit.id(), nextPosition.x, nextPosition.y};
}

void TickExecutor::executeUnitAction(UnitsVariant& unit, const Map& map)
{
	std::visit(
		[&map, this](auto& unit) {
			using T = std::decay_t<decltype(unit)>;
			if constexpr(std::derived_from<T, RangeAttack>)
			{
				if(auto event = executeRangeAttack(unit, map, unit.id()); event)
				{
					d_eventLog->log(std::move(*event));
					return;
				}
			}

			if constexpr(std::derived_from<T, MeleeAttack>)
			{
				if(auto event = executeMeleeAttack(unit, map, unit.id()); event)
				{
					d_eventLog->log(std::move(*event));
					return;
				}
			}

			if(auto event = executeMove(unit, map); event)
			{
				d_eventLog->log(std::move(*event));
				if(!unit.isOnMarch())
				{
					d_eventLog->log(
						io::MarchEnded{unit.id(), unit.position().x, unit.position().y});
				}
				return;
			}
		},
		unit);
}

TickExecutor::TickExecutor(EventLog* eventLog, std::unordered_map<UnitId, UnitsVariant>* units)
	: d_eventLog(eventLog)
	, d_units(units)
	, d_ticksCntr(0)
{
	if(!d_eventLog || !d_units)
	{
		throw std::runtime_error("Failed to initialize TickExecutor");
	}
}

void TickExecutor::run(Map& map, uint32_t numOfTicks)
{
	for(auto i : std::views::iota(0u, numOfTicks))
	{
		if(d_units->empty()) break;
		d_eventLog->incrementTick();

		for(auto& [_, unit] : *d_units)
		{
			executeUnitAction(unit, map);
		}

		auto [dead, alive] = details::splitOnDeadAndAlive(*d_units | std::views::all);
		details::handleAliveUnits(alive | std::views::values, map);

		const bool haveDeads = !dead.empty();
		if(haveDeads)
		{
			details::handleDeadUnits(dead | std::views::values, *d_units, map, *d_eventLog);
		}
	}
}

void TickExecutor::runWhileHaveMarchingUnits(Map& map)
{
	while(1)
	{
		auto units = details::marchingUnits(*d_units | std::views::all);
		if(units.empty()) break;
		run(map, 10);
	}
}

} // namespace sw
