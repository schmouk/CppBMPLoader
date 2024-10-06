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


#include <cstring>
#include <string>
#include <utility>
#include <vector>


#include "bmp_image.h"


namespace bmpl
{
    //===========================================================================
    // Forward declarations
    
    template<typename PixelT>
    struct BMPBestFittingImage : public bmpl::BMPImage<PixelT>;

    template<typename PixelT>
    struct BMPBestFittingColorsImage : public bmpl::BMPImage<PixelT>;

    template<typename PixelT>
    struct BMPBestFittingResolutionImage : public bmpl::BMPImage<PixelT>;

    template<typename PixelT>
    struct BMPBestFittingSizeImage : public bmpl::BMPImage<PixelT>;


    //===========================================================================
    // Template specializations
    
    using RGBBMPBestFittingImage = BMPBestFittingImage<bmpl::clr::RGB>;
    using RGBABMPBestFittingImage = BMPBestFittingImage<bmpl::clr::RGBA>;
    using BGRBMPBestFittingImage = BMPBestFittingImage<bmpl::clr::BGR>;
    using BGRABMPBestFittingImage = BMPBestFittingImage<bmpl::clr::BGRA>;

    using RGBBMPBestFittingColorsImage = BMPBestFittingColorsImage<bmpl::clr::RGB>;
    using RGBABMPBestFittingColorsImage = BMPBestFittingColorsImage<bmpl::clr::RGBA>;
    using BGRBMPBestFittingColorsImage = BMPBestFittingColorsImage<bmpl::clr::BGR>;
    using BGRABMPBestFittingColorsImage = BMPBestFittingColorsImage<bmpl::clr::BGRA>;

    using RGBBMPBestFittingResolutionImage = BMPBestFittingResolutionImage<bmpl::clr::RGB>;
    using RGBABMPBestFittingResolutionImage = BMPBestFittingResolutionImage<bmpl::clr::RGBA>;
    using BGRBMPBestFittingResolutionImage = BMPBestFittingResolutionImage<bmpl::clr::BGR>;
    using BGRABMPBestFittingResolutionImage = BMPBestFittingResolutionImage<bmpl::clr::BGRA>;

    using RGBBMPBestFittingSizeImage = BMPBestFittingSizeImage<bmpl::clr::RGB>;
    using RGBABMPBestFittingSizeImage = BMPBestFittingSizeImage<bmpl::clr::RGBA>;
    using BGRBMPBestFittingSizeImage = BMPBestFittingSizeImage<bmpl::clr::BGR>;
    using BGRABMPBestFittingSizeImage = BMPBestFittingSizeImage<bmpl::clr::BGRA>;


    //===========================================================================
    template<typename PixelT>
    struct BMPBestFittingImage : public bmpl::BMPImage<PixelT>
    {
        using MyBaseClass = bmpl::BMPImage<PixelT>;

        using pixel_type = PixelT;


        inline BMPBestFittingImage() noexcept;

        inline BMPBestFittingImage(
            const std::string& filepath_,
            const std::uint32_t target_width_,
            const std::uint32_t target_height_,
            const std::uint32_t target_colors_count_,
            const std::int32_t target_dpi_x_resolution_,
            std::int32_t target_dpi_y_resolution_ = 0,
            const bool apply_gamma_corection_ = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode_ = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up_ = false
        ) noexcept;

        BMPBestFittingImage(const BMPBestFittingImage&) noexcept = default;
        BMPBestFittingImage(BMPBestFittingImage&&) noexcept = default;

        virtual inline ~BMPBestFittingImage() noexcept = default;


        [[nodiscard]]
        BMPBestFittingImage& operator=(const BMPBestFittingImage&) noexcept = default;

        [[nodiscard]]
        BMPBestFittingImage& operator=(BMPBestFittingImage&&) noexcept = default;

        const bool load_image(
            const std::string& filepath_,
            const std::uint32_t target_width_,
            const std::uint32_t target_height_,
            const std::uint32_t target_colors_count_,
            const std::int32_t target_dpi_x_resolution_,
            std::int32_t target_dpi_y_resolution_ = 0,
            const bool apply_gamma_corection_ = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode_ = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up_ = false
        ) noexcept;

    };


    //===========================================================================
    template<typename PixelT>
    struct BMPBestFittingColorsImage : public bmpl::BMPImage<PixelT>
    {
        using MyBaseClass = bmpl::BMPImage<PixelT>;

        using pixel_type = PixelT;


        inline BMPBestFittingColorsImage() noexcept;

        inline BMPBestFittingColorsImage(
            const std::string& filepath_,
            const std::uint32_t target_colors_count_,
            const bool apply_gamma_corection_ = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode_ = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up_ = false
        ) noexcept;

        BMPBestFittingColorsImage(const BMPBestFittingColorsImage&) noexcept = default;
        BMPBestFittingColorsImage(BMPBestFittingColorsImage&&) noexcept = default;

        virtual inline ~BMPBestFittingColorsImage() noexcept = default;


        [[nodiscard]]
        BMPBestFittingColorsImage& operator=(const BMPBestFittingColorsImage&) noexcept = default;

        [[nodiscard]]
        BMPBestFittingColorsImage& operator=(BMPBestFittingColorsImage&&) noexcept = default;

        const bool load_image(
            const std::string& filepath_,
            const std::uint32_t target_colors_count_,
            const bool apply_gamma_corection_ = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode_ = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up_ = false
        ) noexcept;

    };


    //===========================================================================
    template<typename PixelT>
    struct BMPBestFittingResolutionImage : public bmpl::BMPImage<PixelT>
    {
        using MyBaseClass = bmpl::BMPImage<PixelT>;

        using pixel_type = PixelT;


        inline BMPBestFittingResolutionImage() noexcept;

        inline BMPBestFittingResolutionImage(
            const std::string& filepath_,
            const std::int32_t target_dpi_x_resolution_,
            std::int32_t target_dpi_y_resolution_ = 0,
            const bool apply_gamma_corection_ = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode_ = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up_ = false
        ) noexcept;

        BMPBestFittingResolutionImage(const BMPBestFittingResolutionImage&) noexcept = default;
        BMPBestFittingResolutionImage(BMPBestFittingResolutionImage&&) noexcept = default;

        virtual inline ~BMPBestFittingResolutionImage() noexcept = default;


        [[nodiscard]]
        BMPBestFittingResolutionImage& operator=(const BMPBestFittingResolutionImage&) noexcept = default;

        [[nodiscard]]
        BMPBestFittingResolutionImage& operator=(BMPBestFittingResolutionImage&&) noexcept = default;

        const bool load_image(
            const std::string& filepath_,
            const std::int32_t target_dpi_x_resolution_,
            std::int32_t target_dpi_y_resolution_ = 0,
            const bool apply_gamma_corection_ = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode_ = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up_ = false
        ) noexcept;

    };


    //===========================================================================
    template<typename PixelT>
    struct BMPBestFittingSizeImage : public bmpl::BMPImage<PixelT>
    {
        using MyBaseClass = bmpl::BMPImage<PixelT>;

        using pixel_type = PixelT;


        inline BMPBestFittingSizeImage() noexcept;

        inline BMPBestFittingSizeImage(
            const std::string& filepath_,
            const std::uint32_t target_width_,
            const std::uint32_t target_height_,
            const bool apply_gamma_corection_ = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode_ = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up_ = false
        ) noexcept;

        BMPBestFittingSizeImage(const BMPBestFittingSizeImage&) noexcept = default;
        BMPBestFittingSizeImage(BMPBestFittingSizeImage&&) noexcept = default;

        virtual inline ~BMPBestFittingSizeImage() noexcept = default;


        [[nodiscard]]
        BMPBestFittingSizeImage& operator=(const BMPBestFittingSizeImage&) noexcept = default;

        [[nodiscard]]
        BMPBestFittingSizeImage& operator=(BMPBestFittingSizeImage&&) noexcept = default;

        const bool load_image(
            const std::string& filepath_,
            const std::uint32_t target_width_,
            const std::uint32_t target_height_,
            const bool apply_gamma_corection_ = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode_ = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up_ = false
        ) noexcept;

    };


    //===========================================================================
    // Local implementations

    //===========================================================================
    // BMPBestFittingImage
    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline BMPBestFittingImage<PixelT>::BMPBestFittingImage() noexcept
        : MyBaseClass()
    {}

    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline BMPBestFittingImage<PixelT>::BMPBestFittingImage(
        const std::string& filepath_,
        const std::uint32_t target_width_,
        const std::uint32_t target_height_,
        const std::uint32_t target_colors_count_,
        const std::int32_t target_dpi_x_resolution_,
        std::int32_t target_dpi_y_resolution_,
        const bool apply_gamma_corection_,
        const bmpl::clr::ESkippedPixelsMode skipped_mode_,
        const bool force_bottom_up_
    ) noexcept
        : MyBaseClass()
    {
        load_image(
            filepath_,
            target_width_,
            target_height_,
            target_colors_count_,
            target_dpi_x_resolution_,
            target_dpi_y_resolution_,
            apply_gamma_corection_,
            skipped_mode_,
            force_bottom_up_
        );
    }

    //---------------------------------------------------------------------------
    template<typename PixelT>
    const bool BMPBestFittingImage<PixelT>::load_image(
        const std::string& filepath_,
        const std::uint32_t target_width_,
        const std::uint32_t target_height_,
        const std::uint32_t target_colors_count_,
        const std::int32_t target_dpi_x_resolution_,
        std::int32_t target_dpi_y_resolution_,
        const bool apply_gamma_corection_,
        const bmpl::clr::ESkippedPixelsMode skipped_mode_,
        const bool force_bottom_up_
    ) noexcept
    {
        if (!_set_bmp_loader(filepath_, apply_gamma_correction_, skipped_mode_, force_bottom_up_))
            return false;

        if (this->_bmp_loader_ptr->load_best_fitting_image_content(width_, height_, colors_count_, dpi_x_resolution_, dpi_y_resolution_))
            return _clr_err();
        else
            return _set_err(this->_bmp_loader_ptr->get_error());
    }


    //===========================================================================
    // BMPBestFittingColorsImage
    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline BMPBestFittingColorsImage<PixelT>::BMPBestFittingColorsImage()
        : MyBaseClass()
    {}

    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline BMPBestFittingColorsImage<PixelT>::BMPBestFittingColorsImage(
        const std::string& filepath_,
        const std::uint32_t target_colors_count_,
        const bool apply_gamma_corection_,
        const bmpl::clr::ESkippedPixelsMode skipped_mode_,
        const bool force_bottom_up_
    ) noexcept
        : MyBaseClass()
    {
        load_image(
            filepath_,
            target_colors_count_,
            apply_gamma_corection_,
            skipped_mode_,
            force_bottom_up_
        );
    }

    //---------------------------------------------------------------------------
    template<typename PixelT>
    const bool BMPBestFittingColorsImage<PixelT>::load_image(
        const std::string& filepath_,
        const std::uint32_t target_colors_count_,
        const bool apply_gamma_corection_,
        const bmpl::clr::ESkippedPixelsMode skipped_mode_,
        const bool force_bottom_up_
    ) noexcept
    {
        if (!_set_bmp_loader(filepath_, apply_gamma_correction_, skipped_mode_, force_bottom_up_))
            return false;

        if (this->_bmp_loader_ptr->load_best_fitting_colors_image_content(colors_count_))
            return _clr_err();
        else
            return _set_err(this->_bmp_loader_ptr->get_error());
    }


    //===========================================================================
    // BMPBestFittingResolutionImage
    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline BMPBestFittingResolutionImage<PixelT>::BMPBestFittingResolutionImage()
        : MyBaseClass()
    {}

    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline BMPBestFittingResolutionImage<PixelT>::BMPBestFittingResolutionImage(
        const std::string& filepath_,
        const std::int32_t target_dpi_x_resolution_,
        std::int32_t target_dpi_y_resolution_,
        const bool apply_gamma_corection_,
        const bmpl::clr::ESkippedPixelsMode skipped_mode_,
        const bool force_bottom_up_
    ) noexcept
        : MyBaseClass()
    {
        load_image(
            filepath_,
            target_dpi_x_resolution_,
            target_dpi_y_resolution_,
            apply_gamma_corection_,
            skipped_mode_,
            force_bottom_up_
        );
    }

    //---------------------------------------------------------------------------
    template<typename PixelT>
    const bool BMPBestFittingResolutionImage<PixelT>::load_image(
        const std::string& filepath_,
        const std::int32_t target_dpi_x_resolution_,
        std::int32_t target_dpi_y_resolution_,
        const bool apply_gamma_corection_,
        const bmpl::clr::ESkippedPixelsMode skipped_mode_,
        const bool force_bottom_up_
    ) noexcept
    {
        if (!_set_bmp_loader(filepath_, apply_gamma_correction_, skipped_mode_, force_bottom_up_))
            return false;

        if (this->_bmp_loader_ptr->load_best_fitting_resolution_image_content(dpi_x_resolution_, dpi_y_resolution_))
            return _clr_err();
        else
            return _set_err(this->_bmp_loader_ptr->get_error());
    }


    //===========================================================================
    // BMPBestFittingSizeImage
    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline BMPBestFittingSizeImage<PixelT>::BMPBestFittingSizeImage()
        : MyBaseClass()
    {}

    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline BMPBestFittingSizeImage<PixelT>::BMPBestFittingSizeImage(
        const std::string& filepath_,
        const std::uint32_t target_width_,
        const std::uint32_t target_height_,
        const bool apply_gamma_corection_,
        const bmpl::clr::ESkippedPixelsMode skipped_mode_,
        const bool force_bottom_up_
    ) noexcept
        : MyBaseClass()
    {
        load_image(
            filepath_,
            target_width_,
            target_height_,
            apply_gamma_corection_,
            skipped_mode_,
            force_bottom_up_
        );
    }

    //---------------------------------------------------------------------------
    template<typename PixelT>
    const bool BMPBestFittingSizeImage<PixelT>::load_image(
        const std::string& filepath_,
        const std::uint32_t target_width_,
        const std::uint32_t target_height_,
        const bool apply_gamma_corection_,
        const bmpl::clr::ESkippedPixelsMode skipped_mode_,
        const bool force_bottom_up_
    ) noexcept
    {
        if (!_set_bmp_loader(filepath_, apply_gamma_correction_, skipped_mode_, force_bottom_up_))
            return false;

        if (this->_bmp_loader_ptr->load_best_fitting_size_image_content(width_, height_))
            return _clr_err();
        else
            return _set_err(this->_bmp_loader_ptr->get_error());
    }

}