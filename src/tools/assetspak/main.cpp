#include <boost/program_options.hpp>
#include <Skuld/Storage/libAssets.h>

using namespace boost::program_options;
using namespace Skuld;
using namespace Skuld::Storage;

int main(int argc, char** argv)
{
	options_description mDesc("assetspak");

	mDesc.add_options()
		("help,h", "Show help.")
		("output,o", value<String>(), "Output asset pack");
	variables_map mVM;
	store(command_line_parser(argc, argv).options(mDesc).run(), mVM);

	if (mVM.count("help")) std::cout << mDesc << std::endl;
	else
	{
		if (mVM.count("output") == 0) std::cout << mDesc << std::endl;
		else
		{
			
		}
	}
	return 0;
}