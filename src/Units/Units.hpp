#pragma once

#include <Units/Abilities.hpp>
#include <Units/Unit.hpp>

#include <variant>

namespace sw
{

class Warrior : public Unit, public MeleeAttack
{
  public:
	Warrior(UnitId unitId, uint32_t hp, Position position, uint32_t strength)
		: Unit(unitId, hp, position)
		, MeleeAttack(strength)
	{ }
};

class Archer : public Unit, public RangeAttack, public MeleeAttack
{
  public:
	Archer(UnitId unitId,
		   uint32_t hp,
		   Position position,
		   uint32_t strength,
		   uint32_t agility,
		   uint32_t range)
		: Unit(unitId, hp, position)
		, RangeAttack(agility, range)
		, MeleeAttack(strength)
	{ }
};

using UnitsVariant = std::variant<Warrior, Archer>;
} // namespace sw
