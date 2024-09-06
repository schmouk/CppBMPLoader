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


#include <array>

#include "colors.h"


namespace bmpl
{
    namespace clr
    {
        //===========================================================================
        /* \brief The default Windows color palettes. */
        class WindowsDefaultPalettes
        {
        public:
            using Default2Palette   = std::array<bmpl::clr::RGB, 2>;
            using Default16Palette  = std::array<bmpl::clr::RGB, 16>;
            using Default256Palette = std::array<bmpl::clr::RGB, 256>;

            static const Default2Palette   win_2;
            static const Default16Palette  win95_16;
            static const Default16Palette  winNT_16;
            static const Default16Palette  win10_16;
            static const Default256Palette win95_256;


            inline static const Default2Palette& get_default_2() noexcept
            {
                return _default_2_palette;
            }

            inline static const Default16Palette& get_default_16() noexcept
            {
                return _default_16_palette;
            }

            inline static const Default256Palette& get_default_256() noexcept
            {
                return _default_256_palette;
            }

            inline static void set_default_2(const Default2Palette& default_2) noexcept
            {
                _default_2_palette = default_2;
            }

            inline static void set_default_16(const Default16Palette& default_16) noexcept
            {
                _default_16_palette = default_16;
            }

            inline static void set_default_256(const Default256Palette& default_256) noexcept
            {
                _default_256_palette = default_256;
            }


        private:

            static Default2Palette   _default_2_palette;
            static Default16Palette  _default_16_palette;
            static Default256Palette _default_256_palette;

        };

    }

}

