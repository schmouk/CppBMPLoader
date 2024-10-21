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
* function  as  well  when  compiled  with  standard  c++11  since  no   c++14
* specificities have been used there, but it has not been tested as such.
*/

#include <vector>

#include "bmp_colormap.h"
#include "bmp_file_header.h"
#include "bmp_info.h"
#include "bmp_info_header.h"

#include "../utils/errors.h"
#include "../utils//list_with_status.h"
#include "../utils/little_endian_streaming.h"


namespace bmpl
{
    namespace frmt
    {
        //===========================================================================
        struct BAHeader : public bmpl::utils::ErrorStatus, public bmpl::utils::WarningStatus
        {
            using MyErrBaseClass = bmpl::utils::ErrorStatus;
            using MyWarnBaseClass = bmpl::utils::WarningStatus;

            // Caution: keep the ordering of next four declarations as it is
            BMPFileHeaderBA ba_file_header{};
            const BMPFileHeaderBase* file_header_ptr{ nullptr };
            BMPInfoHeaderBase* info_header_ptr{ nullptr };
            BMPColorMap color_map{};


            inline BAHeader() noexcept = default;
            inline BAHeader(const BAHeader&) noexcept = default;
            inline BAHeader(BAHeader&&) noexcept = default;

            virtual inline ~BAHeader() noexcept = default;

            inline BAHeader& operator= (const BAHeader&) noexcept = default;
            inline BAHeader& operator= (BAHeader&&) noexcept = default;


            inline BAHeader(bmpl::utils::LEInStream& in_stream) noexcept
                : MyErrBaseClass()
                , MyWarnBaseClass()
                , ba_file_header{ BMPFileHeaderBA(in_stream) }
                , file_header_ptr{ create_file_header(in_stream, true) }
                , info_header_ptr{ create_bmp_info_header(in_stream, file_header_ptr) }
                , color_map(in_stream, file_header_ptr, info_header_ptr)
            {
                if (in_stream.failed())
                    _set_err(in_stream.get_error());
                else if (ba_file_header.failed())
                    _set_err(ba_file_header.get_error());
                else if (file_header_ptr == nullptr)
                    _set_err(bmpl::utils::ErrorCode::BAD_BITS_PER_PIXEL_VALUE);
                else if (file_header_ptr->failed())
                    _set_err(file_header_ptr->get_error());
                else if (info_header_ptr == nullptr)
                    _set_err(bmpl::utils::ErrorCode::BAD_INFO_HEADER);
                else if (info_header_ptr->failed())
                    _set_err(info_header_ptr->get_error());
                else if (color_map.failed())
                    _set_err(color_map.get_error());
                else {
                    if (!info_header_ptr->is_vOS21() && !info_header_ptr->is_vOS22())
                        set_warning(bmpl::utils::WarningCode::NOT_OS2_BITMAP_FORMAT);
                    _clr_err();
                }
            }

            [[nodiscard]]
            inline const std::uint32_t get_colors_count() const noexcept
            {
                if (this->info_header_ptr == nullptr)
                    return 0;
                else
                    return this->info_header_ptr->get_colors_count();
            }

            [[nodiscard]]
            inline const std::size_t get_content_offset() const noexcept
            {
                return this->file_header_ptr->get_content_offset();
            }

            [[nodiscard]]
            inline const std::size_t get_offset_to_next() const noexcept
            {
                return this->ba_file_header.get_content_offset();
            }

            [[nodiscard]]
            inline const std::int32_t get_device_x_resolution_dpi() const noexcept
            {
                if (this->info_header_ptr == nullptr)
                    return 0;
                else
                    return std::int32_t(this->info_header_ptr->get_device_x_resolution() * 2.54 / 100 + 0.5);
            }

            [[nodiscard]]
            inline const std::int32_t get_device_y_resolution_dpi() const noexcept
            {
                if (this->info_header_ptr == nullptr)
                    return 0;
                else
                    return std::int32_t(this->info_header_ptr->get_device_y_resolution() * 2.54 / 100 + 0.5);
            }

            [[nodiscard]]
            inline const std::uint32_t get_height() const noexcept
            {
                if (this->info_header_ptr == nullptr)
                    return 0;
                else
                    return this->info_header_ptr->get_height();
            }

            [[nodiscard]]
            inline const std::uint32_t get_width() const noexcept
            {
                if (this->info_header_ptr == nullptr)
                    return 0;
                else
                    return this->info_header_ptr->get_width();
            }

            [[nodiscard]]
            inline const bool is_last_header_in_list() const noexcept
            {
                return get_offset_to_next() == std::size_t(0);
            }

        };


        //===========================================================================
        using BAHeadersList = bmpl::utils::ListWithStatus<bmpl::frmt::BAHeader>;


        //===========================================================================
        class BAHeadersIterStatus : public bmpl::utils::ErrorStatus
        {
        public:
            BAHeadersIterStatus() noexcept = default;

            inline BAHeadersIterStatus(const BAHeadersList& ba_headers_list) noexcept
                : bmpl::utils::ErrorStatus(bmpl::utils::ErrorCode::NO_ERROR)
                , _iter(ba_headers_list.cbegin())
                , _sentinel(ba_headers_list.cend())
                , _inited(true)
            {}


            inline const bmpl::frmt::BAHeader& operator*() noexcept
            {
                if (this->_inited && this->_iter != this->_sentinel) {
                    return *(this->_iter);
                }
                else {
                    _set_err(bmpl::utils::ErrorCode::END_OF_BA_HEADERS_LIST);
                    return BAHeader();  // notice: method .failed() returns true on an empty-constructed BA header; the associated error code is NOT_INITIALIZED
                }
            }


            inline BAHeadersList::const_iterator operator++() noexcept  // notice: pre-increment
            {
                if (this->_inited && this->_iter != this->_sentinel) {
                    return ++(this->_iter);
                }
                else {
                    _set_err(bmpl::utils::ErrorCode::END_OF_BA_HEADERS_LIST);
                    return this->_sentinel;
                }
            }


            inline BAHeadersList::const_iterator operator++(int) noexcept  // notice: post-increment
            {
                if (this->_inited && this->_iter != this->_sentinel) {
                    BAHeadersList::const_iterator ret_iter{ this->_iter };
                    this->_iter++;
                    return ret_iter;
                }
                else {
                    _set_err(bmpl::utils::ErrorCode::END_OF_BA_HEADERS_LIST);
                    return this->_sentinel;
                }
            }


            inline void set(const BAHeadersList& ba_headers_list) noexcept
            {
                this->_iter = ba_headers_list.cbegin();
                this->_sentinel = ba_headers_list.cend();
                this->_inited = true;
                _clr_err();
            }

        private:
            BAHeadersList::const_iterator _iter{};
            BAHeadersList::const_iterator _sentinel{};
            bool _inited{ false };

        };

    }

}