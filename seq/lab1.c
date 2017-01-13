#include <stdio.h>
#include <unistd.h>
#include "sys/time.h"

int main(int argc, const char* argv[]) {
	printf("%d", argc);
	fflush(stdout);
	if(argc == 3){
		//Code tire de l'exemple minuteur.c fourni sur le site du cours
		double timeStart, timeEnd, Texec;
		struct timeval tp;
		gettimeofday (&tp, NULL); // Debut du chronometre
		timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
		
		int prob = atoi(argv[0]);
		int valeur = atoi(argv[1]);
		int alteration = atoi(argv[2]);
		
		int i,j,k;
		
		printf("Debut matrice 0");
		fflush(stdout);
		int matrix[8][8];
		for( i = 0; i < 8; i++){
			for( j = 0; j < 8; j++){
				usleep(1000);
				matrix[i][j] = valeur;
			}
		}
		
		if(prob == 1){
			printf("Debut du prob 1");
			fflush(stdout);
			for( k = 1; k <= alteration; k++){
				for( i = 0; i < 8; i++){
					for( j = 0; j < 8; j++){
						usleep(1000);
						matrix[i][j] = matrix[i][j] + (i+j) * k;
					}
				}
			}
		}
		else if(prob==2){
			printf("Debut du prob 2");
			fflush(stdout);
			for( k = 1; k <= alteration; k++){
				for( i = 0; i < 8; i++){
					for( j = 0; j < 8; j++){
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
		
		for( i = 0; i < 8; i++){
			for( j = 0; j < 8; j++){
				printf("%d ",matrix[i][j]);
			}
		}
		printf("\n");
		printf("%f\n",Texec);
		return 0;
	}
	else{
		return -1;
	}
}