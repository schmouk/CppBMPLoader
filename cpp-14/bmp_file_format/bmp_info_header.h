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
#include "../utils/warnings.h"


namespace bmpl
{
    namespace frmt
    {
        //===========================================================================
        class BMPInfoHeader : public bmpl::utils::ErrorStatus, public bmpl::utils::WarningStatus
        {
        public:
            using MyErrBaseClass = bmpl::utils::ErrorStatus;
            using MyWarnBaseClass = bmpl::utils::WarningStatus;


            static constexpr int NO_RLE{ 0 };
            static constexpr int RLE_8{ 1 };
            static constexpr int RLE_4{ 2 };
            static constexpr int RLE_COLOR_BITMASKS{ 3 };


            std::uint32_t size{ 0 };                    // biSize
            std::int32_t width{ 0 };                    // biWidth
            std::int32_t height{ 0 };                   // biHeight
            std::uint16_t planes_count{ 0 };            // biPlanes
            std::uint16_t bits_per_pixel{ 0 };          // biBitCount
            std::uint32_t compression_mode{ 0 };        // biCompression
            std::uint32_t image_size{ 0 };              // biSizeImage
            std::int32_t device_x_resolution{ 0 };      // biXpelsPerMeter
            std::int32_t device_y_resolution{ 0 };      // biYpelsPerMeter
            std::uint32_t used_colors_count{ 0 };       // biClrUsed
            std::uint32_t important_colors_count{ 0 };  // biClrImportant

            // this is for BMP Version 4 format (Windows 95 and above)
            std::uint32_t red_mask{ 0 };       // Mask identifying bits of red component 
            std::uint32_t green_mask{ 0 };     // Mask identifying bits of green component 
            std::uint32_t blue_mask{ 0 };      // Mask identifying bits of blue component 
            std::uint32_t alpha_mask{ 0 };     // Mask identifying bits of alpha component 
            std::uint32_t cs_type{ 0 };        // Color space type 
            std::int32_t red_endX{ 0 };        // X coordinate of red endpoint
            std::int32_t red_endY{ 0 };        // Y coordinate of red endpoint
            std::int32_t red_endZ{ 0 };        // Z coordinate of red endpoint
            std::int32_t green_endX{ 0 };      // X coordinate of green endpoint
            std::int32_t green_endY{ 0 };      // Y coordinate of green endpoint
            std::int32_t green_endZ{ 0 };      // Z coordinate of green endpoint
            std::int32_t blue_endX{ 0 };       // X coordinate of blue endpoint
            std::int32_t blue_endY{ 0 };       // Y coordinate of blue endpoint
            std::int32_t blue_endZ{ 0 };       // Z coordinate of blue endpoint
            std::uint32_t gamma_red{ 0 };      // Gamma red coordinate scale value
            std::uint32_t gamma_green{ 0 };    // Gamma green coordinate scale value
            std::uint32_t gamma_blue{ 0 };     // Gamma blue coordinate scale value


            BMPInfoHeader() noexcept = default;
            BMPInfoHeader(const BMPInfoHeader&) noexcept = default;
            BMPInfoHeader(BMPInfoHeader&&) noexcept = default;
            virtual ~BMPInfoHeader() noexcept = default;


            inline BMPInfoHeader& operator= (const BMPInfoHeader&) noexcept = default;
            inline BMPInfoHeader& operator= (BMPInfoHeader&&) noexcept = default;


            inline BMPInfoHeader(bmpl::utils::LEInStream& in_stream) noexcept
                : MyErrBaseClass()
                , MyWarnBaseClass()
            {
                load(in_stream);
            }


            const bool load(bmpl::utils::LEInStream& in_stream) noexcept;

        };
    }
}