# Exercise 1
### Zooming and Shrinking
> * Zooming the eyes area of Lena in `lena_256.raw` with the zooming ratio 2:1.
> * Srinking the image with ratio 1:2 *row-column deletion*.
> * Compare ↑2.5 ↓2, ↓2 ↑2.5 and  ↑1.25 with nearest neighbor, bilinear and bicublc interpolation approaches. Calculate the execution time.

# Exercise 2
### Gray-level resolution
> * Quantize the gray-level resolution of `lena_256.raw` and `baboon_256.raw` from 8-bit to 1-bit. Show the result of these quantize images and the corresponding with MSE and PSNR value.

# Exercise 3
### Isopreference test on gray-level resolution
> * Test the isopreference on `lena_256.raw` and `baboon_256.raw` images using the programs writen in problems 1 and 2.

# Exercise 4
### Distance and path
> * Given an image `map.raw` of size 20×20 pixels. Find out the distance from (0, 0) to (19, 19) using D<sub>4</sub>, D<sub>8</sub> and D<sub>m</sub> distance, and show their corresponding shortcut paths in the images.
>   1. Gray-value of the road {80}.
>   2. Gray-value of the road {80, 160}.
>   3. Gray-value of the road {80, 160, 255}.