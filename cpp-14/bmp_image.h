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
    class BMPImage : public bmpl::utils::ErrorStatus, public bmpl::utils::WarningStatus
    {
    public:
        using MyErrBaseClass = bmpl::utils::ErrorStatus;
        using MyWarnBaseClass = bmpl::utils::WarningStatus;

        using pixel_type = PixelT;


        const std::uint32_t width{ 0 };
        const std::uint32_t height{ 0 };
        const std::uint32_t resolution_x{ 0 };
        const std::uint32_t resolution_y{ 0 };
        const std::vector<pixel_type> image_content{};
        const std::string filepath{};


        BMPImage() noexcept = default;
        BMPImage(const BMPImage&) noexcept = default;
        BMPImage(BMPImage&&) noexcept = default;

        virtual inline ~BMPImage() noexcept = default;

        BMPImage& operator= (const BMPImage&) noexcept = default;
        BMPImage& operator= (BMPImage&&) noexcept = default;


        inline BMPImage(
            const std::string& filepath_,
            const std::int32_t width_,
            const std::int32_t height_,
            std::vector<pixel_type> image_content_,
            const bmpl::utils::ErrorStatus& error_status_,
            const bmpl::utils::WarningStatus& warning_status_
        ) noexcept;


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
        inline pixel_type* image_content_ptr() noexcept
        {
            return image_content.data();
        }

        [[nodiscard]]
        inline const std::uint32_t image_size() const noexcept
        {
            return this->height * this->width;
        }

    };


    //===========================================================================
    // Template specializations
    //
    using RGBBMPImage = BMPImage<bmpl::clr::RGB>;
    using RGBABMPImage = BMPImage<bmpl::clr::RGB>;

    using BGRBMPImage = BMPImage<bmpl::clr::BGR>;
    using BGRABMPImage = BMPImage<bmpl::clr::BGRA>;



    //===========================================================================
    // Local implmentations

    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline BMPImage<PixelT>::BMPImage(
        const std::string& filepath_,
        const std::int32_t width_,
        const std::int32_t height_,
        std::vector<PixelT> image_content_,
        const bmpl::utils::ErrorStatus& error_status_,
        const bmpl::utils::WarningStatus& warning_status_
    ) noexcept
        : MyErrBaseClass(error_status_)
        , MyWarnBaseClass(warning_status_)
        , filepath(filepath)
        , width(width_)
        , height(height_)
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

}