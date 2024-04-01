/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmations système - TP n°2
 *
 * Producer.hpp
 */

#pragma once

#include <iostream>
#include <thread>

#include "../ProdOrCons.hpp"
 
#include "osyncstream.hpp"
#include "MessageBox.hpp"

/*
 * Producteur de messages
 */
class Producer : public ProdOrCons< MessageBox > {
public:
    // Le constructeur de ProdOrCons peut être utilisé pour Producer
    using ProdOrCons::ProdOrCons;
 
    void operator()() override {
        // TODO : 
        // - Déposer dans box_ nb_messages_ entiers positifs avec attente
        //   aléatoire avant chaque dépôt.
        // - Afficher des messages entre chaque étape pour suivre l'avancement.

        using microseconds = std::chrono::duration< int, std::micro >;

        for (int i = 0; i < nb_messages_; i++) {
            std::this_thread::sleep_for(microseconds{ random_engine_() });
            box_.put(i+1);
            osyncstream(std::cout) << "Producer " << name_ << " a envoyé la message " << i+1 << " (" << i+1 << "/" << nb_messages_ << ")\n";
        }
    }
};