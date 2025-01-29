#pragma once

/*
MIT License

Copyright (c) 2024-2025 Philippe Schmouker, ph.schmouker (at) gmail.com

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
* function  as  well  when  compiled  with  standard  c++11  since  no   c++14
* specificities have been used there, but it has not been tested as such.
*/


#include <cstdint>

#include "bmp_file_header.h"
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
        struct BMPInfoHeaderWithPalette
        {
            std::uint32_t used_colors_count{ 0 };
            std::uint32_t important_colors_count{ 0 };

            inline const std::uint32_t get_colors_count() const noexcept
            {
                return this->used_colors_count;
            }

        };


        //===========================================================================
        template<typename DimsT>
        struct BMPDims
        {
            DimsT height{ 0 };
            DimsT width{ 0 };
        };


        //===========================================================================
        struct BMPInfoHeaderBase : public bmpl::utils::ErrorStatus, public bmpl::utils::WarningStatus, public BMPInfoHeaderWithPalette
        {
            using MyErrBaseClass = bmpl::utils::ErrorStatus;
            using MyWarnBaseClass = bmpl::utils::WarningStatus;

            std::uint32_t header_size{ 0 };
            std::uint32_t compression_mode{ 0 };
            std::int16_t  bits_per_pixel{ 0 };
            bmpl::clr::ELogicalColorSpace cs_type{ bmpl::clr::DEFAULT_CS_TYPE };


            static constexpr std::uint32_t COMPR_NO_RLE{ 0 };
            static constexpr std::uint32_t COMPR_RLE_8{ 1 };
            static constexpr std::uint32_t COMPR_RLE_4{ 2 };
            static constexpr std::uint32_t COMPR_RLE_COLOR_BITMASKS{ 3 };
            static constexpr std::uint32_t COMPR_EMBEDS_JPEG{ 4 };
            static constexpr std::uint32_t COMPR_EMBEDS_PNG{ 5 };

            static constexpr std::uint32_t COMPR_BITFIELDS{ 3 };
            static constexpr std::uint32_t COMPR_ALPHABITFIELDS{ 6 };

            static constexpr std::uint32_t COMPR_HUFFMAN_1D{ 3 };
            static constexpr std::uint32_t COMPR_RLE_24{ 4 };

            bool top_down_encoding{ false };


            BMPInfoHeaderBase() noexcept = default;
            BMPInfoHeaderBase(const BMPInfoHeaderBase&) noexcept = default;
            BMPInfoHeaderBase(BMPInfoHeaderBase&&) noexcept = default;
            virtual ~BMPInfoHeaderBase() noexcept = default;


            BMPInfoHeaderBase& operator= (const BMPInfoHeaderBase&) noexcept = default;
            BMPInfoHeaderBase& operator= (BMPInfoHeaderBase&&) noexcept = default;


            inline BMPInfoHeaderBase(bmpl::utils::LEInStream& in_stream, const std::uint32_t header_size_) noexcept
                : MyErrBaseClass()
                , MyWarnBaseClass()
                , BMPInfoHeaderWithPalette()
                , header_size(header_size_)
            {
                load(in_stream);
            }

            virtual inline const bool load(bmpl::utils::LEInStream& in_stream) noexcept
            {
                return this->_clr_err();
            }

            [[nodiscard]]
            virtual inline const std::int32_t get_device_x_resolution() const noexcept
            {
                return 2835;  // notice: default value for 72 dpi extended to the standadrd default unit (1 meter)
            }

            [[nodiscard]]
            virtual inline const std::int32_t get_device_y_resolution() const noexcept
            {
                return 2835;  // notice: default value for 72 dpi extended to the standadrd default unit (1 meter)
            }

            [[nodiscard]]
            virtual inline const std::uint32_t get_important_colors_count() const noexcept
            {
                return 0;
            }

            [[nodiscard]]
            virtual inline const std::uint32_t get_height() const noexcept
            {
                return 0;
            }

            [[nodiscard]]
            virtual inline const std::uint32_t get_width() const noexcept
            {
                return 0;
            }

            [[nodiscard]]
            virtual inline const std::uint32_t get_red_mask() const noexcept
            {
                return 0xffff'ffff;
            };

            [[nodiscard]]
            virtual inline const std::uint32_t get_green_mask() const noexcept
            {
                return 0xffff'ffff;
            };

            [[nodiscard]]
            virtual inline const std::uint32_t get_blue_mask() const noexcept
            {
                return 0xffff'ffff;
            };

            [[nodiscard]]
            virtual inline const std::uint32_t get_alpha_mask() const noexcept
            {
                return 0x0000'0000;
            };

            [[nodiscard]]
            virtual inline const bool is_calibrated_rgb_color_space() const noexcept
            {
                return false;
            }

            [[nodiscard]]
            virtual inline const bool is_sRGB_color_space() const noexcept
            {
                return false;
            }

            [[nodiscard]]
            virtual inline const bool is_windows_color_space() const noexcept
            {
                return false;
            }

            [[nodiscard]]
            virtual inline const bool get_gamma_values(double& gamma_red, double& gamma_green, double& gamma_blue) const noexcept
            {
                return false;
            }

            [[nodiscard]]
            virtual inline const bool get_XYZ_end_points(
                std::int32_t& red_endX, std::int32_t& red_endY, std::int32_t& red_endZ,
                std::int32_t& green_endX, std::int32_t& green_endY, std::int32_t& green_endZ,
                std::int32_t& blue_endX, std::int32_t& blue_endY, std::int32_t& blue_endZ
            ) const noexcept
            {
                return false;
            }


            [[nodiscard]] inline virtual const bool is_v1() const { return false; }
            [[nodiscard]] inline virtual const bool is_v2() const { return false; }
            [[nodiscard]] inline virtual const bool is_v3() const { return false; }
            [[nodiscard]] inline virtual const bool is_v3_NT() const { return false; }
            [[nodiscard]] inline virtual const bool is_v3_NT_4() const { return false; }
            [[nodiscard]] inline virtual const bool is_v4() const { return false; }
            [[nodiscard]] inline virtual const bool is_v5() const { return false; }
            [[nodiscard]] inline virtual const bool is_vOS21() const { return false; }
            [[nodiscard]] inline virtual const bool is_vOS22() const { return false; }

            [[nodiscard]] inline virtual const bool may_embed_color_palette() const { return false; }


            inline const bool set_err(const bmpl::utils::ErrorCode err_code) noexcept
            {
                return _set_err(err_code);
            }


        };


        //===========================================================================
        struct BMPInfoHeaderV1 : virtual public BMPDims<std::int32_t>, virtual public BMPInfoHeaderBase
        {
            std::uint16_t planes_count{ 0 };
            std::uint32_t bitmap_size{ 0 };

            inline BMPInfoHeaderV1(const bmpl::frmt::BMPFileHeaderV1* file_header_ptr) noexcept;

            [[nodiscard]]
            virtual inline const std::uint32_t get_height() const noexcept override
            {
                return this->height;
            }

            [[nodiscard]]
            virtual inline const std::uint32_t get_width() const noexcept override
            {
                return this->width;
            }

            [[nodiscard]]
            inline virtual const bool is_v1() const override { return true; }

        };


        //===========================================================================
        template<typename DimsT = std::int16_t>
        struct BMPInfoHeaderV2 : virtual public BMPDims<DimsT>, virtual public BMPInfoHeaderBase
        {
            static constexpr std::uint32_t HEADER_SIZE{ 12 };

            std::uint16_t planes_count{ 0 };


            BMPInfoHeaderV2() noexcept = default;
            BMPInfoHeaderV2(const BMPInfoHeaderV2&) noexcept = default;
            BMPInfoHeaderV2(BMPInfoHeaderV2&&) noexcept = default;
            virtual ~BMPInfoHeaderV2() noexcept = default;


            BMPInfoHeaderV2& operator= (const BMPInfoHeaderV2&) noexcept = default;
            BMPInfoHeaderV2& operator= (BMPInfoHeaderV2&&) noexcept = default;


            inline BMPInfoHeaderV2(bmpl::utils::LEInStream& in_stream) noexcept
                : BMPDims<DimsT>()
                , BMPInfoHeaderBase(in_stream, HEADER_SIZE)
            {
                load(in_stream);
            }

            [[nodiscard]]
            virtual inline const std::uint32_t get_height() const noexcept override
            {
                return this->height;
            }

            [[nodiscard]]
            virtual inline const std::uint32_t get_width() const noexcept override
            {
                return this->width;
            }

            virtual const bool load(bmpl::utils::LEInStream& in_stream) noexcept override;

            [[nodiscard]]
            inline virtual const bool is_v2() const override { return true; }

        };


        //===========================================================================
        struct BMPInfoHeaderV3 : virtual public BMPDims<std::int32_t>, virtual public BMPInfoHeaderBase
        {
            static constexpr std::uint32_t HEADER_SIZE{ 40 };

            std::uint32_t bitmap_size{ 0 };
            std::int32_t device_x_resolution{ 0 };
            std::int32_t device_y_resolution{ 0 };
            std::int16_t planes_count{ 0 };


            BMPInfoHeaderV3() noexcept = default;
            BMPInfoHeaderV3(const BMPInfoHeaderV3&) noexcept = default;
            BMPInfoHeaderV3(BMPInfoHeaderV3&&) noexcept = default;
            virtual ~BMPInfoHeaderV3() noexcept = default;


            BMPInfoHeaderV3& operator= (const BMPInfoHeaderV3&) noexcept = default;
            BMPInfoHeaderV3& operator= (BMPInfoHeaderV3&&) noexcept = default;


            inline BMPInfoHeaderV3(bmpl::utils::LEInStream& in_stream, const bool is_V3_base = true, const bool is_V5_base = false) noexcept
                : BMPDims<std::int32_t>()
                , BMPInfoHeaderBase(in_stream, HEADER_SIZE)
            {
                load(in_stream, is_V3_base, is_V5_base);
            }

            [[nodiscard]]
            virtual inline const std::int32_t get_device_x_resolution() const noexcept override
            {
                return this->device_x_resolution;
            }

            [[nodiscard]]
            virtual inline const std::int32_t get_device_y_resolution() const noexcept override
            {
                return this->device_y_resolution;
            }

            [[nodiscard]]
            virtual inline const std::uint32_t get_height() const noexcept override
            {
                return this->height;
            }

            [[nodiscard]]
            virtual inline const std::uint32_t get_width() const noexcept override
            {
                return this->width;
            }

            virtual const bool load(bmpl::utils::LEInStream& in_stream, const bool is_V3_base, const bool is_V5_base) noexcept;

            [[nodiscard]]
            inline virtual const bool is_v3() const override { return true; }

            [[nodiscard]]
            inline virtual const bool may_embed_color_palette() const override
            {
                return true;
            }

            [[nodiscard]]
            virtual inline const std::uint32_t get_important_colors_count() const noexcept override
            {
                return this->important_colors_count;
            }

        };


        //===========================================================================
        struct BMPInfoHeaderV3_NT : public BMPInfoHeaderV3
        {
            static constexpr std::uint32_t HEADER_SIZE{ 52 };

            std::uint32_t red_mask{ 0xffff'ffff };
            std::uint32_t green_mask{ 0xffff'ffff };
            std::uint32_t blue_mask{ 0xffff'ffff };
            std::uint32_t alpha_mask{ 0xffff'ffff };


            BMPInfoHeaderV3_NT() noexcept = default;
            BMPInfoHeaderV3_NT(const BMPInfoHeaderV3_NT&) noexcept = default;
            BMPInfoHeaderV3_NT(BMPInfoHeaderV3_NT&&) noexcept = default;
            virtual ~BMPInfoHeaderV3_NT() noexcept = default;


            BMPInfoHeaderV3_NT& operator= (const BMPInfoHeaderV3_NT&) noexcept = default;
            BMPInfoHeaderV3_NT& operator= (BMPInfoHeaderV3_NT&&) noexcept = default;


            inline BMPInfoHeaderV3_NT(
                bmpl::utils::LEInStream& in_stream,
                const std::uint32_t header_size_ = HEADER_SIZE,
                const bool is_V4_base = false,
                const bool is_V5_base = false
            ) noexcept
                : BMPInfoHeaderV3(in_stream, false, is_V5_base)
            {
                this->header_size = header_size_;
                load(in_stream, is_V4_base || is_V5_base);
            }

            virtual const bool load(bmpl::utils::LEInStream& in_stream, const bool is_V4_base) noexcept;

            [[nodiscard]]
            virtual inline const std::uint32_t get_red_mask() const noexcept override
            {
                return red_mask;
            };

            [[nodiscard]]
            virtual inline const std::uint32_t get_green_mask() const noexcept override
            {
                return green_mask;
            };

            [[nodiscard]]
            virtual inline const std::uint32_t get_blue_mask() const noexcept override
            {
                return blue_mask;
            };


            [[nodiscard]] inline virtual const bool is_v3() const override { return false; }
            [[nodiscard]] inline virtual const bool is_v3_NT() const override { return true; }

        };


        //===========================================================================
        struct BMPInfoHeaderV3_NT_4 : public BMPInfoHeaderV3_NT
        {
            static constexpr std::uint32_t HEADER_SIZE{ 56 };

            BMPInfoHeaderV3_NT_4() noexcept = default;
            BMPInfoHeaderV3_NT_4(const BMPInfoHeaderV3_NT_4&) noexcept = default;
            BMPInfoHeaderV3_NT_4(BMPInfoHeaderV3_NT_4&&) noexcept = default;
            virtual ~BMPInfoHeaderV3_NT_4() noexcept = default;


            BMPInfoHeaderV3_NT_4& operator= (const BMPInfoHeaderV3_NT_4&) noexcept = default;
            BMPInfoHeaderV3_NT_4& operator= (BMPInfoHeaderV3_NT_4&&) noexcept = default;


            inline BMPInfoHeaderV3_NT_4(
                bmpl::utils::LEInStream& in_stream,
                const std::uint32_t header_size = HEADER_SIZE,
                const bool is_V4_base = false,
                const bool is_V5_base = false
            ) noexcept
                : BMPInfoHeaderV3_NT(in_stream, header_size, is_V4_base, is_V5_base)
            {
                load(in_stream);
            }

            virtual const bool load(bmpl::utils::LEInStream& in_stream) noexcept override;

            [[nodiscard]]
            virtual inline const std::uint32_t get_alpha_mask() const noexcept override
            {
                return alpha_mask;
            };


            [[nodiscard]] inline virtual const bool is_v3_NT() const override { return false; }
            [[nodiscard]] inline virtual const bool is_v3_NT_4() const override { return true; }

        };


        //===========================================================================
        struct BMPInfoHeaderV4 : public BMPInfoHeaderV3_NT_4
        {
            static constexpr std::uint32_t HEADER_SIZE{ 108 };

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


            BMPInfoHeaderV4& operator= (const BMPInfoHeaderV4&) noexcept = default;
            BMPInfoHeaderV4& operator= (BMPInfoHeaderV4&&) noexcept = default;


            inline BMPInfoHeaderV4(
                bmpl::utils::LEInStream& in_stream,
                const std::uint32_t header_size = HEADER_SIZE,
                const bool is_V4_base = true,
                const bool is_V5_base = false
            ) noexcept
                : BMPInfoHeaderV3_NT_4(in_stream, header_size, is_V4_base, is_V5_base)
            {
                load(in_stream, is_V4_base);
            }

            virtual const bool load(bmpl::utils::LEInStream& in_stream, const bool is_V4_base) noexcept;


            [[nodiscard]]
            virtual inline const bool is_calibrated_rgb_color_space() const noexcept
            {
                return (this->compression_mode == COMPR_NO_RLE && this->cs_type == bmpl::clr::ELogicalColorSpace::CALIBRATED_RGB);
            }

            [[nodiscard]]
            virtual inline const bool is_sRGB_color_space() const noexcept
            {
                return (this->compression_mode == COMPR_NO_RLE && this->cs_type == bmpl::clr::ELogicalColorSpace::S_RGB);
            }

            [[nodiscard]]
            virtual inline const bool is_windows_color_space() const noexcept
            {
                return (this->compression_mode == COMPR_NO_RLE && this->cs_type == bmpl::clr::ELogicalColorSpace::WINDOWS_COLOR_SPACE);
            }

            [[nodiscard]]
            virtual const bool get_gamma_values(double& gamma_red_, double& gamma_green_, double& gamma_blue_) const noexcept;

            [[nodiscard]]
            virtual const bool get_XYZ_end_points(
                std::int32_t& red_endX_, std::int32_t& red_endY_, std::int32_t& red_endZ_,
                std::int32_t& green_endX_, std::int32_t& green_endY_, std::int32_t& green_endZ_,
                std::int32_t& blue_endX_, std::int32_t& blue_endY_, std::int32_t& blue_endZ_
            ) const noexcept;

            [[nodiscard]] inline virtual const bool is_v3_NT_4() const override { return false; }
            [[nodiscard]] inline virtual const bool is_v4() const override { return true; }

        };


        //===========================================================================
        struct BMPInfoHeaderV5 : public BMPInfoHeaderV4
        {
            static constexpr std::uint32_t HEADER_SIZE{ 124 };

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


            BMPInfoHeaderV5& operator= (const BMPInfoHeaderV5&) noexcept = default;
            BMPInfoHeaderV5& operator= (BMPInfoHeaderV5&&) noexcept = default;


            inline BMPInfoHeaderV5(bmpl::utils::LEInStream& in_stream) noexcept
                : BMPInfoHeaderV4(in_stream, HEADER_SIZE, false, true)
            {
                load(in_stream);
            }

            virtual const bool load(bmpl::utils::LEInStream& in_stream) noexcept override;

            [[nodiscard]] inline virtual const bool is_v4() const override { return false; }
            [[nodiscard]] inline virtual const bool is_v5() const override { return true; }

        };


        //===========================================================================
        struct BMPInfoHeaderVOS21 : virtual public BMPDims<std::uint16_t>, virtual public BMPInfoHeaderV2<std::uint16_t>
        {
            using MyBaseClass = BMPInfoHeaderV2<std::uint16_t>;


            BMPInfoHeaderVOS21() noexcept = default;
            BMPInfoHeaderVOS21(const BMPInfoHeaderVOS21&) noexcept = default;
            BMPInfoHeaderVOS21(BMPInfoHeaderVOS21&&) noexcept = default;
            virtual ~BMPInfoHeaderVOS21() noexcept = default;


            BMPInfoHeaderVOS21& operator= (const BMPInfoHeaderVOS21&) noexcept = default;
            BMPInfoHeaderVOS21& operator= (BMPInfoHeaderVOS21&&) noexcept = default;


            inline BMPInfoHeaderVOS21(bmpl::utils::LEInStream& in_stream) noexcept
                : BMPDims<std::uint16_t>()
                , MyBaseClass(in_stream)
            {
                load(in_stream);
            }

            [[nodiscard]]
            virtual inline const std::uint32_t get_height() const noexcept override
            {
                return this->height;
            }

            [[nodiscard]]
            virtual inline const std::uint32_t get_width() const noexcept override
            {
                return this->width;
            }

            virtual const bool load(bmpl::utils::LEInStream& in_stream) noexcept override;

            [[nodiscard]]
            inline virtual const bool may_embed_color_palette() const override
            {
                return true;
            }

            [[nodiscard]] inline virtual const bool is_v2() const override { return false; }
            [[nodiscard]] inline virtual const bool is_vOS21() const override { return true; }

        };


        //===========================================================================
        struct BMPInfoHeaderVOS22 : virtual public BMPDims<std::uint32_t>, virtual public BMPInfoHeaderBase
        {
            std::int16_t planes_count{ 0 };
            std::uint32_t bitmap_size{ 0 };
            std::uint32_t device_x_resolution{ 0 };
            std::uint32_t device_y_resolution{ 0 };
            std::int16_t resolution_units{ 0 };
            std::int16_t reserved{ 0 };
            std::int16_t recording_algorithm{ 0 };
            std::int16_t halftoning_rendering_algorithm{ 0 };
            std::uint32_t halftoning_param_1{ 0 };
            std::uint32_t halftoning_param_2{ 0 };
            std::uint32_t color_encoding{ 0 };
            std::uint32_t application_identifier{ 0 };

            static constexpr std::int16_t HALFTONING_NO_ALGORITHM{ 0 };
            static constexpr std::int16_t HALFTONING_DIFFUSION_ALGORITHM{ 1 };
            static constexpr std::int16_t HALFTONING_PANDA_ALGORITHM{ 2 };
            static constexpr std::int16_t HALFTONING_SUPER_CIRCLE_ALGORITHM{ 3 };

            static constexpr std::uint32_t COLOR_ENCODING_RGB{ 0 };


            BMPInfoHeaderVOS22() noexcept = default;
            BMPInfoHeaderVOS22(const BMPInfoHeaderVOS22&) noexcept = default;
            BMPInfoHeaderVOS22(BMPInfoHeaderVOS22&&) noexcept = default;
            virtual ~BMPInfoHeaderVOS22() noexcept = default;


            BMPInfoHeaderVOS22& operator= (const BMPInfoHeaderVOS22&) noexcept = default;
            BMPInfoHeaderVOS22& operator= (BMPInfoHeaderVOS22&&) noexcept = default;


            inline BMPInfoHeaderVOS22(bmpl::utils::LEInStream& in_stream, const std::uint32_t header_size) noexcept
                : BMPDims<std::uint32_t>()
                , BMPInfoHeaderBase(in_stream, header_size)
            {
                load(in_stream);
            }

            [[nodiscard]]
            inline const std::uint32_t get_application_identifier() const noexcept
            {
                return application_identifier;
            }

            [[nodiscard]]
            virtual inline const std::int32_t get_device_x_resolution() const noexcept override
            {
                return this->device_x_resolution;
            }

            [[nodiscard]]
            virtual inline const std::int32_t get_device_y_resolution() const noexcept override
            {
                return this->device_y_resolution;
            }

            [[nodiscard]]
            inline const std::int16_t get_halftoning_mode() const noexcept
            {
                return halftoning_rendering_algorithm;
            }

            [[nodiscard]]
            inline const std::uint16_t get_halftoning_error_diffusion_damping() const noexcept
            {
                if (halftoning_rendering_algorithm == HALFTONING_DIFFUSION_ALGORITHM)
                    return get_halftoning_param_1();
                else
                    return 0;  // notice: indicates no diffusion of errors
            }

            [[nodiscard]]
            inline const std::uint16_t get_halftoning_param_1() const noexcept
            {
                return halftoning_param_1;
            }

            [[nodiscard]]
            inline const std::uint16_t get_halftoning_param_2() const noexcept
            {
                return halftoning_param_2;
            }

            [[nodiscard]]
            inline const std::uint16_t get_halftoning_x_size() const noexcept;

            [[nodiscard]]
            inline const std::uint16_t get_halftoning_y_size() const noexcept;

            [[nodiscard]]
            inline const bool has_halftoning() const noexcept
            {
                return halftoning_rendering_algorithm != HALFTONING_NO_ALGORITHM;
            }

            [[nodiscard]]
            virtual inline const std::uint32_t get_height() const noexcept override
            {
                return this->height;
            }

            [[nodiscard]]
            virtual inline const std::uint32_t get_width() const noexcept override
            {
                return this->width;
            }

            virtual const bool load(bmpl::utils::LEInStream& in_stream) noexcept override;

            [[nodiscard]]
            inline virtual const bool may_embed_color_palette() const override
            {
                return true;
            }

            [[nodiscard]]
            virtual inline const std::uint32_t get_important_colors_count() const noexcept override
            {
                return this->important_colors_count;
            }

            [[nodiscard]] inline virtual const bool is_vOS22() const override { return true; }

        };


        //===========================================================================
        BMPInfoHeaderBase* create_bmp_info_header(
            bmpl::utils::LEInStream& in_stream,
            const bmpl::frmt::BMPFileHeaderBase* file_header_ptr
        ) noexcept;



        //===========================================================================
        // Local implementations

        //---------------------------------------------------------------------------
        template<typename DimsT>
        const bool BMPInfoHeaderV2<DimsT>::load(bmpl::utils::LEInStream& in_stream) noexcept
        {
            if ((in_stream >> this->width >> this->height >> planes_count >> bits_per_pixel).failed())
                return _set_err(in_stream.get_error());

            if (this->width < 0)
                return _set_err(bmpl::utils::ErrorCode::NEGATIVE_WIDTH);

            if (this->width == 0 || this->height == 0)
                return _set_err(bmpl::utils::ErrorCode::INVALID_IMAGE_DIMENSIONS);

            if (this->height < 0) {
                // top-down encoding
                this->height = -this->height;
                top_down_encoding = true;
            }

            if (planes_count != 1)
                set_warning(bmpl::utils::WarningCode::BAD_PLANES_VALUE);

            return _clr_err();
        }

    }
}