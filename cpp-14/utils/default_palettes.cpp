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


#include "default_palettes.h"


namespace bmpl
{
    namespace clr
    {
        //---------------------------------------------------------------------------
        const WindowsDefaultPalettes::Default2Palette WindowsDefaultPalettes::win_2{
            RGB{ 0x00, 0x00, 0x00 },
            RGB{ 0xff, 0xff, 0xff }
        };


        //---------------------------------------------------------------------------
        const WindowsDefaultPalettes::Default16Palette WindowsDefaultPalettes::win95_16{
            RGB{ 0x00, 0x00, 0x00 },
            RGB{ 0x00, 0x00, 0xa8 },
            RGB{ 0x00, 0xa8, 0x00 },
            RGB{ 0x00, 0xa8, 0xa8 },
            RGB{ 0xa8, 0x00, 0x00 },
            RGB{ 0xa8, 0x00, 0xa8 },
            RGB{ 0xa8, 0x54, 0x00 },
            RGB{ 0xa8, 0xa8, 0xa8 },
            RGB{ 0x54, 0x54, 0x54 },
            RGB{ 0x54, 0x54, 0xfc },
            RGB{ 0x54, 0xfc, 0x54 },
            RGB{ 0x54, 0xfc, 0xfc },
            RGB{ 0xfc, 0x54, 0x54 },
            RGB{ 0xfc, 0x54, 0xfc },
            RGB{ 0xfc, 0xfc, 0x54 },
            RGB{ 0xfc, 0xfc, 0xfc }
        };  // notice: source https://github.com/microsoft/terminal/issues/1260


        //---------------------------------------------------------------------------
        const WindowsDefaultPalettes::Default16Palette WindowsDefaultPalettes::winNT_16{
            RGB{ 0x00, 0x00, 0x00 },
            RGB{ 0x00, 0x00, 0x80 },
            RGB{ 0x00, 0x80, 0x00 },
            RGB{ 0x00, 0x80, 0x80 },
            RGB{ 0x80, 0x00, 0x00 },
            RGB{ 0x80, 0x00, 0x80 },
            RGB{ 0x80, 0x80, 0x00 },
            RGB{ 0xc0, 0xc0, 0xc0 },
            RGB{ 0x80, 0x80, 0x80 },
            RGB{ 0x00, 0x00, 0xff },
            RGB{ 0x00, 0xff, 0x00 },
            RGB{ 0x00, 0xff, 0xff },
            RGB{ 0xff, 0x00, 0x00 },
            RGB{ 0xff, 0x00, 0xff },
            RGB{ 0xff, 0xff, 0x00 },
            RGB{ 0xff, 0xff, 0xff }
        };  // notice: source https://github.com/microsoft/terminal/issues/1260


        //---------------------------------------------------------------------------
        const WindowsDefaultPalettes::Default16Palette WindowsDefaultPalettes::win10_16{
            RGB{ 0x0c, 0x0c, 0x0c },
            RGB{ 0x00, 0x37, 0xda },
            RGB{ 0x13, 0xa1, 0x0e },
            RGB{ 0x3a, 0x96, 0xdd },
            RGB{ 0xc5, 0x0f, 0x1f },
            RGB{ 0x88, 0x17, 0x98 },
            RGB{ 0xc1, 0x9c, 0x00 },
            RGB{ 0xcc, 0xcc, 0xcc },
            RGB{ 0x76, 0x76, 0x76 },
            RGB{ 0x3b, 0x78, 0xff },
            RGB{ 0x16, 0xc6, 0x0c },
            RGB{ 0x61, 0xd6, 0xd6 },
            RGB{ 0xe7, 0x48, 0x56 },
            RGB{ 0xb4, 0x00, 0x9e },
            RGB{ 0xf9, 0xf1, 0xa5 },
            RGB{ 0xf2, 0xf2, 0xf2 }
        };  // notice: source https://github.com/microsoft/terminal/issues/1260


        //---------------------------------------------------------------------------
        const WindowsDefaultPalettes::Default256Palette WindowsDefaultPalettes::win95_256{
            RGB{ 0x00, 0x00, 0x00 },
            RGB{ 0x80, 0x00, 0x00 },
            RGB{ 0x00, 0x80, 0x00 },
            RGB{ 0x80, 0x80, 0x00 },
            RGB{ 0x00, 0x00, 0x80 },
            RGB{ 0x80, 0x00, 0x80 },
            RGB{ 0x00, 0x80, 0x80 },
            RGB{ 0xc0, 0xc0, 0xc0 },
            RGB{ 0xc0, 0xdc, 0xc0 },
            RGB{ 0xa6, 0xca, 0xf0 },
            RGB{ 0x2a, 0x3f, 0xaa },
            RGB{ 0x2a, 0x3f, 0xff },
            RGB{ 0x2a, 0x5f, 0x00 },
            RGB{ 0x2a, 0x5f, 0x55 },
            RGB{ 0x2a, 0x5f, 0xaa },
            RGB{ 0x2a, 0x5f, 0xff },
            RGB{ 0x2a, 0x7f, 0x00 },
            RGB{ 0x2a, 0x7f, 0x55 },
            RGB{ 0x2a, 0x7f, 0xaa },
            RGB{ 0x2a, 0x7f, 0xff },
            RGB{ 0x2a, 0x9f, 0x00 },
            RGB{ 0x2a, 0x9f, 0x55 },
            RGB{ 0x2a, 0x9f, 0xaa },
            RGB{ 0x2a, 0x9f, 0xff },
            RGB{ 0x2a, 0xbf, 0x00 },
            RGB{ 0x2a, 0xbf, 0x55 },
            RGB{ 0x2a, 0xbf, 0xaa },
            RGB{ 0x2a, 0xbf, 0xff },
            RGB{ 0x2a, 0xdf, 0x00 },
            RGB{ 0x2a, 0xdf, 0x55 },
            RGB{ 0x2a, 0xdf, 0xaa },
            RGB{ 0x2a, 0xdf, 0xff },
            RGB{ 0x2a, 0xff, 0x00 },
            RGB{ 0x2a, 0xff, 0x55 },
            RGB{ 0x2a, 0xff, 0xaa },
            RGB{ 0x2a, 0xff, 0xff },
            RGB{ 0x55, 0x00, 0x00 },
            RGB{ 0x55, 0x00, 0x55 },
            RGB{ 0x55, 0x00, 0xaa },
            RGB{ 0x55, 0x00, 0xff },
            RGB{ 0x55, 0x1f, 0x00 },
            RGB{ 0x55, 0x1f, 0x55 },
            RGB{ 0x55, 0x1f, 0xaa },
            RGB{ 0x55, 0x1f, 0xff },
            RGB{ 0x55, 0x3f, 0x00 },
            RGB{ 0x55, 0x3f, 0x55 },
            RGB{ 0x55, 0x3f, 0xaa },
            RGB{ 0x55, 0x3f, 0xff },
            RGB{ 0x55, 0x5f, 0x00 },
            RGB{ 0x55, 0x5f, 0x55 },
            RGB{ 0x55, 0x5f, 0xaa },
            RGB{ 0x55, 0x5f, 0xff },
            RGB{ 0x55, 0x7f, 0x00 },
            RGB{ 0x55, 0x7f, 0x55 },
            RGB{ 0x55, 0x7f, 0xaa },
            RGB{ 0x55, 0x7f, 0xff },
            RGB{ 0x55, 0x9f, 0x00 },
            RGB{ 0x55, 0x9f, 0x55 },
            RGB{ 0x55, 0x9f, 0xaa },
            RGB{ 0x55, 0x9f, 0xff },
            RGB{ 0x55, 0xbf, 0x00 },
            RGB{ 0x55, 0xbf, 0x55 },
            RGB{ 0x55, 0xbf, 0xaa },
            RGB{ 0x55, 0xbf, 0xff },
            RGB{ 0x55, 0xdf, 0x00 },
            RGB{ 0x55, 0xdf, 0x55 },
            RGB{ 0x55, 0xdf, 0xaa },
            RGB{ 0x55, 0xdf, 0xff },
            RGB{ 0x55, 0xff, 0x00 },
            RGB{ 0x55, 0xff, 0x55 },
            RGB{ 0x55, 0xff, 0xaa },
            RGB{ 0x55, 0xff, 0xff },
            RGB{ 0x7f, 0x00, 0x00 },
            RGB{ 0x7f, 0x00, 0x55 },
            RGB{ 0x7f, 0x00, 0xaa },
            RGB{ 0x7f, 0x00, 0xff },
            RGB{ 0x7f, 0x1f, 0x00 },
            RGB{ 0x7f, 0x1f, 0x55 },
            RGB{ 0x7f, 0x1f, 0xaa },
            RGB{ 0x7f, 0x1f, 0xff },
            RGB{ 0x7f, 0x3f, 0x00 },
            RGB{ 0x7f, 0x3f, 0x55 },
            RGB{ 0x7f, 0x3f, 0xaa },
            RGB{ 0x7f, 0x3f, 0xff },
            RGB{ 0x7f, 0x5f, 0x00 },
            RGB{ 0x7f, 0x5f, 0x55 },
            RGB{ 0x7f, 0x5f, 0xaa },
            RGB{ 0x7f, 0x5f, 0xff },
            RGB{ 0x7f, 0x7f, 0x00 },
            RGB{ 0x7f, 0x7f, 0x55 },
            RGB{ 0x7f, 0x7f, 0xaa },
            RGB{ 0x7f, 0x7f, 0xff },
            RGB{ 0x7f, 0x9f, 0x00 },
            RGB{ 0x7f, 0x9f, 0x55 },
            RGB{ 0x7f, 0x9f, 0xaa },
            RGB{ 0x7f, 0x9f, 0xff },
            RGB{ 0x7f, 0xbf, 0x00 },
            RGB{ 0x7f, 0xbf, 0x55 },
            RGB{ 0x7f, 0xbf, 0xaa },
            RGB{ 0x7f, 0xbf, 0xff },
            RGB{ 0x7f, 0xdf, 0x00 },
            RGB{ 0x7f, 0xdf, 0x55 },
            RGB{ 0x7f, 0xdf, 0xaa },
            RGB{ 0x7f, 0xdf, 0xff },
            RGB{ 0x7f, 0xff, 0x00 },
            RGB{ 0x7f, 0xff, 0x55 },
            RGB{ 0x7f, 0xff, 0xaa },
            RGB{ 0x7f, 0xff, 0xff },
            RGB{ 0xaa, 0x00, 0x00 },
            RGB{ 0xaa, 0x00, 0x55 },
            RGB{ 0xaa, 0x00, 0xaa },
            RGB{ 0xaa, 0x00, 0xff },
            RGB{ 0xaa, 0x1f, 0x00 },
            RGB{ 0xaa, 0x1f, 0x55 },
            RGB{ 0xaa, 0x1f, 0xaa },
            RGB{ 0xaa, 0x1f, 0xff },
            RGB{ 0xaa, 0x3f, 0x00 },
            RGB{ 0xaa, 0x3f, 0x55 },
            RGB{ 0xaa, 0x3f, 0xaa },
            RGB{ 0xaa, 0x3f, 0xff },
            RGB{ 0xaa, 0x5f, 0x00 },
            RGB{ 0xaa, 0x5f, 0x55 },
            RGB{ 0xaa, 0x5f, 0xaa },
            RGB{ 0xaa, 0x5f, 0xff },
            RGB{ 0xaa, 0x7f, 0x00 },
            RGB{ 0xaa, 0x7f, 0x55 },
            RGB{ 0xaa, 0x7f, 0xaa },
            RGB{ 0xaa, 0x7f, 0xff },
            RGB{ 0xaa, 0x9f, 0x00 },
            RGB{ 0xaa, 0x9f, 0x55 },
            RGB{ 0xaa, 0x9f, 0xaa },
            RGB{ 0xaa, 0x9f, 0xff },
            RGB{ 0xaa, 0xbf, 0x00 },
            RGB{ 0xaa, 0xbf, 0x55 },
            RGB{ 0xaa, 0xbf, 0xaa },
            RGB{ 0xaa, 0xbf, 0xff },
            RGB{ 0xaa, 0xdf, 0x00 },
            RGB{ 0xaa, 0xdf, 0x55 },
            RGB{ 0xaa, 0xdf, 0xaa },
            RGB{ 0xaa, 0xdf, 0xff },
            RGB{ 0xaa, 0xff, 0x00 },
            RGB{ 0xaa, 0xff, 0x55 },
            RGB{ 0xaa, 0xff, 0xaa },
            RGB{ 0xaa, 0xff, 0xff },
            RGB{ 0xd4, 0x00, 0x00 },
            RGB{ 0xd4, 0x00, 0x55 },
            RGB{ 0xd4, 0x00, 0xaa },
            RGB{ 0xd4, 0x00, 0xff },
            RGB{ 0xd4, 0x1f, 0x00 },
            RGB{ 0xd4, 0x1f, 0x55 },
            RGB{ 0xd4, 0x1f, 0xaa },
            RGB{ 0xd4, 0x1f, 0xff },
            RGB{ 0xd4, 0x3f, 0x00 },
            RGB{ 0xd4, 0x3f, 0x55 },
            RGB{ 0xd4, 0x3f, 0xaa },
            RGB{ 0xd4, 0x3f, 0xff },
            RGB{ 0xd4, 0x5f, 0x00 },
            RGB{ 0xd4, 0x5f, 0x55 },
            RGB{ 0xd4, 0x5f, 0xaa },
            RGB{ 0xd4, 0x5f, 0xff },
            RGB{ 0xd4, 0x7f, 0x00 },
            RGB{ 0xd4, 0x7f, 0x55 },
            RGB{ 0xd4, 0x7f, 0xaa },
            RGB{ 0xd4, 0x7f, 0xff },
            RGB{ 0xd4, 0x9f, 0x00 },
            RGB{ 0xd4, 0x9f, 0x55 },
            RGB{ 0xd4, 0x9f, 0xaa },
            RGB{ 0xd4, 0x9f, 0xff },
            RGB{ 0xd4, 0xbf, 0x00 },
            RGB{ 0xd4, 0xbf, 0x55 },
            RGB{ 0xd4, 0xbf, 0xaa },
            RGB{ 0xd4, 0xbf, 0xff },
            RGB{ 0xd4, 0xdf, 0x00 },
            RGB{ 0xd4, 0xdf, 0x55 },
            RGB{ 0xd4, 0xdf, 0xaa },
            RGB{ 0xd4, 0xdf, 0xff },
            RGB{ 0xd4, 0xff, 0x00 },
            RGB{ 0xd4, 0xff, 0x55 },
            RGB{ 0xd4, 0xff, 0xaa },
            RGB{ 0xd4, 0xff, 0xff },
            RGB{ 0xff, 0x00, 0x55 },
            RGB{ 0xff, 0x00, 0xaa },
            RGB{ 0xff, 0x1f, 0x00 },
            RGB{ 0xff, 0x1f, 0x55 },
            RGB{ 0xff, 0x1f, 0xaa },
            RGB{ 0xff, 0x1f, 0xff },
            RGB{ 0xff, 0x3f, 0x00 },
            RGB{ 0xff, 0x3f, 0x55 },
            RGB{ 0xff, 0x3f, 0xaa },
            RGB{ 0xff, 0x3f, 0xff },
            RGB{ 0xff, 0x5f, 0x00 },
            RGB{ 0xff, 0x5f, 0x55 },
            RGB{ 0xff, 0x5f, 0xaa },
            RGB{ 0xff, 0x5f, 0xff },
            RGB{ 0xff, 0x7f, 0x00 },
            RGB{ 0xff, 0x7f, 0x55 },
            RGB{ 0xff, 0x7f, 0xaa },
            RGB{ 0xff, 0x7f, 0xff },
            RGB{ 0xff, 0x9f, 0x00 },
            RGB{ 0xff, 0x9f, 0x55 },
            RGB{ 0xff, 0x9f, 0xaa },
            RGB{ 0xff, 0x9f, 0xff },
            RGB{ 0xff, 0xbf, 0x00 },
            RGB{ 0xff, 0xbf, 0x55 },
            RGB{ 0xff, 0xbf, 0xaa },
            RGB{ 0xff, 0xbf, 0xff },
            RGB{ 0xff, 0xdf, 0x00 },
            RGB{ 0xff, 0xdf, 0x55 },
            RGB{ 0xff, 0xdf, 0xaa },
            RGB{ 0xff, 0xdf, 0xff },
            RGB{ 0xff, 0xff, 0x55 },
            RGB{ 0xff, 0xff, 0xaa },
            RGB{ 0xcc, 0xcc, 0xff },
            RGB{ 0xff, 0xcc, 0xff },
            RGB{ 0x33, 0xff, 0xff },
            RGB{ 0x66, 0xff, 0xff },
            RGB{ 0x99, 0xff, 0xff },
            RGB{ 0xcc, 0xff, 0xff },
            RGB{ 0x00, 0x7f, 0x00 },
            RGB{ 0x00, 0x7f, 0x55 },
            RGB{ 0x00, 0x7f, 0xaa },
            RGB{ 0x00, 0x7f, 0xff },
            RGB{ 0x00, 0x9f, 0x00 },
            RGB{ 0x00, 0x9f, 0x55 },
            RGB{ 0x00, 0x9f, 0xaa },
            RGB{ 0x00, 0x9f, 0xff },
            RGB{ 0x00, 0xbf, 0x00 },
            RGB{ 0x00, 0xbf, 0x55 },
            RGB{ 0x00, 0xbf, 0xaa },
            RGB{ 0x00, 0xbf, 0xff },
            RGB{ 0x00, 0xdf, 0x00 },
            RGB{ 0x00, 0xdf, 0x55 },
            RGB{ 0x00, 0xdf, 0xaa },
            RGB{ 0x00, 0xdf, 0xff },
            RGB{ 0x00, 0xff, 0x55 },
            RGB{ 0x00, 0xff, 0xaa },
            RGB{ 0x2a, 0x00, 0x00 },
            RGB{ 0x2a, 0x00, 0x55 },
            RGB{ 0x2a, 0x00, 0xaa },
            RGB{ 0x2a, 0x00, 0xff },
            RGB{ 0x2a, 0x1f, 0x00 },
            RGB{ 0x2a, 0x1f, 0x55 },
            RGB{ 0x2a, 0x1f, 0xaa },
            RGB{ 0x2a, 0x1f, 0xff },
            RGB{ 0x2a, 0x3f, 0x00 },
            RGB{ 0x2a, 0x3f, 0x55 },
            RGB{ 0xff, 0xbf, 0xf0 },
            RGB{ 0xa0, 0xa0, 0xa4 },
            RGB{ 0x80, 0x80, 0x80 },
            RGB{ 0xff, 0x00, 0x00 },
            RGB{ 0x00, 0xff, 0x00 },
            RGB{ 0xff, 0xff, 0x00 },
            RGB{ 0x00, 0x00, 0xff },
            RGB{ 0xff, 0x00, 0xff },
            RGB{ 0x00, 0xff, 0xff },
            RGB{ 0xff, 0xff, 0xff }
        };  // notice: source https://lospec.com/palette-list/windows-95-256-colours


        //---------------------------------------------------------------------------
        WindowsDefaultPalettes::Default2Palette   WindowsDefaultPalettes::_default_2_palette{ WindowsDefaultPalettes::win_2 };

        WindowsDefaultPalettes::Default16Palette  WindowsDefaultPalettes::_default_16_palette{ WindowsDefaultPalettes::win95_16 };

        WindowsDefaultPalettes::Default256Palette WindowsDefaultPalettes::_default_256_palette{ WindowsDefaultPalettes::win95_256 };

    }

}

