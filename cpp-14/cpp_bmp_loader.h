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

#include "bmp_file_format/ba_header.h"
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
        inline const std::uint32_t get_height(const bmpl::frmt::BAHeader& ba_header) const noexcept;

        [[nodiscard]]
        inline const std::uint32_t get_width() const noexcept;

        [[nodiscard]]
        inline const std::uint32_t get_width(const bmpl::frmt::BAHeader& ba_header) const noexcept;

        virtual bmpl::BMPImage<PixelT> load_image() noexcept;

        virtual bmpl::BMPImage<PixelT> load_next_image() noexcept;

        [[nodiscard]]
        virtual const std::vector<BMPImage<PixelT>> load_all_images() noexcept;

        [[nodiscard]]
        virtual bmpl::BMPImage<PixelT> load_best_fitting_image(
            const std::uint32_t width,
            const std::uint32_t height,
            const std::uint32_t colors_count,
            const std::int32_t dpi_x_resolution,
            std::int32_t dpi_y_resolution = 0
        ) noexcept;

        [[nodiscard]]
        virtual bmpl::BMPImage<PixelT> load_best_fitting_colors_image(
            const std::uint32_t colors_count = 0
        ) noexcept;

        [[nodiscard]]
        virtual bmpl::BMPImage<PixelT> load_best_fitting_resolution_image(
            const std::int32_t dpi_x_resolution,
            std::int32_t dpi_y_resolution = 0
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
        const bmpl::frmt::BMPFileHeaderBase*      _file_header_ptr{ nullptr };
        bmpl::frmt::BMPInfo                       _info{};
        bmpl::bmpf::BitmapLoaderBase<pixel_type>* _bitmap_loader_ptr{ nullptr };


    private:
        std::vector<pixel_type> _image_content{};
        std::vector<bmpl::frmt::BAHeader> _ba_headers_list{};
        int _ba_current_header_index{ -1 };

        const bool _allocate_image_space(const std::size_t width, const std::size_t height) noexcept;

        inline const std::uint32_t _evaluate_colors_count(const std::uint32_t colors_count) const noexcept
        {
            return (colors_count == 0) ? 0xffff'ffff : colors_count;
        }

        std::vector<bmpl::frmt::BAHeader> _load_BA_headers() noexcept;

        bmpl::BMPImage<PixelT> _load_image(const std::size_t content_offset, const std::uint32_t width, const std::uint32_t height) noexcept;

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
            const std::int32_t dpi_x_resolution,
            std::int32_t dpi_y_resolution = 0
        ) noexcept override;

        [[nodiscard]]
        virtual bmpl::BMPImage<PixelT> load_best_fitting_colors_image(
            const std::uint32_t colors_count = 0
        ) noexcept override;

        [[nodiscard]]
        virtual bmpl::BMPImage<PixelT> load_best_fitting_resolution_image(
            const std::int32_t dpi_x_resolution,
            std::int32_t dpi_y_resolution = 0
        ) noexcept override;

        [[nodiscard]]
        virtual bmpl::BMPImage<PixelT> load_best_fitting_size_image(
            const std::uint32_t width,
            const std::uint32_t height
        ) noexcept override;

    };


    //===========================================================================
    // Local implementations  -  BMPBottomUpLoader<PixelT>

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
    inline const std::uint32_t BMPBottomUpLoader<PixelT>::get_height(const bmpl::frmt::BAHeader& ba_header) const noexcept
    {
        return this->ba_header.get_height();
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
    inline const std::uint32_t BMPBottomUpLoader<PixelT>::get_width(const bmpl::frmt::BAHeader& ba_header) const noexcept
    {
        return this->ba_header.get_width();
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    bmpl::BMPImage<PixelT> BMPBottomUpLoader<PixelT>::load_image() noexcept
    {
        if (failed()) {
            // some former error arised, the error status is transmitted to the returned image
            return bmpl::BMPImage<PixelT>(this->_filepath, *this);
        }

        if (this->_file_header_ptr->is_BA_file()) {
            // this is a multi array bitmaps file, let's load the very first image in array
            this->_in_stream.seekg(sizeof std::int16_t);
            bmpl::frmt::BAHeader first_ba_header(this->_in_stream);
            this->_bitmap_loader_ptr = bmpl::bmpf::create_bitmap_loader<PixelT>(
                this->_in_stream,
                first_ba_header.file_header_ptr,
                first_ba_header.info_header_ptr,
                first_ba_header.color_map
            );

            return _load_image(first_ba_header.get_content_offset(), first_ba_header.get_width(), first_ba_header.get_height());
        }
        else {
            // this is a BMP image file
            return _load_image(this->_file_header_ptr->get_content_offset(), get_width(), get_height());
        }
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    bmpl::BMPImage<PixelT> BMPBottomUpLoader<PixelT>::load_next_image() noexcept
    {
        if (failed()) {
            // some former error has arised, the error status is transmitted to the returned image
            return bmpl::BMPImage<PixelT>(this->_filepath, *this);
        }

        if (!this->_file_header_ptr->is_BA_file())
            return bmpl::BMPImage<PixelT>(this->_filepath, bmpl::utils::ErrorCode::END_OF_BITMAP_ARRAY);

        if (this->_ba_current_header_index == -1) {
            this->_ba_headers_list = _load_BA_headers();
        }

        ++this->_ba_current_header_index;
        if (this->_ba_current_header_index >= this->_ba_headers_list.size())
            return bmpl::BMPImage<PixelT>(this->_filepath, bmpl::utils::ErrorCode::END_OF_BA_HEADERS_LIST);
        
        bmpl::frmt::BAHeader ba_header{ this->_ba_headers_list[this->_ba_current_header_index] };
        this->_bitmap_loader_ptr = bmpl::bmpf::create_bitmap_loader<PixelT>(
            this->_in_stream,
            ba_header.file_header_ptr,
            ba_header.info_header_ptr,
            ba_header.color_map
        );

        return _load_image(ba_header.get_content_offset(), ba_header.get_width(), ba_header.get_height());
    }

    
    //---------------------------------------------------------------------------
    template<typename PixelT>
    const std::vector<BMPImage<PixelT>> BMPBottomUpLoader<PixelT>::load_all_images() noexcept
    {
        std::vector<BMPImage<PixelT>> ret_images{};

        if (failed()) {
            // some former error arised, the error status is transmitted to the returned image
            return ret_images;
        }

        if (!_file_header_ptr->is_BA_file()) {
            // only one image to be loaded, can't provide a better one.
            ret_images.push_back(load_image());
            return ret_images;
        }

        // ok, "BA" file
        std::vector<bmpl::frmt::BAHeader> ba_headers_list{ _load_BA_headers() };
        for (bmpl::frmt::BAHeader& ba_header : ba_headers_list)
            ret_images.push_back(_load_image(ba_header.get_content_offset(), ba_header.get_width(), ba_header.get_height()));

        return ret_images;
    }

    
    //---------------------------------------------------------------------------
    template<typename PixelT>
    bmpl::BMPImage<PixelT> BMPBottomUpLoader<PixelT>::load_best_fitting_image(
        const std::uint32_t width,
        const std::uint32_t height,
        const std::uint32_t colors_count,
        const std::int32_t dpi_x_resolution,
        std::int32_t dpi_y_resolution
    ) noexcept
    {
        if (failed()) {
            // some former error arised, the error status is transmitted to the returned image
            return bmpl::BMPImage<PixelT>(this->_filepath, *this);
        }

        if (!_file_header_ptr->is_BA_file()) {
            // only one image to be loaded, can't provide a better one.
            return load_image();
        }

        // ok, "BA" file
        if (dpi_y_resolution == 0)
            dpi_y_resolution = dpi_x_resolution;

        std::uint32_t min_diff_colors{ 0xffff'ffff };
        std::uint32_t min_diff_dims{ 0xffff'ffff };
        std::uint32_t min_diff_resolution{ 0xffff'ffff };
        std::uint32_t diff;
        bmpl::frmt::BAHeader best_fitting_header{};
        bool found{ false };

        const std::uint32_t _colors_count{ _evaluate_colors_count(colors_count) };

        std::vector<bmpl::frmt::BAHeader> ba_headers_list{ _load_BA_headers() };

        for (bmpl::frmt::BAHeader& ba_header : ba_headers_list) {

            // let's first compare dimensions of images
            const std::uint32_t hdr_width{ ba_header.get_width() };
            const std::uint32_t hdr_height{ ba_header.get_height() };
            if (hdr_width <= width && hdr_height <= height) {
                diff = (width - hdr_width) + (height - hdr_height);
                if (diff < min_diff_dims) {
                    min_diff_dims = diff;
                    best_fitting_header = ba_header;
                    found = true;
                }
                else if (diff == min_diff_dims) {
                    // equality of sizes fitting, let's compare then resolutions
                    const std::int32_t img_dpi_x_resolution{ ba_header.get_device_x_resolution_dpi() };
                    const std::int32_t img_dpi_y_resolution{ ba_header.get_device_y_resolution_dpi() };
                    if (img_dpi_x_resolution <= dpi_x_resolution && img_dpi_y_resolution <= dpi_y_resolution) {
                        diff = (dpi_x_resolution - img_dpi_x_resolution) + (dpi_y_resolution - img_dpi_y_resolution);
                        if (diff < min_diff_resolution) {
                            min_diff_resolution = diff;
                            best_fitting_header = ba_header;
                            found = true;
                        }
                        else if (diff == min_diff_resolution) {
                            // equality of sizes and resoltuons fittings, let's compare then colors counts
                            const std::uint32_t _hdr_colors_count{ _evaluate_colors_count(ba_header.get_colors_count()) };
                            if (_hdr_colors_count <= _colors_count) {
                                diff = _colors_count - _hdr_colors_count;
                                if (diff < min_diff_colors) {
                                    min_diff_colors = diff;
                                    best_fitting_header = ba_header;
                                    found = true;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (!found)
            // no best fitting image found, so let's return the first one in list as the BMP standard asks for
            best_fitting_header = ba_headers_list[0];

        return _load_image(
            best_fitting_header.get_content_offset(),
            best_fitting_header.get_width(),
            best_fitting_header.get_height()
        );
    }

    
    //---------------------------------------------------------------------------
    template<typename PixelT>
    bmpl::BMPImage<PixelT> BMPBottomUpLoader<PixelT>::load_best_fitting_colors_image(
        const std::uint32_t colors_count
    ) noexcept
    {
        if (failed()) {
            // some former error arised, the error status is transmitted to the returned image
            return bmpl::BMPImage<PixelT>(this->_filepath, *this);
        }

        if (!_file_header_ptr->is_BA_file()) {
            // only one image to be loaded, can't provide a better one.
            return load_image();
        }

        // ok, "BA" file
        std::uint32_t min_diff{ 0xffff'ffff };
        bmpl::frmt::BAHeader best_fitting_header{};
        bool found{ false };
        const std::uint32_t _colors_count{ _evaluate_colors_count(colors_count) };

        std::vector<bmpl::frmt::BAHeader> ba_headers_list{ _load_BA_headers() };

        for (bmpl::frmt::BAHeader& ba_header : ba_headers_list) {

            const std::uint32_t _hdr_colors_count{ _evaluate_colors_count(ba_header.get_colors_count()) };
            if (_hdr_colors_count <= _colors_count) {
                const std::uint32_t diff{ _colors_count - _hdr_colors_count };
                if (diff < min_diff) {
                    min_diff = diff;
                    best_fitting_header = ba_header;
                    found = true;
                }
            }
        }

        if (!found)
            // no best fitting image found, so let's return the first one in list as the BMP standard asks for
            best_fitting_header = ba_headers_list[0];

        return _load_image(
            best_fitting_header.get_content_offset(),
            best_fitting_header.get_width(),
            best_fitting_header.get_height()
        );
    }

    
    //---------------------------------------------------------------------------
    template<typename PixelT>
    bmpl::BMPImage<PixelT> BMPBottomUpLoader<PixelT>::load_best_fitting_resolution_image(
        const std::int32_t dpi_x_resolution,
        std::int32_t dpi_y_resolution
    ) noexcept
    {
        if (dpi_y_resolution == 0)
            dpi_y_resolution = dpi_x_resolution;

        if (failed()) {
            // some former error arised, the error status is transmitted to the returned image
            return bmpl::BMPImage<PixelT>(this->_filepath, *this);
        }

        if (!_file_header_ptr->is_BA_file()) {
            // only one image to be loaded, can't provide a better one.
            return load_image();
        }

        // ok, "BA" file
        std::int32_t min_diff{ 0x7fff'ffff };
        bmpl::frmt::BAHeader best_fitting_header{};
        bool found{ false };

        std::vector<bmpl::frmt::BAHeader> ba_headers_list{ _load_BA_headers() };

        for (bmpl::frmt::BAHeader& ba_header : ba_headers_list) {

            const std::int32_t img_dpi_x_resolution{ ba_header.get_device_x_resolution_dpi() };
            const std::int32_t img_dpi_y_resolution{ ba_header.get_device_y_resolution_dpi() };
            if (img_dpi_x_resolution <= dpi_x_resolution && img_dpi_y_resolution <= dpi_y_resolution) {
                const std::int32_t diff{ (dpi_x_resolution - img_dpi_x_resolution) + (dpi_y_resolution - img_dpi_y_resolution) };
                if (diff < min_diff) {
                    min_diff = diff;
                    best_fitting_header = ba_header;
                    found = true;
                }
            }
        }

        if (!found)
            // no best fitting image found, so let's return the first one in list as the BMP standard asks for
            best_fitting_header = ba_headers_list[0];

        return _load_image(
            best_fitting_header.get_content_offset(),
            best_fitting_header.get_width(),
            best_fitting_header.get_height()
        );
    }

    
    //---------------------------------------------------------------------------
    template<typename PixelT>
    bmpl::BMPImage<PixelT> BMPBottomUpLoader<PixelT>::load_best_fitting_size_image(
        const std::uint32_t width,
        const std::uint32_t height
    ) noexcept
    {
        if (failed()) {
            // some former error arised, the error status is transmitted to the returned image
            return bmpl::BMPImage<PixelT>(this->_filepath, *this);
        }

        if (!_file_header_ptr->is_BA_file()) {
            // only one image to be loaded, can't provide a better one.
            return load_image();
        }

        // ok, "BA" file
        std::size_t min_diff{ 0xffff'ffff };
        bmpl::frmt::BAHeader best_fitting_header{};
        bool found{ false };

        std::vector<bmpl::frmt::BAHeader> ba_headers_list{ _load_BA_headers() };

        for (bmpl::frmt::BAHeader& ba_header : ba_headers_list) {
            const std::size_t hdr_width{ ba_header.get_width() };
            const std::size_t hdr_height{ ba_header.get_height() };
            if (hdr_width <= width && hdr_height <= height) {
                const std::size_t diff{ (width - hdr_width) + (height - hdr_height) };
                if (diff < min_diff) {
                    min_diff = diff;
                    best_fitting_header = ba_header;
                    found = true;
                }
            }
        }

        if (!found)
            // no best fitting image found, so let's return the first one in list as the BMP standard asks for
            best_fitting_header = ba_headers_list[0];

        return _load_image(
            best_fitting_header.get_content_offset(),
            best_fitting_header.get_width(),
            best_fitting_header.get_height()
        );
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    const bool BMPBottomUpLoader<PixelT>::_allocate_image_space(const std::size_t width, const std::size_t height) noexcept
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

            this->_image_content.assign(width * height, pixel_default_value);
            return true;
        }
        catch (...) {
            return _set_err(bmpl::utils::ErrorCode::INCOHERENT_IMAGE_DIMENSIONS);
        }
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    std::vector<bmpl::frmt::BAHeader> BMPBottomUpLoader<PixelT>::_load_BA_headers() noexcept
    {
        std::vector<bmpl::frmt::BAHeader> ret_headers{};

        if (this->_file_header_ptr == nullptr || !this->_file_header_ptr->is_BA_file())
            return ret_headers;

        this->_in_stream.seekg(0);
        do {
            std::uint16_t file_type;
            if (!(this->_in_stream >> file_type)) {
                _set_err(this->_in_stream.get_error());
                break;
            }

            if (file_type != 0x4142) {
                _set_err(bmpl::utils::ErrorCode::NOT_BITMAP_ARRAY_FILE_HEADER);
                break;
            }

            bmpl::frmt::BAHeader ba_header(this->_in_stream);

            if (ba_header.failed()) {
                _set_err(ba_header.get_error());
                break;
            }
            else {
                ret_headers.push_back(ba_header);
                this->_in_stream.seekg(ba_header.get_offset_to_next());
            }

        } while (!ret_headers.back().is_last_header_in_list());

        return ret_headers;
    }

    //---------------------------------------------------------------------------
    template<typename PixelT>
    bmpl::BMPImage<PixelT> BMPBottomUpLoader<PixelT>::_load_image(
        const std::size_t content_offset,
        const std::uint32_t width,
        const std::uint32_t height
    ) noexcept
    {
        // loads the image bitmap
        if (this->_in_stream.seekg(content_offset).fail()) {
            _set_err(bmpl::utils::ErrorCode::ERRONEOUS_BITMAP_OFFSET);
            return bmpl::BMPImage<PixelT>(this->_filepath, *this);
        }

        if (!this->_allocate_image_space(width, height))
            return bmpl::BMPImage<PixelT>(this->_filepath, *this);

        if (!this->_bitmap_loader_ptr->load(this->_image_content)) {
            _set_err(_bitmap_loader_ptr->get_error());
            return bmpl::BMPImage<PixelT>(this->_filepath, *this);
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


        // and everything was fine
        return bmpl::BMPImage<PixelT>(
            this->_filepath,
            width,
            height,
            this->_info.info_header_ptr->get_colors_count(),
            this->_image_content,
            *this,  // notice: this is the error status
            *this   // notice: this is the warnings status
        );

    }


    //===========================================================================
    // Local implementations  -  BMPLoader<PixelT>
    // 
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
        const std::int32_t dpi_x_resolution,
        std::int32_t dpi_y_resolution
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
        const std::int32_t dpi_x_resolution,
        std::int32_t dpi_y_resolution
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