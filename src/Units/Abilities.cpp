#include <Units/Abilities.hpp>
#include <utils.hpp>

#include <vector>

namespace sw
{

namespace details
{
template <typename Pred>
inline std::vector<Unit*> filterByMinValue(std::span<Unit*> targets, const Pred& pred)
{
	if(targets.empty()) return {};
	if(targets.size() == 1) return {targets[0]};

	std::vector<Unit*> result;
	result.reserve(targets.size());

	auto minValue = pred(*targets[0]);
	for(auto unit : targets)
	{
		const auto value = pred(*unit);
		if(value < minValue)
		{
			result.clear();
			result.push_back(unit);
			minValue = value;
		}
		else if(value == minValue)
		{
			result.push_back(unit);
		}
	}
	return result;
}

inline std::vector<Unit*> getClosestUnits(std::span<Unit*> targets, Position selfPosition)
{
	return filterByMinValue(targets, [selfPosition](const Unit& unit) {
		return utils::calculateDistance(selfPosition, unit.position());
	});
}

inline std::vector<Unit*> getMostDamagedUnits(std::span<Unit*> targets)
{
	return filterByMinValue(targets, [](const Unit& unit) { return unit.hp(); });
}

inline std::vector<Unit*> getLowestId(std::span<Unit*> targets)
{
	return filterByMinValue(targets, [](const Unit& unit) { return unit.id(); });
}
} // namespace details

Unit* RangeAttack::selectTarget(std::span<Unit*> targets, Position selfPosition) const
{
	if(targets.empty()) return nullptr;
	auto closests = details::getClosestUnits(targets, selfPosition);
	if(closests.size() == 1) return closests[0];
	auto damaged = details::getMostDamagedUnits(closests);
	if(damaged.size() == 1) return damaged[0];
	return details::getLowestId(damaged)[0];
}

Unit* MeleeAttack::selectTarget(std::span<Unit*> targets) const
{
	if(targets.empty()) return nullptr;
	auto damaged = details::getMostDamagedUnits(targets);
	if(damaged.size() == 1) return damaged[0];
	return details::getLowestId(damaged)[0];
}
} // namespace sw