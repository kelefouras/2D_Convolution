
//compile using gcc main.c Gaussian_Blur.c input_output.c convolution.c -o p -O3 -march=native -mavx -lm -D_GNU_SOURCE  -g  -pthread -fopenmp
//before run read the readme.txt

#include "Filter2D.h"
#include "input_output.h"
#include "Gaussian_Blur.h"

//the kernel pairs cannot be both large as in this case maddubs instruction cannot work; output must be within [−32,767, +32,767]
void check_whether_maddubs_sacrifices_accuracy();

void check_whether_16_bit_intermediate_results_enough();

void generate_default_images(); //the default routine generates the images
void generate_optimized_images();//the optimized routine generates the images to check whether they are correct

int create_kernel();
void delete_kernel();
int measure_time(int argc, char** argv); //this routine is used to measure the execution time. To get an accurate measurement the process must run for at least 10 seconds


signed char **Mask;//2d gaussian mask with integers
unsigned short int divisor;//divisor of the gaussian mask
unsigned int kernel_size;

signed char *Separable_Mask_y;//1d gaussian mask with integers for y direction
signed char *Separable_Mask_x;//1d gaussian mask with integers for x direction
unsigned short int divisor_xy;//in gaussian blur the divisor of the Separable_Mask_x and Separable_Mask_y is the same


int main( int argc, char** argv){

unsigned char size=3; //SET the kernel size here - USE EITHER size=3 OR size=5 OR size=7 OR size=9 to initialize kernels of size 3x3,5x5,7x7,9x9
kernel_size=size;//make it global scope


if (create_kernel()!=0){
	printf("\n cannot create the gaussian mask");
	exit(EXIT_FAILURE);
}



//Routine that measures the execution time
measure_time(argc, argv);

//-------------To test the correctness of the proposed method un-comment the following routines----------------------------
//generate_default_images();
//generate_optimized_images();


//deallocate mask
delete_kernel();

    return 0;
}


int create_kernel(){

	unsigned int i;

	//allocate memory for the 2D kernel
    Mask = _mm_malloc(kernel_size * sizeof(signed char *),64);
      if (Mask==NULL) {
      	printf("\nerror with malloc fr");
      	return -1;}

      for (i=0;i<kernel_size;i++){
        	 Mask[i]=_mm_malloc(kernel_size * sizeof(signed char),64);

           if (Mask[i]==NULL) {
      	    printf("\nerror with malloc fr");
      	   return -1;
          				}
                  	}

      //allocate memory for the separable kernels (not necessary)
      Separable_Mask_y = _mm_malloc(kernel_size * sizeof(signed char ),64);
            if (Separable_Mask_y==NULL) {
            	printf("\nerror with malloc fr");
            	return -1;}

	  Separable_Mask_x = _mm_malloc(kernel_size * sizeof(signed char ),64);
		  if (Separable_Mask_x==NULL) {
			printf("\nerror with malloc fr");
			return -1;}

  if (kernel_size==5){
	  //THE SUM OF THE COEFFICIENTS MUST EQUAL TO THE DIVISOR
/*
	  Mask[0][0]=2; Mask[0][1]=4;  Mask[0][2]=5;  Mask[0][3]=4;  Mask[0][4]=2;
	  Mask[1][0]=4; Mask[1][1]=9;  Mask[1][2]=12; Mask[1][3]=9;  Mask[1][4]=4;
	  Mask[2][0]=5; Mask[2][1]=12; Mask[2][2]=15;  Mask[2][3]=12;   Mask[2][4]=5;
	  Mask[3][0]=4; Mask[3][1]=9;  Mask[3][2]=12; Mask[3][3]=9;  Mask[3][4]=4;
	  Mask[4][0]=2; Mask[4][1]=4;  Mask[4][2]=5;  Mask[4][3]=4;  Mask[4][4]=2;
	  divisor=159;
*/


	  Mask[0][0]=1; Mask[0][1]=4;  Mask[0][2]=6;  Mask[0][3]=4;  Mask[0][4]=1;
	  Mask[1][0]=4; Mask[1][1]=16;  Mask[1][2]=24; Mask[1][3]=16;  Mask[1][4]=4;
	  Mask[2][0]=6; Mask[2][1]=24; Mask[2][2]=36;  Mask[2][3]=24;   Mask[2][4]=6;
	  Mask[3][0]=4; Mask[3][1]=16;  Mask[3][2]=24; Mask[3][3]=16;  Mask[3][4]=4;
	  Mask[4][0]=1; Mask[4][1]=4;  Mask[4][2]=6;  Mask[4][3]=4;  Mask[4][4]=1;
	  divisor=256;

	  Separable_Mask_y[0]=1;Separable_Mask_y[1]=4;Separable_Mask_y[2]=6;Separable_Mask_y[3]=4;Separable_Mask_y[4]=1;
	  Separable_Mask_x[0]=1;Separable_Mask_x[1]=4;Separable_Mask_x[2]=6;Separable_Mask_x[3]=4;Separable_Mask_x[4]=1;
	  divisor_xy=16; //in separable filters(gaussian blur is separable),  divisorX==divisorY==power of 2
    //15 div 3, 100 div2
  }
 else if (kernel_size==3){
	 //THE SUM OF THE COEFFICIENTS MUST EQUAL TO THE DIVISOR

	    Mask[0][0]=1; Mask[0][1]=2;  Mask[0][2]=1;
        Mask[1][0]=2; Mask[1][1]=4;  Mask[1][2]=2;
        Mask[2][0]=1; Mask[2][1]=2;  Mask[2][2]=1;

        divisor=16;
        //17 C
        //14 B

	 /*
	    Mask[0][0]=32; Mask[0][1]=64;  Mask[0][2]=32;
        Mask[1][0]=8; Mask[1][1]=120;  Mask[1][2]=8;
        Mask[2][0]=32; Mask[2][1]=64; Mask[2][2]=32;

        divisor=392;
*/



}

 else if (kernel_size==7){
	  //THE SUM OF THE COEFFICIENTS MUST EQUAL TO THE DIVISOR
/*
 //accuracy loss because of maddubs instruction
	  Mask[0][0]=1; Mask[0][1]=4;  Mask[0][2]=7;  Mask[0][3]=10;       Mask[0][4]=7;  Mask[0][5]=4;  Mask[0][6]=1;
	  Mask[1][0]=4; Mask[1][1]=12;  Mask[1][2]=26; Mask[1][3]=33;      Mask[1][4]=26;  Mask[1][5]=12;  Mask[1][6]=4;
	  Mask[2][0]=7; Mask[2][1]=26; Mask[2][2]=55;  Mask[2][3]=71;      Mask[2][4]=55;  Mask[2][5]=26;  Mask[2][6]=7;
	  Mask[3][0]=10; Mask[3][1]=33;  Mask[3][2]=71; Mask[3][3]=91;      Mask[3][4]=71;  Mask[3][5]=33;  Mask[3][6]=10;
	  Mask[4][0]=7; Mask[4][1]=26;  Mask[4][2]=55;  Mask[4][3]=71;     Mask[4][4]=55;  Mask[4][5]=26;  Mask[4][6]=7;
	  Mask[5][0]=4; Mask[5][1]=12;  Mask[5][2]=26;  Mask[5][3]=33;     Mask[5][4]=26;  Mask[5][5]=12;  Mask[5][6]=4;
	  Mask[6][0]=1; Mask[6][1]=4;  Mask[6][2]=7;  Mask[6][3]=10;       Mask[6][4]=7;  Mask[6][5]=4;  Mask[6][6]=1;
	  divisor=1115;
*/

	  Mask[0][0]=1; Mask[0][1]=4;  Mask[0][2]=7;  Mask[0][3]=10;       Mask[0][4]=7;  Mask[0][5]=4;  Mask[0][6]=1;
	  Mask[1][0]=4; Mask[1][1]=12;  Mask[1][2]=26; Mask[1][3]=33;      Mask[1][4]=26;  Mask[1][5]=12;  Mask[1][6]=4;
	  Mask[2][0]=7; Mask[2][1]=26; Mask[2][2]=55;  Mask[2][3]=71;      Mask[2][4]=55;  Mask[2][5]=26;  Mask[2][6]=7;
	  Mask[3][0]=10; Mask[3][1]=33;  Mask[3][2]=51; Mask[3][3]=71;      Mask[3][4]=51;  Mask[3][5]=33;  Mask[3][6]=10;
	  Mask[4][0]=7; Mask[4][1]=26;  Mask[4][2]=55;  Mask[4][3]=71;     Mask[4][4]=55;  Mask[4][5]=26;  Mask[4][6]=7;
	  Mask[5][0]=4; Mask[5][1]=12;  Mask[5][2]=26;  Mask[5][3]=33;     Mask[5][4]=26;  Mask[5][5]=12;  Mask[5][6]=4;
	  Mask[6][0]=1; Mask[6][1]=4;  Mask[6][2]=7;  Mask[6][3]=10;       Mask[6][4]=7;  Mask[6][5]=4;  Mask[6][6]=1;
	  divisor=1115-60;//div 3
	 // divisor=1200;//div 2
	 // divisor=1024;

	  Separable_Mask_y[0]=1;Separable_Mask_y[1]=6;Separable_Mask_y[2]=15;Separable_Mask_y[3]=20;Separable_Mask_y[4]=15;Separable_Mask_y[5]=6;Separable_Mask_y[6]=1;
	  Separable_Mask_x[0]=1;Separable_Mask_x[1]=6;Separable_Mask_x[2]=15;Separable_Mask_x[3]=20;Separable_Mask_x[4]=15;Separable_Mask_x[5]=6;Separable_Mask_x[6]=1;
	  divisor_xy=64; //in separable filters(gaussian blur is separable),  divisorX==divisorY==power of 2
	 /*
	  Mask[0][0]=1; Mask[0][1]=6;  Mask[0][2]=15;  Mask[0][3]=20;       Mask[0][4]=15;  Mask[0][5]=6;  Mask[0][6]=1;
	  Mask[1][0]=6; Mask[1][1]=36;  Mask[1][2]=90; Mask[1][3]=120;      Mask[1][4]=90;  Mask[1][5]=36;  Mask[1][6]=6;
	  Mask[2][0]=15; Mask[2][1]=90; Mask[2][2]=225;  Mask[2][3]=300;    Mask[2][4]=225;  Mask[2][5]=90;  Mask[2][6]=15;
	  Mask[3][0]=20; Mask[3][1]=120;  Mask[3][2]=300; Mask[3][3]=400;   Mask[3][4]=300;  Mask[3][5]=120;  Mask[3][6]=20;
	  Mask[4][0]=15; Mask[4][1]=90;  Mask[4][2]=225;  Mask[4][3]=300;   Mask[4][4]=225;  Mask[4][5]=90;  Mask[4][6]=15;
	  Mask[5][0]=6; Mask[5][1]=36;  Mask[5][2]=90;  Mask[5][3]=120;     Mask[5][4]=90;  Mask[5][5]=36;  Mask[5][6]=6;
	  Mask[6][0]=1; Mask[6][1]=6;  Mask[6][2]=15;  Mask[6][3]=20;       Mask[6][4]=15;  Mask[6][5]=6;  Mask[6][6]=1;
	  divisor=4096;
	  */
 }
 else if (kernel_size==9){

	  Separable_Mask_y[0]=1;Separable_Mask_y[1]=8;Separable_Mask_y[2]=28;Separable_Mask_y[3]=56;Separable_Mask_y[4]=70;Separable_Mask_y[5]=56;Separable_Mask_y[6]=28;Separable_Mask_y[7]=8;Separable_Mask_y[8]=1;
	  Separable_Mask_x[0]=1;Separable_Mask_x[1]=8;Separable_Mask_x[2]=28;Separable_Mask_x[3]=56;Separable_Mask_x[4]=70;Separable_Mask_x[5]=56;Separable_Mask_x[6]=28;Separable_Mask_x[7]=8;Separable_Mask_x[8]=1;
	  divisor_xy=256; //in separable filters(gaussian blur is separable),  divisorX==divisorY==power of 2

	  Mask[0][0]=1; Mask[0][1]=2;  Mask[0][2]=3;  Mask[0][3]=4;     Mask[0][4]=5;  Mask[0][5]=4;    Mask[0][6]=3;    Mask[0][7]=2;  Mask[0][8]=1;
	  Mask[1][0]=1; Mask[1][1]=2;  Mask[1][2]=4; Mask[1][3]=6;      Mask[1][4]=8;  Mask[1][5]=6;    Mask[1][6]=4;    Mask[1][7]=2;  Mask[1][8]=1;
	  Mask[2][0]=1; Mask[2][1]=2; Mask[2][2]=4;  Mask[2][3]=8;      Mask[2][4]=16;  Mask[2][5]=12;  Mask[2][6]=8;   Mask[2][7]=4;  Mask[2][8]=1;
	  Mask[3][0]=1; Mask[3][1]=8;  Mask[3][2]=16; Mask[3][3]=24;    Mask[3][4]=32;  Mask[3][5]=24;  Mask[3][6]=16;   Mask[3][7]=8;  Mask[3][8]=1;
	  Mask[4][0]=1; Mask[4][1]=16;  Mask[4][2]=32;  Mask[4][3]=48;  Mask[4][4]=64;  Mask[4][5]=48;  Mask[4][6]=32;   Mask[4][7]=16;  Mask[4][8]=1;
	  Mask[5][0]=1; Mask[5][1]=8;  Mask[5][2]=16;  Mask[5][3]=24;   Mask[5][4]=32;  Mask[5][5]=24;  Mask[5][6]=16;    Mask[5][7]=8;  Mask[5][8]=1;
	  Mask[6][0]=1; Mask[6][1]=4;  Mask[6][2]=8;  Mask[6][3]=12;    Mask[6][4]=16;  Mask[6][5]=12;  Mask[6][6]=8;    Mask[6][7]=4;  Mask[6][8]=1;
	  Mask[7][0]=1; Mask[7][1]=2;  Mask[7][2]=4;  Mask[7][3]=6;     Mask[7][4]=8;  Mask[7][5]=6;    Mask[7][6]=4;    Mask[7][7]=2;  Mask[7][8]=1;
	  Mask[8][0]=1; Mask[8][1]=2;  Mask[8][2]=3;  Mask[8][3]=4;     Mask[8][4]=5;  Mask[8][5]=4;    Mask[8][6]=3;    Mask[8][7]=2;  Mask[8][8]=1;

//divisor=758;//div 2
//divisor=700;//div 3
	  divisor=1024;
 }

 else {
	 printf("\nWrong kernel size");
	 exit(EXIT_FAILURE);
 }

  //check_whether_maddubs_sacrifices_accuracy();
  //check_whether_16_bit_intermediate_results_enough();

  //IMPORTANT: although the routines work for negative kernel values, they cannot be used here because the output pixel value might be negative (which is bad); in this case, the negative value will not be capped to zero. I have not done this, as it needs extra instructions, degrading performance. In gaussian blur no negative values are used.
  return 0;
}


void delete_kernel(){

	for(int i = 0; i < kernel_size; i++)
	    _mm_free(Mask[i]);

	_mm_free(Mask);

	_mm_free(Separable_Mask_x);
	_mm_free(Separable_Mask_y);

}

int measure_time(int argc, char** argv){


	double start_time, run_time;

	if (argc!=3)
		return -1;

	int arg1=strtol(argv[1], NULL, 10);
	char *arg2;
	arg2=argv[2];
    printf("\n read image %s and computed %d times",arg2,arg1);

	start_time = omp_get_wtime();


	read_image(arg2);


	for (int i=0;i<arg1;i++){
		//convolution_optimized_5x5_reg_blocking_16(frame1,filt,M,N,divisor,Mask);
	//	convolution_optimized_5x5_old(frame1,filt,M,N,divisor,Mask);
	convolution_optimized_3x3_reg_blocking_16(frame1,filt,M,N,divisor,Mask);
		//Gaussian_Blur_optimized_5x5_16_seperable(frame1,filt,M,N,Separable_Mask_y,Separable_Mask_x,divisor_xy);
		//Gaussian_Blur_3x3_16_new_todo(frame1,filt,M,N,divisor,Mask);
	// Gaussian_Blur_optimized_5x5_step28_less_div_reg_blocking(frame1,filt,M,N,divisor,Mask);
	 //Gaussian_Blur_optimized_3x3_reg_blocking(frame1,filt,M,N,divisor,Mask);
	// Gaussian_Blur_optimized_3x3_reg_blocking_32(frame1,filt,M,N,divisor,Mask);
	//	Gaussian_Blur_3x3_16_inefficient_LS(frame1,filt,M,N,divisor,Mask);
		//Filter2D_3x3_16(frame1,filt,M,N,divisor,Mask);
		//Gaussian_Blur_7x7_16_separable(frame1,filt,M,N,Separable_Mask_y,Separable_Mask_x,divisor_xy);
		//Gaussian_Blur_9x9_16_separable(frame1,filt,M,N,Separable_Mask_y,Separable_Mask_x,divisor_xy);
		//Gaussian_Blur_9x9_16_separable_extra_array_less_load(frame1,filt,M,N,Separable_Mask_y,Separable_Mask_x,divisor_xy);
		//Gaussian_Blur_3x3_16_ineff_float_div(frame1,filt,M,N,divisor,Mask);
		//Gaussian_Blur_3x3_16_ineff_more_LS(frame1,filt,M,N,divisor,Mask);
		//convolution_optimized_9x9_32(frame1,filt,M,N,divisor,Mask);
		//Gaussian_Blur_default(frame1,filt,M,N,kernel_size,divisor,Mask);
	}
	run_time = omp_get_wtime() - start_time;
	printf("\n omp time is %f seconds \n", run_time);


	write_image2(OUT1);
	return 0;
}




void generate_default_images(){

	int i,extension=0;


	//generate all the images
	for (i=0;i<31;i++){
		sprintf(append,"%d",extension);
		strcat(In_path,IN);
		strcat(In_path,append);
		strcat(In_path,".pgm");

		strcat(Out_path,OUT);
		strcat(Out_path,append);
		strcat(Out_path,"_correct.pgm");

		read_image(In_path);

		//Conv_default_separable(frame1,filt,M,N,kernel_size,Separable_Mask_y,Separable_Mask_x,divisor_xy);
		Conv_default(frame1,filt,M,N,kernel_size,divisor,Mask);

		write_image2(Out_path); //store output image to the disc

		extension++;
		In_path[0]='\0';
		Out_path[0]='\0';
	}

}


void generate_optimized_images(){

	int i,extension=0;


	//generate all the images
	for (i=0;i<31;i++){

		sprintf(append,"%d",extension);
		strcat(In_path,IN);
		strcat(In_path,append);
		strcat(In_path,".pgm");

		strcat(Out_path,OUT);
		strcat(Out_path,append);
		strcat(Out_path,".pgm");

		read_image(In_path);
		if (kernel_size==3){
			//Gaussian_Blur_optimized_3x3_32(frame1,filt,M,N,divisor,Mask);
			//Gaussian_Blur_optimized_3x3_reg_blocking_32(frame1,filt,M,N,divisor,Mask);
			Gaussian_Blur_optimized_3x3_16_reg_blocking(frame1,filt,M,N,divisor,Mask);
			//Gaussian_Blur_optimized_3x3_coeffprop(frame1,filt,M,N,divisor,Mask);
			//convolution_optimized_3x3_reg_blocking_16(frame1,filt,M,N,divisor,Mask);
			//Gaussian_Blur_3x3_16_new_todo(frame1,filt,M,N,divisor,Mask);
			//Filter2D_3x3_16(frame1,filt,M,N,divisor,Mask);


		}
		else if (kernel_size==5){
			Gaussian_Blur_optimized_5x5_16_reg_blocking(frame1,filt,M,N,divisor,Mask);
			//convolution_optimized_5x5_reg_blocking_16(frame1,filt,M,N,divisor,Mask);
			//Gaussian_Blur_optimized_5x5_16_seperable(frame1,filt,M,N,Separable_Mask_y,Separable_Mask_x,divisor_xy);
			//convolution_optimized_5x5_old(frame1,filt,M,N,divisor,Mask);

		}
		else if (kernel_size==7){
			//convolution_optimized_7x7_32(frame1,filt,M,N,divisor,Mask);
			Gaussian_Blur_7x7_32_separable(frame1,filt,M,N,Separable_Mask_y,Separable_Mask_x,divisor_xy);

		}
		else if (kernel_size==9){
			//Gaussian_Blur_9x9_32_separable(frame1,filt,M,N,Separable_Mask_y,Separable_Mask_x,divisor_xy);
			convolution_optimized_9x9_32(frame1,filt,M,N,divisor,Mask);

		}
		else {
			printf("\n THE KERNEL SIZE IS WRONG");
		}

		write_image2(Out_path); //store output image to the disc

		extension++;
		In_path[0]='\0';
		Out_path[0]='\0';
	}

}

void check_whether_maddubs_sacrifices_accuracy(){

	int i,j,temp,flag=0;

	printf("\n----- MADDUBS ACCURACY REPORT -------");

	for (i=0;i<kernel_size;i++){
		for (j=1;j<kernel_size;j++){
			temp=Mask[i][j-1]*255 + Mask[i][j]*255;
			if ( ( temp > 32767 ) || (temp<-32767) ){
				flag=-1;
			}
		}
	}

	if (flag==-1){
		printf("\nWARNING - GENERAL CASE: ACCURACY LOSS IS LIKELY BECAUSE OF MADDUBS INSTRUCTION");
	}
	else {
		printf("\nGENERAL CASE: NO ACCURACY LOSS IS LIKELY BECAUSE OF MADDUBS INSTRUCTION");
	}


//--repeat for Separable_Mask_y
flag=0;
	for (j=1;j<kernel_size;j++){
		temp=Separable_Mask_y[j-1]*255 + Separable_Mask_y[j]*255;
		if ( ( temp > 32767 ) || (temp<-32767) ){
			flag=-1;
		}
	}
	if (flag==-1){
		printf("\nWARNING - SEPARABLE FILTER Y: ACCURACY LOSS IS LIKELY BECAUSE OF MADDUBS INSTRUCTION");
	}
	else {
		printf("\nSEPARABLE FILTER Y: NO ACCURACY LOSS IS LIKELY BECAUSE OF MADDUBS INSTRUCTION");
	}

	//--repeat for Separable_Mask_x
	flag=0;
		for (j=1;j<kernel_size;j++){
			temp=Separable_Mask_x[j-1]*255 + Separable_Mask_x[j]*255;
			if ( ( temp > 32767 ) || (temp<-32767) ){
				flag=-1;
			}
		}
		if (flag==-1){
			printf("\nWARNING - SEPARABLE FILTER X: ACCURACY LOSS IS LIKELY BECAUSE OF MADDUBS INSTRUCTION");
		}
		else {
			printf("\nSEPARABLE FILTER X: NO ACCURACY LOSS IS LIKELY BECAUSE OF MADDUBS INSTRUCTION");
		}
}



void check_whether_16_bit_intermediate_results_enough(){

	int i,j,temp=0;

	for (i=0;i<kernel_size;i++){
		for (j=0;j<kernel_size;j++){
			temp+=Mask[i][j];
		}
	}

	printf("\n----- INTERMEDIATE RESULTS ACCURACY REPORT -------");

	if ((temp*255)>65535)
		printf("\nWARNING - FOR THE GENERAL CASE ACCURACY LOSS USING 16-BIT");
	//else if ((temp*255)>32767)
	//	printf("\nWARNING - FOR THE GENERAL CASE ACCURACY LOSS IS LIKELY USING 16-BIT");
	else
		printf("\nNO ACCURACY LOSS USING 16-BIT FOR THE GENERAL CASE");


	temp=0;
	for (j=0;j<kernel_size;j++){
		temp+=Separable_Mask_y[j];
	}

	if ((temp*255)>65535)
		printf("\nWARNING - SEPARABLE FILTER Y: ACCURACY LOSS USING 16-BIT");
	//else if ((temp*255)>32767)
	//	printf("\nWARNING - SEPARABLE FILTER Y: ACCURACY LOSS IS LIKELY USING 16-BIT");
	else
		printf("\nSEPARABLE FILTER Y: NO ACCURACY LOSS USING 16-BIT");

	temp=0;
	for (j=0;j<kernel_size;j++){
		temp+=Separable_Mask_x[j];
	}

	if ((temp*255)>65535)
		printf("\nWARNING - SEPARABLE FILTER x: ACCURACY LOSS USING 16-BIT");
	//else if ((temp*255)>32767)
	//	printf("\nWARNING - SEPARABLE FILTER x: ACCURACY LOSS IS LIKELY USING 16-BIT");
	else
		printf("\nSEPARABLE FILTER x: NO ACCURACY LOSS USING 16-BIT");

}

//for 7x7 and higher, there are too many coefficients thus it is more efficient to use k coeff and load the input many times.
//if I load many times, then more pixels are computed in each iteration, but there are more load instructions and no reg blocking. Plus in 5x5 I need some extra instructions
//in 5x5 conv loading using many coeffs instead of loading many times is about x1.2 faster. in 3x3 is the opposite. There is a trade off between more calculated pixels per iteration vs less load instructions. in 3x3 the num of extra loads is low, while in 5x5 is  high and thus the results are these.

//for 7x7 and higher, we need 32-bit, unless we use separable filters
//for the largest input size, reg blocking is not efficient. however, for separable filters is ?

