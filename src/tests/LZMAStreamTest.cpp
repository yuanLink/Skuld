#include <Skuld/Algorithm/CompressedStream.h>
#include <Skuld/FileStream.h>
#include <Skuld/Ptr.hpp>

using namespace Skuld::Algorithm;
using namespace Skuld;

void write()
{
	Ptr<Stream> mem(FileStream::Open("haha_lzma.bin", FileMode::Create));
	Ptr<Stream> cop(CompressedStream::CreateLZMAStream(mem, CompressionMode_Compress));
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
	Ptr<Stream> mem(FileStream::Open("haha_lzma.bin", FileMode::Open));
	Ptr<Stream> cop(CompressedStream::CreateLZMAStream(mem, CompressionMode_Decompress));
	char test[110];
	cop->Read(test, sizeof(test));
}

int main()
{
	write();
	read();
	return 0;
}