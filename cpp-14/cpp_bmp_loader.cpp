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


#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>

#include "cpp_bmp_loader.h"
#include "utils/colors.h"


namespace bmpl
{
    void BMPBottomUpLoader::_load_image() noexcept
    {
        if (_in_stream.failed()) {
            _set_err(_in_stream.get_error());
            return;
        }

        // let's set the file cursor position to the starting point of the image coding
        if (_in_stream.seekg(_file_header.content_offset).fail()) {
            _set_err(bmpl::utils::ErrorCode::IRRECOVERABLE_STREAM_ERROR);
            return;
        }

        // reserves final image content space
        _allocate_image_space();

        // finally, loads the BMP image content and decodes it
        switch (_info.info_header.bits_per_pixel)
        {
        case 1:
            _load_1b();
            break;

        case 4:
            _load_4b();
            break;

        case  8:
            _load_8b();
            break;

        case 24:
            _load_24b();
            break;

        default:
            _set_err(bmpl::utils::ErrorCode::BAD_BITS_PER_PIXEL_VALUE);
            break;
        }
    }


    void BMPBottomUpLoader::_load_1b() noexcept
    {
        const std::size_t width{ this->width() };
        const std::size_t index_width{ std::size_t(std::ceil(width / 8.0f)) };
        const std::size_t index_height{ std::size_t(this->height()) };

        if (this->_info.info_header.compression_mode == this->_NO_RLE) {
            //-- no Run Length Encoding --//
            std::vector<std::uint8_t> indexed_content;
            indexed_content.assign(index_width * index_height, 0);

            // loads the indexed content
            if (index_width % 4 == 0) {
                // cool, no padding at end of each line
                // let's load the whole image content at once
                if (this->_in_stream.read(reinterpret_cast<char*>(indexed_content.data()), index_width * index_height).fail())
                    _set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
            }
            else {
                // let's load the image content line per line
                const std::size_t padding_size{ 4 - index_width % 4 };

                char* current_line_ptr{ reinterpret_cast<char*>(indexed_content.data()) };
                for (int line = 0; line < index_height; ++line) {
                    if (this->_in_stream.read(current_line_ptr, index_width).fail()) {
                        _set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                        return;
                    }

                    if (this->_in_stream.seekg(padding_size, std::ios_base::cur).fail()) {
                        _set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                        return;
                    }

                    current_line_ptr += index_width;
                }
            }

            // evaluates the final image content
            std::size_t remaining_pixels{ width };

            auto img_it{ this->image_content.begin() };
            for (auto ndx_it = indexed_content.begin(); ndx_it != indexed_content.end(); ++ndx_it) {
                for (std::uint8_t mask = 0x80; remaining_pixels > 0 && mask > 0; mask >>= 1) {
                    /*
                    if ((*ndx_it & mask) == 0)
                        bmpl::clr::convert(*img_it++, this->_info.color_pallett[0]);
                    else
                        bmpl::clr::convert(*img_it++, this->_info.color_pallett[1]);
                    */
                    bmpl::clr::convert(*img_it++, this->_info.color_pallett[(*ndx_it & mask) != 0]);
                    --remaining_pixels;
                }

                if (remaining_pixels == 0)
                    remaining_pixels = width;
            }
        }
        else {
            //-- Run Length encoding --//
            if (this->_info.info_header.compression_mode != this->_RLE_1) {
                _set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
                return;
            }

            // TODO: implement RLE-1
            // ...

        }

        // once here, everything was fine!
        _clr_err();
    }


    void BMPBottomUpLoader::_load_4b() noexcept
    {
        const std::size_t width{ this->width() };
        const std::size_t index_width{ std::size_t(std::ceil(width / 2.0f)) };
        const std::size_t index_height{ std::size_t(this->height()) };

        if (this->_info.info_header.compression_mode == this->_NO_RLE) {
            //-- no Run Length Encoding --//
            std::vector<std::uint8_t> indexed_content;
            indexed_content.assign(index_width * index_height, 0);

            // loads the indexed content
            if (index_width % 4 == 0) {
                // cool, no padding at end of each line
                // let's load the whole image content at once
                if (this->_in_stream.read(reinterpret_cast<char*>(indexed_content.data()), index_width * index_height).fail())
                    _set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
            }
            else {
                // let's load the image content line per line
                const std::size_t padding_size{ 4 - index_width % 4 };

                char* current_line_ptr{ reinterpret_cast<char*>(indexed_content.data()) };
                for (int line = 0; line < index_height; ++line) {
                    if (this->_in_stream.read(current_line_ptr, index_width).fail()) {
                        _set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                        return;
                    }

                    if (this->_in_stream.seekg(padding_size, std::ios_base::cur).fail()) {
                        _set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                        return;
                    }

                    current_line_ptr += index_width;
                }
            }

            // evaluates the final image content
            std::size_t remaining_pixels{ width };

            auto img_it{ this->image_content.begin() };
            for (auto ndx_it = indexed_content.begin(); ndx_it != indexed_content.end(); ++ndx_it) {
                bmpl::clr::convert(*img_it++, this->_info.color_pallett[*ndx_it >> 4]);
                if (--remaining_pixels > 0) {
                    bmpl::clr::convert(*img_it++, this->_info.color_pallett[*ndx_it & 0x0f]);
                    --remaining_pixels;
                }
                if (remaining_pixels == 0)
                    remaining_pixels = width;
            }
        }
        else {
            //-- Run Length encoding --//
            if (this->_info.info_header.compression_mode != this->_RLE_4) {
                _set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
                return;
            }

            // TODO: implement RLE-4
            // ...
        }

        // once here, everything was fine!
        _clr_err();
    }


    void BMPBottomUpLoader::_load_8b() noexcept
    {
        const std::size_t width{ std::size_t(this->width()) };
        const std::size_t height{ std::size_t(this->height()) };

        if (this->_info.info_header.compression_mode == this->_NO_RLE) {
            //-- no Run Length Encoding --//
            std::vector<std::uint8_t> indexed_content;
            indexed_content.assign(width * height, 0);

            // loads the indexed content
            if (width % 4 == 0) {
                // cool, no padding at end of each line
                // let's load the whole image content at once
                if (this->_in_stream.read(reinterpret_cast<char*>(indexed_content.data()), width * height).fail())
                    _set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
            }
            else {
                // let's load the image content line per line
                const std::size_t padding_size{ 4 - width % 4 };

                char* current_line_ptr{ reinterpret_cast<char*>(indexed_content.data()) };
                for (int line = 0; line < height; ++line) {
                    if (this->_in_stream.read(current_line_ptr, width).fail()) {
                        _set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                        return;
                    }

                    if (this->_in_stream.seekg(padding_size, std::ios_base::cur).fail()) {
                        _set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                        return;
                    }

                    current_line_ptr += width;
                }
            }

            // evaluates the final image content
            auto img_it{ this->image_content.begin() };
            for (auto ndx_it = indexed_content.cbegin(); ndx_it != indexed_content.cend(); )
                bmpl::clr::convert(*img_it++, this->_info.color_pallett[*ndx_it++]);
        }
        else {
            //-- Run Length encoding --//
            if (this->_info.info_header.compression_mode != this->_RLE_8) {
                _set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
                return;
            }

            // TODO: implement RLE-8
            // ...
        }

        // once here, everything was fine!
        _clr_err();
    }


    void BMPBottomUpLoader::_load_24b() noexcept
    {
        const std::size_t width{ std::size_t(this->width()) };
        const std::size_t height{ std::size_t(this->height()) };

        if (width % 4 == 0) {
            // cool, no padding at end of each line
            // let's load the whole image content at once
            if (this->_in_stream.read(reinterpret_cast<char*>(this->image_content.data()), width * height * sizeof pixel_type).fail())
                _set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
        }
        else {
            // let's load the image content line per line
            const std::size_t line_width{ width * sizeof pixel_type };
            const std::size_t padding_size{ 4 - line_width % 4 };

            char* current_line_ptr{ reinterpret_cast<char*>(this->image_content_ptr()) };
            for (int line = 0; line < height; ++line) {
                if (this->_in_stream.read(current_line_ptr, line_width).fail()) {
                    _set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                    return;
                }

                if (this->_in_stream.seekg(padding_size, std::ios_base::cur).fail()) {
                    _set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                    return;
                }

                current_line_ptr += line_width;
            }
        }

        // once here, everything was fine!
        _clr_err();
    }


    void BMPLoader::_reverse_lines_ordering() noexcept
    {
        if (this->is_ok()) {
            const std::size_t line_width{ this->width() * sizeof MyBaseClass::pixel_type };

            std::vector<std::uint8_t> tmp_line;
            tmp_line.assign(line_width, '\0');

            std::uint8_t* upline_ptr{ reinterpret_cast<std::uint8_t*>(this->image_content_ptr()) };
            std::uint8_t* botline_ptr{ reinterpret_cast<std::uint8_t*>(this->image_content_ptr() + (this->height() - 1) * this->width()) };
            std::uint8_t* tmpline_ptr{ tmp_line.data() };

            for (std::size_t i = 0; i < this->height() / 2; ++i) {
                std::memcpy(tmpline_ptr, upline_ptr, line_width);
                std::memcpy(upline_ptr, botline_ptr, line_width);
                std::memcpy(botline_ptr, tmpline_ptr, line_width);

                upline_ptr += line_width;
                botline_ptr -= line_width;
            }
        }
    }

}