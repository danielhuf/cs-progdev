/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmation système - TP n°1
 *
 * server_6.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 12345 // Port du serveur

volatile sig_atomic_t running = 1;

// Handler du serveur
void server_handler( int sig ) {
    printf("* Server - Signal %d reçu, arrêt du programme *\n", sig);
    running = 0;  // Mettre la variable globale running à faux
}

void exit_message() {
    printf("* Server 6 est terminé *\n");
}

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);

    printf("* Server 6 est lancé *\n");

    // Installation du handler pour SIGINT, SIGTERM et SIGPIPE
    struct sigaction server_action;
    server_action.sa_handler = server_handler;
    server_action.sa_flags = 0;
    if (sigaction(SIGINT, &server_action, NULL) == -1 || sigaction(SIGTERM, &server_action, NULL) == -1 || sigaction(SIGPIPE, &server_action, NULL) == -1) {
        perror("Erreur lors de l'installation du handler");
        exit(EXIT_FAILURE);
    }

    // Création de la socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Erreur lors de la création de la socket");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse du serveur
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);  // Accepter les connexions sur toutes les interfaces
    server_address.sin_port = htons(PORT);               // Conversion du port en format réseau

    // Attachement de la socket à l'adresse du serveur
    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        perror("Erreur lors de l'attachement de la socket à l'adresse du serveur");
        exit(EXIT_FAILURE);
    }

    // Mise en écoute de la socket
    if (listen(server_socket, 5) == -1) {
        perror("Erreur lors de la mise en écoute de la socket");
        exit(EXIT_FAILURE);
    }

    // Acceptation d'une connexion
    client_socket = accept(server_socket, (struct sockaddr *) &client_address, &client_address_len);
    if (client_socket == -1) {
        perror("Erreur lors de l'acceptation d'une connexion");
        exit(EXIT_FAILURE);
    }

    // Envoi d'un nombre aléatoire entre 0 et 99 au client
    srand(time(NULL));
    while (running) {
        int nb = rand() % 100;
        printf("Server - PID : %d, PPID : %d, PGID : %d, Envoi du nombre : %d\n", getpid(), getppid(), getpgrp(), nb);
        if (write(client_socket, &nb, sizeof(int)) == -1) {
            perror("Erreur lors de l'envoi du nombre au client");
            break;
        }

        sleep(1);
    }

    // Fermeture des sockets
    close(server_socket);
    close(client_socket);

    printf("* Server 6 est arrêté *\n");
    if (atexit(exit_message) != 0) {
        perror("Erreur lors de l'enregistrement de l'exit handler");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}