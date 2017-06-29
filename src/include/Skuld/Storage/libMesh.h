#pragma once

#include "../IDisposable.h"
#include "../MathStruct.h"

namespace Skuld
{
	namespace Storage
	{
		class MeshFile : public IDisposable
		{
		protected:
			virtual ~MeshFile() {}
		public:
			virtual uint32_t GetObjectCount() const = 0;
		};
	}
}