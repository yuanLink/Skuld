#include "BitmapDecoder.h"
#include <jpeglib.h>
#include <jerror.h>
#include <setjmp.h>
#include <Skuld/Ptr.hpp>
#include <memory>

namespace Skuld
{
	namespace Codec
	{
		class BitmapDecoderJpeg : public BitmapDecoder
		{
		public:
			virtual Bitmap* Decode(Stream* stream, uint32_t mBitmapIndex);
			virtual bool Test(Stream* stream);
		};

		std::shared_ptr<BitmapDecoder> CreateBitmapDecoderJpeg() {
			return std::make_shared<BitmapDecoderJpeg>();
		}

		namespace {
			struct jerror_mgr : jpeg_error_mgr
			{
				jmp_buf jmp;
				static void jerror_exit(j_common_ptr jinfo)
				{
					jerror_mgr* err = (jerror_mgr*)jinfo->err;
					longjmp(err->jmp, 1);
				}
				static void joutput_message(j_common_ptr) {}
				jerror_mgr(jpeg_decompress_struct& cinfo) {
					cinfo.err = jpeg_std_error(this);
					error_exit = jerror_exit;
					output_message = joutput_message;
				}
			};

			struct stream_jpeg_source_mgr : jpeg_source_mgr
			{
				Stream* mStream;
				uint8_t mBuffer[1024];
			};

			static void jpeg_stream_seek(j_decompress_ptr dinfo, long numBytes)
			{
				stream_jpeg_source_mgr* src = (stream_jpeg_source_mgr*)dinfo->src;
				size_t bytes = (size_t)numBytes;

				if (bytes > src->bytes_in_buffer) {
					size_t bytesToSkip = bytes - src->bytes_in_buffer;
					if (bytesToSkip != (size_t)src->mStream->Offset((int32_t)bytesToSkip)) {
						dinfo->err->error_exit((j_common_ptr)dinfo);
						return;
					}

					src->next_input_byte = (const JOCTET*)src->mBuffer;
					src->bytes_in_buffer = 0;
				}
				else {
					src->next_input_byte += numBytes;
					src->bytes_in_buffer -= numBytes;
				}
			}

			static boolean jpeg_stream_read(j_decompress_ptr dinfo)
			{
				stream_jpeg_source_mgr* src = (stream_jpeg_source_mgr*)dinfo->src;
				size_t bytes = src->mStream->Read(src->mBuffer, 1024);

				if (bytes == 0) {
					return FALSE;
				}

				src->next_input_byte = (const JOCTET*)src->mBuffer;
				src->bytes_in_buffer = bytes;
				return TRUE;
			}

			void jpeg_stream_src(j_decompress_ptr cinfo, Stream* mStream)
			{
				struct stream_jpeg_source_mgr * src;
				if (mStream == nullptr)
					ERREXIT(cinfo, JERR_INPUT_EMPTY);

				src = (struct stream_jpeg_source_mgr *)
					(*cinfo->mem->alloc_small) ((j_common_ptr)cinfo, JPOOL_PERMANENT,
						sizeof(struct stream_jpeg_source_mgr));

				cinfo->src = src;
				src->init_source = [](j_decompress_ptr) {};
				src->fill_input_buffer = jpeg_stream_read;
				src->skip_input_data = jpeg_stream_seek;
				src->resync_to_restart = jpeg_resync_to_restart;
				src->term_source = [](j_decompress_ptr) {};
				src->mStream = mStream;
				src->bytes_in_buffer = 0;
				src->next_input_byte = (const JOCTET *)src->mBuffer;
			}
		}

		Bitmap * BitmapDecoderJpeg::Decode(Stream * stream, uint32_t mBitmapIndex)
		{
			jpeg_decompress_struct cinfo;
			jerror_mgr mJErrorMgr(cinfo);
			jpeg_create_decompress(&cinfo);

			if (setjmp(mJErrorMgr.jmp))
			{
				jpeg_destroy_decompress(&cinfo);
				return nullptr;
			}

			jpeg_stream_src(&cinfo, stream);

			if (jpeg_read_header(&cinfo, TRUE) != JPEG_HEADER_OK)
			{
				jpeg_destroy_decompress(&cinfo);
				return nullptr;
			}
			
			PixelFormat mPixelFormat = PixelFormat_RGB_888;

			cinfo.out_color_space = JCS_RGB;

			if (!jpeg_start_decompress(&cinfo))
			{
				jpeg_destroy_decompress(&cinfo);
				return nullptr;
			}

			Ptr<Bitmap> img(Bitmap::CreateBitmap(
				static_cast<uint32_t>(cinfo.image_width), 
				static_cast<uint32_t>(cinfo.image_height), mPixelFormat));
			size_t mStride = static_cast<size_t>(cinfo.image_width * PixelFormatDepth(mPixelFormat) / 8);

			JSAMPARRAY mTempBuffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, 
				static_cast<JDIMENSION>(mStride), 
				1);
			uint8_t *mBuffer = img->GetPixels();

			uint32_t index  = 0;

			while (cinfo.output_scanline < cinfo.output_height)
			{
				jpeg_read_scanlines(&cinfo, mTempBuffer, 1);
				memcpy(&mBuffer[(cinfo.output_scanline - 1) * mStride], mTempBuffer[0], mStride);
				index++;
			}

			jpeg_finish_decompress(&cinfo);

			jpeg_destroy_decompress(&cinfo);

			return img.Detach();
		}

		bool BitmapDecoderJpeg::Test(Stream * stream)
		{
			const uint8_t flag[] = { 0xff, 0xd8, 0xff };
			char f[4];
			if (stream->Peek(f, 3) == 3)
				return (memcmp(f, flag, 3) == 0);
			else return false;
		}
	}
}