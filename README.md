In this repository you can find the fastest routines available for computing the 2D Convolution operation on X86-64 processors (Linux only). 

Although the 2D convolution operation can be applied to several image/video processing applications, this repository includes the GaussianBlur and Filter2D routines (image smoothing). These routines read 8-bit greyscale images and generate new smoothed greyscale images.
For more information about how these routines work see the description of OpenCV library https://docs.opencv.org/master/d4/d86/group__imgproc__filter.html. 

A detailed description of these routines can be found in the paper entitled 'A Methodology for Speeding up the 2D Convolution on
x86-64 Processors' published in ...

The fastest Filter2D routines (used for non-symmetrical kernels) are:
1. convolution_optimized_3x3_reg_blocking_16() and convolution_optimized_3x3_16_many_loads, found in Filter2D.h. The '3x3' relates to the kernel size, while the '16' relates to the intermediate results bit-width.
2. convolution_optimized_5x5_reg_blocking_16(), found in Filter2D.h
3. convolution_optimized_7x7_32(), found in Filter2D.h
4. convolution_optimized_9x9_32(), found in Filter2D.h

The fastest Gaussian Blur routines (used for symmetrical kernels) are:
1. Gaussian_Blur_optimized_3x3_16_reg_blocking() and Gaussian_Blur_3x3_16_more_load(), found in Gaussian_Blur.h
2. Gaussian_Blur_optimized_5x5_16_reg_blocking() and Gaussian_Blur_optimized_5x5_16_seperable, found in Gaussian_Blur.h
3. Gaussian_Blur_7x7_16_separable(), found in Gaussian_Blur.h
4. Gaussian_Blur_9x9_16_separable(), found in Gaussian_Blur.h

The un-optimized routines for both the non-symmetrical and symmetrical kernels are:
 Conv_default() and Conv_default_separable(), found in Gaussian_Blur.h


HOW TO USE:

This project contains 7 main source files. The main.c file, the Gaussian_Blur.h/Gaussian_Blur.c files which include all the routines for the Gaussian_Blur operation, the Filter2D.h/Filter2D.c files which include all the routines for the Filter2D operation and the input_output.h/input_output.c files which include all the necessary routines to load/store the images from/to the disc. Note that in these files there are some routines which are incomplete (used for debugging and other purposes).
You can compile the code by using the following command : gcc main.c Gaussian_Blur.c input_output.c Filter2D.c -o p -O3 -march=native -mavx -lm -D_GNU_SOURCE  -g  -pthread -fopenmp

If you want to measure the performance of the proposed routines you can run the 'measure_performance.sh' script. First, you must specify the kernel size in line 33 of main.c, e.g., 'unsigned char size=3;' will define a kernel of size 3x3; use either size=3 OR size=5 OR size=7 OR size=9. Second, you must un-comment the routine you want to run in line 250 of main.c (un-comment just one routine). Last, you must specify the right path for the input/output images in input_output.h file. 

If you want to test the correctness of the proposed method, i.e., whether the output images are correct or not, then you must comment the 'measure_time(argc, argv);' in line 45 of main.c and uncomment the routines in lines 48 and 49 of main.c. 
Then run the 'test_correctness.sh' script. All the output images will be stored into the 'output images' directory and compared by using the 'compare.sh' script. You must also specify the right path for the input/output images in input_output.h file.

