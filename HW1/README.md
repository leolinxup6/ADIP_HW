# Exercise 1.1 Raw image file format
### Raw image file format
> Try to use Xnview, a raw image viewer, to read the raw image `lena256.raw`.
# Exercise 1.2 Raw image file I/O
> * Read the raw image file `lena256.raw`.
>   1. what is the intensity values of the pixel at the coordinate of the 123<sup>rd</sup> row & 234<sup>th</sup> column?
>   2. what is the intensity values of the 5487<sup>th</sup> pixel stored in `lena256.raw` file? what is the pixel coordinate in the image?
> * Display the `lena256.raw`.
> * Rotate the `lena256.raw`.
> * Split `lena256.raw` into 16 square patchs of the same size, and randomly swap them to output a new image.
> * Given four 8-bit images `p01.raw`, `p02.raw`, `p03.raw`, `p04.raw`. Each image is in square size.
>   1. Please determine the image size from the file information of the raw images.
>   2. Use these four images to assemble a large image th have a good look.
# Exercise 1.3 Brightness
> *  Increase the image brightness. Reading `lena256.raw` and adding value **50** to each pixel value. Write the output to a raw image file.
> * Reading `lena256.raw` and increase the image brightness with random value within **-50** to **50**.
# Exercise 2 PNG image I/O
> * This probelm is to practice calling OpenCV functions.<br>You should open `haha.raw`. Please Draw your Student ID and any word you want to show on the `haha.raw` image. Save the image as png format.
