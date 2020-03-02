#include "bmp.h"
#include <stdio.h>

FILE* myfopen(const char* fname, const char* mode){
	FILE* open = fopen(fname, mode);
	if(open == NULL){
		fprintf(stderr, "Cannot open file %s in mode %s\n", fname, mode);
		exit(EXIT_FAILURE);
	}
	return open;
}

typedef struct{
	int file_size;
	int reserved;
	int offset;
	int DIB_size;
	int width;
	int height;
	short plane;
	short depth;
	int compression;
	int data_size;
	int X_resolution;
	int Y_resolution;
	int number_of_colors;
	int number_of_important_colors;
}header;

typedef struct{
	unsigned char B;
	unsigned char G;
	unsigned char R;
	unsigned char zero;
}palette;

void bmp_save(const char* fname, size_t width, size_t height, size_t depth, color_t (*pixel)(size_t i, size_t j, void** args), void** args){
	FILE* fd = myfopen(fname, "wb");

	fputc('B', fd);
	fputc('M', fd);

	size_t R = 4*((depth*width+31)/32);
	size_t M = height*R;
	size_t C = 1L<<depth;
	if(depth == 24) C = 0;
	size_t P = 4*C;	
	header h;
	h.file_size = 54+P+M; 	
	h.reserved = 0; 			
	h.offset = 54+P; 		
	h.DIB_size = 40;			
	h.width = width;		
	h.height = height;		
	h.plane = 1;			
	h.depth = depth;		
	h.compression = 0;			
	h.data_size = M;			
	h.X_resolution = 2835;		
	h.Y_resolution = 2835;		
	h.number_of_colors = C;			
	h.number_of_important_colors = 0;

	fwrite(&h, sizeof(h), 1, fd);


	if(depth == 24){
		unsigned char (*line)[3] = calloc(R, sizeof(*line));
		if(line == NULL) exit(EXIT_FAILURE);
		for(int j = (int)height-1; j>=0; j--){
			for(int i = 0; i<(int)width; i++){
				color_to_BGR(pixel(i, j, args), *(line+i));
			}
			fwrite(line, R, 1, fd);
		}
		free(line);
	}else if(depth == 8){
		unsigned char* line = NULL;
		line = calloc(R, sizeof(unsigned char));
		if(line == NULL) exit(EXIT_FAILURE);
		palette p[256];
		for(int i = 0x00; i <= 0xff; i++){
			p[i].B = i;
			p[i].G = i;
			p[i].R = i;
			p[i].zero = 0x00;
		}
		
		fwrite(p, sizeof(p), 1, fd);
		
		for(int j = height-1; j>=0; j--){
			for(int i = 0; i<(int)width; i++){
				line[i] = color_to_graylevel(pixel(i, j, args));
			}
			fwrite(line, R, 1, fd);
		}
		free(line);
	}else if(depth == 1){
		unsigned char* line = NULL;
		line = calloc(R, sizeof(unsigned char));
		if(line == NULL) exit(EXIT_FAILURE);
		palette p[2];
		p[0].B = 0x00;
		p[0].G = 0x00;
		p[0].R = 0x00;
		p[0].zero = 0x00;
		p[1].B = 0xff;
		p[1].G = 0xff;
		p[1].R = 0xff;
		p[1].zero = 0x00;
		fwrite(p, sizeof(p), 1, fd);
		for(int j = height-1; j>=0; j--){
			for(int i = 0; i<(int)width; i++){
				line[(i/8)] |= (color_to_BW(pixel(i, j, args)))<<(7-(i%8));
			}
			fwrite(line, R, 1, fd);
			for(int k = 0; k<(int)R; k++) line[k] = 0;
		}
		free(line);
	}
	fclose(fd);
}
