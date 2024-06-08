#pragma once
#include <IO/Commands/SpawnArcher.hpp>
#include <IO/Commands/SpawnWarrior.hpp>
#include <Units/Units.hpp>

namespace sw
{
struct UnitsFactory
{
	[[nodiscard]] static auto spawnArcher(const io::SpawnArcher& command) -> Archer
	{
		return Archer(command.unitId,
					  command.hp,
					  {command.x, command.y},
					  command.range,
					  command.strength,
					  command.agility);
	}

	[[nodiscard]] static auto spawnWarrior(const io::SpawnWarrior& command) -> Warrior
	{
		return Warrior(command.unitId, command.hp, {command.x, command.y}, command.strength);
	}
};
} // namespace sw