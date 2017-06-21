#include <stdlib.h>
#include <Skuld/Storage/Database.h>

using namespace Skuld::Storage;
using namespace Skuld;

int main()
{
	std::shared_ptr<DataRecord> rec = Database::CreateDataRecord();
	rec->Set(0, (uint64_t)100);
	rec->Set(1, 900.0);
	rec->Set(2, "阿拉什客户发开发时是分开");
	rec->ResizeBlob(3, 100);

	std::cout << rec->GetInteger(0) << std::endl;
	rec->GetBlob(3)[1] = 0x88;
	rec->GetText(2)[10] = SKULD_STR('的');
	std::cout << rec->GetText(2) << std::endl;
	std::cout << (uint32_t)rec->GetBlob(3)[1] << std::endl;
	std::cout << rec->GetFloat(1) << std::endl;
	return 0;
}