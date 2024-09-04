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
#include "utils/warnings.h"


namespace bmpl
{
    //===========================================================================
    // Forward declarations
    //
    template<typename PixelT> struct BMPLoader;
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

        BMPBottomUpLoader(
            const std::string& filepath,
            const ESkippedPixelsMode mode = ESkippedPixelsMode::BLACK,
            const bool apply_gamma_corection = false
        ) noexcept;

        virtual inline ~BMPBottomUpLoader() noexcept = default;

        [[nodiscard]]
        inline const std::string err_msg() const noexcept
        {
            return bmpl::utils::error_msg(_filepath, get_error());
        }

        [[nodiscard]]
        const std::int32_t get_device_x_resolution() const noexcept;

        [[nodiscard]]
        const std::int32_t get_device_y_resolution() const noexcept;

        [[nodiscard]]
        const std::vector<std::string> get_warnings_msg() const noexcept;

        [[nodiscard]]
        const std::int32_t height() const noexcept;

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
        const std::int32_t width() const noexcept;


    protected:
        std::string _filepath{};
        ESkippedPixelsMode _skipped_mode{ ESkippedPixelsMode::BLACK };

        // notice: do not modify the ordering of next three declarations
        bmpl::utils::LEInStream _in_stream;
        const bmpl::frmt::BMPFileHeaderBase* _file_header_ptr{ nullptr };;
        bmpl::frmt::BMPInfo _info;

        void _reverse_lines_ordering() noexcept;


    private:
        const bool _allocate_image_space() noexcept;

        void _load_image(const bool apply_gamma_corection) noexcept;

    };


    //===========================================================================
    template<typename PixelT>
    struct BMPLoader : public BMPBottomUpLoader<PixelT>
    {
        using MyBaseClass = BMPBottomUpLoader<PixelT>;

        inline BMPLoader(
            const std::string& filepath,
            const ESkippedPixelsMode mode = ESkippedPixelsMode::BLACK,
            const bool apply_gamma_correction = false
        ) noexcept
            : MyBaseClass(filepath, mode, apply_gamma_correction)
        {
            if (MyBaseClass::_info.info_header_ptr != nullptr &&
                !MyBaseClass::_info.info_header_ptr->top_down_encoding)
            {
                MyBaseClass::_reverse_lines_ordering();
            }
        }

        virtual inline ~BMPLoader() noexcept = default;

    };


    //===========================================================================
    // Local implmentations

    //---------------------------------------------------------------------------
    template<typename PixelT>
    BMPBottomUpLoader<PixelT>::BMPBottomUpLoader(
        const std::string& filepath,
        const ESkippedPixelsMode mode,
        const bool apply_gamma_corection
    ) noexcept
        : MyErrBaseClass()
        , MyWarnBaseClass()
        , _filepath(filepath)
        , _skipped_mode(mode)
        , _in_stream(filepath)
        , _file_header_ptr{ bmpl::frmt::create_file_header(_in_stream) }
        , _info(_in_stream, _file_header_ptr)
    {
        _load_image(apply_gamma_corection);
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    const std::int32_t BMPBottomUpLoader<PixelT>::get_device_x_resolution() const noexcept
    {
        if (this->_info.info_header_ptr == nullptr)
            return 0;
        else
            return this->_info.info_header_ptr->device_x_resolution;
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    const std::int32_t BMPBottomUpLoader<PixelT>::get_device_y_resolution() const noexcept
    {
        if (this->_info.info_header_ptr == nullptr)
            return 0;
        else
            return this->_info.info_header_ptr->device_y_resolution;
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    const std::vector<std::string> BMPBottomUpLoader<PixelT>::get_warnings_msg() const noexcept
    {
        std::vector<std::string> msg_res;
        for (const bmpl::utils::WarningCode warning_code : this->get_warnings())
            msg_res.push_back(bmpl::utils::warning_msg(this->_filepath, warning_code));
        return msg_res;
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    const std::int32_t BMPBottomUpLoader<PixelT>::height() const noexcept
    {
        if (this->_info.info_header_ptr == nullptr)
            return 0;
        else
            return _info.info_header_ptr->get_height();
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    const std::int32_t BMPBottomUpLoader<PixelT>::width() const noexcept
    {
        if (this->_info.info_header_ptr == nullptr)
            return 0;
        else
            return _info.info_header_ptr->get_width();
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    void BMPBottomUpLoader<PixelT>::_reverse_lines_ordering() noexcept
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


    //---------------------------------------------------------------------------
    template<typename PixelT>
    const bool BMPBottomUpLoader<PixelT>::_allocate_image_space() noexcept
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


    //---------------------------------------------------------------------------
    template<typename PixelT>
    void BMPBottomUpLoader<PixelT>::_load_image(const bool apply_gamma_corection) noexcept
    {
        if (this->_in_stream.failed()) {
            _set_err(_in_stream.get_error());
            return;
        }

        if (this->_file_header_ptr == nullptr) {
            _set_err(bmpl::utils::ErrorCode::BAD_FILE_HEADER);
            return;
        }

        if (this->_file_header_ptr->failed()) {
            _set_err(_file_header_ptr->get_error());
        }

        if (this->_info.failed()) {
            _set_err(_info.get_error());
            return;
        }

        // let's set the file cursor position to the starting point of the image coding
        if (this->_in_stream.seekg(_file_header_ptr->get_content_offset()).fail()) {
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
                this->_file_header_ptr,
                this->_info.info_header_ptr,
                this->_info.color_map,
                this->width(),
                this->height()
            )
        };

        if (bitmap_loader_ptr == nullptr) {
            _set_err(bmpl::utils::ErrorCode::BAD_BITS_PER_PIXEL_VALUE);
            return;
        }

        // loads the image bitmap
        if (this->_in_stream.seekg(this->_file_header_ptr->get_content_offset()).fail()) {
            _set_err(bmpl::utils::ErrorCode::ERRONEOUS_BITMAP_OFFSET);
            return;
        }

        if (!bitmap_loader_ptr->load(this->image_content)) {
            _set_err(bitmap_loader_ptr->get_error());
            return;
        }

        // is there gamma correction to apply?
        if (apply_gamma_corection) {
            // yes!
            if (this->_info.info_header_ptr->bits_per_pixel == 64) {
                // ok, gamma correction is already mebedded in HDR encoding
                // nothing to be done
            }
            else if (this->_info.info_header_ptr->is_v4()) {
                const bmpl::frmt::BMPInfoHeaderV4* info_header_ptr = dynamic_cast<const bmpl::frmt::BMPInfoHeaderV4*>(this->_info.info_header_ptr);
                if (info_header_ptr->cs_type == bmpl::clr::ELogicalColorSpace::CALIBRATED_RGB) {
                    const double gamma_r{ double(info_header_ptr->gamma_red) };
                    const double gamma_g{ double(info_header_ptr->gamma_green) };
                    const double gamma_b{ double(info_header_ptr->gamma_blue) };
                    for (auto& pxl : this->image_content)
                        bmpl::clr::gamma_correction(pxl, gamma_r, gamma_g, gamma_b);
                }
            }
            else if (this->_info.info_header_ptr->is_v5()) {
                const bmpl::frmt::BMPInfoHeaderV5* info_header_ptr = dynamic_cast<const bmpl::frmt::BMPInfoHeaderV5*>(this->_info.info_header_ptr);
                if (info_header_ptr->cs_type == bmpl::clr::ELogicalColorSpace::CALIBRATED_RGB) {
                    const double gamma_r{ double(info_header_ptr->gamma_red) };
                    const double gamma_g{ double(info_header_ptr->gamma_green) };
                    const double gamma_b{ double(info_header_ptr->gamma_blue) };
                    for (auto& pxl : this->image_content)
                        bmpl::clr::gamma_correction(pxl, gamma_r, gamma_g, gamma_b);
                }
                else
                    if (info_header_ptr->cs_type == bmpl::clr::ELogicalColorSpace::S_RGB) {
                        for (auto& pxl : this->image_content)
                            bmpl::clr::gamma_correction(pxl, 2.2, 2.2, 2.2);  // notice: gamma value 2.2 is an accepted approximation.
                    }
            }
        }
        else if (this->_info.info_header_ptr->bits_per_pixel == 64) {
            // this is a specific case for which an HDR image is decalibrated to be shown on displays
            constexpr double gamma{ double(1.0 / 2.2) };
            for (auto& pxl : this->image_content)
                bmpl::clr::gamma_correction(pxl, gamma, gamma, gamma);
        }

        // once here, everything was fine
        _clr_err();

        // let's finally append any maybe warning detected during processing
        this->append_warnings(*this->_file_header_ptr);
        this->append_warnings(*this->_info.info_header_ptr);
        this->append_warnings(this->_info.color_map);
        this->append_warnings(*bitmap_loader_ptr);

        this->set_unique_warnings();
    }

}