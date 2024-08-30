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

#include <cmath>

#include "bmp_info_header.h"


namespace bmpl
{
    namespace frmt
    {
        //===========================================================================
        const bool BMPInfoHeaderV2::load(bmpl::utils::LEInStream& in_stream) noexcept
        {
            if (!(in_stream >> width >> height >> planes_count >> bits_per_pixel))
                return _set_err(in_stream.get_error());

            if (width < 0)
                return _set_err(bmpl::utils::ErrorCode::NEGATIVE_WIDTH);

            if (width == 0 || height == 0)
                return _set_err(bmpl::utils::ErrorCode::INVALID_IMAGE_DIMENSIONS);

            if (height < 0) {
                // top-down encoding
                height = -height;
                top_down_encoding = true;
            }

            if (planes_count != 1)
                _set_warning(bmpl::utils::WarningCode::BAD_PLANES_VALUE);

            return _clr_err();
        }


        //===========================================================================
        const bool BMPInfoHeaderV3::load(bmpl::utils::LEInStream& in_stream, const bool is_V3_base, const bool is_V5_base) noexcept
        {

            if (!(in_stream >> width
                            >> height
                            >> planes_count
                            >> bits_per_pixel
                            >> compression_mode
                            >> bitmap_size
                            >> device_x_resolution
                            >> device_y_resolution
                            >> used_colors_count
                            >> important_colors_count))
                return _set_err(in_stream.get_error());

            if (width < 0)
                return _set_err(bmpl::utils::ErrorCode::NEGATIVE_WIDTH);

            if (width == 0 || height == 0)
                return _set_err(bmpl::utils::ErrorCode::INVALID_IMAGE_DIMENSIONS);

            if (height < 0) {
                // top-down encoding
                height = -height;
                top_down_encoding = true;

                if (compression_mode != COMPR_NO_RLE)
                    _set_warning(bmpl::utils::WarningCode::FORBIDDEN_TOP_DOWN_ORIENTATION);
                else if (is_V5_base && (compression_mode == COMPR_EMBEDS_JPEG || compression_mode == COMPR_EMBEDS_PNG))
                    return _set_err(bmpl::utils::ErrorCode::FORBIDDEN_BOTTOM_UP_ORIENTATION);
            }

            if (device_x_resolution > 2.5 * device_y_resolution || device_y_resolution > 2.5 * device_x_resolution)
                // notice: factor 2.5 is per pure convention and has nothing to do with the BMP format
                _set_warning(bmpl::utils::WarningCode::INCOHERENT_RESOLUTIONS);

            if (planes_count != 1)
                _set_warning(bmpl::utils::WarningCode::BAD_PLANES_VALUE);

            if (is_V3_base && compression_mode > COMPR_RLE_4)
                return _set_err(bmpl::utils::ErrorCode::BMP_BAD_ENCODING);

            if (is_V5_base) {
                if (compression_mode == COMPR_EMBEDS_JPEG)
                    return _set_err(bmpl::utils::ErrorCode::NOT_YET_IMPLEMENTED_JPEG_DECODING);
                else if (compression_mode == COMPR_EMBEDS_PNG)
                    return _set_err(bmpl::utils::ErrorCode::NOT_YET_IMPLEMENTED_PNG_DECODING);
            }

            if (bitmap_size == 0 && compression_mode != COMPR_NO_RLE)
                return _set_err(bmpl::utils::ErrorCode::BMP_BAD_ENCODING);

            if (bits_per_pixel != 1 && bits_per_pixel != 4 &&
                bits_per_pixel != 8 && bits_per_pixel != 24 && bits_per_pixel != 64)
            {
                if (bits_per_pixel == 2)
                    _set_warning(bmpl::utils::WarningCode::WIN_CE_2_BITS_PIXELS);
                else if (is_V3_base || (bits_per_pixel != 16 && bits_per_pixel != 32))
                    return _set_err(bmpl::utils::ErrorCode::BAD_BITS_PER_PIXEL_VALUE);
            }

            if (bits_per_pixel != 24) {
                if (important_colors_count > used_colors_count)
                    _set_warning(bmpl::utils::WarningCode::BAD_IMPORTANT_COLORS_COUNT);
            }

            if (bits_per_pixel > 64)
                return _set_err(bmpl::utils::ErrorCode::TOO_BIG_BITS_PER_PIXEL_VALUE);

            if (is_V3_base && bits_per_pixel != 1 && bits_per_pixel != 2 && bits_per_pixel != 4 && bits_per_pixel != 8) {
                if (used_colors_count != 0)
                    _set_warning(bmpl::utils::WarningCode::UNUSED_PALETTE);
            }

            if (used_colors_count == 0 && bits_per_pixel <= 8)  //< 24)
                used_colors_count = 256;

            return _clr_err();
        }


        //===========================================================================
        const bool BMPInfoHeaderV3_NT::load(bmpl::utils::LEInStream& in_stream, const bool is_V4_base) noexcept
        {
            if (failed())
                return false;

            if (compression_mode == COMPR_RLE_COLOR_BITMASKS ||
                compression_mode == COMPR_ALPHABITFIELDS ||
                is_V4_base)
            {
                if (!(in_stream >> red_mask >> green_mask >> blue_mask))
                    return _set_err(in_stream.get_error());

                if (compression_mode == COMPR_ALPHABITFIELDS)
                    if (!(in_stream >> alpha_mask))
                        return _set_err(in_stream.get_error());
            }
            else {
                if (bits_per_pixel == 16) {
                    alpha_mask = 0x8000;
                    red_mask = 0x7c00;
                    green_mask = 0x03e0;
                    blue_mask = 0x001f;
                }
                else if (bits_per_pixel == 32) {
                    alpha_mask = 0xff00'0000;
                    red_mask   = 0x00ff'0000;
                    green_mask = 0x0000'ff00;
                    blue_mask  = 0x0000'00ff;
                }
            }

            if ((red_mask & green_mask) || (red_mask & blue_mask) || (green_mask & blue_mask))
                return _set_err(bmpl::utils::ErrorCode::OVERLAPPING_BITFIELD_MASKS);

            if (compression_mode > COMPR_RLE_COLOR_BITMASKS)
                return _set_err(bmpl::utils::ErrorCode::BMP_BAD_ENCODING);

            if (compression_mode == COMPR_RLE_COLOR_BITMASKS) {
                if (bits_per_pixel != 16 && bits_per_pixel != 32)
                    return _set_err(bmpl::utils::ErrorCode::BAD_BITS_PER_PIXEL_VALUE);
            }
            else {
                // checks pixels depth in bits count
                if (bits_per_pixel != 1 && bits_per_pixel != 4 && bits_per_pixel != 8 &&
                    bits_per_pixel != 16 && bits_per_pixel != 24 && bits_per_pixel != 32 && bits_per_pixel != 64)
                    return _set_err(bmpl::utils::ErrorCode::BAD_BITS_PER_PIXEL_VALUE);
            }

            return _clr_err();
        }


        //===========================================================================
        const bool BMPInfoHeaderV3_NT_4::load(bmpl::utils::LEInStream& in_stream) noexcept
        {
            if (failed())
                return false;

            if (!(in_stream >> alpha_mask))
                return _set_err(in_stream.get_error());

            if ((red_mask & alpha_mask) || (green_mask & alpha_mask) || (blue_mask & alpha_mask))
                return _set_err(bmpl::utils::ErrorCode::OVERLAPPING_BITFIELD_MASKS);

            if (compression_mode == COMPR_NO_RLE) {
                if (bits_per_pixel == 16) {
                    alpha_mask = 0b1000'0000'0000'0000;
                }
                else if (bits_per_pixel == 32) {
                    alpha_mask = 0xff00'0000;
                }
            }

            return _clr_err();
        }


        //===========================================================================
        const bool BMPInfoHeaderV4::load(bmpl::utils::LEInStream& in_stream, const bool is_V4_base) noexcept
        {
            if (failed())
                return false;

            if (compression_mode == COMPR_EMBEDS_JPEG)
                return _set_err(bmpl::utils::ErrorCode::NOT_YET_IMPLEMENTED_JPEG_DECODING);
            else if (compression_mode == COMPR_EMBEDS_PNG)
                return _set_err(bmpl::utils::ErrorCode::NOT_YET_IMPLEMENTED_PNG_DECODING);

            if (!(in_stream >> (std::uint32_t&)cs_type
                            >> red_endX
                            >> red_endY
                            >> red_endZ
                            >> green_endX
                            >> green_endY
                            >> green_endZ
                            >> blue_endX
                            >> blue_endY
                            >> blue_endZ
                            >> gamma_red.value
                            >> gamma_green.value
                            >> gamma_blue.value))
                return _set_err(in_stream.get_error());

            if (cs_type != bmpl::clr::ELogicalColorSpace::CALIBRATED_RGB &&
                cs_type != bmpl::clr::ELogicalColorSpace::S_RGB &&
                cs_type != bmpl::clr::ELogicalColorSpace::WINDOWS_COLOR_SPACE)
            {
                if (is_V4_base || (!is_V4_base &&
                    cs_type != bmpl::clr::ELogicalColorSpace::EMBEDDED_COLOR_PROFILE &&
                    cs_type != bmpl::clr::ELogicalColorSpace::LINKED_COLOR_PROFILE)
                )
                    return _set_err(bmpl::utils::ErrorCode::BAD_COLOR_SPACE_TYPE);
            }
                
            return _clr_err();
        }


        //===========================================================================
        const bool BMPInfoHeaderV4::get_gamma_values(double& gamma_red_, double& gamma_green_, double& gamma_blue_) const noexcept
        {
            if (failed())
                return false;

            if (is_calibrated_rgb_color_space()) {
                gamma_red_ = double(this->gamma_red);
                gamma_green_ = double(this->gamma_green);
                gamma_blue_ = double(this->gamma_blue);
                return true;
            }
            else if (is_windows_color_space()) {
                gamma_red_ = gamma_green_ = gamma_blue_ = 2.2;
                return true;
            }
            else {
                return false;
            }
        }


        //===========================================================================
        const bool BMPInfoHeaderV4::get_XYZ_end_points(
            std::int32_t& red_endX_, std::int32_t& red_endY_, std::int32_t& red_endZ_,
            std::int32_t& green_endX_, std::int32_t& green_endY_, std::int32_t& green_endZ_,
            std::int32_t& blue_endX_, std::int32_t& blue_endY_, std::int32_t& blue_endZ_
        ) const noexcept
        {
            if (this->is_calibrated_rgb_color_space()) {
                red_endX_ = this->red_endX;
                red_endY_ = this->red_endY;
                red_endZ_ = this->red_endZ;
                green_endX_ = this->green_endX;
                green_endY_ = this->green_endY;
                green_endZ_ = this->green_endZ;
                blue_endX_ = this->blue_endX;
                blue_endY_ = this->blue_endY;
                blue_endZ_ = this->blue_endZ;
                return true;
            }
            else {
                return false;
            }
        }


        //===========================================================================
        const bool BMPInfoHeaderV5::load(bmpl::utils::LEInStream& in_stream) noexcept
        {
            if (failed())
                return false;

            if (!(in_stream >> intent >> profile_data >> profile_size >> reserved))
                return _set_err(in_stream.get_error());

            if (intent != LCS_GM_BUSINESS && intent != LCS_GM_GRAPHICS && intent != LCS_GM_IMAGES && intent != LCS_GM_ABS_COLORIMETRIC)
                return _set_err(bmpl::utils::ErrorCode::BAD_INTENT_VALUE);

            if (profile_data != 0 && (profile_data < 128 || profile_data >= in_stream.get_size()))
                return _set_err(bmpl::utils::ErrorCode::BAD_PROFILE_DATA_OFFSET);

            if (profile_data + profile_size > in_stream.get_size())
                return _set_err(bmpl::utils::ErrorCode::BAD_PROFILE_SIZE);

            if (reserved != 0)
                _set_warning(bmpl::utils::WarningCode::NOT_ZERO_RESERVED);

            if (cs_type == bmpl::clr::ELogicalColorSpace::EMBEDDED_COLOR_PROFILE ||
                cs_type == bmpl::clr::ELogicalColorSpace::LINKED_COLOR_PROFILE)
            {
                if (profile_data == 0)
                    return _set_err(bmpl::utils::ErrorCode::MISSING_PROFILE_DATA_OFFSET);
                if (profile_size == 0)
                    return _set_err(bmpl::utils::ErrorCode::MISSING_PROFILE_DATA);
            }

            if (cs_type == bmpl::clr::ELogicalColorSpace::EMBEDDED_COLOR_PROFILE)
                _set_warning(bmpl::utils::WarningCode::EMBEDDED_PROFILE_NOT_IMPLEMENTED);
            else if (cs_type == bmpl::clr::ELogicalColorSpace::LINKED_COLOR_PROFILE)
                _set_warning(bmpl::utils::WarningCode::LINKED_PROFILE_NOT_IMPLEMENTED);

            if (bits_per_pixel == 0 && compression_mode != COMPR_EMBEDS_JPEG && compression_mode != COMPR_EMBEDS_PNG)
                return _set_err(bmpl::utils::ErrorCode::BAD_BITS_PER_PIXEL_VALUE);

            return _clr_err();

        }


        //===========================================================================
        const bool BMPInfoHeaderVOS21::load(bmpl::utils::LEInStream& in_stream) noexcept
        {
            // TODO: implement this
            return _set_err(bmpl::utils::ErrorCode::NOT_YET_IMPLEMENTED_BMP_FORMAT);
        }


        //===========================================================================
        const bool BMPInfoHeaderVOS22_16::load(bmpl::utils::LEInStream& in_stream) noexcept
        {
            // TODO: implement this
            return _set_err(bmpl::utils::ErrorCode::NOT_YET_IMPLEMENTED_BMP_FORMAT);
        }


        //===========================================================================
        const bool BMPInfoHeaderVOS22::load(bmpl::utils::LEInStream& in_stream) noexcept
        {
            // TODO: implement this
            return _set_err(bmpl::utils::ErrorCode::NOT_YET_IMPLEMENTED_BMP_FORMAT);
        }


        //===========================================================================
        const BMPInfoHeaderBase* create_bmp_info_header(bmpl::utils::LEInStream& in_stream) noexcept
        {
            // let's first load the size of the info header
            if (in_stream.failed())
                return nullptr;

            std::uint32_t header_size{ 0 };
            if (!(in_stream >> header_size))
                return nullptr;

            // then, select the correct class to instantiate
            switch (header_size) {
            case 0x0c:
            {   //-- Version 2 or 0S/2.1 of BMP file format --//
                constexpr size_t header_file_size{ 14 };

                BMPInfoHeaderV2* header_ptr{ new BMPInfoHeaderV2(in_stream) };

                if (header_ptr != nullptr && header_ptr->is_ok()) {
                    const std::size_t file_size{ std::size_t(in_stream.get_size()) };
                    const std::size_t actual_bitmap_size{ file_size - header_size - header_file_size };
                    const std::size_t pixels_per_byte{ 8 / std::size_t(header_ptr->bits_per_pixel) };
                    const std::size_t expected_bitmap_size{ std::size_t(std::abs(header_ptr->width)) * std::abs(header_ptr->height) / pixels_per_byte };

                    if (expected_bitmap_size < actual_bitmap_size) {
                        // well, this finally appears to be an OS/2 1.x BMP file
                        in_stream.seekg(header_file_size + 4);  // let's go back to the starting position of this info header, right after its size encoding on 32 bits 
                        BMPInfoHeaderVOS21* header_ptr{ new BMPInfoHeaderVOS21(in_stream) };
                        return header_ptr;
                    }
                }

                return header_ptr;
            }

            case 0x10:
                //-- Version OS/2 2.x part 16 of BMP fie format --//
                return new BMPInfoHeaderVOS22_16(in_stream);

            case 0x28:
            {   //-- Version 3 or 3_NT of BMP file format --//
                constexpr size_t header_file_size{ 14 };

                BMPInfoHeaderV3* header_ptr{ new BMPInfoHeaderV3(in_stream) };

                if (header_ptr != nullptr && header_ptr->failed()) {
                    if (header_ptr->compression_mode == BMPInfoHeaderV3_NT::COMPR_RLE_COLOR_BITMASKS ||
                        header_ptr->compression_mode == BMPInfoHeaderV3_NT::COMPR_NO_RLE)
                    {
                        // well, this finally appears to be a version 3-NT BMP file
                        in_stream.seekg(header_file_size + 4);  // let's go back to the starting position of this info header, right after its size encoding on 32 bits
                        BMPInfoHeaderV3_NT* header_ptr{ new BMPInfoHeaderV3_NT(in_stream) };
                        return header_ptr;
                    }
                }

                return header_ptr;
            }

            case 0x34:
                //-- Vesion 3_NT of BMP file format -->
                return new BMPInfoHeaderV3_NT(in_stream);

            case 0x38:
                //-- Vesion 3_NT with alpha bitfield mask of BMP file format -->
                return new BMPInfoHeaderV3_NT_4(in_stream);

            case 0x40:
                //-- Version OS/2 2.x of BMP file format --//
                return new BMPInfoHeaderVOS22(in_stream);

            case 0x6c:
                //-- Version 4 of BMP file format --//
                return new BMPInfoHeaderV4(in_stream);

            case 0x7c:
                //-- Version 5 of BMP file format --//
                return new BMPInfoHeaderV5(in_stream);

            default:
                //-- Invalid BMP file format --//
                BMPInfoHeaderBase* header_ptr{ new BMPInfoHeaderBase() };
                if (header_ptr != nullptr)
                    header_ptr->set_err(bmpl::utils::ErrorCode::INVALID_HEADER_SIZE);
                return header_ptr;
            }
        }

    }
}