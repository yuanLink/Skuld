#include "EffectSelector.h"
#include <map>
#include <Skuld/Exception.h>

namespace Skuld
{
	namespace Core
	{
		void EffectSelector::SetEffect(const Effect * mEffectFile)
		{
			std::map<String, std::tuple<size_t, Render3D::ShaderType> > mShaderList;

			size_t mFoundShaderCount = 0;
			size_t mEffectCount = mEffectFile->GetEffectCount();

			this->mEffects.resize(mEffectCount);
			this->mEffectNames.resize(mEffectCount);
			this->mEffectLayouts.resize(mEffectCount);

			for (size_t i = 0; i < mEffectCount; i++)
			{
				this->mEffectNames[i] = mEffectFile->GetEffectName(i);

				String mVS = mEffectFile->GetVertexShader(i);
				std::map<String, std::tuple<size_t, Render3D::ShaderType> >::iterator it;
				if ((it = mShaderList.find(mVS)) != mShaderList.end())
					mShaderList[mVS] = std::make_tuple(mFoundShaderCount++, Render3D::ShaderType_VertexShader);
				mEffects[i][0] = std::get<0>(mShaderList[mVS]);

				String mPS = mEffectFile->GetPixelShader(i);
				if ((it = mShaderList.find(mPS)) != mShaderList.end())
					mShaderList[mPS] = std::make_tuple(mFoundShaderCount++, Render3D::ShaderType_PixelShader);
				mEffects[i][1] = std::get<0>(mShaderList[mPS]);

				if (mEffectFile->HasGeometryShader(i))
				{
					String mGS = mEffectFile->GetGeometryShader(i);
					if ((it = mShaderList.find(mGS)) != mShaderList.end())
						mShaderList[mGS] = std::make_tuple(mFoundShaderCount++, Render3D::ShaderType_GeometryShader);
					mEffects[i][2] = std::get<0>(mShaderList[mGS]);
				}
				else mEffects[i][2] = mNoShader;

				if (mEffectFile->HasHullShader(i))
				{
					String mHS = mEffectFile->GetHullShader(i);
					if ((it = mShaderList.find(mHS)) != mShaderList.end())
						mShaderList[mHS] = std::make_tuple(mFoundShaderCount++, Render3D::ShaderType_HullShader);
					mEffects[i][3] = std::get<0>(mShaderList[mHS]);
				}
				else mEffects[i][3] = mNoShader;

				if (mEffectFile->HasDomainShader(i))
				{
					String mDS = mEffectFile->GetDomainShader(i);
					if ((it = mShaderList.find(mDS)) != mShaderList.end())
						mShaderList[mDS] = std::make_tuple(mFoundShaderCount++, Render3D::ShaderType_DomainShader);
					mEffects[i][4] = std::get<0>(mShaderList[mDS]);
				}
				else mEffects[i][4] = mNoShader;
			}
			if (mShaderList.size() != mEffectFile->GetShaderCount()) throw Exception("未知错误");

			for (std::map<String, std::tuple<size_t, Render3D::ShaderType> >::iterator it = mShaderList.begin();
				it != mShaderList.end(); ++it)
			{
				const uint8_t * mShaderBuffer = mEffectFile->GetShader(it->first);
				size_t mShaderSize = mEffectFile->GetShaderSize(it->first);

				mShaders[std::get<0>(it->second)].Detach(
					mContext->CreateShader(mShaderBuffer, mShaderSize, std::get<1>(it->second)));
			}

			for (size_t i = 0; i < mEffectCount; i++)
			{
				const Render3D::ShaderInputLayoutAttri* mAttri = mEffectFile->GetInputLayoutAttri(i);
				size_t mAttriCount = mEffectFile->GetInputLayoutAttriCount(i);
				size_t mShaderIndex = std::get<0>(mShaderList[mEffectFile->GetEffectName(i)]);

				mEffectLayouts[i].Detach(
					mContext->CreateInputLayout(mAttri, mAttriCount, mShaders[mShaderIndex]));
			}
		}
	}
}