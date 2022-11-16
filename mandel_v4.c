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

struct camera
{
	double coord_x;
	double coord_y;
	double width;
	double height;
};

struct render
{
	int width;
	int height;
	double xmin;
	double ymin;
	double xmax;
	double ymax;
	int max_iter;
	int radius;
	int img[WIDTH][HEIGHT];
	char basename[STRMAX];
	struct camera pov;
};


double map(int v, int imin, int imax, double omin, double omax);
void render_init(struct render *set);
void render_image(struct render *set);
void image_save_bw(struct render *set);
void image_save_altern(struct render *set);
void cam2rect(struct render *set, struct camera *pov);

int main(void)
{
	struct render set;
	struct camera pov = {
		-0.76,
		0,
		2.48,
		2.48
	};

	set.pov = pov;

	render_init(&set);
	cam2rect(&set, &(set.pov));
	render_image(&set);
	image_save_altern(&set);

	return 0 ;
}

void cam2rect(struct render *set, struct camera *pov){
	set->xmin = pov->coord_x - pov->width/2.0;
	set->xmax = pov->coord_x + pov->width/2.0;
	set->ymin = pov->coord_y - pov->height/2.0;
	set->ymax = pov->coord_y + pov->height/2.0;
}

void image_save_bw(struct render *set){
	FILE * fout;
	int j;
	int i;

	fout = fopen(strcat(set->basename, ".pbm"), "w");
	if ( fout == NULL ) {
		exit(1);
	} 

	fputs("P1\n", fout);
	fprintf(fout, "%d %d\n", set->width, set->height);
	fprintf(fout, "#max_iter = %d\n", set->max_iter);
	fprintf(fout, "#zone = %f\n", set->xmin);

	for(i=0; i<set->height; i++){
		for(j=0; j<set->width; j++){			
			if(set->img[j][i] == set->max_iter){
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
}

void image_save_altern(struct render *set){
		FILE * fout;
	int j;
	int i;

	fout = fopen(strcat(set->basename, ".pbm"), "w");
	if ( fout == NULL ) {
		exit(1);
	} 

	fputs("P1\n", fout);
	fprintf(fout, "%d %d\n", set->width, set->height);
	fprintf(fout, "#max_iter = %d\n", set->max_iter);
	fprintf(fout, "#zone = %f\n", set->xmin);

	for(i=0; i<set->height; i++){
		for(j=0; j<set->width; j++){			
			if(set->img[j][i] == set->max_iter || set->img[j][i]%2 == 1){
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
}

/* N’oubliez pas de déplacer également les variables nécessaires à ce traitement */
void render_image(struct render *set){
	int py;
	int px;

	for(py=0; py<set->height; py++){
		double y0 = map(py, 0, set->height, set->ymin, set->ymax);
		for(px=0; px<set->width; px++){
			int iter = 0;
			double x0 = map(px, 0, set->width,  set->xmin, set->xmax);
			double xn = 0;
			double yn = 0;
			double xn2 = xn*xn;
			double yn2 = yn*yn;

			while(iter<set->max_iter && xn*xn+yn*yn < set->radius*set->radius){
				yn = 2*xn*yn + y0;
				xn = xn2 - yn2 + x0;
				xn2 = xn*xn;
				yn2 = yn*yn;
				iter++;
			}

/* Vous rajouterez un affichage du numéro de la ligne en cours de calcul, comme cela avait été fait pour le calcul de π */
			
			set->img[px][py] = iter;
		}
	}
}

void render_init(struct render *set){
	int i;

	strcpy(set->basename, "mandel");
	set->width = 1000;
	set->height = 1000;
	set->xmin = -2.0;
	set->xmax = 0.28;
	set->ymin = -1.24;
	set->ymax = 1.24;
	set->max_iter = 100;
	set->radius = 2;
}

double map(int v, int imin, int imax, double omin, double omax){
	double range = imax - imin;
	double percentage = (v - imin) / range;

	return omin + percentage * (omax - omin);
}