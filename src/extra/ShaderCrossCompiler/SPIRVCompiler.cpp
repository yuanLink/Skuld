#include <spirv_glsl.hpp>
#include <spirv_msl.hpp>
#include "Compiler.h"

namespace Skuld
{
	namespace ShaderCrossCompiler
	{
		class SPIRVCompiler : public Compiler
		{
		public:
			virtual Extra::ShaderBinaryFormat Format() { return Extra::ShaderBinaryFormat_SPIRV; }
			virtual bool Compile(const char* mSrc, std::vector<uint8_t>& mDst,
				Render3D::ShaderType mProfile, const char* mMain);
		};

		bool SPIRVCompiler::Compile(const char * mSrc, std::vector<uint8_t>& mDst,
			Render3D::ShaderType mProfile, const char* mMain)
		{
			return false;
		}

		class GLSLCompiler : public SPIRVCompiler
		{
		public:
			virtual Extra::ShaderBinaryFormat Format() { return Extra::ShaderBinaryFormat_GLSL; }
			virtual bool Compile(const char* mSrc, std::vector<uint8_t>& mDst,
				Render3D::ShaderType mProfile, const char* mMain);
		};

		bool GLSLCompiler::Compile(const char * mSrc, std::vector<uint8_t>& mDst,
			Render3D::ShaderType mProfile, const char* mMain)
		{
			std::vector<uint8_t> mSPIRV;
			this->SPIRVCompiler::Compile(mSrc, mSPIRV, mProfile, mMain);

			spirv_cross::CompilerGLSL mTemp(reinterpret_cast<const uint32_t*>(mSPIRV.data()), mSPIRV.size() / 4);

			std::string mGLSL;
			try
			{
				mGLSL = mTemp.compile();
			}
			catch (spirv_cross::CompilerError& e)
			{
				throw Exception(e.what());
			}
			mDst.resize(mGLSL.size() + 1);
			memcpy(mDst.data(), mGLSL.c_str(), mGLSL.size());
			mDst[mGLSL.size()] = 0;

			return true;
		}

		class MetalCompiler : public SPIRVCompiler
		{
		public:
			virtual Extra::ShaderBinaryFormat Format() { return Extra::ShaderBinaryFormat_Metal; }
			virtual bool Compile(const char* mSrc, std::vector<uint8_t>& mDst,
				Render3D::ShaderType mProfile, const char* mMain);
		};

		bool MetalCompiler::Compile(const char * mSrc, std::vector<uint8_t>& mDst, Render3D::ShaderType mProfile, const char * mMain)
		{
			std::vector<uint8_t> mSPIRV;
			this->SPIRVCompiler::Compile(mSrc, mSPIRV, mProfile, mMain);

			spirv_cross::CompilerMSL mTemp(reinterpret_cast<const uint32_t*>(mSPIRV.data()), mSPIRV.size() / 4);

			std::string mMSL;
			try
			{
				mMSL = mTemp.compile();
			}
			catch (spirv_cross::CompilerError& e)
			{
				throw Exception(e.what());
			}
			mDst.resize(mMSL.size() + 1);
			memcpy(mDst.data(), mMSL.c_str(), mMSL.size());
			mDst[mMSL.size()] = 0;

			return true;
		}

		std::shared_ptr<Compiler> CreateGLSLCompiler() {
			return std::make_shared<GLSLCompiler>();
		}

		std::shared_ptr<Compiler> CreateSPIRVCompiler() {
			return std::make_shared<SPIRVCompiler>();
		}

		std::shared_ptr<Compiler> CreateMetalCompiler() {
			return std::make_shared<MetalCompiler>();
		}
	}
}