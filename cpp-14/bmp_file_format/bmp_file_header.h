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

#include "../utils/errors.h"
#include "../utils/little_endian_streaming.h"


namespace bmpl
{
    namespace frmt
    {
        class BMPFileHeader
        {
        public:

            std::uint32_t size{ 0 };            // bfSize
            std::uint32_t content_offset{ 0 };  // bfOffBits
            std::uint32_t reserved{ 0 };        // bfReserved1 and bfReserved2
            std::uint16_t type{ 0 };            // bfType "BM"


            BMPFileHeader() noexcept = default;
            BMPFileHeader(const BMPFileHeader&) noexcept = default;
            BMPFileHeader(BMPFileHeader&&) noexcept = default;
            virtual ~BMPFileHeader() noexcept = default;

            inline BMPFileHeader& operator= (const BMPFileHeader&) noexcept = default;
            inline BMPFileHeader& operator= (BMPFileHeader&&) noexcept = default;


            inline BMPFileHeader(bmpl::utils::LEInStream& in_stream) noexcept
            {
                load(in_stream);
            }


            inline operator bool() const noexcept
            {
                return is_ok();
            }


            inline const bmpl::utils::ErrorStatus get_error() const noexcept
            {
                return _current_error_status;
            }


            inline const bool is_ok() const noexcept
            {
                return get_error() == bmpl::utils::ErrorStatus::NO_ERROR;
            }


            const bool load(bmpl::utils::LEInStream& in_stream) noexcept;


        private:
            bmpl::utils::ErrorStatus _current_error_status{ bmpl::utils::ErrorStatus::FILE_NOT_INITIALIZED };

            inline const bool _clr_err() noexcept
            {
                _set_err(bmpl::utils::ErrorStatus::NO_ERROR);
                return true;
            }

            inline const bool _set_err(const bmpl::utils::ErrorStatus err_code) noexcept
            {
                _current_error_status = err_code;
                return is_ok();
            }

            inline const bool _set_err(bmpl::utils::LEInStream& in_stream) noexcept
            {
                _set_err(in_stream.get_error());
                return in_stream.is_ok();
            }

        };
    }
}