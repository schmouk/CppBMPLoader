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


#include <array>
#include <cstdint>

#include "../utils/colors.h"
#include "../utils/errors.h"
#include "../utils/little_endian_streaming.h"


namespace bmpl
{
    namespace frmt
    {
        struct BMPColorPallett : std::array<bmpl::clr::RGBA, 256>, bmpl::utils::ErrorStatus
        {
            using MyContainerBaseClass = std::array<bmpl::clr::RGBA, 256>;
            using MyErrorBaseClass = bmpl::utils::ErrorStatus;


            std::uint32_t colors_count{ 0 };


            inline BMPColorPallett() noexcept = default;
            inline BMPColorPallett(const BMPColorPallett&) noexcept = default;
            inline BMPColorPallett(BMPColorPallett&&) noexcept = default;

            virtual inline ~BMPColorPallett() noexcept = default;

            inline BMPColorPallett& operator= (const BMPColorPallett&) noexcept = default;
            inline BMPColorPallett& operator= (BMPColorPallett&&) noexcept = default;


            inline BMPColorPallett(bmpl::utils::LEInStream& in_stream, const std::uint32_t colors_count_) noexcept
                : MyContainerBaseClass()
                , MyErrorBaseClass()
                , colors_count(0)
            {
                load(in_stream, colors_count_);
            }


            const bool load(bmpl::utils::LEInStream& in_stream, const std::uint32_t colors_count) noexcept;


            const bmpl::clr::RGBA& operator[] (const std::uint32_t index) noexcept;

        };

    }
}
