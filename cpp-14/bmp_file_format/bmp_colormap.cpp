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


namespace bmpl
{
    namespace frmt
    {
        //===========================================================================
        const bool BMPColorMap::load(bmpl::utils::LEInStream& in_stream, const bmpl::frmt::BMPInfoHeaderBase* info_header_ptr) noexcept
        {
            if (in_stream.failed())
                return _set_err(in_stream.get_error());

            if (info_header_ptr == nullptr)
                return _set_err(bmpl::utils::ErrorCode::BAD_INFO_HEADER);

            if (info_header_ptr->failed())
                return _set_err(info_header_ptr->get_error());

            this->colors_count = info_header_ptr->get_colors_count();

            if (this->colors_count > 0) {
                std::size_t to_be_loaded_count{ this->colors_count };
                if (to_be_loaded_count > 256) {
                    to_be_loaded_count = 256;
                    set_warning(bmpl::utils::WarningCode::TOO_BIG_PALETTE);
                }

                if (info_header_ptr->is_vOS21() || info_header_ptr->is_vOS22()) {
                    // 3 bytes per color map entry
                    auto cmap_it = MyContainerBaseClass::begin();
                    bmpl::clr::BGR bgr;
                    for (std::uint32_t i = 0; i < to_be_loaded_count; ++i) {
                        in_stream >> bgr.b >> bgr.g >> bgr.r;
                        bmpl::clr::convert(*cmap_it++, bgr);
                    }
                    if (in_stream.failed())
                        return _set_err(bmpl::utils::ErrorCode::BAD_COLORMAP_ENCODING);
                }
                else if (bmpl::utils::PLATFORM_IS_LITTLE_ENDIAN) {
                    if (!in_stream.read(reinterpret_cast<char*>(MyContainerBaseClass::data()), std::streamsize(4 * to_be_loaded_count)))
                        return _set_err(bmpl::utils::ErrorCode::BAD_COLORMAP_ENCODING);
                }
                else {
                    bmpl::clr::BGRA bgra;
                    auto cmap_it = MyContainerBaseClass::begin();
                    for (std::uint32_t i = 0; i < to_be_loaded_count; ++i) {
                        in_stream >> bgra;
                        bmpl::clr::convert(*cmap_it++, bgra);
                    }
                    if (in_stream.failed())
                        return _set_err(bmpl::utils::ErrorCode::BAD_COLORMAP_ENCODING);
                }

                if (to_be_loaded_count < this->colors_count)
                    if (!in_stream.seekg(4 * (this->colors_count - to_be_loaded_count), std::ios_base::cur))
                        return _set_err(bmpl::utils::ErrorCode::BAD_COLORMAP_ENCODING);
            }

            // once here, everything was fine
            return _clr_err();
        }

    }
}
