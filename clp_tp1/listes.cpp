/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Concepts des langages de programmation - TP n°1
 *
 * listes.cpp
 */

#include <iostream>
#include <forward_list>
#include <functional>
#include <limits>

#include <cstdlib>
#include <ctime>

// Type liste unidirectionnelle d'entiers
using int_list_t = std::forward_list< int >;
using int_list_iter_t = int_list_t::const_iterator;
using std::cout;

// ============================= 2.1. Création, affichage =============================  //

int_list_t random_list() 
{
    int_list_t list;

    // Ajout d'un nombre aléatoire d'éléments entre 0 et 9 avec des valeurs entre 0 et 99
    for (int i = 0; i < rand() % 10; ++i) 
        list.push_front(rand() % 100);

    return list;
}

void print_list(const int_list_t & list)
{
    // Démarrage de l'affichage par une parenthèse ouvrante
    cout << "( ";

    // Affichage de chaque élément de la liste
    for (int i : list) 
        cout << i << " ";

    // Affichage de la parenthèse fermante et passage à la ligne
    cout << ")" << std::endl;
}

void test_21()
{
    // Création d'une liste aléatoire
    int_list_t list = random_list();

    // Affichage du nom et de la liste
    cout << "\n*** test_21 ***\n";
    print_list(list);

    // On vérifie aditionellement qu'il est possible de gérer une liste vide :

    // On initialise une liste avec un élément par défaut
    int_list_t empty_list = { 1 };

    // On recrée la liste avec 'empty_list' jusqu'à ce qu'elle soit vide
    while (!empty_list.empty()) 
        empty_list = random_list();

    print_list(empty_list);
    cout << "On a bien géré une liste vide !\n";
}

// ============================= 2.2. Application =============================  //

int_list_t map_iter(const int_list_t & list, std::function<int(int)> func)
{
    int_list_t new_list;

    // Application de la fonction f à chaque élément de la liste
    for (int i : list) 
        new_list.push_front(func(i));

    return new_list;
}

void test_22()
{
    // Création d'une liste aléatoire
    int_list_t list = random_list();

    // Affichage du nom et de la liste
    cout << "\n*** test_22 ***\n";
    print_list(list);

    // Application de la fonction lambda à chaque élément de la liste
    list = map_iter(list, [](int i) { return i * 3; });

    // Affichage de la liste modifiée
    print_list(list);
}

// ============================= 2.3. Filtrage =============================  //

int_list_t filter_iter(const int_list_t & list, std::function<bool(int)> predicat)
{
    int_list_t new_list;

    // Ajout des éléments de la liste vérifiant la condition
    for (int i : list) 
        if (predicat(i)) 
            new_list.push_front(i);

    return new_list;
}

void test_23()
{
    // Création d'une liste aléatoire
    int_list_t list = random_list();

    // Affichage du nom et de la liste
    cout << "\n*** test_23 ***\n";
    print_list(list);

    // Application de la fonction lambda à chaque élément de la liste
    list = map_iter(list, [](int i) { return i * 3; });

    // Affichage de la liste modifiée
    print_list(list);

    // Filtrage de la liste
    list = filter_iter(list, [](int i) { return i % 2 == 0; });

    // Affichage de la liste modifiée
    print_list(list);
}

// ============================= 2.4. Capture =============================  //

void test_24()
{

    int coef = rand() % 5 + 1;

    // Création d'une liste aléatoire
    int_list_t list = random_list();

    // Affichage du nom et de la liste
    cout << "\n*** test_24 ***\n";
    print_list(list);

    cout << "coefficient : " << coef << "\n";

    // Application de la fonction lambda à chaque élément de la liste
    list = map_iter(list, [coef](int i) { return i * coef; });

    // Affichage de la liste modifiée
    print_list(list);

    // Filtrage de la liste
    list = filter_iter(list, [](int i) { return i % 2 == 0; });

    // Affichage de la liste modifiée
    print_list(list);

}

// ============================= 2.5. Réduction =============================  //

int reduce(const int_list_t & list, int init, std::function<int(int, int)> func)
{
    int res = init;

    // Application de la fonction f à chaque élément de la liste
    for (int i : list) 
        res = func(res, i);

    return res;
}

void test_25()
{
    // Création d'une liste aléatoire
    int_list_t list = random_list();

    // Affichage du nom et de la liste
    cout << "\n*** test_25 ***\n";
    print_list(list);

    // On vérifie au debut si la liste est vide avant de trouver le minimum et le maximum
    if (list.empty()) 
        cout << "La liste est vide !\n";
    else {
        // Trouver le minimum de la liste en utilisant la fonction reduce
        int res = reduce(list, std::numeric_limits<int>::max(), [](int a, int b) { return a < b ? a : b; });
        cout << "Minimum de la liste : " << res << "\n";

        // Trouver le maximum de la liste en utilisant la fonction reduce
        res = reduce(list, std::numeric_limits<int>::min(), [](int a, int b) { return a > b ? a : b; });
        cout << "Maximum de la liste : " << res << "\n";
    }
}

// ============================= 3.1. Récursion pour reduce() =============================  //

int fold_left_aux(int_list_iter_t it, int_list_iter_t end, int init, std::function<int(int, int)> func)
{
    // Si on est arrivé à la fin de la liste, on retourne le résultat
    if (it == end) 
        return init;

    // On applique la fonction f à l'élément courant et on rappelle la fonction sur l'élément suivant
    return fold_left_aux(std::next(it), end, func(init, *it), func);
}

int fold_left(const int_list_t & list, int init, std::function<int(int, int)> func)
{
    // Appel de la fonction auxiliaire
    return fold_left_aux(list.cbegin(), list.cend(), init, func);
}

void test_31()
{
    // Création d'une liste aléatoire
    int_list_t list = random_list();

    // Affichage du nom et de la liste
    cout << "\n*** test_31 ***\n";
    print_list(list);

    // On vérifie au debut si la liste est vide avant de trouver le minimum et le maximum
    if (list.empty()) 
        cout << "La liste est vide !\n";
    else {
        // Trouver le minimum de la liste en utilisant la fonction recursive
        int res = fold_left(list, std::numeric_limits<int>::max(), [](int a, int b) { return a < b ? a : b; });
        cout << "Minimum de la liste : " << res << "\n";

        // Trouver le maximum de la liste en utilisant la fonction recursive
        res = fold_left(list, std::numeric_limits<int>::min(), [](int a, int b) { return a > b ? a : b; });
        cout << "Maximum de la liste : " << res << "\n";
    }
}

// ============================= 3.2. Récursion pour map() et filter() =============================  //

int_list_t map_aux(int_list_iter_t it, int_list_iter_t end, std::function<int(int)> func)
{
    // Si on est arrivé à la fin de la liste, on retourne une liste vide
    if (it == end) 
        return {};

    // On applique la fonction f à l'élément courant
    int res = func(*it);

    // Ensuite on rappelle la fonction sur l'élément suivant
    int_list_t new_list = map_aux(std::next(it), end, func);

    // On ajoute le résultat au début de la nouvelle liste
    new_list.push_front(res);

    return new_list;
}

int_list_t map(const int_list_t & list, std::function<int(int)> func)
{
    // Appel de la fonction auxiliaire
    return map_aux(list.cbegin(), list.cend(), func);
}

int_list_t filter_aux(int_list_iter_t it, int_list_iter_t end, std::function<bool(int)> predicat)
{
    // Si on est arrivé à la fin de la liste, on retourne une liste vide
    if (it == end) 
        return {};

    // On vérifie si l'élément courant vérifie la condition
    if (predicat(*it)) {

        // Si oui, on récupère l'élément courant
        int res = *it;

        // On rappelle la fonction sur l'élément suivant
        int_list_t new_list = filter_aux(std::next(it), end, predicat);

        // On ajoute l'élément courant au début de la nouvelle liste
        new_list.push_front(res);

        return new_list;
    } else
        // Si non, on rappelle la fonction sur l'élément suivant
        return filter_aux(std::next(it), end, predicat);
}

int_list_t filter(const int_list_t & list, std::function<bool(int)> predicat)
{
    // Appel de la fonction auxiliaire
    return filter_aux(list.cbegin(), list.cend(), predicat);
}

void test_32()
{
    int coef = rand() % 5 + 1;

    // Création d'une liste aléatoire
    int_list_t list = random_list();

    // Affichage du nom et de la liste
    cout << "\n*** test_32 ***\n";
    print_list(list);

    cout << "coefficient : " << coef << "\n";

    // Application de la fonction lambda à chaque élément de la liste
    list = map(list, [coef](int i) { return i * coef; });

    // Affichage de la liste modifiée
    print_list(list);

    // Filtrage de la liste
    list = filter(list, [](int i) { return i % 2 == 0; });

    // Affichage de la liste modifiée
    print_list(list);
}

// ============================= 4.1. Application partielle =============================  //

void test_41()
{
    int coef = rand() % 5 + 1;

    // Création d'une liste aléatoire
    int_list_t list = random_list();

    // Affichage du nom et de la liste
    cout << "\n*** test_41 ***\n";
    print_list(list);

    cout << "coefficient : " << coef << "\n";

    // Application de la fonction obetnue par application partielle à chaque élément de la liste
    // On utilise std::bind() ici pour appliquer std::multiplies() à coef
    list = map(list, std::bind(std::multiplies<int>(), coef, std::placeholders::_1));

    // Affichage de la liste modifiée
    print_list(list);

    // Filtrage de la liste
    list = filter(list, [](int i) { return i % 2 == 0; });

    // Affichage de la liste modifiée
    print_list(list);
}

// ============================= 4.2. Réduction inversée =============================  //

int fold_right_aux(int_list_iter_t it, int_list_iter_t end, int init, std::function<int(int, int)> func)
{
    // Si on est arrivé à la fin de la liste, on retourne le résultat
    if (it == end) 
        return init; 

    // On applique la fonction f à l'élément courant et on rappelle la fonction sur l'élément suivant
    return func(*it, fold_right_aux(std::next(it), end, init, func));
}

int fold_right(const int_list_t & list, int init, std::function<int(int, int)> func)
{
    // Appel de la fonction auxiliaire
    return fold_right_aux(list.cbegin(), list.cend(), init, func);
}

void test_42()
{
    // Création d'une liste aléatoire
    int_list_t list = random_list();

    // Affichage du nom et de la liste
    cout << "\n*** test_42 ***\n";
    print_list(list);

    // On vérifie au debut si la liste est vide avant d'effectuer les opérations
    if (list.empty()) 
        cout << "La liste est vide !\n";
    else {

        // On effectue un fold_left() sur la liste en utilisant l'opération de soustraction et en partant de 0
        int left_fold_res = fold_left(list, 0, [](int a, int b) { return a - b; });

        // On effectue un fold_right() sur la liste en utilisant l'opération de soustraction et en partant de 0
        int right_fold_res = fold_right(list, 0, [](int a, int b) { return a - b; });

        // Affichage des résultats
        cout << "Résultat de fold_left() : " << left_fold_res << "\n";
        cout << "Résultat de fold_right() : " << right_fold_res << "\n";

        if (left_fold_res == right_fold_res)
            cout << "Les résultats sont identiques !\n";
        else
            cout << "Les résultats sont différents !\n";

        /* En effet, l'opération fold_left dans ce cas effectue: 
         * ((((0 - a0) - a1) - a2) - ... an)
         *
         * alors que l'opération fold_right effectue:
         * a0 - (a1 - (a2 - ...(an - 0)))
         * 
         * On peut voir que les parenthèses et les opérandes ne sont pas placés de la même manière,
         * ce qui explique que les résultats sont différents.
        */
    }
}

// =================================== MAIN ===================================  //

int main()
{
    std::srand( std::time( nullptr ));

    test_21();
    test_22();
    test_23();
    test_24();
    test_25();
    test_31();
    test_32();
    test_41();
    test_42();

    return 0;
}

