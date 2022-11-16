#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
	compile with: gcc -W -Wall -ansi -pedantic -o mandel mandel.c
	https://programmationenlangagec.pages.emse.fr/des-calculs-et-des-images/Mandelbrot.html
	https://programmationenlangagec.pages.emse.fr/des-calculs-et-des-images/Grille_evaluation.html
*/

#define STRMAX 256

struct camera
{
	double coord_x;
	double coord_y;
	double width;
	double height;
};

struct pixdiv{
	int iter;
	double x;
	double y;
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
	struct pixdiv * img;
	char basename[STRMAX];
	struct camera pov;
};


double map(int v, int imin, int imax, double omin, double omax);
void render_init(struct render *set, int argc, char *argv[]);
void render_image(struct render *set);
void image_save_bw(struct render *set);
void image_save_altern(struct render *set);
void image_save_grey(struct render *set);
void image_save_grey_smoothed(struct render *set);
void cam2rect(struct render *set, struct camera *pov);
void wrong_args();
int m2v(struct render *set, int x, int y);

int main(int argc, char *argv[])
{
	struct render set;

	render_init(&set, argc, argv);
	cam2rect(&set, &(set.pov));
	render_image(&set);
	image_save_grey_smoothed(&set);

	free(set.img);

	return 0 ;
}

void cam2rect(struct render *set, struct camera *pov){
	set->xmin = pov->coord_x - pov->width/2.0;
	set->xmax = pov->coord_x + pov->width/2.0;
	set->ymin = pov->coord_y - pov->height/2.0;
	set->ymax = pov->coord_y + pov->height/2.0;
}

void image_save_grey_smoothed(struct render *set){
	FILE * fout;
	int j;
	int i;
	int lastLigne = 0;
	double grey;
	double x;
	double y;
	double n_it;

	fout = fopen(strcat(set->basename, ".pgm"), "w");
	if ( fout == NULL ) {
		exit(1);
	}

	fputs("P2\n", fout);
	fprintf(fout, "%d %d\n", set->width, set->height);
	fprintf(fout, "%d\n", set->max_iter);
	fprintf(fout, "#max_iter = %d\n", set->max_iter);
	fprintf(fout, "#zone = %f\n", set->xmin);

	for(i=0; i<set->height; i++){
		for(j=0; j<set->width; j++){			
			if(set->img[m2v(set, j, i)].iter == set->max_iter){
				fputc('0', fout);
			}else{
				x = set->img[m2v(set, j, i)].x;
				y = set->img[m2v(set, j, i)].y;
				n_it = set->img[m2v(set, j, i)].iter;
				grey = 5 + n_it - log(log(x*x + y*y)/log(2))/log(2);
				grey = floor(512 * grey / set->max_iter);
				if(grey > 255){
					grey = 255;
				}

				fprintf(fout, "%d", (int)grey);
			}
			
			if(ftell(fout) - lastLigne > 60){
				lastLigne = ftell(fout);
				fputc('\n', fout);
			}else{
				fputc(' ', fout);
			}
		}
	}

	fclose(fout);
}

/*S’il y a plus de 256 itérations, il faudra procéder à un ré-échantillonnage des niveaux de gris puisque nous ne pouvons en afficher que 256 ([0;255]).*/
void image_save_grey(struct render *set){
	FILE * fout;
	int j;
	int i;
	int lastLigne = 0;

	fout = fopen(strcat(set->basename, ".pgm"), "w");
	if ( fout == NULL ) {
		exit(1);
	}

	fputs("P2\n", fout);
	fprintf(fout, "%d %d\n", set->width, set->height);
	fprintf(fout, "%d\n", set->max_iter);
	fprintf(fout, "#max_iter = %d\n", set->max_iter);
	fprintf(fout, "#zone = %f\n", set->xmin);

	for(i=0; i<set->height; i++){
		for(j=0; j<set->width; j++){			
			if(set->img[m2v(set, j, i)].iter == set->max_iter){
				fputc('0', fout);
			}else{
				fprintf(fout, "%d", set->img[m2v(set, j, i)].iter);
			}
			
			if(ftell(fout) - lastLigne > 60){
				lastLigne = ftell(fout);
				fputc('\n', fout);
			}else{
				fputc(' ', fout);
			}
		}
	}

	fclose(fout);
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
			if(set->img[m2v(set, j, i)].iter == set->max_iter){
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
			if(set->img[m2v(set, j, i)].iter == set->max_iter || set->img[m2v(set, j, i)].iter%2 == 1){
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
	int i;
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

			for(i=0; i<4; i++){
				yn = 2*xn*yn + y0;
				xn = xn2 - yn2 + x0;
				xn2 = xn*xn;
				yn2 = yn*yn;
			}

/* Vous rajouterez un affichage du numéro de la ligne en cours de calcul, comme cela avait été fait pour le calcul de π */

			set->img[m2v(set, px, py)].iter = iter;
			set->img[m2v(set, px, py)].x = xn;
			set->img[m2v(set, px, py)].y = yn;
		}
	}
}

void render_init(struct render *set, int argc, char *argv[]){
	struct camera pov = {
		-0.76,
		0,
		2.48,
		2.48
	};

	strcpy(set->basename, "mandel");
	set->width = 1000;
	set->height = 1000;
	set->xmin = -2.0;
	set->xmax = 0.28;
	set->ymin = -1.24;
	set->ymax = 1.24;
	set->max_iter = 100;
	set->radius = 2;
	set->pov = pov;

	if(argc>1){
		set->max_iter = atoi(argv[1]);
		if(set->max_iter == 0){
			wrong_args();
		}
	}

	if(argc>2){
		char *p;
		char delim[] = ",";

		char* ptr = strtok(argv[2], delim);
		if(ptr ==NULL){wrong_args();}
		set->pov.coord_x = strtod(ptr, &p);

		ptr = strtok(NULL, delim);
		if(ptr ==NULL){wrong_args();}
		set->pov.coord_y = strtod(ptr, &p);

		ptr = strtok(NULL, delim);
		if(ptr ==NULL){wrong_args();}
		set->pov.width = strtod(ptr, &p);

		ptr = strtok(NULL, delim);
		if(ptr ==NULL){wrong_args();}
		set->pov.height = strtod(ptr, &p);
	}

	if(argc>3){
		char *p;
		char delim[] = "x";
		char* ptr = strtok(argv[3], delim);
		if(ptr ==NULL){wrong_args();}
		set->width = strtod(ptr, &p);

		ptr = strtok(NULL, delim);
		if(ptr ==NULL){wrong_args();}
		set->height = strtod(ptr, &p);
	}

	if(argc>4){
		strcpy(set->basename, argv[4]);
	}

	set->img = calloc(set->height*set->width, sizeof(struct pixdiv));

	if ( set->img == NULL ) {
        exit(1);
    }
}

int m2v(struct render *set, int x, int y){
	return x + y*set->width;
}

void wrong_args(){
	fprintf(stderr, "The specified arguments are incorrect\n Usage:\n max_iter cam_x,cam_y,cam_w,cam_h img_widthximg_height basename\n");
	exit(1);
}

double map(int v, int imin, int imax, double omin, double omax){
	double range = imax - imin;
	double percentage = (v - imin) / range;

	return omin + percentage * (omax - omin);
}