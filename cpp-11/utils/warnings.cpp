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

#include "warnings.h"


namespace bmpl
{
    namespace utils
    {
        //---------------------------------------------------------------------------
        const std::string warning_msg(const WarningCode warn_code) noexcept
        {
            switch (warn_code)
            {
            case WarningCode::ALPHA_16_BITS_IMAGE:
                return "an alpha channel is specified for this 16-bits pixels bitmap.";
            case WarningCode::BAD_BITMAP_SIZE_IN_HEADER:
                return "header indicates a bad bitmap size.";
            case WarningCode::BAD_FILE_SIZE_IN_HEADER:
                return "the file size indicated in header is not the actual size of this file.";
            case WarningCode::BAD_HALFTONING_MODE_VALUE:
                return "an invalid value for the halftoning algorithm used with the bitmap data has been detected; CppBMPLoader decodes it nevertheless.";
            case WarningCode::BAD_IMPORTANT_COLORS_COUNT:
                return "the 'important colors' count appears to be greater than the defined colors count.";
            case WarningCode::BAD_PALETTE_INDICES:
                return "one or many palette indices used for pixels are not present in color palette - index 0 used instead.";
            case WarningCode::BAD_PALETTE_SIZE_IN_HEADER:
                return "the info header indicates a bad color palette size - bigger than expected, for instance.";
            case WarningCode::BAD_PLANES_VALUE:
                return "the file header indicates a planes count value different from 1, which is forbidden; CppBMPLibrary decodes this image nevertheless.";
            case WarningCode::BAD_SCANLINE_BYTES_WIDTH:
                return "the specified width for scan lines is incoherent with the expected true width; CppBMPLibrary tries to decode this image nevertheless.";
            case WarningCode::BITFIELDS_AND_PALETT:
                return "a color palette is encoded while bitfields masks are defined.";
            case WarningCode::CORRECTED_GAMMA_BLUE:
                return "strange gamma correction value on blue component, forced to 2.2.";
            case WarningCode::CORRECTED_GAMMA_GREEN:
                return "strange gamma correction value on green component, forced to 2.2.";
            case WarningCode::CORRECTED_GAMMA_RED:
                return "strange gamma correction value on red component, forced to 2.2.";
            case WarningCode::DELTA_MODE_MAY_OVERFLOW:
                return "some value specified in RLE delta mode may cause buffers overflow.";
            case WarningCode::EMBEDDED_PROFILE_NOT_IMPLEMENTED:
                return "ICC embedded profiles are not implemented in CppBMPLoader library which uses plain coded colors instead.";
            case WarningCode::FORBIDDEN_TOP_DOWN_ORIENTATION:
                return "top-down orientation of image is not allowed with RLE-compression; CppBMPLoader library tries to decode this image nevertheless.";
            case WarningCode::GAP_BTW_COLORMAP_AND_BITMAP:
                return "some gap exists between colormap and image bitmap; CppBMPLoader decodes this image nevertheless.";
            case WarningCode::HOT_POINT_SET:
                return "a hot point X and Y values are set in file header while they should not; CppBMPLoader library nevertheless decodes this file as a BMP one.";
            case WarningCode::INCOHERENT_IMAGE_SIZE:
                return "image size in info header is incoherent with specified width and height.";
            case WarningCode::INCOHERENT_RESOLUTIONS:
                return "maybe incoherent image resolutions detected in info header - one of them may be too big compared to the other one.";
            case WarningCode::INVALID_BA_FILE_HEADER_SIZE:
                return "an invalid file header size has beend detected in a bitmap array header; CppBMPLoader library tries to decode images nevertheless.";
            case WarningCode::INVALID_COLOR_ENCODING:
                return "the specified color encoding is invalid - not an RGB one; CppBMPLoader library nevertheless decodes the bitmap as being RGB encoded.";
            case WarningCode::INVALID_DAMPING_VALUE:
                return "the specified damping value for the halftoning error diffusion is invalid (greater than 100%).";
            case WarningCode::INVALID_RESOLUTION_UNITS:
                return "a non-zero resolution units value difference has been detected - this is invalid; CppBMPLoader library decodes this image nevertheless.";
            case WarningCode::LINKED_PROFILE_NOT_IMPLEMENTED:
                return "for security issues, ICC linked profiles are not implemented in CppBMPLoader library which uses plain coded colors instead.";
            case WarningCode::MISSING_BITFIELDS_MASKS:
                return "at least one color bits mask is missing in bitfields definition.";
            case WarningCode::MISSING_COLORMAP_ENTRIES:
                return "a less-than-full-sized colormap has been detected in file; CppBMPLoader library tries to decode this image nevertheless.";
            case WarningCode::NOT_ENOUGH_INDICES_IN_BITMAP:
                return "too small bitmap, last image pixels are set to default value.";
            case WarningCode::NOT_OS2_BITMAP_FORMAT:
                return "a not OS/2 bitmap format has been detected in a bitmap multi-image data file, which should be an encoding error; CppBMPLoader library decodes the related image nevertheless.";
            case WarningCode::NOT_ZERO_RESERVED:
                return "a reserved word is not set to zero while it should have been; CppBMPLoader library tries to decode this image nevertheless.";
            case WarningCode::PALETT_WITH_FULL_COLORS:
                return "a color palette is defined while this is a full-color bitmap.";
            case WarningCode::TOO_BIG_PALETTE:
                return "too many colors are defined in palette, extra ones will be ignored.";
            case WarningCode::TOO_MANY_INDICES_IN_BITMAP:
                return "bitmap contains too many indices according to final image size; extra ones will be ignored.";
            case WarningCode::UNUSED_BITS_SET_TO_1:
                return "some pixels have bitfields unused bits set to 1.";
            case WarningCode::UNUSED_PALETTE:
                return "An unused palette is defined in BMP file.";
            case WarningCode::WIN_CE_2_BITS_PIXELS:
                return "The unusual 2-bits per pixel specification is only allowed with Windows CE; CppBMPLoader library decodes this image nevertheless.";
            default:
                return "unkown warning...";
            }
        }


        //---------------------------------------------------------------------------
        const std::string warning_msg(const std::string& file_path, const WarningCode warn_code) noexcept
        {
            std::strstream msg;
            msg << "file \"" << file_path << "\": Warning #" << int(warn_code) << " - " << warning_msg(warn_code) << std::ends;
            return msg.str();
        }


        //---------------------------------------------------------------------------
        void WarningStatus::set_unique_warnings()
        {
            std::sort(_warnings_list.begin(), _warnings_list.end());
            auto new_end_it{ std::unique(_warnings_list.begin(), _warnings_list.end()) };
            _warnings_list.erase(new_end_it, _warnings_list.end());
        }

    }
}