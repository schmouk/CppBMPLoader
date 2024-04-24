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



namespace bmpl
{
    namespace utils
    {
        enum class ErrorCode : std::uint8_t
        {
            NO_ERROR = 0,
            BAD_DEFAULT_VALUE,
            BAD_PALLETT_ENCODING,
            BMP_BAD_ENCODING,
            CORRUPTED_BMP_FILE,
            END_OF_FILE,
            FILE_NOT_FOUND,
            INPUT_OPERATION_FAILED,
            IRRECOVERABLE_STREAM_ERROR,
            NOT_BMP_ENCODING,
            NOT_INITIALIZED,
            NOT_WINDOWS_BMP,
            OUT_OF_PALLETT_INDEX,
        };


        class ErrorStatus
        {
        public:

            inline ErrorStatus() noexcept
                : _current_error_code(ErrorCode::NOT_INITIALIZED)
            {}


            inline ErrorStatus(const ErrorCode err_code) noexcept
                : _current_error_code(err_code)
            {}

            inline ErrorStatus(const ErrorStatus&) noexcept = default;
            inline ErrorStatus(ErrorStatus&&) noexcept = default;

            inline virtual ~ErrorStatus() noexcept = default;

            inline ErrorStatus& operator= (const ErrorStatus&) noexcept = default;
            inline ErrorStatus& operator= (ErrorStatus&&) noexcept = default;


            inline operator bool() const noexcept
            {
                return is_ok();
            }


            inline const bmpl::utils::ErrorCode get_error() const noexcept
            {
                return _current_error_code;
            }


            inline const bool is_ok() const noexcept
            {
                return get_error() == ErrorCode::NO_ERROR;
            }


        protected:
            ErrorCode _current_error_code;

            inline const bool _clr_err() noexcept
            {
                _set_err(ErrorCode::NO_ERROR);
                return true;
            }

            inline const bool _set_err(const ErrorCode err_code) noexcept
            {
                _current_error_code = err_code;
                return is_ok();
            }

        };

    }
}