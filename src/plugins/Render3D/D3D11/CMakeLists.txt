if(SKULD_WIN32)

add_plugin(Render3D D3D11
	D3D11Factory.cpp
	D3D11Factory.h
	D3D11Context.cpp
	D3D11Context.h
	D3D11InputLayout.cpp
	D3D11InputLayout.h
	D3D11Shader.cpp
	D3D11Shader.h
	D3D11Texture1D.cpp
	D3D11Texture1D.h
	D3D11Texture2D.cpp
	D3D11Texture2D.h
	D3D11Texture3D.cpp
	D3D11Texture3D.h
	D3D11Texture.h
	D3D11Texture.cpp
	D3D11Utility.cpp
	D3D11Utility.h
	D3D11RenderState.cpp
	D3D11RenderState.h
	D3D11BufferObject.cpp
	D3D11BufferObject.h
	D3D11FrameBufferObject.cpp
	D3D11FrameBufferObject.h
)

endif()