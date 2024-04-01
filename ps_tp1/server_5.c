/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmation système - TP n°1
 *
 * server_5.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

volatile sig_atomic_t running = 1;

// Handler du serveur
void server_handler( int sig ) {
    printf("* Server - Signal %d reçu, arrêt du programme *\n", sig);
    running = 0;  // Mettre la variable globale running à faux
}

void exit_message() {
    printf("* Server 5 est terminé *\n");
}

int main()
{
    printf("* Server 5 est lancé *\n");

    // Installation du handler pour SIGINT, SIGTERM et SIGPIPE
    struct sigaction server_action;
    server_action.sa_handler = server_handler;
    server_action.sa_flags = 0;
    if (sigaction(SIGINT, &server_action, NULL) == -1 || sigaction(SIGTERM, &server_action, NULL) == -1 || sigaction(SIGPIPE, &server_action, NULL) == -1) {
        perror("Erreur lors de l'installation du handler");
        exit(EXIT_FAILURE);
    }

    // Ouverture de la FIFO en écriture
    int fifo = open("fifo", O_WRONLY);
    if (fifo == -1) {
        perror("Erreur lors de l'ouverture de la FIFO en écriture");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    while (running) {

        // Génération d'un nombre aléatoire entre 0 et 99
        int nb = rand() % 100;

        // Affichage des informations du processus et du nombre aléatoire entre 0 et 99
        printf("Server - PID : %d, PPID : %d, PGID : %d, Envoi du nombre : %d\n", getpid(), getppid(), getpgrp(), nb);

        // Écriture du nombre aléatoire dans la FIFO
        if (write(fifo, &nb, sizeof(int)) == -1) {
            perror("Erreur lors de l'écriture dans la FIFO");
            break;
        }

        sleep(1);
    }

    // Fermeture de la FIFO
    close(fifo);

    printf("* Server 5 est arrêté *\n");
    if (atexit(exit_message) != 0) {
        perror("Erreur lors de l'enregistrement de l'exit handler");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}