/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Concepts des langages de programmation - TP n°1
 *
 * tris.cpp
 */

#include <iostream>
#include <vector>
#include <utility>
#include <functional>

#include <cstdlib>
#include <ctime>

// Nommons nos types pour plus de facilité d'écriture
using int_tab_t = std::vector< int >;
using std::cout;

// ============================= 1.1 Affichage d'un tableau =============================  //

void print_tab( const int_tab_t & tab )
{
    // Démarrage de l'affichage par un crochet ouvrant
    cout << "[ ";

    // Affichage de chaque élément du tableau
    for ( int i : tab )
        cout << i << " ";

    // Affichage du crochet fermant et passage à la ligne
    cout << "]" << std::endl;
}

void test_11()
{
    // Tableau de 6 entiers ayant les valeurs indiquées
    const int_tab_t tab{ 1, -2, 3, -4, 5, -6 };

    // Affichage du nom et du tableau
    cout << "\n*** test_11 ***\n";
    print_tab( tab );
}

// ================= 1.2. Remplissage d'un tableau avec des nombres aléatoires =================  //

void random_tab( int_tab_t & tab )
{
    // Remplissage du tableau avec des valeurs aléatoires entre -10 et 10
    for ( int & i : tab )
        i = rand() % 21 - 10;
}

void test_12()
{
    // Creation d'un tableau de 10 entiers
    int_tab_t tab( 10 );

    // Remplissage du tableau avec des valeurs aléatoires
    random_tab( tab );

    // Affichage du nom et du tableau
    cout << "\n*** test_12 ***\n";
    print_tab( tab );
}

// ============================= 1.3. Tri par ordre croissant =============================  //

void sort_tab_1( int_tab_t & tab )
{
    int n = tab.size();

    // Tri du tableau par sélection par ordre croissant
    for ( int i = 0; i < n-1; ++i ) {
        int minIndex = i;

        // Recherche du minimum dans le reste du tableau
        for ( int j = i + 1; j < n; ++j )
            if (tab[j] < tab[minIndex] )
                minIndex = j;

        // Echange de la valeur courante avec le minimum
        std::swap( tab[i], tab[minIndex] );
    }
}

void test_13()
{
    // Creation d'un tableau de 10 entiers
    int_tab_t tab( 10 );

    // Remplissage du tableau avec des valeurs aléatoires
    random_tab( tab );

    // Affichage du nom et du tableau
    cout << "\n*** test_13 ***\n";
    print_tab( tab );

    // Tri du tableau
    sort_tab_1( tab );

    // Affichage du tableau trié
    print_tab( tab );
}

// ============================= 1.4. Tri selon un autre critère =============================  //

// Pointeur de fonction pour les fonctions de comparaison 
typedef bool (*Compare)(int, int);

bool less(int a, int b) 
{
    return a < b;
}

bool greater(int a, int b) 
{
    return a > b;
}

void sort_tab_2( int_tab_t & tab, Compare compare )
{
    int n = tab.size();

    // Tri du tableau par sélection
    for ( int i = 0; i < n-1; ++i ) {
        int minMaxIndex = i;

        // Recherche du minimum dans le reste du tableau
        for ( int j = i + 1; j < n; ++j )
            if (compare(tab[j], tab[minMaxIndex]))
                minMaxIndex = j;

        // Echange de la valeur courante avec le minimum
        std::swap( tab[i], tab[minMaxIndex] );
    }
}

void test_14()
{
    // Creation d'un tableau de 10 entiers
    int_tab_t tab( 10 );

    // Remplissage du tableau avec des valeurs aléatoires
    random_tab( tab );

    // Affichage du nom et du tableau
    cout << "\n*** test_14 ***\n";
    print_tab( tab );

    // Tri du tableau par ordre croissant
    sort_tab_2( tab, less );

    // Affichage du tableau trié
    print_tab( tab );

    // Tri du tableau par ordre décroissant
    sort_tab_2( tab, greater );

    // Affichage du tableau trié
    print_tab( tab );
}

// ============================= 1.5. Tri avec fonction lambda =============================  //

void sort_tab_3( int_tab_t & tab, std::function<bool(int, int)> compare )
{
    int n = tab.size();

    // Tri du tableau par sélection
    for ( int i = 0; i < n-1; ++i ) {
        int minMaxIndex = i;

        // Recherche du minimum dans le reste du tableau
        for ( int j = i + 1; j < n; ++j )
            if (compare(tab[j], tab[minMaxIndex]))
                minMaxIndex = j;

        // Echange de la valeur courante avec le minimum
        std::swap( tab[i], tab[minMaxIndex] );
    }
}

void test_15()
{
    // Creation d'un tableau de 10 entiers
    int_tab_t tab( 10 );

    // Remplissage du tableau avec des valeurs aléatoires
    random_tab( tab );

    // Affichage du nom et du tableau
    cout << "\n*** test_15 ***\n";
    print_tab( tab );

    // Tri du tableau par ordre croissant avec une fonction lambda
    sort_tab_3( tab, [](int a, int b) { return abs(a) < abs(b); } );

    // Affichage du tableau trié
    print_tab( tab );

    // Tri du tableau par ordre décroissant avec une fonction lambda
    sort_tab_3( tab, [](int a, int b) { return abs(a) > abs(b); } );

    // Affichage du tableau trié
    print_tab( tab );
}

// =================================== MAIN ===================================  //

int main()
{
    srand( time( nullptr ));

    test_11();
    test_12();
    test_13();
    test_14();
    test_15();

    return 0;
}

