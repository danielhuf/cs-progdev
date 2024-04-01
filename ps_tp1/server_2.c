/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmation système - TP n°1
 *
 * server_2.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
// For sigaction()
#include <signal.h>

// Variable globale pour contrôler l'exécution du programme
volatile sig_atomic_t running = 1;

// Handler du serveur
void stop_handler( int sig ) {
    printf("* Signal %d reçu, arrêt du programme *\n", sig);
    running = 0;  // Mettre la variable globale running à faux
}

void exit_message() {
    printf("* Server 2 est terminé *\n");
}

int main()
{
    printf("* Server 2 est lancé *\n");

    // // Installation du handler pour SIGINT et SIGTERM
    struct sigaction action;
    action.sa_handler = stop_handler;   // Initialisation de la structure sigaction
    action.sa_flags = 0;                // Initialisation des flags
    if (sigaction(SIGINT, &action, NULL) == -1 || sigaction(SIGTERM, &action, NULL) == -1) {
        perror("Erreur lors de l'installation du handler");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    // Boucle infinie
    while (running) {

        // Affichage des informations du processus e du nombre aléatoire entre 0 et 99
        printf("PID : %d, PPID : %d, PGID : %d, Nombre Aléatoire : %d\n", getpid(), getppid(), getpgrp(), rand() % 100);

        // Attente de 1 seconde
        sleep(1);
    }

    printf("* Server 2 est arrêté *\n");

    // Enregistrement de la fonction exit_message() pour être exécutée à la fin du programme
    if (atexit(exit_message) != 0) {
        perror("Erreur lors de l'enregistrement de l'exit handler");
        exit(EXIT_FAILURE);
    }
        
    return EXIT_SUCCESS;
}

/* Après avoir compilé et executé le programme, j'ai tapé CTRL-C dans le terminal (Term1) pour envoyer le signal SIGINT au processus.
 * Vraiment, la message a été affiché, et le processus s'est arrêté.
*/

/* Avant de modifier la fonction stop_handler(), je regarde que la message est bien affiché quand je tape
 * kill -s INT <PID> dans un autre terminal (Term2), où <PID> est le PID du processus en cours d'exécution dans Term1.
 * Cependant, cette message n'est pas affiché quand je tape kill <PID> dans Term2, car le signal envoyé par défaut est SIGTERM, pas SIGINT.
 * Donc je dois modifier le traitement de stop_handler() dans la condition pour qu'elle puisse gérer aussi le signal SIGTERM.
*/

/* Aprés avoir executé un autre fois le programme, je tape kill -9 <PID> dans Term2, où <PID> est le PID du processus en cours d'exécution dans Term1
 * et je vois que le processus s'est arrêté immédiatement, sans que le message soit affiché.
 * Ça c'est naturellement le comportement du signal 'kill -9', qui termine immédiatement le processus cible en envoyant le signal SIGKILL.
 * Si je tape kill <PPID> dans Term2, où <PPID> est le processus père du processus en cours d'exécution dans Term1, je vois que le processus ne s'arrête pas.
 * Cependant, si je tape kill -9 <PPID> dans Term2, le processus s'arrête immédiatement, sans que le message soit affiché.
 * Cela arrive parce que j'ai envoyé le signal SIGKILL au processus père, qui est le processus qui a lancé le programme dans Term1,
 * d'une manière que Term1 soit terminé immédiatement (et la fenêtre de Term1 soit fermée).
*/

/* Après avoir supprimé la modification de la variable globale running, je vois que le programme ne s'arrête pas quand je tape CTRL-C ou kill <PID> dans Term1.
 * Cela arrive parce que le signal handler ne change pas le valeur de la variable globale running, donc le programme continue à s'exécuter sans que SIGINT ou SIGTERM soit traité.
 * Cependant, quand je tape kill -9 <PID> dans Term1, le programme s'arrête immédiatement, sans que le message soit affiché.
 * Cela arrive parce que j'ai envoyé le signal SIGKILL au processus en cours d'exécution dans Term1, d'une manière que le processus soit terminé immédiatement quel que soit le signal handler.
*/

/* Après avoir ajouté la fonction exit_message(), je vois que le message est affiché quand je tape CTRL-C ou kill <PID> dans Term1.
 * Cependant, quand je tape kill -9 <PID> dans Term1, le message n'est pas affiché, car le processus est terminé immédiatement sans que le signal handler soit exécuté.
*/