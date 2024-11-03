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

#include "ba_header.h"


namespace bmpl
{
    namespace frmt
    {
        //===========================================================================
        //  BAHeader

        //---------------------------------------------------------------------------
        BAHeader::BAHeader(bmpl::utils::LEInStream& in_stream) noexcept
            : MyErrBaseClass()
            , MyWarnBaseClass()
            , ba_file_header{ BMPFileHeaderBA(in_stream) }
            , file_header_ptr{ create_file_header(in_stream, true) }
            , info_header_ptr{ create_bmp_info_header(in_stream, file_header_ptr) }
            , color_map(in_stream, file_header_ptr, info_header_ptr)
        {
            if (in_stream.failed())
                _set_err(in_stream.get_error());
            else if (ba_file_header.failed())
                _set_err(ba_file_header.get_error());
            else if (file_header_ptr == nullptr)
                _set_err(bmpl::utils::ErrorCode::BAD_BITS_PER_PIXEL_VALUE);
            else if (file_header_ptr->failed())
                _set_err(file_header_ptr->get_error());
            else if (info_header_ptr == nullptr)
                _set_err(bmpl::utils::ErrorCode::BAD_INFO_HEADER);
            else if (info_header_ptr->failed())
                _set_err(info_header_ptr->get_error());
            else if (color_map.failed())
                _set_err(color_map.get_error());
            else {
                if (!info_header_ptr->is_vOS21() && !info_header_ptr->is_vOS22())
                    set_warning(bmpl::utils::WarningCode::NOT_OS2_BITMAP_FORMAT);
                _clr_err();
            }
        }

        //---------------------------------------------------------------------------
        bmpl::frmt::BAHeadersList BAHeader::get_BA_headers(const std::string& filepath) noexcept
        {
            bmpl::utils::LEInStream in_stream(filepath);
            return BAHeader::get_BA_headers(in_stream);
        }

        //---------------------------------------------------------------------------
        bmpl::frmt::BAHeadersList BAHeader::get_BA_headers(bmpl::utils::LEInStream& in_stream) noexcept
        {
            if (in_stream.failed())
                return bmpl::frmt::BAHeadersList(in_stream.get_error());

            bmpl::frmt::BAHeadersList ret_headers{};

            auto mem_curs_pos{ in_stream.tellg() };
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
                        in_stream.seekg(mem_curs_pos);
                        return ret_headers;
                    }

                    if (in_stream.failed()) {
                        ret_headers.set_error(in_stream.get_error());
                        break;
                    }
                }

            } while (!ret_headers.back().is_last_header_in_list());

            in_stream.seekg(mem_curs_pos);
            return ret_headers;
        }

        //---------------------------------------------------------------------------
        const std::uint32_t BAHeader::get_colors_count() const noexcept
        {
            if (this->info_header_ptr == nullptr)
                return 0;
            else
                return this->info_header_ptr->get_colors_count();
        }

        //---------------------------------------------------------------------------
        const std::size_t BAHeader::get_content_offset() const noexcept
        {
            return this->file_header_ptr->get_content_offset();
        }

        //---------------------------------------------------------------------------
        const std::size_t BAHeader::get_offset_to_next() const noexcept
        {
            return this->ba_file_header.get_content_offset();
        }

        //---------------------------------------------------------------------------
        const std::int32_t BAHeader::get_device_x_resolution_dpi() const noexcept
        {
            if (this->info_header_ptr == nullptr)
                return 0;
            else
                return std::int32_t(this->info_header_ptr->get_device_x_resolution() * 2.54 / 100 + 0.5);
        }

        //---------------------------------------------------------------------------
        const std::int32_t BAHeader::get_device_y_resolution_dpi() const noexcept
        {
            if (this->info_header_ptr == nullptr)
                return 0;
            else
                return std::int32_t(this->info_header_ptr->get_device_y_resolution() * 2.54 / 100 + 0.5);
        }

        //---------------------------------------------------------------------------
        const std::uint32_t BAHeader::get_height() const noexcept
        {
            if (this->info_header_ptr == nullptr)
                return 0;
            else
                return this->info_header_ptr->get_height();
        }

        //---------------------------------------------------------------------------
        const std::uint32_t BAHeader::get_width() const noexcept
        {
            if (this->info_header_ptr == nullptr)
                return 0;
            else
                return this->info_header_ptr->get_width();
        }

        //---------------------------------------------------------------------------
        const bool BAHeader::is_BA_file(const std::string& filepath) noexcept
        {
            bmpl::utils::LEInStream in_stream(filepath);
            return BAHeader::is_BA_file(in_stream);
        }

        //---------------------------------------------------------------------------
        const bool BAHeader::is_BA_file(bmpl::utils::LEInStream& in_stream) noexcept
        {
            if (in_stream.failed())
                return false;

            std::uint16_t control_word;
            if ((in_stream >> control_word).failed())
                return false;

            return control_word == 0x4142;  // i.e. 'BA' as per little-endian encoding

        }

        //---------------------------------------------------------------------------
        const bool BAHeader::is_last_header_in_list() const noexcept
        {
            return get_offset_to_next() == std::size_t(0);
        }


        //===========================================================================
        //  BAHeadersIterStatus

        //---------------------------------------------------------------------------
        BAHeadersIterStatus::BAHeadersIterStatus(
            const std::string& filepath,
            const BAHeadersList& ba_headers_list
        ) noexcept
            : bmpl::utils::ErrorStatus(bmpl::utils::ErrorCode::NO_ERROR)
            , in_stream_ptr{ new bmpl::utils::LEInStream(filepath) }
            , _begin(ba_headers_list.cbegin())
            , _iter(ba_headers_list.cbegin())
            , _sentinel(ba_headers_list.cend())
        {}

        //---------------------------------------------------------------------------
        const bmpl::frmt::BAHeader& BAHeadersIterStatus::operator*() noexcept
        {
            if (this->_iter != this->_sentinel) {
                return *(this->_iter);
            }
            else {
                _set_err(bmpl::utils::ErrorCode::END_OF_BA_HEADERS_LIST);
                return BAHeadersIterStatus::_EMPTY_BA_HEADER;  // notice: method .failed() returns true on an empty-constructed BA header; the associated error code is NOT_INITIALIZED
            }
        }

        //---------------------------------------------------------------------------
        BAHeadersList::const_iterator BAHeadersIterStatus::operator++() noexcept
        {
            if (this->_iter != this->_sentinel) {
                return ++(this->_iter);
            }
            else {
                _set_err(bmpl::utils::ErrorCode::END_OF_BA_HEADERS_LIST);
                return this->_sentinel;
            }
        }

        //---------------------------------------------------------------------------
        BAHeadersList::const_iterator BAHeadersIterStatus::operator++(int) noexcept
        {
            if (this->_iter != this->_sentinel) {
                BAHeadersList::const_iterator ret_iter{ this->_iter };
                this->_iter++;
                return ret_iter;
            }
            else {
                _set_err(bmpl::utils::ErrorCode::END_OF_BA_HEADERS_LIST);
                return this->_sentinel;
            }
        }

        //---------------------------------------------------------------------------
        const bool BAHeadersIterStatus::end() const noexcept
        {
            return this->_iter == this->_sentinel;
        }

        //---------------------------------------------------------------------------
        void BAHeadersIterStatus::reset() noexcept
        {
            this->_iter = this->_begin;
        }

        //---------------------------------------------------------------------------
        const BAHeader BAHeadersIterStatus::_EMPTY_BA_HEADER{};

    }

}