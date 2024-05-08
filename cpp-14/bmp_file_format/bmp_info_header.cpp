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

#include "bmp_info_header.h"


namespace bmpl
{
    namespace frmt
    {
        const bool BMPInfoHeader::load(bmpl::utils::LEInStream& in_stream) noexcept
        {
            if (in_stream.failed())
                return _set_err(in_stream.get_error());

            if (!(in_stream >> size))
                return _set_err(in_stream.get_error());

            if (!(in_stream >> width
                            >> height
                            >> planes_count
                            >> bits_per_pixel
                            >> compression_mode
                            >> image_size
                            >> device_x_resolution
                            >> device_y_resolution
                            >> used_colors_count
                            >> important_colors_count))
                return _set_err(in_stream.get_error());

            if (planes_count != 1)
                _set_warning(bmpl::utils::WarningCode::BAD_PLANES_VALUE);

            if (compression_mode > 3)
                return _set_err(bmpl::utils::ErrorCode::BMP_BAD_ENCODING);

            if (image_size == 0 && compression_mode != 0)
                return _set_err(bmpl::utils::ErrorCode::BMP_BAD_ENCODING);

            if (size != 0 && in_stream.get_size() != size)
                _set_warning(bmpl::utils::WarningCode::BAD_FILE_SIZE_IN_HEADER);

            if (size == 0x28) {
                // BMP 3 format - Windows 3.x or NT
                if (compression_mode == this->RLE_COLOR_BITMASKS) {
                    // color bitmasks are provided - Windows NT format
                    if (bits_per_pixel != 16 && bits_per_pixel != 32)
                        return _set_err(bmpl::utils::ErrorCode::BAD_BITS_PER_PIXEL_VALUE);
                    if (!(in_stream >> red_mask >> green_mask >> blue_mask))
                        return _set_err(in_stream.get_error());
                }
                else {
                    // checks pixels depth in bits count
                    if (bits_per_pixel != 1 && bits_per_pixel != 4 && bits_per_pixel != 8 && bits_per_pixel != 24)
                        return _set_err(bmpl::utils::ErrorCode::BAD_BITS_PER_PIXEL_VALUE);
                }
            }
            else if (size == 0x6c) {
                // BMP 4 format - Windows 95 and above
                if (!(in_stream >> red_mask
                                >> green_mask
                                >> blue_mask
                                >> alpha_mask
                                >> cs_type
                                >> red_endX
                                >> red_endY
                                >> red_endZ
                                >> green_endX
                                >> green_endY
                                >> green_endZ
                                >> blue_endX
                                >> blue_endY
                                >> blue_endZ
                                >> gamma_red
                                >> gamma_green
                                >> gamma_blue))
                    return _set_err(in_stream.get_error());

                if ((red_mask & green_mask & blue_mask & alpha_mask) != 0)
                    return _set_err(bmpl::utils::ErrorCode::OVERLAPPING_BITFIELD_MASKS);
            }
            else {
                return _set_err(bmpl::utils::ErrorCode::NOT_WINDOWS_BMP);
            }


            if (bits_per_pixel != 24) {
                if (used_colors_count == 0)
                    used_colors_count = 1 << bits_per_pixel;
                else if (used_colors_count > std::uint32_t(1 << bits_per_pixel))
                    return _set_err(bmpl::utils::ErrorCode::BMP_BAD_ENCODING);

                if (important_colors_count > used_colors_count)
                    return _set_err(bmpl::utils::ErrorCode::BMP_BAD_ENCODING);
            }
            else {
                if (used_colors_count != 0)
                    return _set_err(bmpl::utils::ErrorCode::BMP_BAD_ENCODING);
            }

            return _clr_err();
        }

    }
}