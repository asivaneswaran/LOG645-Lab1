#include <stdio.h>
#include <unistd.h>
#include "sys/time.h"

void main(int argc, const char* argv[]) {
	//Code tire de l'exemple minuteur.c fourni sur le site du cours
	double timeStart, timeEnd, Texec;
	struc timeval tp;
	gettimeofday (&tp,NULL); //debut du chronometre
	timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
	
	int prob = argv[0];
	int valeur = argv[1];
	int alteration = argv[2];
	
	int[8][8] matrix;
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			usleep(1000);
			matrix[i][j] = valeur;
		}
	}
	
	if(prob == 1){
		for(int k = 1; k <= alteration; k++){
			for(int i = 0; i < 8; i++){
				for(int j = 0; j < 8; j++){
					usleep(1000);
					matrix[i][j] = matrix[i][j] + (i+j) * k;
				}
			}
		}
	}
	else if(prob==2){
		for(int k = 1; k <= alteration; k++){
			for(int i = 0; i < 8; i++){
				for(int j = 0; j < 8; j++){
					if(j==0){
						usleep(1000);
						matrix[i][j] = matrix[i][j] + (i*k);
					}else{
						usleep(1000);
						matrix[i][j] = matrix[i][j] + matrix[i][j-1] * k;
					}
				}
			}
		}
	}
	
	//Code tire de l'exemple minuteur.c fourni sur le site du cours
	gettimeofday (&tp, NULL); // Fin du chronometre
	timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
	Texec = timeEnd - timeStart; //Temps d'execution en secondes
	
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			fprintf('%d',matrix[i][j]);
		}
	}
	fprintf('%f',Texec);
}