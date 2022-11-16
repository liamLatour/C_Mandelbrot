#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
	compile with: gcc -W -Wall -ansi -pedantic -o mandel mandel.c
	https://programmationenlangagec.pages.emse.fr/des-calculs-et-des-images/Mandelbrot.html
	https://programmationenlangagec.pages.emse.fr/des-calculs-et-des-images/Grille_evaluation.html
*/

#define WIDTH 1000 
#define HEIGHT 1000 
#define STRMAX 256

struct render
{
	int width;
	int height;
	double min_reel;
	double min_comp;
	double max_reel;
	double max_comp;
	int max_iter;
	int radius;
	int img[WIDTH][HEIGHT];
	char basename[STRMAX];
};


double map(int v, int imin, int imax, double omin, double omax);

int main(void)
{
	FILE * fout;
	int py;
	int px;
	int j;
	int i;
	struct render set;

	strcpy(set.basename, "mandel");
	set.width = 1000;
	set.height = 1000;
	set.min_reel = -2.0;
	set.min_comp = -1.24;
	set.max_reel = 0.28;
	set.max_comp = 1.24;
	set.max_iter = 100;
	set.radius = 2;

	for(py=0; py<set.height; py++){
		for(px=0; px<set.width; px++){
			int iter = 0;
			double x0 = map(px, 0, set.width, set.min_reel, set.max_reel);
			double y0 = map(py, 0, set.height, set.min_comp, set.max_comp);
			double xn = x0;
			double yn = y0;
			double xn2 = xn*xn;
			double yn2 = yn*yn;

			while(iter<set.max_iter && xn*xn+yn*yn < set.radius*set.radius){
				yn = 2*xn*yn + y0;
				xn = xn2 - yn2 + x0;
				xn2 = xn*xn;
				yn2 = yn*yn;
				iter++;
			}
			
			set.img[px][py] = iter;
		}
	}

	fout = fopen("mandel.pbm", "w");
	if ( fout == NULL ) {
		return 1;
	} 

	fputs("P1\n", fout);
	fprintf(fout, "%d %d\n", WIDTH, HEIGHT);
	fprintf(fout, "#max_iter = %d\n", set.max_iter);
	fprintf(fout, "#zone = %f\n", set.min_reel);

	for(i=0; i<HEIGHT; i++){
		for(j=0; j<WIDTH; j++){			
			if(set.img[j][i] == set.max_iter){
				fputc('1', fout);
			}else{
				fputc('0', fout);
			}
			
			if(j % 35 == 0){
				fputc('\n', fout);
			}else{
				fputc(' ', fout);
			}
		}
	}

	fclose(fout);

	return 0 ;
}

double map(int v, int imin, int imax, double omin, double omax){
	double range = imax - imin;
	double percentage = (v - imin) / range;

	return omin + percentage * (omax - omin);
}