echo THIS SCRIPT FIRST RUNS THE UNOPTIMIZED METHOD FOR 20 GREYSCALE 8-BIT IMAGES AND STORES THE OUTPUT IMAGES. THEN DOES THE SAME FOR THE OPTIMIZED ROUTINES. All the output images will be stored into the output images directory. There you can run the compare.sh script that compares the images of the un-optimized method to the images of the optimized method. 

gcc main.c Gaussian_Blur.c input_output.c convolution.c -o p -O3 -march=native -mavx -lm -D_GNU_SOURCE  -g  -pthread -fopenmp
./p


cd ..
cd output_images
echo
echo
echo -------------comparing images now ...---------------
sh ./compare.sh

cd ..
cd src


