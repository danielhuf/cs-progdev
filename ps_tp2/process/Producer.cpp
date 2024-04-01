/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmations système - TP n°2
 *
 * Producer.cpp
 * c++ -std=c++20 Producer.cpp -o Producer -lpthread -lrt
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
 * Producteur de messages
 */
class Producer : public ProdOrCons< MessageBox > {
public:
    // Le constructeur de ProdOrCons sera utilisé comme constructeur de Producer
    using ProdOrCons::ProdOrCons;
 
    void operator()() override {
        // TODO : déposer dans box nb_messages nombres entiers positifs avec attente
        // aléatoire entre chaque. Afficher des messages pour suivre l'avancement.

        using microseconds = std::chrono::duration< int, std::micro >;

        for (int i = 0; i < nb_messages_; i++) {
            std::this_thread::sleep_for(microseconds{ random_engine_() });
            box_.put(i+1);
            std::cout << "Producer " << name_ << " a envoyé la message " << i+1 << " (" << i+1 << "/" << nb_messages_ << ")\n";
        }
    }
};


int main()
{
    using namespace boost::interprocess;
    Random generator { 50 };

    // TODO : créer la mémoire partagée, la projeter en mémoire,
    // y construire la boîte à lettres, signaler au consommateur
    // que la boîte est prête, lancer le producteur

    // Noms uniques pour des objets partagés
    const char* shared_memory_name = "shared_memory";
    const char* semaphore_name = "semaphore";

    // Création de la mémoire partagée et de la boîte à lettres
    shared_memory_object shm{open_or_create, shared_memory_name, read_write};
    shm.truncate(sizeof(MessageBox));
    mapped_region region{shm, read_write};
    MessageBox* box = new (region.get_address()) MessageBox{};

    // Création du sémaphore nommé et ouverture pour indiquer au consommateur que la boîte est prête
    named_semaphore semaphore{open_or_create, semaphore_name, 0};

    // Lancement du producteur
    Producer producer{ 1, *box, generator, 10 };
    std::thread producer_thread{ std::ref(producer) };
    
    // Attente de la fin du producteur
    semaphore.post();
    producer_thread.join();
    
    return 0;
}