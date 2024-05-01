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


#include <fstream>
#include <string>
#include <vector>

#include "bmp_file_format/bmp_file_header.h"
#include "bmp_file_format/bmp_info.h"
#include "utils/colors.h"
#include "utils/errors.h"
#include "utils/little_endian_streaming.h"


namespace bmpl
{
    //===========================================================================
    // Forward declarations
    //
    class BMPLoader;
    class BMPBottomUpLoader;
    

    //===========================================================================
    class BMPBottomUpLoader : public bmpl::utils::ErrorStatus
    {
    public:
        using MyBaseClass = bmpl::utils::ErrorStatus;


        std::vector<bmpl::clr::RGB> image_content;


        inline BMPBottomUpLoader(const char* filepath) noexcept
            : MyBaseClass()
            , _in_stream(filepath)
            , _file_header(_in_stream)
            , _info(_in_stream)
        {
            _load_image();
        }


        inline BMPBottomUpLoader(const std::string& filepath) noexcept
            : MyBaseClass()
            , _in_stream(filepath)
            , _file_header(_in_stream)
            , _info(_in_stream)
        {
            _load_image();
        }


        virtual inline ~BMPBottomUpLoader() noexcept = default;


        [[nodiscard]]
        inline bmpl::clr::RGB* image_content_ptr() noexcept
        {
            return image_content.data();
        }


        [[nodiscard]]
        inline const std::uint32_t height() const noexcept
        {
            return _info.info_header.height;
        }


        [[nodiscard]]
        inline const std::uint32_t width() const noexcept
        {
            return _info.info_header.width;
        }


    private:
        // notice: do not modify the ordering of next declarations
        bmpl::utils::LEInStream _in_stream;
        bmpl::frmt::BMPFileHeader _file_header;
        bmpl::frmt::BMPInfo _info;

        void _load_image() noexcept;

        void _load_1b() noexcept;
        void _load_4b() noexcept;
        void _load_8b() noexcept;
        void _load_24b() noexcept;

    };


    //===========================================================================
    class BMPLoader : public BMPBottomUpLoader
    {
    public:
        using MyBaseClass = bmpl::utils::ErrorStatus;


        std::vector<bmpl::clr::RGB> image_content;


        inline BMPLoader(const char* filepath) noexcept
            : BMPBottomUpLoader(filepath)
        {
            _reverse_lines_ordering();
        }


        inline BMPLoader(const std::string& filepath) noexcept
            : BMPBottomUpLoader(filepath)
        {
            _reverse_lines_ordering();
        }


        virtual inline ~BMPLoader() noexcept = default;


    protected:
        void _reverse_lines_ordering() noexcept;

    };

}