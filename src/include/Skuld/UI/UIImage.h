#pragma once
#include "../IDisposable.h"

namespace Skuld
{
	namespace UI
	{
		class UIContext;
		class UIImage : public IDisposable
		{
		protected:
			~UIImage(){}
		public:
			virtual uint32_t GetWidth() const = 0;
			virtual uint32_t GetHeight() const = 0;
			virtual UIContext* GetContext() const = 0;
		};
	}
}