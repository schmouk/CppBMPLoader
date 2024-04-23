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


namespace bmpl
{
    namespace clr
    {
        using BGRA = std::uint32_t;

        using RGBA = std::uint32_t;


        inline const RGBA bgra_to_rgba(const BGRA bgra) noexcept
        {
            const std::uint32_t b{ (bgra & 0xff'00'00'00) >> 24 };
            const std::uint32_t g{ (bgra & 0x00'ff'00'00) >> 8 };
            const std::uint32_t r{ (bgra & 0x00'00'ff'00) << 8 };
            return r | g | b;
        }


        struct RGB
        {
            std::uint8_t r{ 0 };
            std::uint8_t g{ 0 };
            std::uint8_t b{ 0 };

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

            [[nodiscard]]
            inline const RGB& set(const RGBA bgra) noexcept
            {
                r = (bgra & 0x00'00'ff'00) << 8;
                g = (bgra & 0x00'ff'00'00) >> 8;
                b = (bgra & 0xff'00'00'00) >> 24;
                return *this;
            }

        };
    }
}