/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmation système - TP n°1
 *
 * client_6.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 12345 // Port du serveur

volatile sig_atomic_t running = 1;

// Handler du client
void server_handler( int sig ) {
    printf("* Client - Signal %d reçu, arrêt du programme *\n", sig);
    running = 0;  // Mettre la variable globale running à faux
}

void exit_message() {
    printf("* Client 6 est terminé *\n");
}

int main() 
{
    int client_socket;
    struct sockaddr_in server_address;

    printf("* Client 6 est lancé *\n");

    // Installation du handler pour SIGINT, SIGTERM et SIGPIPE
    struct sigaction client_action;
    client_action.sa_handler = server_handler;
    client_action.sa_flags = 0;
    if (sigaction(SIGINT, &client_action, NULL) == -1 || sigaction(SIGTERM, &client_action, NULL) == -1 || sigaction(SIGPIPE, &client_action, NULL) == -1) {
        perror("Erreur lors de l'installation du handler");
        exit(EXIT_FAILURE);
    }

    // Création de la socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Erreur lors de la création de la socket");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse du serveur
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");    // Adresse IPv4 de loopback
    server_address.sin_port = htons(PORT);                      // Conversion du port en format réseau

    // Connexion au serveur
    if (connect(client_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        perror("Erreur lors de la connexion au serveur");
        exit(EXIT_FAILURE);
    }

    // Lecture d'un nombre aléatoire depuis la socket
    while(running) {
        int nb;
        ssize_t result = read(client_socket, &nb, sizeof(int));
        if (result > 0) {
            // Si la réception s'est bien passée, on affiche les informations du processus et le nombre aléatoire reçu
            printf("Client - PID : %d, PPID : %d, PGID : %d, Nombre reçu : %d\n", getpid(), getppid(), getpgrp(), nb);
        } else if (result == 0) {
            printf("Client - Connexion fermée par le serveur\n");
            close(client_socket);
            break;
        } else {
            perror("Erreur lors de la réception du nombre aléatoire");
            exit(EXIT_FAILURE);
        }

        sleep(1);
    }

    // Fermeture de la socket
    close(client_socket);

    printf("* Client 6 est arrêté *\n");
    if (atexit(exit_message) != 0) {
        perror("Erreur lors de l'enregistrement de l'exit handler");
        exit(EXIT_FAILURE);
    }
    
    return EXIT_SUCCESS;
}