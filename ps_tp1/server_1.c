/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmation système - TP n°1
 *
 * server_1.c
 */

// for printf()
#include <stdio.h>
// For rand(), srand(), sleep(), EXIT_SUCCESS
#include <stdlib.h>
// For time()
#include <time.h>
// For getpid(), getppid()
#include <unistd.h>

int main()
{

    srand(time(NULL)); // initialisation de rand() pour la génération de nombres aléatoires
    printf("* Server 1 est lancé *\n");

    // Boucle infinie
    while (1) {

        // Affichage des informations du processus
        printf("PID : %d, PPID : %d, PGID : %d\n", getpid(), getppid(), getpgrp());

        // Génération d'un nombre aléatoire entre 0 et 99
        printf("Nombre Aléatoire : %d\n", rand() % 100);

        // Attente de 1 seconde
        sleep(1);
    }

    printf("* Server 1 est terminé *\n");
        
    return EXIT_SUCCESS;
}

/* Après avoir compilé et executé le programme, j'ai tapé la commande 'ps -j' dans un autre terminal (Term2) pour voir les informations.
 * Dans la sortie de 'ps aj', je peux voir une liste de processus, y compris celui que je viens de démarrer dans Term1. 
 * Je peux voir son PID unique qui correspond à ce qui est affiché par le programme, et aussi son PPID, qui est le PID du processus qui l'a lancé (le Term1).
 * Je peux aussi voir le PGID, qui est le même que son PID, et finallement le nom de la commande qui l'a lancé (./server_1)
*/

/* Comme prévu, après avoir arrêté le processus manuellement ('CTRL-C'), le processus n'est plus répertorié dans la sortie de 'ps aj' dans Term2.
 * Après avoir relancé le processus dans Term1 et l'avoir tué avec la commande 'kill <PID>' dans Term2, où <PID> est le PID du nouveau processus, le processus s'est effectivement arrêté.
 */