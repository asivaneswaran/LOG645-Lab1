#include <stdio.h>
#include <unistd.h>
#include "sys/time.h"
#include "mpi.h"

//Fonction pour imprimer dans la console
void print_result(int matrix[8][8]){
	int i;
	printf("Matrice finale:\n");
	for( i = 0; i < 8; i++){
		printf("%d %d %d %d %d %d %d %d\n",matrix[i][0],matrix[i][1],matrix[i][2],matrix[i][3],matrix[i][4],matrix[i][5],matrix[i][6],matrix[i][7]);
	}
}

//Fonction pour traiter la reponse
void traiterReponse(int resultat_finale[8][8], int resultat[4]){
	resultat_finale[resultat[2]][resultat[3]] = resultat[0];
	resultat_finale[resultat[2]][resultat[3]+1] = resultat[1];
}

// Probleme 1 et 2 pour le process maitre 
void probM(int matrix[8][8], int alteration, int nb_proc){
	MPI_Status statut;
	int resultat_finale[8][8];
	int i,j,k;
	//Boucle pour envoyer au process
	for( k = 1; k<nb_proc;k++){
		//Pour chaque process, on choisit les index a envoyer
		i = (int)(k-1)/4;
		j = ((k-1)%4)*2;
		// Envoie a chaque process des donnees
		int tampon[5] = {matrix[i][j], matrix[i][j+1], i, j, alteration};
		MPI_Send(&tampon,5,MPI_INT,k,0,MPI_COMM_WORLD);
	}
	//Boucle pour revevoir les reponses des process
	for(k=1;k<nb_proc;k++){
		int resultat[4];
		MPI_Recv(&resultat,4,MPI_INT,MPI_ANY_SOURCE,1,MPI_COMM_WORLD,&statut);
		traiterReponse(resultat_finale, resultat);
	}
	
	// Impression du resultat dans la console
	print_result(resultat_finale);
}

//Set le buffer pour lenvoie au process maitre
void setBufferEnvoieP1(int bufferE[4], int tmp[2], int bufferR[5]){
	bufferE[0] = tmp[0];
	bufferE[1] = tmp[1];
	bufferE[2] = bufferR[2];
	bufferE[3] = bufferR[3];
}

//Calcul pour la matrice de retour
void calculP1(int tmp[2], int bufferR[5],int idx){
	usleep(1000);
	tmp[0] = bufferR[0] + (bufferR[2]+bufferR[3])*idx;
	tmp[1] = bufferR[1] + (bufferR[2]+bufferR[3]+1)*idx;
}

// Probleme 1 pour les process esclaves
void prob1S(){
	MPI_Status statut;
	int buffer_envoie[4];
	int buffer_recoit[5];
	
	//Recoit des donnees du maitre
	MPI_Recv(&buffer_recoit,5,MPI_INT,0,0,MPI_COMM_WORLD,&statut);
	
	int idx;
	int tmp[2];
	for(idx=0; idx<5; idx++){
		calculP1(tmp,buffer_recoit,idx);
	}
	
	//Set le buffer de renvoie
	setBufferEnvoieP1(buffer_envoie,tmp,buffer_recoit);
	
	//Envoie les donnees au maitre
	MPI_Send(&buffer_envoie,4,MPI_INT,0,1,MPI_COMM_WORLD);
}

//Set le buffer pour lenvoie au process maitre
void setBufferEnvoieP2(int bufferE[4], int tmp[8], int bufferR[5]){
	bufferE[0] = tmp[bufferR[3]];
	bufferE[1] = tmp[bufferR[3]+1];
	bufferE[2] = bufferR[2];
	bufferE[3] = bufferR[3];
}

//Calcul pour la matrice de retour
void calculP2(int tmp[8], int bufferR[5],int idx){
	int j;
	for(j=0;j<=bufferR[3]+1;j++){
		usleep(1000);
		if(j==0){
			tmp[j] = bufferR[j] + (bufferR[2]*idx);
		}else{
			tmp[j] = bufferR[j] + bufferR[j-1]*idx;
		}
	}
}

// Probleme 2 pour les process esclaves
void prob2S(){
	MPI_Status statut;
	int buffer_envoie[4];
	int buffer_recoit[5];
	
	//Recoit les donnees du process maitre
	MPI_Recv(&buffer_recoit,5,MPI_INT,0,0,MPI_COMM_WORLD,&statut);
	
	int idx;
	int tmp[8];
	
	for(idx=0;idx<=buffer_recoit[4];idx++){
		calculP2(tmp,buffer_recoit,idx);
	}
	
	//Set le buffer de renvoie
	setBufferEnvoieP2(buffer_envoie,tmp,buffer_recoit);
	//Envoie des donnees au maitre
	MPI_Send(&buffer_envoie,4,MPI_INT,0,1,MPI_COMM_WORLD);
}

int main(int argc, const char* argv[]) {
	if (argc != 4){
		printf("Il manque des arguments! \n");
		return 0;
	}
	
	int err;
	int master = 0;
	int np;
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
	err = MPI_Comm_size(MPI_COMM_WORLD, &np);
	
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
		if((prob==1) || (prob==2)){
			probM(matrix, alteration, np);
		}else{
			printf("Choix de probleme invalide.\n");
		}
		
		//Code tire de l'exemple minuteur.c fourni sur le site du cours
		gettimeofday (&tp, NULL); // Fin du chronometre
		timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
		Texec = timeEnd - timeStart; //Temps d'execution en secondes	
		
		printf("\n\n");
		printf("Temps d'execution : %f\n",Texec);
		printf("================================================\n");
	} else{
		switch(prob){
			case 1: prob1S();
					break;
			case 2: prob2S();
					break;
		}
	}
	
	// Fermeture du Comm_World
	err = MPI_Finalize();
	
	return 0;

}