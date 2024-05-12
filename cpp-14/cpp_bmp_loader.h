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


// Let's exclude rarely-used stuff from Windows headers
// (gets no effect on non Windows platforms)
#define WIN32_LEAN_AND_MEAN


#include <cmath>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "utils/bitfield_mask.h"

#include "bmp_file_format/bmp_file_header.h"
#include "bmp_file_format/bmp_info.h"
#include "utils/colors.h"
#include "utils/errors.h"
#include "utils/little_endian_streaming.h"


namespace bmpl
{
    //===========================================================================
    // Forward declarations
    //
    template<typename PixelT> class BMPLoader;
    template<typename PixelT> class BMPBottomUpLoader;


    //===========================================================================
    // Types wrappers
    //
    using RGBBMPLoader = BMPLoader<bmpl::clr::RGB>;
    using RGBABMPLoader = BMPLoader<bmpl::clr::RGBA>;

    using RGBBottomUpLoader = BMPBottomUpLoader<bmpl::clr::RGB>;
    using RGBABottomUpLoader = BMPBottomUpLoader<bmpl::clr::RGBA>;

    using BGRBMPLoader = BMPLoader<bmpl::clr::BGR>;
    using BGRABMPLoader = BMPLoader<bmpl::clr::BGRA>;

    using BGRBottomUpLoader = BMPBottomUpLoader<bmpl::clr::BGR>;
    using BGRABottomUpLoader = BMPBottomUpLoader<bmpl::clr::BGRA>;


    //===========================================================================
    template<typename PixelT>
    class BMPBottomUpLoader : public bmpl::utils::ErrorStatus, public bmpl::utils::WarningStatus
    {
    public:
        using MyErrBaseClass = bmpl::utils::ErrorStatus;
        using MyWarnBaseClass = bmpl::utils::WarningStatus;

        using pixel_type = PixelT;


        std::vector<pixel_type> image_content;


        inline BMPBottomUpLoader(const std::string& filepath) noexcept
            : MyErrBaseClass()
            , MyWarnBaseClass()
            , _filepath(filepath)
            , _in_stream(filepath)
            , _file_header(_in_stream)
            , _info(_in_stream)
        {
            _load_image();
        }


        virtual inline ~BMPBottomUpLoader() noexcept = default;


        [[nodiscard]]
        inline const std::string err_msg() const noexcept
        {
            return bmpl::utils::error_msg(_filepath, get_error());
        }


        [[nodiscard]]
        inline const std::int32_t get_device_x_resolution() const noexcept
        {
            return this->_info.info_header.device_x_resolution;
        }


        [[nodiscard]]
        inline const std::int32_t get_device_y_resolution() const noexcept
        {
            return this->_info.info_header.device_y_resolution;
        }


        [[nodiscard]]
        const std::vector<std::string> get_warnings_msg() const noexcept
        {
            std::vector<std::string> msg_res;
            for (const bmpl::utils::WarningCode warning_code : this->get_warnings())
                msg_res.push_back(bmpl::utils::warning_msg(this->_filepath, warning_code));
            return msg_res;
        }


        [[nodiscard]]
        inline const std::uint32_t height() const noexcept
        {
            return _info.info_header.height;
        }


        [[nodiscard]]
        inline pixel_type* image_content_ptr() noexcept
        {
            return image_content.data();
        }


        [[nodiscard]]
        inline const std::uint32_t image_size() const noexcept
        {
            return height() * width();
        }


        [[nodiscard]]
        inline const std::uint32_t width() const noexcept
        {
            return _info.info_header.width;
        }


    protected:
        std::string _filepath{};

        // notice: do not modify the ordering of next declarations
        bmpl::utils::LEInStream _in_stream;
        bmpl::frmt::BMPFileHeader _file_header;
        bmpl::frmt::BMPInfo _info;

        void _reverse_lines_ordering() noexcept
        {
            if (this->is_ok()) {
                const std::size_t line_width{ this->width() * sizeof pixel_type };

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


    private:
        inline void _allocate_image_space() noexcept
        {
            this->image_content.assign(std::size_t(width()) * std::size_t(height()), pixel_type());
        }


        void _load_image() noexcept
        {
            if (_in_stream.failed()) {
                _set_err(_in_stream.get_error());
                return;
            }

            if (_file_header.failed()) {
                _set_err(_file_header.get_error());
                return;
            }

            if (_info.failed()) {
                _set_err(_info.get_error());
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

            case 16:
                _load_16b();
                break;

            case 24:
                _load_24b();
                break;

            case 32:
                _load_32b();
                break;

            default:
                _set_err(bmpl::utils::ErrorCode::BAD_BITS_PER_PIXEL_VALUE);
                break;
            }

            // let's finally append any maybe warning detected during processing
            this->append_warnings(this->_file_header);
            this->append_warnings(this->_info.info_header);
            this->append_warnings(this->_info.color_map);
        }


        void _load_1b() noexcept
        {
            const std::size_t width{ this->width() };
            const std::size_t index_width{ std::size_t(std::ceil(width / 8.0f)) };
            const std::size_t index_height{ std::size_t(this->height()) };

            if (this->_info.info_header.compression_mode == this->_info.info_header.NO_RLE) {
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
                for (auto ndx_it = indexed_content.cbegin(); ndx_it != indexed_content.cend(); ++ndx_it) {
                    for (std::uint8_t mask = 0x80; remaining_pixels > 0 && mask > 0; mask >>= 1) {
                        bmpl::clr::convert(*img_it++, this->_info.color_map[(*ndx_it & mask) != 0]);
                        --remaining_pixels;
                    }

                    if (remaining_pixels == 0)
                        remaining_pixels = width;
                }
            }
            else {
                // No Run Length Encoding is defined by Windows for 2 colors mapped bitmaps
                _set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
                return;
            }

            // once here, everything was fine!
            _clr_err();
        }


        void _load_4b()
        {
            const std::size_t width{ this->width() };
            const std::size_t index_width{ std::size_t(std::ceil(width / 2.0f)) };
            const std::size_t index_height{ std::size_t(this->height()) };

            if (this->_info.info_header.compression_mode == this->_info.info_header.NO_RLE) {
                //-- no Run Length Encoding --//
                std::vector<std::uint8_t> indexed_content;
                indexed_content.assign(index_width * index_height, 0);

                // loads the indexed content
                if (index_width % 4 == 0) {
                    // cool, no padding at end of each line
                    // let's load the whole bitmap content at once
                    if (this->_in_stream.read(reinterpret_cast<char*>(indexed_content.data()), index_width * index_height).fail())
                        _set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                }
                else {
                    // let's load the bitmap content line per line
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
                auto ndx_it{ indexed_content.cbegin() };
                while (ndx_it != indexed_content.cend() && img_it != this->image_content.end())
                {
                    bmpl::clr::convert(*img_it++, this->_info.color_map[*ndx_it >> 4]);
                    if (--remaining_pixels > 0 && img_it != this->image_content.end()) {
                        bmpl::clr::convert(*img_it++, this->_info.color_map[*ndx_it & 0x0f]);
                        --remaining_pixels;
                    }
                    if (remaining_pixels == 0)
                        remaining_pixels = width;
                    ++ndx_it;
                }
                if (img_it != this->image_content.end()) {
                    _set_warning(bmpl::utils::WarningCode::TOO_MANY_INDICES_IN_BITMAP);
                }
                else if (ndx_it != indexed_content.cend()) {
                    _set_warning(bmpl::utils::WarningCode::NOT_ENOUGH_INDICES_IN_BITMAP);
                }
            }
            else {
                //-- Run Length encoding --//
                if (this->_info.info_header.compression_mode != this->_info.info_header.RLE_4) {
                    _set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
                    return;
                }

                // loads the RLE-4 bitmap
                const std::uint32_t bitmap_size{ this->_file_header.size - this->_file_header.content_offset };
                std::vector<std::uint8_t> bitmap;
                bitmap.assign(bitmap_size, std::uint8_t(0));
                if (this->_in_stream.read(reinterpret_cast<char*>(bitmap.data()), bitmap_size).fail()) {
                    _set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                    return;
                }

                // parses then the RLE-4 bitmap
                std::uint32_t num_line{ 0 };
                bool encountered_eof{ false };

                auto img_it{ this->image_content.begin() };
                auto bmp_it = bitmap.cbegin();

                while (bmp_it != bitmap.cend()) {
                    if (*bmp_it > 0) {
                        // encoded mode, repetition of same pixel value n-times
                        std::uint8_t n_rep{ *bmp_it++ };
                        pixel_type pxl_value_0, pxl_value_1;
                        bmpl::clr::convert(pxl_value_0, this->_info.color_map[*bmp_it >> 4]);
                        bmpl::clr::convert(pxl_value_1, this->_info.color_map[*bmp_it & 0x0f]);
                        if (bmp_it != bitmap.cend())
                            bmp_it++;
                        while (n_rep--) {
                            if (img_it == this->image_content.end()) {
                                _set_err(bmpl::utils::ErrorCode::BUFFER_OVERFLOW);
                                return;
                            }
                            *img_it++ = pxl_value_0;
                            if (n_rep) {
                                if (img_it == this->image_content.end()) {
                                    _set_err(bmpl::utils::ErrorCode::BUFFER_OVERFLOW);
                                    return;
                                }
                                *img_it++ = pxl_value_1;
                                --n_rep;
                            }
                            else
                                break;  // shortcut, don't test twice!
                        }
                    }
                    else if (bmp_it != bitmap.cend() - 1) {
                        bmp_it++;
                        switch (*bmp_it++)
                        {
                        case 0:
                            // end of line
                            ++num_line;
                            if (num_line == this->height()) {
                                _set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
                                return;
                            }
                            img_it = this->image_content.begin() + std::size_t(num_line * this->width());
                            break;

                        case 1:
                            // end of bitmap
                            encountered_eof = true;
                            if (bmp_it != bitmap.cend()) {
                                _set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
                                return;
                            }
                            break;

                        case 2:
                            // delta-mode
                            {
                                std::uint8_t delta_pxls;
                                std::uint8_t delta_lines;
                                if (bmp_it != bitmap.cend())
                                    delta_pxls = *bmp_it++;
                                if (bmp_it != bitmap.cend())
                                    delta_lines = *bmp_it++;
                                const std::size_t offset{ std::size_t(delta_pxls) + this->width() * std::size_t(delta_lines) };
                                if ((img_it - this->image_content.begin()) + offset > this->image_size()) {
                                    _set_err(bmpl::utils::ErrorCode::INCOHERENT_DELTA_MODE_VALUES);
                                    return;
                                }
                                img_it += offset;
                            }
                            break;

                        default:
                            // absolute mode
                            std::uint8_t absolute_pixels_count{ *(bmp_it - 1) };
                            const int mod4{ absolute_pixels_count % 4 };
                            const bool padding{ mod4 == 1 || mod4 == 2 };
                            pixel_type pxl_value;

                            while (absolute_pixels_count--) {
                                bmpl::clr::convert(pxl_value, this->_info.color_map[*bmp_it >> 4]);
                                if (img_it == this->image_content.end()) {
                                    _set_err(bmpl::utils::ErrorCode::BUFFER_OVERFLOW);
                                    return;
                                }
                                *img_it++ = pxl_value;
                                if (absolute_pixels_count) {
                                    bmpl::clr::convert(pxl_value, this->_info.color_map[*bmp_it & 0x0f]);
                                    if (img_it == this->image_content.end()) {
                                        _set_err(bmpl::utils::ErrorCode::BUFFER_OVERFLOW);
                                        return;
                                    }
                                    *img_it++ = pxl_value;
                                    --absolute_pixels_count;
                                }
                                if (bmp_it != bitmap.cend())
                                    bmp_it++;
                            }

                            if (padding && bmp_it != bitmap.cend())
                                bmp_it++;
                            break;
                        }
                    }
                }
                if (!encountered_eof) {
                    _set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
                    return;
                }
            }

            // once here, everything was fine!
            _clr_err();
        }


        void _load_8b() noexcept
        {
            const std::size_t width{ std::size_t(this->width()) };
            const std::size_t height{ std::size_t(this->height()) };

            if (this->_info.info_header.compression_mode == this->_info.info_header.NO_RLE) {
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
                auto ndx_it{ indexed_content.cbegin() };
                while (ndx_it != indexed_content.cend() && img_it != this->image_content.end())
                    bmpl::clr::convert(*img_it++, this->_info.color_map[*ndx_it++]);

                if (img_it != this->image_content.end()) {
                    _set_warning(bmpl::utils::WarningCode::TOO_MANY_INDICES_IN_BITMAP);
                }
                else if (ndx_it != indexed_content.cend()) {
                    _set_warning(bmpl::utils::WarningCode::NOT_ENOUGH_INDICES_IN_BITMAP);
                }
            }
            else {
                //-- Run Length encoding --//
                if (this->_info.info_header.compression_mode != this->_info.info_header.RLE_8) {
                    _set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
                    return;
                }

                // loads the RLE-8 bitmap
                const std::uint32_t bitmap_size{ this->_file_header.size - this->_file_header.content_offset };
                std::vector<std::uint8_t> bitmap;
                bitmap.assign(bitmap_size, std::uint8_t(0));
                if (this->_in_stream.read(reinterpret_cast<char*>(bitmap.data()), bitmap_size).fail()) {
                    _set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                    return;
                }

                // parses then the RLE-8 bitmap
                std::uint32_t num_line{ 0 };
                bool encountered_eof{ false };

                auto img_it{ this->image_content.begin() };
                auto bmp_it{ bitmap.cbegin() };

                while (bmp_it != bitmap.cend()) {
                    if (*bmp_it > 0) {
                        // encoded mode, repetition of same pixel value n-times
                        std::uint8_t n_rep{ *bmp_it++ };
                        pixel_type pxl_value;
                        bmpl::clr::convert(pxl_value, this->_info.color_map[*bmp_it]);
                        if (bmp_it != bitmap.cend())
                            bmp_it++;
                        while (n_rep--) {
                            if (img_it == this->image_content.end()) {
                                _set_err(bmpl::utils::ErrorCode::BUFFER_OVERFLOW);
                                return;
                            }
                            *img_it++ = pxl_value;
                        }
                    }
                    else if (bmp_it != bitmap.cend()) {
                        bmp_it++;
                        if (bmp_it != bitmap.cend()) {
                            switch (*bmp_it++)
                            {
                            case 0:
                                // end of line
                                ++num_line;
                                if (num_line == this->height()) {
                                    _set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
                                    return;
                                }
                                img_it = this->image_content.begin() + std::size_t(num_line * this->width());
                                break;

                            case 1:
                                // end of bitmap
                                encountered_eof = true;
                                if (bmp_it != bitmap.cend()) {
                                    _set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
                                    return;
                                }
                                break;

                            case 2:
                                // delta-mode
                                {
                                    std::uint8_t delta_pxls;
                                    std::uint8_t delta_lines;
                                    if (bmp_it != bitmap.cend())
                                        delta_pxls = *bmp_it++;
                                    if (bmp_it != bitmap.cend())
                                        delta_lines = *bmp_it++;
                                    const std::size_t offset{ std::size_t(delta_pxls) + this->width() * std::size_t(delta_lines) };
                                    if ((img_it - this->image_content.begin()) + offset > this->image_size()) {
                                        _set_err(bmpl::utils::ErrorCode::INCOHERENT_DELTA_MODE_VALUES);
                                        return;
                                    }
                                    img_it += offset;
                                }
                                break;

                            default:
                                // absolute mode
                                std::uint8_t absolute_pixels_count{ *(bmp_it - 1) };
                                const bool padding{ absolute_pixels_count % 2 != 0 };
                                while (absolute_pixels_count-- && bmp_it != bitmap.cend()) {
                                    if (img_it == this->image_content.end()) {
                                        _set_err(bmpl::utils::ErrorCode::BUFFER_OVERFLOW);
                                        return;
                                    }
                                    bmpl::clr::convert(*img_it++, this->_info.color_map[*bmp_it++]);
                                }
                                if (padding && bmp_it != bitmap.cend())
                                    bmp_it++;
                                break;
                            }
                        }
                    }
                }

                if (!encountered_eof) {
                    _set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
                    return;
                }

            }

            // once here, everything was fine!
            _clr_err();
        }


        void _load_16b() noexcept
        {
            const std::size_t width{ std::size_t(this->width()) };
            const std::size_t height{ std::size_t(this->height()) };
            const std::size_t padding{ (width % 2) != 0 ? std::size_t(1) : std::size_t(0) };
            const std::size_t line_width{ width + padding };
            const std::size_t mask_size{ line_width * height };

            const bmpl::utils::BitfieldMaskBase* red_mask_ptr{ bmpl::utils::create_bitfield_mask(this->_info.info_header.red_mask) };
            const bmpl::utils::BitfieldMaskBase* green_mask_ptr{ bmpl::utils::create_bitfield_mask(this->_info.info_header.green_mask) };
            const bmpl::utils::BitfieldMaskBase* blue_mask_ptr{ bmpl::utils::create_bitfield_mask(this->_info.info_header.blue_mask) };
            const bmpl::utils::BitfieldMaskBase* alpha_mask_ptr{ bmpl::utils::create_bitfield_mask(this->_info.info_header.alpha_mask) };

            std::vector<std::uint16_t> masked_content;
            masked_content.assign(mask_size, std::uint16_t(0));

            if (bmpl::utils::PLATFORM_IS_LITTLE_ENDIAN) {
                if (this->_in_stream.read(reinterpret_cast<char*>(masked_content.data()), mask_size * sizeof std::uint16_t).fail()) {
                    _set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                    return;
                }
            }
            else {
                // big-endian underlying platform
                for (auto mask_it = masked_content.begin(); mask_it != masked_content.end(); ++mask_it) {
                    if ((this->_in_stream >> *mask_it).failed()) {
                        _set_err(this->_in_stream.get_error());
                        return;
                    }
                }
            }

            int pixel_count{ 0 };
            auto mask_it{ masked_content.cbegin() };
            for (auto pixel_it = this->image_content.begin(); pixel_it != this->image_content.end(); ++pixel_it) {
                const std::uint32_t mask_pxl_value{ *mask_it++ };

                const std::uint32_t r{ red_mask_ptr->get_component_value(mask_pxl_value) };
                const std::uint32_t g{ green_mask_ptr->get_component_value(mask_pxl_value) };
                const std::uint32_t b{ blue_mask_ptr->get_component_value(mask_pxl_value) };
                const std::uint32_t a{ alpha_mask_ptr->get_component_value(mask_pxl_value) };

                bmpl::clr::set_pixel(*pixel_it, r, g, b, a);

                if (++pixel_count == width) {
                    mask_it += padding;
                    pixel_count = 0;
                }
            }

            // once here, everything was fine!
            _clr_err();
        }


        void _load_24b() noexcept
        {
            const std::size_t width{ std::size_t(this->width()) };
            const std::size_t height{ std::size_t(this->height()) };

            // let's load the image content line after line
            const std::size_t line_width{ width * 3 };
            const std::size_t padding_size{ 4 - line_width % 4 };

            std::vector<bmpl::clr::BGR> bitmap_line;
            bitmap_line.assign(width, bmpl::clr::BGR());

            char* bitmap_line_ptr{ reinterpret_cast<char*>(bitmap_line.data()) };
            auto img_it{ this->image_content.begin() };
            for (int line = 0; line < height; ++line) {
                if (this->_in_stream.read(bitmap_line_ptr, line_width).fail()) {
                    _set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                    return;
                }

                for (auto& bitmap_pxl : bitmap_line)
                    bmpl::clr::convert(*img_it++, bitmap_pxl);

                if (padding_size > 0 && this->_in_stream.seekg(padding_size, std::ios_base::cur).fail()) {
                    _set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                    return;
                }
            }

            // once here, everything was fine!
            _clr_err();
        }


        void _load_32b() noexcept
        {

            const std::size_t width{ std::size_t(this->width()) };
            const std::size_t height{ std::size_t(this->height()) };
            const std::size_t mask_size{ width * height };

            const bmpl::utils::BitfieldMaskBase* red_mask_ptr{ bmpl::utils::create_bitfield_mask(this->_info.info_header.red_mask) };
            const bmpl::utils::BitfieldMaskBase* green_mask_ptr{ bmpl::utils::create_bitfield_mask(this->_info.info_header.green_mask) };
            const bmpl::utils::BitfieldMaskBase* blue_mask_ptr{ bmpl::utils::create_bitfield_mask(this->_info.info_header.blue_mask) };
            const bmpl::utils::BitfieldMaskBase* alpha_mask_ptr{ bmpl::utils::create_bitfield_mask(this->_info.info_header.alpha_mask) };

            std::vector<std::uint32_t> masked_content;
            masked_content.assign(mask_size, std::uint32_t(0));

            if (bmpl::utils::PLATFORM_IS_LITTLE_ENDIAN) {
                if (this->_in_stream.read(reinterpret_cast<char*>(masked_content.data()), mask_size * sizeof std::uint32_t).fail()) {
                    _set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                    return;
                }
            }
            else {
                // big-endian underlying platform
                for (auto mask_it = masked_content.begin(); mask_it != masked_content.end(); ++mask_it) {
                    if ((this->_in_stream >> *mask_it).failed()) {
                        _set_err(this->_in_stream.get_error());
                        return;
                    }
                }
            }

            auto mask_it{ masked_content.cbegin() };
            for (auto pixel_it = this->image_content.begin(); pixel_it != this->image_content.end(); ++pixel_it) {
                const std::uint32_t mask_pxl_value{ *mask_it++ };

                bmpl::clr::set_pixel(
                    *pixel_it,
                    red_mask_ptr->get_component_value(mask_pxl_value),
                    green_mask_ptr->get_component_value(mask_pxl_value),
                    blue_mask_ptr->get_component_value(mask_pxl_value),
                    alpha_mask_ptr->get_component_value(mask_pxl_value)
                );
            }

            // once here, everything was fine!
            _clr_err();
        }

    };


    //===========================================================================
    template<typename PixelT>
    class BMPLoader : public BMPBottomUpLoader<PixelT>
    {
    public:
        using MyBaseClass = BMPBottomUpLoader<PixelT>;


        inline BMPLoader(const std::string& filepath) noexcept
            : MyBaseClass(filepath)
        {

            if (!MyBaseClass::_info.info_header.top_down_encoding)
                MyBaseClass::_reverse_lines_ordering();
        }


        virtual inline ~BMPLoader() noexcept = default;

    };

}