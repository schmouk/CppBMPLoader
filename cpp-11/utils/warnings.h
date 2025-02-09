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


#include <algorithm>
#include <cstdint>
#include <string>
#include <strstream>
#include <vector>


namespace bmpl
{
    namespace utils
    {
        //===========================================================================
        enum class WarningCode : std::uint8_t
        {
            NO_WARNING = 0,
            ALPHA_16_BITS_IMAGE,
            BAD_BITMAP_SIZE_IN_HEADER,
            BAD_FILE_SIZE_IN_HEADER,
            BAD_HALFTONING_MODE_VALUE,
            BAD_IMPORTANT_COLORS_COUNT,
            BAD_PALETTE_INDICES,
            BAD_PALETTE_SIZE_IN_HEADER,
            BAD_PLANES_VALUE,
            BAD_SCANLINE_BYTES_WIDTH,
            BITFIELDS_AND_PALETT,
            CORRECTED_GAMMA_BLUE,
            CORRECTED_GAMMA_GREEN,
            CORRECTED_GAMMA_RED,
            DELTA_MODE_MAY_OVERFLOW,
            EMBEDDED_PROFILE_NOT_IMPLEMENTED,
            FORBIDDEN_TOP_DOWN_ORIENTATION,
            GAP_BTW_COLORMAP_AND_BITMAP,
            HOT_POINT_SET,
            INCOHERENT_IMAGE_SIZE,
            INCOHERENT_RESOLUTIONS,
            INVALID_BA_FILE_HEADER_SIZE,
            INVALID_COLOR_ENCODING,
            INVALID_DAMPING_VALUE,
            INVALID_RESOLUTION_UNITS,
            LINKED_PROFILE_NOT_IMPLEMENTED,
            MISSING_BITFIELDS_MASKS,
            MISSING_COLORMAP_ENTRIES,
            NOT_ENOUGH_INDICES_IN_BITMAP,
            NOT_OS2_BITMAP_FORMAT,
            NOT_ZERO_RESERVED,
            PALETT_WITH_FULL_COLORS,
            SOME_GAP_BTW_PALETTE_AND_BITMAP,
            TOO_BIG_PALETTE,
            TOO_MANY_INDICES_IN_BITMAP,
            UNUSED_BITS_SET_TO_1,
            UNUSED_PALETTE,
            WIN_CE_2_BITS_PIXELS,
        };


        //===========================================================================
        using WarningsList = std::vector<WarningCode>;


        //===========================================================================
        const std::string warning_msg(const WarningCode err_code) noexcept;

        const std::string warning_msg(const std::string& file_path, const WarningCode err_code) noexcept;


        //===========================================================================
        class WarningStatus
        {
        public:
            WarningStatus() noexcept = default;
            WarningStatus(const WarningStatus&) noexcept = default;
            WarningStatus(WarningStatus&&) noexcept = default;

            virtual ~WarningStatus() noexcept = default;

            WarningStatus& operator= (const WarningStatus&) noexcept = default;
            WarningStatus& operator= (WarningStatus&&) noexcept = default;


            inline void append_warnings(const WarningStatus& other) noexcept
            {
                if (other.has_warnings())
                    this->_warnings_list.insert(this->_warnings_list.end(), other._warnings_list.begin(), other._warnings_list.end());
            }


            inline operator bool() const noexcept
            {
                return has_warnings();
            }


            inline const WarningsList& get_warnings() const noexcept
            {
                return this->_warnings_list;
            }


            inline const bool has_warnings() const noexcept
            {
                return !this->_warnings_list.empty();
            }


            void set_unique_warnings();


            inline void set_warning(const WarningCode warn_code) noexcept
            {
                this->_warnings_list.push_back(warn_code);
            }


        protected:
            WarningsList _warnings_list{};

            inline void _clr_warnings() noexcept
            {
                this->_warnings_list.clear();
            }

        };

    }
}