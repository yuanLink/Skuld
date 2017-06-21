#pragma once
#include <map>
#include <Skuld/Core/Effect.h>
#include <Skuld/Exception.h>
#include <vector>
#include <memory>

namespace Skuld
{
	namespace Core
	{
		typedef std::vector<Render3D::ShaderInputLayoutAttri> ShaderInputLayout;

		struct EachEffect
		{
			String mName;
			ShaderInputLayout mLayout;
			String mVertexShader;
			String mPixelShader;
			String mGeometryShader;
			String mHullShader;
			String mDomainShader;
			bool mHasGeometryShader = false;
			bool mHasHullShader = false;
			bool mHasDomainShader = false;
		};

		class EffectImpl : public Effect
		{
		protected:
			virtual ~EffectImpl() {}
		public:
			std::map<String, std::vector<uint8_t> > mShaderName;
			std::vector<EachEffect> mEffect;

			String mEffectFlag;

			virtual const uint8_t* GetShader(const String& str) const;
			virtual size_t GetShaderSize(const String& str) const;

			virtual size_t GetShaderCount() const;
			virtual const Render3D::ShaderInputLayoutAttri* GetInputLayoutAttri(size_t mIndex) const;
			virtual size_t GetEffectCount() const;
			virtual String GetVertexShader(size_t mIndex) const;
			virtual String GetPixelShader(size_t mIndex) const;
			virtual String GetGeometryShader(size_t mIndex) const;
			virtual String GetHullShader(size_t mIndex) const;
			virtual String GetDomainShader(size_t mIndex) const;
			virtual bool HasGeometryShader(size_t mIndex) const;
			virtual bool HasHullShader(size_t mIndex) const;
			virtual bool HasDomainShader(size_t mIndex) const;
			virtual String GetEffectName(size_t mIndex) const;
			virtual size_t GetInputLayoutAttriCount(size_t mIndex) const;

			virtual void Save(Stream* mStream) const;
		};
	}
}