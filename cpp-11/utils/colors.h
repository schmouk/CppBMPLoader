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
* function  as  well  when  compiled  with  standard  c++11  since  no   c++14
* specificities have been used there, but it has not been tested as such.
*/


#include <cstdint>

#include "../utils/little_endian_streaming.h"
#include "../utils/types.h"


namespace bmpl
{
    namespace clr
    {
        //===========================================================================
        enum class ELogicalColorSpace : std::uint32_t
        {
            CALIBRATED_RGB         = 0,
            DEVICE_RGB             = 1,
            DEVICE_CMYK            = 2,
            EMBEDDED_COLOR_PROFILE = 0x4d424544,  // i.e. 'MBED',
            LINKED_COLOR_PROFILE   = 0x4c494e4b,  // i.e. 'LINK',
            S_RGB                  = 0x73524742,  // i.e. 'sRGB'
            WINDOWS_COLOR_SPACE    = 0x57696e20,  // i.e. 'Win '
        };

        static constexpr ELogicalColorSpace DEFAULT_CS_TYPE{ ELogicalColorSpace::CALIBRATED_RGB };


        //===========================================================================
        enum class ESkippedPixelsMode : std::uint8_t
        {
            BLACK = 0,
            TRANSPARENCY,
            PALETTE_INDEX_0
        };


        //===========================================================================
        using BGRA = union uBGRA {
            std::uint32_t value{ 0 };
            struct {
                std::uint8_t b;
                std::uint8_t g;
                std::uint8_t r;
                std::uint8_t a;
            };
        };

        inline bmpl::utils::LEInStream& operator>> (bmpl::utils::LEInStream& in_stream, BGRA& bgra) noexcept
        {
            return in_stream >> bgra.value;
        }


        //===========================================================================
        using RGBA = union uRGBA {
            std::uint32_t value{ 0 };
            struct {
                std::uint8_t r;
                std::uint8_t g;
                std::uint8_t b;
                std::uint8_t a;
            };
        };

        inline bmpl::utils::LEInStream& operator>> (bmpl::utils::LEInStream& in_stream, RGBA& rgba) noexcept
        {
            return in_stream >> rgba.value;
        }


        //===========================================================================
        using RGB = union uRGB {
            // notice: belowing order of components is only valid for BMP format (little-endian coding of triplets)
            struct {
                std::uint8_t r;
                std::uint8_t g;
                std::uint8_t b;
            };
        };


        //===========================================================================
        using BGR = union uBGR {
            // notice: belowing order of components is only valid for BMP format (little-endian coding of triplets)
            struct {
                std::uint8_t b;
                std::uint8_t g;
                std::uint8_t r;
            };
        };


        //===========================================================================
        using BGRA_HDR = union uBGR64 {
            std::uint64_t value{ 0 };
            struct {
                std::uint16_t b;
                std::uint16_t g;
                std::uint16_t r;
                std::uint16_t a;
            };
        };


        //===========================================================================
        inline void convert(BGRA& bgra, const BGRA& other) noexcept
        {
            bgra = other;
        }

        inline void convert(BGRA& bgra, const RGBA& rgba) noexcept
        {
            bgra.r = rgba.r;
            bgra.g = rgba.g;
            bgra.b = rgba.b;
            bgra.a = rgba.a;
        }

        inline void convert(BGRA& bgra, const BGR& bgr) noexcept
        {
            bgra.r = bgr.r;
            bgra.g = bgr.g;
            bgra.b = bgr.b;
            bgra.a = 0;
        }

        inline void convert(BGRA& bgra, const RGB& rgb) noexcept
        {
            bgra.r = rgb.r;
            bgra.g = rgb.g;
            bgra.b = rgb.b;
            bgra.a = 0;
        }

        inline void convert(BGRA& bgra, const BGRA_HDR& bgra64) noexcept
        {
            bgra.r = (bgra64.r >= 8192) ? 255 : bgra64.r >> 5;
            bgra.g = (bgra64.g >= 8192) ? 255 : bgra64.g >> 5;
            bgra.b = (bgra64.b >= 8192) ? 255 : bgra64.b >> 5;
            bgra.a = (bgra64.a >= 8192) ? 255 : bgra64.a >> 5;
        }

        inline void convert(RGBA& rgba, const BGRA& bgra) noexcept
        {
            rgba.r = bgra.r;
            rgba.g = bgra.g;
            rgba.b = bgra.b;
            rgba.a = bgra.a;
        }

        inline void convert(RGBA& rgba, const RGBA& other) noexcept
        {
            rgba = other;
        }

        inline void convert(RGBA& rgba, const RGB& rgb) noexcept
        {
            rgba.r = rgb.r;
            rgba.g = rgb.g;
            rgba.b = rgb.b;
            rgba.a = 0;
        }

        inline void convert(RGBA& rgba, const BGR& bgr) noexcept
        {
            rgba.r = bgr.r;
            rgba.g = bgr.g;
            rgba.b = bgr.b;
            rgba.a = 0;
        }

        inline void convert(RGBA& rgba, const BGRA_HDR& bgra64) noexcept
        {
            rgba.r = (bgra64.r >= 8192) ? 255 : bgra64.r >> 5;
            rgba.g = (bgra64.g >= 8192) ? 255 : bgra64.g >> 5;
            rgba.b = (bgra64.b >= 8192) ? 255 : bgra64.b >> 5;
            rgba.a = (bgra64.a >= 8192) ? 255 : bgra64.a >> 5;
        }

        inline void convert(RGB& rgb, const BGRA& bgra) noexcept
        {
            rgb.r = bgra.r;
            rgb.g = bgra.g;
            rgb.b = bgra.b;
        }

        inline void convert(RGB& rgb, const RGBA& rgba) noexcept
        {
            rgb.r = rgba.r;
            rgb.g = rgba.g;
            rgb.b = rgba.b;
        }

        inline void convert(RGB& rgb, const BGR& brg) noexcept
        {
            rgb.r = brg.r;
            rgb.g = brg.g;
            rgb.b = brg.b;
        }

        inline void convert(RGB& rgb, const RGB& other) noexcept
        {
            rgb = other;
        }

        inline void convert(RGB& rgb, const BGRA_HDR& bgra64) noexcept
        {
            rgb.r = (bgra64.r >= 8192) ? 255 : bgra64.r >> 5;
            rgb.g = (bgra64.g >= 8192) ? 255 : bgra64.g >> 5;
            rgb.b = (bgra64.b >= 8192) ? 255 : bgra64.b >> 5;
        }

        inline void convert(BGR& brg, const BGRA& bgra) noexcept
        {
            brg.r = bgra.r;
            brg.g = bgra.g;
            brg.b = bgra.b;
        }

        inline void convert(BGR& brg, const RGBA& rgba) noexcept
        {
            brg.r = rgba.r;
            brg.g = rgba.g;
            brg.b = rgba.b;
        }

        inline void convert(BGR& brg, const RGB& rgb) noexcept
        {
            brg.r = rgb.r;
            brg.g = rgb.g;
            brg.b = rgb.b;
        }

        inline void convert(BGR& brg, const BGR& other) noexcept
        {
            brg = other;
        }

        inline void convert(BGR& brg, const BGRA_HDR& bgra64) noexcept
        {
            brg.r = (bgra64.r >= 8192) ? 255 : bgra64.r >> 5;
            brg.g = (bgra64.g >= 8192) ? 255 : bgra64.g >> 5;
            brg.b = (bgra64.b >= 8192) ? 255 : bgra64.b >> 5;
        }


        //===========================================================================
        template<typename PixelT>
        inline void gamma_correction(PixelT& pixel, const double gamma_r, const double gamma_g, const double gamma_b)
        {
            constexpr double coeff{ 255.0 };
            pixel.r = std::uint8_t(coeff * std::pow(pixel.r / coeff, gamma_r));
            pixel.g = std::uint8_t(coeff * std::pow(pixel.g / coeff, gamma_g));
            pixel.b = std::uint8_t(coeff * std::pow(pixel.b / coeff, gamma_b));
        }

        template<>
        inline void gamma_correction(BGRA_HDR& pixel, const double gamma_r, const double gamma_g, const double gamma_b)
        {
            constexpr double coeff{ double(1 << 13) };
            pixel.r = std::uint8_t(coeff * std::pow(pixel.r / coeff, gamma_r));
            pixel.g = std::uint8_t(coeff * std::pow(pixel.g / coeff, gamma_g));
            pixel.b = std::uint8_t(coeff * std::pow(pixel.b / coeff, gamma_b));
        }


        //===========================================================================
        inline void set_full_transparency(RGBA& rgba) noexcept
        {
            rgba.a = 255;
        }


        inline void set_full_transparency(BGRA& bgra) noexcept
        {
            bgra.a = 255;
        }


        inline void set_full_transparency(RGB& rgb) noexcept
        {}


        inline void set_full_transparency(BGR& bgr) noexcept
        {}


        inline void set_full_transparency(BGRA_HDR& bgra64) noexcept
        {
            bgra64.a = 0x1fff;
        }


        //===========================================================================
        inline void set_pixel(RGBA& rgba, const std::uint32_t r, const std::uint32_t g, const std::uint32_t b, const std::uint32_t a) noexcept
        {
            rgba.r = r;
            rgba.g = g;
            rgba.b = b;
            rgba.a = a;
        }


        inline void set_pixel(RGBA& rgba, const std::uint32_t r, const std::uint32_t g, const std::uint32_t b) noexcept
        {
            set_pixel(rgba, r, g, b, 0);
        }


        inline void set_pixel(BGRA& bgra, const std::uint32_t r, const std::uint32_t g, const std::uint32_t b, const std::uint32_t a) noexcept
        {
            bgra.r = r;
            bgra.g = g;
            bgra.b = b;
            bgra.a = a;
        }


        inline void set_pixel(BGRA& bgra, const std::uint32_t r, const std::uint32_t g, const std::uint32_t b) noexcept
        {
            set_pixel(bgra, r, g, b, 0);
        }


        inline void set_pixel(RGB& rgb, const std::uint32_t r, const std::uint32_t g, const std::uint32_t b) noexcept
        {
            rgb.r = r;
            rgb.g = g;
            rgb.b = b;
        }


        inline void set_pixel(RGB& rgb, const std::uint32_t r, const std::uint32_t g, const std::uint32_t b, const std::uint32_t a) noexcept
        {
            set_pixel(rgb, r, g, b);
        }


        inline void set_pixel(BGR& bgr, const std::uint32_t r, const std::uint32_t g, const std::uint32_t b) noexcept
        {
            bgr.r = r;
            bgr.g = g;
            bgr.b = b;
        }


        inline void set_pixel(BGR& bgr, const std::uint32_t r, const std::uint32_t g, const std::uint32_t b, const std::uint32_t a) noexcept
        {
            set_pixel(bgr, r, g, b);
        }

    }
}