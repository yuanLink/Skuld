#include "BitmapEncoder.h"
#include <jpeglib.h>
#include <jerror.h>
#include <setjmp.h>
#include <Skuld/Ptr.hpp>

namespace Skuld
{
	namespace Codec
	{
		class BitmapEncoderJpeg : public BitmapEncoder
		{
			void Encode(const Bitmap* bmp, Stream* mStream);
			BitmapFormat Format() { return BitmapFormat_JPEG; }
		};

		std::shared_ptr<BitmapEncoder> CreateBitmapEncoderJpeg() {
			return std::make_shared<BitmapEncoderJpeg>();
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
				jerror_mgr(jpeg_compress_struct& cinfo) {
					cinfo.err = jpeg_std_error(this);
					error_exit = jerror_exit;
					output_message = joutput_message;
				}
			};

			struct stream_jpeg_dest_mgr : jpeg_destination_mgr
			{
				Stream* mStream;
				JOCTET mBuffer[1024];
			};

			static boolean jpeg_stream_empty_output(j_compress_ptr cinfo)
			{
				stream_jpeg_dest_mgr* dest = (stream_jpeg_dest_mgr*)cinfo->dest;

				if (!dest->mStream->Write(dest->mBuffer, 1024))
				{
					ERREXIT(cinfo, JERR_FILE_WRITE);
					return FALSE;
				}

				dest->next_output_byte = dest->mBuffer;
				dest->free_in_buffer = 1024;
				return TRUE;
			}

			static void jpeg_stream_write(j_compress_ptr cinfo)
			{
				stream_jpeg_dest_mgr* dest = (stream_jpeg_dest_mgr*)cinfo->dest;

				size_t size = 1024 - dest->free_in_buffer;
				if (size > 0) {
					if (!dest->mStream->Write(dest->mStream, (uint32_t)size)) {
						ERREXIT(cinfo, JERR_FILE_WRITE);
						return;
					}
				}
				dest->mStream->Flush();
			}

			void jpeg_stream_dest(j_compress_ptr dinfo, Stream* stream)
			{
				stream_jpeg_dest_mgr* dest;
				dinfo->dest = (struct jpeg_destination_mgr *)
					(*dinfo->mem->alloc_small) ((j_common_ptr)dinfo, JPOOL_PERMANENT,
						sizeof(stream_jpeg_dest_mgr));

				dest = (stream_jpeg_dest_mgr*)dinfo->dest;
				dest->next_output_byte = dest->mBuffer;
				dest->mStream = stream;
				dest->free_in_buffer = 1024;
				dest->init_destination = [](j_compress_ptr) {};
				dest->empty_output_buffer = jpeg_stream_empty_output;
				dest->term_destination = jpeg_stream_write;
			}
		}

		void BitmapEncoderJpeg::Encode(const Bitmap * bmp2, Stream * mStream)
		{
			Ptr<Bitmap> bmp(bmp2->Convert(PixelFormat_RGB_888));
			jpeg_compress_struct cinfo;
			jerror_mgr cerr(cinfo);
			jpeg_create_compress(&cinfo);

			PixelFormat mPixelFormat = bmp->GetPixelFormat();
			
			jpeg_stream_dest(&cinfo, mStream);
			cinfo.image_width = static_cast<JDIMENSION>(bmp->GetWidth());
			cinfo.image_height = static_cast<JDIMENSION>(bmp->GetHeight());
			cinfo.input_components = PixelFormatDepth(mPixelFormat) / 8;
			cinfo.in_color_space = J_COLOR_SPACE::JCS_RGB;

			jpeg_set_defaults(&cinfo);
			jpeg_set_quality(&cinfo, 100, TRUE);

			jpeg_start_compress(&cinfo, TRUE);

			size_t mStride = PixelFormatDepth(mPixelFormat) / 8 * cinfo.image_width;
			uint8_t* mBuffer = const_cast<uint8_t*>(bmp->GetPixels());

			JSAMPROW row;
			while (cinfo.next_scanline < cinfo.image_height)
			{
				row = (JSAMPROW)&mBuffer[cinfo.next_scanline * mStride];
				jpeg_write_scanlines(&cinfo, &row, 1);
			}
			jpeg_finish_compress(&cinfo);
			jpeg_destroy_compress(&cinfo);
		}
	}
}