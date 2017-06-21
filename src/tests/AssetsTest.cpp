#include <Skuld/Storage/libAssets.h>
#include <Skuld/Ptr.hpp>
#include <Skuld/FileStream.h>

using namespace Skuld::Storage;
using namespace Skuld;

int main()
{
	{
		Ptr<Asset> mAsset = Asset::Create(
			MakePtr(FileStream::Open("libassets.asst", Create)), "libassets");

		mAsset->AddFile("font", SKULD_BUILD_DIR "/../../resources/test2.TTF");
		mAsset->AddFile("png", SKULD_BUILD_DIR "/../../resources/test.png");

		mAsset->RemoveFile("font");
	}

	{
		Ptr<Asset> mAsset = Asset::Open(MakePtr(FileStream::Open("libassets.asst", Open)));

		Ptr<Asset> mAsset2 = mAsset->Repack(MakePtr(FileStream::Open("libassets.new.asst", Create)));
	}
	return 0;
}