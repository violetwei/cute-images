/* FILE: A3_solutions.c is where you will code your answers for Assignment 3.
 * 
 * Each of the functions below can be considered a start for you. They have 
 * the correct specification and are set up correctly with the header file to
 * be run by the tester programs.  
 *
 * You should leave all of the code as is, especially making sure not to change
 * any return types, function name, or argument lists, as this will break
 * the automated testing. 
 *
 * Your code should only go within the sections surrounded by
 * comments like "REPLACE EVERTHING FROM HERE... TO HERE.
 *
 * The assignment document and the header A3_solutions.h should help
 * to find out how to complete and test the functions. Good luck!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "A3_provided_functions.h"

unsigned char*
bmp_open( char* bmp_filename,        unsigned int *width, 
          unsigned int *height,      unsigned int *bits_per_pixel, 
          unsigned int *padding,     unsigned int *data_size, 
          unsigned int *data_offset                                  )
{
  unsigned char *img_data=NULL;
  // REPLACE EVERYTHING FROM HERE
  //printf( "UNIMPLEMENTED FUNCTION: bmp_open has not yet been coded. Please complete before submitting!\n");
   FILE *bmpfile = fopen(bmp_filename, "rb");

  if(bmpfile == NULL){
    return img_data;
  }
  
  char b, m;
  fread(&b, 1, 1, bmpfile);
  fread(&m, 1, 1, bmpfile);

  //read the overall file size
  unsigned int overallFileSize;
  fread(&overallFileSize, 1, sizeof(unsigned int), bmpfile);

  //Rewind file pointer to the beginning and read the entire contents
  rewind(bmpfile);

  // malloc a piece of heap memory to store bmp data
  unsigned char *mallocData= (unsigned char*)malloc(sizeof(unsigned char) * overallFileSize);
  fread(mallocData, 1, overallFileSize, bmpfile);

  // helper_p1 and helper_p2 are helper pointers interpreting data in mallocData
  // they point to some place of the data array
  // then dereference the value of helper_p1 and helper_p2

  *data_size = overallFileSize;

  unsigned int *helper_p1 = (unsigned int*)(mallocData+18);
  *width= *helper_p1;

  helper_p1 = (unsigned int*)(mallocData+22);
  *height= *helper_p1;

  helper_p1= (unsigned int*)(mallocData+10);
  *data_offset= *helper_p1;

  unsigned short int *helper_p2= (unsigned short int*)(mallocData+28);
  *bits_per_pixel= *helper_p2;

  *padding = ((*bits_per_pixel)/8)%4;

  img_data= mallocData;
  
  // TO HERE!  
  return img_data;  
}

void 
bmp_close( unsigned char **img_data )
{
  // REPLACE EVERYTHING FROM HERE
  //printf( "UNIMPLEMENTED FUNCTION: bmp_close has not yet been coded. Please complete before submitting!\n");
  if(*img_data != NULL){
    free(*img_data);
    //img_data=NULL;
  }
  // TO HERE!  
}

unsigned char***  
bmp_scale( unsigned char*** pixel_array, unsigned char* header_data, unsigned int header_size,
           unsigned int* width, unsigned int* height, unsigned int num_colors,
           float scale )
{
  unsigned char*** new_pixel_array = NULL; 
  // REPLACE EVERYTHING FROM HERE
  //printf( "UNIMPLEMENTED FUNCTION: bmp_scale has not yet been coded. Please complete before submitting!\n");

  if(pixel_array == NULL){
    return NULL;
  }

  unsigned int *help_p1 = (unsigned int*)(header_data+18);
  *width = (unsigned int)((float)(*help_p1) * scale);
  *help_p1 = *width;

  unsigned int *help_p2 = (unsigned int*)(header_data+22);
  *height=(unsigned int)((float)(*help_p2) * scale);
  *help_p2 = *height;


  //allocate new heap memory for the 3D scaled array
  unsigned char*** malloc_3DHeap=(unsigned char***)malloc((*height) * sizeof(unsigned char**));
  int i, j;

  for(i=0; i<*height; i++){
    malloc_3DHeap[i]=(unsigned char**)malloc((*width) * sizeof(unsigned char*));
    for(j=0; j<*width; j++){
      malloc_3DHeap[i][j]=(unsigned char*)malloc(num_colors * sizeof(unsigned char));
    }
  }

  //for(int k=0; k<num_colors; k++){
    for(i=0; i<*help_p2; i++){
      for(j=0; j<*help_p1; j++){
        malloc_3DHeap[i][j]=pixel_array[(int)(i/scale)][(int)(j/scale)];//[num_colors];
      }
    }
  //}

  new_pixel_array=malloc_3DHeap;

  // TO HERE! 
  return new_pixel_array;
}         

int 
bmp_collage( char* background_image_filename,     char* foreground_image_filename, 
             char* output_collage_image_filename, int row_offset,                  
             int col_offset,                      float scale )
{
  // REPLACE EVERYTHING FROM HERE
  //printf( "UNIMPLEMENTED FUNCTION: bmp_collage has not yet been coded. Please complete before submitting!\n");

  unsigned char*   header_data_f;
  unsigned int     header_size_f, width_f, height_f, num_colors_f;
  unsigned char*** pixel_array_foreground = NULL;


  unsigned char*   header_data_b;
  unsigned int     header_size_b, width_b, height_b, num_colors_b;
  unsigned char*** pixel_array_background = NULL;
  
  pixel_array_foreground = bmp_to_3D_array( foreground_image_filename,       &header_data_f, 
                                 &header_size_f,  &width_f, 
                                 &height_f,       &num_colors_f   );


  unsigned char*** scaled_pixel_array = bmp_scale( pixel_array_foreground, header_data_f, header_size_f,
                                                   &width_f, &height_f, num_colors_f,  scale   );


  pixel_array_background = bmp_to_3D_array( background_image_filename,       &header_data_b, 
                                 &header_size_b,  &width_b, 
                                 &height_b,       &num_colors_b   );

  int r=0;
  int c=0;
  

  
  for(int i=row_offset; i<row_offset+height_f; i++){
    for(int j=col_offset; j<col_offset+width_f; j++){
      int color = 0;
      if(scaled_pixel_array[r][c][color] != 0){
        for(; color<num_colors_f; color++){
          pixel_array_background[i][j][color]=scaled_pixel_array[r][c][color];
        }
      }
      c++;
    }
    r++;
    c=0;
  }
  
  bmp_from_3D_array( output_collage_image_filename, header_data_b, header_size_b, pixel_array_background,
                     width_b,   height_b,      num_colors_b                       );


  // TO HERE! 
  return 0;
}              

