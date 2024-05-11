#pragma once

#include <iostream>

namespace sw
{
	class EventLogVisitor {
	private:
		std::ostream& _stream;

	public:
		EventLogVisitor(std::ostream& stream) :
			_stream(stream)
		{
		}

		template <class TField>
		void visit(const char* name, TField& field)
		{
			_stream << name << "=" << field << " ";
		}
	};
}