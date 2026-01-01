#pragma once

/*
MIT License

Copyright (c) 2024-2026 Philippe Schmouker, ph.schmouker (at) gmail.com

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


/*
* The code provided here is an example of  use  of  library  CppBMPLoader.  It
* instantiates BMPImage's defined in this library and decodes then the related 
* images buffer.
* It uses Qt as a mean to display the loaded  images.  For  a  use  of  OpenCV
* instead, just have a look to example code `example_opencv.cpp`.
*
* The code provided here is valid with both c++14 and c++20  versions  of  the
* library.
*
* You can use this code with any BMP file of your own.  You  are  nevertheless
* encouraged to use the test suite "BMP suite" in its last version (2.8):
*     - see https://entropymine.com/jason/bmpsuite/bmpsuite/html/bmpsuite.html
*       to get an overview of the content of this suite and
*     - see https://github.com/jsummers/bmpsuite/ to get access to the code to
*       generate this suite, or
*     - download the suite from here and extract it from the  zipped  archive:
*       https://entropymine.com/jason/bmpsuite/
*
* Finally, directory `./images/` contains the whole BMP Suite 2.8 in directory
* `.images/bmpsuite-2.8/` plus simple BMP images at format V 1.x in  directory
* `.images/v1/` (these are lacking in BMP suite).  See  `.images/v1/README.md`
* for explanations on the content of each version 1 BMP file that is in there.
*/


#include <algorithm>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


#include <QImage>


#include "../cpp_bmp_loader.h"


//===================================================================================
/* \brief Loading an RGB image. */
QImage load_RGB_image(
    const std::string& image_filepath,
    const bool apply_gamma_correction = false,
    const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK) noexcept;

/* \brief Loading an RGBA image. */
QImage load_RGBA_image(
    const std::string& image_filepath,
    const bool apply_gamma_correction = false,
    const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK) noexcept;

/* \brief Loading a BGR image. */
QImage load_BGR_image(
    const std::string& image_filepath,
    const bool apply_gamma_correction = false,
    const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK) noexcept;

/* \brief Loading a BGRA image. */
QImage load_BGRA_image(
    const std::string& image_filepath,
    const bool apply_gamma_correction = false,
    const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK) noexcept;
