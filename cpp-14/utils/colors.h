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

#include "../utils/little_endian_streaming.h"


namespace bmpl
{
    namespace clr
    {
        struct BGRA
        {
            using Components = std::uint32_t;

            union {
                Components value{ 0 };
                struct {
                    std::uint8_t b;
                    std::uint8_t g;
                    std::uint8_t r;
                    std::uint8_t a;
                };
            } comps;

            inline BGRA() noexcept = default;
            inline BGRA(const BGRA&) noexcept = default;
            inline BGRA(BGRA&&) noexcept = default;
            inline virtual ~BGRA() noexcept = default;

            inline BGRA& operator= (const BGRA&) noexcept = default;
            inline BGRA& operator= (BGRA&&) noexcept = default;

            inline friend bmpl::utils::LEInStream& operator>> (bmpl::utils::LEInStream& in_stream, BGRA& bgra) noexcept
            {
                return in_stream >> bgra.comps.value;
            }
        };


        struct RGBA
        {
            using Components = std::uint32_t;

            union {
                Components value{ 0 };
                struct {
                    std::uint8_t r;
                    std::uint8_t g;
                    std::uint8_t b;
                    std::uint8_t a;
                };
            } comps;

            inline RGBA() noexcept = default;
            inline RGBA(const RGBA&) noexcept = default;
            inline RGBA(RGBA&&) noexcept = default;
            inline virtual ~RGBA() noexcept = default;

            inline RGBA& operator= (const RGBA&) noexcept = default;
            inline RGBA& operator= (RGBA&&) noexcept = default;

            inline RGBA(const BGRA& bgra) noexcept
            {
                set(bgra);
            }

            inline RGBA& operator= (const BGRA& bgra) noexcept
            {
                return set(bgra);
            }

            inline RGBA& set(const BGRA& bgra) noexcept
            {
                this->comps.value = bgra.comps.value;
                this->comps.r = bgra.comps.b;
                this->comps.b = bgra.comps.r;
                return *this;
            }

            inline friend bmpl::utils::LEInStream& operator>> (bmpl::utils::LEInStream& in_stream, RGBA& bgra) noexcept
            {
                return in_stream >> bgra.comps.value;
            }

        };


        struct RGB
        {
            
            struct Components
            {
                std::uint8_t r{ 0 };
                std::uint8_t g{ 0 };
                std::uint8_t b{ 0 };
            };
            
            Components comps;

            inline RGB() noexcept = default;
            inline RGB(const RGB&) noexcept = default;
            inline RGB(RGB&&) noexcept = default;
            inline virtual ~RGB() noexcept = default;

            inline RGB& operator= (const RGB&) noexcept = default;
            inline RGB& operator= (RGB&&) noexcept = default;

            inline RGB(const BGRA bgra) noexcept
            {
                (void) set(bgra);
            }

            inline const RGB& set(const BGRA bgra) noexcept
            {
                comps.r = bgra.comps.r;
                comps.g = bgra.comps.g;
                comps.b = bgra.comps.b;
                return *this;
            }

        };


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

    }
}