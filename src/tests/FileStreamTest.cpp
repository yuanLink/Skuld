#include <Skuld/FileStream.h>
#include <Skuld/Ptr.hpp>
#include <vector>

using namespace Skuld;

void write()
{
	Ptr<FileStream> mStream = MakePtr(FileStream::Open("fs_test.bin", FileMode::Create));
	mStream->WriteString("caosb");
	std::vector<uint8_t> vec(16 * 1024 * 11 - 10);
	mStream->Write(vec.data(), 16 * 1024 * 11 - 10);
	mStream->Offset(100);
	mStream->WriteString("llhsb");
}

void read()
{
	Ptr<FileStream> mStream = MakePtr(FileStream::Open("fs_test.bin", FileMode::Open));
	std::string t1 = mStream->ReadString().GetStr();
	mStream->Offset(16 * 1024 * 11 + 90);
	std::string t2 = mStream->ReadString().GetStr();
}

int main()
{
	write();
	read();

	return 0;
}