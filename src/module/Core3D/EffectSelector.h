#pragma once
#include <vector>
#include <array>
#include <Skuld/Render3D/ShaderObject.h>
#include <Skuld/Render3D/Render3DContext.h>
#include <Skuld/Render3D/InputLayout.h>
#include <Skuld/Ptr.hpp>
#include <Skuld/Core/Effect.h>

namespace Skuld
{
	namespace Core
	{
		class EffectSelector
		{
		private:
			std::vector<Ptr<Render3D::ShaderObject> > mShaders;

			std::vector<std::array<size_t, 5> > mEffects;
			std::vector<String> mEffectNames;
			std::vector<Ptr<Render3D::InputLayout> > mEffectLayouts;

			size_t mCurrentEffect = mNotSetEffect;
			Ptr<Render3D::Render3DContext> mContext;

			static const size_t mNotSetEffect = ~(size_t)0;
			static const size_t mNoShader = ~(size_t)0;
		public:

			EffectSelector(Ptr<Render3D::Render3DContext> mContext) : mContext(mContext) {}

			void SetEffect(const Effect* mEffectFile);
		};
	}
}