#include <Simulation/Map.hpp>
#include <cstdint>
#include <types.hpp>

#include <algorithm>
#include <set>
#include <vector>

namespace sw
{

auto Map::getUnitsInRange(Position center, uint32_t to, uint32_t from) const -> std::vector<UnitId>
{
	std::set<int64_t> ys;
	std::set<int64_t> xs;

	for(int64_t i = center.y - to; i <= center.y + to; i++)
	{
		ys.insert(std::clamp(i, 0ll, static_cast<int64_t>(d_height - 1)));
	}

	for(int64_t i = center.x - to; i <= center.x + to; i++)
	{

		xs.insert(std::clamp(i, 0ll, static_cast<int64_t>(d_width - 1)));
	}

	std::vector<UnitId> neighbourUnits;
	for(const auto y : ys)
	{
		for(const auto x : xs)
		{
			if(std::abs(static_cast<int64_t>(x - center.x)) >= from &&
			   std::abs(static_cast<int64_t>(y - center.y)) >= from)
			{

				const auto id = d_field[y][x];
				if(id) neighbourUnits.push_back(id);
			}
		}
	}

	return neighbourUnits;
}

auto Map::getUnitsInRange(UnitId id, uint32_t range, uint32_t from) const -> std::vector<UnitId>
{
	assert(d_positions.contains(id));

	const auto position = d_positions.at(id);
	auto units = getUnitsInRange(position, range, from);
	std::erase(units, id);
	return units;
}

auto Map::unitPosition(UnitId id) const -> Position
{
	return d_positions.at(id);
}

} // namespace sw