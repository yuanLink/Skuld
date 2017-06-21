#include <Skuld/String.h>

using namespace Skuld;

int main()
{
	String str = "风格和和嘎哈嘎嘎哈哈";
	std::string c = str.GetUTF8Str();
	std::cout << String(c.c_str(), false);
	return 0;
}