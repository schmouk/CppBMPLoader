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

#include "bmp_file_bitmaps/bitfield_mask.h"
#include "bmp_file_bitmaps/bmp_bitmaps.h"

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
    enum class ESkippedPixelsMode : std::uint8_t
    {
        BLACK = 0,
        TRANSPARENCY,
        PALETTE_INDEX_0
    };


    //===========================================================================
    template<typename PixelT>
    class BMPBottomUpLoader : public bmpl::utils::ErrorStatus, public bmpl::utils::WarningStatus
    {
    public:
        using MyErrBaseClass = bmpl::utils::ErrorStatus;
        using MyWarnBaseClass = bmpl::utils::WarningStatus;

        using pixel_type = PixelT;


        std::vector<pixel_type> image_content;


        inline BMPBottomUpLoader(const std::string& filepath, const ESkippedPixelsMode mode = ESkippedPixelsMode::BLACK) noexcept
            : MyErrBaseClass()
            , MyWarnBaseClass()
            , _filepath(filepath)
            , _skipped_mode(mode)
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
        inline const std::int32_t height() const noexcept
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
        inline const std::int32_t width() const noexcept
        {
            return _info.info_header.width;
        }


    protected:
        std::string _filepath{};
        ESkippedPixelsMode _skipped_mode{ ESkippedPixelsMode::BLACK };

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
        inline const bool _allocate_image_space() noexcept
        {
            try {
                pixel_type pixel_default_value{};
                switch (_skipped_mode)
                {
                case ESkippedPixelsMode::TRANSPARENCY:
                    bmpl::clr::set_full_transparency(pixel_default_value);
                    break;

                case ESkippedPixelsMode::PALETTE_INDEX_0:
                    bmpl::clr::convert(pixel_default_value, this->_info.color_map[0]);
                    break;
                }

                this->image_content.assign(std::size_t(width()) * std::size_t(height()), pixel_default_value);
                return true;
            }
            catch (...) {
                return _set_err(bmpl::utils::ErrorCode::INCOHERENT_IMAGE_DIMENSIONS);
            }
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
            if (!_allocate_image_space())
                return;

            // finally, loads the BMP image content and decodes it
            bmpl::bmpf::BitmapLoaderBase<pixel_type>* bitmap_loader_ptr{
                bmpl::bmpf::create_bitmap_loader<pixel_type>(
                    this->_in_stream,
                    this->_file_header,
                    this->_info.info_header,
                    this->_info.color_map,
                    this->width(),
                    this->height()
                )
            };

            if (bitmap_loader_ptr == nullptr) {
                _set_err(bmpl::utils::ErrorCode::BAD_BITS_PER_PIXEL_VALUE);
                return;
            }

            if (!bitmap_loader_ptr->load(this->image_content)) {
                _set_err(bitmap_loader_ptr->get_error());
                return;
            }
            
            // is there gamma correction to apply?
            if (this->_info.info_header.bmp_v4 && this->_info.info_header.cs_type == bmpl::utils::ELogicalColorSpace::CALIBRATED_RGB) {
                // yes!
                const double gamma_r{ double(this->_info.info_header.gamma_red) };
                const double gamma_g{ double(this->_info.info_header.gamma_green) };
                const double gamma_b{ double(this->_info.info_header.gamma_blue) };
                for (auto& pxl : this->image_content)
                    bmpl::clr::gamma_correction(pxl, gamma_r, gamma_g, gamma_b);
            }

            // once here, everything was fine
            _clr_err();

            // let's finally append any maybe warning detected during processing
            this->append_warnings(this->_file_header);
            this->append_warnings(this->_info.info_header);
            this->append_warnings(this->_info.color_map);
            this->append_warnings(*bitmap_loader_ptr);
        }

    };


    //===========================================================================
    template<typename PixelT>
    class BMPLoader : public BMPBottomUpLoader<PixelT>
    {
    public:
        using MyBaseClass = BMPBottomUpLoader<PixelT>;


        inline BMPLoader(const std::string& filepath, const ESkippedPixelsMode mode = ESkippedPixelsMode::BLACK) noexcept
            : MyBaseClass(filepath, mode)
        {

            if (!MyBaseClass::_info.info_header.top_down_encoding)
                MyBaseClass::_reverse_lines_ordering();
        }


        virtual inline ~BMPLoader() noexcept = default;

    };

}