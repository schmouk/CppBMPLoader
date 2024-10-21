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
* function  as  well  when  compiled  with  standard  c++11  since  no   c++14
* specificities have been used there, but it has not been tested as such.
*/


#include "bmp_colormap.h"
#include "../utils/default_palettes.h"


namespace bmpl
{
    namespace frmt
    {
        //===========================================================================
        BMPColorMap::BMPColorMap(
            bmpl::utils::LEInStream& in_stream,
            const bmpl::frmt::BMPFileHeaderBase* file_header_ptr,
            const bmpl::frmt::BMPInfoHeaderBase* info_header_ptr
        ) noexcept
            : MyErrBaseClass()
            , MyWarnBaseClass()
            , MyContainerBaseClass()
        {
            load(in_stream, file_header_ptr, info_header_ptr);
        }


        //===========================================================================
        BMPColorMap::pixel_type& BMPColorMap::operator[] (const std::uint32_t index) noexcept
        {
            if (index >= this->colors_count) {
                if (!_bad_index_warn_already_set) {
                    set_warning(bmpl::utils::WarningCode::BAD_PALETTE_INDICES);
                    _bad_index_warn_already_set = true;
                }
                // notice: we use entry 0 as the default color for bad indices
                return MyContainerBaseClass::operator[](0);
            }
            else {
                return MyContainerBaseClass::operator[](index);
            }
        }


        //===========================================================================
        const bool BMPColorMap::load(
            bmpl::utils::LEInStream& in_stream,
            const bmpl::frmt::BMPFileHeaderBase* file_header_ptr,
            const bmpl::frmt::BMPInfoHeaderBase* info_header_ptr) noexcept
        {
            if (in_stream.failed())
                return _set_err(in_stream.get_error());

            if (file_header_ptr == nullptr)
                return _set_err(bmpl::utils::ErrorCode::BAD_FILE_HEADER);

            if (file_header_ptr->failed())
                _set_err(file_header_ptr->get_error());

            if (file_header_ptr->is_BA_file())
                return _clr_err();

            if (info_header_ptr == nullptr)
                return _set_err(bmpl::utils::ErrorCode::BAD_INFO_HEADER);

            if (info_header_ptr->failed())
                return _set_err(info_header_ptr->get_error());

            if (!info_header_ptr->may_embed_color_palette())
                _set_err(bmpl::utils::ErrorCode::INCOHERENT_BMP_LOADER_IMPLEMENTATION);

            this->colors_count = info_header_ptr->get_colors_count();

            if (info_header_ptr->is_v1()) {
                // this is a V1.X BMP format
                auto cmap_it = MyContainerBaseClass::begin();

                for (std::uint32_t i = 0; i < info_header_ptr->used_colors_count; ++i) {
                    switch (info_header_ptr->used_colors_count) {
                    case 2:
                        bmpl::clr::convert(*cmap_it++, bmpl::clr::WindowsDefaultPalettes::get_default_2()[i]);
                        break;
                    case 16:
                        bmpl::clr::convert(*cmap_it++, bmpl::clr::WindowsDefaultPalettes::get_default_16()[i]);
                        break;
                    default:
                        bmpl::clr::convert(*cmap_it++, bmpl::clr::WindowsDefaultPalettes::get_default_256()[i]);
                        break;
                    }
                }
            }
            else if (this->colors_count > 0) {
                const std::size_t palette_size{ file_header_ptr->get_content_offset() - file_header_ptr->get_header_size() - info_header_ptr->header_size };
                std::size_t expected_colors_count{ palette_size / 4 };
                std::uint32_t bytes_per_palette_color{ 4 };

                std::size_t to_be_loaded_count{ this->colors_count };
                if (to_be_loaded_count > 256) {
                    to_be_loaded_count = 256;
                    set_warning(bmpl::utils::WarningCode::TOO_BIG_PALETTE);
                }

                if (info_header_ptr->is_vOS21()) {
                    // 3 bytes per color map entry
                    bytes_per_palette_color = 3;
                    expected_colors_count = palette_size / 3;

                    if (to_be_loaded_count > expected_colors_count) {
                        to_be_loaded_count = expected_colors_count;
                        set_warning(bmpl::utils::WarningCode::TOO_BIG_PALETTE);
                    }

                    auto cmap_it = MyContainerBaseClass::begin();
                    bmpl::clr::BGR bgr{};
                    for (std::size_t i = 0; i < to_be_loaded_count; ++i) {
                        if ((in_stream >> bgr.b >> bgr.g >> bgr.r).failed())
                            break;
                        bmpl::clr::convert(*cmap_it++, bgr);
                    }
                    if (in_stream.failed())
                        return _set_err(bmpl::utils::ErrorCode::BAD_COLORMAP_ENCODING);
                }
                else if (bmpl::utils::PLATFORM_IS_LITTLE_ENDIAN) {
                    if (to_be_loaded_count > expected_colors_count) {
                        to_be_loaded_count = expected_colors_count;
                        set_warning(bmpl::utils::WarningCode::TOO_BIG_PALETTE);
                    }
                    if (!in_stream.read(reinterpret_cast<char*>(MyContainerBaseClass::data()), std::streamsize(bytes_per_palette_color * to_be_loaded_count)))
                        return _set_err(bmpl::utils::ErrorCode::BAD_COLORMAP_ENCODING);
                }
                else {
                    if (to_be_loaded_count > expected_colors_count) {
                        to_be_loaded_count = expected_colors_count;
                        set_warning(bmpl::utils::WarningCode::TOO_BIG_PALETTE);
                    }
                    bmpl::clr::BGRA bgra;
                    auto cmap_it = MyContainerBaseClass::begin();
                    for (std::size_t i = 0; i < to_be_loaded_count; ++i) {
                        if ((in_stream >> bgra).failed())
                            break;;
                        bmpl::clr::convert(*cmap_it++, bgra);
                    }
                    if (in_stream.failed())
                        return _set_err(bmpl::utils::ErrorCode::BAD_COLORMAP_ENCODING);
                }

                const std::size_t expected_palette_size{ bytes_per_palette_color * expected_colors_count };
                const std::size_t expected_bitmap_offset{ file_header_ptr->get_header_size() + info_header_ptr->header_size + expected_palette_size };
                if (expected_bitmap_offset < file_header_ptr->get_content_offset())
                    set_warning(bmpl::utils::WarningCode::GAP_BTW_COLORMAP_AND_BITMAP);
                else if (expected_bitmap_offset > file_header_ptr->get_content_offset())
                    set_warning(bmpl::utils::WarningCode::MISSING_COLORMAP_ENTRIES);

            }

            // once here, everything was fine
            return _clr_err();
        }

    }
}
