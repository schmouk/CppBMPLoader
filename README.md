# CppBMPLoader
An exhaustive BMP Loader, MIT-licensed, implemented for c++14 and c++20

Caution: This BMP loader library only decodes **Windows** BMP files encoded 
according to **BMP format versions 3 and above**. Versions 1 and 2 are not
decoded. The specific *OS/1.x* and *OS/2.x* BMP file formats are currently 
not dealed with by **CppBMPLoader** either.


## Genesis of this project
The genesis of this project is the initial creation of the **OGLFW** library, 
an object overlay of the **GLFW** library, and the need to associate it with 
a library for loading images under the **MIT license** or from the public 
domain, free of rights, to associate them with icons provided in OGLFW. 

## No way to find such a library - Alternative
Being unable to find such a library under this type of license, a solution was 
to let users of the original **OGLFW** library load the images by themselves 
using the external library of their choice, even if not under the MIT license.

Another solution was to propose an `Icon` class in library **OGLFW** with a 
virtual image loading method not implemented in this base class. This second 
solution allowed users to derive their own classes from this Icon base class 
and to implement the image loading method corresponding to the format of the 
images to be loaded (possibly using already existing but less permissively 
licensed libraries).  
To this end, and as an example of implementation, a `BMPIcon` derived class 
has been provided in the **OGLFW** library. Its implementation had to rely on 
an external library allowing the loading of **BMP** images. Unfortunately, 
no library met the required licensing criteria or, when unlicensed or under 
**MIT** license, did not allow decoding of all cases of BMP image encoding.

## CppBMPLoader is the implemented solution
The **CppBMPLoader** library therefore fills this gap by offering the only 
loading of **BMP** images in all the forms that this image format allows and 
under the permissive MIT license. It will also meet many other needs when one 
sees the quantity of questions asked on this subject on the Internet and the 
number of BMPLoader libraries already existing, but apparently incomplete, that 
can be found on *GitHub*, programmed in c++ or in other programming languages.

## c++14 and c++20 versions
Initially, two versions of the library are offered, one responding to standard 
c++14 of the programming language and the other responding to standard c++20 
and offering more advanced programming concepts (e.g. programmed in the form 
of c++ modules).

## Notice
Notice: by April and May 2024, the original library **OGLFW** is still under 
development.
