/**
 * @file Nostromo.c
 * @ProjetAlien
 * @author Jonathan Moze - Mahel Debbah
 * @date 9 Janvier 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "stdbool.h"
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <time.h> 

/**
 * @struct Personnage
 * Caractéristiques d'un personnage
 */
struct Personnage {
    char *nom;
    char *race;
};

/**
 * @struct pid_name
 * Association entre un passager et son PID
 */
struct pid_name {
    pid_t pid;
    char *nom;
};

int nombrePassager = 8; //nombre de passagers à mettre à jour progressivement
struct Personnage *personnage_courant; //hack pour acceder au passager dans le handler
bool stillalive = true;  //statut de l'alien, permet de sortir le père de la boucle


/**
 * Handler humain : l'humain écrit son testament et meurt.
 * 
 * @param sig   valeur signal
 * 
 */
void
handlerHuman( int sig ){
    //TODO
    //Utilisez getpid, sprintf, fopen, fputs et fclose
        int pid =  getpid ();
        char mypid[10];
        sprintf(mypid, "%d", pid);
        char buffer[60];
        sprintf(buffer, "Je suis %s et ma fin est proche.", personnage_courant->nom);
        const char* mode = "w";
        FILE *f = fopen(strcat(mypid, ".txt"), mode);
        fputs(buffer, f);
        fclose(f);
        exit(0);
}

/**
 * Handler alien : l'alien affiche son décès sur le terminal et envoie un signal au père
 * 
 * @param sig   valeur signal
 * 
 */
void
handlerAlien( int sig ){
    
    //TODO
    //Affiche sa mort dans le terminal
    //Envoie un signal au père
    printf("- Ah je suis l'alien et je meurs. -\n");
    kill(getppid() , SIGUSR1);

    exit(0);

}


/**
 * Handler pere : On annonce la fin de la partie et on sort de la boucle.
 * 
 * @param sig   valeur signal
 * 
 */
void
handlerPere( int sig ){

    //TODO
    //Annonce la fin de la partie
    //Sort le père de sa boucle
    printf("Trop fort l'alien est mort.\n");
    stillalive = false;
}


/**
 * Simulation avec 8 passagers.
 */
int
genererPassagers( void )
{

    struct Personnage personnages[] = {
            {"Ellen Ripley", "humain"},
            {"Arthur Dallas", "humain"},
            {"Gilbert Kane", "humain"},
            {"Ash", "humain"},
            {"Joan Lambert", "humain"},
            {"Dennis Parker", "humain"},
            {"Samuel Brett", "humain"},
            {"Toby", "alien"},
    }; //A initialiser correctement

    struct pid_name *tab_pid_name = malloc(nombrePassager * sizeof( struct pid_name )); //A initialiser correctement dans la suite


    printf( "* Dans l'espace lointain...\n" ); //On commence le jeu


    for ( int i = 0; i < nombrePassager; i++ ) {
      
        //TODO : Génération des passagers et gestion des handlers des fils


        personnage_courant = &personnages[i];
        pid_t p = fork();
        if( p == 0){
            if(personnages[i].race == "alien"){
                signal(SIGUSR1, handlerAlien);
                pause();
            }
            else{
                signal(SIGUSR1, handlerHuman);
                pause();
            }
        }

      //TODO : Création du tableau "passager / PID" du père
      tab_pid_name[i].pid = p;
      tab_pid_name[i].nom = personnages[i].nom;


    }

    //TODO : Gestion du handler du père
    signal(SIGUSR1, handlerPere);


    while( stillalive ) {
        
        //TODO
        //Affiche le décès d'un passager et lui envoie un signal
        //attend 3 sec


        srand(time(NULL));
        int nb = rand() % 8;

        if(tab_pid_name[nb].pid != -1){
            
            
            
            if( strcmp(tab_pid_name[nb].nom, "Ellen Ripley") != 0){
                kill(tab_pid_name[nb].pid, SIGUSR1);
                printf("RIP - %s (%d)\n", tab_pid_name[nb].nom , tab_pid_name[nb].pid);
                tab_pid_name[nb].pid = -1;
                nombrePassager--;

                sleep(3);

            }
            
            
        }
        
        
    }

    printf("Il y a %d survivant(s).\n", nombrePassager);

    //TODO : On tue les processus fils restant
    for(int i=0; i<8; i++){
        if(tab_pid_name[i].pid != -1){
            kill(tab_pid_name[i].pid, SIGKILL);
        }
        
    }
   
    printf( "* Dans l'espace personne ne vous entendra crier.\n" );

    free(tab_pid_name);
    return ( EXIT_SUCCESS );
}

/**
 * Fonction principale.
 * @return  code de retour
 */
int
main( void )
{

    genererPassagers();

    return EXIT_SUCCESS;
    
}
