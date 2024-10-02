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


/*
* The code provided here is an example of  use  of  library  CppBMPLoader.  It
* creates  a  BMP  loader  for  every type of BMPImage defined in this library
* and loads then the related BMP images.
* It uses OpenCV as a mean to display the loaded  images.  For  a  use  of  Qt
* instead, just have a look to example code `example_qt.cpp`.
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

#include "example_opencv.h"


//===================================================================================
/* \brief Loading and Displaying an RGB image. */
void load_and_show_RGB_image(
    const std::string& image_filepath,
    const bool apply_gamma_correction,
    const bmpl::clr::ESkippedPixelsMode skipped_mode) noexcept
{
    // let's load the related image
    bmpl::RGBBMPImage bmp_image(image_filepath, apply_gamma_correction, skipped_mode);
    // did loading succeed?
    if (!bmp_image) {  // notice: "if (bmp_image.failed())" would fit also
        // whatever the error, just display it
        std::cout << bmp_image.get_error_msg() << std::endl;
    }
    else {
        // loading was fine
        // let's display maybe warnings
        if (bmp_image.has_warnings()) {
            for (auto& msg : bmp_image.get_warnings_msg())
                std::cout << msg << std::endl;
        }
        // then let's show the loaded image and wait for a key press
        cv::Mat image(cv::Size(bmp_image.get_width(), bmp_image.get_height()), CV_8UC3, bmp_image.get_content_ptr());
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
        cv::imshow("RGB image", image);
        cv::waitKey(0);
    }
}


//===================================================================================
/* \brief Loading and Displaying an RGBA image. */
void load_and_show_RGBA_image(
    const std::string& image_filepath,
    const bool apply_gamma_correction,
    const bmpl::clr::ESkippedPixelsMode skipped_mode) noexcept
{
    // let's load the related image
    bmpl::RGBABMPImage bmp_image(image_filepath, apply_gamma_correction, skipped_mode);
    // did loading succeed?
    if (!bmp_image) {  // notice: "if (bmp_image.failed())" would fit also
        // whatever the error, just display it
        std::cout << bmp_image.get_error_msg() << std::endl;
    }
    else {
        // loading was fine
        // let's display maybe warnings
        if (bmp_image.has_warnings()) {
            for (auto& msg : bmp_image.get_warnings_msg())
                std::cout << msg << std::endl;
        }
        // then let's show the loaded image and wait for a key press
        cv::Mat image(cv::Size(bmp_image.get_width(), bmp_image.get_height()), CV_8UC4, bmp_image.get_content_ptr());
        cv::cvtColor(image, image, cv::COLOR_RGBA2BGRA);
        cv::imshow("RGBA image", image);
        cv::waitKey(0);
    }
}


//===================================================================================
/* \brief Loading and Displaying a BGR image. */
void load_and_show_BGR_image(
    const std::string& image_filepath,
    const bool apply_gamma_correction,
    const bmpl::clr::ESkippedPixelsMode skipped_mode) noexcept
{
    // let's load the related image
    bmpl::BGRBMPImage bmp_image(image_filepath, apply_gamma_correction, skipped_mode);
    // did loading succeed?
    if (!bmp_image) {  // notice: "if (bmp_image.failed())" would fit also
        // whatever the error, just display it
        std::cout << bmp_image.get_error_msg() << std::endl;
    }
    else {
        // loading was fine
        // let's display maybe warnings
        if (bmp_image.has_warnings()) {
            for (auto& msg : bmp_image.get_warnings_msg())
                std::cout << msg << std::endl;
        }
        // then let's show the loaded image and wait for a key press
        cv::Mat image(cv::Size(bmp_image.get_width(), bmp_image.get_height()), CV_8UC3, bmp_image.get_content_ptr());
        cv::imshow("BGR-full", image);
        cv::waitKey(0);
    }
}


//===================================================================================
/* \brief Loading and Displaying a BGRA image. */
void load_and_show_BGRA_image(
    const std::string& image_filepath,
    const bool apply_gamma_correction,
    const bmpl::clr::ESkippedPixelsMode skipped_mode) noexcept
{
    // let's load the related image
    bmpl::BGRABMPImage bmp_image(image_filepath, apply_gamma_correction, skipped_mode);
    // did loading succeed?
    if (!bmp_image) {  // notice: "if (bmp_image.failed())" would fit also
        // whatever the error, just display it
        std::cout << bmp_image.get_error_msg() << std::endl;
    }
    else {
        // loading was fine
        // let's display maybe warnings
        if (bmp_image.has_warnings()) {
            for (auto& msg : bmp_image.get_warnings_msg())
                std::cout << msg << std::endl;
        }
        // then let's show the loaded image and wait for a key press
        cv::Mat image(cv::Size(bmp_image.get_width(), bmp_image.get_height()), CV_8UC4, bmp_image.get_content_ptr());
        cv::imshow("BGRA image", image);
        cv::waitKey(0);
    }
}


//===================================================================================
int main()
{
    // let's avoid OpenCV logs displayed on console
    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_ERROR);

    constexpr bool APPLY_GAMMA_CORRECTION{ true };

    // RGB image
    // we'll use the test image with 8 bits per pixel, a 256 color palette and RLE compression
    // the default values no-gamma-correction and set-to-black-missing-pixels are used in this call.
    load_and_show_RGB_image("../../images/bmpsuite-2.8/g/pal8rle.bmp");

    // RGBA image
    // we'll use a 64-bits per pixel BMP image with gamma correction
    // the default value set-to-black-missing-pixels is used in this call.
    load_and_show_RGBA_image("../../images/bmpsuite-2.8/q/rgba64.bmp", APPLY_GAMMA_CORRECTION);

    // BGR image
    // we'll use a 16 colors palette image with RLE compression and missing definitions of pixels
    // we force no-gamma-correction and missing pixels will be set to the 0-indexed color in the palette (blue color here)
    load_and_show_BGR_image("../../images/bmpsuite-2.8/q/pal4rletrns.bmp", !APPLY_GAMMA_CORRECTION, bmpl::clr::ESkippedPixelsMode::PALETTE_INDEX_0);

    // BGRA image
    // we'll use a special OS/2 Bitmap Array file, first image in file will be shown
    // the default values no-gamma-correction and set-to-black-missing-pixels are used in this call.
    load_and_show_BGRA_image("../../images/bmpsuite-2.8/x/ba-bm.bmp");

}