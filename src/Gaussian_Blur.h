//You can find here the fast convolution routines for symmetrical kernels (gaussianblur)


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <emmintrin.h>
#include <limits.h>
#include <pmmintrin.h>
#include <immintrin.h>
#include <stdint.h>	/* for uint64 definition */
#include <sched.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <omp.h>


//default routines
void Conv_default(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N,const unsigned int filter_size,const unsigned int divisor, signed char **filter5x5);
void Conv_default_separable(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned int kernel_size, signed char *kernel_y, signed char *kernel_x, const unsigned int divisor_xy);


//------------------------9x9 routines ------------------------------------
void Gaussian_Blur_9x9_16_separable(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, signed char *kernel_y, signed char *kernel_x, const unsigned short int divisor_xy);
void prelude_9x9_16_Ymask_3(const int row, const unsigned int M,unsigned char **frame1, unsigned char *temp, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f, const __m256i mask_prelude);
void prelude_9x9_16_Ymask_2(const int row, const unsigned int M,unsigned char **frame1, unsigned char *temp, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f, const __m256i mask_prelude);
void prelude_9x9_16_Ymask_1(const int row, const unsigned int M,unsigned char **frame1, unsigned char *temp, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f, const __m256i mask_prelude);
void prelude_9x9_16_Ymask_0(const int row,const unsigned int M, unsigned char **frame1, unsigned char *temp, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f, const __m256i mask_prelude);
void prelude_9x9_16_Xmask(unsigned char **frame1,unsigned char **filt,unsigned char *temp, const unsigned int N,const unsigned int M,const int row, const signed char mask_vector_x[][32],  const unsigned int division_case, const __m256i f,const unsigned int REMINDER_ITERATIONS_X,const unsigned int REMINDER_ITERATIONS_Y,const signed char mask_vector_y[][32],const unsigned short int divisor_xy,signed char *kernel_x);
void loop_reminder_9x9_16_blur(unsigned char **frame1,unsigned char **filt,unsigned char *temp,const unsigned int N,const unsigned int M,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS_X,const unsigned int REMINDER_ITERATIONS_Y,const unsigned int division_case,const signed char mask_vector_x[][32],const signed char mask_vector_y[][32], const __m256i f, const unsigned short int divisor_xy,signed char *kernel_x);
void loop_reminder_9x9_16_blur_Y(unsigned char **frame1,unsigned char **filt,unsigned char *temp,const unsigned int N,const unsigned int M,const unsigned int row, const unsigned int REMINDER_ITERATIONS_Y,const unsigned int division_case,const signed char mask_vector_y[][32], const __m256i f, const unsigned short int divisor_xy);
void loop_reminder_9x9_16_blur_X(unsigned char **frame1,unsigned char **filt,unsigned char *temp,const unsigned int N,const unsigned int M,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS_X,const unsigned int division_case,const signed char mask_vector_x[][32], const __m256i f, const unsigned short int divisor_xy,signed char *kernel_x);

void Gaussian_Blur_9x9_32_separable(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, signed char *kernel_y, signed char *kernel_x, const unsigned short int divisor_xy);
void prelude_9x9_32_Ymask_3(const int row, const unsigned int M,unsigned char **frame1, unsigned char *temp, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f, const __m256i mask_prelude);
void prelude_9x9_32_Ymask_2(const int row, const unsigned int M,unsigned char **frame1, unsigned char *temp, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f, const __m256i mask_prelude);
void prelude_9x9_32_Ymask_1(const int row, const unsigned int M,unsigned char **frame1, unsigned char *temp, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f, const __m256i mask_prelude);
void prelude_9x9_32_Ymask_0(const int row,const unsigned int M, unsigned char **frame1, unsigned char *temp, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f, const __m256i mask_prelude);
void prelude_9x9_32_Xmask(unsigned char **frame1,unsigned char **filt,unsigned char *temp, const unsigned int N,const unsigned int M,const int row, const signed char mask_vector_x[][32],  const unsigned int division_case, const __m256i f,const unsigned int REMINDER_ITERATIONS_X,const unsigned int REMINDER_ITERATIONS_Y,const signed char mask_vector_y[][32],const unsigned short int divisor_xy,signed char *kernel_x);
void loop_reminder_9x9_32_blur_Y(unsigned char **frame1,unsigned char **filt,unsigned char *temp,const unsigned int N,const unsigned int M,const unsigned int row, const unsigned int REMINDER_ITERATIONS_Y,const unsigned int division_case,const signed char mask_vector_y[][32], const __m256i f, const unsigned short int divisor_xy);
void loop_reminder_9x9_32_blur_X(unsigned char **frame1,unsigned char **filt,unsigned char *temp,const unsigned int N,const unsigned int M,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS_X,const unsigned int division_case,const signed char mask_vector_x[][32], const __m256i f, const unsigned short int divisor_xy,signed char *kernel_x);

__m256i fill_zeros(__m256i r0, const __m256i mask_prelude );//make one routine
__m256i fill_3zeros(__m256i r0, const __m256i mask_prelude );
__m256i fill_2zeros(__m256i r0, const __m256i mask_prelude );
__m256i fill_1zeros(__m256i r0, const __m256i mask_prelude );

void Gaussian_Blur_9x9_16_separable_old(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, signed char *kernel_y, signed char *kernel_x, const unsigned short int divisor_xy);
void Gaussian_Blur_9x9_16_separable_extra_array_less_load(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, signed char *kernel_y, signed char *kernel_x, const unsigned short int divisor_xy);

//------------------------7x7 routines ------------------------------------
void Gaussian_Blur_7x7_16_separable(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, signed char *kernel_y, signed char *kernel_x, const unsigned short int divisor_xy);
void prelude_7x7_16_Ymask_2_new(const int row, const unsigned int M,unsigned char **frame1, unsigned char *temp, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f, const __m256i mask_prelude);
void prelude_7x7_16_Ymask_1_new(const int row, const unsigned int M,unsigned char **frame1, unsigned char *temp, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f, const __m256i mask_prelude);
void prelude_7x7_16_Ymask_0_new(const int row,const unsigned int M, unsigned char **frame1, unsigned char *temp, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f, const __m256i mask_prelude);
void prelude_7x7_16_Xmask_new(unsigned char **frame1,unsigned char **filt,unsigned char *temp, const unsigned int N,const unsigned int M,const int row, const signed char mask_vector_x[][32],  const unsigned int division_case, const __m256i f,const unsigned int REMINDER_ITERATIONS_XY,const signed char mask_vector_y[][32],const unsigned short int divisor_xy,signed char *kernel_x);
void loop_reminder_7x7_16_blur_Y(unsigned char **frame1,unsigned char **filt,unsigned char *temp,const unsigned int N,const unsigned int M,const unsigned int row, const unsigned int REMINDER_ITERATIONS_Y,const unsigned int division_case,const signed char mask_vector_y[][32], const __m256i f, const unsigned short int divisor_xy);
int loop_reminder_7x7_16_blur_X(unsigned char **frame1,unsigned char **filt,unsigned char *temp,const unsigned int N,const unsigned int M,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS_X,const unsigned int division_case,const signed char mask_vector_x[][32], const __m256i f, const unsigned short int divisor_xy,signed char *kernel_x);

void Gaussian_Blur_7x7_32_separable(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, signed char *kernel_y, signed char *kernel_x, const unsigned short int divisor_xy);
void prelude_7x7_32_Ymask_2_new(const int row, const unsigned int M,unsigned char **frame1, unsigned char *temp, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f, const __m256i mask_prelude);
void prelude_7x7_32_Ymask_1_new(const int row, const unsigned int M,unsigned char **frame1, unsigned char *temp, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f, const __m256i mask_prelude);
void prelude_7x7_32_Ymask_0_new(const int row,const unsigned int M, unsigned char **frame1, unsigned char *temp, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f, const __m256i mask_prelude);
void prelude_7x7_32_Xmask_new(unsigned char **frame1,unsigned char **filt,unsigned char *temp, const unsigned int N,const unsigned int M,const int row, const signed char mask_vector_x[][32],  const unsigned int division_case, const __m256i f,const unsigned int REMINDER_ITERATIONS_XY,const signed char mask_vector_y[][32],const unsigned short int divisor_xy,signed char *kernel_x);
void loop_reminder_7x7_32_blur_Y(unsigned char **frame1,unsigned char **filt,unsigned char *temp,const unsigned int N,const unsigned int M,const unsigned int row, const unsigned int REMINDER_ITERATIONS_Y,const unsigned int division_case,const signed char mask_vector_y[][32], const __m256i f, const unsigned short int divisor_xy);
int loop_reminder_7x7_32_blur_X(unsigned char **frame1,unsigned char **filt,unsigned char *temp,const unsigned int N,const unsigned int M,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS_X,const unsigned int division_case,const signed char mask_vector_x[][32], const __m256i f, const unsigned short int divisor_xy,signed char *kernel_x);


void Gaussian_Blur_7x7_32_separable_blocking_old(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, signed char *kernel_y, signed char *kernel_x, const unsigned short int divisor_xy);
void Gaussian_Blur_7x7_32_separable_old(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, signed char *kernel_y, signed char *kernel_x, const unsigned short int divisor_xy);
extern __m256i my_rshift16_by_one(__m256i m0,__m256i mask3);
__m256i prelude_7x7_Ymask_3(const int row, const int col, unsigned char **frame1, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f, const __m256i mask_prelude, const __m256i mask_even_16, const __m256i mask_odd_16);
__m256i prelude_7x7_Ymask_2(const int row, const int col, unsigned char **frame1, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f, const __m256i mask_prelude, const __m256i mask_even_16, const __m256i mask_odd_16);
__m256i prelude_7x7_Ymask_1(const int row, const int col, unsigned char **frame1, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f, const __m256i mask_prelude, const __m256i mask_even_16, const __m256i mask_odd_16);
void prelude_7x7_Xmask(unsigned char **filt,const int row, const int col, const __m256i r0, const signed char mask_vector_x[][32],  const unsigned int division_case, const __m256i f, const __m256i mask_even_32, const __m256i mask_odd_32);
void loop_reminder_7x7_blur(unsigned char **frame1,unsigned char **filt,const unsigned int N,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const signed char mask_vector_x[][32],const signed char mask_vector_y[][32], const __m256i f, const unsigned short int divisor_xy);

void Gaussian_Blur_7x7_16_separable_blocking_old(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, signed char *kernel_y, signed char *kernel_x, const unsigned short int divisor_xy);
void Gaussian_Blur_7x7_16_separable_old(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, signed char *kernel_y, signed char *kernel_x, const unsigned short int divisor_xy);
__m256i prelude_7x7_16_Ymask_3(const int row, const int col, unsigned char **frame1, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f, const __m256i mask_prelude);
__m256i prelude_7x7_16_Ymask_2(const int row, const int col, unsigned char **frame1, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f, const __m256i mask_prelude);
__m256i prelude_7x7_16_Ymask_1(const int row, const int col, unsigned char **frame1, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f, const __m256i mask_prelude);
void prelude_7x7_16_Xmask(unsigned char **filt,const int row, const int col, const __m256i r0, const signed char mask_vector_x[][32],  const unsigned int division_case, const __m256i f);
void loop_reminder_7x7_16_blur(unsigned char **frame1,unsigned char **filt,const unsigned int N,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const signed char mask_vector_x[][32],const signed char mask_vector_y[][32], const __m256i f, const unsigned short int divisor_xy);


//------------------------5x5 routines ------------------------------------
// THE MAIN ROUTINEs HERE ARE THE :
//Gaussian_Blur_optimized_5x5_16_seperable - THIS FASTEST FOR DIVISOR=POWER OF 2
//Gaussian_Blur_optimized_5x5_16_reg_blocking() - THIS FASTEST FOR DIVISOR=NOT POWER OF 2

void Gaussian_Blur_optimized_5x5_16(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);
void Gaussian_Blur_optimized_5x5_16_reg_blocking(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);
int loop_reminder_high_reminder_values_less_div(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c0_sh1,const __m256i c1_sh1,const __m256i c2_sh1,const __m256i c0_sh2,const __m256i c1_sh2,const __m256i c2_sh2, const __m256i f,const unsigned short int divisor,signed char **filter5x5);
int loop_reminder_low_reminder_values_less_div(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c0_sh1,const __m256i c1_sh1,const __m256i c2_sh1,const __m256i c0_sh2,const __m256i c1_sh2,const __m256i c2_sh2, const __m256i c0_sh3,const __m256i c1_sh3,const __m256i c2_sh3,const __m256i c0_sh4,const __m256i c1_sh4,const __m256i c2_sh4,const __m256i c0_sh5,const __m256i c1_sh5,const __m256i c2_sh5,const __m256i f);
int loop_reminder_first_less_div(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c0_sh1,const __m256i c1_sh1,const __m256i c2_sh1,const __m256i c0_sh2,const __m256i c1_sh2,const __m256i c2_sh2, const __m256i f,const unsigned short int divisor, signed char **filter5x5);
int loop_reminder_last_less_div(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c0_sh1,const __m256i c1_sh1,const __m256i c2_sh1,const __m256i c0_sh2,const __m256i c1_sh2,const __m256i c2_sh2, const __m256i f,const unsigned short int divisor, signed char **filter5x5);
int loop_reminder_last_less_div_special_case(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c0_sh1,const __m256i c1_sh1,const __m256i c2_sh1,const __m256i c0_sh2,const __m256i c1_sh2,const __m256i c2_sh2, const __m256i f,const unsigned short int divisor, signed char **filter5x5);

__m256i division(const unsigned int division_case, __m256i m2, const __m256i f);
const unsigned int prepare_for_division(const unsigned short int divisor);

__m256i division_32(const unsigned int division_case, __m256i m2, const __m256i f);
const unsigned int prepare_for_division_32(const unsigned int divisor);

void print_vector_8(__m256i v);
void print_vector_16(__m256i v);
void print_vector_32(__m256i v);



//------------5x5 seperable filter routines------------------------------
//this routine is the fastest for divisor power of 2
void Gaussian_Blur_optimized_5x5_16_seperable(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, signed char *kernel_y, signed char *kernel_x, const unsigned short int divisor_xy);
void prelude_5x5_16_Ymask_1_new(const int row, const unsigned int M,unsigned char **frame1, unsigned char *temp, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f);
void prelude_5x5_16_Ymask_0_new(const int row,const unsigned int M, unsigned char **frame1, unsigned char *temp, const signed char mask_vector_y[][32],  const unsigned int division_case, const __m256i f);
void prelude_5x5_16_Xmask_new(unsigned char **frame1,unsigned char **filt,unsigned char *temp, const unsigned int N,const unsigned int M,const int row, const signed char mask_vector_x[][32],  const unsigned int division_case, const __m256i f,const unsigned int REMINDER_ITERATIONS_XY,const signed char mask_vector_y[][32],const unsigned short int divisor_xy,signed char *kernel_x);
void loop_reminder_5x5_16_blur_Y(unsigned char **frame1,unsigned char **filt,unsigned char *temp,const unsigned int N,const unsigned int M,const unsigned int row, const unsigned int REMINDER_ITERATIONS_Y,const unsigned int division_case,const signed char mask_vector_y[][32], const __m256i f, const unsigned short int divisor_xy);
void loop_reminder_5x5_16_blur_X(unsigned char **frame1,unsigned char **filt,unsigned char *temp,const unsigned int N,const unsigned int M,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS_X,const unsigned int division_case,const signed char mask_vector_x[][32], const __m256i f, const unsigned short int divisor_xy,signed char *kernel_x);

extern __m256i insert_one_zeros_front(__m256i r0,__m256i mask_prelude);
extern __m256i insert_two_zeros_front(__m256i r0,__m256i mask_prelude);

//------------------------5x5 routines function inline division------------------------------------
void Gaussian_Blur_optimized_5x5_step28_less_div_reg_blocking_caseC(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);

//reminder for first and last two rows has not been implemented yet
void Gaussian_Blur_optimized_5x5_seperable_filter_less_div(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned int divisor, const signed char filter_5x5[5][5]);
int loop_reminder_low_reminder_values_seperable(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const __m256i cx,const __m256i cx_sh1,const __m256i cx_sh2,const __m256i cx_sh3,const __m256i cx_sh4,const __m256i cx_sh5,const __m256i cy0, const __m256i cy1,const __m256i cy2,const __m256i cy0_sh1,const __m256i cy1_sh1, const __m256i cy2_sh1,const __m256i f);
int loop_reminder_high_reminder_values_seperable(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const __m256i cx,const __m256i cx_sh1,const __m256i cx_sh2,const __m256i cx_sh3,const __m256i cx_sh4,const __m256i cx_sh5,const __m256i cy0, const __m256i cy1,const __m256i cy2,const __m256i cy0_sh1,const __m256i cy1_sh1, const __m256i cy2_sh1,const __m256i f,const unsigned int divisor);


//------------------------3x3 routines ------------------------------------
// THE MAIN ROUTINES HERE ARE THE Gaussian_Blur_3x3_16_more_load and the Gaussian_Blur_optimized_3x3_16_reg_blocking()
void Gaussian_Blur_3x3_16_more_load(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);
int loop_reminder_3x3_new(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const unsigned short int divisor,signed char **filter,const __m256i c0,const __m256i c1,const __m256i f);
int loop_reminder_3x3_new_first_last_rows(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const unsigned short int divisor,signed char **filter,const __m256i c0,const __m256i c1,const __m256i f);


void Gaussian_Blur_optimized_3x3_16_reg_blocking(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);
void Gaussian_Blur_optimized_3x3(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);
int loop_reminder_3x3(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const unsigned short int divisor,signed char **filter,const __m256i c0,const __m256i c1,const __m256i c0_sh1,const __m256i c1_sh1,const __m256i c0_sh2,const __m256i c1_sh2, const __m256i c0_sh3,const __m256i c1_sh3,const __m256i f);
int loop_reminder_3x3_first_values(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const unsigned short int divisor,signed char **filter,const __m256i c0,const __m256i c1,const __m256i c0_sh1,const __m256i c1_sh1,const __m256i c0_sh2,const __m256i c1_sh2, const __m256i c0_sh3,const __m256i c1_sh3,const __m256i f);
int loop_reminder_3x3_last_values(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const unsigned short int divisor,signed char **filter,const __m256i c0,const __m256i c1,const __m256i c0_sh1,const __m256i c1_sh1,const __m256i c0_sh2,const __m256i c1_sh2, const __m256i c0_sh3,const __m256i c1_sh3,const __m256i f);
int loop_reminder_3x3_last_row_only(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const unsigned short int divisor,signed char **filter,const __m256i c0,const __m256i c1,const __m256i c0_sh1,const __m256i c1_sh1,const __m256i c0_sh2,const __m256i c1_sh2, const __m256i c0_sh3,const __m256i c1_sh3,const __m256i f);

//3x3 routines with 32bit intermediate results
void Gaussian_Blur_optimized_3x3_reg_blocking_32(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);
void Gaussian_Blur_optimized_3x3_32(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);
int loop_reminder_3x3_32(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const unsigned short int divisor,signed char **filter,const __m256i c0,const __m256i c1,const __m256i c0_sh1,const __m256i c1_sh1,const __m256i c0_sh2,const __m256i c1_sh2, const __m256i c0_sh3,const __m256i c1_sh3,const __m256i f);
int loop_reminder_3x3_first_values_32(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const unsigned short int divisor,signed char **filter,const __m256i c0,const __m256i c1,const __m256i c0_sh1,const __m256i c1_sh1,const __m256i c0_sh2,const __m256i c1_sh2, const __m256i c0_sh3,const __m256i c1_sh3,const __m256i f);
int loop_reminder_3x3_last_values_32(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const unsigned short int divisor,signed char **filter,const __m256i c0,const __m256i c1,const __m256i c0_sh1,const __m256i c1_sh1,const __m256i c0_sh2,const __m256i c1_sh2, const __m256i c0_sh3,const __m256i c1_sh3,const __m256i f);
int loop_reminder_3x3_last_row_only_32(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const unsigned short int divisor,signed char **filter,const __m256i c0,const __m256i c1,const __m256i c0_sh1,const __m256i c1_sh1,const __m256i c0_sh2,const __m256i c1_sh2, const __m256i c0_sh3,const __m256i c1_sh3,const __m256i f);


//inefficient routines to explain the performance of the optimizations one by one
void Gaussian_Blur_3x3_16_ineff_more_LS(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);
//this is wrong
void Gaussian_Blur_3x3_16_ineff_float_div(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);
//this is wrong
__m256i float_div(__m256i a );

//------------------------5x5 routines coefficient propagation------------------------------------
void Gaussian_Blur_optimized_5x5_coeffprop(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);

//------------------------3x3 routines coefficient propagation------------------------------------
void Gaussian_Blur_optimized_3x3_coeffprop(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);

