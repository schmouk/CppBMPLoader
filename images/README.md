This directory contains two subdirectories.

- `bmpsuite-2.8` contains the BMP suite in its version 2.8 that has been used to validate library **CppBMPLoader**.
- `v1` contains very simple BMP images in the very first BMP format version 1.x that is lacking in **bmpsuite 2.8**.

The **BMP suite 2.8** embeds various BMP test images. Explanations can be found in the file `bmpsuite-2.8/readme.txt`, copyright notice is avalibale in file `bmpsuite-2.8/COPYING.txt`. We have removed all other stuff from the original suite, notably the C code that has beend developped to create the BMP images and which is under the same license, plus the ICC profiles which are provided under another license. Please have a look to these two text files.  
NOTICE: `bmpsuite-2.8/html/bmpsuite.html` is a simple HTML page that displays all the test images in your favorite browser. Just double-click on it to get an overview of this great BMP test suite.

**v1** contains three simple BMP images :
- `v1/2.bmp` contains a black and white checherboard - 1 bit per pixel, 2-colors palette (black, and white)
- `v1/16.bmp` contains a 16-colors palette (4 bits per pixel) checkerboard and can be used to validate different default system palettes according to successive Windows versions
- `v1/256.bmp` contains a 256-colors palette (8 bits per pixel) checkerboard associated with the default Windows system palette.