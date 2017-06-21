#include <boost/program_options.hpp>
#include <Skuld/Codec/BitmapCodec.h>
#include <Skuld/Ptr.hpp>
#include <Skuld/FileStream.h>

using namespace boost::program_options;
using namespace Skuld;
using namespace Skuld::Codec;

int main(int argc, char** argv)
{
	options_description mDesc("imgconv");

	mDesc.add_options()
		("help,h", "Show help.")
		("input,i", value<String>(), "Input image.")
		("output,o", value<String>(), "Output image")
		("format,f", value<String>(), "Output image format");
	variables_map mVM;
	store(command_line_parser(argc, argv).options(mDesc).run(), mVM);

	if (mVM.count("help")) std::cout << mDesc << std::endl;
	else
	{
		BitmapFormat mFormat = BitmapFormat_ICRAW;
		if (mVM.count("format")) mFormat = BitmapFormatFromString(mVM["format"].as<String>());
		
		if (mVM.count("input") == 0 || mVM.count("output") == 0) std::cout << mDesc << std::endl;
		else
		{
			Ptr<Stream> mIn(FileStream::Open(mVM["input"].as<String>(), FileMode::Open));
			if (mIn == nullptr)
			{
				std::cout << "Cannot open input file" << std::endl;
				return -1;
			}

			Ptr<Bitmap> mBitmap(DecodeBitmapFromStream(mIn));
			if (mBitmap == nullptr)
			{
				std::cout << "Cannot decode file" << std::endl;
				return -1;
			}

			Ptr<Stream> mOut(FileStream::Open(mVM["output"].as<String>(), FileMode::Create));
			if (mOut == nullptr)
			{
				std::cout << "Cannot open output file" << std::endl;
				return -1;
			}

			EncodeBitmapFromStream(mOut, mBitmap, mFormat);
		}
	}

	return 0;
}