#include <boost/any.hpp>
#include <vector>
#include <Skuld/Storage/Database.h>

namespace Skuld
{
	namespace Storage
	{
		class Record : public DataRecord
		{
		public:
			virtual ~Record() {}

			std::vector<boost::any> mData;

			DbValueType GetType(size_t mIndex) const;

			void Set(size_t mIndex, uint64_t mValue);
			void Set(size_t mIndex, double mValue);
			void Set(size_t mIndex, const String& mValue);
			void Set(size_t mIndex, const uint8_t* mValue, size_t mValueSize);
			void SetNull(size_t mIndex);

			uint64_t GetInteger(size_t mIndex) const;
			double GetFloat(size_t mIndex) const;
			const String& GetText(size_t mIndex) const;
			String& GetText(size_t mIndex);
			size_t GetBlobSize(size_t mIndex) const;
			void ResizeBlob(size_t mIndex, size_t mSize);
			uint8_t* GetBlob(size_t mIndex);
			const uint8_t* GetBlob(size_t mIndex) const;
		};
	}
}