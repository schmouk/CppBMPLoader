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


#include "bmp_file_format/ba_header.h"
#include "bmp_loader/bmp_loader.h"
#include "utils/list_with_status.h"


namespace bmpl
{
    //===========================================================================
    template<typename PixelT>
    class BMPImage : public bmpl::utils::ErrorStatus, public bmpl::utils::WarningStatus
    {
    public:
        using MyErrBaseClass = bmpl::utils::ErrorStatus;
        using MyWarnBaseClass = bmpl::utils::WarningStatus;
        using MyBMPLoaderBaseClass = bmpl::lodr::BMPLoaderBase<PixelT>;

        using pixel_type = PixelT;


        inline BMPImage() noexcept;

        inline BMPImage(const bmpl::utils::ErrorCode err_code) noexcept;

        inline BMPImage(
            const std::string& filepath_,
            const bool apply_gamma_correction_ = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode_ = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up_ = false
        ) noexcept;

        BMPImage(
            bmpl::utils::LEInStream& in_stream,
            bmpl::frmt::BAHeader& ba_header,
            const bool apply_gamma_correction = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up = false
        ) noexcept;

        BMPImage(const BMPImage&) noexcept = default;
        BMPImage(BMPImage&&) noexcept = default;

        virtual inline ~BMPImage() noexcept = default;


        BMPImage& operator=(const BMPImage&) noexcept = default;
        BMPImage& operator=(BMPImage&&) noexcept = default;


        [[nodiscard]]
        inline MyBMPLoaderBaseClass* get_bmp_loader_ptr() noexcept;

        [[nodiscard]]
        inline const std::uint32_t get_colors_count() const noexcept;

        [[nodiscard]]
        inline PixelT* get_content_ptr() noexcept;

        [[nodiscard]]
        inline const std::int32_t get_device_x_resolution_dpi() const noexcept;

        [[nodiscard]]
        inline const std::int32_t get_device_y_resolution_dpi() const noexcept;

        [[nodiscard]]
        inline const std::string get_error_msg() const noexcept;

        [[nodiscard]]
        inline const std::string get_filepath() const noexcept;

        [[nodiscard]]
        inline const std::uint32_t get_height() const noexcept;

        [[nodiscard]]
        const std::vector<std::string> get_warnings_msg() const noexcept;

        [[nodiscard]]
        inline const std::uint32_t get_width() const noexcept;

        [[nodiscard]]
        inline const std::uint64_t image_size() const noexcept;

        const bool load_image(
            const std::string& filepath_,
            const bool apply_gamma_correction_ = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode_ = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up_ = false
        ) noexcept;

        [[nodiscard]]
        inline const bool is_BA_file() const noexcept;

        [[nodiscard]]
        static inline const bool is_BA_file(const std::string& filepath) noexcept;

        [[nodiscard]]
        static const bool is_BA_file(bmpl::utils::LEInStream& in_stream) noexcept;


    protected:
        [[nodiscard]]
        const bool _set_bmp_loader(
            const std::string& filepath_,
            const bool apply_gamma_correction_,
            const bmpl::clr::ESkippedPixelsMode skipped_mode_,
            const bool force_bottom_up_
        ) noexcept;


    private:
        MyBMPLoaderBaseClass* _bmp_loader_ptr{ nullptr };

        static inline const std::int32_t _resolution_to_dpi(const std::int32_t resolution) noexcept;
        void _reverse_lines_ordering() noexcept;

    };


    //===========================================================================
    // Template specializations
    //
    using RGBBMPImage = BMPImage<bmpl::clr::RGB>;
    using RGBABMPImage = BMPImage<bmpl::clr::RGBA>;

    using BGRBMPImage = BMPImage<bmpl::clr::BGR>;
    using BGRABMPImage = BMPImage<bmpl::clr::BGRA>;


    //===========================================================================
    template<typename BMPImageT>
    using BMPImagesList = bmpl::utils::ListWithStatus<BMPImageT>;


    //===========================================================================
    template<typename BMPImageT>
    const BMPImagesList<BMPImageT> load_all_images(
        const std::string& filepath_,
        const bool apply_gamma_correction_ = false,
        const bmpl::clr::ESkippedPixelsMode skipped_mode_ = bmpl::clr::ESkippedPixelsMode::BLACK,
        const bool force_bottom_up_ = false
    ) noexcept;


    //===========================================================================
    class NextImageLoader : public bmpl::utils::ErrorStatus
    {
    public:
        
        using MyErrBaseClass = bmpl::utils::ErrorStatus;


        NextImageLoader() = delete;
        virtual ~NextImageLoader() = default;

        NextImageLoader(const std::string& filepath) noexcept;

        template<typename BMPImageT>
        BMPImageT load(
            const bool apply_gamma_correction = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up = false
        ) noexcept;

        const std::string get_error_msg() const noexcept;

        const std::string get_filepath() const noexcept;

        void reset() noexcept;


    private:
        bmpl::frmt::BAHeadersList _ba_headers{};
        bmpl::frmt::BAHeadersIterStatus _ba_hdr_iter{};
        std::string _filepath{};

    };



    //===========================================================================
    // Local implementations

    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline BMPImage<PixelT>::BMPImage() noexcept
        : MyErrBaseClass(bmpl::utils::ErrorCode::NOT_INITIALIZED)
        , MyWarnBaseClass()
    {}


    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline BMPImage<PixelT>::BMPImage(const bmpl::utils::ErrorCode err_code) noexcept
        : MyErrBaseClass(err_code)
        , MyWarnBaseClass()
    {}


    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline BMPImage<PixelT>::BMPImage(
        const std::string& filepath_,
        const bool apply_gamma_correction_,
        const bmpl::clr::ESkippedPixelsMode skipped_mode_,
        const bool force_bottom_up_
    ) noexcept
        : MyErrBaseClass()
        , MyWarnBaseClass()
    {
        load_image(filepath_, apply_gamma_correction_, skipped_mode_, force_bottom_up_);
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    BMPImage<PixelT>::BMPImage(
        bmpl::utils::LEInStream& in_stream,
        bmpl::frmt::BAHeader& ba_header,
        const bool apply_gamma_correction,
        const bmpl::clr::ESkippedPixelsMode skipped_mode,
        const bool force_bottom_up
    ) noexcept
    {
        this->_bmp_loader_ptr = bmpl::lodr::create_bmp_loader<PixelT>(in_stream, ba_header, apply_gamma_correction, skipped_mode, force_bottom_up);
        
        if (this->_bmp_loader_ptr == nullptr)
            _set_err(bmpl::utils::ErrorCode::BMP_LOADER_INSTANTIATION_FAILED);
        else if (this->_bmp_loader_ptr->failed())
            _set_err(this->_bmp_loader_ptr->get_error());
        else {
            if (this->_bmp_loader_ptr->load_image_content())
                _clr_err();
            else
                _set_err(this->_bmp_loader_ptr->get_error());
        }

        if (ba_header.has_warnings()) {
            this->_bmp_loader_ptr->append_warnings(ba_header);
            this->_bmp_loader_ptr->set_unique_warnings();
        }

    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline typename BMPImage<PixelT>::MyBMPLoaderBaseClass* BMPImage<PixelT>::get_bmp_loader_ptr() noexcept
    {
        return this->_bmp_loader_ptr;
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline const std::uint32_t BMPImage<PixelT>::get_colors_count() const noexcept
    {
        if (failed())
            return 0;
        else
            return this->_bmp_loader_ptr->get_colors_count();
    }

    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline PixelT* BMPImage<PixelT>::get_content_ptr() noexcept
    {
        if (failed())
            return nullptr;
        else
            return static_cast<PixelT*>(this->_bmp_loader_ptr->image_content.data());
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline const std::int32_t BMPImage<PixelT>::get_device_x_resolution_dpi() const noexcept
    {
        return _resolution_to_dpi(failed() ? 0 : this->_bmpl_loader_ptr->get_device_x_resolution());
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline const std::int32_t BMPImage<PixelT>::get_device_y_resolution_dpi() const noexcept
    {
        return _resolution_to_dpi(failed() ? 0 : this->_bmpl_loader_ptr->get_device_y_resolution());
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline const std::string BMPImage<PixelT>::get_error_msg() const noexcept
    {
        return bmpl::utils::error_msg(this->get_filepath(), get_error());
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline const std::string BMPImage<PixelT>::get_filepath() const noexcept
    {
        if (this->_bmp_loader_ptr == nullptr)
            return "";
        else
            return this->_bmp_loader_ptr->filepath;
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline const std::uint32_t BMPImage<PixelT>::get_height() const noexcept
    {
        if (failed())
            return 0;
        else
            return this->_bmp_loader_ptr->image_height;
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    const std::vector<std::string> BMPImage<PixelT>::get_warnings_msg() const noexcept
    {
        if (failed())
            return std::vector<std::string>();
        else
            return this->_bmp_loader_ptr->get_warnings_msg();
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline const std::uint32_t BMPImage<PixelT>::get_width() const noexcept
    {
        if (failed())
            return 0;
        else
            return this->_bmp_loader_ptr->image_width;
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline const std::uint64_t BMPImage<PixelT>::image_size() const noexcept
    {
        return std::uint64_t(get_height()) * std::uint64_t(get_width());
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline const bool BMPImage<PixelT>::is_BA_file() const noexcept
    {
        if (failed())
            return false;
        else
            return this->_bmp_loader_ptr->is_BA_file();
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    inline const bool BMPImage<PixelT>::is_BA_file(const std::string& filepath) noexcept
    {
        return BMPImage<PixelT>::is_BA_file(bmpl::utils::LEInStream(filepath));
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    const bool BMPImage<PixelT>::is_BA_file(bmpl::utils::LEInStream& in_stream) noexcept
    {
        if (in_stream.failed())
            return false;

        std::uint16_t file_type{ 0 };
        in_stream >> file_type;

        return in_stream.is_ok() && file_type == 0x4142;  // i.e. 'BA' as little-endian encoded
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    const bool BMPImage<PixelT>::load_image(
        const std::string& filepath_,
        const bool apply_gamma_correction_,
        const bmpl::clr::ESkippedPixelsMode skipped_mode_,
        const bool force_bottom_up_
    ) noexcept
    {
        if (!_set_bmp_loader(filepath_, apply_gamma_correction_, skipped_mode_, force_bottom_up_))
            return false;

        if (this->_bmp_loader_ptr->load_image_content())
            return _clr_err();
        else
            return _set_err(this->_bmp_loader_ptr->get_error());
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    const bool BMPImage<PixelT>::_set_bmp_loader(
        const std::string& filepath_,
        const bool apply_gamma_correction_,
        const bmpl::clr::ESkippedPixelsMode skipped_mode_,
        const bool force_bottom_up_
    ) noexcept
    {
        if (this->_bmp_loader_ptr != nullptr)
            delete this->_bmp_loader_ptr;

        this->_bmp_loader_ptr = bmpl::lodr::create_bmp_loader<PixelT>(filepath_, apply_gamma_correction_, skipped_mode_, force_bottom_up_);
        
        if (this->_bmp_loader_ptr == nullptr)
            _set_err(bmpl::utils::ErrorCode::BMP_LOADER_INSTANTIATION_FAILED);

        if (this->_bmp_loader_ptr->failed())
            return _set_err(this->_bmp_loader_ptr->get_error());

        return true;
    }


    //---------------------------------------------------------------------------
    template<typename PixelT>
    static inline const std::int32_t BMPImage<PixelT>::_resolution_to_dpi(const std::int32_t resolution) noexcept
    {
        return std::int32_t(resolution * 2.54 / 100 + 0.5);
    }


    //---------------------------------------------------------------------------
    template<typename BMPImageT>
    const BMPImagesList<BMPImageT> load_all_images(
        const std::string& filepath,
        const bool apply_gamma_correction,
        const bmpl::clr::ESkippedPixelsMode skipped_mode,
        const bool force_bottom_up
    ) noexcept
    {
        using BMPLoaderBase = typename BMPImageT::MyBMPLoaderBaseClass;

        bmpl::utils::LEInStream in_stream(filepath);
        if (!BMPImageT::is_BA_file(in_stream))
            return BMPImagesList<BMPImageT>(bmpl::utils::ErrorCode::NOT_BITMAP_ARRAY_FILE_HEADER);

        bmpl::frmt::BAHeadersList ba_headers{ BMPLoaderBase::get_BA_headers(in_stream) };

        if (ba_headers.failed())
            return BMPImagesList<BMPImageT>(ba_headers.get_error());

        BMPImagesList<BMPImageT> images_list{};

        for (auto& ba_hdr : ba_headers) {
            images_list.push_back(BMPImageT(in_stream, ba_hdr, apply_gamma_correction, skipped_mode, force_bottom_up));
        }

        return images_list;
    }


    //---------------------------------------------------------------------------
    template<typename BMPImageT>
    BMPImageT NextImageLoader::load(
        const bool apply_gamma_correction,
        const bmpl::clr::ESkippedPixelsMode skipped_mode,
        const bool force_bottom_up
    ) noexcept
    {
        using BMPLoaderBase = typename BMPImageT::MyBMPLoaderBaseClass;

        if (failed())
            return BMPImageT(get_error());

        if (this->_ba_hdr_iter.failed())
            return BMPImageT(this->_ba_hdr_iter.get_error());

        if (this->_ba_hdr_iter.end())
            return BMPImageT(bmpl::utils::ErrorCode::END_OF_BA_HEADERS_LIST);

        bmpl::frmt::BAHeader ba_header{ *(this->_ba_hdr_iter++) };

        if (ba_header.failed())
            return BMPImageT(ba_header.get_error());

        return BMPImageT(
            *(this->_ba_hdr_iter.in_stream_ptr),
            ba_header,
            apply_gamma_correction, skipped_mode, force_bottom_up);
    }

}