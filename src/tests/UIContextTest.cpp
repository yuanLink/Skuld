#include <Windows.h>
#include <Skuld/UI/UIContext.h>
#include <Skuld/Ptr.hpp>
#include <Skuld/FileStream.h>

using namespace Skuld::UI;
using namespace Skuld;

void savebmp(const uint8_t * pdata, char * bmp_file, int width, int height)
{
	int size = width*height * 4 * sizeof(char);
	BITMAPFILEHEADER bfh;
	bfh.bfType = (WORD)0x4d42;  //bm  
	bfh.bfSize = size  // data size  
		+ sizeof(BITMAPFILEHEADER) // first section size  
		+ sizeof(BITMAPINFOHEADER) // second section size  
		;
	bfh.bfReserved1 = 0; // reserved  
	bfh.bfReserved2 = 0; // reserved  
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//真正的数据的位置  

	BITMAPINFOHEADER bih;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = width;
	bih.biHeight = -height;//
	bih.biPlanes = 1;//为1，不用改  
	bih.biBitCount = 32;
	bih.biCompression = 0;//不压缩  
	bih.biSizeImage = size;
	bih.biXPelsPerMeter = 2835;//像素每米  
	bih.biYPelsPerMeter = 2835;
	bih.biClrUsed = 0;//已用过的颜色，24位的为0  
	bih.biClrImportant = 0;
	FILE * fp = fopen(bmp_file, "wb");
	if (!fp) return;
	fwrite(&bfh, 8, 1, fp);
	fwrite(&bfh.bfReserved2, sizeof(bfh.bfReserved2), 1, fp);
	fwrite(&bfh.bfOffBits, sizeof(bfh.bfOffBits), 1, fp);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(pdata, size, 1, fp);
	fclose(fp);
}

namespace Skuld
{
	namespace UI
	{
		class UI;
		class UIContext;
		UIContext* CreateCairoContext(uint32_t mWidth, uint32_t mHeight);
		UI* CreateUI(UIContext* mContext);
	}
}

int main()
{
	UIContext* pContext = CreateCairoContext(300, 300);
	//pContext->DrawEllipse(50, 50, 100, 50, float4(1.0, 0.0, 0.0, 1.0));
	pContext->FillRect(50, 50, 100, 100, Skuld::float4(1.0, 1.0, 0.0, 1.0));

	pContext->DrawString(0, 0, "asdfasasfaf مرحباً", 16, nullptr, Skuld::float4(0.0f, 0.0f, 1.0f, 1.0f));

	//UIImage* image = pContext->CreateUIImage(
	//	MakePtr(FileStream::Open("../../../resources/test.png", FileMode::Open)));
	//pContext->DrawImage(0, 0, image);

	savebmp(pContext->GetPixelData(), "test.bmp", pContext->GetWidth(), pContext->GetHeight());
	
	//image->Dispose();
	pContext->Dispose();
	return 0;
}