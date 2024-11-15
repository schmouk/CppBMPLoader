# CppBMPLoader documentation
This documentation relates to the sole API of library **CppBMPLoader**. It does not document the internal implementation of this library.

The API of **CppBMPLoader** is exactly the same for both the c++14 and the c++20 implementations of the library.

Shortly speeking, this API offers a templated class BMPImage<> for the loading of BMP images content. It offers also four specializations of it that you will use in your application. It finally offers classes and functions to extract and load all images from a **BA** file or just the one of them that best fits dimensions, colors and/or resolutions criterias.

Many error cases and warnings are detected by **CppBMPLoader**. Errors are always fatal. Execution stops on any encountered error. Warnings do not stop your application and are accumulated along the processing of BMP and BA files. You can get access to the error code and to a list of warning codes as well as to the associated messages since they are attached to the related faulty BMP images.

Notice: the whole code of **CppBMPLoader** is embedded in namespace `bmpl`.

Up to november 2024, the c++20 version of the code is not yet released.  

Finally, there must still be typos that are remaining in this documentation. They'll be fixed over time. Or not. We'll see.


---
# Examples of code

Examples of code are provided in directory [examples](./examples).

There,
* [example_opencv.cpp](./examples/example_opencv.cpp) shows code that displays images using external library ***OpenCV***;
* [example_qt.cpp](.examples/example_qt.cpp) and [example_qt.h](.:examples/example_qt.h) show code that displays images using external library ***Qt***.

Notice: `example_opencv.cpp` is the only code example that has been validated with the associated external library. The ***Qt*** example is just provided as an example of code and has not been validated by our side.


---
---
# How to load an image from a BMP file
First, include the next header file in your code.
```
#include "bmp_image.h"
```
Notice: including this header file includes all the needed stuff from the library: the API plus its internal implementation header files.

The easiest way to load an image from a BMP file is then to instantiate the templated class `bmpl::BMPImage<>`, four specializations of which are available:
```
namespace bmpl
{
    using RGBBMPImage = BMPImage<bmpl::clr::RGB>;
    using RGBABMPImage = BMPImage<bmpl::clr::RGBA>;
    using BGRBMPImage = BMPImage<bmpl::clr::BGR>;
    using BGRABMPImage = BMPImage<bmpl::clr::BGRA>;
}
```

The `BMP` format encodes pixels values with BGR or BGRA ordered components. The content of the loaded images, once decoded by **CppBMPLoader**, gets its pixels encoded according to one of these four formats:
- BGR, i.e. 3x8-bits components, blue component in lower byte in memory and red component in the higher byte in memory - as specified by `BMP` format or as are internally stored for images in the image processing library **OpenCV**, for instance;
- BGRA, i.e. 4x8-bits components with an alpha channel;
- RGB, i.e. 3x8-bits components, red component in lower byte in memory and blue component in the higher byte in memory;
- RGBA, i.e. 4x8-bits components with an alpha channel.

Hence the names of the templated class specializations.

## Code example
This simple code loads an RGB image from a BMP file:
```
#include "bmp_image.h"

bmpl::RGBBMPImage bmp_image("the/path/to/the/file.bmp");
```

The type of the stored pixels can be found back from the created image via this type wrapper: `bmpl::RGBBMPImage::pixel_type`.

Once decoded, pixels are stored as contiguous in a memory buffer, line after line, from left to right for every line. You will eventually want to get access to this buffer, for any processing or display reason. Class `BMPImage<>` provides a method that returns a pointer to the very beginning of the image buffer:
```
pixel_type* image_buffer{ bmp_image.get_content_ptr() };
```

Should you need to access individual components of pixels, `RGB` and `BGR` pixels provide `.r`, `.g` and `.b` attributes while `RGBA` and `BGRA` pixels provide same attributes plus attribute `.a`. Meanwhile, `RGBA` and `BGRA` pixel own also a global attribute `.value` that embeds the four components as a single `std::uint32_t` value.

Finally, library **CppBMPLoader** delivers images with top line being line 0, while `BMP` images are encoded with bottom line first and top line last - as are stored images and textures in **OpenGL**  for instance. You may ask for a bottom-up delivery of decoded images by **CppBMPLoader**, a constructor parameter exists that you can set to `true` for this, while it defaults to `false`.


---
## class `bmpl::BMPImage<>`

### Introduction
This templated class provides access to the loading of images from BMP files.
```
namespace bmpl
{
    template<typename PixelT>
    class BMPImage;
}
```
It has four specializations:
```
namespace bmpl
{
    using RGBBMPImage  = BMPImage<bmpl::clr::RGB>;
    using RGBABMPImage = BMPImage<bmpl::clr::RGBA>;
    using BGRBMPImage  = BMPImage<bmpl::clr::BGR>;
    using BGRABMPImage = BMPImage<bmpl::clr::BGRA>;
}
```

They all inherit from `bmpl::utils::ErrorStatus` and `bmpl::utils::WarningStatus` which are usefull to get access to any error status as well as to a list of maybe warnings that are associated with the instantiated or loaded image. We'll explain this later.

### Constructors

---
#### *`BMPImage() noexcept = default;`*
The default empty constructor.  
The error status associated with an empty image is `bmpl::utils::ErrorCode::NOT_INITIALIZED`.  
A later call to method `load()` will then initialize the image and set its error status accordingly: `bmpl::utils::ErrorCode::NO_ERROR` if things went fine, or any other error code in case of some loading failure - **CppBMPLoader** detects many of them.  
You may use this default constructor to declare a `bmpl::BMPImage<>` image while the path to the BMP file will be later known. You'll then later load the image with the `load_image()` method, passing it the file path as an argument.

---
#### *`BMPImage(const BMPImage&) noexcept = default;`*
The default copy constructor.

#### *`BMPImage(BMPImage&&) noexcept = default;`*
The default move constructor.

---
#### *`BMPImage(const std::string& filepath, const bool apply_gamma_correction = false, const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK, const bool force_bottom_up = false) noexcept;`*
This is the constructor you will mainly call to load BMP images from files at creation time. It automatically loads the BMP image and sets its error status and warnings list.  
See also *`load_image()`*.

Arguments:
- *`const std::string& filepath`*  
  The path to the BMP file from which the image is to be loaded.

- *`const bool apply_gamma_correction = false`*  
  In some circumstances gamma correction is to be applied on the loaded image. Set this parameter to `true` if you wish to apply gamma correction on the image. If this does not mean anything to you, set it to `false`: **CppBMPLoader** will hold the gamma correction for you when specified in BMP files.  
  Defaults to `false`.

- *`bmpl::clr::ESkippedPixelsMode skipped_mode`*  
  In some circumstances the *Run Length Encoding* of pixels specified by format `BMP`, which specifies a way to set pixels offsets into the image, may leave some pixels unspecified. This should be an encoding error but it may be encountered in some BMP images. **CppBMPLoader** deals with this and allows one from three default values to be set to missing pixels:  
  *`bmpl::clr::ESkippedPixelsMode::BLACK`* sets missing pixels to black color;  
  *`bmpl::clr::ESkippedPixelsMode::PALETTE_INDEX_0`* sets missing pixels to the color of index 0 in the color palette; or  
  *`bmpl::clr::ESkippedPixelsMode::TRANSPARENCY`* sets the alpha channel of missing pixels as full transparency, if such a channel exists (`RGBA` and `BGRA` pixels).  
  Defaults to *`bmpl::clr::ESkippedPixelsMode::BLACK`*.  

- *`const bool force_bottom_up = false`*  
  **CppBMPLoader** automatically loads images with top line first and bottom line last in buffer. In some circumstances you may wish to load images in the reverse order (which is the default oredring of lines in `BMP` format as well as the one specified by **OpenGL**). Set this argument to `true` if you want to load the image with bottom line first and top line last in the image buffer.  
  Defaults to `false`.


### Operators

---
#### *`BMPImage& operator=(const BMPImage&) noexcept = default;`*
The default copy assignment.

#### *`BMPImage& operator=(BMPImage&&) noexcept = default;`*
The default move assignment.

---
#### *`const bool operator! () const noexcept;`*
Returns `true` when some error occured or while the image is not yet initialized. Returns `false` otherwise (i.e. no error).  
Seel also `is_ok()`, `failed()` and `get_error_msg()`.  
Notice: this is a wrapper to method `failed()`. It is inherited from class `bmpl::utils::ErrorStatus`.


### Methods

---
#### *`const bool load_image(const std::string& filepath, const bool apply_gamma_correction = false, const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK, const bool force_bottom_up = false) noexcept;`*
Loads an image from a specified `BMP` file and sets its error status and its list of warnings.  
Returns `true` if loading was successfull or `false` otherwise.  
You will mainly call this method to load BMP images from files after having instantiating a `BMPImage` with the default empty constructor, or to load a new image from another `BMP` file once the image has already been created and loaded.

Arguments:
- *`const std::string& filepath`*  
  The path to the BMP file from which the image is to be loaded.

- *`const bool apply_gamma_correction = false`*  
  In some circumstances gamma correction is to be applied on the loaded image. Set this parameter to `true` if you wish to apply gamma correction on the image. If this does not mean anything to you, set it to `false`: **CppBMPLoader** will hold the gamma correction for you when specified in BMP files.  
  Defaults to `false`.

- *`bmpl::clr::ESkippedPixelsMode skipped_mode`*  
  In some circumstances, the BMP format *Run Length* encoding of pixels, which allows for the specification of pixels offsets into the image, leaves some pixels unspecified. This should be an encoding error but it may be encountered in some BMP images. **CppBMPLoader** deals with this and allows one from three default values to be set to missing pixels:  
  *`bmpl::clr::ESkippedPixelsMode::BLACK`* sets missing pixels to black color;  
  *`bmpl::clr::ESkippedPixelsMode::PALETTE_INDEX_0`* sets missing pixels to the color of index 0 in the color palette; or  
  *`bmpl::clr::ESkippedPixelsMode::TRANSPARENCY`* sets the alpha channel of missing pixels as full transparency, if such a channel exists (`RGBA` and `BGRA` pixels).  
  Defaults to *`bmpl::clr::ESkippedPixelsMode::BLACK`*.  

- *`const bool force_bottom_up = false`*  
  **CppBMPLoader** automatically loads images with top line first and bottom line last in buffer. In some circumstances you may wish to load images in the reverse order (which is the default oredring of lines in `BMP` format as well as the one specified by **OpenGL**). Set this argument to `true` if you want to load the image with bottom line first and top line last in the image buffer.  
  Defaults to `false`.

---
#### *`PixelT* get_content_ptr() noexcept;`*
Returns a pointer to the very first pixel of the image buffer.  
You will call this to apply any processing of your own on the image content or, more often, to display its content.

---
#### *`const bool is_ok() const noexcept;`*
Returns `true` when the image has been successfully loaded, or `false` if no image has yet been loaded or when loading failed.  
See also *`failed()`*, *`operator !`* and *`get_error_msg()`*.  
Notice: inherited from class `bmpl::utils::ErrorStatus`.

#### *`const bool failed() const noexcept;`*
Returns `true` when some arror occured or while the image is not yet initialized. Returns `false` otherwise (i.e. no error).  
See also *`is_ok()`*, *`operator !`* and *`get_error_msg()`*.  
Notice: inherited from class `bmpl::utils::ErrorStatus`.

#### *`const std::string get_error_msg() const noexcept;`*
Returns a formatted string containing English text related to the error encountered, if any.  
See also *`is_ok()`*, *`operator !`* and *`failed()`*.  

---
#### *`const bool has_warnings() const noexcept`*
Returns `true` is some warnings have been detected while loading the image from file, or `false` otherwise.  
See also *`get_warnings_msg()`*.  
Notice: inherited from class `bmpl::utils::WarningStatus`.

#### *`std::vector<std::string> get_warnings_msg() const noexcept;`*
Returns a vector of strings, each containing formatted English text related to a warning detected while loading the image from file. Multiple same warnings are returned only once each in this list.  
See also *`has_warnings()`*.

---
#### *`const std::uint32_t get_height() const noexcept;`*
Returns the height of the image (i.e. its lines count) once it has been successfully loaded, or zero otherwise.  
See also *`get_width()`* and *`image_size()`*

#### *`const std::uint32_t get_width() const noexcept;`*
Returns the width of the image (i.e. its lines count) once it has been successfully loaded, or zero otherwise.  
See also *`get_height()`* and *`image_size()`*

#### *`const std::uint64_t image_size() const noexcept;`*
Returns the size (i.e. overall pixels count) of the image once it has been successfully loaded, or zero otherwise.  
See also *`get_height()`* and *`get_width()`*

---
#### *`const std::string get_filepath() const noexcept;`*
Returns the filepath string associated with the image, i.e. the path to the loaded BMP file.

---
#### *`const std::uint32_t get_colors_count() const noexcept;`*
Returns the number of colors encoded in the color palette as specified in the BMP file. May be zero if no color palette is present in the BMP file.

---
#### *`const std::int32_t get_device_x_resolution_dpi() const noexcept;`*
Returns the resolution along the horizontal axis used to encode the image in the BMP file. This value is provided by **CppBMPLoader** in dots-per-inch units (i.e. *dpi*; it will mostly get `72` dpi as a value), while the BMP format specifies resolutions in pixels-per-meter units (mostly `2835` as its value).  
See also *`get_device_y_resolution_dpi()`*.

#### *`const std::int32_t get_device_y_resolution_dpi() const noexcept;`*
Returns the resolution along the vertical axis used to encode the image in the BMP file. This value is provided by **CppBMPLoader** in dots-per-inch units (i.e. *dpi*; it will mostly get `72` dpi as a value), while the BMP format specifies resolutions in pixels-per-meter units (mostly `2835` as its value).  
See also *`get_device_x_resolution_dpi()`*.

---
#### *`static const bool BMPImage<PixelT>::is_BA_file(const std::string& filepath) noexcept;`*
Returns `true` if the file specified by the passed filepath is a `BA` file, or `false` otherwise. Might not be of a great interest to you since **CppBMPLoader** automatically deals with such files.  
Notice: this is a class method. It can be called without instantiating the class.


---
---
# Loading images from a 'BA' file
This is a specificity of library **CppBMPLoader**. It helps loading also images from `BA` files. These files embed arrays of `BMP` encoded images. It is an OS/2 format rather than a Windows one which rather uses the Windows `BMP` format for the encoding of images.

This format has mostly been used to associate in one single file many encodings of a same image with different number of colors, different resolutions or different dimensions, the best fitting one with the finally targetted device being used then for display or printing.

So, **CppBMPLoader** helps loading:
* all images from a `BA` file at once as a list;
* all images from a `BA` file one after the other;
* the best fitting image according to some criteria (dimensions, colors, resolutions) from a `BA` file.

Notice: `BA` files names get the same suffix as `BMP` files ones: `.bmp`. Both formats are differenciated by the two first bytes of the files content: `'BM'` for `BMP` files and `'BA'` for `BA` files. **CppBMPLoader** takes care of this and sets proper error code and message if needed. You can also check the 'BA'-ness of a file with a call to the library method *`const bool bmpl::BMPImage<>::is_BA_file(const std::string& filepath)`*. There is no need here to specify a pixel type as the template argument of class `BMPImage` since a default one is specified in the declaration of this templated class in the header file `bmp_image.h`. So, the angle brackets may be used empty here.

---
## How to load all images from a `BA` file
First, include the next header file in your code.
```
#include "bmp_image.h"
```
Notice: including this header file includes all the needed stuff from the library: the API plus its internal implementation header files.

Then, just call function *`bmpl::load_all_images<...>(filepath)`* with the type of `BMPImage` you wish to load as the template argument of the function (i.e. replacing the ellipsis). You'll get as the result a vector of BMP images of the type you required.

### Code example
Next example shows a way to use this.
```
#include <iostream>
#include "bmp_image.h"

using ImageType = RGBABMPImage;

bmpl::BMPImagesList<ImageType> images_list("the/path/to/the/bafile.bmp");

if (images_list.failed()) {
    std::cout << images_list.get_error_msg() << std::endl;
}
else {
    if (images_list.has_warnings()) {
        for (auto& msg : images_list.get_warnings_msg())
            std::cout << msg << std::endl;
    }

    for (const ImageType& image : images_list) {
        if (image.failed) {
            std::cout << image.get_error_msg() << std::endl;
        }
        else {
            for (auto& msg : image.get_warnings_msg())
                std::cout << msg << std::endl;
            _display_(image);  // whatever your display function is
        }
    }
}
```


---
#### *`template<typename BMPImageT> const BMPImagesList<BMPImageT> load_all_images<BMPImageT>(const std::string& filepath, const bool apply_gamma_correction = false, const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK, const bool force_bottom_up = false) noexcept;`*
Loads all BMP images that are contained in a `BA` file. Returns a vector of the loaded BMP images, associated with an error status and a warnings list - see also the documentation of the type of the returned vector of images: `class BMPImagesList<BMPImageT>`.

The template argument may be one of next predefined types:
```
namespace bmpl
{
    using RGBBMPImage  = BMPImage<bmpl::clr::RGB>;
    using RGBABMPImage = BMPImage<bmpl::clr::RGBA>;
    using BGRBMPImage  = BMPImage<bmpl::clr::BGR>;
    using BGRABMPImage = BMPImage<bmpl::clr::BGRA>;
}
```
These image types relate to the types of pixel they contain:
- BGR, i.e. 3x8-bits components, blue component in lower byte in memory and red component in the higher byte in memory - as specified by `BMP` format or as are internally stored for images in the image processing library **OpenCV**, for instance;
- BGRA, i.e. 4x8-bits components with an alpha channel;
- RGB, i.e. 3x8-bits components, red component in lower byte in memory and blue component in the higher byte in memory;
- RGBA, i.e. 4x8-bits components with an alpha channel.

Arguments:
- *`const std::string& filepath`*  
  The path to the BMP file from which the image is to be loaded.

- *`const bool apply_gamma_correction = false`*  
  In some circumstances gamma correction is to be applied on the loaded image. Set this parameter to `true` if you wish to apply gamma correction on the image. If this does not mean anything to you, set it to `false`: **CppBMPLoader** will hold the gamma correction for you when specified in BMP files.  
  Defaults to `false`.

- *`bmpl::clr::ESkippedPixelsMode skipped_mode`*  
  In some circumstances, the BMP format *Run Length* encoding of pixels, which allows for the specification of pixels offsets into the image, leaves some pixels unspecified. This should be an encoding error but it may be encountered in some BMP images. **CppBMPLoader** deals with this and allows one from three default values to be set to missing pixels:  
  *`bmpl::clr::ESkippedPixelsMode::BLACK`* sets missing pixels to black color;  
  *`bmpl::clr::ESkippedPixelsMode::PALETTE_INDEX_0`* sets missing pixels to the color of index 0 in the color palette; or  
  *`bmpl::clr::ESkippedPixelsMode::TRANSPARENCY`* sets the alpha channel of missing pixels as full transparency, if such a channel exists (`RGBA` and `BGRA` pixels).  
  Defaults to *`bmpl::clr::ESkippedPixelsMode::BLACK`*.  

- *`const bool force_bottom_up = false`*  
  **CppBMPLoader** automatically loads images with top line first and bottom line last in buffer. In some circumstances you may wish to load images in the reverse order (which is the default oredring of lines in `BMP` format as well as the one specified by **OpenGL**). Set this argument to `true` if you want to load the image with bottom line first and top line last in the image buffer.  
  Defaults to `false`.

---
#### *`template<typename BMPImageT> struct BMPImagesList;`*
The template class for lists of BMP images.

It is declared in header file `"bmp_image.h"`. It can be understood as:
```
namespace bmpl
{
    template<typename BMPImageT>
    struct BMPImagesList
        : public std::vector<ItemT>
        , public bmpl::utils::ErrorStatus
        , public bmpl::utils::WarningStatus;
}
```
So, you get access to all operators and methods of inherited class `std::vector` (e.g. indexing, iterators, size, etc.) plus **CppBMPLoader** next goodies (errors and warnings, see next subsection).  
Should any error or warning have been detected while loading images from file, the resulting list contains the error and warning codes so that you can check failure status and get related error and warnings messages.

---
##### *`const bool is_ok() const noexcept;`*
Returns `true` when the images list has been successfully loaded, or `false` otherwise. The list may have been loaded without error while some loaded images were faulty; always check the error status of loaded images before using any of them.  
See also *`failed()`*, *`operator !`* and *`get_error_msg()`*.  
Notice: inherited from class `bmpl::utils::ErrorStatus`.

##### *`const bool failed() const noexcept;`*
Returns `true` when some arror occured or while loading the list of images, or `false` otherwise (i.e. no error). The list may have been loaded without error while some loaded images were faulty; always check the error status of loaded images before using any of them.  
See also *`is_ok()`*, *`operator !`* and *`get_error_msg()`*.  
Notice: inherited from class `bmpl::utils::ErrorStatus`.

##### *`const std::string get_error_msg() const noexcept;`*
Returns a string containing formatted English text related to the error encountered while loading the list of images, if any.  
See also *`is_ok()`*, *`operator !`* and *`failed()`*.  

---
##### *`const bool has_warnings() const noexcept`*
Returns `true` is some warnings have been detected while loading the list of images from file, or `false` otherwise.  
See also *`get_warnings_msg()`*.  
Notice: inherited from class `bmpl::utils::WarningStatus`.

##### *`std::vector<std::string> get_warnings_msg() const noexcept;`*
Returns a vector of strings, each containing formatted English text related to a warning detected while loading the list of images from file. Multiple same warnings are returned only once each in this list.  
See also *`has_warnings()`*.



---
## How to load images one after the other from a `BA` file
Previous function `bmpl::load_all_images<...>()` loads all images at once. This may be memory space consuming. **CppBMPLoader** helps saving space memory while loading BMP images from a `BA` file by loading images one after the other.

For this, first, include the next header file in your code.
```
#include "bmp_image.h"
```
Notice: including this header file includes all the needed stuff from the library: the API plus its internal implementation header files.

Then, instantiate class *`bmpl::NextImageLoader`* and repeatedly call method *`load()`* until the returned image states a loading error or until method `end()` returns `true` - see code example below. You'll then have completed the loading of all images that are contained in the `BA` file. Method *`reset()`* helps you *rewinding* the running through the `BA` file and load again images starting at the beginning of the file.

Notice: If you aim at loading the best fitting image from a BA file according to some criteria (e.g. dimensions, resolutions or colors count), prefer to call any of the functions described in next section rather than instantiating class *`bmpl::NextImageLoader`* and find the best fitting one by your own. The algorithms that are implemented in the provided **CppBMPLoader** functions are well suited for lowering memory consumption as well as CPU consumption.


### Code example
Next code example shows a way to load images one after another from a `BA` file.

```
#include <iostream>
#include "bmp_image.h"

using ImageType = bmpl::RGBBMPImage;
bmpl::NextImageLoader<ImageType> next_image_loader("the/path/to/the/bafile.bmp");

if (next_image_loader.failed()) {
    std::cout << next_image_loader.get_error_msg() << std::endl;
}
else
{
    next_image_loader.reset();  // not useful here; just to show the call

    while (!next_image_loader.end()) {
        ImageType next_image{ next_image_loader.load() };

        if (next_image.failed()) {
            std::cout << next_image.get_error_msg() << std::endl;
        }
        else {
            for (auto& msg : next_image.get_warnings_msg())
                std::cout << msg << std::endl;
            _display_(next_image);  // whatever your display function is
        }
    }
}
```


---
### *`class bmpl::NextImageLoader`*
Declared as 
```
 template<typename BMPImageT>
 class NextImageLoader : public bmpl::utils::ErrorStatus;
```

Notice: it inherits from `bmpl::utils::ErrorStatus` which is usefull to get access to error status. The inherited methods are explained further.

### Constructors

---
#### *`NextImageLoader() noexcept = delete;`*
The empty constructor is deleted. Instantiation of this class with no parameter is then forbidden, as well as are copy and move constructors and copy and move assignment operators.

---
#### *`NextImageLoader(const std::string& filepath, const bool apply_gamma_correction = false, const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK, const bool force_bottom_up = false) noexcept;`*
This is the constructor you must call to isntantiate a next-image-loader. It automatically sets internal state for the loading of all BMP images and for the running through them.  
See also *`load_image()`*.

Arguments:
- *`const std::string& filepath`*  
  The path to the BA file from which the images are to be loaded.

- *`const bool apply_gamma_correction = false`*  
  In some circumstances gamma correction is to be applied on the loaded images. Set this parameter to `true` if you wish to apply gamma correction on the images. If this does not mean anything to you, set it to `false`: **CppBMPLoader** will hold the gamma correction for you when specified in BA files.  
  Defaults to `false`.

- *`bmpl::clr::ESkippedPixelsMode skipped_mode`*  
  In some circumstances, the BMP format *Run Length* encoding of pixels, which allows for the specification of pixels offsets into the image, leaves some pixels unspecified. This should be an encoding error but it may be encountered in some BMP images. **CppBMPLoader** deals with this and allows one from three default values to be set to missing pixels:  
  *`bmpl::clr::ESkippedPixelsMode::BLACK`* sets missing pixels to black color;  
  *`bmpl::clr::ESkippedPixelsMode::PALETTE_INDEX_0`* sets missing pixels to the color of index 0 in the color palette; or  
  *`bmpl::clr::ESkippedPixelsMode::TRANSPARENCY`* sets the alpha channel of missing pixels as full transparency, if such a channel exists (`RGBA` and `BGRA` pixels).  
  Defaults to *`bmpl::clr::ESkippedPixelsMode::BLACK`*.  

- *`const bool force_bottom_up = false`*  
  **CppBMPLoader** automatically loads images with top line first and bottom line last in buffer. In some circumstances you may wish to load images in the reverse order (which is the default oredring of lines in `BMP` format as well as the one specified by **OpenGL**). Set this argument to `true` if you want to load the image with bottom line first and top line last in the image buffer.  
  Defaults to `false`.

### Operators

---
#### *`const bool operator! () const noexcept;`*
Returns `true` when some arror occured while instantiating the next-image-loader or when the last loading of an image was faulty. Returns `false` otherwise (i.e. no error).  
Seel also `is_ok()`, `failed()` and `get_error_msg()`.  
Notice: this is a wrapper to method `failed()`. It is inherited from class `bmpl::utils::ErrorStatus`.

### Methods

---
#### *`const bool end() const noexcept;`*
Returns `true` when the list of contained images has been exhausted, or `false` otherwise.

---
#### *`const bool is_ok() const noexcept;`*
Returns `true` when the instantiation of the next-image-loader was ok, or `false` otherwise.  
See also *`failed()`*, *`operator !`* and *`get_error_msg()`*.  
Notice: inherited from class `bmpl::utils::ErrorStatus`.

#### *`const bool failed() const noexcept;`*
Returns `true` when some arror occured at instantiation time, or `false` otherwise (i.e. no error).  
See also *`is_ok()`*, *`operator !`* and *`get_error_msg()`*.  
Notice: inherited from class `bmpl::utils::ErrorStatus`.

#### *`const std::string get_error_msg() const noexcept;`*
Returns a string containing formatted English text related to the error encountered, if any, at instantiation time.  
See also *`is_ok()`*, *`operator !`* and *`failed()`*.  

---
#### *`const std::string get_filepath() const noexcept;`*
Returns the filepath to the BA file.

---
#### *`BMPImageT load_image() noexcept;`*
Loads next image in the specified `BA` file and sets its error status and warnings list.  
Returns a copy of the loaded image.  
If the end of the list of images contained in the BA file has been reached, the returned image is set to be faulty: method *`.is_ok()`* applied to the returned image returns `false` (*`.failed()`* and *`operator !`* return `true`) and the associated error code is *`bmpl::utils::ErrorCode::END_OF_BA_HEADERS_LIST`*. Meanwhile, a call to the next-image-loader method *`.end()`* returns `true` also.  
Notice: after loading an image, always check its error status before using its content. A faulty image does not limit the running through the contained images in the `BA` file so its faulty status is only associated with it and not with the next-image-loader.

---
#### *`void reset() noexcept;`*
Resets the internal state of the next-image-loader. The next call to method *`load_image()`* returns the very first image in the `BA` file, as long as the next-image-loader is not faulty itself of course. Then, next calls to *`load_image()`*  run through the whole list until its completion, one image after the other and until the last one in file.



---
## How to load the image that best fits a specified colors count from a `BA` file

The colors count is determined by the number of bits per pixel : 1 bit -> 2 colors, 4 bits -> 16 colors, 8 bits -> 256 colors, 16 bits -> 65,536 colors, 24 bits -> "true" colors, 32 bits -> "true" colors plus alpha channel, 64 bits -> HDR encoding (13 bits per color component according to `BMP` specifications).

The best fitting image according to the number of bits per pixel is the first one in `BA` file that is less or equal to the targetted number you specify and that is the most approaching this target number.

So, would multiple images fit the criteria, the first fitting one found in the BA file is returned.

Should all images that are contained in the BA file be encoded with a bigger number of bit per pixel than the targetted one, the first image in file will be returned - this is the specification of BA format: when no image fits a specified criteria, the first image in file is provided.

To get the best fitting image according to the number of colors it is encoded with, first include the next header file in your code.
```
#include "bmp_best_fitting_image.h"
```
Notice: including this header file includes all the needed stuff from the library: the API plus its internal implementation header files.

Then, instantiate the templated class *`bmpl::BMPBestFittingColorsImage<>`*. This class inherits from the templated class *`bmpl::BMPImage<>`* and, as such, provides the exact same API for error and warnings status, for access to its image buffer and the like - see *`bmpl::BMPImage<>`*. It is declared this way:
```
namespace bmpl
{
    template<typename PixelT>
    struct BMPBestFittingColorsImage : public bmpl::BMPImage<PixelT>;
}
```

Four specializations are available, each related to one pixel encoding type (i.e. RGB, RGBA, BGR and BGRA):
```
namespace bmpl
{
    using RGBBMPBestFittingColorsImage  = BMPBestFittingColorsImage<bmpl::clr::RGB>;
    using RGBABMPBestFittingColorsImage = BMPBestFittingColorsImage<bmpl::clr::RGBA>;
    using BGRBMPBestFittingColorsImage  = BMPBestFittingColorsImage<bmpl::clr::BGR>;
    using BGRABMPBestFittingColorsImage = BMPBestFittingColorsImage<bmpl::clr::BGRA>;
}
```

### Code example
Next example shows a way to use this.
```
#include <iostream>
#include "bmp_best_fitting_image.h"

// searches for an image with a 256 (at most) colors palette
bmpl::BGRABMPBestFittingColorsImage best_fitting_image("your/filepath/to/the/bafile.bmp", 8);  // 256 colors = 8 bits per pixel

if (best_fitting_image.failed()) {
    std::cout << best_fitting_image.get_error_msg() << std::endl;
}
else {
    for (auto& msg : best_fitting_image.get_warnings_msg())
        std::cout << msg << std::endl;
    _display_(best_fitting_image);  // whatever your display function is
}
```

---
### *`class bmpl::BMPBestFittingColorsImage`*
Declared as 
```
template<typename PixelT>
struct BMPBestFittingColorsImage : public bmpl::BMPImage<PixelT>;
```
Reminder: This class inherits from the templated class *`bmpl::BMPImage<>`* and, as such, provides the exact same API for error and warnings status, for accessing its image buffer and the like - see *`bmpl::BMPImage<>`*.

The template argument *`PixelT`* defines the encoding of contained pixels in the filan image. Library **CppBMPLoader** defines four types of pixels:
- BGR, i.e. 3x8-bits components, blue component in lower byte in memory and red component in the higher byte in memory - as specified by `BMP` format or as are internally stored for images in the image processing library **OpenCV**, for instance;
- BGRA, i.e. 4x8-bits components with an alpha channel;
- RGB, i.e. 3x8-bits components, red component in lower byte in memory and blue component in the higher byte in memory;
- RGBA, i.e. 4x8-bits components with an alpha channel.

**CppBMPLoader** defines also four specializations of templated class *`bmpl::BMPBestFittingColorsImage`*:
```
namespace bmpl
{
    using RGBBMPBestFittingColorsImage  = BMPBestFittingColorsImage<bmpl::clr::RGB>;
    using RGBABMPBestFittingColorsImage = BMPBestFittingColorsImage<bmpl::clr::RGBA>;
    using BGRBMPBestFittingColorsImage  = BMPBestFittingColorsImage<bmpl::clr::BGR>;
    using BGRABMPBestFittingColorsImage = BMPBestFittingColorsImage<bmpl::clr::BGRA>;
}
```
These are the ones you should use in the code of your application.


### Constructors

---
#### *`BMPBestFittingColorsImage() noexcept = default;`*
The default empty constructor.  
You may instantiate an empty image with this constructor. Its error status is then be set to `bmpl::utils::ErrorCode::NOT_INITIALIZED`. You will then call method *`load_image()`* to load the best fitting image from a BA file.

---
#### *`BMPBestFittingColorsImage(const bmpl::BMPImage<PixelT> image) noexcept;`*
This constructors copies a *`bmpl::BMPImage<>`* with same template argument into this best-fitting-image. It might be that you will never have to use this constructor in your applications, but it gets uses internally in library **CppBMPLoader**.

---
#### *`BMPBestFittingColorsImage(const std::string& filepath, const std::uint32_t target_bits_per_pixel, const bool apply_gamma_corection = false, const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK, const bool force_bottom_up = false) noexcept;`*

Arguments:
- *`const std::string& filepath`*  
  The path to the BA file from which the image is to be loaded.

- *`target_bits_per_pixel`*  
  The targetted bits per pixels that encodes colors. The loaded image will be the first image in `BA` file that is at the nearest of this value and not above it.

- *`const bool apply_gamma_correction = false`*  
  In some circumstances gamma correction is to be applied on the loaded image. Set this parameter to `true` if you wish to apply gamma correction on the image. If this does not mean anything to you, set it to `false`: **CppBMPLoader** will hold the gamma correction for you when specified in BMP files.  
  Defaults to `false`.

- *`bmpl::clr::ESkippedPixelsMode skipped_mode`*  
  In some circumstances, the BMP format *Run Length* encoding of pixels, which allows for the specification of pixels offsets into the image, leaves some pixels unspecified. This should be an encoding error but it may be encountered in some BMP images. **CppBMPLoader** deals with this and allows one from three default values to be set to missing pixels:  
  *`bmpl::clr::ESkippedPixelsMode::BLACK`* sets missing pixels to black color;  
  *`bmpl::clr::ESkippedPixelsMode::PALETTE_INDEX_0`* sets missing pixels to the color of index 0 in the color palette; or  
  *`bmpl::clr::ESkippedPixelsMode::TRANSPARENCY`* sets the alpha channel of missing pixels as full transparency, if such a channel exists (`RGBA` and `BGRA` pixels).  
  Defaults to *`bmpl::clr::ESkippedPixelsMode::BLACK`*.  

- *`const bool force_bottom_up = false`*  
  **CppBMPLoader** automatically loads images with top line first and bottom line last in buffer. In some circumstances you may wish to load images in the reverse order (which is the default oredring of lines in `BMP` format as well as the one specified by **OpenGL**). Set this argument to `true` if you want to load the image with bottom line first and top line last in the image buffer.  
  Defaults to `false`.

---
#### *`BMPBestFittingColorsImage(const BMPBestFittingColorsImage&) noexcept = default;`*
The default copy constructor.

#### *`BMPBestFittingColorsImage(BMPBestFittingColorsImage&&) noexcept = default;`*
The default move constructor.


### Operators

---
#### *`BMPBestFittingColorsImage& operator=(const BMPBestFittingColorsImage&) noexcept = default;`*
The default copy assignement operator.

#### *`BMPBestFittingColorsImage& operator=(BMPBestFittingColorsImage&&) noexcept = default;`*
The default move assignment operator.

---
#### *`const bool operator! () const noexcept;`*
Returns `true` when some arror occured while loading the image was faulty. Returns `false` otherwise (i.e. no error).  
Seel also `is_ok()`, `failed()` and `get_error_msg()`.  
Notice: this is a wrapper to method `failed()`.  
Inherited from class *`bmpl::BMPImage<>`*.


### Methods

---
#### *`const bool load_image(const std::string& filepath, const std::uint32_t target_bits_per_pixel, const bool apply_gamma_correction = false, const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK, const bool force_bottom_up = false) noexcept;`*
Loads an image from a specified `BA` file and sets its error status and warnings list.  
Returns `true` if loading was successfull or `false` otherwise.  
You will mainly call this method to load BMP images from BA files after having instantiating a *`BMPBestFittingColorsImage`* with the default empty constructor, or to load a new image from another `BA` file.

Arguments:
- *`const std::string& filepath`*  
  The path to the BMP file from which the image is to be loaded.

- *`target_bits_per_pixel`*  
  The targetted bits per pixels that encode colors. The loaded image will be the one that is at the nearest of this value and not above it.

- *`const bool apply_gamma_correction = false`*  
  In some circumstances gamma correction is to be applied on the loaded image. Set this parameter to `true` if you wish to apply gamma correction on the image. If this does not mean anything to you, set it to `false`: **CppBMPLoader** will hold the gamma correction for you when specified in BMP files.  
  Defaults to `false`.

- *`bmpl::clr::ESkippedPixelsMode skipped_mode`*  
  In some circumstances, the BMP format *Run Length* encoding of pixels, which allows for the specification of pixels offsets into the image, leaves some pixels unspecified. This should be an encoding error but it may be encountered in some BMP images. **CppBMPLoader** deals with this and allows one from three default values to be set to missing pixels:  
  *`bmpl::clr::ESkippedPixelsMode::BLACK`* sets missing pixels to black color;  
  *`bmpl::clr::ESkippedPixelsMode::PALETTE_INDEX_0`* sets missing pixels to the color of index 0 in the color palette; or  
  *`bmpl::clr::ESkippedPixelsMode::TRANSPARENCY`* sets the alpha channel of missing pixels as full transparency, if such a channel exists (`RGBA` and `BGRA` pixels).  
  Defaults to *`bmpl::clr::ESkippedPixelsMode::BLACK`*.  

- *`const bool force_bottom_up = false`*  
  **CppBMPLoader** automatically loads images with top line first and bottom line last in buffer. In some circumstances you may wish to load images in the reverse order (which is the default oredring of lines in `BMP` format as well as the one specified by **OpenGL**). Set this argument to `true` if you want to load the image with bottom line first and top line last in the image buffer.  
  Defaults to `false`.

---
#### *`PixelT* get_content_ptr() noexcept;`*
Returns a pointer to the very first pixel of the image buffer.  
You will call this to apply any processing of your own on the image content or, more often, to display its content.  
Inherited from class *`bmpl::BMPImage<>`*.

---
#### *`const bool is_ok() const noexcept;`*
Returns `true` when the image has been successfully loaded, or `false` if no image has yet been loaded or when loading failed.  
See also *`failed()`*, *`operator !`* and *`get_error_msg()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`const bool failed() const noexcept;`*
Returns `true` when some arror occured or while the image is not yet initialized. Returns `false` otherwise (i.e. no error).  
See also *`is_ok()`*, *`operator !`* and *`get_error_msg()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`const std::string get_error_msg() const noexcept;`*
Returns a string containing English text related to the error encountered, if any.  
See also *`is_ok()`*, *`operator !`* and *`failed()`*.  
Inherited from class *`bmpl::BMPImage<>`*.  

---
#### *`const bool has_warnings() const noexcept`*
Returns `true` is some warnings have been detected while loading the image from file, or `false` otherwise.  
See also *`get_warnings_msg()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`std::vector<std::string> get_warnings_msg() const noexcept;`*
Returns a vector of strings, each containing English text related to a warning detected while loading the image from file. Multiple same warnings are returned only once each in this list.  
See also *`has_warnings()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

---
#### *`const std::uint32_t get_height() const noexcept;`*
Returns the height of the image (i.e. lines count) once it has been loaded, or zero otherwise.  
See also *`get_width()`* and *`image_size()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`const std::uint32_t get_width() const noexcept;`*
Returns the width of the image (i.e. lines count) once it has been loaded, or zero otherwise.  
See also *`get_height()`* and *`image_size()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`const std::uint64_t image_size() const noexcept;`*
Returns the size (i.e. overall pixels count) of the image once it has been loaded, or zero otherwise.  
See also *`get_height()`* and *`get_width()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

---
#### *`const std::string get_filepath() const noexcept;`*
Returns the filepath string associated with the image, i.e. the path to the loaded BMP file.  
Inherited from class *`bmpl::BMPImage<>`*.

---
#### *`const std::uint32_t get_colors_count() const noexcept;`*
Returns the number of colors encoded in the color palette as specified in the BMP file. May be zero if no color palette is present in the BMP file.  
Inherited from class *`bmpl::BMPImage<>`*.

---
#### *`const std::int32_t get_device_x_resolution_dpi() const noexcept;`*
Returns the resolution along the horizontal axis used to encode the image in the BMP file. This value is provided by **CppBMPLoader** in dots-per-inch units (it will mostly get `72` dpi as a value), while the BMP format specifies resolutions in pixels-per-meter units (mostly `2835` as its value).  
See also *`get_device_y_resolution_dpi()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`const std::int32_t get_device_y_resolution_dpi() const noexcept;`*
Returns the resolution along the vertical axis used to encode the image in the BMP file. This value is provided by **CppBMPLoader** in dots-per-inch units (it will mostly get `72` dpi as a value), while the BMP format specifies resolutions in pixels-per-meter units (mostly `2835` as its value).  
See also *`get_device_x_resolution_dpi()`*.  
Inherited from class *`bmpl::BMPImage<>`*.



---
## How to load the image that best fits specified dimensions from a `BA` file

The dimensions are the width and the height of an image.

The best fitting image according to its dimensions is the first one in `BA` file from images for which horizontal (i.e. width) and vertical (i.e. height) dimensions are less than the related targetted ones and for which the sum of the differences is the smallest.

So, would multiple images fit the criteria, the first fitting one found in the BA file is provided.

Should no image that is contained in the BA file fits the targetted dimensions, the first image in file will be returned - this is the specification of BA format: when no image fits criteria the first one in file is provided.

To get the best fitting image according to its dimensions, first include the next header file in your code.
```
#include "bmp_best_fitting_image.h"
```
Notice: including this header file includes all the needed stuff from the library: the API plus its internal implementation header files.

Then, instantiate the templated class *`bmpl::BMPBestFittingSizeImage<>`*. This class inherits from the templated class *`bmpl::BMPImage<>`* and, as such, provides the exact same API for error and warnings status, for accessing its image buffer and the like - see *`bmpl::BMPImage<>`*. It is declared this way:
```
namespace bmpl
{
    template<typename PixelT>
    struct BMPBestFittingSizeImage : public bmpl::BMPImage<PixelT>;
}
```

Four specializations are available, each related to one pixel encoding type (i.e. RGB, RGBA, BGR and BGRA):
```
namespace bmpl
{
    using RGBBMPBestFittingSizeImage  = BMPBestFittingSizeImage<bmpl::clr::RGB>;
    using RGBABMPBestFittingSizeImage = BMPBestFittingSizeImage<bmpl::clr::RGBA>;
    using BGRBMPBestFittingSizeImage  = BMPBestFittingSizeImage<bmpl::clr::BGR>;
    using BGRABMPBestFittingSizeImage = BMPBestFittingSizeImage<bmpl::clr::BGRA>;
}
```

### Code example
Next example shows a way to use this.
```
#include <iostream>
#include "bmp_best_fitting_image.h"

// searches for an image with 800 x 640 dimensions
bmpl::BGRABMPBestFittingSizeImage best_fitting_image("the/path/to/the/bafile.bmp", 800, 640);

if (best_fitting_image.failed()) {
    std::cout << best_fitting_image.get_error_msg() << std::endl;
}
else {
    for (auto& msg : best_fitting_image.get_warnings_msg())
        std::cout << msg << std::endl;
    _display_(best_fitting_image);  // whatever your display function is
}
```

---
### *`class bmpl::BMPBestFittingSizeImage`*
Declared as 
```
template<typename PixelT>
struct BMPBestFittingSizeImage : public bmpl::BMPImage<PixelT>;
```
Reminder: This class inherits from the templated class *`bmpl::BMPImage<>`* and, as such, provides the exact same API for error and warnings status, for accessing its image buffer and the like - see *`bmpl::BMPImage<>`*.

The template argument *`PixelT`* defines the encoding of contained pixels in the image. Library **CppBMPLoader** defines four types of pixels:
- BGR, i.e. 3x8-bits components, blue component in lower byte in memory and red component in the higher byte in memory - as specified by `BMP` format or as are internally stored for images in the image processing library **OpenCV**, for instance;
- BGRA, i.e. 4x8-bits components with an alpha channel;
- RGB, i.e. 3x8-bits components, red component in lower byte in memory and blue component in the higher byte in memory;
- RGBA, i.e. 4x8-bits components with an alpha channel.

**CppBMPLoader** defines also four specializations of templated class *`bmpl::BMPBestFittingSizeImage`*:
```
namespace bmpl
{
    using RGBBMPBestFittingSizeImage  = BMPBestFittingSizeImage<bmpl::clr::RGB>;
    using RGBABMPBestFittingSizeImage = BMPBestFittingSizeImage<bmpl::clr::RGBA>;
    using BGRBMPBestFittingSizeImage  = BMPBestFittingSizeImage<bmpl::clr::BGR>;
    using BGRABMPBestFittingSizeImage = BMPBestFittingSizeImage<bmpl::clr::BGRA>;
}
```
These are the ones you should use in the code of your application.


### Constructors

---
#### *`BMPBestFittingSizeImage() noexcept = default;`*
The default empty constructor.  
You may instantiate an empty image with this constructor. Its error status is then be set to `bmpl::utils::ErrorCode::NOT_INITIALIZED`. You will then call method *`load_image()`* to load the best fitting image from a BA file.

---
#### *`BMPBestFittingSizeImage(const bmpl::BMPImage<PixelT> image) noexcept;`*
This constructors copies a *`bmpl::BMPImage<>`* with same template argument into this best-fitting-image. It might be that you will never have to use this constructor in your applications, but it gets uses internally in library **CppBMPLoader**.

---
#### *`BMPBestFittingSizeImage(const std::string& filepath, const std::uint32_t target_width, const std::uint32_t target_height, const bool apply_gamma_corection = false, const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK, const bool force_bottom_up = false) noexcept;`*

Arguments:
- *`const std::string& filepath`*  
  The path to the BA file from which the image is to be loaded.

- *`target_width`*  
  The targetted width for the returned image. The loaded image will be the one that is at the nearest of this value and the targetted height value while not above them.

- *`target_height`*  
  The targetted height for the returned image. The loaded image will be the one that is at the nearest of this value and the targetted width value while not above them.

- *`const bool apply_gamma_correction = false`*  
  In some circumstances gamma correction is to be applied on the loaded image. Set this parameter to `true` if you wish to apply gamma correction on the image. If this does not mean anything to you, set it to `false`: **CppBMPLoader** will hold the gamma correction for you when specified in BMP files.  
  Defaults to `false`.

- *`bmpl::clr::ESkippedPixelsMode skipped_mode`*  
  In some circumstances, the BMP format *Run Length* encoding of pixels, which allows for the specification of pixels offsets into the image, leaves some pixels unspecified. This should be an encoding error but it may be encountered in some BMP images. **CppBMPLoader** deals with this and allows one from three default values to be set to missing pixels:  
  *`bmpl::clr::ESkippedPixelsMode::BLACK`* sets missing pixels to black color;  
  *`bmpl::clr::ESkippedPixelsMode::PALETTE_INDEX_0`* sets missing pixels to the color of index 0 in the color palette; or  
  *`bmpl::clr::ESkippedPixelsMode::TRANSPARENCY`* sets the alpha channel of missing pixels as full transparency, if such a channel exists (`RGBA` and `BGRA` pixels).  
  Defaults to *`bmpl::clr::ESkippedPixelsMode::BLACK`*.  

- *`const bool force_bottom_up = false`*  
  **CppBMPLoader** automatically loads images with top line first and bottom line last in buffer. In some circumstances you may wish to load images in the reverse order (which is the default oredring of lines in `BMP` format as well as the one specified by **OpenGL**). Set this argument to `true` if you want to load the image with bottom line first and top line last in the image buffer.  
  Defaults to `false`.

---
#### *`BMPBestFittingSizeImage(const BMPBestFittingSizeImage&) noexcept = default;`*
The default copy constructor.

#### *`BMPBestFittingSizeImage(BMPBestFittingSizeImage&&) noexcept = default;`*
The default move constructor.


### Operators

---
#### *`BMPBestFittingSizeImage& operator=(const BMPBestFittingSizeImage&) noexcept = default;`*
The default copy assignement operator.

#### *`BMPBestFittingSizeImage& operator=(BMPBestFittingSizeImage&&) noexcept = default;`*
The default move assignment operator.

---
#### *`const bool operator! () const noexcept;`*
Returns `true` when some arror occured while loading the image was faulty. Returns `false` otherwise (i.e. no error).  
Seel also `is_ok()`, `failed()` and `get_error_msg()`.  
Notice: this is a wrapper to method `failed()`.  
Inherited from class *`bmpl::BMPImage<>`*.


### Methods

---
#### *`const bool load_image(const std::string& filepath, const std::uint32_t target_width, const std::uint32_t target_height, const bool apply_gamma_correction = false, const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK, const bool force_bottom_up = false) noexcept;`*
Loads an image from a specified `BA` file and sets its error status and warnings list.  
Returns `true` if loading was successfull or `false` otherwise.  
You will mainly call this method to load BMP images from BA files after having instantiating a *`BMPBestFittingSizeImage`* with the default empty constructor, or to load a new image from another `BA` file.

Arguments:
- *`const std::string& filepath`*  
  The path to the BMP file from which the image is to be loaded.

- *`target_width`*  
  The targetted width for the returned image. The loaded image will be the one that is at the nearest of this value and the targetted height value while not above them.

- *`target_height`*  
  The targetted height for the returned image. The loaded image will be the one that is at the nearest of this value and the targetted width value while not above them.

- *`const bool apply_gamma_correction = false`*  
  In some circumstances gamma correction is to be applied on the loaded image. Set this parameter to `true` if you wish to apply gamma correction on the image. If this does not mean anything to you, set it to `false`: **CppBMPLoader** will hold the gamma correction for you when specified in BMP files.  
  Defaults to `false`.

- *`bmpl::clr::ESkippedPixelsMode skipped_mode`*  
  In some circumstances, the BMP format *Run Length* encoding of pixels, which allows for the specification of pixels offsets into the image, leaves some pixels unspecified. This should be an encoding error but it may be encountered in some BMP images. **CppBMPLoader** deals with this and allows one from three default values to be set to missing pixels:  
  *`bmpl::clr::ESkippedPixelsMode::BLACK`* sets missing pixels to black color;  
  *`bmpl::clr::ESkippedPixelsMode::PALETTE_INDEX_0`* sets missing pixels to the color of index 0 in the color palette; or  
  *`bmpl::clr::ESkippedPixelsMode::TRANSPARENCY`* sets the alpha channel of missing pixels as full transparency, if such a channel exists (`RGBA` and `BGRA` pixels).  
  Defaults to *`bmpl::clr::ESkippedPixelsMode::BLACK`*.  

- *`const bool force_bottom_up = false`*  
  **CppBMPLoader** automatically loads images with top line first and bottom line last in buffer. In some circumstances you may wish to load images in the reverse order (which is the default oredring of lines in `BMP` format as well as the one specified by **OpenGL**). Set this argument to `true` if you want to load the image with bottom line first and top line last in the image buffer.  
  Defaults to `false`.

---
#### *`PixelT* get_content_ptr() noexcept;`*
Returns a pointer to the very first pixel of the image buffer.  
You will call this to apply any processing of your own on the image content or, more often, to display its content.  
Inherited from class *`bmpl::BMPImage<>`*.

---
#### *`const bool is_ok() const noexcept;`*
Returns `true` when the image has been successfully loaded, or `false` if no image has yet been loaded or when loading failed.  
See also *`failed()`*, *`operator !`* and *`get_error_msg()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`const bool failed() const noexcept;`*
Returns `true` when some arror occured or while the image is not yet initialized. Returns `false` otherwise (i.e. no error).  
See also *`is_ok()`*, *`operator !`* and *`get_error_msg()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`const std::string get_error_msg() const noexcept;`*
Returns a string containing English text related to the error encountered, if any.  
See also *`is_ok()`*, *`operator !`* and *`failed()`*.  
Inherited from class *`bmpl::BMPImage<>`*.  

---
#### *`const bool has_warnings() const noexcept`*
Returns `true` is some warnings have been detected while loading the image from file, or `false` otherwise.  
See also *`get_warnings_msg()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`std::vector<std::string> get_warnings_msg() const noexcept;`*
Returns a vector of strings, each containing English text related to a warning detected while loading the image from file. Multiple same warnings are returned only once each in this list.  
See also *`has_warnings()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

---
#### *`const std::uint32_t get_height() const noexcept;`*
Returns the height of the image (i.e. lines count) once it has been loaded, or zero otherwise.  
See also *`get_width()`* and *`image_size()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`const std::uint32_t get_width() const noexcept;`*
Returns the width of the image (i.e. lines count) once it has been loaded, or zero otherwise.  
See also *`get_height()`* and *`image_size()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`const std::uint64_t image_size() const noexcept;`*
Returns the size (i.e. overall pixels count) of the image once it has been loaded, or zero otherwise.  
See also *`get_height()`* and *`get_width()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

---
#### *`const std::string get_filepath() const noexcept;`*
Returns the filepath string associated with the image, i.e. the path to the loaded BMP file.  
Inherited from class *`bmpl::BMPImage<>`*.

---
#### *`const std::uint32_t get_colors_count() const noexcept;`*
Returns the number of colors encoded in the color palette as specified in the BMP file. May be zero if no color palette is present in the BMP file.  
Inherited from class *`bmpl::BMPImage<>`*.

---
#### *`const std::int32_t get_device_x_resolution_dpi() const noexcept;`*
Returns the resolution along the horizontal axis used to encode the image in the BMP file. This value is provided by **CppBMPLoader** in dots-per-inch units (it will mostly get `72` dpi as a value), while the BMP format specifies resolutions in pixels-per-meter units (mostly `2835` as its value).  
See also *`get_device_y_resolution_dpi()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`const std::int32_t get_device_y_resolution_dpi() const noexcept;`*
Returns the resolution along the vertical axis used to encode the image in the BMP file. This value is provided by **CppBMPLoader** in dots-per-inch units (it will mostly get `72` dpi as a value), while the BMP format specifies resolutions in pixels-per-meter units (mostly `2835` as its value).  
See also *`get_device_x_resolution_dpi()`*.  
Inherited from class *`bmpl::BMPImage<>`*.



---
## How to load the image that best fits specified resolutions from a `BA` file

Every `BMP` image header contains its resolutions definition, the horizontal and the vertical ones. The `BMP` format evaluates these resolutions in dots per meter as their unit. Library **CPPBmpLoader** evaluates them as dots per inch (i.e. dpi). As such, most images prepared for their displaying on screens have 72 dpi resolutions (2835 dots per meter). Images prepared for their printing on other devices may have higher resolutions (e.g. for printers). It may be then that in some `BA` files some of the contained images are provided with different resolutions.

The best fitting image according to its resolutions is the first one in file for which horizontal and vertical resolutions are less than the related targetted one and for which the sum of the differences is the smallest.

So, would multiple images fit the criteria, the first fitting one found in the BA file is provided.

Should no image that is contained in the BA file fits the targetted dimensions, the first image in file will be returned - this is the specification of BA format: when no image fits criteria the first one in file is provided.

To get the best fitting image according to its resolutions, first include the next header file in your code.
```
#include "bmp_best_fitting_image.h"
```
Notice: including this header file includes all the needed stuff from the library: the API plus its internal implementation header files.

Then, instantiate the templated class *`bmpl::BMPBestFittingResolutionImage<>`*. This class inherits from the templated class *`bmpl::BMPImage<>`* and, as such, provides the exact same API for error and warnings status, for accessing its image buffer and the like - see *`bmpl::BMPImage<>`*. It is declared this way:
```
namespace bmpl
{
    template<typename PixelT>
    struct BMPBestFittingResolutionImage : public bmpl::BMPImage<PixelT>;
}
```

Four specializations are available, each related to one pixel encoding type (i.e. RGB, RGBA, BGR and BGRA):
```
namespace bmpl
{
    using RGBBMPBestFittingResolutionImage  = BMPBestFittingResolutionImage<bmpl::clr::RGB>;
    using RGBABMPBestFittingResolutionImage = BMPBestFittingResolutionImage<bmpl::clr::RGBA>;
    using BGRBMPBestFittingResolutionImage  = BMPBestFittingResolutionImage<bmpl::clr::BGR>;
    using BGRABMPBestFittingResolutionImage = BMPBestFittingResolutionImage<bmpl::clr::BGRA>;
}
```

### Code example
Next example shows a way to use this.
```
#include <iostream>
#include "bmp_best_fitting_image.h"

// searches for an image with 72 dpi resolution on both horizontal and vertical axis
bmpl::RGBBMPBestFittingResolutionImage best_fitting_image("the/path/to/the/bafile.bmp", 72, 72);

if (best_fitting_image.failed()) {
    std::cout << best_fitting_image.get_error_msg() << std::endl;
}
else {
    for (auto& msg : best_fitting_image.get_warnings_msg())
        std::cout << msg << std::endl;
    _display_(best_fitting_image);  // whatever your display function is
}
```

---
### *`class bmpl::BMPBestFittingResolutionImage`*
Declared as 
```
template<typename PixelT>
struct BMPBestFittingResolutionImage : public bmpl::BMPImage<PixelT>;
```
Reminder: This class inherits from the templated class *`bmpl::BMPImage<>`* and, as such, provides the exact same API for error and warnings status, for accessing its image buffer and the like - see *`bmpl::BMPImage<>`*.

The template argument *`PixelT`* defines the encoding of contained pixels in the filan image. Library **CppBMPLoader** defines four types of pixels:
- BGR, i.e. 3x8-bits components, blue component in lower byte in memory and red component in the higher byte in memory - as specified by `BMP` format or as are internally stored for images in the image processing library **OpenCV**, for instance;
- BGRA, i.e. 4x8-bits components with an alpha channel;
- RGB, i.e. 3x8-bits components, red component in lower byte in memory and blue component in the higher byte in memory;
- RGBA, i.e. 4x8-bits components with an alpha channel.

**CppBMPLoader** defines also four specializations of templated class *`bmpl::BMPBestFittingSizeImage`*:
```
namespace bmpl
{
    using RGBBMPBestFittingResolutionImage  = BMPBestFittingResolutionImage<bmpl::clr::RGB>;
    using RGBABMPBestFittingResolutionImage = BMPBestFittingResolutionImage<bmpl::clr::RGBA>;
    using BGRBMPBestFittingResolutionImage  = BMPBestFittingResolutionImage<bmpl::clr::BGR>;
    using BGRABMPBestFittingResolutionImage = BMPBestFittingResolutionImage<bmpl::clr::BGRA>;
}
```
These are the ones you should use in the code of your application.


### Constructors

---
#### *`BMPBestFittingResolutionImage() noexcept = default;`*
The default empty constructor.  
You may instantiate an empty image with this constructor. Its error status is then set to `bmpl::utils::ErrorCode::NOT_INITIALIZED`. You will then call method *`load_image()`* to load the best fitting image from a `BA` file.

---
#### *`BMPBestFittingResolutionImage(const bmpl::BMPImage<PixelT> image) noexcept;`*
This constructors copies a *`bmpl::BMPImage<>`* with same template argument into this best-fitting-image. It might be that you will never have to use this constructor in your applications, but it gets uses internally in library **CppBMPLoader**.

---
#### *`BMPBestFittingResolutionImage(const std::string& filepath, const std::int32_t target_dpi_x_resolution, const std::int32_t target_dpi_y_resolution, const bool apply_gamma_corection = false, const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK, const bool force_bottom_up = false) noexcept;`*

Arguments:
- *`const std::string& filepath`*  
  The path to the BA file from which the image is to be loaded.

- *`target_dpi_x_resolution`*  
  The targetted horizontal resolution for the returned image, expressed in dots-per-inch (dpi). The loaded image will be the one that is at the nearest of this value and of the targetted vertical resolution value while not above them.

- *`target_dpi_y_resolution`*  
  The targetted vertical resolution for the returned image, expressed in dots-per-inch (dpi). The loaded image will be the one that is at the nearest of this value and of the targetted horizontal resolution value while not above them.

- *`const bool apply_gamma_correction = false`*  
  In some circumstances gamma correction is to be applied on the loaded image. Set this parameter to `true` if you wish to apply gamma correction on the image. If this does not mean anything to you, set it to `false`: **CppBMPLoader** will hold the gamma correction for you when specified in BMP files.  
  Defaults to `false`.

- *`bmpl::clr::ESkippedPixelsMode skipped_mode`*  
  In some circumstances, the BMP format *Run Length* encoding of pixels, which allows for the specification of pixels offsets into the image, leaves some pixels unspecified. This should be an encoding error but it may be encountered in some BMP images. **CppBMPLoader** deals with this and allows one from three default values to be set to missing pixels:  
  *`bmpl::clr::ESkippedPixelsMode::BLACK`* sets missing pixels to black color;  
  *`bmpl::clr::ESkippedPixelsMode::PALETTE_INDEX_0`* sets missing pixels to the color of index 0 in the color palette; or  
  *`bmpl::clr::ESkippedPixelsMode::TRANSPARENCY`* sets the alpha channel of missing pixels as full transparency, if such a channel exists (`RGBA` and `BGRA` pixels).  
  Defaults to *`bmpl::clr::ESkippedPixelsMode::BLACK`*.  

- *`const bool force_bottom_up = false`*  
  **CppBMPLoader** automatically loads images with top line first and bottom line last in buffer. In some circumstances you may wish to load images in the reverse order (which is the default oredring of lines in `BMP` format as well as the one specified by **OpenGL**). Set this argument to `true` if you want to load the image with bottom line first and top line last in the image buffer.  
  Defaults to `false`.

---
#### *`BMPBestFittingResolutionImage(const std::string& filepath, const std::int32_t target_dpi_resolution, const bool apply_gamma_corection = false, const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK, const bool force_bottom_up = false) noexcept;`*
Same as above, but with same resolution specified for both horizontal and vertical axis.

Arguments:
- *`const std::string& filepath`*  
  The path to the BA file from which the image is to be loaded.

- *`target_dpi_resolution`*  
  The targetted horizontal and vertical resolutions for the returned image. The loaded image will be the one that is at the nearest of this value for both horizontal and vertical resolution values while not above them.

- *`const bool apply_gamma_correction = false`*  
  In some circumstances gamma correction is to be applied on the loaded image. Set this parameter to `true` if you wish to apply gamma correction on the image. If this does not mean anything to you, set it to `false`: **CppBMPLoader** will hold the gamma correction for you when specified in BMP files.  
  Defaults to `false`.

- *`bmpl::clr::ESkippedPixelsMode skipped_mode`*  
  In some circumstances, the BMP format *Run Length* encoding of pixels, which allows for the specification of pixels offsets into the image, leaves some pixels unspecified. This should be an encoding error but it may be encountered in some BMP images. **CppBMPLoader** deals with this and allows one from three default values to be set to missing pixels:  
  *`bmpl::clr::ESkippedPixelsMode::BLACK`* sets missing pixels to black color;  
  *`bmpl::clr::ESkippedPixelsMode::PALETTE_INDEX_0`* sets missing pixels to the color of index 0 in the color palette; or  
  *`bmpl::clr::ESkippedPixelsMode::TRANSPARENCY`* sets the alpha channel of missing pixels as full transparency, if such a channel exists (`RGBA` and `BGRA` pixels).  
  Defaults to *`bmpl::clr::ESkippedPixelsMode::BLACK`*.  

- *`const bool force_bottom_up = false`*  
  **CppBMPLoader** automatically loads images with top line first and bottom line last in buffer. In some circumstances you may wish to load images in the reverse order (which is the default oredring of lines in `BMP` format as well as the one specified by **OpenGL**). Set this argument to `true` if you want to load the image with bottom line first and top line last in the image buffer.  
  Defaults to `false`.

---
#### *`BMPBestFittingResolutionImage(const BMPBestFittingSizeImage&) noexcept = default;`*
The default copy constructor.

#### *`BMPBestFittingResolutionImage(BMPBestFittingSizeImage&&) noexcept = default;`*
The default move constructor.


### Operators

---
#### *`BMPBestFittingResolutionImage& operator=(const BMPBestFittingResolutionImage&) noexcept = default;`*
The default copy assignement operator.

#### *`BMPBestFittingResolutionImage& operator=(BMPBestFittingResolutionImage&&) noexcept = default;`*
The default move assignment operator.

---
#### *`const bool operator! () const noexcept;`*
Returns `true` when some arror occured while loading the image was faulty. Returns `false` otherwise (i.e. no error).  
Seel also `is_ok()`, `failed()` and `get_error_msg()`.  
Notice: this is a wrapper to method `failed()`.  
Inherited from class *`bmpl::BMPImage<>`*.


### Methods

---
#### *`const bool load_image(const std::string& filepath, const std::int32_t target_dpi_x_resolution, const std::int32_t target_dpi_y_resolution, const bool apply_gamma_correction = false, const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK, const bool force_bottom_up = false) noexcept;`*
Loads an image from a specified `BA` file and sets its error status and warnings list.  
Returns `true` if loading was successfull or `false` otherwise.  
You will mainly call this method to load BMP images from BA files after having instantiating a *`BMPBestFittingResolutionImage`* with the default empty constructor, or to load a new image from another `BA` file.

Arguments:
- *`const std::string& filepath`*  
  The path to the BMP file from which the image is to be loaded.

- *`target_dpi_x_resolution`*  
  The targetted horizontal resolution for the returned image. The loaded image will be the one that is at the nearest of this value and the targetted vertical resolution value while not above them.

- *`target_dpi_y_resolution`*  
  The targetted vertical resolution for the returned image. The loaded image will be the one that is at the nearest of this value and the targetted horizontal resolution value while not above them.

- *`const bool apply_gamma_correction = false`*  
  In some circumstances gamma correction is to be applied on the loaded image. Set this parameter to `true` if you wish to apply gamma correction on the image. If this does not mean anything to you, set it to `false`: **CppBMPLoader** will hold the gamma correction for you when specified in BMP files.  
  Defaults to `false`.

- *`bmpl::clr::ESkippedPixelsMode skipped_mode`*  
  In some circumstances, the BMP format *Run Length* encoding of pixels, which allows for the specification of pixels offsets into the image, leaves some pixels unspecified. This should be an encoding error but it may be encountered in some BMP images. **CppBMPLoader** deals with this and allows one from three default values to be set to missing pixels:  
  *`bmpl::clr::ESkippedPixelsMode::BLACK`* sets missing pixels to black color;  
  *`bmpl::clr::ESkippedPixelsMode::PALETTE_INDEX_0`* sets missing pixels to the color of index 0 in the color palette; or  
  *`bmpl::clr::ESkippedPixelsMode::TRANSPARENCY`* sets the alpha channel of missing pixels as full transparency, if such a channel exists (`RGBA` and `BGRA` pixels).  
  Defaults to *`bmpl::clr::ESkippedPixelsMode::BLACK`*.  

- *`const bool force_bottom_up = false`*  
  **CppBMPLoader** automatically loads images with top line first and bottom line last in buffer. In some circumstances you may wish to load images in the reverse order (which is the default oredring of lines in `BMP` format as well as the one specified by **OpenGL**). Set this argument to `true` if you want to load the image with bottom line first and top line last in the image buffer.  
  Defaults to `false`.

---
#### *`const bool load_image(const std::string& filepath, const std::int32_t target_dpi_resolution, cconst bool apply_gamma_correction = false, const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK, const bool force_bottom_up = false) noexcept;`*
Same as above, but with same resolution specified for both horizontal and vertical axis.  
Loads an image from a specified `BA` file and sets its error status and warnings list.  
Returns `true` if loading was successfull or `false` otherwise.  
You will mainly call this method to load BMP images from BA files after having instantiating a *`BMPBestFittingResolutionImage`* with the default empty constructor, or to load a new image from another `BA` file.

Arguments:
- *`const std::string& filepath`*  
  The path to the BMP file from which the image is to be loaded.

- *`target_dpi_resolution`*  
  The targetted horizontal and vertical resolutions for the returned image. The loaded image will be the one that is at the nearest of this value for both horizontal and vertical resolution values while not above them.

- *`const bool apply_gamma_correction = false`*  
  In some circumstances gamma correction is to be applied on the loaded image. Set this parameter to `true` if you wish to apply gamma correction on the image. If this does not mean anything to you, set it to `false`: **CppBMPLoader** will hold the gamma correction for you when specified in BMP files.  
  Defaults to `false`.

- *`bmpl::clr::ESkippedPixelsMode skipped_mode`*  
  In some circumstances, the BMP format *Run Length* encoding of pixels, which allows for the specification of pixels offsets into the image, leaves some pixels unspecified. This should be an encoding error but it may be encountered in some BMP images. **CppBMPLoader** deals with this and allows one from three default values to be set to missing pixels:  
  *`bmpl::clr::ESkippedPixelsMode::BLACK`* sets missing pixels to black color;  
  *`bmpl::clr::ESkippedPixelsMode::PALETTE_INDEX_0`* sets missing pixels to the color of index 0 in the color palette; or  
  *`bmpl::clr::ESkippedPixelsMode::TRANSPARENCY`* sets the alpha channel of missing pixels as full transparency, if such a channel exists (`RGBA` and `BGRA` pixels).  
  Defaults to *`bmpl::clr::ESkippedPixelsMode::BLACK`*.  

- *`const bool force_bottom_up = false`*  
  **CppBMPLoader** automatically loads images with top line first and bottom line last in buffer. In some circumstances you may wish to load images in the reverse order (which is the default oredring of lines in `BMP` format as well as the one specified by **OpenGL**). Set this argument to `true` if you want to load the image with bottom line first and top line last in the image buffer.  
  Defaults to `false`.

---
#### *`PixelT* get_content_ptr() noexcept;`*
Returns a pointer to the very first pixel of the image buffer.  
You will call this to apply any processing of your own on the image content or, more often, to display its content.  
Inherited from class *`bmpl::BMPImage<>`*.

---
#### *`const bool is_ok() const noexcept;`*
Returns `true` when the image has been successfully loaded, or `false` if no image has been loaded or when loading failed.  
See also *`failed()`*, *`operator !`* and *`get_error_msg()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`const bool failed() const noexcept;`*
Returns `true` when some arror occured or while the image is not yet initialized. Returns `false` otherwise (i.e. no error).  
See also *`is_ok()`*, *`operator !`* and *`get_error_msg()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`const std::string get_error_msg() const noexcept;`*
Returns a string containing English text related to the error encountered, if any.  
See also *`is_ok()`*, *`operator !`* and *`failed()`*.  
Inherited from class *`bmpl::BMPImage<>`*.  

---
#### *`const bool has_warnings() const noexcept`*
Returns `true` is some warnings have been detected while loading the image from file, or `false` otherwise.  
See also *`get_warnings_msg()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`std::vector<std::string> get_warnings_msg() const noexcept;`*
Returns a vector of strings, each containing English text related to a warning detected while loading the image from file. Multiple same warnings are returned only once each in this list.  
See also *`has_warnings()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

---
#### *`const std::uint32_t get_height() const noexcept;`*
Returns the height of the image (i.e. lines count) once it has been loaded, or zero otherwise.  
See also *`get_width()`* and *`image_size()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`const std::uint32_t get_width() const noexcept;`*
Returns the width of the image (i.e. lines count) once it has been loaded, or zero otherwise.  
See also *`get_height()`* and *`image_size()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`const std::uint64_t image_size() const noexcept;`*
Returns the size (i.e. overall pixels count) of the image once it has been loaded, or zero otherwise.  
See also *`get_height()`* and *`get_width()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

---
#### *`const std::string get_filepath() const noexcept;`*
Returns the filepath string associated with the image, i.e. the path to the loaded BMP file.  
Inherited from class *`bmpl::BMPImage<>`*.

---
#### *`const std::uint32_t get_colors_count() const noexcept;`*
Returns the number of colors encoded in the color palette as specified in the BMP file. May be zero if no color palette is present in the BMP file.  
Inherited from class *`bmpl::BMPImage<>`*.

---
#### *`const std::int32_t get_device_x_resolution_dpi() const noexcept;`*
Returns the resolution along the horizontal axis used to encode the image in the BMP file. This value is provided by **CppBMPLoader** in dots-per-inch units (it will mostly get `72` dpi as a value), while the BMP format specifies resolutions in pixels-per-meter units (mostly `2835` as its value).  
See also *`get_device_y_resolution_dpi()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`const std::int32_t get_device_y_resolution_dpi() const noexcept;`*
Returns the resolution along the vertical axis used to encode the image in the BMP file. This value is provided by **CppBMPLoader** in dots-per-inch units (it will mostly get `72` dpi as a value), while the BMP format specifies resolutions in pixels-per-meter units (mostly `2835` as its value).  
See also *`get_device_x_resolution_dpi()`*.  
Inherited from class *`bmpl::BMPImage<>`*.



---
## How to load the image that best fits specified dimensions, then resolutions, then colors count from a `BA` file

As seen above, images contained in a `BA` file can be selected according to their colors count, dimensions or resolutions. A dedicated class helps selecting the best image in file according to all of these characteristics.

The implemented algorithm is a little bit restrictive but simple. The provided image from the `BA` file will be the one that best fits the dimensions criteria. If many images fit this first criteria, the provided one will be the one that best fits the resolutions criteria. If multiple images still fit the two first criterias, the provided one will be the one that best fits the colors count criteria. Finally, if many images still fit all the criterias, the first one of them in the `BA` file will be provided.

Should no image that is contained in the BA file fits the targetted dimensions, the first image in file will be returned - this is the specification of BA format: when no image fits criteria the first one in file is provided.

To get the best fitting image according to those criterias, first include the next header file in your code.
```
#include "bmp_best_fitting_image.h"
```
Notice: including this header file includes all the needed stuff from the library: the API plus its internal implementation header files.

Then, instantiate the templated class *`bmpl::BMPBestFittingImage<>`*. This class inherits from the templated class *`bmpl::BMPImage<>`* and, as such, provides the exact same API for error and warnings status, for accessiing its image buffer and the like - see *`bmpl::BMPImage<>`*. It is declared this way:
```
namespace bmpl
{
    template<typename PixelT>
    struct BMPBestFittingImage : public bmpl::BMPImage<PixelT>;
}
```

Four specializations are available, each related to one pixel encoding type (i.e. RGB, RGBA, BGR and BGRA):
```
namespace bmpl
{
    using RGBBMPBestFittingImage  = BMPBestFittingImage<bmpl::clr::RGB>;
    using RGBABMPBestFittingImage = BMPBestFittingImage<bmpl::clr::RGBA>;
    using BGRBMPBestFittingImage  = BMPBestFittingImage<bmpl::clr::BGR>;
    using BGRABMPBestFittingImage = BMPBestFittingImage<bmpl::clr::BGRA>;
}
```

### Code example
Next example shows a way to use this.
```
#include <iostream>
#include "bmp_best_fitting_image.h"

// searches for an image with multilpe criterias: 800x640 dimensions, 8 bits per pixel, 72 dpi resolutions
bmpl::RGBBMPBestFittingImage best_fitting_image("your/filepath/to/the/bafile.bmp", 800, 640, 8, 72, 72);

if (best_fitting_image.failed()) {
    std::cout << best_fitting_image.get_error_msg() << std::endl;
}
else {
    for (auto& msg : best_fitting_image.get_warnings_msg())
        std::cout << msg << std::endl;
    _display_(best_fitting_image);  // whatever your display function is
}
```

---
### *`class bmpl::BMPBestFittingImage`*
Declared as 
```
template<typename PixelT>
struct BMPBestFittingImage : public bmpl::BMPImage<PixelT>;
```
Reminder: This class inherits from the templated class *`bmpl::BMPImage<>`* and, as such, provides the exact same API for error and warnings status, for access to its image buffer and the like - see *`bmpl::BMPImage<>`*.

The template argument *`PixelT`* defines the encoding of contained pixels in the filan image. Library **CppBMPLoader** defines four types of pixels:
- BGR, i.e. 3x8-bits components, blue component in lower byte in memory and red component in the higher byte in memory - as specified by `BMP` format or as are internally stored for images in the image processing library **OpenCV**, for instance;
- BGRA, i.e. 4x8-bits components with an alpha channel;
- RGB, i.e. 3x8-bits components, red component in lower byte in memory and blue component in the higher byte in memory;
- RGBA, i.e. 4x8-bits components with an alpha channel.

**CppBMPLoader** defines also four specializations of templated class *`bmpl::BMPBestFittingImage`*:
```
namespace bmpl
{
    using RGBBMPBestFittingImage  = BMPBestFittingImage<bmpl::clr::RGB>;
    using RGBABMPBestFittingImage = BMPBestFittingImage<bmpl::clr::RGBA>;
    using BGRBMPBestFittingImage  = BMPBestFittingImage<bmpl::clr::BGR>;
    using BGRABMPBestFittingImage = BMPBestFittingImage<bmpl::clr::BGRA>;
}
```
These are the ones you should use in the code of your application.


### Constructors

---
#### *`BMPBestFittingImage() noexcept = default;`*
The default empty constructor.  
You may instantiate an empty image with this constructor. Its error status is then be set to `bmpl::utils::ErrorCode::NOT_INITIALIZED`. You will then call method *`load_image()`* to load the best fitting image from a BA file.

---
#### *`BMPBestFittingImage(const bmpl::BMPImage<PixelT> image) noexcept;`*
This constructors copies a *`bmpl::BMPImage<>`* with same template argument into this best-fitting-image. It might be that you will never have to use this constructor in your applications, but it gets uses internally in library **CppBMPLoader**.

---
#### *`BMPBestFittingImage(const std::string& filepath, const std::uint32_t target_width, const std::uint32_t target_height, const std::uint32_t target_bits_per_pixel, const std::int32_t target_dpi_x_resolution, std::int32_t target_dpi_y_resolution, const bool apply_gamma_corection = false, const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK, const bool force_bottom_up = false) noexcept;`*

Arguments:
- *`const std::string& filepath`*  
  The path to the BA file from which the image is to be loaded.
  
- *`target_width`*  
  The targetted width for the returned image.

- *`target_height`*  
  The targetted height for the returned image.

- *`target_bits_per_pixel`*  
  The targetted bits per pixels that encode colors.

- *`target_dpi_x_resolution`*  
  The targetted horizontal resolution for the returned image.

- *`target_dpi_y_resolution`*  
  The targetted vertical resolution for the returned image.

- *`const bool apply_gamma_correction = false`*  
  In some circumstances gamma correction is to be applied on the loaded image. Set this parameter to `true` if you wish to apply gamma correction on the image. If this does not mean anything to you, set it to `false`: **CppBMPLoader** will hold the gamma correction for you when specified in BMP files.  
  Defaults to `false`.

- *`bmpl::clr::ESkippedPixelsMode skipped_mode`*  
  In some circumstances, the BMP format *Run Length* encoding of pixels, which allows for the specification of pixels offsets into the image, leaves some pixels unspecified. This should be an encoding error but it may be encountered in some BMP images. **CppBMPLoader** deals with this and allows one from three default values to be set to missing pixels:  
  *`bmpl::clr::ESkippedPixelsMode::BLACK`* sets missing pixels to black color;  
  *`bmpl::clr::ESkippedPixelsMode::PALETTE_INDEX_0`* sets missing pixels to the color of index 0 in the color palette; or  
  *`bmpl::clr::ESkippedPixelsMode::TRANSPARENCY`* sets the alpha channel of missing pixels as full transparency, if such a channel exists (`RGBA` and `BGRA` pixels).  
  Defaults to *`bmpl::clr::ESkippedPixelsMode::BLACK`*.  

- *`const bool force_bottom_up = false`*  
  **CppBMPLoader** automatically loads images with top line first and bottom line last in buffer. In some circumstances you may wish to load images in the reverse order (which is the default oredring of lines in `BMP` format as well as the one specified by **OpenGL**). Set this argument to `true` if you want to load the image with bottom line first and top line last in the image buffer.  
  Defaults to `false`.

---
#### *`BMPBestFittingImage(const std::string& filepath, const std::uint32_t target_width, const std::uint32_t target_height, const std::uint32_t target_bits_per_pixel, const std::int32_t target_dpi_resolution, const bool apply_gamma_corection = false, const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK, const bool force_bottom_up = false) noexcept;`*
Same as above, but with same resolution specified for both horizontal and vertical axis.

Arguments:
- *`const std::string& filepath`*  
  The path to the BA file from which the image is to be loaded.
  
- *`target_width`*  
  The targetted width for the returned image.

- *`target_height`*  
  The targetted height for the returned image.

- *`target_bits_per_pixel`*  
  The targetted bits per pixels that encode colors.

- *`target_dpi_resolution`*  
  The targetted horizontal and vertical resolutions for the returned image. The loaded image will be the one that is at the nearest of this value for both horizontal and vertical resolution values while not above them.

- *`const bool apply_gamma_correction = false`*  
  In some circumstances gamma correction is to be applied on the loaded image. Set this parameter to `true` if you wish to apply gamma correction on the image. If this does not mean anything to you, set it to `false`: **CppBMPLoader** will hold the gamma correction for you when specified in BMP files.  
  Defaults to `false`.

- *`bmpl::clr::ESkippedPixelsMode skipped_mode`*  
  In some circumstances, the BMP format *Run Length* encoding of pixels, which allows for the specification of pixels offsets into the image, leaves some pixels unspecified. This should be an encoding error but it may be encountered in some BMP images. **CppBMPLoader** deals with this and allows one from three default values to be set to missing pixels:  
  *`bmpl::clr::ESkippedPixelsMode::BLACK`* sets missing pixels to black color;  
  *`bmpl::clr::ESkippedPixelsMode::PALETTE_INDEX_0`* sets missing pixels to the color of index 0 in the color palette; or  
  *`bmpl::clr::ESkippedPixelsMode::TRANSPARENCY`* sets the alpha channel of missing pixels as full transparency, if such a channel exists (`RGBA` and `BGRA` pixels).  
  Defaults to *`bmpl::clr::ESkippedPixelsMode::BLACK`*.  

- *`const bool force_bottom_up = false`*  
  **CppBMPLoader** automatically loads images with top line first and bottom line last in buffer. In some circumstances you may wish to load images in the reverse order (which is the default oredring of lines in `BMP` format as well as the one specified by **OpenGL**). Set this argument to `true` if you want to load the image with bottom line first and top line last in the image buffer.  
  Defaults to `false`.

---
#### *`BMPBestFittingImage(const BMPBestFittingImage&) noexcept = default;`*
The default copy constructor.

#### *`BMPBestFittingImage(BMPBestFittingImage&&) noexcept = default;`*
The default move constructor.


### Operators

---
#### *`BMPBestFittingImage& operator=(const BMPBestFittingImage&) noexcept = default;`*
The default copy assignement operator.

#### *`BMPBestFittingImage& operator=(BMPBestFittingImage&&) noexcept = default;`*
The default move assignment operator.

---
#### *`const bool operator! () const noexcept;`*
Returns `true` when some arror occured while loading the image was faulty. Returns `false` otherwise (i.e. no error).  
Seel also `is_ok()`, `failed()` and `get_error_msg()`.  
Notice: this is a wrapper to method `failed()`.  
Inherited from class *`bmpl::BMPImage<>`*.


### Methods

---
#### *`const bool load_image(const std::string& filepath, const std::uint32_t target_width, const std::uint32_t target_height, const std::uint32_t target_bits_per_pixel, const std::int32_t target_dpi_x_resolution, const std::int32_t target_dpi_y_resolution, const bool apply_gamma_correction = false, const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK, const bool force_bottom_up = false) noexcept;`*
Loads an image from a specified `BA` file and sets its error status and warnings list.  
Returns `true` if loading was successfull or `false` otherwise.  
You will mainly call this method to load BMP images from BA files after having instantiating a *`BMPBestFittingResolutionImage`* with the default empty constructor, or to load a new image from another `BA` file.

Arguments:
- *`const std::string& filepath`*  
  The path to the BMP file from which the image is to be loaded.
  
- *`target_width`*  
  The targetted width for the returned image.

- *`target_height`*  
  The targetted height for the returned image.

- *`target_bits_per_pixel`*  
  The targetted bits per pixels that encode colors.

- *`target_dpi_x_resolution`*  
  The targetted horizontal resolution for the returned image. The loaded image will be the one that is at the nearest of this value and the targetted vertical resolution value while not above them.

- *`target_dpi_y_resolution`*  
  The targetted vertical resolution for the returned image. The loaded image will be the one that is at the nearest of this value and the targetted horizontal resolution value while not above them.

- *`const bool apply_gamma_correction = false`*  
  In some circumstances gamma correction is to be applied on the loaded image. Set this parameter to `true` if you wish to apply gamma correction on the image. If this does not mean anything to you, set it to `false`: **CppBMPLoader** will hold the gamma correction for you when specified in BMP files.  
  Defaults to `false`.

- *`bmpl::clr::ESkippedPixelsMode skipped_mode`*  
  In some circumstances, the BMP format *Run Length* encoding of pixels, which allows for the specification of pixels offsets into the image, leaves some pixels unspecified. This should be an encoding error but it may be encountered in some BMP images. **CppBMPLoader** deals with this and allows one from three default values to be set to missing pixels:  
  *`bmpl::clr::ESkippedPixelsMode::BLACK`* sets missing pixels to black color;  
  *`bmpl::clr::ESkippedPixelsMode::PALETTE_INDEX_0`* sets missing pixels to the color of index 0 in the color palette; or  
  *`bmpl::clr::ESkippedPixelsMode::TRANSPARENCY`* sets the alpha channel of missing pixels as full transparency, if such a channel exists (`RGBA` and `BGRA` pixels).  
  Defaults to *`bmpl::clr::ESkippedPixelsMode::BLACK`*.  

- *`const bool force_bottom_up = false`*  
  **CppBMPLoader** automatically loads images with top line first and bottom line last in buffer. In some circumstances you may wish to load images in the reverse order (which is the default oredring of lines in `BMP` format as well as the one specified by **OpenGL**). Set this argument to `true` if you want to load the image with bottom line first and top line last in the image buffer.  
  Defaults to `false`.

---
#### *`const bool load_image(const std::string& filepath, const std::uint32_t target_width, const std::uint32_t target_height, const std::uint32_t target_bits_per_pixel, const std::int32_t target_dpi_resolution, cconst bool apply_gamma_correction = false, const bmpl::clr::ESkippedPixelsMode skipped_mode = bmpl::clr::ESkippedPixelsMode::BLACK, const bool force_bottom_up = false) noexcept;`*
Same as above, but with same resolution specified for both horizontal and vertical axis.  
Loads an image from a specified `BA` file and sets its error status and warnings list.  
Returns `true` if loading was successfull or `false` otherwise.  
You will mainly call this method to load BMP images from BA files after having instantiating a *`BMPBestFittingResolutionImage`* with the default empty constructor, or to load a new image from another `BA` file.

Arguments:
- *`const std::string& filepath`*  
  The path to the BMP file from which the image is to be loaded.
  
- *`target_width`*  
  The targetted width for the returned image.

- *`target_height`*  
  The targetted height for the returned image.

- *`target_bits_per_pixel`*  
  The targetted bits per pixels that encode colors.

- *`target_dpi_resolution`*  
  The targetted horizontal and vertical resolutions for the returned image. The loaded image will be the one that is at the nearest of this value for both horizontal and vertical resolution values while not above them.

- *`const bool apply_gamma_correction = false`*  
  In some circumstances gamma correction is to be applied on the loaded image. Set this parameter to `true` if you wish to apply gamma correction on the image. If this does not mean anything to you, set it to `false`: **CppBMPLoader** will hold the gamma correction for you when specified in BMP files.  
  Defaults to `false`.

- *`bmpl::clr::ESkippedPixelsMode skipped_mode`*  
  In some circumstances, the BMP format *Run Length* encoding of pixels, which allows for the specification of pixels offsets into the image, leaves some pixels unspecified. This should be an encoding error but it may be encountered in some BMP images. **CppBMPLoader** deals with this and allows one from three default values to be set to missing pixels:  
  *`bmpl::clr::ESkippedPixelsMode::BLACK`* sets missing pixels to black color;  
  *`bmpl::clr::ESkippedPixelsMode::PALETTE_INDEX_0`* sets missing pixels to the color of index 0 in the color palette; or  
  *`bmpl::clr::ESkippedPixelsMode::TRANSPARENCY`* sets the alpha channel of missing pixels as full transparency, if such a channel exists (`RGBA` and `BGRA` pixels).  
  Defaults to *`bmpl::clr::ESkippedPixelsMode::BLACK`*.  

- *`const bool force_bottom_up = false`*  
  **CppBMPLoader** automatically loads images with top line first and bottom line last in buffer. In some circumstances you may wish to load images in the reverse order (which is the default oredring of lines in `BMP` format as well as the one specified by **OpenGL**). Set this argument to `true` if you want to load the image with bottom line first and top line last in the image buffer.  
  Defaults to `false`.

---
#### *`PixelT* get_content_ptr() noexcept;`*
Returns a pointer to the very first pixel of the image buffer.  
You will call this to apply any processing of your own on the image content or, more often, to display its content.  
Inherited from class *`bmpl::BMPImage<>`*.

---
#### *`const bool is_ok() const noexcept;`*
Returns `true` when the image has been successfully loaded, or `false` if no image has been loaded or when loading failed.  
See also *`failed()`*, *`operator !`* and *`get_error_msg()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`const bool failed() const noexcept;`*
Returns `true` when some arror occured or while the image is not yet initialized. Returns `false` otherwise (i.e. no error).  
See also *`is_ok()`*, *`operator !`* and *`get_error_msg()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`const std::string get_error_msg() const noexcept;`*
Returns a string containing English text related to the error encountered, if any.  
See also *`is_ok()`*, *`operator !`* and *`failed()`*.  
Inherited from class *`bmpl::BMPImage<>`*.  

---
#### *`const bool has_warnings() const noexcept`*
Returns `true` is some warnings have been detected while loading the image from file, or `false` otherwise.  
See also *`get_warnings_msg()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`std::vector<std::string> get_warnings_msg() const noexcept;`*
Returns a vector of strings, each containing English text related to a warning detected while loading the image from file. Multiple same warnings are returned only once each in this list.  
See also *`has_warnings()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

---
#### *`const std::uint32_t get_height() const noexcept;`*
Returns the height of the image (i.e. lines count) once it has been loaded, or zero otherwise.  
See also *`get_width()`* and *`image_size()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`const std::uint32_t get_width() const noexcept;`*
Returns the width of the image (i.e. lines count) once it has been loaded, or zero otherwise.  
See also *`get_height()`* and *`image_size()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`const std::uint64_t image_size() const noexcept;`*
Returns the size (i.e. overall pixels count) of the image once it has been loaded, or zero otherwise.  
See also *`get_height()`* and *`get_width()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

---
#### *`const std::string get_filepath() const noexcept;`*
Returns the filepath string associated with the image, i.e. the path to the loaded BMP file.  
Inherited from class *`bmpl::BMPImage<>`*.

---
#### *`const std::uint32_t get_colors_count() const noexcept;`*
Returns the number of colors encoded in the color palette as specified in the BMP file. May be zero if no color palette is present in the BMP file.  
Inherited from class *`bmpl::BMPImage<>`*.

---
#### *`const std::int32_t get_device_x_resolution_dpi() const noexcept;`*
Returns the resolution along the horizontal axis used to encode the image in the BMP file. This value is provided by **CppBMPLoader** in dots-per-inch units (it will mostly get `72` dpi as a value), while the BMP format specifies resolutions in pixels-per-meter units (mostly `2835` as its value).  
See also *`get_device_y_resolution_dpi()`*.  
Inherited from class *`bmpl::BMPImage<>`*.

#### *`const std::int32_t get_device_y_resolution_dpi() const noexcept;`*
Returns the resolution along the vertical axis used to encode the image in the BMP file. This value is provided by **CppBMPLoader** in dots-per-inch units (it will mostly get `72` dpi as a value), while the BMP format specifies resolutions in pixels-per-meter units (mostly `2835` as its value).  
See also *`get_device_x_resolution_dpi()`*.  
Inherited from class *`bmpl::BMPImage<>`*.



---
## The Error Codes and related Messages
Let's just copy the actual content of the related code to get an overall view on all error codes and associated English messages. This code can be found in file [`utils/errors.cpp`](./cpp-14/utils/errors.cpp) (same content for c++14 and c++20 versions of the library). This code is embedded in namespace `bmpl::utils`:
```
const std::string error_msg(const ErrorCode err_code) noexcept
{
    switch (err_code)
    {
    case ErrorCode::NO_ERROR:
        return "no error.";
    case ErrorCode::BAD_BITS_PER_PIXEL_VALUE:
        return "bad bits count per pixel value.";
    case ErrorCode::BAD_COLOR_SPACE_TYPE:
        return "bad color space type specification.";
    case ErrorCode::BAD_DEFAULT_VALUE:
        return "bad default value.";
    case ErrorCode::BAD_COLORMAP_ENCODING:
        return "bad encoding for colors map.";
    case ErrorCode::BAD_FILE_HEADER:
        return "bad encoding of file header.";
    case ErrorCode::BAD_INFO_HEADER:
        return "bad encoding of info header.";
    case ErrorCode::BAD_INTENT_VALUE:
        return "bad intent value.";
    case ErrorCode::BAD_PROFILE_DATA_OFFSET:
        return "bad specified offset for data pofile.";
    case ErrorCode::BAD_PROFILE_SIZE:
        return "bad specified size for data profile.";
    case ErrorCode::BMP_BAD_ENCODING:
        return "bad BMP format encoding.";
    case ErrorCode::BMP_LOADER_INSTANTIATION_FAILED:
        return "no BMP Loader can be instantiated, maybe because of memory resource lacking.";
    case ErrorCode::BUFFER_OVERFLOW:
        return "incoherent run length encoding creates a buffer overflow";
    case ErrorCode::CORRUPTED_BMP_FILE:
        return "corrupted BMP file.";
    case ErrorCode::END_OF_BA_HEADERS_LIST:
        return "no more BMP image to be loaded from file.";
    case ErrorCode::END_OF_FILE:
        return "encountered unexpected end-of-file.";
    case ErrorCode::ERRONEOUS_BITMAP_OFFSET:
        return "the offset of the bitmap as specified in the file header appears to be invalid.";
    case ErrorCode::FILE_NOT_FOUND:
        return "file not found.";
    case ErrorCode::FORBIDDEN_BOTTOM_UP_ORIENTATION:
        return "bottom-up image orientation is forbidden with JPEG and PNG embedded encodings.";
    case ErrorCode::INCOHERENT_BITS_PER_PIXEL_VALUE:
        return "incoherent number of bits per pixel with specified compression mode.";
    case ErrorCode::INCOHERENT_BMP_LOADER_IMPLEMENTATION:
        return "incoherent implementation of Cpp BMP Loader - this is a program error.";
    case ErrorCode::INCOHERENT_COMPRESSION_MODE:
        return "forbidden compression mode (1 or 2) associated with bitfield masks encoding";
    case ErrorCode::INCOHERENT_DELTA_MODE_VALUES:
        return "incoherent delta mode values - creates buffers overflow.";
    case ErrorCode::INCOHERENT_IMAGE_DIMENSIONS:
        return "some image dimension as indicated in header is incoherent - maybe too big.";
    case ErrorCode::INCOHERENT_RUN_LENGTH_ENCODING:
        return "incoherent Run Length Encoding control value.";
    case ErrorCode::INPUT_OPERATION_FAILED:
        return "some imput operation failed on file.";
    case ErrorCode::INVALID_BA_NEXT_OFFSET_VALUE:
        return "an invalid value for field 'next offset' has been detected in a BA file header.";
    case ErrorCode::INVALID_BITMAP_OFFSET:
        return "the specified bitmap offset in file is invalid - gets a bigger value than the file size.";
    case ErrorCode::INVALID_DEVICE_RESOLUTION:
        return "some device resolution is invalid (maybe negative, for instance).";
    case ErrorCode::INVALID_HEADER_SIZE:
        return "the specified size for the info header is invalid.";
    case ErrorCode::INVALID_IMAGE_DIMENSIONS:
        return "some dimension of image is set to zero.";
    case ErrorCode::INVALID_NEXT_OFFSET:
        return "a specified offset to the next array header has been detected as invalid - too small value.";
    case ErrorCode::INVALID_NEXT_OFFSET_TOO_BIG:
        return "a specified offset to the next array header has been detected as invalid - too big value.";
    case ErrorCode::INVALID_OS2_BITMAP_RECORDING:
        return "a non-zero recording mode in OS/2 bitmap has been detected; this is invalid.";
    case ErrorCode::INVALID_PROFILE_DATA_OFFSET:
        return "the specified offset for profile data is invalid.";
    case ErrorCode::IRRECOVERABLE_STREAM_ERROR:
        return "encountered some irrecoverable file streaming error.";
    case ErrorCode::MISSING_PROFILE_DATA:
        return "profile data is missing.";
    case ErrorCode::MISSING_PROFILE_DATA_OFFSET:
        return "profile data offset is missing.";
    case ErrorCode::NEGATIVE_WIDTH:
        return "info header specifies a negative width for final image, which is forbidden.";
    case ErrorCode::NOT_BMP_ENCODING:
        return "file is not a BMP file.";
    case ErrorCode::NOT_BITMAP_ARRAY_FILE_HEADER:
        return "a bad type for a bitmap array file header has been detected.";
    case ErrorCode::NOT_INITIALIZED:
        return "file is currently not initialized.";
    case ErrorCode::NOT_WINDOWS_BMP:
        return "file is not a Windows BMP file.";
    case ErrorCode::NOT_YET_IMPLEMENTED_HUFFMAN_1D_DECODING:
        return "Library CppBMPLoader does not yet implement embedded Huffman-1D decoding.";
    case ErrorCode::NOT_YET_IMPLEMENTED_JPEG_DECODING:
        return "Library CppBMPLoader does not yet implement embedded JPEG decoding.";
    case ErrorCode::NOT_YET_IMPLEMENTED_PNG_DECODING:
        return "Library CppBMPLoader does not yet implement embedded PNG decoding'.";
    case ErrorCode::OVERLAPPING_BITFIELD_MASKS:
        return "some bitfield masks are overlapping.";
    case ErrorCode::RLE_INPUT_OPERATION_FAILED:
        return "input operation on Run Length Encoded bitmap has failed.";
    case ErrorCode::TOO_BIG_BITS_PER_PIXEL_VALUE:
        return "too big value for bits count per pixel.";
    case ErrorCode::UNABLE_TO_CREATE_BITMAP_LOADER:
        return "unable to internally create a bitmap loader - maybe ran out of memory.";
    default:
        return "unkown error...";
    }
}
```



---
## The Warnings Codes and related Messages
Let's just copy the actual content of the related code to get an overall view on all warnings codes and associated English messages. This code can be found in file [`utils/warnings.cpp`](./cpp-14/utils/warnings.cpp) (same content for c++14 and c++20 versions of the library). This code is embedded in namespace `bmpl::utils`:
```
const std::string warning_msg(const WarningCode warn_code) noexcept
{
    switch (warn_code)
    {
    case WarningCode::ALPHA_16_BITS_IMAGE:
        return "an alpha channel is specified for this 16-bits pixels bitmap.";
    case WarningCode::BAD_BITMAP_SIZE_IN_HEADER:
        return "header indicates a bad bitmap size.";
    case WarningCode::BAD_FILE_SIZE_IN_HEADER:
        return "the file size indicated in header is not the actual size of this file.";
    case WarningCode::BAD_HALFTONING_MODE_VALUE:
        return "an invalid value for the halftoning algorithm used with the bitmap data has been detected; CppBMPLoader decodes it nevertheless.";
    case WarningCode::BAD_IMPORTANT_COLORS_COUNT:
        return "the 'important colors' count appears to be greater than the defined colors count.";
    case WarningCode::BAD_PALETTE_INDICES:
        return "one or many palette indices used for pixels are not present in color palette - index 0 used instead.";
    case WarningCode::BAD_PALETTE_SIZE_IN_HEADER:
        return "the info header indicates a bad color palette size - bigger than expected, for instance.";
    case WarningCode::BAD_PLANES_VALUE:
        return "the file header indicates a planes count value different from 1, which is forbidden; CppBMPLibrary decodes this image nevertheless.";
    case WarningCode::BAD_SCANLINE_BYTES_WIDTH:
        return "the specified width for scan lines is incoherent with the expected true width; CppBMPLibrary tries to decode this image nevertheless.";
    case WarningCode::BITFIELDS_AND_PALETT:
        return "a color palette is encoded while bitfields masks are defined.";
    case WarningCode::CORRECTED_GAMMA_BLUE:
        return "strange gamma correction value on blue component, forced to 2.2.";
    case WarningCode::CORRECTED_GAMMA_GREEN:
        return "strange gamma correction value on green component, forced to 2.2.";
    case WarningCode::CORRECTED_GAMMA_RED:
        return "strange gamma correction value on red component, forced to 2.2.";
    case WarningCode::DELTA_MODE_MAY_OVERFLOW:
        return "some value specified in RLE delta mode may cause buffers overflow.";
    case WarningCode::EMBEDDED_PROFILE_NOT_IMPLEMENTED:
        return "ICC embedded profiles are not implemented in CppBMPLoader library which uses plain coded colors instead.";
    case WarningCode::FORBIDDEN_TOP_DOWN_ORIENTATION:
        return "top-down orientation of image is not allowed with RLE-compression; CppBMPLoader library tries to decode this image nevertheless.";
    case WarningCode::GAP_BTW_COLORMAP_AND_BITMAP:
        return "some gap exists between colormap and image bitmap; CppBMPLoader decodes this image nevertheless.";
    case WarningCode::HOT_POINT_SET:
        return "a hot point X and Y values are set in file header while they should not; CppBMPLoader library nevertheless decodes this file as a BMP one.";
    case WarningCode::INCOHERENT_IMAGE_SIZE:
        return "image size in info header is incoherent with specified width and height.";
    case WarningCode::INCOHERENT_RESOLUTIONS:
        return "maybe incoherent image resolutions detected in info header - one of them may be too big compared to the other one.";
    case WarningCode::INVALID_BA_FILE_HEADER_SIZE:
        return "an invalid file header size has beend detected in a bitmap array header; CppBMPLoader library tries to decode images nevertheless.";
    case WarningCode::INVALID_COLOR_ENCODING:
        return "the specified color encoding is invalid - not an RGB one; CppBMPLoader library nevertheless decodes the bitmap as being RGB encoded.";
    case WarningCode::INVALID_DAMPING_VALUE:
        return "the specified damping value for the halftoning error diffusion is invalid (greater than 100%).";
    case WarningCode::INVALID_RESOLUTION_UNITS:
        return "a non-zero resolution units value difference has been detected - this is invalid; CppBMPLoader library decodes this image nevertheless.";
    case WarningCode::LINKED_PROFILE_NOT_IMPLEMENTED:
        return "for security issues, ICC linked profiles are not implemented in CppBMPLoader library which uses plain coded colors instead.";
    case WarningCode::MISSING_BITFIELDS_MASKS:
        return "at least one color bits mask is missing in bitfields definition.";
    case WarningCode::MISSING_COLORMAP_ENTRIES:
        return "a less-than-full-sized colormap has been detected in file; CppBMPLoader library tries to decode this image nevertheless.";
    case WarningCode::NOT_ENOUGH_INDICES_IN_BITMAP:
        return "too small bitmap, last image pixels are set to default value.";
    case WarningCode::NOT_OS2_BITMAP_FORMAT:
        return "a not OS/2 bitmap format has been detected in a bitmap multi-image data file, which should be an encoding error; CppBMPLoader library decodes the related image nevertheless.";
    case WarningCode::NOT_ZERO_RESERVED:
        return "a reserved word is not set to zero while it should have been; CppBMPLoader library tries to decode this image nevertheless.";
    case WarningCode::PALETT_WITH_FULL_COLORS:
        return "a color palette is defined while this is a full-color bitmap.";
    case WarningCode::TOO_BIG_PALETTE:
        return "too many colors are defined in palette, extra ones will be ignored.";
    case WarningCode::TOO_MANY_INDICES_IN_BITMAP:
        return "bitmap contains too many indices according to final image size; extra ones will be ignored.";
    case WarningCode::UNUSED_BITS_SET_TO_1:
        return "some pixels have bitfields unused bits set to 1.";
    case WarningCode::UNUSED_PALETTE:
        return "An unused palette is defined in BMP file.";
    case WarningCode::WIN_CE_2_BITS_PIXELS:
        return "The unusual 2-bits per pixel specification is only allowed with Windows CE; CppBMPLoader library decodes this image nevertheless.";
    default:
        return "unkown warning...";
    }
}
```

---
*end of documentation*
