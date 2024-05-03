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

#include "bmp_file_header.h"


namespace bmpl
{
    namespace frmt
    {
        const bool BMPFileHeader::load(bmpl::utils::LEInStream& in_stream) noexcept
        {
            if (in_stream.failed())
                return _set_err(in_stream.get_error());

            if (!(in_stream >> type >> size >> reserved >> content_offset))
                return _set_err(in_stream.get_error());

            if (type != 0x4d42)  // i.e. "BM", little-endian encoded on 16 bits-
                return _set_err(bmpl::utils::ErrorCode::NOT_BMP_ENCODING);

            if (in_stream.get_size() != size)
                return _set_err(bmpl::utils::ErrorCode::CORRUPTED_BMP_FILE);

            if (content_offset >= size - 14)
                return _set_err(bmpl::utils::ErrorCode::BMP_BAD_ENCODING);

            return _clr_err();
        }

    }
}