#pragma once
#include <types.hpp>

#include <cassert>

#include <unordered_map>

namespace sw
{
class Map
{
	using Cell = UnitId;
	using Field = std::vector<std::vector<Cell>>;
	using UnitsPosition = std::unordered_map<UnitId, Position>;

	Field d_field;
	UnitsPosition d_positions;
	const uint32_t d_width;
	const uint32_t d_height;

	[[nodiscard]] std::vector<UnitId>
	getUnitsInRange(Position center, uint32_t to, uint32_t from) const;

  public:
	Map(uint32_t width, uint32_t height)
		: d_width(width)
		, d_height(height)
	{
		d_field.resize(d_height);
		for(auto& row : d_field)
			row.resize(d_width);
	}

	void spawnUnit(UnitId id, Position position)
	{
		assert(!d_positions.contains(id));

		d_field[position.y][position.x] = id;
		d_positions[id] = position;
	}

	void removeUnit(UnitId id)
	{
		assert(d_positions.contains(id));

		const auto position = d_positions[id];
		d_field[position.y][position.x] = 0;
		d_positions.erase(id);
	}

	void moveUnit(UnitId id, Position position)
	{
		assert(d_positions.contains(id));

		auto oldPosition = d_positions[id];
		if(oldPosition == position) return;

		d_positions[id] = position;
		d_field[oldPosition.y][oldPosition.x] = 0;
		d_field[position.y][position.x] = id;
	}

	[[nodiscard]] auto width() const noexcept
	{
		return d_width;
	}

	[[nodiscard]] auto height() const noexcept
	{
		return d_height;
	}

	[[nodiscard]] auto
	getUnitsInRange(UnitId id, uint32_t range, uint32_t from = 0) const -> std::vector<UnitId>;
	[[nodiscard]] auto unitPosition(UnitId id) const -> Position;
};
} // namespace sw
