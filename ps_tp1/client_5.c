/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmation système - TP n°1
 *
 * client_5.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

volatile sig_atomic_t running = 1;

// Handler du client
void client_handler( int sig ) {
    printf("* Client - Signal %d reçu, arrêt du programme *\n", sig);
    running = 0;  // Mettre la variable globale running à faux
}

void exit_message() {
    printf("* Client 5 est terminé *\n");
}

int main() 
{
    printf("* Client 5 est lancé *\n");

    // Installation du handler pour SIGINT, SIGTERM et SIGPIPE
    struct sigaction client_action;
    client_action.sa_handler = client_handler;
    client_action.sa_flags = 0;
    if (sigaction(SIGINT, &client_action, NULL) == -1 || sigaction(SIGTERM, &client_action, NULL) == -1 || sigaction(SIGPIPE, &client_action, NULL) == -1) {
        perror("Erreur lors de l'installation du handler");
        exit(EXIT_FAILURE);
    }

    // Ouverture de la FIFO en lecture
    int fifo = open("fifo", O_RDONLY);
    if (fifo == -1) {
        perror("Erreur lors de l'ouverture de la FIFO en lecture");
        exit(EXIT_FAILURE);
    }

    // Lecture d'un nombre aléatoire depuis la FIFO
    while (running) {

        int nb;
        ssize_t result = read(fifo, &nb, sizeof(int));
        if (result > 0) {
            // Si la lecture s'est bien passée, on affiche les informations du processus et le nombre aléatoire reçu
            printf("Client - PID : %d, PPID : %d, PGID : %d, Nombre reçu : %d\n", getpid(), getppid(), getpgrp(), nb);
        } else if (result == 0) {
            printf("Client - Fin de la lecture\n");
            close(fifo);
            running = 0;
        } else {
            perror("Erreur lors de la lecture");
            exit(EXIT_FAILURE);
        }

        sleep(1);
    }

    // Fermeture de la FIFO
    close(fifo);

    printf("* Client 5 est arrêté *\n");
    if (atexit(exit_message) != 0) {
        perror("Erreur lors de l'enregistrement de l'exit handler");
        exit(EXIT_FAILURE);
    }
    
    return EXIT_SUCCESS;
}