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

#include "bmp_colormap.h"
#include "bmp_info_header.h"
#include "../utils/errors.h"
#include "../utils/little_endian_streaming.h"


namespace bmpl
{
    namespace frmt
    {
        //===========================================================================
        struct BMPInfo : public bmpl::utils::ErrorStatus
        {
            using MyErrBaseClass = bmpl::utils::ErrorStatus;

            const BMPInfoHeaderBase* info_header_ptr{ nullptr };
            BMPColorMap color_map;


            inline BMPInfo() noexcept = default;
            inline BMPInfo(const BMPInfo&) noexcept = default;
            inline BMPInfo(BMPInfo&&) noexcept = default;

            virtual inline ~BMPInfo() noexcept = default;

            inline BMPInfo& operator= (const BMPInfo&) noexcept = default;
            inline BMPInfo& operator= (BMPInfo&&) noexcept = default;


            inline BMPInfo(bmpl::utils::LEInStream& in_stream, const bmpl::frmt::BMPFileHeaderBase* file_header_ptr) noexcept
                : MyErrBaseClass()
                , info_header_ptr{ bmpl::frmt::create_bmp_info_header(in_stream, file_header_ptr) }
                , color_map(in_stream, file_header_ptr, info_header_ptr)
            {
                if (in_stream.failed())
                    _set_err(in_stream.get_error());
                else if (file_header_ptr == nullptr)
                    _set_err(bmpl::utils::ErrorCode::BAD_FILE_HEADER);
                else if (file_header_ptr->failed())
                    _set_err(file_header_ptr->get_error());
                else if (info_header_ptr == nullptr)
                    _set_err(bmpl::utils::ErrorCode::BAD_INFO_HEADER);
                else if (info_header_ptr->failed())
                    _set_err(info_header_ptr->get_error());
                else if (color_map.failed())
                    _set_err(color_map.get_error());
                else
                    _clr_err();
            }

        };
    }
}