#pragma once

#include <iostream>

namespace sw
{
	class CommandVisitor {
	private:
		std::istream& _stream;

	public:
		CommandVisitor(std::istream& stream) :
			_stream(stream)
		{
		}

		template <class TField>
		void visit(const char*, TField& field)
		{
			_stream >> field;
		}
	};
}