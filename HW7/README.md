# Exercise 1
### Periodic noise
> * Use Notch filtering to remove the periodic noise in frequency domain. Show the clean image.
> * Use band reject filtering to remove the periodic noise in frequency domain. Show the clean image.

# Exercise 2
### Deblur
> * Perform **inverse filter** and **Wiener filter** separately on `motion_flower.raw` to unblur the image which is blurred by motion as *g(x, y)* function. Show the output images.
> $$ g(x,y) = \int_0^1 f(x, y+0.01t)dt $$