#include <stdio.h>

/*
	compile with: gcc -W -Wall -ansi -pedantic -o mandel mandel.c
	https://programmationenlangagec.pages.emse.fr/des-calculs-et-des-images/Mandelbrot.html
	https://programmationenlangagec.pages.emse.fr/des-calculs-et-des-images/Grille_evaluation.html
*/

double map(int v, int imin, int imax, double omin, double omax);

int main(void)
{
	int H = 70;
	int W = 200;
	int max_iter = 100;
	int radius = 2;
	int py = 0;
	int px = 0;

	for(py=0; py<H; py++){
		for(px=0; px<W; px++){
			int iter = 0;
			double x0 = map(px, 0, W, -2.0, 0.28);
			double y0 = map(py, 0, H, -1.24, 1.24);
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
			
			if(iter != max_iter){
				printf(" ");
			}else{
				printf("*");
			}
		}
		printf("\n");
	}

	return 0 ;
}

double map(int v, int imin, int imax, double omin, double omax){
	double range = imax - imin;
	double percentage = (v - imin) / range;

	return omin + percentage * (omax - omin);
}