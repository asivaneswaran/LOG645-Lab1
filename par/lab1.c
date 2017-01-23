#include <stdio.h>
#include <unistd.h>
#include "sys/time.h"
#include "mpi.h"

// Probleme 1 pour le process maitre 
void prob1Master(int matrix[8][8], int alteration, int nb_proc){
	
}

// Probleme 1 pour les process esclaves
void prob1Slave(){
	
}

// Probleme 2 pour le process maitre 
void prob2Master(int matrix[8][8], int alteration, int nb_proc){
	
}

// Probleme 2 pour les process esclaves
void prob2Slave(){
	
}

int main(int argc, const char* argv[]) {
	if (argc != 4){
		printf("Il manque des arguments! \n");
		return 0;
	}
	
	int err;
	int master = 0;
	int nb_proc;
	int mon_id;
	
	//acceder aux parametres 
	int prob = atoi(argv[1]);
	int valeur = atoi(argv[2]);
	int alteration = atoi(argv[3]);
		
	// Creation d'un Comm_World
	err = MPI_Init(&argc, &argv);
	if(err != MPI_SUCCESS){
		printf("Probleme lors de l'initialisation de MPI. \n");
		return -1;
	}
	err = MPI_Comm_rank(MPI_COMM_WORLD, &mon_id);
	err = MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);
	
	if(mon_id == 0){
		//Code tire de l'exemple minuteur.c fourni sur le site du cours
		double timeStart, timeEnd, Texec;
		struct timeval tp;
		gettimeofday (&tp, NULL); // Debut du chronometre
		timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
		
		
		printf("================================================\n");
		printf("PAR: prob %d, init %d, iter %d\n\n", prob, valeur, alteration);
		
		int i,j,k;

		// Initialisation de la matrice de depart
		int matrix[8][8];
		for( i = 0; i < 8; i++){
			for( j = 0; j < 8; j++){
				usleep(1000);
				matrix[i][j] = valeur;
			}
		}	
		
		switch(prob){
			case 1:
					prob1Master(matrix, alteration, nb_proc);
					break;
			case 2:
					prob2Master(matrix, alteration, nb_proc);
					break;
			default:
					printf("Choix de probleme inexistant!\n");
					break;
		}
		
		//Code tire de l'exemple minuteur.c fourni sur le site du cours
		gettimeofday (&tp, NULL); // Fin du chronometre
		timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
		Texec = timeEnd - timeStart; //Temps d'execution en secondes	
	} else{
		switch(prob){
			case 1: prob1Slave();
					break;
			case 2: prob2Slave();
					break;
		}
	}
	
	// Fermeture du Comm_World
	err = MPI_Finalize();
	
	printf("Matrice finale:\n");
	for( i = 0; i < 8; i++){
		printf("%d %d %d %d %d %d %d %d\n",matrix[i][0],matrix[i][1],matrix[i][2],matrix[i][3],matrix[i][4],matrix[i][5],matrix[i][6],matrix[i][7]);
	}
	printf("\n\n");
	printf("Temps d'execution : %f\n",Texec);
	printf("================================================\n");
	return 0;

}