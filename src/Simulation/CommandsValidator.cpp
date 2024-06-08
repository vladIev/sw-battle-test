#include <Simulation/CommandsValidator.hpp>
#include <Simulation/Map.hpp>
#include <Simulation/Simulation.hpp>

#include <cassert>

namespace sw
{
namespace details
{
[[nodiscard]] inline bool isPositionOnMap(Position pos, const Map& map)
{
	return pos.x < map.width() && pos.y < map.height();
}

[[nodiscard]] inline std::string mapInfo(const Map& map)
{
	return "width = " + std::to_string(map.width()) + ", height = " + std::to_string(map.height());
}

} // namespace details

auto CommandsValidator::validateSpawnCommand(std::string_view command,
											 UnitId unitId,
											 Position position,
											 const Simulation& simulation) noexcept
	-> ErrorMessageOpt
{
	if(simulation.d_units.contains(unitId))
	{
		return {std::string("Error: ") + command.data() +
				"command failed. Duplicated unit id: " + std::to_string(unitId)};
	}

	if(!details::isPositionOnMap(position, *simulation.d_map))
	{
		return {std::string("Error: ") + command.data() + " command failed. Destination coords " +
				position.asString() +
				" are out of the map's boundaries: " + details::mapInfo(*simulation.d_map)};
	}
	return std::nullopt;
}

auto CommandsValidator::validate(const io::CreateMap& command,
								 const Simulation& simulation) noexcept -> ErrorMessageOpt
{
	if(simulation.d_map)
	{
		return {"Error: CREATE MAP command failed. Map already exists"};
	}
	return std::nullopt;
}

auto CommandsValidator::validate(const io::March& command,
								 const Simulation& simulation) noexcept -> ErrorMessageOpt
{
	assert(simulation.d_map);

	if(!simulation.d_units.contains(command.unitId))
	{
		return {"Error: MARCH command failed. Unknow unit id: " + std::to_string(command.unitId)};
	}

	const Position destination = {command.targetX, command.targetY};
	if(!details::isPositionOnMap(destination, *simulation.d_map))
	{
		return {"Error: MARCH command failed. Destination coords " + destination.asString() +
				" are out of the map's boundaries: " + details::mapInfo(*simulation.d_map)};
	}

	return std::nullopt;
}

auto CommandsValidator::validate(const io::SpawnArcher& command,
								 const Simulation& simulation) noexcept -> ErrorMessageOpt
{
	return validateSpawnCommand("SPAWN ARCHER", command.unitId, {command.x, command.y}, simulation);
}

auto CommandsValidator::validate(const io::SpawnWarrior& command,
								 const Simulation& simulation) noexcept -> ErrorMessageOpt
{
	return validateSpawnCommand(
		"SPAWN WARRIOR", command.unitId, {command.x, command.y}, simulation);
}

auto CommandsValidator::validate(const io::Wait& command,
								 const Simulation& simulation) noexcept -> ErrorMessageOpt
{
	return std::nullopt;
}
} // namespace sw