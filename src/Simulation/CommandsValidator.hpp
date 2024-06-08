#pragma once
#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Commands/SpawnArcher.hpp>
#include <IO/Commands/SpawnWarrior.hpp>
#include <IO/Commands/Wait.hpp>
#include <types.hpp>

#include <optional>
#include <string>

namespace sw
{
class Simulation;
class CommandsValidator
{
	using ErrorMessageOpt = std::optional<std::string>;
	[[nodiscard]] static auto
	validateSpawnCommand(std::string_view command,
						 UnitId unitId,
						 Position position,
						 const Simulation& simulation) noexcept -> ErrorMessageOpt;

  public:
	[[nodiscard]] static auto validate(const io::CreateMap& command,
									   const Simulation& simulation) noexcept -> ErrorMessageOpt;

	[[nodiscard]] static auto validate(const io::March& command,
									   const Simulation& simulation) noexcept -> ErrorMessageOpt;

	[[nodiscard]] static auto validate(const io::SpawnArcher& command,
									   const Simulation& simulation) noexcept -> ErrorMessageOpt;

	[[nodiscard]] static auto validate(const io::SpawnWarrior& command,
									   const Simulation& simulation) noexcept -> ErrorMessageOpt;

	[[nodiscard]] static auto validate(const io::Wait& command,
									   const Simulation& simulation) noexcept -> ErrorMessageOpt;
};
} // namespace sw