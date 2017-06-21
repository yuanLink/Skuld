#include <Skuld/Exception.h>

namespace Skuld
{
	Exception::Exception(const String& msg)
	{
		_msg = msg;
	}

	String Exception::Message()
	{
		return this->_msg;
	}

	Exception::~Exception(){}
}