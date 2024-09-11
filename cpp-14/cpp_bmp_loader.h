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
#include "bmp_image.h"
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
    template<typename PixelT>
    class BMPBottomUpLoader : public bmpl::utils::ErrorStatus, public bmpl::utils::WarningStatus
    {
    public:
        using MyErrBaseClass = bmpl::utils::ErrorStatus;
        using MyWarnBaseClass = bmpl::utils::WarningStatus;

        using pixel_type = PixelT;

        static constexpr bool APPLY_GAMMA_CORRECTION{ true };


        BMPBottomUpLoader(
            const std::string& filepath,
            const bool apply_gamma_corection = false,
            const bmpl::clr::ESkippedPixelsMode mode = bmpl::clr::ESkippedPixelsMode::BLACK
        ) noexcept;

        BMPBottomUpLoader(
            const std::string& filepath,
            const bmpl::clr::ESkippedPixelsMode mode = bmpl::clr::ESkippedPixelsMode::BLACK
        ) noexcept;

        virtual inline ~BMPBottomUpLoader() noexcept = default;

        [[nodiscard]]
        inline const std::string get_error_msg() const noexcept;

        [[nodiscard]]
        const std::vector<std::string> get_warnings_msg() const noexcept;

        [[nodiscard]]
        inline const std::uint32_t get_height() const noexcept;

        [[nodiscard]]
        inline const std::uint32_t get_width() const noexcept;

        virtual bmpl::BMPImage<PixelT> load_image() noexcept;

        virtual bmpl::BMPImage<PixelT> load_next_image() noexcept;

        [[nodiscard]]
        virtual const std::vector<BMPImage<PixelT>> load_all_images() noexcept;

        [[nodiscard]]
        virtual bmpl::BMPImage<PixelT> load_best_fitting_image(
            const std::uint32_t width,
            const std::uint32_t height,
            const std::uint32_t colors_count,
            const std::uint32_t dpi_x_resolution,
            std::uint32_t dpi_y_resolution = 0
        ) noexcept;

        [[nodiscard]]
        virtual bmpl::BMPImage<PixelT> load_best_fitting_colors_image(
            const std::uint32_t colors_count = 0
        ) noexcept;

        [[nodiscard]]
        virtual bmpl::BMPImage<PixelT> load_best_fitting_resolution_image(
            const std::uint32_t dpi_x_resolution,
            std::uint32_t dpi_y_resolution = 0
        ) noexcept;

        [[nodiscard]]
        virtual bmpl::BMPImage<PixelT> load_best_fitting_size_image(
            const std::uint32_t width,
            const std::uint32_t height
        ) noexcept;


    protected:
        std::string                               _filepath{};
        bmpl::clr::ESkippedPixelsMode             _skipped_mode{ bmpl::clr::ESkippedPixelsMode::BLACK };
        bool                                      _apply_gamma_correction{ !APPLY_GAMMA_CORRECTION };
        // notice: do not modify the ordering of next four declarations since the related intializations MUST BE DONE in this order
        bmpl::utils::LEInStream                   _in_stream{};
        const bmpl::frmt::BMPFileHeaderBase*      _file_header_ptr{ nullptr };;
        bmpl::frmt::BMPInfo                       _info{};
        bmpl::bmpf::BitmapLoaderBase<pixel_type>* _bitmap_loader_ptr{ nullptr };


    private:
        std::vector<pixel_type> _image_content{};

        const bool _allocate_image_space() noexcept;
        void _load_image(const std::size_t content_offset) noexcept;

    };


    //===========================================================================
    template<typename PixelT>
    struct BMPLoader : public BMPBottomUpLoader<PixelT>
    {
        using MyBaseClass = BMPBottomUpLoader<PixelT>;

        inline BMPLoader(
            const std::string& filepath,
            const bool apply_gamma_correction = false,
            const bmpl::clr::ESkippedPixelsMode mode = bmpl::clr::ESkippedPixelsMode::BLACK
        ) noexcept
            : MyBaseClass(filepath, apply_gamma_correction, mode)
        {}

        inline BMPLoader(
            const std::string& filepath,
            const bmpl::clr::ESkippedPixelsMode mode
        ) noexcept
            : MyBaseClass(filepath, mode)
        {}

        virtual inline ~BMPLoader() noexcept = default;

        virtual bmpl::BMPImage<PixelT> load_image() noexcept override;

        virtual bmpl::BMPImage<PixelT> load_next_image() noexcept override;

        [[nodiscard]]
        virtual const std::vector<BMPImage<PixelT>> load_all_images() noexcept override;

        [[nodiscard]]
        virtual bmpl::BMPImage<PixelT> load_best_fitting_image(
            const std::uint32_t width,
            const std::uint32_t height,
            const std::uint32_t colors_count,
            const std::uint32_t dpi_x_resolution,
            std::uint32_t dpi_y_resolution = 0
        ) noexcept override;

        [[nodiscard]]
        virtual bmpl::BMPImage<PixelT> load_best_fitting_colors_image(
            const std::uint32_t colors_count = 0
        ) noexcept override;

        [[nodiscard]]
        virtual bmpl::BMPImage<PixelT> load_best_fitting_resolution_image(
            const std::uint32_t dpi_x_resolution,
            std::uint32_t dpi_y_resolution = 0
        ) noexcept override;

        [[nodiscard]]
        virtual bmpl::BMPImage<PixelT> load_best_fitting_size_image(
            const std::uint32_t width,
            const std::uint32_t height
        ) noexcept override;

    };


    //===========================================================================
    // Local implmentations

    //---------------------------------------------------------------------------
    template<typename PixelT>
    BMPBottomUpLoader<PixelT>::BMPBottomUpLoader(
        const std::string& filepath,
        const bool apply_gamma_correction,
        const bmpl::clr::ESkippedPixelsMode mode
    ) noexcept
        : MyErrBaseClass()
        , MyWarnBaseClass()
        , _filepath(filepath)
        , _skipped_mode(mode)
        , _apply_gamma_correction(apply_gamma_correction)
        , _in_stream(filepath)
        , _file_header_ptr{ bmpl::frmt::create_file_header(_in_stream) }
        , _info(_in_stream, _file_header_ptr)
        , _bitmap_loader_ptr{ bmpl::bmpf::create_bitmap_loader<PixelT>(
            this->_in_stream,
            this->_file_header_ptr,
            this->_info.info_header_ptr,
            this->_info.color_map) }
    {
        if (_in_stream.failed())
            _set_err(_in_stream.get_error());
        else if (_file_header_ptr == nullptr)
            _set_err(bmpl::utils::ErrorCode::BAD_FILE_HEADER);
        else if (_file_header_ptr->failed())
            _set_err(_file_header_ptr->get_error());
        else if (_info.failed())
            _set_err(_info.get_error());
        else if (_bitmap_loader_ptr == nullptr)
            _set_err(bmpl::utils::ErrorCode::BAD_BITS_PER_PIXEL_VALUE);
        else if (_bitmap_loader_ptr->failed())
            _set_err(_bitmap_loader_ptr->get_error());
        else
            _clr_err();
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    BMPBottomUpLoader<PixelT>::BMPBottomUpLoader(
        const std::string& filepath,
        const bmpl::clr::ESkippedPixelsMode mode
    ) noexcept
        : MyErrBaseClass()
        , MyWarnBaseClass()
        , _filepath(filepath)
        , _skipped_mode(mode)
        , _apply_gamma_correction(!APPLY_GAMMA_CORRECTION)
        , _in_stream(filepath)
        , _file_header_ptr{ bmpl::frmt::create_file_header(_in_stream) }
        , _info(_in_stream, _file_header_ptr)
        , _bitmap_loader_ptr{ bmpl::bmpf::create_bitmap_loader<PixelT>(
            this->_in_stream,
            this->_file_header_ptr,
            this->_info.info_header_ptr,
            this->_info.color_map) }
    {
        if (_in_stream.failed())
            _set_err(_in_stream.get_error());
        else if (_file_header_ptr == nullptr)
            _set_err(bmpl::utils::ErrorCode::BAD_FILE_HEADER);
        else if (_file_header_ptr->failed())
            _set_err(_file_header_ptr->get_error());
        else if (_info.failed())
            _set_err(_info.get_error());
        else if (_bitmap_loader_ptr == nullptr)
            _set_err(bmpl::utils::ErrorCode::BAD_BITS_PER_PIXEL_VALUE);
        else if (_bitmap_loader_ptr->failed())
            _set_err(_bitmap_loader_ptr->get_error());
        else
            _clr_err();
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline const std::string BMPBottomUpLoader<PixelT>::get_error_msg() const noexcept
    {
        return bmpl::utils::error_msg(_filepath, get_error());
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline const std::uint32_t BMPBottomUpLoader<PixelT>::get_height() const noexcept
    {
        if (this->_info.info_header_ptr == nullptr)
            return 0;
        else
            return this->_info.info_header_ptr->get_height();
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
    inline const std::uint32_t BMPBottomUpLoader<PixelT>::get_width() const noexcept
    {
        if (this->_info.info_header_ptr == nullptr)
            return 0;
        else
            return this->_info.info_header_ptr->get_width();
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    bmpl::BMPImage<PixelT> BMPBottomUpLoader<PixelT>::load_image() noexcept
    {
        if (failed()) {
            // some former error arised, the error status is transmitted to the returned image
            return bmpl::BMPImage<PixelT>(this->_filepath, *this);
        }

        _load_image(this->_file_header_ptr->get_content_offset());

        if (failed()) {
            // erroneous loading, the error status is transmitted to the returned image
            return bmpl::BMPImage<PixelT>(this->_filepath, *this);
        }
        else {
            // everything was fine
            return bmpl::BMPImage<PixelT>(
                this->_filepath,
                get_width(),
                get_height(),
                this->_info.info_header_ptr->get_colors_count(),
                this->_image_content,
                *this,  // notice: this is the error status
                *this   // notice: this is the warnings status
            );
        }
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    bmpl::BMPImage<PixelT> BMPBottomUpLoader<PixelT>::load_next_image() noexcept
    {
        // TODO: implement this

        return bmpl::BMPImage<PixelT>(this->_filepath, bmpl::utils::ErrorStatus(bmpl::utils::ErrorCode::NOT_YET_IMPLEMENTED_METHOD));
    }

    
    //---------------------------------------------------------------------------
    template<typename PixelT>
    const std::vector<BMPImage<PixelT>> BMPBottomUpLoader<PixelT>::load_all_images() noexcept
    {
        // TODO: implement this

        return std::vector<BMPImage<PixelT>>();
    }

    
    //---------------------------------------------------------------------------
    template<typename PixelT>
    bmpl::BMPImage<PixelT> BMPBottomUpLoader<PixelT>::load_best_fitting_image(
        const std::uint32_t width,
        const std::uint32_t height,
        const std::uint32_t colors_count,
        const std::uint32_t dpi_x_resolution,
        std::uint32_t dpi_y_resolution
    ) noexcept
    {
        if (dpi_y_resolution == 0)
            dpi_y_resolution = dpi_x_resolution;

        // TODO: implement this

        return bmpl::BMPImage<PixelT>(this->_filepath, bmpl::utils::ErrorStatus(bmpl::utils::ErrorCode::NOT_YET_IMPLEMENTED_METHOD));
    }

    
    //---------------------------------------------------------------------------
    template<typename PixelT>
    bmpl::BMPImage<PixelT> BMPBottomUpLoader<PixelT>::load_best_fitting_colors_image(
        const std::uint32_t colors_count
    ) noexcept
    {
        // TODO: implement this

        return bmpl::BMPImage<PixelT>(this->_filepath, bmpl::utils::ErrorStatus(bmpl::utils::ErrorCode::NOT_YET_IMPLEMENTED_METHOD));
    }

    
    //---------------------------------------------------------------------------
    template<typename PixelT>
    bmpl::BMPImage<PixelT> BMPBottomUpLoader<PixelT>::load_best_fitting_resolution_image(
        const std::uint32_t dpi_x_resolution,
        std::uint32_t dpi_y_resolution
    ) noexcept
    {
        if (dpi_y_resolution == 0)
            dpi_y_resolution = dpi_x_resolution;

        // TODO: implement this
        return bmpl::BMPImage<PixelT>(this->_filepath, bmpl::utils::ErrorStatus(bmpl::utils::ErrorCode::NOT_YET_IMPLEMENTED_METHOD));
    }

    
    //---------------------------------------------------------------------------
    template<typename PixelT>
    bmpl::BMPImage<PixelT> BMPBottomUpLoader<PixelT>::load_best_fitting_size_image(
        const std::uint32_t width,
        const std::uint32_t height
    ) noexcept
    {
        // TODO: implement this

        return bmpl::BMPImage<PixelT>(this->_filepath, bmpl::utils::ErrorStatus(bmpl::utils::ErrorCode::NOT_YET_IMPLEMENTED_METHOD));
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    const bool BMPBottomUpLoader<PixelT>::_allocate_image_space() noexcept
    {
        try {
            pixel_type pixel_default_value{};
            switch (_skipped_mode)
            {
            case bmpl::clr::ESkippedPixelsMode::TRANSPARENCY:
                bmpl::clr::set_full_transparency(pixel_default_value);
                break;

            case bmpl::clr::ESkippedPixelsMode::PALETTE_INDEX_0:
                bmpl::clr::convert(pixel_default_value, this->_info.color_map[0]);
                break;
            }

            this->_image_content.assign(std::size_t(get_width()) * std::size_t(get_height()), pixel_default_value);
            return true;
        }
        catch (...) {
            return _set_err(bmpl::utils::ErrorCode::INCOHERENT_IMAGE_DIMENSIONS);
        }
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    void BMPBottomUpLoader<PixelT>::_load_image(const std::size_t content_offset) noexcept
    {
        // loads the image bitmap
        if (this->_in_stream.seekg(content_offset).fail()) {
            _set_err(bmpl::utils::ErrorCode::ERRONEOUS_BITMAP_OFFSET);
            return;
        }
        if (!this->_allocate_image_space())
            return;

        if (!_bitmap_loader_ptr->load(this->_image_content)) {
            _set_err(_bitmap_loader_ptr->get_error());
            return;
        }

        // is there gamma correction to apply?
        if (this->_apply_gamma_correction) {
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
                    for (auto& pxl : this->_image_content)
                        bmpl::clr::gamma_correction(pxl, gamma_r, gamma_g, gamma_b);
                }
            }
            else if (this->_info.info_header_ptr->is_v5()) {
                const bmpl::frmt::BMPInfoHeaderV5* info_header_ptr = dynamic_cast<const bmpl::frmt::BMPInfoHeaderV5*>(this->_info.info_header_ptr);
                if (info_header_ptr->cs_type == bmpl::clr::ELogicalColorSpace::CALIBRATED_RGB) {
                    const double gamma_r{ double(info_header_ptr->gamma_red) };
                    const double gamma_g{ double(info_header_ptr->gamma_green) };
                    const double gamma_b{ double(info_header_ptr->gamma_blue) };
                    for (auto& pxl : this->_image_content)
                        bmpl::clr::gamma_correction(pxl, gamma_r, gamma_g, gamma_b);
                }
                else
                    if (info_header_ptr->cs_type == bmpl::clr::ELogicalColorSpace::S_RGB) {
                        for (auto& pxl : this->_image_content)
                            bmpl::clr::gamma_correction(pxl, 2.2, 2.2, 2.2);  // notice: gamma value 2.2 is a widely accepted approximation.
                    }
            }
        }
        else if (this->_info.info_header_ptr->bits_per_pixel == 64) {
            // this is a specific case for which an HDR image has to be decalibrated to be shown on displays
            constexpr double gamma{ double(1.0 / 2.2) };  // notice: 2.2 is a very common value that suits most cases
            for (auto& pxl : this->_image_content)
                bmpl::clr::gamma_correction(pxl, gamma, gamma, gamma);
        }

        // once here, everything was fine
        _clr_err();

        // let's finally append any maybe warning detected during processing
        this->append_warnings(*this->_file_header_ptr);
        this->append_warnings(*this->_info.info_header_ptr);
        this->append_warnings(this->_info.color_map);
        this->append_warnings(*_bitmap_loader_ptr);

        this->set_unique_warnings();
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    bmpl::BMPImage<PixelT> BMPLoader<PixelT>::load_image() noexcept
    {
        bmpl::BMPImage<PixelT> image{ MyBaseClass::load_image() };

        if (image.is_ok()) {
            if (MyBaseClass::_info.info_header_ptr != nullptr &&
                !MyBaseClass::_info.info_header_ptr->top_down_encoding)
            {
                image.reverse_lines_ordering();
            }
        }
        
        return image;
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    bmpl::BMPImage<PixelT> BMPLoader<PixelT>::load_next_image() noexcept
    {
        bmpl::BMPImage<PixelT> image{ MyBaseClass::load_next_image() };

        if (image.is_ok()) {
            if (MyBaseClass::_info.info_header_ptr != nullptr &&
                !MyBaseClass::_info.info_header_ptr->top_down_encoding)
            {
                image.reverse_lines_ordering();
            }
        }

        return image;
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    const std::vector<BMPImage<PixelT>> BMPLoader<PixelT>::load_all_images() noexcept
    {
        std::vector<BMPImage<PixelT>> images{ MyBaseClass::load_all_images() };

        for (auto& img : images) {
            if (MyBaseClass::_info.info_header_ptr != nullptr &&
                !MyBaseClass::_info.info_header_ptr->top_down_encoding)
            {
                img.reverse_lines_ordering();
            }
        }

        return images;
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    bmpl::BMPImage<PixelT> BMPLoader<PixelT>::load_best_fitting_image(
        const std::uint32_t width,
        const std::uint32_t height,
        const std::uint32_t colors_count,
        const std::uint32_t dpi_x_resolution,
        std::uint32_t dpi_y_resolution
    ) noexcept
    {
        bmpl::BMPImage<PixelT> image{ MyBaseClass::load_best_fitting_image(width, height, colors_count, dpi_x_resolution, dpi_y_resolution) };

        if (image.is_ok()) {
            if (MyBaseClass::_info.info_header_ptr != nullptr &&
                !MyBaseClass::_info.info_header_ptr->top_down_encoding)
            {
                image.reverse_lines_ordering();
            }
        }

        return image;
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    bmpl::BMPImage<PixelT> BMPLoader<PixelT>::load_best_fitting_colors_image(
        const std::uint32_t colors_count
    ) noexcept
    {
        bmpl::BMPImage<PixelT> image{ MyBaseClass::load_best_fitting_colors_image(colors_count) };

        if (image.is_ok()) {
            if (MyBaseClass::_info.info_header_ptr != nullptr &&
                !MyBaseClass::_info.info_header_ptr->top_down_encoding)
            {
                image.reverse_lines_ordering();
            }
        }

        return image;
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    bmpl::BMPImage<PixelT> BMPLoader<PixelT>::load_best_fitting_resolution_image(
        const std::uint32_t dpi_x_resolution,
        std::uint32_t dpi_y_resolution
    ) noexcept
    {
        bmpl::BMPImage<PixelT> image{ MyBaseClass::load_best_fitting_resolution_image(dpi_x_resolution, dpi_y_resolution) };

        if (image.is_ok()) {
            if (MyBaseClass::_info.info_header_ptr != nullptr &&
                !MyBaseClass::_info.info_header_ptr->top_down_encoding)
            {
                image.reverse_lines_ordering();
            }
        }

        return image;
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    bmpl::BMPImage<PixelT> BMPLoader<PixelT>::load_best_fitting_size_image(
        const std::uint32_t width,
        const std::uint32_t height
    ) noexcept
    {
        bmpl::BMPImage<PixelT> image{ MyBaseClass::load_best_fitting_size_image(width, height) };

        if (image.is_ok()) {
            if (MyBaseClass::_info.info_header_ptr != nullptr &&
                !MyBaseClass::_info.info_header_ptr->top_down_encoding)
            {
                image.reverse_lines_ordering();
            }
        }

        return image;
    }

}