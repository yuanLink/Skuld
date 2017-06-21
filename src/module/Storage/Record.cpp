#include "Record.h"
#include <Skuld/Exception.h>

namespace Skuld
{
	namespace Storage
	{
		DbValueType Record::GetType(size_t mIndex) const
		{
			if (mIndex >= mData.size()) throw Exception("索引超出数组范围");
			if (mData[mIndex].empty()) return DbValueType_Null;
			if (mData[mIndex].type() == boost::typeindex::type_id<uint64_t>())
				return DbValueType_Integer;
			else if (mData[mIndex].type() == boost::typeindex::type_id<double>())
				return DbValueType_Float;
			else if (mData[mIndex].type() == boost::typeindex::type_id<String>())
				return DbValueType_Text;
			else if (mData[mIndex].type() == boost::typeindex::type_id<std::vector<uint8_t>>())
				return DbValueType_Blob;
			else throw Exception("类型未知");
		}

		void Record::Set(size_t mIndex, uint64_t mValue)
		{
			if (mIndex >= mData.size()) mData.resize(mIndex + 1);
			mData[mIndex] = mValue;
		}

		void Record::Set(size_t mIndex, double mValue)
		{
			if (mIndex >= mData.size()) mData.resize(mIndex + 1);
			mData[mIndex] = mValue;
		}

		void Record::Set(size_t mIndex, const String & mValue)
		{
			if (mIndex >= mData.size()) mData.resize(mIndex + 1);
			mData[mIndex] = String(mValue);
		}

		void Record::Set(size_t mIndex, const uint8_t * mValue, size_t mValueSize)
		{
			if (mIndex >= mData.size()) mData.resize(mIndex + 1);
			mData[mIndex] = std::vector<uint8_t>(mValueSize);
			memcpy(boost::any_cast<std::vector<uint8_t>>(&mData[mIndex])->data(), mValue, mValueSize);
		}

		void Record::SetNull(size_t mIndex)
		{
			if (mIndex >= mData.size()) mData.resize(mIndex + 1);
			mData[mIndex] = boost::any();
		}

		uint64_t Record::GetInteger(size_t mIndex) const
		{
			if (mIndex >= mData.size()) throw Exception("索引超出数组范围");
			if (mData[mIndex].type() == boost::typeindex::type_id<uint64_t>())
				return boost::any_cast<uint64_t>(mData[mIndex]);
			else if (mData[mIndex].type() == boost::typeindex::type_id<double>())
				return (uint64_t)boost::any_cast<double>(mData[mIndex]);
			throw Exception("类型错误");
		}

		double Record::GetFloat(size_t mIndex) const
		{
			if (mIndex >= mData.size()) throw Exception("索引超出数组范围");
			if (mData[mIndex].type() == boost::typeindex::type_id<double>())
				return boost::any_cast<double>(mData[mIndex]);
			else if (mData[mIndex].type() == boost::typeindex::type_id<uint64_t>())
				return (double)boost::any_cast<uint64_t>(mData[mIndex]);
			throw Exception("类型错误");
		}

		const String & Record::GetText(size_t mIndex) const
		{
			if (mIndex >= mData.size()) throw Exception("索引超出数组范围");
			if (mData[mIndex].type() != boost::typeindex::type_id<String>())
				throw Exception("类型错误");
			return *boost::any_cast<String>(&mData[mIndex]);
		}

		String & Record::GetText(size_t mIndex)
		{
			if (mIndex >= mData.size()) throw Exception("索引超出数组范围");
			if (mData[mIndex].type() != boost::typeindex::type_id<String>())
				throw Exception("类型错误");
			return *boost::any_cast<String>(&mData[mIndex]);
		}

		size_t Record::GetBlobSize(size_t mIndex) const
		{
			if (mIndex >= mData.size()) throw Exception("索引超出数组范围");
			if (mData[mIndex].type() != boost::typeindex::type_id<std::vector<uint8_t>>())
				throw Exception("类型错误");
			return boost::any_cast<std::vector<uint8_t>>(&mData[mIndex])->size();
		}

		void Record::ResizeBlob(size_t mIndex, size_t mSize)
		{
			if (mIndex >= mData.size()) mData.resize(mIndex + 1);
			if (mData[mIndex].empty()) mData[mIndex] = std::vector<uint8_t>(mSize);
			else
			{
				if (mData[mIndex].type() != boost::typeindex::type_id<std::vector<uint8_t>>())
					throw Exception("类型错误");
				boost::any_cast<std::vector<uint8_t>>(&mData[mIndex])->resize(mSize);
			}
		}

		uint8_t * Record::GetBlob(size_t mIndex)
		{
			if (mIndex >= mData.size()) throw Exception("索引超出数组范围");
			if (mData[mIndex].type() != boost::typeindex::type_id<std::vector<uint8_t>>())
				throw Exception("类型错误");
			return boost::any_cast<std::vector<uint8_t>>(&mData[mIndex])->data();
		}

		const uint8_t * Record::GetBlob(size_t mIndex) const
		{
			if (mIndex >= mData.size()) throw Exception("索引超出数组范围");
			if (mData[mIndex].type() != boost::typeindex::type_id<std::vector<uint8_t>>())
				throw Exception("类型错误");
			return boost::any_cast<std::vector<uint8_t>>(&mData[mIndex])->data();
		}
	}
}