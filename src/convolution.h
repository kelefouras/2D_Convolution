//You can find here the fast Filter2D routines for non-symmetrical kernels (general case)

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


//------------------------9x9 routines ------------------------------------
// THE MAIN ROUTINE HERE IS THE convolution_optimized_9x9_32

void convolution_optimized_9x9_32(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);
__m256i main_block_pre_9x9(const __m256i r0,const __m256i r1,const __m256i r2,const __m256i r3,const __m256i r4,const __m256i r5,const __m256i r6,const __m256i r7,const __m256i r8, const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c3,const __m256i c4,const __m256i c5,const __m256i c6,const __m256i c7,const __m256i c8);
void prelude_block_3_9x9(unsigned char **frame1,unsigned char **filt,const unsigned int row,const unsigned int load_row, const unsigned int M,const unsigned int division_case, const __m256i f, const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c3,const __m256i c4,const __m256i c5,const __m256i c6,const __m256i c7);
__m256i prelude_block_pre_9x9_3(const __m256i r0,const __m256i r1,const __m256i r2,const __m256i r3,const __m256i r4,const __m256i r5,const __m256i r6,const __m256i r7, const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c3,const __m256i c4,const __m256i c5,const __m256i c6,const __m256i c7);
__m256i prelude_block_pre_9x9_3m(unsigned char **frame1,const unsigned int load_row, const unsigned int load_col, const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c3,const __m256i c4,const __m256i c5,const __m256i c6,const __m256i c7);
void prelude_block_2_9x9(unsigned char **frame1,unsigned char **filt,const unsigned int row,const unsigned int load_row, const unsigned int M,const unsigned int division_case, const __m256i f, const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c3,const __m256i c4,const __m256i c5,const __m256i c6);
__m256i prelude_block_pre_9x9_2(const __m256i r0,const __m256i r1,const __m256i r2,const __m256i r3,const __m256i r4,const __m256i r5,const __m256i r6, const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c3,const __m256i c4,const __m256i c5,const __m256i c6);
__m256i prelude_block_pre_9x9_2m(unsigned char **frame1,const unsigned int load_row, const unsigned int load_col, const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c3,const __m256i c4,const __m256i c5,const __m256i c6);
void prelude_block_1_9x9(unsigned char **frame1,unsigned char **filt,const unsigned int row,const unsigned int load_row, const unsigned int M,const unsigned int division_case, const __m256i f, const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c3,const __m256i c4,const __m256i c5);
__m256i prelude_block_pre_9x9_1(const __m256i r0,const __m256i r1,const __m256i r2,const __m256i r3,const __m256i r4,const __m256i r5, const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c3,const __m256i c4,const __m256i c5);
__m256i prelude_block_pre_9x9_1m(unsigned char **frame1,const unsigned int load_row, const unsigned int load_col, const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c3,const __m256i c4,const __m256i c5);
void prelude_block_0_9x9(unsigned char **frame1,unsigned char **filt,const unsigned int row,const unsigned int load_row, const unsigned int M,const unsigned int division_case, const __m256i f, const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c3,const __m256i c4);
__m256i prelude_block_pre_9x9_0(const __m256i r0,const __m256i r1,const __m256i r2,const __m256i r3,const __m256i r4, const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c3,const __m256i c4);
__m256i prelude_block_pre_9x9_0m(unsigned char **frame1,const unsigned int load_row, const unsigned int load_col, const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c3,const __m256i c4);

void loop_reminder_9x9(unsigned char **frame1,unsigned char **filt,const unsigned int row, const unsigned int col,const unsigned int N, const unsigned int M,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const signed char mask_vector[][32], const __m256i f,const unsigned short int divisor,signed char **filter9x9);
void default_9x9(unsigned char **frame1,unsigned char **filt,const unsigned int row,const unsigned int col_off, const unsigned int M, const unsigned int divisor,signed char **filter);
void default_9x9_pre(unsigned char **frame1,unsigned char **filt, const unsigned int row,const unsigned int col_off, const unsigned int M, const unsigned int N,const unsigned int divisor,signed char **filter);
void loop_reminder_9x9_row_boundaries(unsigned char **frame1,unsigned char **filt,const unsigned int row, const unsigned int N, const unsigned int M,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const signed char mask_vector[][32], const __m256i f,const unsigned short int divisor,signed char **filter9x9);
__m256i load_rows(unsigned char **frame1, const int col_off, const unsigned int it, const unsigned int row, const unsigned int col, const unsigned int N, const unsigned int REMINDER_ITERATIONS,const signed char mask_vector[][32]);


//------------------------7x7 routines ------------------------------------
// THE MAIN ROUTINE HERE IS THE convolution_optimized_7x7_32

//in this case, the number of coefficients is too high and therefore it is more efficient to use 1 coef and load the input many times
//reg blocking cannot be applied though
void convolution_optimized_7x7_32(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);
void loop_reminder_7x7(unsigned char **frame1,unsigned char **filt,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const signed char mask_vector[][32], const __m256i f,const unsigned short int divisor,signed char **filter7x7);
void loop_reminder_7x7_row_boundaries(unsigned char **frame1,unsigned char **filt,const unsigned int N,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const signed char mask_vector[][32], const __m256i f,const unsigned short int divisor,signed char **filter7x7);
int scalar_iterations_4(unsigned char **frame1, const unsigned int N,const unsigned int row, const unsigned int col, signed char **filter7x7);
int scalar_iterations_5(unsigned char **frame1, const unsigned int N,const unsigned int row, const unsigned int col, signed char **filter7x7);
int scalar_iterations_6(unsigned char **frame1, const unsigned int N,const unsigned int row, const unsigned int col, signed char **filter7x7);

__m256i main_block_7x7(unsigned char **frame1,const int row, const int col, const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c3,const __m256i c4,const __m256i c5,const __m256i c6,const __m256i ones,const __m256i mask_even_32);
__m256i main_block_pre_7x7(const __m256i r0,const __m256i r1,const __m256i r2,const __m256i r3,const __m256i r4,const __m256i r5,const __m256i r6, const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c3,const __m256i c4,const __m256i c5,const __m256i c6,const __m256i ones,const __m256i mask_even_32);

__m256i prelude_block_3_7x7(unsigned char **frame1,const int row, const int col, const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c3,const __m256i c4,const __m256i c5,const __m256i ones,const __m256i mask_even_32);
__m256i prelude_block_3_7x7_no_load( const __m256i r0,const __m256i r1,const __m256i r2,const __m256i r3,const __m256i r4, const __m256i r5,const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c3,const __m256i c4,const __m256i c5,const __m256i ones,const __m256i mask_even_32);
__m256i prelude_block_2_7x7(unsigned char **frame1,const int row, const int col, const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c3,const __m256i c4,const __m256i ones,const __m256i mask_even_32);
__m256i prelude_block_2_7x7_no_load( const __m256i r0,const __m256i r1,const __m256i r2,const __m256i r3,const __m256i r4, const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c3,const __m256i c4,const __m256i ones,const __m256i mask_even_32);
__m256i prelude_block_1_7x7(unsigned char **frame1,const int row, const int col, const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c3,const __m256i ones,const __m256i mask_even_32);
__m256i prelude_block_1_7x7_no_load( const __m256i r0,const __m256i r1,const __m256i r2,const __m256i r3, const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c3,const __m256i ones,const __m256i mask_even_32);

__m256i insert_one_zeros_front(__m256i r0,__m256i mask_prelude);
__m256i insert_two_zeros_front(__m256i r0,__m256i mask_prelude);
__m256i insert_three_zeros_front(__m256i r0,  __m256i mask_prelude);
__m256i insert_four_zeros_front(__m256i input, __m256i mask_prelude);

__m256i my_rshift16_by_one(__m256i m0,__m256i mask3);

//inefficient and incomplete routines
void convolution_old_7x7(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);
int convolution_old_7x7_blocking(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);


//------------------------5x5 routines ------------------------------------
// THE MAIN ROUTINE HERE IS THE convolution_optimized_5x5_reg_blocking
void convolution_optimized_5x5_reg_blocking_16(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);
void convolution_optimized_5x5_16(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);
int conv_loop_reminder_high_reminder_values_less_div(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const signed char mask_vector[][32], const __m256i f,const unsigned short int divisor,signed char **filter5x5);
int conv_loop_reminder_low_reminder_values_less_div(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const signed char mask_vector[][32],const __m256i f);
int conv_loop_reminder_first_less_div(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const signed char mask_vector[][32], const __m256i f,const unsigned short int divisor, signed char **filter5x5);
int conv_loop_reminder_last_less_div(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const signed char mask_vector[][32], const __m256i f,const unsigned short int divisor, signed char **filter5x5);
int conv_loop_reminder_last_less_div_special_case(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const signed char mask_vector[][32], const __m256i f,const unsigned short int divisor, signed char **filter5x5);

//this routine uses less coefficients by loading the input more times
void convolution_optimized_5x5_old(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);


//------------------------3x3 routines ------------------------------------
// THE MAIN ROUTINES HERE ARE THE Filter2D_3x3_16 and convolution_optimized_3x3_reg_blocking
void Filter2D_3x3_16(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);
int Filter2D_loop_reminder_3x3(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const unsigned short int divisor,signed char **filter,const __m256i c0,const __m256i c1,const __m256i c2,const __m256i f);
int Filter2D_loop_reminder_3x3_first_last_rows(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const unsigned short int divisor,signed char **filter,const __m256i c0,const __m256i c1,const __m256i c2,const __m256i f);

void convolution_optimized_3x3_reg_blocking_16(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);
void convolution_optimized_3x3_16(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned short int divisor, signed char **filter);
int conv_loop_reminder_3x3(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N,const unsigned int row, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const unsigned short int divisor,signed char **filter,const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c0_sh1,const __m256i c1_sh1,const __m256i c2_sh1,const __m256i c0_sh2,const __m256i c1_sh2,const __m256i c2_sh2, const __m256i c0_sh3,const __m256i c1_sh3,const __m256i c2_sh3,const __m256i f);
int conv_loop_reminder_3x3_first_values(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const unsigned short int divisor,signed char **filter,const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c0_sh1,const __m256i c1_sh1,const __m256i c2_sh1,const __m256i c0_sh2,const __m256i c1_sh2,const __m256i c2_sh2, const __m256i c0_sh3,const __m256i c1_sh3,const __m256i c2_sh3,const __m256i f);
int conv_loop_reminder_3x3_last_values(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const unsigned short int divisor,signed char **filter,const __m256i c0,const __m256i c1,const __m256i c2,const __m256i c0_sh1,const __m256i c1_sh1,const __m256i c2_sh1,const __m256i c0_sh2,const __m256i c1_sh2,const __m256i c2_sh2, const __m256i c0_sh3,const __m256i c1_sh3,const __m256i c2_sh3,const __m256i f);
extern int loop_reminder_3x3_last_row_only(unsigned char **frame1,unsigned char **filt,const unsigned int M, const unsigned int N, const unsigned int col,const unsigned int REMINDER_ITERATIONS,const unsigned int division_case,const unsigned short int divisor,signed char **filter,const __m256i c0,const __m256i c1,const __m256i c0_sh1,const __m256i c1_sh1,const __m256i c0_sh2,const __m256i c1_sh2, const __m256i c0_sh3,const __m256i c1_sh3,const __m256i f);


extern __m256i division(const unsigned int division_case, __m256i m2, const __m256i f);
extern const unsigned int prepare_for_division(const unsigned short int divisor);
extern __m256i division_32(const unsigned int division_case, __m256i m2, const __m256i f);
extern const unsigned int prepare_for_division_32(const unsigned int divisor);

extern void print_vector_8(__m256i v);
extern void print_vector_16(__m256i v);
extern void print_vector_32(__m256i v);
void debug(int row, int col, unsigned char **frame1, signed char **kernel);
