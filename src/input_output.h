//you can find here the routines for reading/writing the images from/to the disc

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <emmintrin.h>
#include <limits.h>
#include <pmmintrin.h>
#include <immintrin.h>

char In_path[100];
char Out_path[100];
char append[3];

#define IN "/home/wave/Desktop/convolution/my_github/input_images/a"
#define IN1 "/home/wave/Desktop/convolution/my_github/input_images/a0.pgm" //1024x1024 - P5
#define IN2 "/home/wave/Desktop/convolution/my_github/input_images/a1.pgm" //512x512
#define IN3 "/home/wave/Desktop/convolution/my_github/input_images/a2.pgm" //640x426
#define IN4 "/home/wave/Desktop/convolution/my_github/input_images/a3.pgm" //800x800 - P5
#define IN5 "/home/wave/Desktop/convolution/my_github/input_images/a4.pgm" //640x480
#define IN6 "/home/wave/Desktop/convolution/my_github/input_images/a5.pgm" //965x965  - P5
#define IN7 "/home/wave/Desktop/convolution/my_github/input_images/a6.pgm" //300x200
#define IN8 "/home/wave/Desktop/convolution/my_github/input_images/a7.pgm" //295x400
#define IN9 "/home/wave/Desktop/convolution/my_github/input_images/a8.pgm" //480x640  - P5
#define IN10 "/home/wave/Desktop/convolution/my_github/input_images/a9.pgm" //720x486
#define IN11 "/home/wave/Desktop/convolution/my_github/input_images/a10.pgm" //1280x853  -P5
#define IN12 "/home/wave/Desktop/convolution/my_github/input_images/a11.pgm" //1920x1280  -P5
#define IN13 "/home/wave/Desktop/convolution/my_github/input_images/a12.pgm" //5184x3456  -P5
#define IN14 "/home/wave/Desktop/convolution/my_github/input_images/a13.pgm" //600x600
#define IN15 "/home/wave/Desktop/convolution/my_github/input_images/a14.pgm" //507x505
#define IN16 "/home/wave/Desktop/convolution/my_github/input_images/a15.pgm" //475x460
#define IN17 "/home/wave/Desktop/convolution/my_github/input_images/a16.pgm" //633x621
#define IN18 "/home/wave/Desktop/convolution/my_github/input_images/a17.pgm"
#define IN19 "/home/wave/Desktop/convolution/my_github/input_images/a18.pgm"
#define IN20 "/home/wave/Desktop/convolution/my_github/input_images/a19.pgm"
#define IN21 "/home/wave/Desktop/convolution/my_github/input_images/a20.pgm"
#define IN22 "/home/wave/Desktop/convolution/my_github/input_images/a21.pgm"
#define IN23 "/home/wave/Desktop/convolution/my_github/input_images/a22.pgm"
#define IN24 "/home/wave/Desktop/convolution/my_github/input_images/a23.pgm"
#define IN25 "/home/wave/Desktop/convolution/my_github/input_images/a24.pgm"
#define IN26 "/home/wave/Desktop/convolution/my_github/input_images/a25.pgm"
#define IN27 "/home/wave/Desktop/convolution/my_github/input_images/a26.pgm"
#define IN28 "/home/wave/Desktop/convolution/my_github/input_images/a27.pgm"
#define IN29 "/home/wave/Desktop/convolution/my_github/input_images/a28.pgm"
#define IN30 "/home/wave/Desktop/convolution/my_github/input_images/a29.pgm"
#define IN31 "/home/wave/Desktop/convolution/my_github/input_images/a30.pgm"



#define OUT "/home/wave/Desktop/convolution/my_github/output_images/a"
#define OUT1 "/home/wave/Desktop/convolution/my_github/output_images/a0_correct.pgm"
#define OUT2 "/home/wave/Desktop/convolution/my_github/output_images/a1_correct.pgm"
#define OUT3 "/home/wave/Desktop/convolution/my_github/output_images/a2_correct.pgm"
#define OUT4 "/home/wave/Desktop/convolution/my_github/output_images/a3_correct.pgm"
#define OUT5 "/home/wave/Desktop/convolution/my_github/output_images/a4_correct.pgm"
#define OUT6 "/home/wave/Desktop/convolution/my_github/output_images/a5_correct.pgm"
#define OUT7 "/home/wave/Desktop/convolution/my_github/output_images/a6_correct.pgm"
#define OUT8 "/home/wave/Desktop/convolution/my_github/output_images/a7_correct.pgm"
#define OUT9 "/home/wave/Desktop/convolution/my_github/output_images/a8_correct.pgm"
#define OUT10 "/home/wave/Desktop/convolution/my_github/output_images/a19_correct.pgm"
#define OUT11 "/home/wave/Desktop/convolution/my_github/output_images/a10_correct.pgm"
#define OUT12 "/home/wave/Desktop/convolution/my_github/output_images/a11_correct.pgm"
#define OUT13 "/home/wave/Desktop/convolution/my_github/output_images/a12_correct.pgm"
#define OUT14 "/home/wave/Desktop/convolution/my_github/output_images/a13_correct.pgm"
#define OUT15 "/home/wave/Desktop/convolution/my_github/output_images/a14_correct.pgm"
#define OUT16 "/home/wave/Desktop/convolution/my_github/output_images/a15_correct.pgm"
#define OUT17 "/home/wave/Desktop/convolution/my_github/output_images/a16_correct.pgm"
#define OUT18 "/home/wave/Desktop/convolution/my_github/output_images/a17_correct.pgm"
#define OUT19 "/home/wave/Desktop/convolution/my_github/output_images/a18_correct.pgm"
#define OUT20 "/home/wave/Desktop/convolution/my_github/output_images/a19_correct.pgm"
#define OUT21 "/home/wave/Desktop/convolution/my_github/output_images/a20_correct.pgm"
#define OUT22 "/home/wave/Desktop/convolution/my_github/output_images/a21_correct.pgm"
#define OUT23 "/home/wave/Desktop/convolution/my_github/output_images/a22_correct.pgm"
#define OUT24 "/home/wave/Desktop/convolution/my_github/output_images/a23_correct.pgm"
#define OUT25 "/home/wave/Desktop/convolution/my_github/output_images/a24_correct.pgm"
#define OUT26 "/home/wave/Desktop/convolution/my_github/output_images/a25_correct.pgm"
#define OUT27 "/home/wave/Desktop/convolution/my_github/output_images/a26_correct.pgm"
#define OUT28 "/home/wave/Desktop/convolution/my_github/output_images/a27_correct.pgm"
#define OUT29 "/home/wave/Desktop/convolution/my_github/output_images/a28_correct.pgm"
#define OUT30 "/home/wave/Desktop/convolution/my_github/output_images/a29_correct.pgm"
#define OUT31 "/home/wave/Desktop/convolution/my_github/output_images/a30_correct.pgm"


#define OUT_NAME1 "/home/wave/Desktop/convolution/my_github/output_images/out1.pgm"
#define OUT_NAME2 "/home/wave/Desktop/convolution/my_github/output_images/out2.pgm"
#define OUT_NAME3 "/home/wave/Desktop/convolution/my_github/output_images/out3.pgm"
#define OUT_NAME4 "/home/wave/Desktop/convolution/my_github/output_images/out4.pgm"


unsigned int M;//cols
unsigned int N;//rows

unsigned char **frame1;//input image
unsigned char **filt;//output image

int create_IO_arrays();
void free_IO_arrays();
//int create_IO_arrays_with_zeros();
//void free_IO_arrays_with_zeros();


void read_image(char* filename);
void read_image_and_put_zeros_around(char* filename);

void write_image2(char* filename);

void openfile(char *filename, FILE** finput);
int getint(FILE *fp);





