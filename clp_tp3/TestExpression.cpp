/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Concepts des langages de programmation - TP n°3
 *
 * TestExpression.cpp
 * c++ -std=c++20 -o TestExpression TestExpression.cpp Expression.cpp -lgtest -lpthread
 */

#include <sstream>
#include <utility>

#include <gtest/gtest.h>

#include "Expression.hpp"

void TestAffichage( Expression const& e, std::string const& s )
{
    std::ostringstream os;
    os << e;
    EXPECT_EQ( os.str(), s );
}

TEST( TestExpression, TestAffichageNombre )
{
    Nombre n( 12345 );
    TestAffichage( n, "12345" );
}

TEST ( TestExpression, TestAffichageVariable )
{
    Variable v( "x1" );
    TestAffichage( v, "x1" );
}

TEST ( TestExpression, TestDerivationNombre )
{
    Nombre n( 12345 );
    Expression* e = n.derive( "x" );
    TestAffichage( *e, "0" );
    delete e;
}

TEST ( TestExpression, TestDerivationVariable1 )
{
    Variable v( "x" );
    Expression* e = v.derive( "x" );
    TestAffichage( *e, "1" );
    delete e;
}

TEST ( TestExpression, TestDerivationVariable2 )
{
    Variable v( "x" );
    Expression* e = v.derive( "y" );
    TestAffichage( *e, "0" );
    delete e;
}

TEST ( TestExpression, TestCompteur )
{
    EXPECT_EQ( Expression::compteur, 0 );

    Expression* e1 = new Nombre( 12345 );
    EXPECT_EQ( Expression::compteur, 1 );

    Expression* e2 = new Variable( "x" );
    EXPECT_EQ( Expression::compteur, 2 );

    Expression* e3 = e1->derive( "x" );
    EXPECT_EQ( Expression::compteur, 3 );

    delete e1;
    EXPECT_EQ( Expression::compteur, 2 );
    
    delete e2;
    delete e3;
    EXPECT_EQ( Expression::compteur, 0 );
}

TEST ( TestExpression, TestClonageNombre )
{
    Nombre n( 12345 );
    Expression* e = n.clone();
    TestAffichage( *e, "12345" );
    delete e;
}

TEST ( TestExpression, TestClonageVariable )
{
    Variable v( "x" );
    Expression* e = v.clone();
    TestAffichage( *e, "x" );
    delete e;
}

TEST ( TestExpression, TestClonageDerive )
{
    Variable v( "x" );
    Expression* e1 = v.derive( "x" );
    Expression* e2 = e1->clone();
    TestAffichage( *e2, "1" );
    delete e1;
    delete e2;
}

TEST ( TestExpression, TestAffichageAddition )
{
    Expression* e1 = new Addition( new Variable( "x" ), new Nombre( 12345 ) );
    TestAffichage( *e1, "(x + 12345)" );

    Expression* e2 = new Addition( new Variable( "x" ), new Variable( "y" ) );
    TestAffichage( *e2, "(x + y)" );

    delete e1;
    delete e2;
}

TEST ( TestExpression, TestDerivationAddition )
{
    Variable v( "x" );

    Expression* e1 = new Addition( v.clone(), new Nombre( -12345 ) );
    Expression* e2 = e1->derive( "x" );
    TestAffichage( *e2, "(1 + 0)" );

    Expression* e3 = new Addition( v.clone(), new Variable( "y" ) );
    Expression* e4 = e3->derive( "x" );
    TestAffichage( *e4, "(1 + 0)" );

    Expression* e5 = new Addition( v.clone(), v.clone() );
    Expression* e6 = e5->derive( "x" );
    TestAffichage( *e6, "(1 + 1)" );

    Expression* e7 = e6->derive( "x" );
    TestAffichage( *e7, "(0 + 0)" );

    delete e1;
    delete e2;
    delete e3;
    delete e4;
    delete e5;
    delete e6;
    delete e7;
}

TEST ( TestExpression, TestClonageAddition )
{
    Variable v( "x" );

    Expression* e1 = new Addition( v.clone(), new Nombre( -12345 ) );
    Expression* e2 = e1->clone();
    TestAffichage( *e2, "(x + -12345)" );

    Expression* e3 = new Addition( v.clone(), new Variable( "y" ) );
    Expression* e4 = e3->clone();
    TestAffichage( *e4, "(x + y)" );

    delete e1;
    delete e2;
    delete e3;
    delete e4;
} 

TEST ( TestExpression, TestAffichageMultiplication )
{
    Expression* e1 = new Multiplication( new Variable( "x" ), new Nombre( 12345 ) );
    TestAffichage( *e1, "(x * 12345)" );

    Expression* e2 = new Multiplication( new Variable( "x" ), new Variable( "y" ) );
    TestAffichage( *e2, "(x * y)" );

    delete e1;
    delete e2;
}

TEST ( TestExpression, TestDerivationMultiplication )
{
    Variable v( "x" );

    Expression* e1 = new Multiplication( v.clone(), new Nombre( -12345 ) );
    Expression* e2 = e1->derive( "x" );
    TestAffichage( *e2, "((1 * -12345) + (x * 0))" );

    Expression* e3 = new Multiplication( v.clone(), new Variable( "y" ) );
    Expression* e4 = e3->derive( "x" );
    TestAffichage( *e4, "((1 * y) + (x * 0))" );

    Expression* e5 = new Multiplication( v.clone(), v.clone() );
    Expression* e6 = e5->derive( "x" );
    TestAffichage( *e6, "((1 * x) + (x * 1))" );

    Expression* e7 = e6->derive( "x" );
    TestAffichage( *e7, "(((0 * x) + (1 * 1)) + ((1 * 1) + (x * 0)))" );

    delete e1;
    delete e2;
    delete e3;
    delete e4;
    delete e5;
    delete e6;
    delete e7;
} 

TEST ( TestExpression, TestClonageMultiplication )
{
    Variable v( "x" );

    Expression* e1 = new Multiplication( v.clone(), new Nombre( -12345 ) );
    Expression* e2 = e1->clone();
    TestAffichage( *e2, "(x * -12345)" );

    Expression* e3 = new Multiplication( v.clone(), new Variable( "y" ) );
    Expression* e4 = e3->clone();
    TestAffichage( *e4, "(x * y)" );

    delete e1;
    delete e2;
    delete e3;
    delete e4;
}

TEST ( TestExpression, TestSimplificationAddition )
{
    Expression* e1 = new Addition( new Variable( "x" ), new Nombre( 0 ) );
    Expression* e2 = e1->simplify();
    TestAffichage( *e2, "x" );

    Expression* e3 = new Addition( new Variable( "x" ), new Nombre( 12345 ) );
    Expression* e4 = e3->derive( "x" )->simplify();
    TestAffichage( *e4, "1" );

    Expression* e5 = new Addition( new Addition( new Variable( "x" ), new Variable( "x" ) ), new Variable( "x" ) );
    Expression* e6 = e5->derive( "x" )->simplify();
    TestAffichage( *e6, "3" );

    Expression* e7 = new Addition( new Variable("x"), new Variable("x") );
    Expression* e8 = new Addition( new Variable("x"), new Variable("x") );
    Expression* e9 = new Addition(e7, e8);
    Expression* e10 = e9->derive( "x" )->simplify();
    TestAffichage(*e10, "4");

    Expression* e11 = new Addition( new Nombre( 4 ), new Nombre( 5 ) );
    Expression* e12 = e11->simplify();
    TestAffichage( *e12, "9" );

    delete e1;
    delete e2;
    delete e3;
    delete e4;
    delete e5;
    delete e6;
    delete e9; // e7 et e8 sont déjà libérés dans le destructeur de e9
    delete e10;
    delete e11;
    delete e12;
}

TEST (TestExpression, TestSimplificationMultiplication )
{
    Expression* e1 = new Multiplication( new Variable( "x" ), new Nombre( 0 ) );
    Expression* e2 = e1->simplify();
    TestAffichage( *e2, "0" );

    Expression* e3 = new Multiplication( new Variable( "x" ), new Nombre( 1 ) );
    Expression* e4 = e3->simplify();
    TestAffichage( *e4, "x" );

    Expression* e5 = new Multiplication( new Nombre( 4 ), new Nombre( 5 ) );
    Expression* e6 = e5->simplify();
    TestAffichage( *e6, "20" );

    Expression* e7 = new Multiplication( new Variable( "x" ), new Variable( "x" ) );
    Expression* e8 = e7->derive( "x" )->simplify();
    TestAffichage( *e8, "(x + x)" );

    Expression* e9 = new Multiplication( new Nombre( 2 ), new Nombre( 0.5 ) );
    Expression* e10 = new Multiplication( new Nombre( 10 ), new Nombre( 0.1 ) );
    Expression* e11 = new Multiplication(e9, e10);
    Expression* e12 = e11->simplify();
    TestAffichage( *e12, "1" );

    delete e1;
    delete e2;
    delete e3;
    delete e4;
    delete e5;
    delete e6;
    delete e7;
    delete e8;
    delete e11; // e9 et e10 sont déjà libérés dans le destructeur de e11
    delete e12;
}

TEST (TesExpression, TestSimplificationAdditionMultiplication )
{
    Expression* e1 = new Multiplication( new Nombre( 1 ), new Addition( new Variable( "x" ), new Nombre( 0 ) ) );
    Expression* e2 = e1->simplify();
    TestAffichage( *e2, "x" );

    delete e1;
    delete e2;
}

TEST (TestExpression, TestNombreInstances)
{
    // Réinitialisation du compteur
    Expression::compteur = 0;

    // Création d'une expression complexe
    Expression* expr = new Addition(
        new Nombre( 2 ),
        new Multiplication(
            new Variable( "x" ),
            new Nombre( 3 )
        )
    );

    // Dérivation de l'expression
    Expression* derived = expr->derive( "x" );

    // Simplification de l'expression dérivée
    Expression* simplified = derived->simplify();

    // Affichage de l'expression simplifiée
    cout << *simplified << endl;

    // Affichage du nombre d'instances créées
    cout << "Nombre d'instances créées : " << Expression::compteur << endl; // 8

    delete expr;
    delete derived;
    delete simplified;

}
 
int main( int argc, char * argv[] )
{
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}