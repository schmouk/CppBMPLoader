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


#include <cstdint>


namespace bmpl
{
    namespace utils
    {
        //===========================================================================
        struct Frac16_16
        {
            std::uint32_t value{ 0x0001'0000 };  // i.e. 1.0

            Frac16_16() noexcept = default;
            virtual ~Frac16_16() noexcept = default;

            inline Frac16_16(const std::uint32_t val) noexcept
                : value(val)
            {}

            inline Frac16_16(const float val) noexcept
            {
                *this = val;
            }

            inline Frac16_16(const double val) noexcept
            {
                *this = val;
            }

            inline operator float() const noexcept
            {
                return float(value >> 16) + float(value & 0xffff) / 65536.0f;
            }

            inline operator double() const noexcept
            {
                return double(value >> 16) + double(value & 0xffff) / 65536.0;
            }

            inline Frac16_16& operator= (const std::uint32_t val) noexcept
            {
                value = val;
                return *this;
            }

            inline Frac16_16& operator= (const float val) noexcept
            {
                return operator=(double(val));
            }

            Frac16_16& operator= (const double val) noexcept;

            inline const bool operator== (const std::uint32_t val) const noexcept
            {
                return this->value == val;
            }

            inline const bool operator== (const float val) const noexcept
            {
                return float(*this) == val;
            }

            inline const bool operator== (const double val) const noexcept
            {
                return double(*this) == val;
            }

            inline const bool operator!= (const std::uint32_t val) const noexcept
            {
                return this->value != val;
            }

            inline const bool operator!= (const float val) const noexcept
            {
                return float(*this) != val;
            }

            inline const bool operator!= (const double val) const noexcept
            {
                return double(*this) != val;
            }

            inline const bool operator< (const float val) const noexcept
            {
                return float(*this) < val;
            }

            inline const bool operator< (const double val) const noexcept
            {
                return double(*this) < val;
            }

            inline const bool operator> (const float val) const noexcept
            {
                return float(*this) > val;
            }

            inline const bool operator> (const double val) const noexcept
            {
                return double(*this) > val;
            }

        };

    }
}