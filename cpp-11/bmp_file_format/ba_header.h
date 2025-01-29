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

#include <map>
#include <string>
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
        // Forward declaration
        struct BAHeader;


        //===========================================================================
        using BAHeadersList = bmpl::utils::ListWithStatus<bmpl::frmt::BAHeader>;


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


            BAHeader() noexcept = default;
            BAHeader(const BAHeader&) noexcept = default;
            BAHeader(BAHeader&&) noexcept = default;

            virtual ~BAHeader() noexcept = default;

            BAHeader& operator= (const BAHeader&) noexcept = default;
            BAHeader& operator= (BAHeader&&) noexcept = default;


            BAHeader(bmpl::utils::LEInStream& in_stream) noexcept;


            [[nodiscard]]
            static BAHeadersList get_BA_headers(const std::string& filepath) noexcept;

            [[nodiscard]]
            static BAHeadersList get_BA_headers(bmpl::utils::LEInStream& in_stream) noexcept;

            [[nodiscard]]
            const std::uint32_t get_bits_per_pixel() const noexcept;

            [[nodiscard]]
            const std::uint32_t get_colors_count() const noexcept;

            [[nodiscard]]
            const std::size_t get_content_offset() const noexcept;

            [[nodiscard]]
            const std::int32_t get_device_x_resolution_dpi() const noexcept;

            [[nodiscard]]
            const std::int32_t get_device_y_resolution_dpi() const noexcept;

            [[nodiscard]]
            const std::uint32_t get_height() const noexcept;

            [[nodiscard]]
            const std::size_t get_offset_to_next() const noexcept;

            [[nodiscard]]
            const std::uint32_t get_width() const noexcept;

            [[nodiscard]]
            static const bool is_BA_file(const std::string& filepath) noexcept;

            [[nodiscard]]
            static const bool is_BA_file(bmpl::utils::LEInStream& in_stream) noexcept;

            [[nodiscard]]
            const bool is_last_header_in_list() const noexcept;

        };


        //===========================================================================
        class BAHeadersIterStatus : public bmpl::utils::ErrorStatus
        {
        public:
            bmpl::utils::LEInStream* in_stream_ptr{ nullptr };

            BAHeadersIterStatus() noexcept = default;
            BAHeadersIterStatus(const BAHeadersIterStatus&) noexcept = default;

            virtual ~BAHeadersIterStatus() noexcept = default;

            BAHeadersIterStatus(const std::string& filepath, const BAHeadersList& ba_headers_list) noexcept;

            const bmpl::frmt::BAHeader& operator*() noexcept;
            BAHeadersList::const_iterator operator++() noexcept;        // notice: pre-increment
            BAHeadersList::const_iterator operator++(int) noexcept;     // notice: post-increment

            [[nodiscard]]
            const bool end() const noexcept;

            void reset() noexcept;


        private:
            static const BAHeader _EMPTY_BA_HEADER;

            BAHeadersList::const_iterator _begin{};
            BAHeadersList::const_iterator _iter{};
            BAHeadersList::const_iterator _sentinel{};

        };

    }

}