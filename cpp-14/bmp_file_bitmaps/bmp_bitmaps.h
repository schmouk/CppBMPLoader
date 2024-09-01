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


#include <cstdint>
#include <vector>

#include "bitfield_mask.h"
#include "../bmp_file_format/bmp_colormap.h"
#include "../bmp_file_format/bmp_info_header.h"
#include "../bmp_file_format/bmp_file_header.h"
#include "../utils/errors.h"
#include "../utils/little_endian_streaming.h"
#include "../utils/warnings.h"


namespace bmpl
{
    namespace bmpf
    {
        //===========================================================================
        template<typename PixelT>
        class BitmapLoaderBase : public bmpl::utils::ErrorStatus, public bmpl::utils::WarningStatus
        {
        public:

            bmpl::utils::LEInStream& in_stream;
            bmpl::frmt::BMPFileHeader file_header;
            const bmpl::frmt::BMPInfoHeaderBase* info_header_ptr;
            bmpl::frmt::BMPColorMap color_map;
            const std::int32_t image_width;
            const std::int32_t image_height;


            using MyErrBaseClass = bmpl::utils::ErrorStatus;
            using MyWarnBaseClass = bmpl::utils::WarningStatus;


            inline BitmapLoaderBase() noexcept = default;
            inline virtual ~BitmapLoaderBase() noexcept = default;


            inline BitmapLoaderBase(
                bmpl::utils::LEInStream& in_stream_,
                bmpl::frmt::BMPFileHeader& file_header_,
                const bmpl::frmt::BMPInfoHeaderBase* info_header_ptr_,
                bmpl::frmt::BMPColorMap& color_map_,
                const std::int32_t image_width_,
                const std::int32_t image_height_
            )
                : MyErrBaseClass()
                , MyWarnBaseClass()
                , in_stream(in_stream_)
                , file_header(file_header_)
                , info_header_ptr(info_header_ptr_)
                , color_map(color_map_)
                , image_width(image_width_)
                , image_height(image_height_)
            {}


            inline virtual const bool load(std::vector<PixelT>& image_content) noexcept
            {
                return _set_err(bmpl::utils::ErrorCode::NOT_YET_IMPLEMENTED_BMP_FORMAT);
            }


        protected:
            static inline std::size_t _evaluate_padding(const std::size_t line_width) noexcept
            {
                const std::size_t mod4{ line_width % 4 };
                return (mod4 > 0) ? (4 - mod4) : 0;
            }
        };


        //===========================================================================
        template<typename PixelT>
        class BitmapLoader1bit : public BitmapLoaderBase<PixelT>
        {
        public:

            using MyBaseClass = BitmapLoaderBase<PixelT>;

            inline BitmapLoader1bit(
                bmpl::utils::LEInStream& in_stream,
                bmpl::frmt::BMPFileHeader& file_header,
                const bmpl::frmt::BMPInfoHeaderBase* info_header_ptr,
                bmpl::frmt::BMPColorMap& color_map,
                const std::int32_t image_width,
                const std::int32_t image_height
            ) noexcept
                : MyBaseClass(in_stream, file_header, info_header_ptr, color_map, image_width, image_height)
            {}

            virtual const bool load(std::vector<PixelT>& image_content) noexcept override;

        };


        //===========================================================================
        template<typename PixelT>
        class BitmapLoader2bits : public BitmapLoaderBase<PixelT>
        {
        public:

            using MyBaseClass = BitmapLoaderBase<PixelT>;

            inline BitmapLoader2bits(
                bmpl::utils::LEInStream& in_stream,
                bmpl::frmt::BMPFileHeader& file_header,
                const bmpl::frmt::BMPInfoHeaderBase* info_header_ptr,
                bmpl::frmt::BMPColorMap& color_map,
                const std::int32_t image_width,
                const std::int32_t image_height
            ) noexcept
                : MyBaseClass(in_stream, file_header, info_header_ptr, color_map, image_width, image_height)
            {}

            virtual const bool load(std::vector<PixelT>& image_content) noexcept override;

        };


        //===========================================================================
        template<typename PixelT>
        class BitmapLoader4bits : public BitmapLoaderBase<PixelT>
        {
        public:

            using MyBaseClass = BitmapLoaderBase<PixelT>;

            inline BitmapLoader4bits(
                bmpl::utils::LEInStream& in_stream,
                bmpl::frmt::BMPFileHeader& file_header,
                const bmpl::frmt::BMPInfoHeaderBase* info_header_ptr,
                bmpl::frmt::BMPColorMap& color_map,
                const std::int32_t image_width,
                const std::int32_t image_height
            ) noexcept
                : MyBaseClass(in_stream, file_header, info_header_ptr, color_map, image_width, image_height)
            {}

            virtual const bool load(std::vector<PixelT>& image_content) noexcept override;

        };


        //===========================================================================
        template<typename PixelT>
        class BitmapLoader4bitsRLE : public BitmapLoaderBase<PixelT>
        {
        public:

            using MyBaseClass = BitmapLoaderBase<PixelT>;

            inline BitmapLoader4bitsRLE(
                bmpl::utils::LEInStream& in_stream,
                bmpl::frmt::BMPFileHeader& file_header,
                const bmpl::frmt::BMPInfoHeaderBase* info_header_ptr,
                bmpl::frmt::BMPColorMap& color_map,
                const std::int32_t image_width,
                const std::int32_t image_height
            ) noexcept
                : MyBaseClass(in_stream, file_header, info_header_ptr, color_map, image_width, image_height)
            {}

            virtual const bool load(std::vector<PixelT>& image_content) noexcept override;

        };


        //===========================================================================
        template<typename PixelT>
        class BitmapLoader8bits : public BitmapLoaderBase<PixelT>
        {
        public:

            using MyBaseClass = BitmapLoaderBase<PixelT>;

            inline BitmapLoader8bits(
                bmpl::utils::LEInStream& in_stream,
                bmpl::frmt::BMPFileHeader& file_header,
                const bmpl::frmt::BMPInfoHeaderBase* info_header_ptr,
                bmpl::frmt::BMPColorMap& color_map,
                const std::int32_t image_width,
                const std::int32_t image_height
            ) noexcept
                : MyBaseClass(in_stream, file_header, info_header_ptr, color_map, image_width, image_height)
            {}

            virtual const bool load(std::vector<PixelT>& image_content) noexcept override;

        };


        //===========================================================================
        template<typename PixelT>
        class BitmapLoader8bitsRLE : public BitmapLoaderBase<PixelT>
        {
        public:

            using MyBaseClass = BitmapLoaderBase<PixelT>;

            inline BitmapLoader8bitsRLE(
                bmpl::utils::LEInStream& in_stream,
                bmpl::frmt::BMPFileHeader& file_header,
                const bmpl::frmt::BMPInfoHeaderBase* info_header_ptr,
                bmpl::frmt::BMPColorMap& color_map,
                const std::int32_t image_width,
                const std::int32_t image_height
            ) noexcept
                : MyBaseClass(in_stream, file_header, info_header_ptr, color_map, image_width, image_height)
            {}

            virtual const bool load(std::vector<PixelT>& image_content) noexcept override;

        };


        //===========================================================================
        template<typename PixelT>
        class BitmapLoader16bits : public BitmapLoaderBase<PixelT>
        {
        public:

            using MyBaseClass = BitmapLoaderBase<PixelT>;

            inline BitmapLoader16bits(
                bmpl::utils::LEInStream& in_stream,
                bmpl::frmt::BMPFileHeader& file_header,
                const bmpl::frmt::BMPInfoHeaderBase* info_header_ptr,
                bmpl::frmt::BMPColorMap& color_map,
                const std::int32_t image_width,
                const std::int32_t image_height
            ) noexcept
                : MyBaseClass(in_stream, file_header, info_header_ptr, color_map, image_width, image_height)
            {}

            virtual const bool load(std::vector<PixelT>& image_content) noexcept override;

        };


        //===========================================================================
        template<typename PixelT>
        class BitmapLoader24bits : public BitmapLoaderBase<PixelT>
        {
        public:

            using MyBaseClass = BitmapLoaderBase<PixelT>;

            inline BitmapLoader24bits(
                bmpl::utils::LEInStream& in_stream,
                bmpl::frmt::BMPFileHeader& file_header,
                const bmpl::frmt::BMPInfoHeaderBase* info_header_ptr,
                bmpl::frmt::BMPColorMap& color_map,
                const std::int32_t image_width,
                const std::int32_t image_height
            ) noexcept
                : MyBaseClass(in_stream, file_header, info_header_ptr, color_map, image_width, image_height)
            {}

            virtual const bool load(std::vector<PixelT>& image_content) noexcept override;

        };


        //===========================================================================
        template<typename PixelT>
        class BitmapLoader32bits : public BitmapLoaderBase<PixelT>
        {
        public:

            using MyBaseClass = BitmapLoaderBase<PixelT>;

            inline BitmapLoader32bits(
                bmpl::utils::LEInStream& in_stream,
                bmpl::frmt::BMPFileHeader& file_header,
                const bmpl::frmt::BMPInfoHeaderBase* info_header_ptr,
                bmpl::frmt::BMPColorMap& color_map,
                const std::int32_t image_width,
                const std::int32_t image_height
            ) noexcept
                : MyBaseClass(in_stream, file_header, info_header_ptr, color_map, image_width, image_height)
            {}

            virtual const bool load(std::vector<PixelT>& image_content) noexcept override;

        };


        //===========================================================================
        template<typename PixelT>
        class BitmapLoader64bits : public BitmapLoaderBase<PixelT>
        {
        public:

            using MyBaseClass = BitmapLoaderBase<PixelT>;

            inline BitmapLoader64bits(
                bmpl::utils::LEInStream& in_stream,
                bmpl::frmt::BMPFileHeader& file_header,
                const bmpl::frmt::BMPInfoHeaderBase* info_header_ptr,
                bmpl::frmt::BMPColorMap& color_map,
                const std::int32_t image_width,
                const std::int32_t image_height
            ) noexcept
                : MyBaseClass(in_stream, file_header, info_header_ptr, color_map, image_width, image_height)
            {}

            virtual const bool load(std::vector<PixelT>& image_content) noexcept override;

        };


        //===========================================================================
        template<typename PixelT>
        BitmapLoaderBase<PixelT>* create_bitmap_loader(
            bmpl::utils::LEInStream& in_stream,
            bmpl::frmt::BMPFileHeader& file_header,
            const bmpl::frmt::BMPInfoHeaderBase* info_header_ptr,
            bmpl::frmt::BMPColorMap& color_map,
            const std::int32_t image_width,
            const std::int32_t image_height
        ) noexcept
        {
            if (info_header_ptr == nullptr) {
                return nullptr;
            }

            switch (info_header_ptr->bits_per_pixel)
            {
            case 1:
                return new BitmapLoader1bit<PixelT>(in_stream, file_header, info_header_ptr, color_map, image_width, image_height);

            case 2:
                return new BitmapLoader2bits<PixelT>(in_stream, file_header, info_header_ptr, color_map, image_width, image_height);

            case 4:
                if (info_header_ptr->compression_mode == info_header_ptr->COMPR_NO_RLE)
                    return new BitmapLoader4bits<PixelT>(in_stream, file_header, info_header_ptr, color_map, image_width, image_height);
                else
                    return new BitmapLoader4bitsRLE<PixelT>(in_stream, file_header, info_header_ptr, color_map, image_width, image_height);

            case  8:
                if (info_header_ptr->compression_mode == info_header_ptr->COMPR_NO_RLE)
                    return new BitmapLoader8bits<PixelT>(in_stream, file_header, info_header_ptr, color_map, image_width, image_height);
                else
                    return new BitmapLoader8bitsRLE<PixelT>(in_stream, file_header, info_header_ptr, color_map, image_width, image_height);

            case 16:
                return new BitmapLoader16bits<PixelT>(in_stream, file_header, info_header_ptr, color_map, image_width, image_height);

            case 24:
                return new BitmapLoader24bits<PixelT>(in_stream, file_header, info_header_ptr, color_map, image_width, image_height);

            case 32:
                return new BitmapLoader32bits<PixelT>(in_stream, file_header, info_header_ptr, color_map, image_width, image_height);

            case 64:
                return new BitmapLoader64bits<PixelT>(in_stream, file_header, info_header_ptr, color_map, image_width, image_height);

            default:
                return nullptr;
            }
        }



        //===========================================================================
        template<typename PixelT>
        const bool BitmapLoader1bit<PixelT>::load(std::vector<PixelT>& image_content) noexcept
        {
            if (this->info_header_ptr == nullptr)
                return this->_set_err(bmpl::utils::ErrorCode::BAD_INFO_HEADER);

            if (this->info_header_ptr->compression_mode != this->info_header_ptr->COMPR_NO_RLE) {
                // No Run Length Encoding is defined by Windows for 2 colors mapped bitmaps
                return this->_set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
            }

            const std::size_t width{ std::size_t(this->image_width) };
            const std::size_t index_width{ std::size_t(std::ceil(width / 8.0f)) };
            const std::size_t index_height{ std::size_t(this->image_height) };

            std::vector<std::uint8_t> indexed_content;
            indexed_content.assign(index_width * index_height, 0);

            // loads the indexed content
            if (index_width % 4 == 0) {
                // cool, no padding at end of each line
                // let's load the whole image content at once
                if (!(this->in_stream.read(reinterpret_cast<char*>(indexed_content.data()), index_width * index_height)))
                    if (this->in_stream.eof())
                        return this->_set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                    else
                        return this->_set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
            }
            else {
                // let's load the image content line after line
                const std::size_t padding_size{ this->_evaluate_padding(width) };

                char* current_line_ptr{ reinterpret_cast<char*>(indexed_content.data()) };
                for (int line = 0; line < index_height; ++line) {
                    if (!(this->in_stream.read(current_line_ptr, index_width))) {
                        if (this->in_stream.eof())
                            return this->_set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                        else
                            return this->_set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                    }

                    if (padding_size > 0 && !(this->in_stream.seekg(padding_size, std::ios_base::cur))) {
                        return this->_set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                    }

                    current_line_ptr += index_width;
                }
            }

            // evaluates the final image content
            std::size_t line_remaining_pixels{ width };

            auto img_it{ image_content.begin() };
            auto ndx_it = indexed_content.cbegin();
            while (ndx_it != indexed_content.cend()) {
                for (std::uint8_t mask = 0x80; line_remaining_pixels > 0 && mask > 0 && img_it != image_content.end(); mask >>= 1) {
                    bmpl::clr::convert(*img_it++, this->color_map[(*ndx_it & mask) != 0]);  // notice: "!= 0" optimization to avoid bits shifting
                    --line_remaining_pixels;
                }
                ++ndx_it;
                if (line_remaining_pixels == 0)
                    line_remaining_pixels = width;
            }

            if (img_it != image_content.end()) {
                this->set_warning(bmpl::utils::WarningCode::TOO_MANY_INDICES_IN_BITMAP);
            }
            else if (ndx_it != indexed_content.cend()) {
                this->set_warning(bmpl::utils::WarningCode::NOT_ENOUGH_INDICES_IN_BITMAP);
            }

            // once here, everything was fine!
            return this->_clr_err();
        }



        //===========================================================================
        template<typename PixelT>
        const bool BitmapLoader2bits<PixelT>::load(std::vector<PixelT>& image_content) noexcept
        {
            // Notice: this is a specific case with Windows CE
            if (this->info_header_ptr == nullptr)
                return this->_set_err(bmpl::utils::ErrorCode::BAD_INFO_HEADER);

            if (this->info_header_ptr->compression_mode != this->info_header_ptr->COMPR_NO_RLE) {
                // No Run Length Encoding is defined by Windows CE
                return this->_set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
            }

            const std::size_t width{ std::size_t(this->image_width) };
            const std::size_t index_width{ std::size_t(std::ceil(width / 4.0f)) };
            const std::size_t index_height{ std::size_t(this->image_height) };

            std::vector<std::uint8_t> indexed_content;
            indexed_content.assign(index_width * index_height, 0);

            // loads the indexed content
            if (index_width % 4 == 0) {
                // cool, no padding at end of each line
                // let's load the whole image content at once
                if (!(this->in_stream.read(reinterpret_cast<char*>(indexed_content.data()), index_width * index_height)))
                    if (this->in_stream.eof())
                        return this->_set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                    else
                        return this->_set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
            }
            else {
                // let's load the image content line after line
                const std::size_t padding_size{ this->_evaluate_padding(width) };

                char* current_line_ptr{ reinterpret_cast<char*>(indexed_content.data()) };
                for (int line = 0; line < index_height; ++line) {
                    if (!(this->in_stream.read(current_line_ptr, index_width))) {
                        if (this->in_stream.eof())
                            return this->_set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                        else
                            return this->_set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                    }

                    if (padding_size > 0 && !(this->in_stream.seekg(padding_size, std::ios_base::cur))) {
                        return this->_set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                    }

                    current_line_ptr += index_width;
                }
            }

            // evaluates the final image content
            std::size_t line_remaining_pixels{ width };

            auto img_it{ image_content.begin() };
            auto ndx_it = indexed_content.cbegin();
            while (ndx_it != indexed_content.cend()) {
                for (std::uint8_t mask = 0xc0, shift = 6; line_remaining_pixels > 0 && mask > 0 && img_it != image_content.end(); mask >>= 2, shift -= 2) {
                    bmpl::clr::convert(*img_it++, this->color_map[(*ndx_it & mask) >> shift]);
                    --line_remaining_pixels;
                }
                ++ndx_it;
                if (line_remaining_pixels == 0)
                    line_remaining_pixels = width;
            }

            if (img_it != image_content.end()) {
                this->set_warning(bmpl::utils::WarningCode::TOO_MANY_INDICES_IN_BITMAP);
            }
            else if (ndx_it != indexed_content.cend()) {
                this->set_warning(bmpl::utils::WarningCode::NOT_ENOUGH_INDICES_IN_BITMAP);
            }

            // once here, everything was fine!
            return this->_clr_err();
        }



        //===========================================================================
        template<typename PixelT>
        const bool BitmapLoader4bits<PixelT>::load(std::vector<PixelT>& image_content) noexcept
        {
            const std::size_t width{ std::size_t(this->image_width) };
            const std::size_t index_width{ std::size_t(std::ceil(width / 2.0f)) };
            const std::size_t index_height{ std::size_t(this->image_height) };

            std::vector<std::uint8_t> indexed_content;
            indexed_content.assign(index_width * index_height, 0);

            // loads the indexed content
            if (index_width % 4 == 0) {
                // cool, no padding at end of each line
                // let's load the whole bitmap content at once
                if (!(this->in_stream.read(reinterpret_cast<char*>(indexed_content.data()), index_width * index_height))) {
                    if (this->in_stream.eof())
                        return this->_set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                    else
                        return this->_set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                }
            }
            else {
                // let's load the bitmap content line after line
                const std::size_t padding_size{ this->_evaluate_padding(width) };

                char* current_line_ptr{ reinterpret_cast<char*>(indexed_content.data()) };
                for (int line = 0; line < index_height; ++line) {
                    if (!(this->in_stream.read(current_line_ptr, index_width))) {
                        if (this->in_stream.eof())
                            return this->_set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                        else
                            return this->_set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                    }

                    if (padding_size > 0 && !(this->in_stream.seekg(padding_size, std::ios_base::cur))) {
                        return  this->_set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                    }

                    current_line_ptr += index_width;
                }
            }

            // evaluates the final image content
            std::size_t line_remaining_pixels{ width };

            auto img_it{ image_content.begin() };
            auto ndx_it{ indexed_content.cbegin() };
            while (ndx_it != indexed_content.cend() && img_it != image_content.end())
            {
                bmpl::clr::convert(*img_it++, this->color_map[*ndx_it >> 4]);
                if (--line_remaining_pixels > 0 && img_it != image_content.end()) {
                    bmpl::clr::convert(*img_it++, this->color_map[*ndx_it & 0x0f]);
                    --line_remaining_pixels;
                }
                if (line_remaining_pixels == 0)
                    line_remaining_pixels = width;
                ++ndx_it;
            }

            if (img_it != image_content.end()) {
                this->set_warning(bmpl::utils::WarningCode::TOO_MANY_INDICES_IN_BITMAP);
            }
            else if (ndx_it != indexed_content.cend()) {
                this->set_warning(bmpl::utils::WarningCode::NOT_ENOUGH_INDICES_IN_BITMAP);
            }

            this->append_warnings(this->color_map);

            // once here, everything was fine!
            return this->_clr_err();
        }


        //===========================================================================
        template<typename PixelT>
        const bool BitmapLoader4bitsRLE<PixelT>::load(std::vector<PixelT>& image_content) noexcept
        {
            if (this->info_header_ptr == nullptr)
                return this->_set_err(bmpl::utils::ErrorCode::BAD_INFO_HEADER);

            if (this->info_header_ptr->compression_mode != this->info_header_ptr->COMPR_RLE_4) {
                return this->_set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
            }

            // loads the RLE-4 bitmap
            const std::uint32_t bitmap_size{ this->file_header.size - this->file_header.content_offset };
            std::vector<std::uint8_t> bitmap;
            bitmap.assign(bitmap_size, std::uint8_t(0));
            if (!(this->in_stream.read(reinterpret_cast<char*>(bitmap.data()), bitmap_size))) {
                if (this->in_stream.eof())
                    return this->_set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                else
                    return this->_set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
            }

            // parses then the RLE-4 bitmap
            std::uint32_t width{ std::uint32_t(this->image_width) };
            std::uint32_t num_line{ 0 };
            std::uint32_t x{ 0 };
            bool encountered_eof{ false };

            auto img_it{ image_content.begin() };
            auto bmp_it = bitmap.cbegin();

            while (bmp_it != bitmap.cend()) {
                if (*bmp_it > 0) {
                    // encoded mode, repetition of same pixel value n-times
                    std::uint8_t n_rep{ *bmp_it++ };
                    PixelT pxl_value_0, pxl_value_1;
                    bmpl::clr::convert(pxl_value_0, this->color_map[*bmp_it >> 4]);
                    bmpl::clr::convert(pxl_value_1, this->color_map[*bmp_it & 0x0f]);
                    if (bmp_it != bitmap.cend())
                        bmp_it++;

                    if (x + std::uint32_t(n_rep) > width) {
                        this->set_warning(bmpl::utils::WarningCode::DELTA_MODE_MAY_OVERFLOW);
                        num_line += (x + n_rep) / width;
                        x = (x + n_rep) % width;
                    }
                    else
                        x += n_rep;

                    while (n_rep--) {
                        if (img_it == image_content.end()) {
                            return this->_set_err(bmpl::utils::ErrorCode::BUFFER_OVERFLOW);
                        }
                        *img_it++ = pxl_value_0;
                        if (n_rep) {
                            if (img_it == image_content.end()) {
                                return this->_set_err(bmpl::utils::ErrorCode::BUFFER_OVERFLOW);
                            }
                            *img_it++ = pxl_value_1;
                            --n_rep;
                        }
                        else
                            break;  // shortcut, don't test twice n_rep to zero!
                    }
                }
                else if (bmp_it != bitmap.cend() - 1) {
                    bmp_it++;
                    switch (*bmp_it++)
                    {
                    case 0:
                        // end of line
                        ++num_line;
                        x = 0;
                        if (num_line == this->image_height) {
                            return this->_set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
                        }
                        img_it = image_content.begin() + std::size_t(num_line * width);
                        break;

                    case 1:
                        // end of bitmap
                        encountered_eof = true;
                        if (bmp_it != bitmap.cend()) {
                            return this->_set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
                        }
                        break;

                    case 2:
                    {
                        // delta-mode
                        std::uint8_t delta_pxls{ 0 };
                        std::uint8_t delta_lines{ 0 };
                        if (bmp_it != bitmap.cend())
                            delta_pxls = *bmp_it++;
                        if (bmp_it != bitmap.cend())
                            delta_lines = *bmp_it++;
                        
                        if (x + delta_pxls > width) {
                            this->set_warning(bmpl::utils::WarningCode::DELTA_MODE_MAY_OVERFLOW);
                            num_line += (x + delta_pxls) / width;
                            x = (x + delta_pxls) % width;
                        }
                        else
                            x += delta_pxls;

                        const std::size_t offset{ std::size_t(delta_pxls) + std::size_t(width) * std::size_t(delta_lines) };
                        if ((img_it - image_content.begin()) + offset > std::size_t(width) * this->image_height) {
                            return this->_set_err(bmpl::utils::ErrorCode::INCOHERENT_DELTA_MODE_VALUES);
                        }
                        img_it += offset;
                    }
                    break;

                    default:
                        // absolute mode
                        std::uint8_t absolute_pixels_count{ *(bmp_it - 1) };
                        const int mod4{ absolute_pixels_count % 4 };
                        const bool padding{ mod4 == 1 || mod4 == 2 };
                        PixelT pxl_value;

                        if (x + std::uint32_t(absolute_pixels_count) > width) {
                            this->set_warning(bmpl::utils::WarningCode::DELTA_MODE_MAY_OVERFLOW);
                            num_line += (x + absolute_pixels_count) / width;
                            x = (x + absolute_pixels_count) % width;
                        }
                        else
                            x += absolute_pixels_count;

                        while (absolute_pixels_count--) {
                            bmpl::clr::convert(pxl_value, this->color_map[*bmp_it >> 4]);
                            if (img_it == image_content.end()) {
                                return this->_set_err(bmpl::utils::ErrorCode::BUFFER_OVERFLOW);
                            }
                            *img_it++ = pxl_value;
                            if (absolute_pixels_count) {
                                bmpl::clr::convert(pxl_value, this->color_map[*bmp_it & 0x0f]);
                                if (img_it == image_content.end()) {
                                    return this->_set_err(bmpl::utils::ErrorCode::BUFFER_OVERFLOW);
                                }
                                *img_it++ = pxl_value;
                                --absolute_pixels_count;
                            }
                            if (bmp_it != bitmap.cend())
                                bmp_it++;
                        }

                        if (padding && bmp_it != bitmap.cend())
                            bmp_it++;

                        break;
                    }
                }
            }

            if (!encountered_eof) {
                return this->_set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
            }

            this->append_warnings(this->color_map);

            // once here, everything was fine!
            return this->_clr_err();
        }


        //===========================================================================
        template<typename PixelT>
        const bool BitmapLoader8bits<PixelT>::load(std::vector<PixelT>& image_content) noexcept
        {
            const std::size_t width{ std::size_t(this->image_width) };
            const std::size_t height{ std::size_t(this->image_height) };

            //-- no Run Length Encoding --//
            std::vector<std::uint8_t> indexed_content;
            indexed_content.assign(width * height, 0);

            // loads the indexed content
            if (width % 4 == 0) {
                // cool, no padding at end of each line
                // let's load the whole image content at once
                if (!(this->in_stream.read(reinterpret_cast<char*>(indexed_content.data()), width * height))) {
                    if (this->in_stream.eof())
                        return this->_set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                    else
                        return this->_set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                }
            }
            else {
                // let's load the image content line per line
                const std::size_t padding_size{ this->_evaluate_padding(width) };

                char* current_line_ptr{ reinterpret_cast<char*>(indexed_content.data()) };
                for (int line = 0; line < height; ++line) {
                    if (!(this->in_stream.read(current_line_ptr, width))) {
                        if (this->in_stream.eof())
                            return this->_set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                        else
                            return this->_set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                    }

                    if (padding_size > 0 && !(this->in_stream.seekg(padding_size, std::ios_base::cur))) {
                        return this->_set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                    }

                    current_line_ptr += width;
                }
            }

            // evaluates the final image content
            auto img_it{ image_content.begin() };
            auto ndx_it{ indexed_content.cbegin() };
            while (ndx_it != indexed_content.cend() && img_it != image_content.end())
                bmpl::clr::convert(*img_it++, this->color_map[*ndx_it++]);

            if (img_it != image_content.end()) {
                this->set_warning(bmpl::utils::WarningCode::NOT_ENOUGH_INDICES_IN_BITMAP);
            }
            else if (ndx_it != indexed_content.cend()) {
                this->set_warning(bmpl::utils::WarningCode::TOO_MANY_INDICES_IN_BITMAP);
            }

            this->append_warnings(this->color_map);

            // once here, everything was fine!
            return this->_clr_err();
        }


        //===========================================================================
        template<typename PixelT>
        const bool BitmapLoader8bitsRLE<PixelT>::load(std::vector<PixelT>& image_content) noexcept
        {
            if (this->info_header_ptr == nullptr)
                return this->_set_err(bmpl::utils::ErrorCode::BAD_INFO_HEADER);

            if (this->info_header_ptr->compression_mode != this->info_header_ptr->COMPR_RLE_8) {
                return this->_set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
            }

            // loads the RLE-8 bitmap
            const std::uint32_t bitmap_size{ this->file_header.size - this->file_header.content_offset };
            std::vector<std::uint8_t> bitmap;
            bitmap.assign(bitmap_size, std::uint8_t(0));
            if (!(this->in_stream.read(reinterpret_cast<char*>(bitmap.data()), bitmap_size))) {
                if (this->in_stream.eof())
                    return this->_set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                else
                    return this->_set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
            }

            // parses then the RLE-8 bitmap
            std::uint32_t width{ std::uint32_t(this->image_width) };
            std::uint32_t num_line{ 0 };
            std::uint32_t x{ 0 };
            bool encountered_eof{ false };

            auto img_it{ image_content.begin() };
            auto bmp_it{ bitmap.cbegin() };

            while (bmp_it != bitmap.cend()) {
                if (*bmp_it > 0) {
                    // encoded mode, repetition of same pixel value n-times
                    std::uint8_t n_rep{ *bmp_it++ };
                    PixelT pxl_value;
                    bmpl::clr::convert(pxl_value, this->color_map[*bmp_it]);
                    if (bmp_it != bitmap.cend())
                        bmp_it++;

                    if (x + std::uint32_t(n_rep) > width) {
                        this->set_warning(bmpl::utils::WarningCode::DELTA_MODE_MAY_OVERFLOW);
                        num_line += (x + n_rep) / width;
                        x = (x + n_rep) % width;
                    }
                    else
                        x += n_rep;

                    while (n_rep--) {
                        if (img_it == image_content.end()) {
                            return this->_set_err(bmpl::utils::ErrorCode::BUFFER_OVERFLOW);
                        }
                        *img_it++ = pxl_value;
                    }
                }
                else if (bmp_it != bitmap.cend()) {
                    bmp_it++;
                    if (bmp_it != bitmap.cend()) {
                        switch (*bmp_it++)
                        {
                        case 0:
                            // end of line
                            ++num_line;
                            x = 0;
                            if (num_line == this->image_height) {
                                return this->_set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
                            }
                            img_it = image_content.begin() + std::size_t(num_line * width);
                            break;

                        case 1:
                            // end of bitmap
                            encountered_eof = true;
                            if (bmp_it != bitmap.cend()) {
                                return this->_set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
                            }
                            break;

                        case 2:
                            // delta-mode
                        {
                            std::uint8_t delta_pxls{ 0 };
                            std::uint8_t delta_lines{ 0 };
                            if (bmp_it != bitmap.cend())
                                delta_pxls = *bmp_it++;
                            if (bmp_it != bitmap.cend())
                                delta_lines = *bmp_it++;

                            if (x + std::uint32_t(delta_pxls) > width) {
                                this->set_warning(bmpl::utils::WarningCode::DELTA_MODE_MAY_OVERFLOW);
                                num_line += (x + delta_pxls) / width;
                                x = (x + delta_pxls) % width;
                            }
                            else
                                x += delta_pxls;

                            const std::size_t offset{ std::size_t(delta_pxls) + width * std::size_t(delta_lines) };
                            if ((img_it - image_content.begin()) + offset > width * std::size_t(this->image_height)) {
                                return this->_set_err(bmpl::utils::ErrorCode::INCOHERENT_DELTA_MODE_VALUES);
                            }
                            img_it += offset;
                        }
                        break;

                        default:
                            // absolute mode
                            std::uint8_t absolute_pixels_count{ *(bmp_it - 1) };
                            const bool padding{ absolute_pixels_count % 2 != 0 };

                            if (x + std::uint32_t(absolute_pixels_count) > width) {
                                this->set_warning(bmpl::utils::WarningCode::DELTA_MODE_MAY_OVERFLOW);
                                num_line += (x + absolute_pixels_count) / width;
                                x = (x + absolute_pixels_count) % width;
                            }
                            else
                                x += absolute_pixels_count;

                            while (absolute_pixels_count-- && bmp_it != bitmap.cend()) {
                                if (img_it == image_content.end()) {
                                    return this->_set_err(bmpl::utils::ErrorCode::BUFFER_OVERFLOW);
                                }
                                bmpl::clr::convert(*img_it++, this->color_map[*bmp_it++]);
                            }
                            if (padding && bmp_it != bitmap.cend())
                                bmp_it++;
                            break;
                        }
                    }
                }
            }

            if (!encountered_eof) {
                return this->_set_err(bmpl::utils::ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING);
            }

            this->append_warnings(this->color_map);

            // once here, everything was fine!
            return this->_clr_err();
        }


        //===========================================================================
        template<typename PixelT>
        const bool BitmapLoader16bits<PixelT>::load(std::vector<PixelT>& image_content) noexcept
        {
            if (this->info_header_ptr == nullptr)
                return this->_set_err(bmpl::utils::ErrorCode::BAD_INFO_HEADER);

            const std::size_t width{ std::size_t(this->image_width) };
            const std::size_t height{ std::size_t(this->image_height) };
            const std::size_t padding{ (width % 2) != 0 ? std::size_t(1) : std::size_t(0) };
            const std::size_t line_width{ width + padding };
            const std::size_t mask_size{ line_width * height };

            const bmpl::bmpf::BitfieldMaskBase* red_mask_ptr{ bmpl::bmpf::create_bitfield_mask(this->info_header_ptr->get_red_mask()) };
            const bmpl::bmpf::BitfieldMaskBase* green_mask_ptr{ bmpl::bmpf::create_bitfield_mask(this->info_header_ptr->get_green_mask()) };
            const bmpl::bmpf::BitfieldMaskBase* blue_mask_ptr{ bmpl::bmpf::create_bitfield_mask(this->info_header_ptr->get_blue_mask()) };
            const bmpl::bmpf::BitfieldMaskBase* alpha_mask_ptr{ bmpl::bmpf::create_bitfield_mask(this->info_header_ptr->get_alpha_mask()) };

            std::vector<std::uint16_t> masked_content;
            masked_content.assign(mask_size, std::uint16_t(0));

            if (bmpl::utils::PLATFORM_IS_LITTLE_ENDIAN) {
                if (!(this->in_stream.read(reinterpret_cast<char*>(masked_content.data()), mask_size * sizeof std::uint16_t))) {
                    if (this->in_stream.eof())
                        return this->_set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                    else
                        return this->_set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                }
            }
            else {
                // big-endian underlying platform
                /*
                for (auto mask_it = masked_content.begin(); mask_it != masked_content.end(); ++mask_it) {
                    if (!(this->in_stream >> *mask_it)) {
                        return this->_set_err(this->in_stream.get_error());
                    }
                }
                */
                for (auto& mask : masked_content) {
                    if (!(this->in_stream >> mask)) {
                        return this->_set_err(this->in_stream.get_error());
                    }
                }

            }

            int line_pixels_count{ 0 };
            auto mask_it{ masked_content.cbegin() };
            //for (auto pixel_it = image_content.begin(); pixel_it != image_content.end(); ++pixel_it) {
            for (auto& pixel : image_content) {
                if (mask_it == masked_content.cend()) {
                    this->set_warning(bmpl::utils::WarningCode::NOT_ENOUGH_INDICES_IN_BITMAP);
                    break;
                }

                const std::uint32_t mask_pxl_value{ *mask_it++ };

                const std::uint32_t r{ red_mask_ptr->get_component_value(mask_pxl_value) };
                const std::uint32_t g{ green_mask_ptr->get_component_value(mask_pxl_value) };
                const std::uint32_t b{ blue_mask_ptr->get_component_value(mask_pxl_value) };
                const std::uint32_t a{ alpha_mask_ptr->get_component_value(mask_pxl_value) };

                //bmpl::clr::set_pixel(*pixel_it, r, g, b, a);
                bmpl::clr::set_pixel(pixel, r, g, b, a);

                if (++line_pixels_count == width) {
                    mask_it += padding;
                    line_pixels_count = 0;
                }
            }

            if (mask_it != masked_content.cend()) {
                this->set_warning(bmpl::utils::WarningCode::TOO_MANY_INDICES_IN_BITMAP);
            }

            // once here, everything was fine!
            return this->_clr_err();
        }


        //===========================================================================
        template<typename PixelT>
        const bool BitmapLoader24bits<PixelT>::load(std::vector<PixelT>& image_content) noexcept
        {
            const std::size_t width{ std::size_t(this->image_width) };
            const std::size_t height{ std::size_t(this->image_height) };

            // let's load the image content line after line
            const std::size_t line_width{ width * 3 };
            const std::size_t padding_size{ this->_evaluate_padding(line_width) };

            std::vector<bmpl::clr::BGR> bitmap_line;
            bitmap_line.assign(width, bmpl::clr::BGR());

            char* bitmap_line_ptr{ reinterpret_cast<char*>(bitmap_line.data()) };
            auto img_it{ image_content.begin() };
            for (int line = 0; line < height; ++line) {
                if (!(this->in_stream.read(bitmap_line_ptr, line_width))) {
                    if (this->in_stream.eof())
                        return this->_set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                    else
                        return this->_set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                }

                for (auto& bitmap_pxl : bitmap_line)
                    bmpl::clr::convert(*img_it++, bitmap_pxl);  // notice: no need of tests on image content overflow

                if (padding_size > 0 && !(this->in_stream.seekg(padding_size, std::ios_base::cur))) {
                    return this->_set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                }
            }

            // once here, everything was fine!
            return this->_clr_err();
        }


        //===========================================================================
        template<typename PixelT>
        const bool BitmapLoader32bits<PixelT>::load(std::vector<PixelT>& image_content) noexcept
        {
            if (this->info_header_ptr == nullptr)
                return this->_set_err(bmpl::utils::ErrorCode::BAD_INFO_HEADER);

                const std::size_t width{ std::size_t(this->image_width) };
            const std::size_t height{ std::size_t(this->image_height) };
            const std::size_t mask_size{ width * height };

            const bmpl::bmpf::BitfieldMaskBase* red_mask_ptr{ bmpl::bmpf::create_bitfield_mask(this->info_header_ptr->get_red_mask()) };
            const bmpl::bmpf::BitfieldMaskBase* green_mask_ptr{ bmpl::bmpf::create_bitfield_mask(this->info_header_ptr->get_green_mask()) };
            const bmpl::bmpf::BitfieldMaskBase* blue_mask_ptr{ bmpl::bmpf::create_bitfield_mask(this->info_header_ptr->get_blue_mask()) };
            const bmpl::bmpf::BitfieldMaskBase* alpha_mask_ptr{ bmpl::bmpf::create_bitfield_mask(this->info_header_ptr->get_alpha_mask()) };

            std::vector<std::uint32_t> masked_content;
            masked_content.assign(mask_size, std::uint32_t(0));

            if (bmpl::utils::PLATFORM_IS_LITTLE_ENDIAN) {
                if (!(this->in_stream.read(reinterpret_cast<char*>(masked_content.data()), mask_size * sizeof std::uint32_t))) {
                    if (this->in_stream.eof())
                        return this->_set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                    else
                        return this->_set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                }
            }
            else {
                // big-endian underlying platform
                for (auto& mask : masked_content) {
                    if (!(this->in_stream >> mask)) {
                        return this->_set_err(this->in_stream.get_error());
                    }
                }
            }

            auto mask_it{ masked_content.cbegin() };
            //for (auto pixel_it = image_content.begin(); pixel_it != image_content.end(); ++pixel_it) {
            for (auto& pixel : image_content) {
                if (mask_it == masked_content.cend()) {
                    this->set_warning(bmpl::utils::WarningCode::NOT_ENOUGH_INDICES_IN_BITMAP);
                    break;
                }

                const std::uint32_t mask_pxl_value{ *mask_it++ };

                bmpl::clr::set_pixel(
                    //*pixel_it,
                    pixel,
                    red_mask_ptr->get_component_value(mask_pxl_value),
                    green_mask_ptr->get_component_value(mask_pxl_value),
                    blue_mask_ptr->get_component_value(mask_pxl_value),
                    alpha_mask_ptr->get_component_value(mask_pxl_value)
                );
            }

            if (mask_it != masked_content.cend()) {
                this->set_warning(bmpl::utils::WarningCode::TOO_MANY_INDICES_IN_BITMAP);
            }

            // once here, everything was fine!
            return this->_clr_err();
        }


        //===========================================================================
        template<typename PixelT>
        const bool BitmapLoader64bits<PixelT>::load(std::vector<PixelT>& image_content) noexcept
        {
            const std::size_t bmp_size{ std::size_t(this->image_width) * std::size_t(this->image_height) };

            std::vector<bmpl::clr::BGRA_HDR> bitmap;
            bitmap.assign(bmp_size, bmpl::clr::BGRA_HDR());

            if (bmpl::utils::PLATFORM_IS_LITTLE_ENDIAN) {
                if (!(this->in_stream.read(reinterpret_cast<char*>(bitmap.data()), bmp_size * sizeof bmpl::clr::BGRA_HDR))) {
                    if (this->in_stream.eof())
                        return this->_set_err(bmpl::utils::ErrorCode::END_OF_FILE);
                    else
                        return this->_set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                }
            }
            else {
                for (auto bmp_it = bitmap.begin(); bmp_it != bitmap.end(); ++bmp_it) {
                    if (!(this->in_stream >> bmp_it->b)) {
                        return this->_set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                    }
                    if (!(this->in_stream >> bmp_it->g)) {
                        return this->_set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                    }
                    if (!(this->in_stream >> bmp_it->r)) {
                        return this->_set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                    }
                    if (!(this->in_stream >> bmp_it->a)) {
                        return this->_set_err(bmpl::utils::ErrorCode::INPUT_OPERATION_FAILED);
                    }
                }
            }

            auto img_it = image_content.begin();
            for (const auto& bitmap_pxl : bitmap) {
                if (img_it == image_content.end()) {
                    this->set_warning(bmpl::utils::WarningCode::TOO_MANY_INDICES_IN_BITMAP);
                    break;
                }
                bmpl::clr::convert(*img_it++, bitmap_pxl);
            }

            if (img_it != image_content.end()) {
                this->set_warning(bmpl::utils::WarningCode::NOT_ENOUGH_INDICES_IN_BITMAP);
            }

            // once here, everything was fine!
            return this->_clr_err();
        }
    }
}