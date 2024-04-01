/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmations système - TP n°2
 *
 * Consumer.hpp
 */

#pragma once

#include <iostream>
#include <thread>

#include "../ProdOrCons.hpp"
 
#include "osyncstream.hpp"
#include "MessageBox.hpp"


/*
 * Consommateur de messages
 */
class Consumer : public ProdOrCons< MessageBox > {
public:
    // Le constructeur de ProdOrCons sera utilisé comme constructeur de Consumer
    using ProdOrCons::ProdOrCons;
 
    void operator()() override {
        // TODO :
        // - Retirer de box_ nb_messages_ entiers avec attente aléatoire avant
        //   chaque retrait.
        // - Afficher des messages entre chaque étape pour suivre l'avancement.
        // - Afficher un message d'erreur si un nombre négatif est extrait.

        using microseconds = std::chrono::duration< int, std::micro >;

        for (int i = 0; i < nb_messages_; i++) {
            std::this_thread::sleep_for(microseconds{ random_engine_() });
            int message = box_.get();
            if (message < 0) {
                osyncstream(std::cout) << "ERROR : Consumer " << name_ << " a reçu un message négatif : " << message << "\n";
            } else {
                osyncstream(std::cout) << "Consumer " << name_ << " a reçu la message " << message << " (" << i+1 << "/" << nb_messages_ << ")\n";
            }
        }
    }
};