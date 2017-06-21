#include <vector>
#include <algorithm>
#include <Skuld/UI/UI.h>
#include <Skuld/FileStream.h>
#include "UICairoContext.h"
#include <mutex>
#include "UIInternal.h"

namespace Skuld
{
	namespace UI
	{
		class PrivateUI : public UI
		{
		protected:
			virtual ~PrivateUI();
		public:
			PrivateUI(UIContext* pContext) : UI(pContext){
				pContext->AddRef();
			}
			std::vector<UIControl*> Controls;
			int32_t FocusIndex = -1;
			std::mutex mMutex;

			void AddControl(UIControl* pControl);
			void DestroyControl(UIControl* pControl);
			const UIContext* GetContext() const;
			void SetContext(UIContext* pContext);
			
			void OnPaint();
		};

		UI::UI(UIContext* pContext) : pContext(pContext) {}
		
		UI::~UI()
		{
			if (this->pContext)
				this->pContext->Dispose();
		}

		void PrivateUI::AddControl(UIControl * pControl)
		{
			std::lock_guard<std::mutex> mLock(this->mMutex);
			pControl->AddRef();
			this->Controls.push_back(pControl);

			std::sort(this->Controls.begin(), this->Controls.end(), [](const UIControl* a, const UIControl* b) {
				return a->GetPosition().z < b->GetPosition().z;
			});
		}

		void PrivateUI::DestroyControl(UIControl * pControl)
		{
			std::lock_guard<std::mutex> mLock(this->mMutex);
			std::vector<UIControl*>::iterator it = std::find(this->Controls.begin(), this->Controls.end(), pControl);
			if (it == this->Controls.end()) return;
			this->Controls.erase(it);

			std::sort(this->Controls.begin(), this->Controls.end(), [](const UIControl* a, const UIControl* b) {
				return a->GetPosition().z < b->GetPosition().z;
			});

			pControl->OnDestroy(this->pContext);
			pControl->Dispose();
		}
		
		void PrivateUI::OnPaint()
		{
			std::lock_guard<std::mutex> mLock(this->mMutex);
			if (this->pContext)
			{
				for (std::vector<UIControl*>::iterator it = this->Controls.begin(); it != this->Controls.end(); ++it)
				{
					if ((*it)->GetVisible())
						(*it)->OnPaint(this->pContext);
				}
			}
		}

		const UIContext* PrivateUI::GetContext() const 
		{
			return this->pContext;
		}

		void PrivateUI::SetContext(UIContext * pContext)
		{
			std::lock_guard<std::mutex> mLock(this->mMutex);
			if (this->pContext) pContext->Dispose();
			pContext->AddRef();
			this->pContext = pContext;
		}

		PrivateUI::~PrivateUI()
		{
			std::lock_guard<std::mutex> mLock(this->mMutex);
			for (auto it : Controls)
			{
				it->OnDestroy(this->pContext);
				it->Dispose();
			}
		}

		UI * CreateUI(UIContext* mContext)
		{
			auto ret = new (std::nothrow) PrivateUI(mContext);
			return ret;
		}
	}
}