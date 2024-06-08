#include <Simulation/Map.hpp>
#include <types.hpp>

#include <algorithm>
#include <iterator>

namespace sw
{

auto Map::getUnitsInRange(Position center, uint32_t range) const -> std::vector<UnitId>
{
	std::set<uint32_t> ys;
	std::set<uint32_t> xs;

	for(int64_t i = center.y - range; i <= center.y + range; i++)
	{
		ys.insert(std::clamp(i, 0ll, static_cast<int64_t>(d_height - 1)));
	}

	for(int64_t i = center.x - range; i <= center.x + range; i++)
	{
		xs.insert(std::clamp(i, 0ll, static_cast<int64_t>(d_width - 1)));
	}

	std::vector<UnitId> neighbourUnits;
	for(const auto y : ys)
	{
		for(const auto x : xs)
		{
			std::ranges::copy(d_field[y][x], std::back_inserter(neighbourUnits));
		}
	}

	return neighbourUnits;
}

auto Map::getUnitsInRange(UnitId id, uint32_t range) const -> std::vector<UnitId>
{
	assert(d_positions.contains(id));

	const auto position = d_positions.at(id);
	auto units = getUnitsInRange(position, range);
	if(units.size() == 1)
	{
		units.clear();
		return units;
	}

	auto selfIt = std::ranges::find(units, id);
	if(selfIt != units.end())
	{
		*selfIt = *units.rbegin();
		units.pop_back();
	}
	return units;
}

auto Map::unitPosition(UnitId id) const -> Position
{
	return d_positions.at(id);
}

} // namespace sw