#pragma once

#include "Render3DFactory.h"

namespace Skuld
{
	namespace Render3D
	{
		enum ShaderInputLayoutAttriType
		{
			ShaderInputLayoutAttriType_int,
			ShaderInputLayoutAttriType_uint,
			ShaderInputLayoutAttriType_float,
			ShaderInputLayoutAttriType_short,
			ShaderInputLayoutAttriType_ushort,
			ShaderInputLayoutAttriType_byte,
			ShaderInputLayoutAttriType_sbyte
		};

		class ShaderInputLayoutAttri
		{
		public:
			String mName;
			uint32_t mSlot;
			ShaderInputLayoutAttriType mType;
			uint32_t mSize;
			ShaderInputLayoutAttri(const String& mName, uint32_t mSlot, ShaderInputLayoutAttriType mType, uint32_t mSize) :
				mName(mName), mType(mType), mSize(mSize), mSlot(mSlot) {}
			ShaderInputLayoutAttri() {}
		};

		class InputLayout : public IDisposable
		{
		protected:
			virtual ~InputLayout() {}
		public:
			virtual const Render3DFactory* GetFactory() const = 0;
		};
	}
}