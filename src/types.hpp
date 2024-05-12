#pragma once

#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Commands/SpawnArcher.hpp>
#include <IO/Commands/SpawnWarrior.hpp>
#include <IO/Commands/Wait.hpp>

#include <variant>
#include <vector>

namespace sw {
using Command = std::variant<io::CreateMap, io::March, io::SpawnArcher,
                             io::SpawnWarrior, io::Wait>;
using Scenario = std::vector<Command>;
} // namespace sw
