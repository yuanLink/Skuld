#include <Skuld/Bitmap.h>
#include <memory>
#include <vector>
#include <Skuld/Exception.h>
#include <Skuld/Ptr.hpp>

namespace Skuld
{
	const uint32_t mPixelFormatDepths[] = {
		32,
		32,
		24,
		24,
		16,
		16
	};

	uint32_t PixelFormatDepth(PixelFormat format) { return mPixelFormatDepths[format]; }

	class InternalBitmap : public Bitmap
	{
	public:
		~InternalBitmap();
		PixelFormat mPixelFormat;
		uint32_t mWidth, mHeight;

		std::vector<uint8_t> mPixels;

		uint32_t GetWidth() const { return mWidth; }
		uint32_t GetHeight() const { return mHeight; }
		PixelFormat GetPixelFormat() const { return mPixelFormat; }

		Bitmap* Convert(PixelFormat format) const;

		const uint8_t *GetPixels() const { return mPixels.data(); }
		uint8_t *GetPixels() { return mPixels.data(); }

		Bitmap* Clone() const;

		inline size_t CalcSize() const {
			return (mWidth * mHeight * mPixelFormatDepths[mPixelFormat] + 7) / 8;
		}
	};

	static InternalBitmap* CreateBitmap(uint32_t width, uint32_t height, PixelFormat format)
	{
		Ptr<InternalBitmap> ret(new (std::nothrow) InternalBitmap());
		if (ret == nullptr) return nullptr;

		ret->mWidth = width;
		ret->mHeight = height;
		ret->mPixelFormat = format;

		ret->mPixels.resize(ret->CalcSize());
		return ret.Detach();
	}

	InternalBitmap::~InternalBitmap()
	{
	}

	template<PixelFormat mSrcFormat, PixelFormat mDstFormat>
	class PixelFormatConverter
	{
	public:
		static void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount);
	};

	template<>
	class PixelFormatConverter<PixelFormat_RGB_565, PixelFormat_RGB_565>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			memcpy(mDstBuffer, mSrcBuffer, mPixelCount * 2);
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_ARGB_1555, PixelFormat_ARGB_1555>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			memcpy(mDstBuffer, mSrcBuffer, mPixelCount * 2);
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_BGRA_8888, PixelFormat_BGRA_8888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			memcpy(mDstBuffer, mSrcBuffer, mPixelCount * 4);
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_RGBA_8888, PixelFormat_RGBA_8888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			memcpy(mDstBuffer, mSrcBuffer, mPixelCount * 4);
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_RGB_888, PixelFormat_RGB_888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			memcpy(mDstBuffer, mSrcBuffer, mPixelCount * 3);
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_BGR_888, PixelFormat_BGR_888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			memcpy(mDstBuffer, mSrcBuffer, mPixelCount * 3);
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_BGRA_8888, PixelFormat_RGBA_8888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			for (size_t i = 0; i < mPixelCount; i++) {
				mDstBuffer[i * 4 + 3] = mSrcBuffer[i * 4 + 3];
				mDstBuffer[i * 4 + 1] = mSrcBuffer[i * 4 + 1];
				mDstBuffer[i * 4 + 2] = mSrcBuffer[i * 4];
				mDstBuffer[i * 4] = mSrcBuffer[i * 4 + 2];
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_RGBA_8888, PixelFormat_BGRA_8888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			for (size_t i = 0; i < mPixelCount; i++) {
				mDstBuffer[i * 4 + 3] = mSrcBuffer[i * 4 + 3];
				mDstBuffer[i * 4 + 1] = mSrcBuffer[i * 4 + 1];
				mDstBuffer[i * 4 + 2] = mSrcBuffer[i * 4];
				mDstBuffer[i * 4] = mSrcBuffer[i * 4 + 2];
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_RGB_888, PixelFormat_BGR_888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			for (size_t i = 0; i < mPixelCount; i++) {
				mDstBuffer[i * 3] = mSrcBuffer[i * 3 + 2];
				mDstBuffer[i * 3 + 1] = mSrcBuffer[i * 3 + 1];
				mDstBuffer[i * 3 + 2] = mSrcBuffer[i * 3];
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_BGR_888, PixelFormat_RGB_888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			for (size_t i = 0; i < mPixelCount; i++) {
				mDstBuffer[i * 3] = mSrcBuffer[i * 3 + 2];
				mDstBuffer[i * 3 + 1] = mSrcBuffer[i * 3 + 1];
				mDstBuffer[i * 3 + 2] = mSrcBuffer[i * 3];
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_RGBA_8888, PixelFormat_RGB_888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			for (size_t i = 0; i < mPixelCount; i++) {
				mDstBuffer[i * 3] = mSrcBuffer[i * 4];
				mDstBuffer[i * 3 + 1] = mSrcBuffer[i * 4 + 1];
				mDstBuffer[i * 3 + 2] = mSrcBuffer[i * 4 + 2];
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_RGB_888, PixelFormat_RGBA_8888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			for (size_t i = 0; i < mPixelCount; i++) {
				mDstBuffer[i * 4] = mSrcBuffer[i * 3];
				mDstBuffer[i * 4 + 1] = mSrcBuffer[i * 3 + 1];
				mDstBuffer[i * 4 + 2] = mSrcBuffer[i * 3 + 2];
				mDstBuffer[i * 4 + 3] = 0xff;
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_BGRA_8888, PixelFormat_BGR_888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			for (size_t i = 0; i < mPixelCount; i++) {
				mDstBuffer[i * 3] = mSrcBuffer[i * 4];
				mDstBuffer[i * 3 + 1] = mSrcBuffer[i * 4 + 1];
				mDstBuffer[i * 3 + 2] = mSrcBuffer[i * 4 + 2];
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_BGR_888, PixelFormat_BGRA_8888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			for (size_t i = 0; i < mPixelCount; i++) {
				mDstBuffer[i * 4] = mSrcBuffer[i * 3];
				mDstBuffer[i * 4 + 1] = mSrcBuffer[i * 3 + 1];
				mDstBuffer[i * 4 + 2] = mSrcBuffer[i * 3 + 2];
				mDstBuffer[i * 4 + 3] = 0xff;
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_BGRA_8888, PixelFormat_RGB_888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			for (size_t i = 0; i < mPixelCount; i++) {
				mDstBuffer[i * 3] = mSrcBuffer[i * 4 + 2];
				mDstBuffer[i * 3 + 1] = mSrcBuffer[i * 4 + 1];
				mDstBuffer[i * 3 + 2] = mSrcBuffer[i * 4];
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_RGB_888, PixelFormat_BGRA_8888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			for (size_t i = 0; i < mPixelCount; i++) {
				mDstBuffer[i * 4] = mSrcBuffer[i * 3 + 2];
				mDstBuffer[i * 4 + 1] = mSrcBuffer[i * 3 + 1];
				mDstBuffer[i * 4 + 2] = mSrcBuffer[i * 3];
				mDstBuffer[i * 4 + 3] = 0xff;
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_RGBA_8888, PixelFormat_BGR_888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			for (size_t i = 0; i < mPixelCount; i++) {
				mDstBuffer[i * 3] = mSrcBuffer[i * 4 + 2];
				mDstBuffer[i * 3 + 1] = mSrcBuffer[i * 4 + 1];
				mDstBuffer[i * 3 + 2] = mSrcBuffer[i * 4];
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_BGR_888, PixelFormat_RGBA_8888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			for (size_t i = 0; i < mPixelCount; i++) {
				mDstBuffer[i * 4] = mSrcBuffer[i * 3 + 2];
				mDstBuffer[i * 4 + 1] = mSrcBuffer[i * 3 + 1];
				mDstBuffer[i * 4 + 2] = mSrcBuffer[i * 3];
				mDstBuffer[i * 4 + 3] = 0xff;
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_RGBA_8888, PixelFormat_RGB_565>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			uint16_t* _mDstBuffer = reinterpret_cast<uint16_t*>(mDstBuffer);

			for (size_t i = 0; i < mPixelCount; i++)
			{
				_mDstBuffer[i] = static_cast<uint16_t>(
					((mSrcBuffer[i * 4] >> 3) << 11) |
					((mSrcBuffer[i * 4 + 1] >> 2) << 5) |
					((mSrcBuffer[i * 4 + 2] >> 3)));
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_BGRA_8888, PixelFormat_RGB_565>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			uint16_t* _mDstBuffer = reinterpret_cast<uint16_t*>(mDstBuffer);

			for (size_t i = 0; i < mPixelCount; i++)
			{
				_mDstBuffer[i] = static_cast<uint16_t>(
					((mSrcBuffer[i * 4 + 2] >> 3) << 11) |
					((mSrcBuffer[i * 4 + 1] >> 2) << 5) |
					((mSrcBuffer[i * 4] >> 3)));
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_RGB_888, PixelFormat_RGB_565>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			uint16_t* _mDstBuffer = reinterpret_cast<uint16_t*>(mDstBuffer);

			for (size_t i = 0; i < mPixelCount; i++)
			{
				_mDstBuffer[i] = static_cast<uint16_t>(
					((mSrcBuffer[i * 3] >> 3) << 11) |
					((mSrcBuffer[i * 3 + 1] >> 2) << 5) |
					((mSrcBuffer[i * 3 + 2] >> 3)));
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_BGR_888, PixelFormat_RGB_565>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			uint16_t* _mDstBuffer = reinterpret_cast<uint16_t*>(mDstBuffer);

			for (size_t i = 0; i < mPixelCount; i++)
			{
				_mDstBuffer[i] = static_cast<uint16_t>(
					((mSrcBuffer[i * 3 + 2] >> 3) << 11) |
					((mSrcBuffer[i * 3 + 1] >> 2) << 5) |
					((mSrcBuffer[i * 3] >> 3)));
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_RGB_565, PixelFormat_BGRA_8888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			const uint16_t* _mSrcBuffer = reinterpret_cast<const uint16_t*>(mSrcBuffer);

			for (size_t i = 0; i < mPixelCount; i++)
			{
				mDstBuffer[i * 4 + 2] = (_mSrcBuffer[i] & 0xF800) >> 8;
				mDstBuffer[i * 4 + 1] = (_mSrcBuffer[i] & 0x07E0) >> 3;
				mDstBuffer[i * 4] = (_mSrcBuffer[i] & 0x001F) << 3;
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_RGB_565, PixelFormat_RGBA_8888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			const uint16_t* _mSrcBuffer = reinterpret_cast<const uint16_t*>(mSrcBuffer);

			for (size_t i = 0; i < mPixelCount; i++)
			{
				mDstBuffer[i * 4] = (_mSrcBuffer[i] & 0xF800) >> 8;
				mDstBuffer[i * 4 + 1] = (_mSrcBuffer[i] & 0x07E0) >> 3;
				mDstBuffer[i * 4 + 2] = (_mSrcBuffer[i] & 0x001F) << 3;
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_RGB_565, PixelFormat_BGR_888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			const uint16_t* _mSrcBuffer = reinterpret_cast<const uint16_t*>(mSrcBuffer);

			for (size_t i = 0; i < mPixelCount; i++)
			{
				mDstBuffer[i * 3 + 2] = (_mSrcBuffer[i] & 0xF800) >> 8;
				mDstBuffer[i * 3 + 1] = (_mSrcBuffer[i] & 0x07E0) >> 3;
				mDstBuffer[i * 3] = (_mSrcBuffer[i] & 0x001F) << 3;
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_RGB_565, PixelFormat_RGB_888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			const uint16_t* _mSrcBuffer = reinterpret_cast<const uint16_t*>(mSrcBuffer);

			for (size_t i = 0; i < mPixelCount; i++)
			{
				mDstBuffer[i * 3] = (_mSrcBuffer[i] & 0xF800) >> 8;
				mDstBuffer[i * 3 + 1] = (_mSrcBuffer[i] & 0x07E0) >> 3;
				mDstBuffer[i * 3 + 2] = (_mSrcBuffer[i] & 0x001F) << 3;
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_RGBA_8888, PixelFormat_ARGB_1555>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			uint16_t* _mDstBuffer = reinterpret_cast<uint16_t*>(mDstBuffer);

			for (size_t i = 0; i < mPixelCount; i++)
			{
				_mDstBuffer[i] = static_cast<uint16_t>(
					(mSrcBuffer[i * 4 + 3] > 0 ? 0x8000 : 0x0000) |
					((mSrcBuffer[i * 4] >> 3) << 10) |
					((mSrcBuffer[i * 4 + 1] >> 3) << 5) |
					((mSrcBuffer[i * 4 + 2] >> 3)));
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_BGRA_8888, PixelFormat_ARGB_1555>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			uint16_t* _mDstBuffer = reinterpret_cast<uint16_t*>(mDstBuffer);

			for (size_t i = 0; i < mPixelCount; i++)
			{
				_mDstBuffer[i] = static_cast<uint16_t>(
					(mSrcBuffer[i * 4 + 3] > 0 ? 0x8000 : 0x0000) |
					((mSrcBuffer[i * 4 + 2] >> 3) << 10) |
					((mSrcBuffer[i * 4 + 1] >> 3) << 5) |
					((mSrcBuffer[i * 4] >> 3)));
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_RGB_888, PixelFormat_ARGB_1555>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			uint16_t* _mDstBuffer = reinterpret_cast<uint16_t*>(mDstBuffer);

			for (size_t i = 0; i < mPixelCount; i++)
			{
				_mDstBuffer[i] = static_cast<uint16_t>(
					0x8000 |
					((mSrcBuffer[i * 3] >> 3) << 11) |
					((mSrcBuffer[i * 3 + 1] >> 2) << 5) |
					((mSrcBuffer[i * 3 + 2] >> 3)));
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_BGR_888, PixelFormat_ARGB_1555>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			uint16_t* _mDstBuffer = reinterpret_cast<uint16_t*>(mDstBuffer);

			for (size_t i = 0; i < mPixelCount; i++)
			{
				_mDstBuffer[i] = static_cast<uint16_t>(
					0x8000 |
					((mSrcBuffer[i * 3 + 2] >> 3) << 11) |
					((mSrcBuffer[i * 3 + 1] >> 2) << 5) |
					((mSrcBuffer[i * 3] >> 3)));
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_ARGB_1555, PixelFormat_BGRA_8888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			const uint16_t* _mSrcBuffer = reinterpret_cast<const uint16_t*>(mSrcBuffer);

			for (size_t i = 0; i < mPixelCount; i++)
			{
				mDstBuffer[i * 4 + 3] = (_mSrcBuffer[i] & 0x8000) > 0 ? 0xff : 0;
				mDstBuffer[i * 4 + 2] = (_mSrcBuffer[i] & 0x7C00) >> 8;
				mDstBuffer[i * 4 + 1] = (_mSrcBuffer[i] & 0x03E0) >> 3;
				mDstBuffer[i * 4] = (_mSrcBuffer[i] & 0x001F) << 3;
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_ARGB_1555, PixelFormat_RGBA_8888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			const uint16_t* _mSrcBuffer = reinterpret_cast<const uint16_t*>(mSrcBuffer);

			for (size_t i = 0; i < mPixelCount; i++)
			{
				mDstBuffer[i * 4 + 3] = (_mSrcBuffer[i] & 0x8000) > 0 ? 0xff : 0;
				mDstBuffer[i * 4] = (_mSrcBuffer[i] & 0x7C00) >> 8;
				mDstBuffer[i * 4 + 1] = (_mSrcBuffer[i] & 0x03E0) >> 3;
				mDstBuffer[i * 4 + 2] = (_mSrcBuffer[i] & 0x001F) << 3;
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_ARGB_1555, PixelFormat_BGR_888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			const uint16_t* _mSrcBuffer = reinterpret_cast<const uint16_t*>(mSrcBuffer);

			for (size_t i = 0; i < mPixelCount; i++)
			{
				mDstBuffer[i * 3 + 2] = (_mSrcBuffer[i] & 0x7C00) >> 8;
				mDstBuffer[i * 3 + 1] = (_mSrcBuffer[i] & 0x03E0) >> 3;
				mDstBuffer[i * 3] = (_mSrcBuffer[i] & 0x001F) << 3;
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_ARGB_1555, PixelFormat_RGB_888>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			const uint16_t* _mSrcBuffer = reinterpret_cast<const uint16_t*>(mSrcBuffer);

			for (size_t i = 0; i < mPixelCount; i++)
			{
				mDstBuffer[i * 3] = (_mSrcBuffer[i] & 0x7C00) >> 8;
				mDstBuffer[i * 3 + 1] = (_mSrcBuffer[i] & 0x03E0) >> 3;
				mDstBuffer[i * 3 + 2] = (_mSrcBuffer[i] & 0x001F) << 3;
			}
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_ARGB_1555, PixelFormat_RGB_565>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			const uint16_t* _mSrcBuffer = reinterpret_cast<const uint16_t*>(mSrcBuffer);
			uint16_t* _mDstBuffer = reinterpret_cast<uint16_t*>(mDstBuffer);

			for (size_t i = 0; i < mPixelCount; i++)
				_mDstBuffer[i] = (_mSrcBuffer[i] & 0x001F) | ((_mSrcBuffer[i] & 0x7FE0) << 1);
		}
	};

	template<>
	class PixelFormatConverter<PixelFormat_RGB_565, PixelFormat_ARGB_1555>
	{
	public:
		static inline void __ConvertBuffer(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer,
			size_t mPixelCount)
		{
			const uint16_t* _mSrcBuffer = reinterpret_cast<const uint16_t*>(mSrcBuffer);
			uint16_t* _mDstBuffer = reinterpret_cast<uint16_t*>(mDstBuffer);

			for (size_t i = 0; i < mPixelCount; i++)
				_mDstBuffer[i] = (_mSrcBuffer[i] & 0x001F) | ((_mSrcBuffer[i] & 0x7FE0) >> 1) | 0x8000;
		}
	};

	template<PixelFormat mSrcFormat>
	inline void _Convert(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer, size_t mPixelCount,
		PixelFormat mDstFormat) {
#define SKULD_CONVERT(_format) \
		case _format:PixelFormatConverter<mSrcFormat, _format>::__ConvertBuffer(mDstBuffer, mSrcBuffer, mPixelCount);break

		switch (mDstFormat)
		{
			SKULD_CONVERT(PixelFormat_BGRA_8888);
			SKULD_CONVERT(PixelFormat_RGBA_8888);
			SKULD_CONVERT(PixelFormat_RGB_888);
			SKULD_CONVERT(PixelFormat_BGR_888);

			SKULD_CONVERT(PixelFormat_RGB_565);
			SKULD_CONVERT(PixelFormat_ARGB_1555);
		default:
			break;
		}

#undef SKULD_CONVERT
	}

	static void _ConvertPixelFormat(uint8_t* mDstBuffer, const uint8_t* mSrcBuffer, size_t mPixelCount,
		PixelFormat mSrcFormat, PixelFormat mDstFormat)
	{
#define SKULD_CONVERT(_format) \
		case _format:_Convert<_format>(mDstBuffer, mSrcBuffer, mPixelCount, mDstFormat);break

		switch (mSrcFormat)
		{
			SKULD_CONVERT(PixelFormat_BGRA_8888);
			SKULD_CONVERT(PixelFormat_RGBA_8888);
			SKULD_CONVERT(PixelFormat_RGB_888);
			SKULD_CONVERT(PixelFormat_BGR_888);

			SKULD_CONVERT(PixelFormat_RGB_565);
			SKULD_CONVERT(PixelFormat_ARGB_1555);
		default:
			break;
		}

#undef SKULD_CONVERT
	}

	Bitmap * InternalBitmap::Convert(PixelFormat format) const
	{
		Ptr<InternalBitmap> ret = Skuld::CreateBitmap(mWidth, mHeight, format);
		size_t size = mWidth * mHeight;

		_ConvertPixelFormat(ret->mPixels.data(), mPixels.data(), size, mPixelFormat, format);

		if (ret)
		{
			if (ret->GetPixelFormat() != format)
				throw Exception("转换编码失败");
			return ret.Detach();
		}
		return nullptr;
	}

	Bitmap * InternalBitmap::Clone() const
	{
		InternalBitmap* ret = Skuld::CreateBitmap(mWidth, mHeight, mPixelFormat);
		memcpy(ret->mPixels.data(), mPixels.data(), CalcSize());
		return ret;
	}

	Bitmap * Bitmap::CreateBitmap(uint32_t width, uint32_t height, PixelFormat format)
	{
		return Skuld::CreateBitmap(width, height, format);
	}
}