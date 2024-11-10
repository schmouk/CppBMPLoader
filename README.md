# CppBMPLoader
An exhaustive BMP Loader, MIT-licensed, implemented for c++14 and c++20

This BMP loader library decodes **Windows** BMP files encoded according to 
BMP format versions 1 up to 5, as well as the specific *OS/2 .x* and *OS/2 
2.x* BMP (*BitMaP*) and BA (*Bitmap multi images Array*) file formats.

It nevertheless currently does not decode JPEG and PNG encoded images that 
are embedded in BMP files, nor does it decode images embedding Windows 
color profiles.

## Genesis of this project
The genesis of this project is the initial creation of another library that
needed to be associate with an external library for loading BMP images under
the **MIT license** or from the public domain, free of rights, to associate 
them with icons. 

Unfortunately, no such library has been found under this type of license that 
would provide the docoding of **all** the different standardized formats of 
BMP images.

## CppBMPLoader is the implemented solution
The **CppBMPLoader** library therefore fills this gap by offering the only 
loading of **BMP** images in all the forms that this image format allows and 
under the permissive MIT license. It will also meet many other needs when one 
sees the number of questions asked on this topic on the Internet and the 
number of BMPLoader libraries already existing, but apparently incomplete, that 
can be found on *GitHub*, programmed in c++ or in other programming languages.

## c++14 and c++20 versions
Initially, two versions of the library are offerred, one responding to standard 
c++14 and the other responding to standard c++20 (offering more advanced 
programming concepts, e.g. *ranges* and *views* or STL extensions).

## Notice (1/2)
The c++14 standard version of **CppBMPLoader** does not use any c++14 
specificities but only c++11 ones. It has been compiled and validated with
a c++14 compiler but should as well be compilable with a c++11 compiler as
well.

## Notice (2/2)
All released code has been validated. For this purpose, we used the great
[**BMP Suite Image List**](https://entropymine.com/jason/bmpsuite/bmpsuite/html/bmpsuite.html)
provided by Jason Summers - see also 
[https://entropymine.com/jason/bmpsuite/](https://entropymine.com/jason/bmpsuite/)
and augmented it with images we created by our own for the version 1 of Windows 
BMP format (lacking in Jason's suite) and with OS/2 **BA** files that we either 
created from scratch or downloaded from the Web, extracted from some original 
ISO images of OS/2.


# Documentation
The API of **CppBMPLoader** is documented in [documentation.md](./documentation.doc).
This API is exactly the same for both the c++14 and the c++20 implementations of
the library.

Shortly speeking, this API offers a templated class BMPImage<> for the loading 
of BMP images content and classes and functions to extract and load all images 
from a **BA** file or just the one of them that best fits size, colors and/or 
resolution criterias.


# Reading the code
For those of you who are the most motivated for deeply diving into this 
library code, please notice that no implementation documentation is provided 
for **CppBMPLoader**.

The design decision made for its first implementation, a.k.a. cpp-14, has been 
to factorize as much code as possible. This was a tremendous decision when at 
bugs fixing time: each bug had to be fixed in one and only one place. This was 
also a very deceptive decision about the final readability of the code. So, you 
should avoid to have a first look at this code: prefer to go into the c++20 
implementation of **CppBMPLoader** to have a far easier reading of it. There, 
we've preferred to duplicate the already debugged code, to provide a clear 
class for each type of object that is internally managed by the library.

Meanwhile, experienced c++ developers should get no trouble in understanding 
the directories tree structure and the code itself as well.  
See directories **cpp-14** or **cpp-20** and enjoy!
