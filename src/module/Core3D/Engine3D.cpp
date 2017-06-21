#include <Skuld/Render3D/Render3DFactory.h>
#include <Skuld/Ptr.hpp>
#include <Skuld/Exception.h>
#include <Skuld/GlobalConfig.h>
#include <Skuld/Render3D/Render3DContext.h>
#include <Skuld/Core/Engine3D.h>
#include <Skuld/Storage/libAssets.h>

#include "../Support/PluginEntry.h"
#include "../UI/UIInternal.h"
#include "EffectSelector.h"
#include "EffectImpl.h"
#include "TextureManager.h"

using namespace Skuld::Render3D;

namespace Skuld
{
	namespace Core
	{
		class Engine3DImpl : public Engine3D
		{
		protected:
			virtual ~Engine3DImpl() {}

			Ptr<Render3DFactory> mFactory;
			Ptr<Render3DContext> mContext;
			Ptr<UI::Displayable> mDisplayable;
			Ptr<UI::UI> mUI;
			Ptr<SkuldEngine> mEngine;

			EffectSelector mEffectSelector;
			TextureManager mTextureManager;

			std::map<String, Ptr<Storage::Asset> > mAssets;
		public:
			Engine3DImpl(Ptr<Render3DFactory> mFactory,
				Ptr<Render3DContext> mContext,
				Ptr<UI::Displayable> mDisplayable,
				Ptr<UI::UI> mUI,
				Ptr<SkuldEngine> mEngine
			) :
				mFactory(mFactory),
				mContext(mContext),
				mDisplayable(mDisplayable),
				mUI(mUI),
				mEngine(mEngine),
				mEffectSelector(mContext),
				mTextureManager(mContext)
			{
			}
			virtual void LoadAssets(Storage::Asset *mAsset);
		};

		Engine3D * Engine3D::Create3DEngine(SkuldEngine* mEngine, UI::Displayable* mDisplayable)
		{
			if (mEngine->plugins[tRender3D] == nullptr) throw Exception("插件未注册");

			Ptr<Render3DFactory> mRender3DFactory = mEngine->CreateRender3DFactory();
			Ptr<Render3DContext> mRender3DContext = mRender3DFactory->CreateContext(
				mDisplayable->GetWindowInfo(), GetGlobalConfig());

			UI::UI* mUI = UI::CreateUI(MakePtr(
				UI::CreateCairoContext(mDisplayable->GetWidth(), mDisplayable->GetHeight())));

			mDisplayable->AddRef();
			mEngine->AddRef();
			return new Engine3DImpl(
				mRender3DFactory,
				mRender3DContext,
				MakePtr(mDisplayable),
				MakePtr(mUI),
				MakePtr(mEngine)
			);
		}

		void Engine3DImpl::LoadAssets(Storage::Asset * mAsset)
		{
			CheckFlags mFlags = GetCurrentCheckFlags(mEngine);
			if (!IsCheckFlagsAvaliable(mFlags, mAsset->GetCheckFlags())) throw Exception("资源包不能用于当前程序");

			mAsset->AddRef();
			mAssets[mAsset->GetName()] = MakePtr(mAsset);
		}
	}
}