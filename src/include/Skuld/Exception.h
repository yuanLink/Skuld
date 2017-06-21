#pragma once
#include "Export.h"
#include "String.h"

namespace Skuld
{
	class SKULD_EXPORT Exception
	{
	protected:
		String _msg;
	public:
		Exception(const String& msg);
		virtual String Message();
		virtual ~Exception();
	};
}