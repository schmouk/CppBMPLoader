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


#include <cmath>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../bmp_file_format/ba_header.h"
#include "../bmp_file_bitmaps/bitfield_mask.h"
#include "../bmp_file_bitmaps/bmp_bitmaps.h"
#include "../bmp_file_format/bmp_file_header.h"
#include "../bmp_file_format/bmp_info.h"
#include "../utils/colors.h"
#include "../utils/errors.h"
#include "../utils/little_endian_streaming.h"
#include "../utils/warnings.h"


namespace bmpl
{
    namespace lodr
    {
        //===========================================================================
        // Forward declarations
        //
        template<typename PixelT> class BMPLoader;
        template<typename PixelT> class BMPBottomUpLoader;


        //===========================================================================
        // Types wrappers
        //
        template<typename PixelT>
        using BMPLoaderBase = BMPBottomUpLoader<PixelT>;

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

            std::vector<pixel_type> image_content{};
            std::string filepath{};
            std::uint32_t image_height{ 0 };
            std::uint32_t image_width{ 0 };


            BMPBottomUpLoader(
                const std::string& filepath,
                const bool apply_gamma_corection = false,
                const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK
            ) noexcept;

            BMPBottomUpLoader(
                const std::string& filepath,
                const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK
            ) noexcept;

            BMPBottomUpLoader(
                const bmpl::utils::LEInStream& in_stream,
                const bmpl::frmt::BAHeader& ba_header,
                const bool apply_gamma_correction = false,
                const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK
            ) noexcept;

            virtual inline ~BMPBottomUpLoader() noexcept = default;

            [[nodiscard]]
            bmpl::frmt::BAHeadersList get_BA_headers() noexcept;

            [[nodiscard]]
            static bmpl::frmt::BAHeadersList get_BA_headers(bmpl::utils::LEInStream& in_stream) noexcept;

            [[nodiscard]]
            inline const std::uint32_t get_colors_count() const noexcept;

            [[nodiscard]]
            inline const std::uint32_t get_content_offset() const noexcept;

            [[nodiscard]]
            inline const std::int32_t get_device_x_resolution() const noexcept;

            [[nodiscard]]
            inline const std::int32_t get_device_y_resolution() const noexcept;

            [[nodiscard]]
            inline const std::string get_error_msg() const noexcept;

            [[nodiscard]]
            inline const std::uint32_t get_file_size() const noexcept;

            [[nodiscard]]
            inline const std::uint32_t get_height() const noexcept;

            [[nodiscard]]
            inline const bool get_top_down_encoding() const noexcept;

            [[nodiscard]]
            const std::vector<std::string> get_warnings_msg() const noexcept;

            [[nodiscard]]
            inline const std::uint32_t get_width() const noexcept;

            [[nodiscard]]
            inline const bool is_BA_file() const noexcept;

            [[nodiscard]]
            virtual const bool load_image_content() noexcept;


        protected:
            bmpl::clr::ESkippedPixelsMode             _skipped_mode{ bmpl::clr::ESkippedPixelsMode::BLACK };
            bool                                      _apply_gamma_correction{ !APPLY_GAMMA_CORRECTION };
            // notice: do not modify the ordering of next four declarations since the related intializations MUST BE DONE in this order
            bmpl::utils::LEInStream                   _in_stream{};
            const bmpl::frmt::BMPFileHeaderBase*      _file_header_ptr{ nullptr };
            bmpl::frmt::BMPInfo                       _info{};
            bmpl::bmpf::BitmapLoaderBase<pixel_type>* _bitmap_loader_ptr{ nullptr };


        private:
            std::vector<bmpl::frmt::BAHeader> _ba_headers_list{};
            int _ba_current_header_index{ -1 };

            const bool _allocate_image_space(const std::size_t image_width, const std::size_t image_height) noexcept;

            inline const std::uint32_t _evaluate_colors_count(const std::uint32_t colors_count) const noexcept
            {
                return (colors_count == 0) ? 0xffff'ffff : colors_count;
            }

            const bool _load_image_content(const std::size_t content_offset, const std::uint32_t image_width, const std::uint32_t image_height) noexcept;

        };


        //===========================================================================
        template<typename PixelT>
        class BMPLoader : public BMPBottomUpLoader<PixelT>
        {
        public:
            using MyBaseClass = BMPBottomUpLoader<PixelT>;

            inline BMPLoader(
                const std::string& filepath,
                const bool apply_gamma_correction = false,
                const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK
            ) noexcept
                : MyBaseClass(filepath, apply_gamma_correction, skipped_mode)
            {}

            inline BMPLoader(
                const std::string& filepath,
                const bmpl::clr::ESkippedPixelsMode skipped_mode
            ) noexcept
                : MyBaseClass(filepath, skipped_mode)
            {}

            BMPLoader(
                const bmpl::utils::LEInStream& in_stream,
                const bmpl::frmt::BAHeader& ba_header,
                const bool apply_gamma_correction = false,
                const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK
            ) noexcept
                : MyBaseClass(in_stream, ba_header, apply_gamma_correction, skipped_mode)
            {}

            virtual inline ~BMPLoader() noexcept = default;

            [[nodiscard]]
            virtual const bool load_image_content() noexcept override;


        private:
            void _reverse_lines_ordering() noexcept;

        };


        //===========================================================================
        template<typename PixelT>
        BMPLoaderBase<PixelT>* create_bmp_loader(
            const std::string& filepath,
            const bool apply_gamma_correction = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up = false
        ) noexcept
        {
            if (force_bottom_up)
                return new bmpl::lodr::BMPBottomUpLoader<PixelT>(filepath, apply_gamma_correction, skipped_mode);
            else
                return new bmpl::lodr::BMPLoader<PixelT>(filepath, apply_gamma_correction, skipped_mode);
        }


        //===========================================================================
        template<typename PixelT>
        BMPLoaderBase<PixelT>* create_bmp_loader(
            const bmpl::utils::LEInStream& in_stream,
            const bmpl::frmt::BAHeader& ba_header,
            const bool apply_gamma_correction = false,
            const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK,
            const bool force_bottom_up = false
        ) noexcept
        {
            if (force_bottom_up)
                return new bmpl::lodr::BMPBottomUpLoader<PixelT>(in_stream, ba_header, apply_gamma_correction, skipped_mode);
            else
                return new bmpl::lodr::BMPLoader<PixelT>(in_stream, ba_header, apply_gamma_correction, skipped_mode);
        }



        //===========================================================================
        // Local implementations  -  BMPBottomUpLoader<PixelT>

        //---------------------------------------------------------------------------
        template<typename PixelT>
        BMPBottomUpLoader<PixelT>::BMPBottomUpLoader(
            const std::string& filepath,
            const bool apply_gamma_correction,
            const bmpl::clr::ESkippedPixelsMode skipped_mode
        ) noexcept
            : MyErrBaseClass()
            , MyWarnBaseClass()
            , filepath(filepath)
            , _skipped_mode(skipped_mode)
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
            const bmpl::clr::ESkippedPixelsMode skipped_mode
        ) noexcept
            : MyErrBaseClass()
            , MyWarnBaseClass()
            , filepath(filepath)
            , _skipped_mode(skipped_mode)
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
        BMPBottomUpLoader<PixelT>::BMPBottomUpLoader(
            const bmpl::utils::LEInStream& in_stream,
            const bmpl::frmt::BAHeader& ba_header,
            const bool apply_gamma_correction,
            const bmpl::clr::ESkippedPixelsMode skipped_mode
        ) noexcept
            : MyErrBaseClass()
            , MyWarnBaseClass()
            , filepath(in_stream.filepath)
            , _skipped_mode(skipped_mode)
            , _apply_gamma_correction(!APPLY_GAMMA_CORRECTION)
            , _in_stream(in_stream.filepath)
            , _file_header_ptr{ ba_header.file_header_ptr }
            , _info(ba_header.info_header_ptr, ba_header.color_map)
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
        bmpl::frmt::BAHeadersList BMPBottomUpLoader<PixelT>::get_BA_headers() noexcept
        {
            if (this->_file_header_ptr == nullptr)
                return bmpl::frmt::BAHeadersList(bmpl::utils::ErrorCode::BAD_FILE_HEADER);
            
            if (!this->_file_header_ptr->is_BA_file())
                return bmpl::frmt::BAHeadersList(bmpl::utils::ErrorCode::NOT_BITMAP_ARRAY_FILE_HEADER);

            return get_BA_headers(this->_in_stream);
        }


        //---------------------------------------------------------------------------
        template<typename PixelT>
        bmpl::frmt::BAHeadersList BMPBottomUpLoader<PixelT>::get_BA_headers(bmpl::utils::LEInStream& in_stream) noexcept
        {
            if (in_stream.failed())
                return bmpl::frmt::BAHeadersList(in_stream.get_error());

            bmpl::frmt::BAHeadersList ret_headers{};

            in_stream.seekg(0);
            do {
                std::uint16_t file_type;
                if ((in_stream >> file_type).failed()) {
                    ret_headers.set_error(in_stream.get_error());
                    break;
                }

                if (file_type != 0x4142) {
                    ret_headers.set_error(bmpl::utils::ErrorCode::NOT_BITMAP_ARRAY_FILE_HEADER);
                    break;
                }

                bmpl::frmt::BAHeader ba_header(in_stream);

                if (ba_header.failed()) {
                    ret_headers.set_error(ba_header.get_error());
                    break;
                }
                else {
                    ret_headers.push_back(ba_header);

                    try {
                        in_stream.seekg(ba_header.get_offset_to_next());
                    }
                    catch (...) {
                        ret_headers.set_error(bmpl::utils::ErrorCode::INVALID_BA_NEXT_OFFSET_VALUE);
                        return ret_headers;
                    }

                    if (in_stream.failed()) {
                        ret_headers.set_error(in_stream.get_error());
                        break;
                    }
                }

            } while (!ret_headers.back().is_last_header_in_list());

            return ret_headers;
        }


        //---------------------------------------------------------------------------
        template<typename PixelT>
        inline const std::uint32_t BMPBottomUpLoader<PixelT>::get_colors_count() const noexcept
        {
            return this->_info.color_map.colors_count;
        }


        //---------------------------------------------------------------------------
        template<typename PixelT>
        inline const std::uint32_t BMPBottomUpLoader<PixelT>::get_content_offset() const noexcept
        {
            if (this->_file_header_ptr == nullptr)
                return 0;
            else
                return std::uint32_t(this->_file_header_ptr->get_content_offset());
        }

        
        //---------------------------------------------------------------------------
        template<typename PixelT>
        inline const std::int32_t BMPBottomUpLoader<PixelT>::get_device_x_resolution() const noexcept
        {
            if (this->_info.info_header_ptr == nullptr)
                return 0;
            else
                return this->_info.info_header_ptr->get_device_x_resolution();
        }


        //---------------------------------------------------------------------------
        template<typename PixelT>
        inline const std::int32_t BMPBottomUpLoader<PixelT>::get_device_y_resolution() const noexcept
        {
            if (this->_info.info_header_ptr == nullptr)
                return 0;
            else
                return this->_info.info_header_ptr->get_device_y_resolution();
        }


        //---------------------------------------------------------------------------
        template<typename PixelT>
        inline const std::string BMPBottomUpLoader<PixelT>::get_error_msg() const noexcept
        {
            return bmpl::utils::error_msg(filepath, get_error());
        }


        //---------------------------------------------------------------------------
        template<typename PixelT>
        inline const std::uint32_t BMPBottomUpLoader<PixelT>::get_file_size() const noexcept
        {
            if (this->_file_header_ptr == nullptr)
                return 0;
            else
                return this->_file_header_ptr->file_size;
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
        inline const bool BMPBottomUpLoader<PixelT>::get_top_down_encoding() const noexcept
        {
            if (this->_info.info_header_ptr == nullptr)
                return false;
            else
                return this->_info.info_header_ptr->top_down_encoding;
        }


        //---------------------------------------------------------------------------
        template<typename PixelT>
        const std::vector<std::string> BMPBottomUpLoader<PixelT>::get_warnings_msg() const noexcept
        {
            std::vector<std::string> msg_res;
            for (const bmpl::utils::WarningCode warning_code : this->get_warnings())
                msg_res.push_back(bmpl::utils::warning_msg(this->filepath, warning_code));
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
        inline const bool BMPBottomUpLoader<PixelT>::is_BA_file() const noexcept
        {
            if (failed())
                return false;
            else
                return this->_file_header_ptr->is_BA_file();
        }


        //---------------------------------------------------------------------------
        template<typename PixelT>
        const bool BMPBottomUpLoader<PixelT>::load_image_content() noexcept
        {
            if (failed()) {
                // some former error arised
                return false;
            }

            if (is_BA_file()) {
                // this is a multi array bitmaps file, let's load the content of the very first image in array
                this->_in_stream.seekg(sizeof std::int16_t);
                bmpl::frmt::BAHeader first_ba_header(this->_in_stream);
                this->_bitmap_loader_ptr = bmpl::bmpf::create_bitmap_loader<PixelT>(
                    this->_in_stream,
                    first_ba_header.file_header_ptr,
                    first_ba_header.info_header_ptr,
                    first_ba_header.color_map
                );

                return _load_image_content(first_ba_header.get_content_offset(), first_ba_header.get_width(), first_ba_header.get_height());
            }
            else {
                // this is a BMP image file
                return _load_image_content(
                    this->_file_header_ptr->get_content_offset(),
                    this->_info.info_header_ptr->get_width(),
                    this->_info.info_header_ptr->get_height());
            }
        }


        //---------------------------------------------------------------------------
        template<typename PixelT>
        const bool BMPBottomUpLoader<PixelT>::_allocate_image_space(const std::size_t image_width, const std::size_t image_height) noexcept
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

                this->image_content.assign(image_width * image_height, pixel_default_value);
                return true;
            }
            catch (...) {
                return _set_err(bmpl::utils::ErrorCode::INCOHERENT_IMAGE_DIMENSIONS);
            }
        }


        //---------------------------------------------------------------------------
        template<typename PixelT>
        const bool BMPBottomUpLoader<PixelT>::_load_image_content(
            const std::size_t content_offset,
            const std::uint32_t image_width,
            const std::uint32_t image_height
        ) noexcept
        {
            this->image_content.clear();
            this->image_width = 0;
            this->image_height = 0;

            // loads the image bitmap
            if (this->_in_stream.seekg(content_offset).fail()) {
                _set_err(bmpl::utils::ErrorCode::ERRONEOUS_BITMAP_OFFSET);
                return false;
            }

            if (!this->_allocate_image_space(image_width, image_height))
                return false;

            if (!this->_bitmap_loader_ptr->load(this->image_content)) {
                _set_err(_bitmap_loader_ptr->get_error());
                return false;
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
                                bmpl::clr::gamma_correction(pxl, 2.2, 2.2, 2.2);  // notice: gamma value 2.2 is a widely accepted approximation.
                        }
                }
            }
            else if (this->_info.info_header_ptr->bits_per_pixel == 64) {
                // this is a specific case for which an HDR image has to be decalibrated to be shown on displays
                constexpr double gamma{ double(1.0 / 2.2) };  // notice: 2.2 is a very common value that suits most cases
                for (auto& pxl : this->image_content)
                    bmpl::clr::gamma_correction(pxl, gamma, gamma, gamma);
            }

            // once here, everything was fine
            this->image_width = image_width;
            this->image_height = image_height;
            _clr_err();

            // let's finally append any maybe warning detected during processing
            this->append_warnings(*this->_file_header_ptr);
            this->append_warnings(*this->_info.info_header_ptr);
            this->append_warnings(this->_info.color_map);
            this->append_warnings(*_bitmap_loader_ptr);

            this->set_unique_warnings();

            return true;  // this->image_content;
        }


        //===========================================================================
        // Local implementations  -  BMPLoader<PixelT>

        //---------------------------------------------------------------------------
        template<typename PixelT>
        const bool BMPLoader<PixelT>::load_image_content() noexcept
        {
            if (MyBaseClass::load_image_content()) {
                if (MyBaseClass::_info.info_header_ptr != nullptr &&
                    !MyBaseClass::_info.info_header_ptr->top_down_encoding)
                {
                    _reverse_lines_ordering();
                }
                return true;
            }
            else
                return false;
        }


        //---------------------------------------------------------------------------
        template<typename PixelT>
        void BMPLoader<PixelT>::_reverse_lines_ordering() noexcept
        {
            if (this->is_ok()) {

                const std::size_t image_width{ this->image_width };
                const std::size_t image_height{ this->image_height };
                const std::size_t line_width{ image_width * sizeof PixelT };

                std::vector<std::uint8_t> tmp_line;
                tmp_line.assign(line_width, '\0');

                std::uint8_t* upline_ptr{ reinterpret_cast<std::uint8_t*>(this->image_content.data()) };
                std::uint8_t* botline_ptr{ reinterpret_cast<std::uint8_t*>(this->image_content.data() + (image_height - 1) * image_width) };
                std::uint8_t* tmpline_ptr{ tmp_line.data() };

                for (std::size_t i = 0; i < image_height / 2; ++i) {
                    std::memcpy(tmpline_ptr, upline_ptr, line_width);
                    std::memcpy(upline_ptr, botline_ptr, line_width);
                    std::memcpy(botline_ptr, tmpline_ptr, line_width);

                    upline_ptr += line_width;
                    botline_ptr -= line_width;
                }
            }
        }
    }

}