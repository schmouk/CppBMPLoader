/*
MIT License

Copyright (c) 2024 Philippe Schmouker, ph.schmouker (at) gmail.com

Permission is hereby granted,  free of charge,  to any person obtaining a copy
of this software and associated documentation files (the "Software"),  to deal
in the Software without restriction,  including without limitation the  rights
to use,  copy,  modify,  merge,  publish,  distribute, sublicense, and/or sell
copies of the Software,  and  to  permit  persons  to  whom  the  Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS",  WITHOUT WARRANTY OF ANY  KIND,  EXPRESS  OR
IMPLIED,  INCLUDING  BUT  NOT  LIMITED  TO  THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT  SHALL  THE
AUTHORS  OR  COPYRIGHT  HOLDERS  BE  LIABLE  FOR  ANY CLAIM,  DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,  ARISING FROM,
OUT  OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


/*
* NOTICE: code here is implemented according to the c++14 standard.  It should
* function  as  well  when  compiled  with  standard  c++11  because  no c++14
* specificities have been used there, but it has not been tested as such.
*/

#include "bmp_file_header.h"
#include "../bmp_file_bitmaps/bmp_bitmaps.h"


namespace bmpl
{
    namespace frmt
    {
        //===========================================================================
        const BMPFileHeaderBase* create_file_header(bmpl::utils::LEInStream& in_stream, const bool from_ba_file) noexcept
        {
            std::int16_t file_type{ 0 };

            if (in_stream.failed())
                // file not found
                return nullptr;

            if ((in_stream >> file_type).failed())
                // streaming error
                return nullptr;

            switch (file_type) {
            case 0:  // i.e. BMP version 1.X
                return new BMPFileHeaderV1(in_stream);

            case 0x4142:  // i.e. 'BA' as little-endian encoded on 16 bits
                return new BMPFileHeaderBA(in_stream);

            case 0x4d42:  // i.e. "BM" as little-endian encoded on 16 bits
                return new BMPFileHeaderBM(in_stream, from_ba_file);

            default:
                // not a format decoded by library CppBMPLoader
                return nullptr;
            };
        }


        //===========================================================================
        const bool BMPFileHeaderBA::load(bmpl::utils::LEInStream& in_stream) noexcept
        {
            if (in_stream.failed() || (in_stream >> header_size >> offset_to_next >> screen_width >> screen_height).failed())
                return _set_err(in_stream.get_error());

            if (offset_to_next != 0 && offset_to_next < BMPFileHeaderBA::SIZE + 14 + 12)
                return _set_err(bmpl::utils::ErrorCode::INVALID_NEXT_OFFSET);

            if (offset_to_next > in_stream.get_size())
                return _set_err(bmpl::utils::ErrorCode::INVALID_NEXT_OFFSET_TOO_BIG);

            return _clr_err();
        }


        //===========================================================================
        const bool BMPFileHeaderBM::load(bmpl::utils::LEInStream& in_stream, const bool from_ba_file) noexcept
        {
            if (in_stream.failed())
                return _set_err(in_stream.get_error());

            if ((in_stream >> file_size >> reserved1 >> reserved2 >> content_offset).failed())
                return _set_err(in_stream.get_error());
            
            if (!from_ba_file && in_stream.get_size() != file_size)
                set_warning(bmpl::utils::WarningCode::BAD_FILE_SIZE_IN_HEADER);

            if (reserved1 != 0 || reserved2 != 0)
                set_warning(bmpl::utils::WarningCode::HOT_POINT_SET);

            if (content_offset >= in_stream.get_size())
                return _set_err(bmpl::utils::ErrorCode::INVALID_BITMAP_OFFSET);

            return _clr_err();
        }


        //===========================================================================
        const bool BMPFileHeaderV1::load(bmpl::utils::LEInStream& in_stream) noexcept
        {
            if (in_stream.failed())
                return _set_err(in_stream.get_error());

            if ((in_stream >> bitmap_width >> bitmap_height >> bitmap_bytes_width >> planes_count >> bits_per_pixel).failed())
                return _set_err(in_stream.get_error());

            if (bitmap_width < 0)
                return _set_err(bmpl::utils::ErrorCode::NEGATIVE_WIDTH);

            if (bitmap_height < 0)
                set_warning(bmpl::utils::WarningCode::FORBIDDEN_TOP_DOWN_ORIENTATION);

            if (planes_count != 1)
                set_warning(bmpl::utils::WarningCode::BAD_PLANES_VALUE);

            if (bits_per_pixel != 1 && bits_per_pixel != 4 && bits_per_pixel != 8)
                return _set_err(bmpl::utils::ErrorCode::BAD_BITS_PER_PIXEL_VALUE);

            const std::int16_t scanline_width{ bitmap_width / (8 / bits_per_pixel) };
            const std::int16_t padding{ std::int16_t(bmpl::bmpf::evaluate_padding(scanline_width)) };
            if (bitmap_bytes_width != scanline_width + padding)
                set_warning(bmpl::utils::WarningCode::BAD_SCANLINE_BYTES_WIDTH);

            return _clr_err();
        }

    }
}