#include <Skuld/Algorithm/EncryptedStream.h>
#include <Skuld/FileStream.h>
#include <Skuld/Ptr.hpp>

using namespace Skuld::Algorithm;
using namespace Skuld;

const uint8_t mPad[] = {
	0x6, 0x8, 0x6e, 0x88, 0x19, 0xff, 0x0, 0x89
};

void write()
{
	Ptr<Stream> mem(FileStream::Open("haha.bin", FileMode::Create));
	Ptr<Stream> cop(EncryptedStream::CreateXORStream(mem, EncryptionMode_Encrypt, mPad, sizeof(mPad)));
	cop->WriteString("abcdabcdbcd");
	cop->WriteString("12345678");
	cop->WriteString("qwertyuio");
	cop->WriteString("asdfghjkl");
	cop->WriteString("zxcvbnm");
	cop->WriteString("llhsbllhsbllhsb");
	cop->WriteString("caosbcaosbcaosb");
}

void read()
{
	Ptr<Stream> mem(FileStream::Open("haha.bin", FileMode::Open));
	Ptr<Stream> cop(EncryptedStream::CreateXORStream(mem, EncryptionMode_Decrypt, mPad, sizeof(mPad))); std::wstring t1 = cop->ReadString().GetWStr();
	std::wstring t2 = cop->ReadString().GetWStr();
	std::wstring t3 = cop->ReadString().GetWStr();
	std::wstring t4 = cop->ReadString().GetWStr();
	std::wstring t5 = cop->ReadString().GetWStr();
	std::wstring t6 = cop->ReadString().GetWStr();
	std::wstring t7 = cop->ReadString().GetWStr();
	std::wstring t8 = cop->ReadString().GetWStr();
}

int main()
{
	write();
	read();
	return 0;
}