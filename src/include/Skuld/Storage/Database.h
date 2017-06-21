#pragma once

#include "../IDisposable.h"
#include "../String.h"
#include <memory>

namespace Skuld
{
	namespace Storage
	{
		enum DbValueType
		{
			DbValueType_Integer,
			DbValueType_Float,
			DbValueType_Text,
			DbValueType_Blob,
			DbValueType_Null
		};

		class DataRecord
		{
		public:
			virtual ~DataRecord() {};
			virtual DbValueType GetType(size_t mIndex) const = 0;

			virtual void Set(size_t mIndex, uint64_t mValue) = 0;
			virtual void Set(size_t mIndex, double mValue) = 0;
			virtual void Set(size_t mIndex, const String& mValue)= 0;
			virtual void Set(size_t mIndex, const uint8_t* mValue, size_t mValueSize) = 0;
			virtual void SetNull(size_t mIndex) = 0;

			virtual uint64_t GetInteger(size_t mIndex) const = 0;
			virtual double GetFloat(size_t mIndex) const = 0;
			virtual const String& GetText(size_t mIndex) const = 0;
			virtual String& GetText(size_t mIndex) = 0;
			virtual size_t GetBlobSize(size_t mIndex) const = 0;
			virtual void ResizeBlob(size_t mIndex, size_t mSize) = 0;
			virtual uint8_t* GetBlob(size_t mIndex) = 0;
			virtual const uint8_t* GetBlob(size_t mIndex) const = 0;
		};

		class Database :public IDisposable
		{
		protected:
			virtual ~Database() {}

			static SKULD_EXPORT DataRecord* _CreateDataRecord();
		public:
			static SKULD_EXPORT Database* CreateSQLiteDatabase(const String& path);
			static std::shared_ptr<DataRecord> CreateDataRecord() { return std::shared_ptr<DataRecord>(_CreateDataRecord()); }
		};
	}
}