#pragma once
#include <Units/Unit.hpp>

#include <span>

namespace sw
{

class MeleeAttack
{
	uint32_t d_strength;

  public:
	MeleeAttack(uint32_t strength)
		: d_strength(strength)
	{ }

	virtual uint32_t meleeAttack(Unit* target)
	{
		const auto dmg = d_strength;
		target->damage(dmg);
		return dmg;
	}

	[[nodiscard]] virtual Unit* selectTarget(std::span<Unit*> units) const;
};

class RangeAttack
{
	uint32_t d_agility;
	uint32_t d_range;

  public:
	RangeAttack(uint32_t agility, uint32_t range)
		: d_agility(agility)
		, d_range(range)
	{ }

	[[nodiscard]] uint32_t rangeAttackDistance() const noexcept
	{
		return d_range;
	}

	virtual uint32_t rangeAttack(Unit* target) noexcept
	{
		const auto dmg = d_agility;
		target->damage(dmg);
		return dmg;
	}

	[[nodiscard]] virtual Unit* selectTarget(std::span<Unit*> targets, Position selfPosition) const;
};
} // namespace sw
