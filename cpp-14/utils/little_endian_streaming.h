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


#include <cstdint>
#include <fstream>
#include <string>

#include "errors.h"


namespace bmpl
{
    namespace utils
    {
        //===========================================================================
        static inline constexpr bool check_little_endianness() noexcept
        {
            std::uint16_t one{ 1 };
            return (*(std::uint8_t*)(&one) == 1);
        }


        static const bool PLATFORM_IS_LITTLE_ENDIAN{ check_little_endianness() };


        //===========================================================================
        class LEInStream : public std::ifstream, public bmpl::utils::ErrorStatus
        {
        public:

            using MyBaseClass = std::ifstream;
            using MyErrClass = bmpl::utils::ErrorStatus;

            using pos_type = MyBaseClass::pos_type;

            std::string filepath{};

            LEInStream() noexcept = default;


            inline LEInStream(const char* filepath) noexcept
                : MyBaseClass(filepath, std::ios::binary)
                , MyErrClass()
                , filepath(filepath)
            {
                _check_creation_ok();
            }


            inline LEInStream(const std::string& filepath) noexcept
                : MyBaseClass(filepath, std::ios::binary)
                , MyErrClass()
                , filepath(filepath)
            {
                _check_creation_ok();
            }


            virtual ~LEInStream() noexcept = default;


            const pos_type get_size() noexcept;  // notice: type 'pos_type' is inherited from base class std::ifstream


            LEInStream& operator>>(std::int8_t& value);
            LEInStream& operator>>(std::int16_t& value);
            LEInStream& operator>>(std::int32_t& value);
            LEInStream& operator>>(std::int64_t& value);
            LEInStream& operator>>(std::uint8_t& value);
            LEInStream& operator>>(std::uint16_t& value);
            LEInStream& operator>>(std::uint32_t& value);
            LEInStream& operator>>(std::uint64_t& value);
            

        private:
            void _check_creation_ok() noexcept;
            void _set_err() noexcept;

        };


    }
}