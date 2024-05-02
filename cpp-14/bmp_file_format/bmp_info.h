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

#include "bmp_color_pallett.h"
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
            using MyBaseClass = bmpl::utils::ErrorStatus;

            BMPInfoHeader info_header;
            BMPColorPallett color_pallett;


            inline BMPInfo() noexcept = default;
            inline BMPInfo(const BMPInfo&) noexcept = default;
            inline BMPInfo(BMPInfo&&) noexcept = default;

            virtual inline ~BMPInfo() noexcept = default;

            inline BMPInfo& operator= (const BMPInfo&) noexcept = default;
            inline BMPInfo& operator= (BMPInfo&&) noexcept = default;


            inline BMPInfo(bmpl::utils::LEInStream& in_stream) noexcept
                : MyBaseClass()
                , info_header(in_stream)
                , color_pallett(in_stream, info_header)
            {
                if (in_stream.failed())
                    _set_err(in_stream.get_error());
                else if (info_header.failed())
                    _set_err(info_header.get_error());
                else if (color_pallett.failed())
                    _set_err(color_pallett.get_error());
                else
                    _clr_err();
            }

        };
    }
}