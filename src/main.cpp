#include <Simulation/Simulation.hpp>
#include <types.hpp>

#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Commands/SpawnArcher.hpp>
#include <IO/Commands/SpawnWarrior.hpp>
#include <IO/Commands/Wait.hpp>
#include <IO/EventLogs/MapCreated.hpp>
#include <IO/EventLogs/MarchEnded.hpp>
#include <IO/EventLogs/MarchStarted.hpp>
#include <IO/EventLogs/UnitAttacked.hpp>
#include <IO/EventLogs/UnitDied.hpp>
#include <IO/EventLogs/UnitMoved.hpp>
#include <IO/EventLogs/UnitSpawned.hpp>
#include <IO/System/CommandParser.hpp>
#include <IO/System/EventLog.hpp>
#include <IO/System/PrintDebug.hpp>

#include <fstream>
#include <iostream>

using namespace sw;

namespace details
{

template <typename CommandT>
auto generateCommandHandler(Scenario& scenario)
{
	return [&scenario](CommandT command) {
		printDebug(std::cout, command);
		scenario.emplace_back(std::move(command));
	};
}

auto parseScenario(std::ifstream& file) -> Scenario
{
	Scenario scenario;

	io::CommandParser parser;
	parser.add<io::CreateMap>(generateCommandHandler<io::CreateMap>(scenario))
		.add<io::SpawnWarrior>(generateCommandHandler<io::SpawnWarrior>(scenario))
		.add<io::SpawnArcher>(generateCommandHandler<io::SpawnArcher>(scenario))
		.add<io::March>(generateCommandHandler<io::March>(scenario))
		.add<io::Wait>(generateCommandHandler<io::Wait>(scenario));

	parser.parse(file);

	return scenario;
}

template <typename Event>
auto generateEventHandler()
{
	return [](auto& event) { printDebug(std::cout, event); };
}

auto makeEventLog() -> EventLog
{
	EventLog eventLog;

	eventLog.listen<io::MapCreated>(generateEventHandler<io::MapCreated>());
	eventLog.listen<io::UnitSpawned>(generateEventHandler<io::UnitSpawned>());
	eventLog.listen<io::MarchStarted>(generateEventHandler<io::MarchStarted>());
	eventLog.listen<io::UnitMoved>(generateEventHandler<io::UnitMoved>());
	eventLog.listen<io::MarchEnded>(generateEventHandler<io::MarchEnded>());
	eventLog.listen<io::UnitAttacked>(generateEventHandler<io::UnitAttacked>());
	eventLog.listen<io::UnitDied>(generateEventHandler<io::UnitDied>());

	return eventLog;
}
} // namespace details

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		throw std::runtime_error("Error: No file specified in command line argument");
	}

	const auto pathToCommandsFile = std::string_view(argv[1]);
	std::ifstream file(pathToCommandsFile);
	if(!file)
	{
		throw std::runtime_error("Error: File not found - " + std::string(pathToCommandsFile));
	}

	auto scenario = details::parseScenario(file);
	auto eventLog = details::makeEventLog();
	Simulation simulation(std::move(scenario), std::move(eventLog));
	try
	{
		simulation.run();
	}
	catch(std::runtime_error& e)
	{
		std::cout << "Simulation fialed with execption: " << e.what();
	}

	return 0;
}