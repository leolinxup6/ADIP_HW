# Exercise 1
### Bit plane
> * Hide `baboon_256.raw` in `lena_256.raw`and save the file as `lena_with_baboon.raw`. Try not to visually alter `baboon_256.raw` and `lena_256.raw` as you can. Calculate MSE and PSNR of your `lena_with_baboon.raw` with respect to the original `lena_256.raw`. Your design should have the PSNR value as high as possible.
> Extract your modified `baboon_256.raw` from `lena_with_baboon.raw` . Calculate MSE and PSNR of your extracted `baboon.raw` with respect to the original `lena_256.raw`. Your design should have the PSNR value as high as possible.

# Exercise 2
### Bit-plane and Negative
> * Synthesize eight bit-plane images(`a512×512.raw`, ..., `h512×512.raw`) back to the original image. The bit-plane images are given in random order and some of them have been process by the image negative effect.

# Exercise 3
### Gray-level Transformation
> * Use the following methods to adjust the darkened image `livingroom_d512.raw` and the whiten image `cameraman_b512.raw` to improve their contrast.
>   1. Log transformation(c=100 and c = 20)
>   2. Power-Law transformation(γ=0.2 and γ=10, and try different c)

# Exercise 4
### Histogram Equalization
> * Plot the histograms of the `livingroom_d512.raw` and `cameraman_b512.raw`. You have to implement your own histogram function but you can use OpenCV to plot it.
> * Perform Histogram Equalization(implement your own function) on `livingroom_d512.raw` and `cameraman_b512.raw`. Plot their histograms and compare the histograms before and after histogram equalization.