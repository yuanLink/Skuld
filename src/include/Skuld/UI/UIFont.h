#pragma once

#include "../IDisposable.h"

namespace Skuld
{
	namespace UI
	{
		class UIContext;

		class UIFont : public IDisposable
		{
		protected:
			virtual ~UIFont(){}
		public:
			virtual UIContext* GetContext() const = 0;
		};
	}
}