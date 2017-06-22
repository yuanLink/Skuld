#include "EffectImpl.h"
#include <json/json.h>
#include <Skuld/Exception.h>
#include <Skuld/Ptr.hpp>
#include <functional>
#include <algorithm>
#include <Skuld/FileStream.h>
#include <Skuld/Text.h>

namespace Skuld
{
	namespace Core
	{
		static std::function<Extra::ShaderCompileFunc> mCompilerFunc;

		static void CheckCompiler()
		{
			if (mCompilerFunc != nullptr) return;
			throw Exception("未加载ShaderCrossCompiler");
		}

		static std::vector<uint8_t> CompileShader(const std::string& mUTF8HLSL,
			const char* mProfile, const char* mEntry)
		{
			Ptr<Extra::SCC_Data> mRet = mCompilerFunc(mUTF8HLSL.c_str(), Extra::ShaderBinaryFormat_DXBC);
			if (mRet->DataSize() == 0)
				throw Exception(String("编译失败：") + mRet->ErrorOrWarning());
			std::vector<uint8_t> mR(mRet->DataSize());
			memcpy(mR.data(), mRet->Data(), mR.size());
			return std::move(mR);
		}

		static void SortBySlot(ShaderInputLayout::iterator mBegin, ShaderInputLayout::iterator mEnd) {
			std::sort(mBegin, mEnd,
				[](const Render3D::ShaderInputLayoutAttri& a, const Render3D::ShaderInputLayoutAttri& b) {
				return a.mSlot < b.mSlot;
			});
		}

		Effect * Effect::LoadFromStream(Stream * mStream)
		{
			Ptr<EffectImpl> mRet(new EffectImpl());

			char mFlag[4];
			mStream->Read(mFlag, 4);
			if (memcmp(mFlag, "EFCT", 4)) return nullptr;

			uint32_t mEffCnt, mShaCnt;
			mStream->Read(&mEffCnt);
			mStream->Read(&mShaCnt);
			mRet->mEffect.resize(mEffCnt);
			mRet->mEffectFlag = mStream->ReadString();

			for (EachEffect& mEff : mRet->mEffect)
			{
				mEff.mName = mStream->ReadString();

				uint32_t mAttriCnt;;
				mStream->Read(&mAttriCnt);
				mEff.mLayout.resize(mAttriCnt);
				for (Render3D::ShaderInputLayoutAttri& mAttri : mEff.mLayout)
				{
					mAttri.mName = mStream->ReadString();
					mStream->Read(&mAttri.mSlot);
					mStream->Read(&mAttri.mType);
					mStream->Read(&mAttri.mSize);
				}
				SortBySlot(mEff.mLayout.begin(), mEff.mLayout.end());
				mEff.mVertexShader = mStream->ReadString();
				mEff.mPixelShader = mStream->ReadString();
				mEff.mGeometryShader = mStream->ReadString();
				mEff.mHullShader = mStream->ReadString();
				mEff.mDomainShader = mStream->ReadString();
				mStream->Read(&mEff.mHasGeometryShader);
				mStream->Read(&mEff.mHasHullShader);
				mStream->Read(&mEff.mHasDomainShader);
			}

			for (uint32_t i = 0; i < mShaCnt; i++)
			{
				std::vector<uint8_t> &mShader = mRet->mShaderName[mStream->ReadString()];
				uint32_t mCodeSize;
				mStream->Read(&mCodeSize);
				mShader.resize(mCodeSize);
				mStream->Read(mShader.data(), mShader.size());
			}

			return mRet.Detach();
		}

		Effect * Effect::CompileFromString(const String & mHLSL, const String & mEffectScript)
		{
			CheckCompiler();

			Ptr<EffectImpl> mRet(new EffectImpl());

			std::string mHLSL_ = mHLSL.GetUTF8Str();

			Json::Reader mReader;
			Json::Value mJSONRoot;
			if (!mReader.parse(mEffectScript.GetUTF8Str(), mJSONRoot))
				throw Exception("JSON解析失败");

			size_t mEffectCount = mJSONRoot.size();
			std::vector<std::string> mEffectName = mJSONRoot.getMemberNames();

			for (size_t i = 0; i < mEffectCount; i++)
			{
				Json::Value mEffect = mJSONRoot[mEffectName[i]];
				if (!mEffect.isObject())
					throw Exception("属性类型错误");

				Json::Value mVertexLayout = mEffect["VertexLayout"];
				if (!mVertexLayout.isObject())
					throw Exception("属性类型错误");

				std::vector<std::string> mVertexLayoutAttriName = mVertexLayout.getMemberNames();
				size_t mVertexLayoutAttriCount = mVertexLayout.size();
				EachEffect mCompiledEffect;
				mCompiledEffect.mName = String(mEffectName[i].c_str(), false);

				for (size_t j = 0; j < mVertexLayoutAttriCount; j++)
				{
					Json::Value mAttri = mVertexLayout[mVertexLayoutAttriName[j]];
					if (!mAttri.isObject())
						throw Exception("属性类型错误");

					Json::Value mAttriSize = mAttri["Size"];
					if (!mAttriSize.isIntegral())
						throw Exception("属性类型错误");

					Json::Value mAttriSlot = mAttri["Slot"];
					if (!mAttriSlot.isIntegral())
						throw Exception("属性类型错误");

					if (mAttriSize.asUInt64() == 0 || mAttriSize.asUInt64() > 4)
						throw Exception("不支持的属性长度");

					Json::Value mAttriType = mAttri["Type"];
					if (!mAttriType.isString())
						throw Exception("属性类型错误");

					String mAttriTypeStr(mAttriType.asCString(), false);

					if (mAttriSize.asUInt64() == 3 && (mAttriTypeStr == "short" || mAttriTypeStr == "ushort" ||
						mAttriTypeStr == "byte" || mAttriTypeStr == "sbyte"))
						throw Exception("不支持的属性长度");

					Render3D::ShaderInputLayoutAttriType mType;
					if (mAttriTypeStr == "int") mType = Render3D::ShaderInputLayoutAttriType_int;
					else if (mAttriTypeStr == "uint") mType = Render3D::ShaderInputLayoutAttriType_uint;
					else if (mAttriTypeStr == "float") mType = Render3D::ShaderInputLayoutAttriType_float;
					else if (mAttriTypeStr == "short") mType = Render3D::ShaderInputLayoutAttriType_short;
					else if (mAttriTypeStr == "ushort") mType = Render3D::ShaderInputLayoutAttriType_ushort;
					else if (mAttriTypeStr == "byte") mType = Render3D::ShaderInputLayoutAttriType_byte;
					else if (mAttriTypeStr == "sbyte") mType = Render3D::ShaderInputLayoutAttriType_sbyte;
					else throw Exception("未知类型");

					uint32_t mSize = mAttriSize.asUInt();
					uint32_t mSlot = mAttriSlot.asUInt();

					mCompiledEffect.mLayout.push_back(Render3D::ShaderInputLayoutAttri(
						String(mVertexLayoutAttriName[j].c_str(), false),
						mSlot, mType, mSize));
				}
				SortBySlot(mCompiledEffect.mLayout.begin(), mCompiledEffect.mLayout.end());

				Json::Value mVertexShader = mEffect["VertexShader"];
				if (!mVertexShader.isString())
					throw Exception("属性类型错误");
				Json::Value mPixelShader = mEffect["PixelShader"];
				if (!mPixelShader.isString())
					throw Exception("属性类型错误");
				Json::Value mGeometryShader = mEffect["GeometryShader"];
				Json::Value mHullShader = mEffect["HullShader"];
				Json::Value mDomainShader = mEffect["DomainShader"];

				String mPSEntry(mPixelShader.asCString(), false);
				String mVSEntry(mVertexShader.asCString(), false);

				if (mRet->mShaderName.find(mPSEntry) == mRet->mShaderName.end())
				{
					mRet->mShaderName[mPSEntry] = std::move(
						CompileShader(mHLSL_, "ps_6_0", mPSEntry.GetUTF8Str().c_str())
					);
				}
				mCompiledEffect.mPixelShader = mPSEntry;

				if (mRet->mShaderName.find(mVSEntry) == mRet->mShaderName.end())
				{
					mRet->mShaderName[mVSEntry] = std::move(
						CompileShader(mHLSL_, "vs_6_0", mVSEntry.GetUTF8Str().c_str())
					);
				}
				mCompiledEffect.mVertexShader = mVSEntry;

				if (mGeometryShader.isString())
				{
					String mGSEntry(mGeometryShader.asCString(), false);
					if (mRet->mShaderName.find(mGSEntry) == mRet->mShaderName.end())
					{
						mRet->mShaderName[mGSEntry] = std::move(
							CompileShader(mHLSL_, "gs_6_0", mGSEntry.GetUTF8Str().c_str())
						);
					}
					mCompiledEffect.mGeometryShader = mGSEntry;
					mCompiledEffect.mHasGeometryShader = true;
				}
				else if (!mGeometryShader.isNull()) throw Exception("属性类型错误");

				if (mHullShader.isString())
				{
					String mHSEntry(mHullShader.asCString(), false);
					if (mRet->mShaderName.find(mHSEntry) == mRet->mShaderName.end())
					{
						mRet->mShaderName[mHSEntry] = std::move(
							CompileShader(mHLSL_, "hs_6_0", mHSEntry.GetUTF8Str().c_str())
						);
					}
					mCompiledEffect.mHullShader = mHSEntry;
					mCompiledEffect.mHasHullShader = true;
				}
				else if (!mHullShader.isNull()) throw Exception("属性类型错误");

				if (mDomainShader.isString())
				{
					String mDSEntry(mDomainShader.asCString(), false);
					if (mRet->mShaderName.find(mDSEntry) == mRet->mShaderName.end())
					{
						mRet->mShaderName[mDSEntry] = std::move(
							CompileShader(mHLSL_, "ds_6_0", mDSEntry.GetUTF8Str().c_str())
						);
					}
					mCompiledEffect.mDomainShader = mDSEntry;
					mCompiledEffect.mHasDomainShader = true;
				}
				else if (!mDomainShader.isNull()) throw Exception("属性类型错误");

				mRet->mEffect.push_back(std::move(mCompiledEffect));
			}
			return mRet.Detach();
		}

		Effect* Effect::CompileFromFile(const String& mHLSLFile, const String& mEffectScriptFile)
		{
			Ptr<FileStream> mHLSL = FileStream::Open(mHLSLFile, Open);
			Ptr<FileStream> mEffectScript = FileStream::Open(mEffectScriptFile, Open);

			if (mHLSL == nullptr || mEffectScript == nullptr) throw Exception("文件不存在");

			return CompileFromStream(mHLSL, mEffectScript);
		}

		Effect* Effect::CompileFromStream(Stream* mHLSL, Stream* mEffectScript)
		{
			if (mHLSL == nullptr || mEffectScript == nullptr) throw Exception("参数错误");

			return CompileFromString(
				ReadAllTextFromStream(mHLSL), ReadAllTextFromStream(mEffectScript));
		}

		void EffectImpl::Save(Stream * mStream) const
		{
			mStream->Write("EFCT", 4);
			uint32_t mEffCnt = static_cast<uint32_t>(mEffect.size());
			uint32_t mShaCnt = static_cast<uint32_t>(mShaderName.size());
			mStream->Write(&mEffCnt);
			mStream->Write(&mShaCnt);
			mStream->WriteString(mEffectFlag);

			for (const EachEffect& mEff : mEffect)
			{
				uint32_t mAttriCnt = static_cast<uint32_t>(mEff.mLayout.size());
				mStream->WriteString(mEff.mName);
				mStream->Write(&mAttriCnt);
				for (const Render3D::ShaderInputLayoutAttri& mAttri : mEff.mLayout)
				{
					mStream->WriteString(mAttri.mName);
					mStream->Write(&mAttri.mSlot);
					mStream->Write(&mAttri.mType);
					mStream->Write(&mAttri.mSize);
				}
				mStream->WriteString(mEff.mVertexShader);
				mStream->WriteString(mEff.mPixelShader);
				mStream->WriteString(mEff.mGeometryShader);
				mStream->WriteString(mEff.mHullShader);
				mStream->WriteString(mEff.mDomainShader);
				mStream->Write(&mEff.mHasGeometryShader);
				mStream->Write(&mEff.mHasHullShader);
				mStream->Write(&mEff.mHasDomainShader);
			}

			for (const std::pair<String, std::vector<uint8_t> >& mShader : mShaderName)
			{
				mStream->WriteString(mShader.first);
				uint32_t mCodeSize = static_cast<uint32_t>(mShader.second.size());
				mStream->Write(&mCodeSize);
				mStream->Write(mShader.second.data(), mShader.second.size());
			}
		}

		const uint8_t* EffectImpl::GetShader(const String& str) const {
			if (mShaderName.find(str) != mShaderName.end())
				return mShaderName.at(str).data();
			return nullptr;
		}

		size_t EffectImpl::GetShaderSize(const String& str) const {
			if (mShaderName.find(str) != mShaderName.end())
				return mShaderName.at(str).size();
			return ~(size_t)0;
		}

		size_t EffectImpl::GetShaderCount() const { return mShaderName.size(); }
		const Render3D::ShaderInputLayoutAttri* EffectImpl::GetInputLayoutAttri(size_t mIndex) const {
			if (mIndex >= mEffect.size()) throw Exception("数组越界");
			return mEffect[mIndex].mLayout.data();
		}
		size_t EffectImpl::GetInputLayoutAttriCount(size_t mIndex) const {
			if (mIndex >= mEffect.size()) throw Exception("数组越界");
			return mEffect[mIndex].mLayout.size();
		}
		size_t EffectImpl::GetEffectCount() const {
			return mEffect.size();
		}
		String EffectImpl::GetVertexShader(size_t mIndex) const {
			if (mIndex >= mEffect.size()) throw Exception("数组越界");
			return mEffect[mIndex].mVertexShader;
		}
		String EffectImpl::GetPixelShader(size_t mIndex) const {
			if (mIndex >= mEffect.size()) throw Exception("数组越界");
			return mEffect[mIndex].mPixelShader;
		}
		String EffectImpl::GetGeometryShader(size_t mIndex) const {
			if (mIndex >= mEffect.size()) throw Exception("数组越界");

			if (!mEffect[mIndex].mHasGeometryShader) throw Exception("数据不存在");
			return mEffect[mIndex].mGeometryShader;
		}
		String EffectImpl::GetHullShader(size_t mIndex) const {
			if (mIndex >= mEffect.size()) throw Exception("数组越界");

			if (!mEffect[mIndex].mHasHullShader) throw Exception("数据不存在");
			return mEffect[mIndex].mHullShader;
		}
		String EffectImpl::GetDomainShader(size_t mIndex) const {
			if (mIndex >= mEffect.size()) throw Exception("数组越界");

			if (!mEffect[mIndex].mHasDomainShader) throw Exception("数据不存在");
			return mEffect[mIndex].mDomainShader;
		}
		bool EffectImpl::HasGeometryShader(size_t mIndex) const {
			if (mIndex >= mEffect.size()) throw Exception("数组越界");
			return mEffect[mIndex].mHasGeometryShader;
		}
		bool EffectImpl::HasHullShader(size_t mIndex) const {
			if (mIndex >= mEffect.size()) throw Exception("数组越界");
			return mEffect[mIndex].mHasHullShader;
		}
		bool EffectImpl::HasDomainShader(size_t mIndex) const {
			if (mIndex >= mEffect.size()) throw Exception("数组越界");
			return mEffect[mIndex].mHasDomainShader;
		}
		String EffectImpl::GetEffectName(size_t mIndex) const {
			if (mIndex >= mEffect.size()) throw Exception("数组越界");
			return mEffect[mIndex].mName;
		}

		void LoadShaderCrossCompiler(std::function<Extra::ShaderCompileFunc> mEntry)
		{
			mCompilerFunc = mEntry;
		}
	}
}