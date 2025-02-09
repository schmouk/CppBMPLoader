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


#include <array>
#include <cstdint>
#include <type_traits>

#include "bmp_info_header.h"
#include "../utils/colors.h"
#include "../utils/errors.h"
#include "../utils/little_endian_streaming.h"
#include "../utils/warnings.h"


namespace bmpl
{
    namespace frmt
    {
        //===========================================================================
        class BMPColorMap : public bmpl::utils::ErrorStatus, public bmpl::utils::WarningStatus, public std::array<bmpl::clr::BGRA, 256>
        {
        public:
            using MyErrBaseClass = bmpl::utils::ErrorStatus;
            using MyWarnBaseClass = bmpl::utils::WarningStatus;
            using MyContainerBaseClass = std::array<bmpl::clr::BGRA, 256>;

            using pixel_type = bmpl::clr::BGRA;

            std::uint32_t colors_count{ 0 };


            BMPColorMap() noexcept = default;
            BMPColorMap(const BMPColorMap&) noexcept = default;
            BMPColorMap(BMPColorMap&&) noexcept = default;

            virtual ~BMPColorMap() noexcept = default;

            BMPColorMap& operator= (const BMPColorMap&) noexcept = default;
            BMPColorMap& operator= (BMPColorMap&&) noexcept = default;

            BMPColorMap(
                bmpl::utils::LEInStream& in_stream,
                const bmpl::frmt::BMPFileHeaderBase* file_header_ptr,
                const bmpl::frmt::BMPInfoHeaderBase* info_header_ptr
            ) noexcept;

            const bool load(
                bmpl::utils::LEInStream& in_stream,
                const bmpl::frmt::BMPFileHeaderBase* file_header_ptr,
                const bmpl::frmt::BMPInfoHeaderBase* info_header_ptr
            ) noexcept;

            pixel_type& operator[] (const std::uint32_t index) noexcept;

            inline const pixel_type& operator[] (const std::uint32_t index) const noexcept
            {
                return operator[](index);
            }


        private:
            bool _bad_index_warn_already_set{ false };

        };

    }

}
