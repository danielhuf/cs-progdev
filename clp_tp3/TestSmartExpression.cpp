/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Concepts des langages de programmation - TP n°3
 *
 * TestSmartExpression.cpp
 * c++ -std=c++20 -o TestSmartExpression TestSmartExpression.cpp SmartExpression.cpp -lgtest -lpthread
 */

#include <sstream>
#include <utility>

#include <gtest/gtest.h>

#include "SmartExpression.hpp"

TEST( TestSmartExpression, TestNombreInstances )
{
    // Réinitialisation du compteur
    SmartExpression::compteur = 0;

    // Création d'une expression complexe
    unique_ptr<SmartExpression> expr = make_unique<Addition>(
        make_unique<Nombre>( 2 ),
        make_unique<Multiplication>(
            make_unique<Variable>( "x" ),
            make_unique<Nombre>( 3 )
        )
    );

    // Dérivation de l'expression
    unique_ptr<SmartExpression> derived = expr->derive( "x" );

    // Simplification de l'expression dérivée
    unique_ptr<SmartExpression> simplified = derived->simplify();

    // Affichage de l'expression simplifiée
    cout << *simplified << endl;

    // Affichage du nombre d'instances créées
    cout << "Nombre d'instances créées : " << SmartExpression::compteur << endl;
}
 
int main( int argc, char * argv[] )
{
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
