#pragma once

#include <iostream>

namespace sw
{
	class PrintDebugVisitor {
	private:
		std::ostream& _stream;

	public:
		PrintDebugVisitor(std::ostream& stream)
			:
			_stream(stream)
		{
		}

		template <typename T>
		void visit(const char* name, const T& value)
		{
			_stream << name << "=" << value << ' ';
		}
	};


}