#pragma once

#include <Skuld/Render3D/Render3DFactory.h>
#include <Skuld/Check.h>
#include <d3d11.h>
#include <atlbase.h>

#pragma comment(lib, "d3d11.lib")

namespace Skuld
{
	namespace Render3D
	{
		class D3D11Factory : public Render3DFactory
		{
		protected:
			~D3D11Factory() {}
		public:
			Render3DContext* CreateContext(const WindowInfo* mInfo, const Config* mConfig) const;
			CheckFlags GetCheckFlag() const { return AvaliableRender3DAPI_D3D11; }
		};
	}
}