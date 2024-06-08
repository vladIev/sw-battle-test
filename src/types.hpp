#pragma once

#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Commands/SpawnArcher.hpp>
#include <IO/Commands/SpawnWarrior.hpp>
#include <IO/Commands/Wait.hpp>

#include <sstream>
#include <variant>
#include <vector>

namespace sw
{
using Command = std::variant<io::CreateMap, io::March, io::SpawnArcher, io::SpawnWarrior, io::Wait>;
using Scenario = std::vector<Command>;
using UnitId = uint32_t;

struct Position
{
	uint32_t x;
	uint32_t y;

	auto operator=(const Position& other) -> Position&
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	[[nodiscard]] auto operator==(const Position& other) const
	{
		return x == other.x && y == other.y;
	}

	[[nodiscard]] auto operator!=(const Position& other) const
	{
		return !(*this == other);
	}

	[[nodiscard]] auto asString() const -> std::string
	{
		std::ostringstream ss;
		ss << "(" << std::to_string(x) << ", " << std::to_string(y) << ")";
		return ss.str();
	}
};
} // namespace sw
