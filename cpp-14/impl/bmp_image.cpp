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


#include "../bmp_image.h"

namespace bmpl
{
    //---------------------------------------------------------------------------
    NextImageLoader::NextImageLoader(const std::string& filepath) noexcept
        : MyErrBaseClass()
        , _filepath(filepath)
        , _ba_headers(bmpl::frmt::BAHeader::get_BA_headers(filepath))
        , _ba_hdr_iter(filepath, _ba_headers)
    {
        if (this->_ba_headers.failed()) {
            _set_err(this->_ba_headers.get_error());
        }
        else if (this->_ba_hdr_iter.failed()) {
            _set_err(this->_ba_hdr_iter.get_error());
        }
        else {
            _clr_err();
        }
    }


    //---------------------------------------------------------------------------
    const std::string NextImageLoader::get_error_msg() const noexcept
    {
        return bmpl::utils::error_msg(this->get_filepath(), get_error());
    }


    //---------------------------------------------------------------------------
    const std::string NextImageLoader::get_filepath() const noexcept
    {
        return this->_filepath;
    }


    //---------------------------------------------------------------------------
    void NextImageLoader::reset() noexcept
    {
        this->_ba_hdr_iter.reset();
    }

}
