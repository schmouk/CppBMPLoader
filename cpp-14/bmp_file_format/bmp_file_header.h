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
#include "../utils//warnings.h"


namespace bmpl
{
    namespace frmt
    {
        //===========================================================================
        struct BMPFileHeaderBase : public bmpl::utils::ErrorStatus, public bmpl::utils::WarningStatus
        {
            using MyErrBaseClass = bmpl::utils::ErrorStatus;
            using MyWarnBaseClass = bmpl::utils::WarningStatus;

            std::uint32_t file_size{ 0 };

            BMPFileHeaderBase() noexcept = default;
            BMPFileHeaderBase(const BMPFileHeaderBase&) noexcept = default;
            BMPFileHeaderBase(BMPFileHeaderBase&&) noexcept = default;
            virtual ~BMPFileHeaderBase() noexcept = default;

            inline BMPFileHeaderBase& operator= (const BMPFileHeaderBase&) noexcept = default;
            inline BMPFileHeaderBase& operator= (BMPFileHeaderBase&&) noexcept = default;


            inline BMPFileHeaderBase(bmpl::utils::LEInStream& in_stream) noexcept
                : MyErrBaseClass()
                , MyWarnBaseClass()
            {
                load(in_stream);
            }

            virtual inline const std::size_t get_content_offset() const noexcept
            {
                return 0;
            }

            inline const std::size_t get_file_size() const noexcept
            {
                return std::size_t(file_size);
            }

            virtual inline const std::size_t get_header_size() const noexcept
            {
                return 0;
            }

            virtual inline const bool load(bmpl::utils::LEInStream& in_stream) noexcept
            {
                return _clr_err();
            }

            virtual inline const bool is_BA_file() const noexcept
            {
                return false;
            }

            virtual inline const bool is_BM_file() const noexcept
            {
                return false;
            }

            virtual inline const bool is_V1_file() const noexcept
            {
                return false;
            }

        };


        //===========================================================================
        struct BMPFileHeaderBA : public BMPFileHeaderBase
        {
            using MyBaseClass = BMPFileHeaderBase;

            static constexpr std::size_t SIZE{ 14 };

            std::uint32_t offset_to_next{ 0 };
            std::int16_t  screen_width{ 0 };
            std::int16_t  screen_height{ 0 };

            BMPFileHeaderBA() noexcept = default;
            BMPFileHeaderBA(const BMPFileHeaderBA&) noexcept = default;
            BMPFileHeaderBA(BMPFileHeaderBA&&) noexcept = default;
            virtual ~BMPFileHeaderBA() noexcept = default;

            inline BMPFileHeaderBA& operator= (const BMPFileHeaderBA&) noexcept = default;
            inline BMPFileHeaderBA& operator= (BMPFileHeaderBA&&) noexcept = default;

            inline BMPFileHeaderBA(bmpl::utils::LEInStream& in_stream) noexcept
                : MyBaseClass()
            {
                load(in_stream);
            }

            virtual inline const std::size_t get_content_offset() const noexcept override
            {
                return std::size_t(offset_to_next);
            }

            virtual inline const std::size_t get_header_size() const noexcept override
            {
                return SIZE;
            }

            inline const bool is_last_image() const noexcept
            {
                return offset_to_next == 0;
            }

            virtual const bool load(bmpl::utils::LEInStream& in_stream) noexcept override;

            virtual inline const bool is_BA_file() const noexcept override
            {
                return true;
            }

        };


        //===========================================================================
        struct BMPFileHeaderBM : public BMPFileHeaderBase
        {
            using MyBaseClass = BMPFileHeaderBase;

            static constexpr std::size_t SIZE{ 14 };

            std::int16_t  reserved1{ 0 };
            std::int16_t  reserved2{ 0 };
            std::uint32_t content_offset{ 0 };

            BMPFileHeaderBM() noexcept = default;
            BMPFileHeaderBM(const BMPFileHeaderBM&) noexcept = default;
            BMPFileHeaderBM(BMPFileHeaderBM&&) noexcept = default;
            virtual ~BMPFileHeaderBM() noexcept = default;

            inline BMPFileHeaderBM& operator= (const BMPFileHeaderBM&) noexcept = default;
            inline BMPFileHeaderBM& operator= (BMPFileHeaderBM&&) noexcept = default;

            inline BMPFileHeaderBM(bmpl::utils::LEInStream& in_stream) noexcept
                : MyBaseClass()
            {
                load(in_stream);
            }

            virtual inline const std::size_t get_content_offset() const noexcept override
            {
                return std::size_t(content_offset);
            }

            virtual inline const std::size_t get_header_size() const noexcept override
            {
                return SIZE;
            }

            virtual const bool load(bmpl::utils::LEInStream& in_stream) noexcept override;

            virtual inline const bool is_BM_file() const noexcept override
            {
                return true;
            }

        };


        //===========================================================================
        struct BMPFileHeaderV1 : public BMPFileHeaderBase
        {
            using MyBaseClass = BMPFileHeaderBase;

            static constexpr std::size_t SIZE{ 10 };

            std::int16_t bitmap_width{ 0 };
            std::int16_t bitmap_height{ 0 };
            std::int16_t bitmap_bytes_width{ 0 };
            std::uint8_t planes_count{ 0 };
            std::uint8_t bits_per_pixel{ 0 };

            BMPFileHeaderV1() noexcept = default;
            BMPFileHeaderV1(const BMPFileHeaderV1&) noexcept = default;
            BMPFileHeaderV1(BMPFileHeaderV1&&) noexcept = default;
            virtual ~BMPFileHeaderV1() noexcept = default;

            inline BMPFileHeaderV1& operator= (const BMPFileHeaderV1&) noexcept = default;
            inline BMPFileHeaderV1& operator= (BMPFileHeaderV1&&) noexcept = default;

            inline BMPFileHeaderV1(bmpl::utils::LEInStream& in_stream) noexcept
                : MyBaseClass()
            {
                load(in_stream);
            }

            virtual inline const std::size_t get_header_size() const noexcept override
            {
                return SIZE;
            }

            virtual const bool load(bmpl::utils::LEInStream& in_stream) noexcept override;

            virtual inline const bool is_V1_file() const noexcept override
            {
                return true;
            }

        };


        //===========================================================================
        const BMPFileHeaderBase* create_file_header(bmpl::utils::LEInStream& in_stream) noexcept;

    }
}