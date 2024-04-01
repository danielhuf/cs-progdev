/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmation système - TP n°1
 *
 * server_4.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

volatile sig_atomic_t running = 1;

// Handler du processus père
void parent_handler( int sig ) {
    printf("* Père - Signal %d reçu, arrêt du programme (ID : %d) *\n", sig, getpid());
    running = 0; 
}

void exit_message() {
    printf("* Server 4 est terminé *\n");
}

int main()
{
    // Création d'un tube
    int tube[2];
    if (pipe(tube) == -1) {
        perror("Erreur lors de la création du tube");
        exit(EXIT_FAILURE);
    }

    // Création d'un processus fils
    pid_t child_pid = fork();

    // Vérification de la création du processus fils
    if (child_pid == -1) {
        perror("Erreur lors de la création du processus fils");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {

        printf("* Fils - Server 4 est lancé (ID : %d) *\n", getpid());

        // Fermeture de l'extrémité d'écriture du tube
        close(tube[1]);

        int nb;
        while (read(tube[0], &nb, sizeof(int)) > 0) {
            printf("Fils - PID : %d, PPID : %d, PGID : %d, Nombre reçu : %d\n", getpid(), getppid(), getpgrp(), nb);
        }

        // Fermeture de l'extrémité de lecture du tube
        close(tube[0]);

        printf("* Fils - Fin du programme (ID : %d) *\n", getpid());
    } else {
        printf("* Pére - Server 4 est lancé (ID : %d) *\n", getpid());

        // Fermeture de l'extrémité de lecture du tube
        close(tube[0]);

        srand(time(NULL));

        // Installation du handler pour SIGINT, SIGTERM et SIGPIPE
        struct sigaction parent_action;            
        parent_action.sa_handler = parent_handler;       
        parent_action.sa_flags = 0;
        if (sigaction(SIGINT, &parent_action, NULL) == -1 || sigaction(SIGTERM, &parent_action, NULL) == -1 || sigaction(SIGPIPE, &parent_action, NULL) == -1) {
            perror("Erreur lors de l'installation du handler");
            exit(EXIT_FAILURE);
        }

        while (running) {
            int nb = rand() % 100;
            printf("Pére - PID : %d, PPID : %d, PGID : %d, Envoi du nombre : %d\n", getpid(), getppid(), getpgrp(), nb);

            // Ecriture du nombre aléatoire dans le tube
            if (write(tube[1], &nb, sizeof(int)) == -1) {
                perror("Erreur lors de l'écriture dans le tube");
                break; // Sortie de la boucle si le tube est fermé par le fils
            }
            
            sleep(1);
        }

        // Fermeture de l'extrémité d'écriture du tube
        close(tube[1]);

        printf("* Père - Fin du programme (ID : %d) *\n", getpid());
    }

    // Attente de la fin du processus fils
    if (child_pid > 0) {
        int status;
        waitpid(child_pid, &status, 0);
    }
    
    printf("* Server 4 est arrêté *\n");
    if (atexit(exit_message) != 0) {
        perror("Erreur lors de l'enregistrement de l'exit handler");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

/* Après avoir compilé et executé le programme, j'ai remarqué que les processus s'arrêtent avec 'CTRL-C' et kill sur le fils et le père.
 * Cependant, j'ai remarqué que en arrêtant le père en premier, le fils affiche son message de fin, pendant que le père ne l'affiche pa son message de fin quand le fils est arrêté en premier.
*/

/* Pour modifier le comportement du père en sorte qu'il affiche aussi son message de fin quand le fils est arrêté en premier, j'ai utilisé un gestionnaire du signal SIGPIPE dans le père.
 * Comme ça, le père peut détecter la fermeture du tube par le fils et à partir du déclenchement du signal SIGPIPE, afficher son message de fin et terminer son exécution proprement.
*/