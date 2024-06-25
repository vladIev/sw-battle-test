#pragma once

#include <functional>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>

namespace sw
{
class EventLog
{
  private:
	using Tick = uint64_t;
	std::unordered_map<std::type_index, std::function<void(void*, Tick)>> _handlers;
	Tick _tick{0};

  public:
	template <class TEvent>
	void listen(std::function<void(TEvent&, Tick)> handler)
	{
		auto [it, inserted] =
			_handlers.emplace(std::type_index(typeid(TEvent)),
							  [handler = std::move(handler)](void* event, Tick tick) {
								  handler(*static_cast<TEvent*>(event), tick);
							  });
		if(!inserted)
		{
			throw std::runtime_error("Event already exists: " + std::string(TEvent::Name));
		}
	}

	template <class TEvent>
	void log(TEvent&& event)
	{
		auto handler = _handlers.find(std::type_index(typeid(TEvent)));
		if(handler == _handlers.end())
			throw std::runtime_error("Unknown event: " + std::string(TEvent::Name));

		handler->second(&event, _tick);
	}

	void incrementTick() noexcept
	{
		_tick++;
	}
};
} // namespace sw