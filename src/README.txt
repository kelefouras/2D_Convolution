
In this paper, a methodology for accelerating the 2D direct Convolution operation on x86-64 processors is presented.

2D convolution has many applications. This software includes several very fast routines for image smoothing. It reads one or more images from the 'input images' directory, runs one of the optimized or un-optomized routines and stores the output image(s) to the 'output images' directory. 

The fast convolution routines for non-symmetrical kernels (general case) are:
1. convolution_optimized_5x5_reg_blocking() found in convolution.h
2. convolution_optimized_3x3_reg_blocking() found in convolution.h

The fast convolution routines for symmetrical kernels (Gaussian Blur) are:
1. Gaussian_Blur_optimized_5x5_step28_less_div_reg_blocking() found in Gaussian_Blur.h
2. Gaussian_Blur_optimized_3x3_reg_blocking() found in Gaussian_Blur.h

The un-optimized routine for both the non-symmetrical and symmetrical kernels is
 Gaussian_Blur_default() found in Gaussian_Blur.h


To measure the performance of the proposed routines run the 'measure_performance.sh' script. First, you must specify the kernel size in line 24 of main.c, i.e., 'unsigned char size=3;'. Second, you must un-comment the routine you want to run in line 135 of main.c (un-comment just one routine). Last, you must specify the right path for the input/output images in input_output.h file. 



If you want to test the correctness of the proposed method, i.e., whether the output images are correct or not, then you must comment the 'measure_time(argc, argv);' in line 32 of main.c and uncomment the routines in lines 35 and 36 of main.c. 
Then run the 'test_correctness.sh' script. All the output images will be stored into the 'output images' directory and compared by using the 'compare.sh' script. You must also specify the right path for the input/output images in input_output.h file.
