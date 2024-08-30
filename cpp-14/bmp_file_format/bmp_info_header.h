#pragma once

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


#include <cstdint>

#include "../utils/colors.h"
#include "../utils/errors.h"
#include "../utils/little_endian_streaming.h"
#include "../utils/types.h"
#include "../utils/warnings.h"


namespace bmpl
{
    namespace frmt
    {
        //===========================================================================
        struct BMPInfoHeaderBase : public bmpl::utils::ErrorStatus, public bmpl::utils::WarningStatus
        {
            using MyErrBaseClass = bmpl::utils::ErrorStatus;
            using MyWarnBaseClass = bmpl::utils::WarningStatus;

            std::uint32_t header_size{ 0 };
            std::uint32_t compression_mode{ 0 };
            std::uint16_t bits_per_pixel{ 0 };
            bmpl::clr::ELogicalColorSpace cs_type{ bmpl::clr::DEFAULT_CS_TYPE };
            bool bmp_v4{ false };


            static constexpr int COMPR_NO_RLE{ 0 };
            static constexpr int COMPR_RLE_8{ 1 };
            static constexpr int COMPR_RLE_4{ 2 };
            static constexpr int COMPR_RLE_COLOR_BITMASKS{ 3 };
            static constexpr int COMPR_EMBEDS_JPEG{ 4 };
            static constexpr int COMPR_EMBEDS_PNG{ 5 };
            static constexpr int COMPR_BITFIELDS{ 3 };
            static constexpr int COMPR_ALPHABITFIELDS{ 6 };

            bool top_down_encoding{ false };


            BMPInfoHeaderBase() noexcept = default;
            BMPInfoHeaderBase(const BMPInfoHeaderBase&) noexcept = default;
            BMPInfoHeaderBase(BMPInfoHeaderBase&&) noexcept = default;
            virtual ~BMPInfoHeaderBase() noexcept = default;


            inline BMPInfoHeaderBase& operator= (const BMPInfoHeaderBase&) noexcept = default;
            inline BMPInfoHeaderBase& operator= (BMPInfoHeaderBase&&) noexcept = default;


            inline BMPInfoHeaderBase(bmpl::utils::LEInStream& in_stream) noexcept
                : MyErrBaseClass()
                , MyWarnBaseClass()
            {
                load(in_stream);
            }


            virtual inline const bool load(bmpl::utils::LEInStream& in_stream) noexcept
            {
                return this->_clr_err();
            }


            virtual inline const std::uint32_t get_colors_count() const noexcept
            {
                return 0;
            }


            virtual inline const std::uint32_t get_important_colors_count() const noexcept
            {
                return 0;
            }


            virtual inline const std::int32_t get_height() const noexcept
            {
                return 0;
            }


            virtual inline const std::int32_t get_width() const noexcept
            {
                return 0;
            }


            virtual inline const std::uint32_t get_red_mask() const noexcept
            {
                return 0xffff'ffff;
            };


            virtual inline const std::uint32_t get_green_mask() const noexcept
            {
                return 0xffff'ffff;
            };


            virtual inline const std::uint32_t get_blue_mask() const noexcept
            {
                return 0xffff'ffff;
            };


            virtual inline const std::uint32_t get_alpha_mask() const noexcept
            {
                return 0x0000'0000;
            };


            virtual inline const bool is_calibrated_rgb_color_space() const noexcept
            {
                return false;
            }

            virtual inline const bool is_sRGB_color_space() const noexcept
            {
                return false;
            }

            virtual inline const bool is_windows_color_space() const noexcept
            {
                return false;
            }

            virtual inline const bool get_gamma_values(double& gamma_red, double& gamma_green, double& gamma_blue) const noexcept
            {
                return false;
            }

            virtual inline const bool get_XYZ_end_points(
                std::int32_t& red_endX, std::int32_t& red_endY, std::int32_t& red_endZ,
                std::int32_t& green_endX, std::int32_t& green_endY, std::int32_t& green_endZ,
                std::int32_t& blue_endX, std::int32_t& blue_endY, std::int32_t& blue_endZ
            ) const noexcept
            {
                return false;
            }


            inline virtual const bool is_v1() const { return false; }
            inline virtual const bool is_v2() const { return false; }
            inline virtual const bool is_v3() const { return false; }
            inline virtual const bool is_v3_NT() const { return false; }
            inline virtual const bool is_v3_NT_4() const { return false; }
            inline virtual const bool is_v4() const { return false; }
            inline virtual const bool is_v5() const { return false; }
            inline virtual const bool is_vOS21() const { return false; }
            inline virtual const bool is_vOS22() const { return false; }

            inline virtual const bool may_embed_color_palette() const { return false; }


            inline const bool set_err(const bmpl::utils::ErrorCode err_code) noexcept
            {
                return _set_err(err_code);
            }


        };


        //===========================================================================
        struct BMPInfoHeaderWithPalette
        {
            std::uint32_t used_colors_count{ 0 };
            std::uint32_t important_colors_count{ 0 };
        };


        //===========================================================================
        struct BMPInfoHeaderV1 : public BMPInfoHeaderBase
        {
            inline BMPInfoHeaderV1(bmpl::utils::LEInStream& in_stream) noexcept
                : BMPInfoHeaderBase(in_stream)
            {
                _set_err(bmpl::utils::ErrorCode::NOT_IMPLEMENTED_BMP_V1);
            }

            inline virtual const bool is_v1() const { return true; }

        };


        //===========================================================================
        struct BMPInfoHeaderV2 : public BMPInfoHeaderBase
        {
            std::int16_t width{ 0 };
            std::int16_t height{ 0 };
            std::uint16_t planes_count{ 0 };


            BMPInfoHeaderV2() noexcept = default;
            BMPInfoHeaderV2(const BMPInfoHeaderV2&) noexcept = default;
            BMPInfoHeaderV2(BMPInfoHeaderV2&&) noexcept = default;
            virtual ~BMPInfoHeaderV2() noexcept = default;


            inline BMPInfoHeaderV2& operator= (const BMPInfoHeaderV2&) noexcept = default;
            inline BMPInfoHeaderV2& operator= (BMPInfoHeaderV2&&) noexcept = default;


            inline BMPInfoHeaderV2(bmpl::utils::LEInStream& in_stream) noexcept
                : BMPInfoHeaderBase(in_stream)
            {
                load(in_stream);
            }

            virtual inline const std::int32_t get_height() const noexcept override
            {
                return std::int32_t(height);
            }

            virtual inline const std::int32_t get_width() const noexcept override
            {
                return std::int32_t(width);
            }

            virtual const bool load(bmpl::utils::LEInStream& in_stream) noexcept;

            inline virtual const bool is_v2() const { return true; }

        };


        //===========================================================================
        struct BMPInfoHeaderV3 : virtual public BMPInfoHeaderWithPalette, virtual public BMPInfoHeaderBase
        {
            std::int32_t width{ 0 };
            std::int32_t height{ 0 };
            std::uint16_t planes_count{ 0 };
            std::uint32_t bitmap_size{ 0 };
            std::int32_t device_x_resolution{ 0 };
            std::int32_t device_y_resolution{ 0 };


            BMPInfoHeaderV3() noexcept = default;
            BMPInfoHeaderV3(const BMPInfoHeaderV3&) noexcept = default;
            BMPInfoHeaderV3(BMPInfoHeaderV3&&) noexcept = default;
            virtual ~BMPInfoHeaderV3() noexcept = default;


            inline BMPInfoHeaderV3& operator= (const BMPInfoHeaderV3&) noexcept = default;
            inline BMPInfoHeaderV3& operator= (BMPInfoHeaderV3&&) noexcept = default;


            inline BMPInfoHeaderV3(bmpl::utils::LEInStream& in_stream, const bool is_V3_base = true, const bool is_V5_base = false) noexcept
                : BMPInfoHeaderWithPalette()
                , BMPInfoHeaderBase(in_stream)
            {
                load(in_stream, is_V3_base, is_V5_base);
            }

            virtual inline const std::int32_t get_height() const noexcept override
            {
                return height;
            }

            virtual inline const std::int32_t get_width() const noexcept override
            {
                return width;
            }

            virtual const bool load(bmpl::utils::LEInStream& in_stream, const bool is_V3_base, const bool is_V5_base) noexcept;


            inline virtual const bool is_v3() const { return true; }

            inline virtual const bool may_embed_color_palette() const override
            {
                return true;
            }

            virtual inline const std::uint32_t get_colors_count() const noexcept override
            {
                return this->used_colors_count;;
            }

            virtual inline const std::uint32_t get_important_colors_count() const noexcept override
            {
                return this->important_colors_count;
            }

        };


        //===========================================================================
        struct BMPInfoHeaderV3_NT : public BMPInfoHeaderV3
        {
            std::uint32_t red_mask{ 0xffff'ffff };
            std::uint32_t green_mask{ 0xffff'ffff };
            std::uint32_t blue_mask{ 0xffff'ffff };
            std::uint32_t alpha_mask{ 0xffff'ffff };


            BMPInfoHeaderV3_NT() noexcept = default;
            BMPInfoHeaderV3_NT(const BMPInfoHeaderV3_NT&) noexcept = default;
            BMPInfoHeaderV3_NT(BMPInfoHeaderV3_NT&&) noexcept = default;
            virtual ~BMPInfoHeaderV3_NT() noexcept = default;


            inline BMPInfoHeaderV3_NT& operator= (const BMPInfoHeaderV3_NT&) noexcept = default;
            inline BMPInfoHeaderV3_NT& operator= (BMPInfoHeaderV3_NT&&) noexcept = default;


            inline BMPInfoHeaderV3_NT(bmpl::utils::LEInStream& in_stream, const bool is_V4_base = false, const bool is_V5_base = false) noexcept
                : BMPInfoHeaderV3(in_stream, false, is_V5_base)
            {
                load(in_stream, is_V4_base || is_V5_base);
            }

            virtual const bool load(bmpl::utils::LEInStream& in_stream, const bool is_V4_base) noexcept;

            virtual inline const std::uint32_t get_red_mask() const noexcept override
            {
                return red_mask;
            };

            virtual inline const std::uint32_t get_green_mask() const noexcept override
            {
                return green_mask;
            };

            virtual inline const std::uint32_t get_blue_mask() const noexcept override
            {
                return blue_mask;
            };


            inline virtual const bool is_v3() const { return false; }
            inline virtual const bool is_v3_NT() const { return true; }



        };


        //===========================================================================
        struct BMPInfoHeaderV3_NT_4 : public BMPInfoHeaderV3_NT
        {
            BMPInfoHeaderV3_NT_4() noexcept = default;
            BMPInfoHeaderV3_NT_4(const BMPInfoHeaderV3_NT_4&) noexcept = default;
            BMPInfoHeaderV3_NT_4(BMPInfoHeaderV3_NT_4&&) noexcept = default;
            virtual ~BMPInfoHeaderV3_NT_4() noexcept = default;


            inline BMPInfoHeaderV3_NT_4& operator= (const BMPInfoHeaderV3_NT_4&) noexcept = default;
            inline BMPInfoHeaderV3_NT_4& operator= (BMPInfoHeaderV3_NT_4&&) noexcept = default;


            inline BMPInfoHeaderV3_NT_4(bmpl::utils::LEInStream& in_stream, const bool is_V4_base = false, const bool is_V5_base = false) noexcept
                : BMPInfoHeaderV3_NT(in_stream, is_V4_base, is_V5_base)
            {
                load(in_stream);
            }

            virtual const bool load(bmpl::utils::LEInStream& in_stream) noexcept;

            virtual inline const std::uint32_t get_alpha_mask() const noexcept override
            {
                return alpha_mask;
            };


            inline virtual const bool is_v3_NT() const { return false; }
            inline virtual const bool is_v3_NT_4() const { return true; }

        };


        //===========================================================================
        struct BMPInfoHeaderV4 : public BMPInfoHeaderV3_NT_4
        {
            std::int32_t red_endX{ -1 };
            std::int32_t red_endY{ -1 };
            std::int32_t red_endZ{ -1 };
            std::int32_t green_endX{ -1 };
            std::int32_t green_endY{ -1 };
            std::int32_t green_endZ{ -1 };
            std::int32_t blue_endX{ -1 };
            std::int32_t blue_endY{ -1 };
            std::int32_t blue_endZ{ -1 };
            bmpl::utils::Frac16_16 gamma_red{ 1.0 };
            bmpl::utils::Frac16_16 gamma_green{ 1.0 };
            bmpl::utils::Frac16_16 gamma_blue{ 1.0 };


            BMPInfoHeaderV4() noexcept = default;
            BMPInfoHeaderV4(const BMPInfoHeaderV4&) noexcept = default;
            BMPInfoHeaderV4(BMPInfoHeaderV4&&) noexcept = default;
            virtual ~BMPInfoHeaderV4() noexcept = default;


            inline BMPInfoHeaderV4& operator= (const BMPInfoHeaderV4&) noexcept = default;
            inline BMPInfoHeaderV4& operator= (BMPInfoHeaderV4&&) noexcept = default;


            inline BMPInfoHeaderV4(bmpl::utils::LEInStream& in_stream, const bool is_V4_base = true, const bool is_V5_base = false) noexcept
                : BMPInfoHeaderV3_NT_4(in_stream, is_V4_base, is_V5_base)
            {
                load(in_stream, is_V4_base);
            }

            virtual const bool load(bmpl::utils::LEInStream& in_stream, const bool is_V4_base) noexcept;


            virtual inline const bool is_calibrated_rgb_color_space() const noexcept
            {
                return (this->compression_mode == COMPR_NO_RLE && this->cs_type == bmpl::clr::ELogicalColorSpace::CALIBRATED_RGB);
            }

            virtual inline const bool is_sRGB_color_space() const noexcept
            {
                return (this->compression_mode == COMPR_NO_RLE && this->cs_type == bmpl::clr::ELogicalColorSpace::S_RGB);
            }

            virtual inline const bool is_windows_color_space() const noexcept
            {
                return (this->compression_mode == COMPR_NO_RLE && this->cs_type == bmpl::clr::ELogicalColorSpace::WINDOWS_COLOR_SPACE);
            }

            virtual const bool get_gamma_values(double& gamma_red_, double& gamma_green_, double& gamma_blue_) const noexcept;

            virtual const bool get_XYZ_end_points(
                std::int32_t& red_endX_, std::int32_t& red_endY_, std::int32_t& red_endZ_,
                std::int32_t& green_endX_, std::int32_t& green_endY_, std::int32_t& green_endZ_,
                std::int32_t& blue_endX_, std::int32_t& blue_endY_, std::int32_t& blue_endZ_
            ) const noexcept;

            inline virtual const bool is_v3_NT() const { return false; }
            inline virtual const bool is_v4() const { return true; }

        };


        //===========================================================================
        struct BMPInfoHeaderV5 : public BMPInfoHeaderV4
        {
            static constexpr std::uint32_t HEADER_SIZE{ 128 };

            static constexpr std::uint32_t LCS_GM_BUSINESS{ 1 };
            static constexpr std::uint32_t LCS_GM_GRAPHICS{ 2 };
            static constexpr std::uint32_t LCS_GM_IMAGES{ 4 };
            static constexpr std::uint32_t LCS_GM_ABS_COLORIMETRIC{ 8 };

            std::uint32_t intent{ 0 };
            std::uint32_t profile_data{ 0 };
            std::uint32_t profile_size{ 0 };
            std::uint32_t reserved{ 0 };

            BMPInfoHeaderV5() noexcept = default;
            BMPInfoHeaderV5(const BMPInfoHeaderV5&) noexcept = default;
            BMPInfoHeaderV5(BMPInfoHeaderV5&&) noexcept = default;
            virtual ~BMPInfoHeaderV5() noexcept = default;


            inline BMPInfoHeaderV5& operator= (const BMPInfoHeaderV5&) noexcept = default;
            inline BMPInfoHeaderV5& operator= (BMPInfoHeaderV5&&) noexcept = default;


            inline BMPInfoHeaderV5(bmpl::utils::LEInStream& in_stream) noexcept
                : BMPInfoHeaderV4(in_stream, false, true)
            {
                load(in_stream);
            }

            virtual const bool load(bmpl::utils::LEInStream& in_stream) noexcept;

            inline virtual const bool is_v4() const { return false; }
            inline virtual const bool is_v5() const { return true; }

        };


        //===========================================================================
        struct BMPInfoHeaderVOS21 : public BMPInfoHeaderBase
        {
            std::uint16_t width{ 0 };
            std::uint16_t height{ 0 };
            std::uint16_t planes_count{ 0 };


            BMPInfoHeaderVOS21() noexcept = default;
            BMPInfoHeaderVOS21(const BMPInfoHeaderVOS21&) noexcept = default;
            BMPInfoHeaderVOS21(BMPInfoHeaderVOS21&&) noexcept = default;
            virtual ~BMPInfoHeaderVOS21() noexcept = default;


            inline BMPInfoHeaderVOS21& operator= (const BMPInfoHeaderVOS21&) noexcept = default;
            inline BMPInfoHeaderVOS21& operator= (BMPInfoHeaderVOS21&&) noexcept = default;


            inline BMPInfoHeaderVOS21(bmpl::utils::LEInStream& in_stream) noexcept
                : BMPInfoHeaderBase(in_stream)
            {
                load(in_stream);
            }

            virtual inline const std::int32_t get_height() const noexcept override
            {
                return std::int32_t(height);
            }

            virtual inline const std::int32_t get_width() const noexcept override
            {
                return std::int32_t(width);
            }

            virtual const bool load(bmpl::utils::LEInStream& in_stream) noexcept;

            inline virtual const bool is_vOS21() const { return true; }

        };


        //===========================================================================
        struct BMPInfoHeaderVOS22_16 : virtual public BMPInfoHeaderWithPalette, virtual public BMPInfoHeaderBase
        {
            std::int32_t width{ 0 };
            std::int32_t height{ 0 };
            std::uint16_t planes_count{ 0 };
            std::uint32_t bitmap_size{ 0 };
            std::uint32_t device_x_resolution{ 0 };
            std::uint32_t device_y_resolution{ 0 };
            std::int16_t resolution_units{ 0 };
            std::int16_t reserved{ 0 };
            std::int16_t recording_algorithm{ 0 };
            std::int16_t halftoning_rendering_algorithm{ 0 };
            std::uint16_t halftoning_reserved_1{ 0 };
            std::uint16_t halftoning_reserved_2{ 0 };
            std::uint32_t color_encoding{ 0 };
            std::uint32_t application_identifier{ 0 };


            BMPInfoHeaderVOS22_16() noexcept = default;
            BMPInfoHeaderVOS22_16(const BMPInfoHeaderVOS22_16&) noexcept = default;
            BMPInfoHeaderVOS22_16(BMPInfoHeaderVOS22_16&&) noexcept = default;
            virtual ~BMPInfoHeaderVOS22_16() noexcept = default;


            inline BMPInfoHeaderVOS22_16& operator= (const BMPInfoHeaderVOS22_16&) noexcept = default;
            inline BMPInfoHeaderVOS22_16& operator= (BMPInfoHeaderVOS22_16&&) noexcept = default;


            inline BMPInfoHeaderVOS22_16(bmpl::utils::LEInStream& in_stream) noexcept
                : BMPInfoHeaderWithPalette()
                , BMPInfoHeaderBase(in_stream)
            {
                load(in_stream);
            }

            virtual inline const std::int32_t get_height() const noexcept override
            {
                return height;
            }

            virtual inline const std::int32_t get_width() const noexcept override
            {
                return width;
            }

            virtual const bool load(bmpl::utils::LEInStream& in_stream) noexcept;

            inline virtual const bool may_embed_color_palette() const override
            {
                return true;
            }

            virtual inline const std::uint32_t get_colors_count() const noexcept override
            {
                return this->used_colors_count;;
            }

            virtual inline const std::uint32_t get_important_colors_count() const noexcept override
            {
                return this->important_colors_count;
            }

        };


        //===========================================================================
        struct BMPInfoHeaderVOS22 : public BMPInfoHeaderVOS22_16
        {
            BMPInfoHeaderVOS22() noexcept = default;
            BMPInfoHeaderVOS22(const BMPInfoHeaderVOS22&) noexcept = default;
            BMPInfoHeaderVOS22(BMPInfoHeaderVOS22&&) noexcept = default;
            virtual ~BMPInfoHeaderVOS22() noexcept = default;


            inline BMPInfoHeaderVOS22& operator= (const BMPInfoHeaderVOS22&) noexcept = default;
            inline BMPInfoHeaderVOS22& operator= (BMPInfoHeaderVOS22&&) noexcept = default;


            inline BMPInfoHeaderVOS22(bmpl::utils::LEInStream& in_stream) noexcept
                : BMPInfoHeaderVOS22_16(in_stream)
            {
                load(in_stream);
            }

            virtual const bool load(bmpl::utils::LEInStream& in_stream) noexcept;

            inline virtual const bool is_vOS22() const { return true; }

        };


        //===========================================================================
        const BMPInfoHeaderBase* create_bmp_info_header(bmpl::utils::LEInStream& in_stream) noexcept;

    }
}