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


#include <cmath>

#include "types.h"


namespace bmpl
{
    namespace utils
    {
        //===========================================================================
        Frac16_16& Frac16_16::operator= (const double val) noexcept
        {
            if (val >= 65536.0) {
                this->value = std::uint32_t(0xffff'ffff);
            }
            else if (val <= 0.0) {
                this->value = std::uint32_t(0);
            }
            else {
                const double d_mantissa{ std::trunc(val) };

                const std::uint32_t mantissa{ std::uint32_t(d_mantissa)};
                const std::uint32_t frac{ std::uint32_t((val - d_mantissa) * 65536.0) };
                
                this->value = (mantissa << 16) + (frac & 0xffff);
            }

            return *this;
        }

    }
}