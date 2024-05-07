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

#include "warnings.h"


namespace bmpl
{
    namespace utils
    {


        const std::string warning_msg(const WarningCode warn_code) noexcept
        {
            switch (warn_code)
            {
            case WarningCode::ALPHA_16_BITS_IMAGE:
                return "an alpha channel is specified for this 16-bits pixels bitmap.";
            case WarningCode::BAD_BITMAP_SIZE_IN_HEADER:
                return "header indicates a bad bitmap size.";
            case WarningCode::BAD_FILE_SIZE_IN_HEADER:
                return "header indicates a bad BMP file size.";
            case WarningCode::BAD_PALETT_INDICES:
                return "one or many palett indices used for pixels are not present in color palett.";
            case WarningCode::BAD_PALETT_SIZE_IN_HEADER:
                return "header indicates a bad color palett size.";
            case WarningCode::BAD_PLANES_VALUE:
                return "header indicates a planes count value different from 1.";
            case WarningCode::BITFIELDS_AND_PALETT:
                return "a color palett is encoded while bitfields masks are defined.";
            case WarningCode::MISSING_BITFIELDS_MASKS:
                return "at least on color bits mask is missing in bitfields definition.";
            case WarningCode::PALETT_WITH_FULL_COLORS:
                return "a color palett is defined while this is a full-color bitmap.";
            case WarningCode::SOME_GAP_BTW_PALETT_AND_BITMAP:
                return "unused bytes appear between colors palett and start of bitmap.";
            case WarningCode::TOO_MANY_COLORS_IN_PALETT:
                return "too many colors are defined in palett, some of them won't be used.";
            case WarningCode::UNUSED_BITS_SET_TO_1:
                return "some pixels have bitfields unused bits set to 1.";
            default:
                return "unkown warning...";
            }
        }


        const std::string warning_msg(const std::string& file_path, const WarningCode warn_code) noexcept
        {
            std::strstream msg;
            msg << "file \"" << file_path << "\": Warning #" << int(warn_code) << " - " << warning_msg(warn_code) << std::ends;
            return msg.str();
        }

    }
}