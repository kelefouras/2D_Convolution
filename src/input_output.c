
#include "input_output.h"

char header[100];

void free_IO_arrays(){
int i;

	for (i=0;i<N;i++)
		_mm_free(frame1[i]);
	_mm_free(frame1);


	for (i=0;i<N;i++)
		_mm_free(filt[i]);
	_mm_free(filt);

	printf("\narrays are freed");
}


int create_IO_arrays(){

	int i,j;

          frame1 = _mm_malloc(N * sizeof(unsigned char *),64);
            if (frame1==NULL) {
            	printf("\nerror with malloc fr");
            	return -1;}
               for (i=0;i<N;i++){
                frame1[i]=_mm_malloc(M * sizeof(unsigned char),64);
                if (frame1[i]==NULL) {
            	   printf("\nerror with malloc fr");
            	   return -1;
                				}
                        	}

                  filt = _mm_malloc(N * sizeof(unsigned char *),64);
                   if (filt==NULL) {
                	   printf("\nerror with malloc filt");
                	   return -1;
                   	   	   }
                      for (i=0;i<N;i++){
                      	filt[i]=_mm_malloc(M * sizeof(unsigned char),64);
                        if (filt[i]==NULL) {
                    	  printf("\nerror with malloc filt");
                    	  return -1;
                      	  	  	  	  }
                               	}

                      for (i=0;i<N;i++)
                    	  for (j=0;j<M;j++)
                    		  filt[i][j]=0;

  printf("\n Arrays have been successfully created");

  return 0;
}

/*
void free_IO_arrays_with_zeros(){
int i;

	for (i=0;i<N+4;i++)
		free(frame1[i]);
	free(frame1);


	for (i=0;i<N+4;i++)
		free(filt[i]);
	free(filt);

	printf("\narrays are freed");
}


int create_IO_arrays_with_zeros(){

	int i;

          frame1 = _mm_malloc((N+4) * sizeof(unsigned char *),64);
            if (frame1==NULL) {
            	printf("\nerror with malloc fr");
            	return -1;}
               for (i=0;i<N+4;i++){
                frame1[i]=_mm_malloc((M+4) * sizeof(unsigned char),64);
                if (frame1[i]==NULL) {
            	   printf("\nerror with malloc fr");
            	   return -1;
                				}
                        	}

                  filt = _mm_malloc((N+4) * sizeof(unsigned char *),64);
                   if (filt==NULL) {
                	   printf("\nerror with malloc filt");
                	   return -1;
                   	   	   }
                      for (i=0;i<N+4;i++){
                      	filt[i]=_mm_malloc((M+4) * sizeof(unsigned char),64);
                        if (filt[i]==NULL) {
                    	  printf("\nerror with malloc filt");
                    	  return -1;
                      	  	  	  	  }
                               	}

  printf("\n Arrays have been successfully created");

  return 0;
}
*/

void read_image(char* filename)
{

 int c;
  FILE *finput;
  int i,j,temp;

  printf("\nReading %s image from disk ...",filename);
  finput=NULL;
  openfile(filename,&finput);

if ((header[0]=='P') && (header[1]=='2') ){
 for (j=0; j<N; j++){
  for (i=0; i<M; i++) {

	if( fscanf (finput,"%d",&temp) == EOF)
		exit(EXIT_FAILURE);

     frame1[j][i]= (unsigned char) temp;
  }
  }
}
else if ((header[0]=='P') && (header[1]=='5') ){
	 for (j=0; j<N; j++){
	  for (i=0; i<M; i++) {
	c=getc(finput);
	frame1[j][i]= (unsigned char) c;
	  }
	  }
	}
else {
   printf("\n problem with reading image");
   exit(EXIT_FAILURE);
}


  fclose(finput);
  printf("\nimage successfully read from disc\n");

}

/*
void read_image_and_put_zeros_around(char* filename)
{

 int c;
  FILE *finput;
  int i,j,temp;

  printf("\nReading %s image from disk ...",filename);
  finput=NULL;
  openfile(filename,&finput,0);

  if ((header[0]=='P') && (header[1]=='2') ){
   for (j=2; j<N+2; j++){
    for (i=2; i<M+2; i++) {

  	fscanf (finput,"%d",&temp);
   frame1[j][i]= (unsigned char) temp;
    }
    }
  }
  else if ((header[0]=='P') && (header[1]=='5') ){
  	 for (j=2; j<N+2; j++){
  	  for (i=2; i<M+2; i++) {
  	c=getc(finput);
  	frame1[j][i]= (unsigned char) c;
  	  }
  	  }
  	}
  else {
     printf("\n problem with reading image");
     exit(-1);
  }

  for (i=0;i<M+4;i++){//zero first two and last two rows
	  frame1[0][i]=0;
	  frame1[1][i]=0;
	  frame1[N+2][i]=0;
	  frame1[N+3][i]=0;
  }

  for (i=0;i<N+4;i++){//zero first two and last two cols
   frame1[i][0]=0;
   frame1[i][1]=0;
   frame1[i][M+2]=0;
   frame1[i][M+3]=0;
  }

  fclose(finput);
  printf("\nimage successfully read from disc\n");

}

*/


void write_image2(char* filename)
{

  FILE* foutput;
  int i,j;



  printf("  Writing result to disk ...\n");

  if ((foutput=fopen(filename,"wb"))==NULL) {
    fprintf(stderr,"Unable to open file %s for writing\n",filename);
    exit(-1);
  }

  fprintf(foutput,"P2\n");
  fprintf(foutput,"%d %d\n",M,N);
  fprintf(foutput,"%d\n",255);

  for (j=0; j<N; ++j) {
    for (i=0; i<M; ++i) {
      fprintf(foutput,"%3d ",filt[j][i]);
      if (i%32==31) fprintf(foutput,"\n");
    }
    if (M%32!=0) fprintf(foutput,"\n");
  }
  fclose(foutput);

  free_IO_arrays();
}




void openfile(char *filename, FILE** finput)
{
  int x0, y0,x;

//int aa;

  if ((*finput=fopen(filename,"rb"))==NULL) {
    fprintf(stderr,"Unable to open file %s for reading\n",filename);
    exit(-1);
  }

  if (fscanf(*finput,"%s",header) == EOF)
	  exit(EXIT_FAILURE);

  x0=getint(*finput);
  y0=getint(*finput);
  printf("\t header is %s, while x=%d,y=%d",header,x0,y0);

  M=x0;
  N=y0;

  printf("\n Image dim are M=%d,N=%d",M,N);


    if (create_IO_arrays()==-1)
	  	  exit(EXIT_FAILURE);//arrays not created



  //printf("\t header is %c %c",header[0],header[1]);

/*
  if ((x0!=M) || (y0!=N)) {
    fprintf(stderr,"Image dimensions do not match: %d x %d expected instead\n", N, M);
    exit(-1);
  }
*/

  x=getint(*finput); /* read and throw away the range info */
  printf("\n range info is %d",x);

}






int getint(FILE *fp) /* adapted from "xv" source code */
{
  int c, i, firstchar;//, garbage;

  /* note:  if it sees a '#' character, all characters from there to end of
     line are appended to the comment string */

  /* skip forward to start of next number */
  c = getc(fp);
  while (1) {
    /* eat comments */
    if (c=='#') {
      /* if we're at a comment, read to end of line */
      char cmt[256], *sp;

      sp = cmt;  firstchar = 1;
      while (1) {
        c=getc(fp);
        if (firstchar && c == ' ') firstchar = 0;  /* lop off 1 sp after # */
        else {
          if (c == '\n' || c == EOF) break;
          if ((sp-cmt)<250) *sp++ = c;
        }
      }
      *sp++ = '\n';
      *sp   = '\0';
    }

    if (c==EOF) return 0;
    if (c>='0' && c<='9') break;   /* we've found what we were looking for */

    /* see if we are getting garbage (non-whitespace) */
   // if (c!=' ' && c!='\t' && c!='\r' && c!='\n' && c!=',')
    //	garbage=1;

    c = getc(fp);
  }

  /* we're at the start of a number, continue until we hit a non-number */
  i = 0;
  while (1) {
    i = (i*10) + (c - '0');
    c = getc(fp);
    if (c==EOF) return i;
    if (c<'0' || c>'9') break;
  }
  return i;
}









