#pragma once
#include <cstdint>
#include <optional>
#include <types.hpp>

namespace sw
{
class Unit
{
  private:
	std::optional<Position> d_marchDestination;
	Position d_position;
	UnitId d_unitId;
	uint32_t d_hp;

  protected:
	Unit(UnitId unitId, uint32_t hp, Position position)
		: d_marchDestination(std::nullopt)
		, d_position(position)
		, d_unitId(unitId)
		, d_hp(hp)
	{ }

  public:
	auto damage(uint32_t value) noexcept -> uint32_t
	{
		d_hp = (value >= d_hp) ? 0 : d_hp - value;
		return hp();
	}

	auto march(Position pos) noexcept -> Position
	{
		d_marchDestination = pos;
		return position();
	}

	void move(Position pos) noexcept
	{
		d_position = pos;
	}

	[[nodiscard]] auto position() const noexcept -> Position
	{
		return d_position;
	}

	[[nodiscard]] auto hp() const noexcept -> uint32_t
	{
		return d_hp;
	}

	[[nodiscard]] auto id() const noexcept -> UnitId
	{
		return d_unitId;
	}

	[[nodiscard]] auto isAlive() const noexcept -> bool
	{
		return hp() > 0;
	}

	[[nodiscard]] auto isOnMarch() const noexcept -> bool
	{
		return d_marchDestination.has_value() && d_marchDestination != position();
	}

	[[nodiscard]] auto marchDestination() const noexcept -> std::optional<Position>
	{
		return d_marchDestination;
	}
};
} // namespace sw