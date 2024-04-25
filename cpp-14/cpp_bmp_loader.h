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

#include "bmp_file_format/bmp_file_header.h"
#include "bmp_file_format/bmp_info.h"
#include "utils/errors.h"
#include "utils/little_endian_streaming.h"


namespace bmpl
{
    class BMPLoader : public bmpl::utils::ErrorStatus
    {
    public:
        using MyBaseClass = bmpl::utils::ErrorStatus;


        inline BMPLoader(const char* filepath) noexcept
            : MyBaseClass()
            , _in_stream(filepath)
            , _file_header(_in_stream)
            , _info(_in_stream)
        {}


        inline BMPLoader(const std::string& filepath) noexcept
            : MyBaseClass()
            , _in_stream(filepath)
            , _file_header(_in_stream)
            , _info(_in_stream)
        {}


    private:
        bmpl::utils::LEInStream _in_stream;

        // notice: do not modify the ordering of next declarations
        bmpl::frmt::BMPFileHeader _file_header;
        bmpl::frmt::BMPInfo _info;

    };
}