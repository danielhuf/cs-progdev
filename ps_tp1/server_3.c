/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmation système - TP n°1
 *
 * server_3.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
// for waitpid()
#include <sys/wait.h>

volatile sig_atomic_t running = 1;

// Variable globale pour stocker le PID du processus fils
pid_t child_pid = 0;

// Handler pour SIGINT et SIGTERM du processus père
void stop_handler( int sig ) {
    printf("* Père - Signal %d reçu, arrêt du programme (ID : %d) *\n", sig, getpid());

    // Si le processus père a un fils, envoyer un signal SIGTERM au fils
    if (child_pid > 0) {
        printf("* Père - Envoi du signal %d au fils (ID : %d) *\n", sig, child_pid);
        kill(child_pid, SIGTERM);
    }

    running = 0;
}

// Handler pour SIGINT et SIGTERM du processus fils
void child_handler( int sig ) {
    printf("* Fils - Signal %d reçu, arrêt du programme (ID : %d) *\n", sig, getpid());
    exit(EXIT_SUCCESS);
}

void exit_message() {
    printf("* Server 3 est terminé *\n");
}

void print_info(const char *process) {
    srand(time(NULL));

    // Affichage des informations du processus et du nombre aléatoire entre 0 et 99
    printf("%s - PID : %d, PPID : %d, PGID : %d, Nombre Aléatoire : %d\n", process, getpid(), getppid(), getpgrp(), rand() % 100);
}

int main()
{

    // Installation du handler pour SIGINT, SIGTERM
    struct sigaction parent_action;            
    parent_action.sa_handler = stop_handler;       
    parent_action.sa_flags = 0;        
    if (sigaction(SIGINT, &parent_action, NULL) == -1 || sigaction(SIGTERM, &parent_action, NULL) == -1) {
        perror("Erreur lors de l'installation du handler");
        exit(EXIT_FAILURE);
    }

    // Création d'un processus fils
    child_pid = fork();

    // Vérification de la création du processus fils
    if (child_pid == -1) {
        perror("Erreur lors de la création du processus fils");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) { // Processus fils
        struct sigaction child_action;            
        child_action.sa_handler = child_handler;
        child_action.sa_flags = 0; 

        // Installation du handler pour SIGINT, SIGTERM
        if (sigaction(SIGINT, &child_action, NULL) == -1 || sigaction(SIGTERM, &child_action, NULL) == -1) {
            perror("Erreur lors de l'installation du handler");
            exit(EXIT_FAILURE);
        }

        printf("* Fils - Server 3 est lancé (ID : %d) *\n", getpid());

        while (running) {
            print_info("Fils");
            sleep(1);
        }

        printf("* Fils - Fin du programme (ID : %d) *\n", getpid());
    } else {
        printf("* Pére - Server 3 est lancé (ID : %d) *\n", getpid());

        int status;
        while (running) {
            print_info("Père");
            sleep(1);

            // Vérification de l'état du processus fils
            if (waitpid(child_pid, &status, WNOHANG) > 0) {
                printf("* Père - Le fils s'est terminé *\n");
                break;
            }
        }

        printf("* Père - Fin du programme (ID : %d) *\n", getpid());
    }

    // Attente de la fin du processus fils
    if (child_pid > 0) {
        int status;
        waitpid(child_pid, &status, 0);
    }
    
    printf("* Server 3 est arrêté *\n");
    if (atexit(exit_message) != 0) {
        perror("Erreur lors de l'enregistrement de l'exit handler");
        exit(EXIT_FAILURE);
    }
    
    return EXIT_SUCCESS;
}

/* Comme expliqué dans la page de getpid(), "un fils créé par fork() hérite de l'ID de groupe de processus de son père, de manère à ce que le PGID du fils soit préservé."
 * Lorsque j'ai appuyé sur 'CTRL-C', un signal SIGINT a été envoyé à tous les processus qui appartiennent au même groupe du processus en cours.
 * Cela signifie que le signal SIGINT a été envoyé au processus père et au processus fils, mais la gestion du signal SIGINT est exécutée séparément pour chaque processus.
 * Par conséquent, les messages de "Signal reçu" sont affichés deux fois (chacun pour un processus différent).
*/

/* Après avoir executé le programme, j'ai tapé la commande 'ps aj' pour visualiser les deux processus (père et fils) en cours.
 * Après avoir arreté le processus fils avec la commande 'kill <PID>', où <PID> est le PID du processus fils, le processus fils s'est arrêté, mais le processus père est toujours en cours.
 * En tapant la commande 'ps aj' à nouveau, j'ai remarqué que le processus fils est marqué comme "<defunct>", ce qui signifie que le processus fils est devenu un "zombie" qui reste parce que son père ne l'a pas détruit correctement.
*/

/* Après avoir exécuté le programme et ensuite tué le processus père avec la commande 'kill <PID>', où <PID> est le PID du processus père, j'ai remarqué que le processus fils est resté en cours.
 * En tapant la commande 'ps aj' pour vérifier ce comportement, j'ai remarqué que le processus père n'est pas plus dans la liste des processus en cours, mais le processus fils oui.
 * Cela arrive parce que le processus fils est devenu un processus orphelin, c'est-à-dire un processus qui n'a pas de processus père, et il a été ensuite adopté par le processus init, qui est le processus ancêtre de tous les processus.
*/

/* Après avoir modifié le programme pour que le processus père arrête après que le processus fils soit arrêté, j'ai relancé le programme et ensuite j'ai tué le père.
 * J'ai remarqué que le comportement du processus fils reste le même que dans le cas précédent, c'est-à-dire que le processus fils est devenu un processus orphelin et il a continué à s'exécuter de manière indépendante.
*/

/* Pour arrêter le processus fils après que le processus père soit arrêté, j'ai modifié le gestionnaire de signal du père pour envoyer un signal SIGTERM au fils, si le père reçoit un signal SIGINT ou SIGTERM, et demander l'arrêt du fils.
 * Après la boucle, j'ai ajouté une condition pour assurer que le processus fils est vraiment terminé.
 * Inversement, si le processus fils se termine, le père vérifie cette condition et sort de la boucle.
*/