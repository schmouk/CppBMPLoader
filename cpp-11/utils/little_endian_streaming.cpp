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

#include "little_endian_streaming.h"

namespace bmpl
{
    namespace utils
    {
        //---------------------------------------------------------------------------
        const LEInStream::pos_type LEInStream::get_size() noexcept
        {
            pos_type current_stream_pos{ tellg() };
            seekg(0, end);
            pos_type stream_size{ tellg() };
            seekg(current_stream_pos);

            return stream_size;
        }


        //---------------------------------------------------------------------------
        void LEInStream::_check_creation_ok() noexcept
        {
            if (good())
                _clr_err();
            else
                MyErrClass::_set_err(bmpl::utils::ErrorCode::FILE_NOT_FOUND);
        }


        //---------------------------------------------------------------------------
        LEInStream& LEInStream::operator>>(std::int8_t& value)
        {
            char val;
            if (read(&val, 1)) {
                _clr_err();
                value = std::int8_t(val);
            }
            else
                _set_err();

            return *this;
        }


        //---------------------------------------------------------------------------
        LEInStream& LEInStream::operator>>(std::int16_t& value)
        {
            _clr_err();

            if (PLATFORM_IS_LITTLE_ENDIAN) {
                if (!read(reinterpret_cast<char*>(&value), 2))
                    _set_err();
            }
            else {
                char val[2];
                if (read(val, 2))
                    value = std::int16_t(
                        std::uint16_t(std::uint8_t(*val)) + (std::uint16_t(std::uint8_t(val[1])) << 8)
                    );
                else
                    _set_err();
            }

            return *this;
        }


        //---------------------------------------------------------------------------
        LEInStream& LEInStream::operator>>(std::int32_t& value)
        {
            _clr_err();

            if (PLATFORM_IS_LITTLE_ENDIAN) {
                if (!read(reinterpret_cast<char*>(&value), 4))
                    _set_err();
            }
            else {
                char val[4];
                if (read(val, 4))
                    value = std::int32_t(
                        std::uint32_t(std::uint8_t(*val)) + (std::uint32_t(std::uint8_t(val[1])) << 8) +
                        (std::uint32_t(std::uint8_t(val[2])) << 16) + (std::uint32_t(std::uint8_t(val[3])) << 24)
                    );
                else
                    _set_err();
            }

            return *this;
        }


        //---------------------------------------------------------------------------
        LEInStream& LEInStream::operator>>(std::int64_t& value)
        {
            _clr_err();

            if (PLATFORM_IS_LITTLE_ENDIAN) {
                if (!read(reinterpret_cast<char*>(&value), 8))
                    _set_err();
            }
            else {
                char val[8];
                if (read(val, 8))
                    value = std::int64_t(
                        std::uint64_t(std::uint8_t(*val)) + (std::uint64_t(std::uint8_t(val[1])) << 8) +
                        (std::uint64_t(std::uint8_t(val[2])) << 16) + (std::uint64_t(std::uint8_t(val[3])) << 24) +
                        (std::uint64_t(std::uint8_t(val[4])) << 32) + (std::uint64_t(std::uint8_t(val[5])) << 40) +
                        (std::uint64_t(std::uint8_t(val[6])) << 48) + (std::uint64_t(std::uint8_t(val[7])) << 56)
                    );
                else
                    _set_err();
            }

            return *this;
        }


        //---------------------------------------------------------------------------
        LEInStream& LEInStream::operator>>(std::uint8_t& value)
        {
            char val;
            if (read(&val, 1)) {
                _clr_err();
                value = std::uint8_t(val);
            }
            else
                _set_err();

            return *this;
        }


        //---------------------------------------------------------------------------
        LEInStream& LEInStream::operator>>(std::uint16_t& value)
        {
            _clr_err();

            if (PLATFORM_IS_LITTLE_ENDIAN) {
                if (!read(reinterpret_cast<char*>(&value), 2))
                    _set_err();
            }
            else {
                char val[2];
                if (read(val, 2))
                    value = std::uint16_t(std::uint8_t(*val)) + (std::uint16_t(std::uint8_t(val[1])) << 8);
                else
                    _set_err();
            }

            return *this;
        }


        //---------------------------------------------------------------------------
        LEInStream& LEInStream::operator>>(std::uint32_t& value)
        {
            _clr_err();

            if (PLATFORM_IS_LITTLE_ENDIAN) {
                if (!read(reinterpret_cast<char*>(&value), 4))
                    _set_err();
            }
            else {
                char val[4];
                if (read(val, 4))
                    value = std::uint32_t(std::uint8_t(*val)) + (std::uint32_t(std::uint8_t(val[1])) << 8) +
                    (std::uint32_t(std::uint8_t(val[2])) << 16) + (std::uint32_t(std::uint8_t(val[3])) << 24);
                else
                    _set_err();
            }

            return *this;
        }


        //---------------------------------------------------------------------------
        LEInStream& LEInStream::operator>>(std::uint64_t& value)
        {
            _clr_err();

            if (PLATFORM_IS_LITTLE_ENDIAN) {
                if (!read(reinterpret_cast<char*>(&value), 8))
                    _set_err();
            }
            else {
                char val[8];
                if (read(val, 8)) {
                    value = std::uint64_t(std::uint8_t(*val)) + (std::uint64_t(std::uint8_t(val[1])) << 8) +
                        (std::uint64_t(std::uint8_t(val[2])) << 16) + (std::uint64_t(std::uint8_t(val[3])) << 24) +
                        (std::uint64_t(std::uint8_t(val[4])) << 32) + (std::uint64_t(std::uint8_t(val[5])) << 40) +
                        (std::uint64_t(std::uint8_t(val[6])) << 48) + (std::uint64_t(std::uint8_t(val[7])) << 56);
                }
                else {
                    _set_err();
                }
            }

            return *this;
        }


        //---------------------------------------------------------------------------
        void LEInStream::_set_err() noexcept
        {
            if (fail())
                MyErrClass::_set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
            else if (bad())
                MyErrClass::_set_err(bmpl::utils::ErrorCode::IRRECOVERABLE_STREAM_ERROR);
            else if (eof())
                MyErrClass::_set_err(bmpl::utils::ErrorCode::END_OF_FILE);
            else if (good())
                _clr_err();
            else
                MyErrClass::_set_err(bmpl::utils::ErrorCode::CORRUPTED_BMP_FILE);
        }

    }
}