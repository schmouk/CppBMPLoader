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
#include <utility>

#include "../utils/little_endian_streaming.h"


namespace bmpl
{
    namespace clr
    {
        //===========================================================================
        using BGRA = union uBGRA {
            std::uint32_t value{ 0 };
            struct {
                std::uint8_t b;
                std::uint8_t g;
                std::uint8_t r;
                std::uint8_t a;
            };
            std::uint8_t comps[4];

            static constexpr int R{ 2 };
            static constexpr int G{ 1 };
            static constexpr int B{ 0 };
            static constexpr int A{ 3 };
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
            std::uint8_t comps[4];

            static constexpr int R{ 0 };
            static constexpr int G{ 1 };
            static constexpr int B{ 2 };
            static constexpr int A{ 3 };
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
            std::uint8_t comps[3]{ 0, 0, 0 };

            static constexpr int R{ 0 };
            static constexpr int G{ 1 };
            static constexpr int B{ 2 };
        };


        //===========================================================================
        using BGR = union uBGR {
            // notice: belowing order of components is only valid for BMP format (little-endian coding of triplets)
            struct {
                std::uint8_t b;
                std::uint8_t g;
                std::uint8_t r;
            };
            std::uint8_t comps[3]{ 0, 0, 0 };

            static constexpr int R{ 2 };
            static constexpr int G{ 1 };
            static constexpr int B{ 0 };
        };


        //===========================================================================
        inline void convert(RGBA& rgba, const BGRA& bgra) noexcept
        {
            rgba.value = bgra.value;
            /*
            rgba.r = bgra.b;
            rgba.b = bgra.r;
            */
            std::swap(rgba.r, rgba.b);
        }

        inline void convert(BGRA& bgra, const RGBA& rgba) noexcept
        {
            bgra.value = rgba.value;
            /*
            bgra.r = rgba.b;
            bgra.b = rgba.r;
            */
            std::swap(bgra.b, bgra.r);
        }

        inline void convert(BGRA& bgra, const RGB& rgb) noexcept
        {
            bgra.r = rgb.r;
            bgra.g = rgb.b;
            bgra.b = rgb.g;
            bgra.a = 0;
        }

        inline void convert(BGRA& bgra, const BGR& bgr) noexcept
        {
            bgra.r = bgr.r;
            bgra.g = bgr.b;
            bgra.b = bgr.g;
            bgra.a = 0;
        }

        inline void convert(RGBA& rgba, const RGB& rgb) noexcept
        {
            rgba.r = rgb.r;
            rgba.g = rgb.b;
            rgba.b = rgb.g;
            rgba.a = 0;
        }

        inline void convert(RGBA& rgba, const BGR& bgr) noexcept
        {
            rgba.r = bgr.r;
            rgba.g = bgr.b;
            rgba.b = bgr.g;
            rgba.a = 0;
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

        inline void convert(BGR& brg, const RGB& rgb) noexcept
        {
            brg.r = rgb.r;
            brg.g = rgb.g;
            brg.b = rgb.b;
        }


        //===========================================================================
        template<typename ClrT>
        struct is_color
        {
            static constexpr bool value{ false };
        };

        template<typename ClrT>
        inline constexpr bool is_color_v()
        {
            return is_color<ClrT>::value;
        }


        template<>
        struct is_color<BGRA>
        {
            static constexpr bool value{ true };
        };

        template<>
        struct is_color<RGBA>
        {
            static constexpr bool value{ true };
        };

        template<>
        struct is_color<RGB>
        {
            static constexpr bool value{ true };
        };

        template<>
        struct is_color<BGR>
        {
            static constexpr bool value{ true };
        };

    }
}