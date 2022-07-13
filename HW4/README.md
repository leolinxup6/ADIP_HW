# Exercise 1
### Histogram processing
> * Use ***Histogram Matching*** to match the histograms of `house512.raw` to that of `lena512.raw`. Show the matched output of `house512.raw`. Also show the histogram of `lena512.raw` as well as the histograms of `house512.raw` before and after histogram matching.
> * Use ***Local Histogram Equalization*** to enhance `house512.raw`.Try at least three different mask sizes to process it. Show and compare the input and output images. Also show and compare the histograms.

# Exercise 2
### Image Averaging
> * Try to use image averaging technique to average the frames selected by you in `street.avi` to extract the static background image, i.e., removing the moving cars. Try to find and compare the results with different number of frames(10, 200, all). Show how the outcome is affected by different traffic flows in the scene.

# Exercise 3
### Edge Detection
> * Perform the following filter on `walkbridge.raw`.
>   1. Apply 3×3 Laplacian filter shown below with zero padding and replicated padding to extend images for filtering. Display the outputs of these two filters.
>   2. Design 3×3 High-boost filter based on two filters shown below by setting A=1, 2, 4. Use replicated padding to complete this problem. Display all result images.
>
> |0|-1|0|
> |:---:|:---:|:---:|
> |**-1**|**+4**|**-1**|
> |**0**|**-1**|**0**|
> 
> |-1|-1|-1|
> |:---:|:---:|:---:|
> |**-1**|**+8**|**-1**|
> |**-1**|**-1**|**-1**|

# Exercise 4
### Gaussion filter
> * Experiment the following test on `turtle512.raw` with mirrored padding to extend images for filtering.
>   1. Apply 5×5 Gaussian filters with σ=0.8, 1.3 and 2.0. to the given image. Display the result.
>   2. Difference of Gaussian(DoG) is a feature enhancement algorithm that involves the **subtraction** of one Gaussian blurred version of an original image(σ<sub>2</sub>) **from** another less blurred version of the original(σ<sub>1</sub>) with σ<sub>1</sub> < σ<sub>2</sub>. Perform 5×5 DoG on the given image with σ<sub>1</sub>=0.5 and σ<sub>2</sub>=1.5. Display the result.