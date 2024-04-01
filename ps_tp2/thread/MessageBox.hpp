/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Programmations système - TP n°2
 *
 * MessageBox.hpp
 */

#pragma once

#include <mutex>
#include <condition_variable>

#include "../BasicMessageBox.hpp"

/*
 * FIFO d'echange de messages entre producteurs et consommateurs
 * Version pour synchronisation entre threads
 */
class MessageBox : public BasicMessageBox {
public:
    void put( int message ) {
        // TODO :
        // - Ajouter les instructions de synchronisation
        // - Ne pas faire d'affichage dans cette méthode

        std::unique_lock<std::mutex> lock(mutex);

        not_full.wait(lock, [this](){ return nb_messages < box_size_; });    // On attend que la boite ne soit pas pleine
        basic_put( message );                                                // On ajoute le message
        nb_messages++;                                                       // On incrémente le nombre de messages
        not_empty.notify_one();                                              // On notifie qu'il y a un message de plus
    }
 
    int get() {
        // TODO :
        // - Ajouter les instructions de synchronisation
        // - Ne pas faire d'affichage dans cette méthode

        std::unique_lock<std::mutex> lock(mutex);

        not_empty.wait(lock, [this](){ return nb_messages > 0; });           // On attend qu'il y ait un message
        int message{ basic_get() };                                          // On récupère le message
        nb_messages--;                                                       // On décrémente le nombre de messages
        not_full.notify_one();                                               // On notifie qu'il y a un message de moins
        
        return message;
    }
private:
    // TODO : 
    // - Ajouter les objets de synchronisation
    std::mutex mutex;
    std::condition_variable not_empty;
    std::condition_variable not_full;
    int nb_messages = 0;
};