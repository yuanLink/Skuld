#include "Compiler.h"
#include <functional>
#include <Skuld/Ptr.hpp>
#include <spirv_glsl.hpp>

namespace Skuld
{
	namespace ShaderCrossCompiler
	{
		std::shared_ptr<Compiler> CreateeDXBCCompiler();
	}
}

namespace Skuld
{
	namespace Extra
	{
		namespace {
			class CompilerRegister {
			public:
				std::vector<std::function<std::shared_ptr<ShaderCrossCompiler::Compiler>()>> mObjects;
				void Register(std::function<std::shared_ptr<ShaderCrossCompiler::Compiler>()> func)  {
					mObjects.push_back(func);
				}

				CompilerRegister() {
					Register(ShaderCrossCompiler::CreateeDXBCCompiler);
				}
			};

			static CompilerRegister mRegister;

			class SCC_DataImpl : public SCC_Data
			{
			protected:
				virtual ~SCC_DataImpl() {}
			public:
				std::string mErrOrWarning;
				std::vector<uint8_t> mData;

				virtual const uint8_t *Data() const { return mData.data(); }
				virtual size_t DataSize() const { return mData.size(); }
				virtual const char* ErrorOrWarning() const { return mErrOrWarning.c_str(); }
			};
		}

		std::string SPIRVToGLSL(const SCC_Data* mData)
		{
			spirv_cross::CompilerGLSL mTemp(reinterpret_cast<const uint32_t*>(mData->Data()), mData->DataSize() / 4);
			return mTemp.compile();
		}

		SCC_Data* CompileHLSL(const char* mHLSL, ShaderBinaryFormat mFormat)
		{
			for (std::function<std::shared_ptr<ShaderCrossCompiler::Compiler>()>& mCompilerFactory : mRegister.mObjects)
			{
				std::shared_ptr<ShaderCrossCompiler::Compiler> mCompiler = mCompilerFactory();
				if (mCompiler->Format() == mFormat)
				{
					try
					{
						Ptr<SCC_DataImpl> mRet(new SCC_DataImpl());
						bool mStatus = mCompiler->Compile(mHLSL, mRet->mData);
						if (!mStatus) {
							mRet->mErrOrWarning = mCompiler->mWarning;
						}
						return mRet.Detach();
					}
					catch (Exception& e)
					{
						Ptr<SCC_DataImpl> mRet(new SCC_DataImpl());
						mRet->mErrOrWarning = e.Message().GetStr();
						return mRet.Detach();
					}
				}
			}
			return nullptr;
		}
	}
}