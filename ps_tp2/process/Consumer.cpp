/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmations système - TP n°2
 *
 * Consumer.cpp
 * c++ -std=c++20 Consumer.cpp -o Consumer -lpthread -lrt
 */

#include <iostream>
#include <thread>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_semaphore.hpp>

#include "../Random.hpp"
#include "../ProdOrCons.hpp"

#include "MessageBox.hpp"

/*
 * Consommateur de messages
 */
class Consumer : public ProdOrCons< MessageBox > {
public:
    // Le constructeur de ProdOrCons sera utilisé comme constructeur de Consumer
    using ProdOrCons::ProdOrCons;
 
    void operator()() override {
        // TODO :retirer de box_ nb_messages_ entiers avec attente aléatoire avant
        // chaque retrait. Afficher des messages pour suivre l'avancement.
        // Afficher un message d'erreur si un nombre négatif est extrait.

        using microseconds = std::chrono::duration< int, std::micro >;

        for (int i = 0; i < nb_messages_; i++) {
            std::this_thread::sleep_for(microseconds{ random_engine_() });
            int message = box_.get();
            if (message < 0) {
                std::cout << "ERROR : Consumer " << name_ << " a reçu un message négatif : " << message << "\n";
            } else {
                std::cout << "Consumer " << name_ << " a reçu la message " << message << " (" << i+1 << "/" << nb_messages_ << ")\n";
            }
        }
    }
};


int main()
{
    using namespace boost::interprocess;
    Random generator { 50 };

    // TODO : attendre que le producteur ait signalé que la boîte
    // est prête, accéder à la mémoire partagée, la projeter en mémoire,
    // y accéder comme boîte à lettres, lancer le consommateur

    // Noms uniques pour des objets partagés
    const char* shared_memory_name = "shared_memory";
    const char* semaphore_name = "semaphore";
 
    // Création du sémaphore nommé et attente de son ouverture
    named_semaphore semaphore{open_or_create, semaphore_name, 0};
    semaphore.wait();

    // Ouverture de la mémoire partagée et de la boîte à lettres
    shared_memory_object shm{open_only, shared_memory_name, read_write};
    mapped_region region{shm, read_write};
    MessageBox* box = static_cast<MessageBox*>(region.get_address());

    // Lancement du consommateur
    Consumer consumer{ 1, *box, generator, 10 };
    std::thread consumer_thread{ std::ref(consumer) };
    consumer_thread.join();

    // Nettoyage des objets partagés
    shared_memory_object::remove(shared_memory_name);
    named_semaphore::remove(semaphore_name);
    
    return 0;
}