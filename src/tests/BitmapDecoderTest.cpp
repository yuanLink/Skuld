#include <Skuld/Codec/BitmapCodec.h>
#include <Skuld/Ptr.hpp>
#include <Skuld/FileStream.h>

using namespace Skuld::Codec;
using namespace Skuld;

int main()
{
	Ptr<Bitmap> bmp =
		DecodeBitmapFromStream(
			MakePtr(FileStream::Open(SKULD_BUILD_DIR"/../../resources/test.jpg", 
					FileMode::Open))
		);

	EncodeBitmapFromStream(
		MakePtr(FileStream::Open("test-indexed.jpg", FileMode::Create)),
		bmp, BitmapFormat_JPEG
	);
	return 0;
}