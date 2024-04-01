/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmations système - TP n°2
 *
 * Threads.cpp
 * c++ -std=c++20 Threads.cpp -o Threads -lpthread
 */

#include <iostream>
#include <thread>
 
 
#include "../Random.hpp"

#include "osyncstream.hpp"
#include "Producer.hpp"
#include "Consumer.hpp"
#include "MessageBox.hpp"
 
/*
 * Test avec 1 producteur et 1 consommateur
 */
void one_producer_one_consumer() {
    // TODO :
    // - Créer un générateur de nombres aléatoires
    Random generator { 50 };

    // - Créer une boîte à lettres
    MessageBox box;

    // - Créer un producteur et un consommateur
    Producer producer ( 1, box, generator, 20 );
    Consumer consumer (1, box, generator, 20);

    // - Créer les threads correspondants
    std::thread producer_thread ( producer );
    std::thread consumer_thread ( consumer );

    // - Attendre la fin des threads
    producer_thread.join();
    consumer_thread.join();
}

/*
 * Test avec plusieurs producteurs et consommateurs
 */
void several_producers_and_consumers() {
    // TODO :
    // - Créer un générateur de nombres aléatoires
    Random generator { 50 };

    // - Créer une boîte à lettres
    MessageBox box;

    // - Créer entre 10 et 20 consommateurs et 2 fois plus de producteurs
    Random generator2 { 10 };
    int nb_consumers = generator2() + 10;

    osyncstream{ std::cout } << "* Consumers : " << nb_consumers << " / Producers : " << nb_consumers*2 << " *\n\n";

    std::vector< std::thread > group;
    for ( unsigned int i = 0; i < nb_consumers; i++ ) {
        // - Créer les threads correspondants
        group.push_back(std::thread{ Consumer{ i+1, box, generator, 20 } });
        group.push_back(std::thread{ Producer{ 2*i + 1, box, generator, 10} });
        group.push_back(std::thread{ Producer{ 2*i + 2, box, generator, 10 } });
    }

    // - Attendre la fin des threads
    for ( auto& thread : group ) {
        thread.join();
    }
}

int main() {
    //one_producer_one_consumer();
    several_producers_and_consumers();
    return 0;
}

/* En effet, après avoir compilé et exécuté le programme la première fois, en appelant la fonction one_producer_one_consumer(), je constate que le consommateur reçoit parfois des messages négatifs. 
 * Après avoir ajouté les instructions de synchronisation dans MessageBox, je constate que le consommateur ne reçoit plus de messages négatifs.
*/