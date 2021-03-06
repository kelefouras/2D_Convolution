echo THIS SCRIPT RUNS THE PROPOSED METHOD FOR 20 GREYSCALE 8-BIT IMAGES AND PRINTS THE EXECUTION TIME VALUE - THE PROPOSED METHOD IS RUN TWO TIMES FOR EACH IMAGE

echo running a0.pgm
gcc main.c Gaussian_Blur.c input_output.c Filter2D.c -o p -O3 -march=native -mavx -lm -D_GNU_SOURCE  -g  -pthread -fopenmp

./p 20000 /home/wave/Desktop/convolution/my_github/input_images/a0.pgm
./p 20000 /home/wave/Desktop/convolution/my_github/input_images/a0.pgm

echo
echo running a1.pgm
./p 60000 /home/wave/Desktop/convolution/my_github/input_images/a1.pgm
./p 60000 /home/wave/Desktop/convolution/my_github/input_images/a1.pgm

echo
echo running a2.pgm
./p 60000 /home/wave/Desktop/convolution/my_github/input_images/a2.pgm
./p 60000 /home/wave/Desktop/convolution/my_github/input_images/a2.pgm

echo
echo running a3.pgm
./p 30000 /home/wave/Desktop/convolution/my_github/input_images/a3.pgm
./p 30000 /home/wave/Desktop/convolution/my_github/input_images/a3.pgm


echo
echo running a4.pgm
./p 60000 /home/wave/Desktop/convolution/my_github/input_images/a4.pgm
./p 60000 /home/wave/Desktop/convolution/my_github/input_images/a4.pgm


echo
echo running a5.pgm
./p 20000 /home/wave/Desktop/convolution/my_github/input_images/a5.pgm
./p 20000 /home/wave/Desktop/convolution/my_github/input_images/a5.pgm


echo
echo running a6.pgm
./p 160000 /home/wave/Desktop/convolution/my_github/input_images/a6.pgm
./p 160000 /home/wave/Desktop/convolution/my_github/input_images/a6.pgm


echo
echo running a7.pgm
./p 80000 /home/wave/Desktop/convolution/my_github/input_images/a7.pgm
./p 80000 /home/wave/Desktop/convolution/my_github/input_images/a7.pgm

echo
echo running a8.pgm
./p 60000 /home/wave/Desktop/convolution/my_github/input_images/a8.pgm
./p 60000 /home/wave/Desktop/convolution/my_github/input_images/a8.pgm


echo
echo running a9.pgm
./p 60000 /home/wave/Desktop/convolution/my_github/input_images/a9.pgm
./p 60000 /home/wave/Desktop/convolution/my_github/input_images/a9.pgm


echo
echo running a10.pgm
./p 20000 /home/wave/Desktop/convolution/my_github/input_images/a10.pgm
./p 20000 /home/wave/Desktop/convolution/my_github/input_images/a10.pgm

echo
echo running a11.pgm
./p 10000 /home/wave/Desktop/convolution/my_github/input_images/a11.pgm
./p 10000 /home/wave/Desktop/convolution/my_github/input_images/a11.pgm


echo
echo running a12.pgm
./p 2000 /home/wave/Desktop/convolution/my_github/input_images/a12.pgm
./p 2000 /home/wave/Desktop/convolution/my_github/input_images/a12.pgm


echo
echo running a13.pgm
./p 40000 /home/wave/Desktop/convolution/my_github/input_images/a13.pgm
./p 40000 /home/wave/Desktop/convolution/my_github/input_images/a13.pgm

echo
echo running a14.pgm
./p 60000 /home/wave/Desktop/convolution/my_github/input_images/a14.pgm
./p 60000 /home/wave/Desktop/convolution/my_github/input_images/a14.pgm

echo
echo running a15.pgm
./p 60000 /home/wave/Desktop/convolution/my_github/input_images/a15.pgm
./p 60000 /home/wave/Desktop/convolution/my_github/input_images/a15.pgm



echo
echo running a16.pgm
./p 60000 /home/wave/Desktop/convolution/my_github/input_images/a16.pgm
./p 60000 /home/wave/Desktop/convolution/my_github/input_images/a16.pgm


echo
echo running a17.pgm
./p 20000 /home/wave/Desktop/convolution/my_github/input_images/a17.pgm
./p 20000 /home/wave/Desktop/convolution/my_github/input_images/a17.pgm

echo
echo running a25.pgm
./p 2000 /home/wave/Desktop/convolution/my_github/input_images/a25.pgm
./p 2000 /home/wave/Desktop/convolution/my_github/input_images/a25.pgm

echo
echo running a26.pgm
./p 4000 /home/wave/Desktop/convolution/my_github/input_images/a26.pgm
./p 4000 /home/wave/Desktop/convolution/my_github/input_images/a26.pgm



