# CppBMPLoader  [![Latest release](http://img.shields.io/github/release/schmouk/cppbmploader.svg?style=plastic&labelColor=blueviolet&color=success)](https://github.com/schmouk/cppbmploader/releases/latest)
An exhaustive BMP Loader, MIT-licensed, implemented for c++11

This BMP loader library decodes **Windows** BMP files encoded according to 
BMP format versions 1 up to 5, as well as the specific *OS/2 1.x* and *OS/2 
2.x* BMP (*BitMaP*) and BA (*Bitmap multi images Array*) file formats.

It nevertheless currently does not decode JPEG and PNG encoded images that 
are embedded in BMP files, nor does it decode images embedding Windows 
color profiles.

## Genesis of this project
The genesis of this project is the initial creation of another library that
needed to be associated with an external library for loading BMP images under
the **MIT license** or from the public domain, free of rights, to associate 
them with icons. 

Unfortunately, no such library has been found under this type of license that 
would provide for the decoding of **all** the different standardized formats of 
BMP images.

## CppBMPLoader is the implemented solution
The **CppBMPLoader** library therefore fills this gap by offering the only 
loading of **BMP** images in all the forms that this image format allows and 
under the permissive MIT license. It will also meet many other needs when one 
sees the number of questions asked on this topic on the Internet and the 
number of BMPLoader libraries already existing, but apparently incomplete, that 
can be found on *GitHub*, programmed in c++ or in other programming languages.

## c++11 version
Initially, only a c++11 version of the library code is offerred.

## Notice (1/2)
The c++11 version of **CppBMPLoader** has been compiled with c++14 standard as provided by Visual Studio 2022. Meanwhile, it does not use any c++14 
specificities but only c++11 ones. While having been compiled and validated with
a c++14 compiler it should as well be compilable with a c++11 compiler.

## Notice (2/2)
All released code has been validated. For this purpose, we used the great
[**BMP Suite Image List**](https://entropymine.com/jason/bmpsuite/bmpsuite/html/bmpsuite.html)
provided by Jason Summers - see also 
[https://entropymine.com/jason/bmpsuite/](https://entropymine.com/jason/bmpsuite/)
and we augmented it with images we created by our own for the version 1 of Windows 
BMP format (lacking in Jason's suite) and with OS/2 **BA** files that we either 
created from scratch or downloaded from the Web, extracted from some original 
ISO images of OS/2.


# Documentation
The API of **CppBMPLoader** is documented in [documentation.md](./documentation.doc).

Shortly speeking, this API offers a templated class BMPImage<> for the loading 
of BMP images content and classes and functions to extract and load all images 
from a **BA** file or just the one of them that best fits dimensions, colors 
and/or resolutions criterias.

Many error cases and warnings are detected by **CppBMPLoader**. Errors are always 
fatal. Execution stops on any encountered error. Warnings do not stop your 
application and are accumulated along the processing of BMP and BA files. You 
get access to the error code and to a list of warning codes as well to the 
associated messages at will, since they are attached to the faulty BMP images.


# Reading the code
For those of you who are the most motivated for deeply diving into this 
library code, please notice that no implementation documentation is provided 
for **CppBMPLoader**.

The design decision made for its first implementation has been 
to factorize as much code as possible. This was a tremendous decision when at 
bugs fixing time: each bug had to be fixed in one and only one place. This was 
also a very deceptive decision about the final readability of the code. It might be that a further version of libray **CppBMPLoader** will embed c++20 or even c++23 code. Should this happen, a full refactoring would then happen. Duplicating the already debugged code, to provide a 
single clear class for each type of object and for each format version that is 
internally managed by the library, would eventually enhance the readiness of the code. So, wait and see.

Meanwhile, experienced c++ developers should get no trouble in understanding 
the directories tree structure and the code itself as well, which is 
commented everywhere it seemed to be useful.  
See directories **cpp-14** or **cpp-20** and enjoy!
