#include <Simulation/CommandsValidator.hpp>
#include <Simulation/Map.hpp>
#include <Simulation/Simulation.hpp>

#include <format>
#include <string>

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
	return std::format("width = {}, height = {}", map.width(), map.height());
}

} // namespace details

auto CommandsValidator::validateSpawnCommand(std::string_view command,
											 UnitId unitId,
											 Position position,
											 const Simulation& simulation) noexcept
	-> ErrorMessageOpt
{
	if(unitId == 0)
	{
		return {std::format(
			"Error: {} command failed. Incorrect unit id: {})", command.data(), unitId)};
	}

	if(simulation.d_units.contains(unitId))
	{
		return {std::format(
			"Error: {}, command failed. Duplicated unit id: {}", command.data(), unitId)};
	}

	if(!details::isPositionOnMap(position, *simulation.d_map))
	{
		return {std::format(
			"Error: {} command failed. Destination coords {}  are out of the map's boundaries: {}",
			command.data(),
			position.asString(),
			details::mapInfo(*simulation.d_map))};
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
	if(command.height <= 0 || command.width <= 0)
	{
		return {std::format("Error: CREATE MAP command failed. Invalid map size: {}x{}",
							command.width,
							command.height)};
	}
	return std::nullopt;
}

auto CommandsValidator::validate(const io::March& command,
								 const Simulation& simulation) noexcept -> ErrorMessageOpt
{
	assert(simulation.d_map);

	if(!simulation.d_units.contains(command.unitId))
	{
		return {std::format("Error: MARCH command failed. Unknow unit id: ", command.unitId)};
	}

	const Position destination = {command.targetX, command.targetY};
	if(!details::isPositionOnMap(destination, *simulation.d_map))
	{
		return {std::format(
			"Error: MARCH command failed. Destination coords {} are out of the map's boundaries: ",
			destination.asString(),
			details::mapInfo(*simulation.d_map))};
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