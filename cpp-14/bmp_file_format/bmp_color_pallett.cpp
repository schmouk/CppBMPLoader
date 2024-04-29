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


#include <array>

#include "bmp_color_pallett.h"


namespace bmpl
{
    namespace frmt
    {
        const bool BMPColorPallett::load(bmpl::utils::LEInStream& in_stream, const std::uint32_t colors_count_) noexcept
        {
            this->colors_count = colors_count_;

            if (!in_stream.is_ok())
                return _set_err(in_stream.get_error());

            if (colors_count_ > 0) {

                auto pallett_it = MyContainerBaseClass::begin();

                if (bmpl::utils::PLATFORM_IS_LITTLE_ENDIAN) {
                    if (!in_stream.read(reinterpret_cast<char*>(MyContainerBaseClass::data()), std::streamsize(4 * colors_count_)))
                        _set_err(bmpl::utils::ErrorCode::BAD_PALLETT_ENCODING);
                }
                else {
                    bmpl::clr::BGRA bgra;
                    for (std::uint32_t i = 0; i < this->colors_count; ++i) {
                        in_stream >> bgra;
                        bmpl::clr::set(*pallett_it++, bgra);
                    }
                    if (!in_stream.is_ok())
                        _set_err(bmpl::utils::ErrorCode::BAD_PALLETT_ENCODING);
                }
            }

            return is_ok();
        }


        const bmpl::clr::RGBA& BMPColorPallett::operator[] (const std::uint32_t index) noexcept
        {
            if (index >= this->colors_count) {
                _set_err(bmpl::utils::ErrorCode::OUT_OF_PALLETT_INDEX);
                return (*this)[0];
            }
            else {
                return MyContainerBaseClass::operator[](index);
            }
        }

    }
}
