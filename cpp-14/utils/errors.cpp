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

#include "errors.h"


namespace bmpl
{
    namespace utils
    {


        const std::string error_msg(const ErrorCode err_code) noexcept
        {
            switch (err_code)
            {
            case ErrorCode::NO_ERROR:
                return "no error.";
            case ErrorCode::BAD_BITS_PER_PIXEL_VALUE:
                return "bad bits count per pixel value.";
            case ErrorCode::BAD_DEFAULT_VALUE:
                return "bad default value.";
            case ErrorCode::BAD_COLORMAP_ENCODING:
                return "bad encoding for colors map.";
            case ErrorCode::BMP_BAD_ENCODING:
                return "bad BMP format encoding.";
            case ErrorCode::BUFFER_OVERFLOW:
                return "incoherent run length encoding creates a buffer overflow";
            case ErrorCode::CORRUPTED_BMP_FILE:
                return "corrupted BMP file.";
            case ErrorCode::END_OF_FILE:
                return "encountered unexpected end-of-file.";
            case ErrorCode::FILE_NOT_FOUND:
                return "file not found.";
            case ErrorCode::INCOHERENT_COMPRESSION_MODE:
                return "forbidden compression mode (1 or 2) associated with bitfield masks encoding";
            case ErrorCode::INCOHERENT_DELTA_MODE_VALUES:
                return "incoherent delta mode values - creates buffers overflow.";
            case ErrorCode::INCOHERENT_IMAGE_DIMENSIONS:
                return "some image dimension as indicated in header is incoherent with image size.";
            case ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING:
                return "incoherent Run Length Encoding control value.";
            case ErrorCode::INPUT_OPERATION_FAILED:
                return "some imput operation failed.";
            case ErrorCode::INVALID_DEVICE_RESOLUTION:
                return "some device resolution is invalid (maybe negative, for instance).";
            case ErrorCode::INVALID_IMAGE_DIMENSIONS:
                return "some dimension of image is invalid (maybe negative, for instance).";
            case ErrorCode::IRRECOVERABLE_STREAM_ERROR:
                return "encountered some irrecoverable file streaming error.";
            case ErrorCode::NOT_BMP_ENCODING:
                return "file is not a BMP file.";
            case ErrorCode::NOT_INITIALIZED:
                return "file is currently not initialized.";
            case ErrorCode::NOT_WINDOWS_BMP:
                return "file is not a Windows BMP file.";
            case ErrorCode::OUT_OF_COLORMAP_INDEX:
                return "encountered a color index not contained in color map.";
            case ErrorCode::OVERLAPPING_BITFIELD_MASKS:
                return "some bitfield masks are overlapping.";
            case ErrorCode::RLE_INPUT_OPERATION_FAILED:
                return "input operation on Run Length Encoded bitmap has failed.";
            default:
                return "unkown error...";
            }
        }


        const std::string error_msg(const std::string& file_path, const ErrorCode err_code) noexcept
        {
            std::strstream msg;
            msg << "file \"" << file_path << "\": ERROR #" << int(err_code) << " - " << error_msg(err_code) << std::ends;
            return msg.str();
        }

    }
}