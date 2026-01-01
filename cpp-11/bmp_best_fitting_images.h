#pragma once

/*
MIT License

Copyright (c) 2024-2026 Philippe Schmouker, ph.schmouker (at) gmail.com

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
    template<typename PixelT>
    struct BMPBestFittingImage : public bmpl::BMPImage<PixelT>
    {
        using MyImageBaseClass = bmpl::BMPImage<PixelT>;

        using pixel_type = PixelT;


        BMPBestFittingImage() noexcept = default;

        inline BMPBestFittingImage(const bmpl::BMPImage<PixelT> image) noexcept;

        inline BMPBestFittingImage(
            const std::string& filepath,
            const std::uint32_t target_width,
            const std::uint32_t target_height,
            const std::uint32_t target_bits_per_pixel,
            const std::int32_t target_dpi_x_resolution,
            std::int32_t target_dpi_y_resolution,
            const bool apply_gamma_corection = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up = false
        ) noexcept;

        inline BMPBestFittingImage(
            const std::string& filepath,
            const std::uint32_t target_width,
            const std::uint32_t target_height,
            const std::uint32_t target_bits_per_pixel,
            const std::int32_t target_dpi_resolution,
            const bool apply_gamma_corection = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up = false
        ) noexcept;

        BMPBestFittingImage(const BMPBestFittingImage&) noexcept = default;
        BMPBestFittingImage(BMPBestFittingImage&&) noexcept = default;

        virtual ~BMPBestFittingImage() noexcept = default;


        BMPBestFittingImage& operator=(const BMPBestFittingImage&) noexcept = default;
        BMPBestFittingImage& operator=(BMPBestFittingImage&&) noexcept = default;

        BMPBestFittingImage load_image(
            const std::string& filepath,
            const std::uint32_t target_width,
            const std::uint32_t target_height,
            const std::uint32_t target_bits_per_pixel,
            const std::int32_t target_dpi_x_resolution,
            std::int32_t target_dpi_y_resolution,
            const bool apply_gamma_corection = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up = false
        ) noexcept;

        inline BMPBestFittingImage load_image(
            const std::string& filepath,
            const std::uint32_t target_width,
            const std::uint32_t target_height,
            const std::uint32_t target_bits_per_pixel,
            const std::int32_t target_dpi_resolution,
            const bool apply_gamma_corection = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up = false
        ) noexcept;

    };


    //===========================================================================
    template<typename PixelT>
    struct BMPBestFittingColorsImage : public bmpl::BMPImage<PixelT>
    {
        using MyImageBaseClass = bmpl::BMPImage<PixelT>;

        using pixel_type = PixelT;


        BMPBestFittingColorsImage() noexcept = default;

        inline BMPBestFittingColorsImage(const bmpl::BMPImage<PixelT> image) noexcept;

        inline BMPBestFittingColorsImage(
            const std::string& filepath,
            const std::uint32_t target_bits_per_pixel,
            const bool apply_gamma_corection = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up = false
        ) noexcept;

        BMPBestFittingColorsImage(const BMPBestFittingColorsImage&) noexcept = default;
        BMPBestFittingColorsImage(BMPBestFittingColorsImage&&) noexcept = default;

        virtual ~BMPBestFittingColorsImage() noexcept = default;


        BMPBestFittingColorsImage& operator=(const BMPBestFittingColorsImage&) noexcept = default;
        BMPBestFittingColorsImage& operator=(BMPBestFittingColorsImage&&) noexcept = default;

        BMPBestFittingColorsImage load_image(
            const std::string& filepath,
            const std::uint32_t target_bits_per_pixel,
            const bool apply_gamma_corection = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up = false
        ) noexcept;

    };


    //===========================================================================
    template<typename PixelT>
    struct BMPBestFittingResolutionImage : public bmpl::BMPImage<PixelT>
    {
        using MyImageBaseClass = bmpl::BMPImage<PixelT>;

        using pixel_type = PixelT;


        BMPBestFittingResolutionImage() noexcept = default;

        inline BMPBestFittingResolutionImage(const bmpl::BMPImage<PixelT> image) noexcept;

        inline BMPBestFittingResolutionImage(
            const std::string& filepath,
            const std::int32_t target_dpi_x_resolution,
            std::int32_t target_dpi_y_resolution,
            const bool apply_gamma_corection = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up = false
        ) noexcept;

        inline BMPBestFittingResolutionImage(
            const std::string& filepath,
            const std::int32_t target_dpi_resolution,
            const bool apply_gamma_corection = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up = false
        ) noexcept;

        BMPBestFittingResolutionImage(const BMPBestFittingResolutionImage&) noexcept = default;
        BMPBestFittingResolutionImage(BMPBestFittingResolutionImage&&) noexcept = default;

        virtual ~BMPBestFittingResolutionImage() noexcept = default;


        BMPBestFittingResolutionImage& operator=(const BMPBestFittingResolutionImage&) noexcept = default;
        BMPBestFittingResolutionImage& operator=(BMPBestFittingResolutionImage&&) noexcept = default;

        BMPBestFittingResolutionImage load_image(
            const std::string& filepath,
            const std::int32_t target_dpi_x_resolution,
            std::int32_t target_dpi_y_resolution,
            const bool apply_gamma_corection = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up = false
        ) noexcept;

        BMPBestFittingResolutionImage load_image(
            const std::string& filepath,
            const std::int32_t target_dpi_resolution,
            const bool apply_gamma_corection = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up = false
        ) noexcept;

    };


    //===========================================================================
    template<typename PixelT>
    struct BMPBestFittingSizeImage : public bmpl::BMPImage<PixelT>
    {
        using MyImageBaseClass = bmpl::BMPImage<PixelT>;

        using pixel_type = PixelT;


        BMPBestFittingSizeImage() noexcept = default;

        inline BMPBestFittingSizeImage(const bmpl::BMPImage<PixelT> image) noexcept;

        inline BMPBestFittingSizeImage(
            const std::string& filepath,
            const std::uint32_t target_width,
            const std::uint32_t target_height,
            const bool apply_gamma_corection = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up_= false
        ) noexcept;

        BMPBestFittingSizeImage(const BMPBestFittingSizeImage&) noexcept = default;
        BMPBestFittingSizeImage(BMPBestFittingSizeImage&&) noexcept = default;

        virtual ~BMPBestFittingSizeImage() noexcept = default;


        BMPBestFittingSizeImage& operator=(const BMPBestFittingSizeImage&) noexcept = default;
        BMPBestFittingSizeImage& operator=(BMPBestFittingSizeImage&&) noexcept = default;

        BMPBestFittingSizeImage load_image(
            const std::string& filepath,
            const std::uint32_t target_width,
            const std::uint32_t target_height,
            const bool apply_gamma_corection = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up = false
        ) noexcept;

    };


    //===========================================================================
    // Local implementations

    //===========================================================================
    // BMPBestFittingImage
    //---------------------------------------------------------------------------
    template<typename PixelT>
    BMPBestFittingImage<PixelT>::BMPBestFittingImage(const bmpl::BMPImage<PixelT> image) noexcept
        : MyImageBaseClass(image)
    {}

    //---------------------------------------------------------------------------
    template<typename PixelT>
    BMPBestFittingImage<PixelT>::BMPBestFittingImage(
        const std::string& filepath,
        const std::uint32_t target_width,
        const std::uint32_t target_height,
        const std::uint32_t target_bits_per_pixel,
        const std::int32_t target_dpi_x_resolution,
        std::int32_t target_dpi_y_resolution,
        const bool apply_gamma_corection,
        const bmpl::clr::ESkippedPixelsMode skipped_mode,
        const bool force_bottom_up
    ) noexcept
        : MyImageBaseClass()
    {
        *this = load_image(
            filepath,
            target_width,
            target_height,
            target_bits_per_pixel,
            target_dpi_x_resolution,
            target_dpi_y_resolution,
            apply_gamma_corection,
            skipped_mode,
            force_bottom_up
        );
    }

    //---------------------------------------------------------------------------
    template<typename PixelT>
    BMPBestFittingImage<PixelT>::BMPBestFittingImage(
        const std::string& filepath,
        const std::uint32_t target_width,
        const std::uint32_t target_height,
        const std::uint32_t target_bits_per_pixel,
        const std::int32_t target_dpi_resolution,
        const bool apply_gamma_corection,
        const bmpl::clr::ESkippedPixelsMode skipped_mode,
        const bool force_bottom_up
    ) noexcept
        : MyImageBaseClass()
    {
        *this = load_image(
            filepath,
            target_width,
            target_height,
            target_bits_per_pixel,
            target_dpi_resolution,
            target_dpi_resolution,
            apply_gamma_corection,
            skipped_mode,
            force_bottom_up
        );
    }

    //---------------------------------------------------------------------------
    template<typename PixelT>
    BMPBestFittingImage<PixelT> BMPBestFittingImage<PixelT>::load_image(
        const std::string& filepath,
        const std::uint32_t target_width,
        const std::uint32_t target_height,
        const std::uint32_t target_bits_per_pixel,
        const std::int32_t target_dpi_x_resolution,
        std::int32_t target_dpi_y_resolution,
        const bool apply_gamma_correction,
        const bmpl::clr::ESkippedPixelsMode skipped_mode,
        const bool force_bottom_up
    ) noexcept
    {
        if (!bmpl::frmt::BAHeader::is_BA_file(filepath)) {
            // only one image to be loaded, can't provide a better one.
            return MyImageBaseClass(filepath, apply_gamma_correction, skipped_mode, force_bottom_up);
        }

        // ok, "BA" file
        if (target_dpi_y_resolution == 0)
            target_dpi_y_resolution = target_dpi_x_resolution;

        bmpl::utils::LEInStream in_stream(filepath);
        if (in_stream.failed()) {
            // some error happenned while attempting to open the compressed image file
            return MyImageBaseClass(in_stream.get_error());
        }

        bmpl::frmt::BAHeadersList ba_headers_list{ bmpl::frmt::BAHeader::get_BA_headers(in_stream) };
        if (ba_headers_list.failed()) {
            // an error happened while reading the list of BA headers
            return MyImageBaseClass(ba_headers_list.get_error());
        }

        std::uint32_t min_diff_bits_per_pixel{ 0xffff'ffff };
        std::uint32_t min_diff_dims{ 0xffff'ffff };
        std::uint32_t min_diff_resolution{ 0xffff'ffff };
        std::uint32_t diff;
        bmpl::frmt::BAHeader best_fitting_header{};
        bool found{ false };

        for (bmpl::frmt::BAHeader& ba_header : ba_headers_list) {

            // let's first compare dimensions of images
            const std::uint32_t hdr_width{ ba_header.get_width() };
            const std::uint32_t hdr_height{ ba_header.get_height() };
            if (hdr_width <= target_width && hdr_height <= target_height) {
                diff = (target_width - hdr_width) + (target_height - hdr_height);
                if (diff < min_diff_dims) {
                    min_diff_dims = diff;
                    min_diff_bits_per_pixel = target_bits_per_pixel - ba_header.get_bits_per_pixel();
                    min_diff_resolution = (target_dpi_x_resolution - ba_header.get_device_x_resolution_dpi()) +
                        (target_dpi_y_resolution - ba_header.get_device_y_resolution_dpi());;
                    best_fitting_header = ba_header;
                    found = true;
                }
                else if (diff == min_diff_dims) {
                    // equality of sizes fitting, let's compare then resolutions
                    const std::int32_t img_dpi_x_resolution{ ba_header.get_device_x_resolution_dpi() };
                    const std::int32_t img_dpi_y_resolution{ ba_header.get_device_y_resolution_dpi() };
                    if (img_dpi_x_resolution <= target_dpi_x_resolution && img_dpi_y_resolution <= target_dpi_y_resolution) {
                        diff = (target_dpi_x_resolution - img_dpi_x_resolution) +
                            (target_dpi_y_resolution - img_dpi_y_resolution);
                        if (diff < min_diff_resolution) {
                            min_diff_resolution = diff;
                            min_diff_bits_per_pixel = target_bits_per_pixel - ba_header.get_bits_per_pixel();
                            best_fitting_header = ba_header;
                            found = true;
                        }
                        else if (diff == min_diff_resolution) {
                            // equality of sizes and resolutions fittings, let's compare then colors counts
                            const std::uint32_t _bits_per_pixel{ ba_header.get_bits_per_pixel() };
                            if (_bits_per_pixel <= target_bits_per_pixel) {
                                diff = target_bits_per_pixel - _bits_per_pixel;
                                if (diff < min_diff_bits_per_pixel) {
                                    min_diff_bits_per_pixel = diff;
                                    best_fitting_header = ba_header;
                                    found = true;
                                }
                            }
                        }
                    }
                }
            }

            if (min_diff_bits_per_pixel == 0 && min_diff_dims == 0 && min_diff_resolution == 0)
                break;  // prunning: we've just found the perfect targetted image!
        }

        if (!found) {
            // no best fitting image found, so let's return the first one in list as the BMP standard asks for
            best_fitting_header = ba_headers_list[0];
        }

        return MyImageBaseClass(
            in_stream,
            best_fitting_header,
            apply_gamma_correction,
            skipped_mode,
            force_bottom_up
        );

    }

    //---------------------------------------------------------------------------
    template<typename PixelT>
    BMPBestFittingImage<PixelT> BMPBestFittingImage<PixelT>::load_image(
        const std::string& filepath,
        const std::uint32_t target_width,
        const std::uint32_t target_height,
        const std::uint32_t target_bits_per_pixel,
        const std::int32_t target_dpi_resolution,
        const bool apply_gamma_corection,
        const bmpl::clr::ESkippedPixelsMode skipped_mode,
        const bool force_bottom_up
    ) noexcept
    {
        return load_image(
            filepath,
            target_width,
            target_height,
            target_bits_per_pixel,
            target_dpi_resolution, 
            target_dpi_resolution,
            apply_gamma_corection,
            skipped_mode,
            force_bottom_up
        );
    }


    //===========================================================================
    // BMPBestFittingColorsImage
    //---------------------------------------------------------------------------
    template<typename PixelT>
    BMPBestFittingColorsImage<PixelT>::BMPBestFittingColorsImage(const bmpl::BMPImage<PixelT> image) noexcept
        : MyImageBaseClass(image)
    {}

    //---------------------------------------------------------------------------
    template<typename PixelT>
    BMPBestFittingColorsImage<PixelT>::BMPBestFittingColorsImage(
        const std::string& filepath,
        const std::uint32_t target_bits_per_pixel,
        const bool apply_gamma_corection,
        const bmpl::clr::ESkippedPixelsMode skipped_mode,
        const bool force_bottom_up
    ) noexcept
        : MyImageBaseClass()
    {
        *this = load_image(
            filepath,
            target_bits_per_pixel,
            apply_gamma_corection,
            skipped_mode,
            force_bottom_up
        );
    }

    //---------------------------------------------------------------------------
    template<typename PixelT>
    BMPBestFittingColorsImage<PixelT> BMPBestFittingColorsImage<PixelT>::load_image(
        const std::string& filepath,
        const std::uint32_t target_bits_per_pixel,
        const bool apply_gamma_correction,
        const bmpl::clr::ESkippedPixelsMode skipped_mode,
        const bool force_bottom_up
    ) noexcept
    {
        if (!bmpl::frmt::BAHeader::is_BA_file(filepath)) {
            // only one image to be loaded, can't provide a better one.
            return MyImageBaseClass(filepath, apply_gamma_correction, skipped_mode, force_bottom_up);
        }

        // ok, "BA" file
        bmpl::utils::LEInStream in_stream(filepath);
        if (in_stream.failed()) {
            // some error happenned while attempting to open the compressed image file
            return MyImageBaseClass(in_stream.get_error());
        }

        bmpl::frmt::BAHeadersList ba_headers_list{ bmpl::frmt::BAHeader::get_BA_headers(in_stream) };
        if (ba_headers_list.failed()) {
            // an error happened while reading the list of BA headers
            return MyImageBaseClass(ba_headers_list.get_error());
        }

        std::uint32_t min_diff_bits_per_pixel{ 0xffff'ffff };
        bmpl::frmt::BAHeader best_fitting_header{};
        bool found{ false };

        for (bmpl::frmt::BAHeader& ba_header : ba_headers_list) {
            // let's compare colors counts
            const std::uint32_t _bits_per_pixel{ ba_header.get_bits_per_pixel() };

            if (_bits_per_pixel <= target_bits_per_pixel) {
                std::uint32_t diff{ target_bits_per_pixel - _bits_per_pixel };

                if (diff < min_diff_bits_per_pixel) {
                    min_diff_bits_per_pixel = diff;
                    best_fitting_header = ba_header;
                    found = true;

                    if (diff == 0)
                        break;  // prunning: we've just found the perfect targetted image!
                }
            }

        }

        if (!found) {
            // no best fitting image found, so let's return the first one in list as the BMP standard asks for
            best_fitting_header = ba_headers_list[0];
        }

        return MyImageBaseClass(
            in_stream,
            best_fitting_header,
            apply_gamma_correction,
            skipped_mode,
            force_bottom_up
        );

    }


    //===========================================================================
    // BMPBestFittingResolutionImage
    //---------------------------------------------------------------------------
    template<typename PixelT>
    BMPBestFittingResolutionImage<PixelT>::BMPBestFittingResolutionImage(const bmpl::BMPImage<PixelT> image) noexcept
        : MyImageBaseClass(image)
    {}

    //---------------------------------------------------------------------------
    template<typename PixelT>
    BMPBestFittingResolutionImage<PixelT>::BMPBestFittingResolutionImage(
        const std::string& filepath,
        const std::int32_t target_dpi_x_resolution,
        std::int32_t target_dpi_y_resolution,
        const bool apply_gamma_correction,
        const bmpl::clr::ESkippedPixelsMode skipped_mode,
        const bool force_bottom_up
    ) noexcept
        : MyImageBaseClass()
    {
        *this = load_image(
            filepath,
            target_dpi_x_resolution,
            target_dpi_y_resolution,
            apply_gamma_correction,
            skipped_mode,
            force_bottom_up
        );
    }

    //---------------------------------------------------------------------------
    template<typename PixelT>
    BMPBestFittingResolutionImage<PixelT>::BMPBestFittingResolutionImage(
        const std::string& filepath,
        const std::int32_t target_dpi_resolution,
        const bool apply_gamma_correction,
        const bmpl::clr::ESkippedPixelsMode skipped_mode,
        const bool force_bottom_up
    ) noexcept
        : MyImageBaseClass()
    {
        *this = load_image(
            filepath,
            target_dpi_resolution,
            target_dpi_resolution,
            apply_gamma_correction,
            skipped_mode,
            force_bottom_up
        );
    }

    //---------------------------------------------------------------------------
    template<typename PixelT>
    BMPBestFittingResolutionImage<PixelT> BMPBestFittingResolutionImage<PixelT>::load_image(
        const std::string& filepath,
        const std::int32_t target_dpi_x_resolution,
        std::int32_t target_dpi_y_resolution,
        const bool apply_gamma_correction,
        const bmpl::clr::ESkippedPixelsMode skipped_mode,
        const bool force_bottom_up
    ) noexcept
    {
        if (!bmpl::frmt::BAHeader::is_BA_file(filepath)) {
            // only one image to be loaded, can't provide a better one.
            return MyImageBaseClass(filepath, apply_gamma_correction, skipped_mode, force_bottom_up);
        }

        // ok, "BA" file
        if (target_dpi_y_resolution == 0)
            target_dpi_y_resolution = target_dpi_x_resolution;

        bmpl::utils::LEInStream in_stream(filepath);
        if (in_stream.failed()) {
            // some error happenned while attempting to open the compressed image file
            return MyImageBaseClass(in_stream.get_error());
        }

        bmpl::frmt::BAHeadersList ba_headers_list{ bmpl::frmt::BAHeader::get_BA_headers(in_stream) };
        if (ba_headers_list.failed()) {
            // an error happened while reading the list of BA headers
            return MyImageBaseClass(ba_headers_list.get_error());
        }

        std::uint32_t min_diff_resolution{ 0xffff'ffff };
        bmpl::frmt::BAHeader best_fitting_header{};
        bool found{ false };

        for (bmpl::frmt::BAHeader& ba_header : ba_headers_list) {
            // let's compare resolutions
            const std::int32_t img_dpi_x_resolution{ ba_header.get_device_x_resolution_dpi() };
            const std::int32_t img_dpi_y_resolution{ ba_header.get_device_y_resolution_dpi() };
            if (img_dpi_x_resolution <= target_dpi_x_resolution && img_dpi_y_resolution <= target_dpi_y_resolution) {
                std::uint32_t diff{
                    (std::uint32_t(target_dpi_x_resolution) - img_dpi_x_resolution) +
                    (std::uint32_t(target_dpi_y_resolution) - img_dpi_y_resolution)
                };

                if (diff < min_diff_resolution) {
                    min_diff_resolution = diff;
                    best_fitting_header = ba_header;
                    found = true;

                    if (diff == 0)
                        break;  // prunning: we've just found the perfect targetted image!
                }
            }
        }

        if (!found) {
            // no best fitting image found, so let's return the first one in list as the BMP standard asks for
            best_fitting_header = ba_headers_list[0];
        }

        return MyImageBaseClass(
            in_stream,
            best_fitting_header,
            apply_gamma_correction,
            skipped_mode,
            force_bottom_up
        );

    }

    //---------------------------------------------------------------------------
    template<typename PixelT>
    BMPBestFittingResolutionImage<PixelT> BMPBestFittingResolutionImage<PixelT>::load_image(
        const std::string& filepath,
        const std::int32_t target_dpi_resolution,
        const bool apply_gamma_correction,
        const bmpl::clr::ESkippedPixelsMode skipped_mode,
        const bool force_bottom_up
    ) noexcept
    {
        return load_image(
            filepath,
            target_dpi_resolution,
            target_dpi_resolution,
            apply_gamma_correction,
            skipped_mode,
            force_bottom_up
        );
    }


    //===========================================================================
    // BMPBestFittingSizeImage
    //---------------------------------------------------------------------------
    template<typename PixelT>
    BMPBestFittingSizeImage<PixelT>::BMPBestFittingSizeImage(const bmpl::BMPImage<PixelT> image) noexcept
        : MyImageBaseClass(image)
    {}

    //---------------------------------------------------------------------------
    template<typename PixelT>
    BMPBestFittingSizeImage<PixelT>::BMPBestFittingSizeImage(
        const std::string& filepath,
        const std::uint32_t target_width,
        const std::uint32_t target_height,
        const bool apply_gamma_correction,
        const bmpl::clr::ESkippedPixelsMode skipped_mode,
        const bool force_bottom_up
    ) noexcept
        : MyImageBaseClass()
    {
        *this = load_image(
            filepath,
            target_width,
            target_height,
            apply_gamma_correction,
            skipped_mode,
            force_bottom_up
        );
    }

    //---------------------------------------------------------------------------
    template<typename PixelT>
    BMPBestFittingSizeImage<PixelT> BMPBestFittingSizeImage<PixelT>::load_image(
        const std::string& filepath,
        const std::uint32_t target_width,
        const std::uint32_t target_height,
        const bool apply_gamma_correction,
        const bmpl::clr::ESkippedPixelsMode skipped_mode,
        const bool force_bottom_up
    ) noexcept
    {
        if (!bmpl::frmt::BAHeader::is_BA_file(filepath)) {
            // only one image to be loaded, can't provide a better one.
            return MyImageBaseClass(filepath, apply_gamma_correction, skipped_mode, force_bottom_up);
        }

        // ok, "BA" file
        bmpl::utils::LEInStream in_stream(filepath);
        if (in_stream.failed()) {
            // some error happenned while attempting to open the compressed image file
            return MyImageBaseClass(in_stream.get_error());
        }

        bmpl::frmt::BAHeadersList ba_headers_list{ bmpl::frmt::BAHeader::get_BA_headers(in_stream) };
        if (ba_headers_list.failed()) {
            // an error happened while reading the list of BA headers
            return MyImageBaseClass(ba_headers_list.get_error());
        }

        std::uint32_t min_diff_dims{ 0xffff'ffff };
        bmpl::frmt::BAHeader best_fitting_header{};
        bool found{ false };

        for (bmpl::frmt::BAHeader& ba_header : ba_headers_list) {
            // let's compare dimensions of images
            const std::uint32_t hdr_width{ ba_header.get_width() };
            const std::uint32_t hdr_height{ ba_header.get_height() };

            if (hdr_width <= target_width && hdr_height <= target_height) {
                std::uint32_t diff{ (target_width - hdr_width) + (target_height - hdr_height) };

                if (diff < min_diff_dims) {
                    min_diff_dims = diff;
                    best_fitting_header = ba_header;
                    found = true;

                    if (diff == 0)
                        break;  // prunning: we've just found the perfect targetted image!
                }
            }
        }

        if (!found) {
            // no best fitting image found, so let's return the first one in list as the BMP standard asks for
            best_fitting_header = ba_headers_list[0];
        }

        return MyImageBaseClass(
            in_stream,
            best_fitting_header,
            apply_gamma_correction,
            skipped_mode,
            force_bottom_up
        );

    }


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

}