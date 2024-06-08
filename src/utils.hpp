#pragma once

#include <Units/Units.hpp>
#include <cmath>
#include <types.hpp>

namespace sw
{
namespace utils
{
[[nodiscard]] inline double calculateDistance(Position a, Position b) noexcept
{
	return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

[[nodiscard]] inline Unit* getUnitPtr(UnitsVariant& unitVariant) noexcept
{
	return std::visit([](auto& unit) -> Unit* { return &unit; }, unitVariant);
}

} // namespace utils
} // namespace sw