#pragma once

#include "details/PrintDebugVisitor.hpp"

namespace sw
{
	template <typename TCommand>
	void printDebug(std::ostream& stream, TCommand& data)
	{
		stream << data.Name << ' ';
		PrintDebugVisitor visitor(stream);
		data.visit(visitor);
		stream << "\n";
	}
}