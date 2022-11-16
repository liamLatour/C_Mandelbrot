#include <stdio.h>

/*
	compile with: gcc -W -Wall -ansi -pedantic -o mandel mandel.c
	https://programmationenlangagec.pages.emse.fr/des-calculs-et-des-images/Mandelbrot.html
	https://programmationenlangagec.pages.emse.fr/des-calculs-et-des-images/Grille_evaluation.html
*/

#define WIDTH 1000 
#define HEIGHT 1000 

double map(int v, int imin, int imax, double omin, double omax);

int main(void)
{
	int img[WIDTH][HEIGHT];
	FILE * fout;
	int max_iter = 100;
	int radius = 2;
	int py = 0;
	int px = 0;
	int i = 0;
	int j = 0;

	for(py=0; py<HEIGHT; py++){
		for(px=0; px<WIDTH; px++){
			int iter = 0;
			double x0 = map(px, 0, WIDTH, -2.0, 0.28);
			double y0 = map(py, 0, HEIGHT, -1.24, 1.24);
			double xn = x0;
			double yn = y0;
			double xn2 = xn*xn;
			double yn2 = yn*yn;

			while(iter<max_iter && xn*xn+yn*yn < radius*radius){
				yn = 2*xn*yn + y0;
				xn = xn2 - yn2 + x0;
				xn2 = xn*xn;
				yn2 = yn*yn;
				iter++;
			}
			
			img[px][py] = iter;
		}
	}

	fout = fopen("mandel.pbm", "w");
	if ( fout == NULL ) {
		return 1;
	} 

	fputs("P1\n", fout);
	fprintf(fout, "%d %d\n", WIDTH, HEIGHT);
	fprintf(fout, "#max_iter = %d\n", max_iter);
	fprintf(fout, "#zone = %d\n", max_iter);

	for(i=0; i<HEIGHT; i++){
		for(j=0; j<WIDTH; j++){			
			if(img[j][i] == max_iter){
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