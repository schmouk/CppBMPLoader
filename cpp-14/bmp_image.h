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

#include "utils/colors.h"
#include "utils/errors.h"
#include "utils/warnings.h"


namespace bmpl
{   //===========================================================================
    template<typename PixelT>
    struct BMPImage : public bmpl::utils::ErrorStatus, public bmpl::utils::WarningStatus
    {
        using MyErrBaseClass = bmpl::utils::ErrorStatus;
        using MyWarnBaseClass = bmpl::utils::WarningStatus;

        using pixel_type = PixelT;


        std::vector<pixel_type> image_content{};
        std::string filepath{};
        std::uint32_t width{ 0 };
        std::uint32_t height{ 0 };
        std::uint32_t resolution_x{ 0 };
        std::uint32_t resolution_y{ 0 };
        std::uint32_t colors_count{ 0 };

        inline BMPImage(
            const std::string& filepath_,
            const bmpl::utils::ErrorStatus error_status_
        ) noexcept;


        inline BMPImage(
            const std::string& filepath_,
            const std::uint32_t width_,
            const std::uint32_t height_,
            const std::uint32_t colors_count_,
            std::vector<pixel_type> image_content_,
            const bmpl::utils::ErrorStatus& error_status_,
            const bmpl::utils::WarningStatus& warning_status_
        ) noexcept;

        [[nodiscard]]
        inline operator bool() const noexcept
        {
            return is_ok();
        }

        [[nodiscard]]
        inline const std::string get_error_msg() const noexcept
        {
            return bmpl::utils::error_msg(this->filepath, get_error());
        }

        [[nodiscard]]
        inline const std::uint32_t get_device_x_resolution_dpi() const noexcept
        {
            return std::uint32_t(this->resolution_x * 2.54 / 100 + 0.5);
        }

        [[nodiscard]]
        inline const std::uint32_t get_device_y_resolution_dpi() const noexcept
        {
            return std::uint32_t(this->resolution_y * 2.54 / 100 + 0.5);
        }

        [[nodiscard]]
        const std::vector<std::string> get_warnings_msg() const noexcept;

        [[nodiscard]]
        inline pixel_type* get_content_ptr() noexcept
        {
            return image_content.data();
        }

        [[nodiscard]]
        inline const std::uint32_t image_size() const noexcept
        {
            return this->height * this->width;
        }

        void reverse_lines_ordering() noexcept;

    };


    //===========================================================================
    // Template specializations
    //
    using RGBBMPImage = BMPImage<bmpl::clr::RGB>;
    using RGBABMPImage = BMPImage<bmpl::clr::RGBA>;

    using BGRBMPImage = BMPImage<bmpl::clr::BGR>;
    using BGRABMPImage = BMPImage<bmpl::clr::BGRA>;



    //===========================================================================
    // Local implmentations

    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline BMPImage<PixelT>::BMPImage(const std::string& filepath_, const bmpl::utils::ErrorStatus error_status_) noexcept
        : MyErrBaseClass(error_status_)
        , filepath(filepath_)
    {}

    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline BMPImage<PixelT>::BMPImage(
        const std::string& filepath_,
        const std::uint32_t width_,
        const std::uint32_t height_,
        const std::uint32_t colors_count_,
        std::vector<PixelT> image_content_,
        const bmpl::utils::ErrorStatus& error_status_,
        const bmpl::utils::WarningStatus& warning_status_
    ) noexcept
        : MyErrBaseClass(error_status_)
        , MyWarnBaseClass(warning_status_)
        , filepath(filepath_)
        , width(width_)
        , height(height_)
        , colors_count(colors_count_)
        , image_content{ std::move(image_content_) }
    {}


    //---------------------------------------------------------------------------
    template<typename PixelT>
    const std::vector<std::string> BMPImage<PixelT>::get_warnings_msg() const noexcept
    {
        std::vector<std::string> msg_res;
        for (const bmpl::utils::WarningCode warning_code : this->get_warnings())
            msg_res.push_back(bmpl::utils::warning_msg(this->filepath, warning_code));
        return msg_res;
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    void BMPImage<PixelT>::reverse_lines_ordering() noexcept
    {
        if (this->is_ok()) {
            const std::size_t line_width{ this->width * sizeof PixelT };

            std::vector<std::uint8_t> tmp_line;
            tmp_line.assign(line_width, '\0');

            std::uint8_t* upline_ptr{ reinterpret_cast<std::uint8_t*>(this->image_content.data()) };
            std::uint8_t* botline_ptr{ reinterpret_cast<std::uint8_t*>(this->image_content.data() + (this->height - 1) * this->width) };
            std::uint8_t* tmpline_ptr{ tmp_line.data() };

            for (std::size_t i = 0; i < this->height / 2; ++i) {
                std::memcpy(tmpline_ptr, upline_ptr, line_width);
                std::memcpy(upline_ptr, botline_ptr, line_width);
                std::memcpy(botline_ptr, tmpline_ptr, line_width);

                upline_ptr += line_width;
                botline_ptr -= line_width;
            }
        }
    }

}