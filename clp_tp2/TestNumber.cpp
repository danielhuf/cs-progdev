/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Concepts des langages de programmation - TP n°2
 *
 * TestNumber.cpp
 * c++ -std=c++20 -o TestNumber Number.cpp TestNumber.cpp -lgtest -lpthread
 */

#include <exception>
#include <sstream>
#include <string>
#include <utility>

#include <cstdlib>
#include <ctime>


#include <gtest/gtest.h>

#include "Number.hpp"

// Tests de la construction par défaut
TEST( TestNumber, TestNumber0 )
{
    Number n{ 0 };
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "0" );
}
 
TEST( TestNumber, TestNumber12345678 )
{
    Number n{ 12345678 };
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "12345678" );
}
 
TEST( TestNumber, TestNumberBig )
{
    Number n{ 12345123451234512345UL };
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "12345123451234512345" );
}

TEST( TestNumber, TestNumberRandom )
{
    auto r{ std::rand() };
    Number n{ static_cast< unsigned long >( r )};
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), std::to_string( r ));
}

// Tests de la construction par copie
TEST( TestNumber, TestNumberCopy0 )
{
    Number n{ 0 };
    Number m{ n };
    std::ostringstream os;
    os << m;
    EXPECT_EQ( os.str(), "0" );
}

TEST( TestNumber, TestNumberCopy12345678 )
{
    Number n{ 12345678 };
    Number m{ n };
    std::ostringstream os;
    os << m;
    EXPECT_EQ( os.str(), "12345678" );
}

TEST( TestNumber, TestNumberCopyBig )
{
    Number n{ 12345123451234512345UL };
    Number m{ n };
    std::ostringstream os;
    os << m;
    EXPECT_EQ( os.str(), "12345123451234512345" );
}

// Tests de l'opérateur d'affectation
TEST( TestNumber, TestNumberAssign0 )
{
    Number n{ 0 };
    Number m{ 12345678 };
    m = n;
    std::ostringstream os;
    os << m;
    EXPECT_EQ( os.str(), "0" );
}

TEST( TestNumber, TestNumberAssign12345678 )
{
    Number n{ 12345678 };
    Number m{ 0 };
    m = n;
    std::ostringstream os;
    os << m;
    EXPECT_EQ( os.str(), "12345678" );
}

TEST( TestNumber, TestNumberAssignBig )
{
    Number n{ 12345123451234512345UL };
    Number m = n;
    std::ostringstream os;
    os << m;
    EXPECT_EQ( os.str(), "12345123451234512345" );
}

// Tests de l'addition
TEST( TestNumber, TestNumberAdd0 )
{
    Number n{ 0 };
    n.add( 0 );
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "0" );
}

TEST( TestNumber, TestNumberAdd12345678 )
{
    Number n{ 12345678 };
    n.add( 0 );
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "12345678" );
}

TEST( TestNumber, TestNumberAddBig )
{
    Number n{ 12345678999 };
    n.add( 101 );
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "12345679100" );
}

// Tests de la multiplication
TEST( TestNumber, TestNumberMultiply0_1 )
{
    Number n{ 0 };
    n.multiply( 0 );
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "0" );
}

TEST( TestNumber, TestNumberMultiply0_2 )
{
    Number n{ 0 };
    n.multiply( 12345678 );
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "0" );
}

TEST( TestNumber, TestNumberMultiply0_3 )
{
    Number n{ 12345678 };
    n.multiply( 0 );
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "0" );
}

TEST( TestNumber, TestNumberMultiply12345678 )
{
    Number n{ 12345678 };
    n.multiply( 42 );
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "518518476" );
}

TEST( TestNumber, TestNumberMultiplyBig )
{
    Number n{ 12345678999 };
    n.multiply( 101 );
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "1246913578899" );
}

// Test de la factorielle
TEST( TestNumber, TestFactorial123 )
{
    std::ostringstream os;
    os << factorial( 123 );;
    EXPECT_EQ( os.str(), "121463043670253296757662432418812958554542170884833823153289181618292"
                         "358923621676688311569606126402021707358352212940477825910915704116514"
                         "72186029519906261646730733907419814952960000000000000000000000000000" );
}

// Tests de la construction par string
TEST( TestNumber, TestNumberString0 )
{
    Number n{ "0" };
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "0" );
}

TEST( TestNumber, TestNumberString12345678 )
{
    Number n{ "12345678" };
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "12345678" );
}

TEST( TestNumber, TestNumberStringBig )
{
    Number n{ "12345123451234512345" };
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "12345123451234512345" );
}

TEST( TestNumber, TestNotNumberString_1 )
{
    EXPECT_THROW( Number n{ "abc" }, std::invalid_argument );
}

TEST( TestNumber, TestNotNumberString_2 )
{
    EXPECT_THROW( Number n{ "" }, std::invalid_argument );
}

TEST( TestNumber, TestNotNumberString_3 )
{
    EXPECT_THROW( Number n{ "123a" }, std::invalid_argument );
}

// Tests du flux d'entrée
TEST( TestNumber, TestNumberInput0 )
{
    std::istringstream in{ "0" };
    Number n { 1 };
    in >> n;
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "0" );
}

TEST( TestNumber, TestNumberInput12345678 )
{
    std::istringstream in{ "12345678" };
    Number n { 1 };
    in >> n;
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "12345678" );
}

TEST( TestNumber, TestNumberInputBig )
{
    std::istringstream in{ "12345123451234512345" };
    Number n { "1" };
    in >> n;
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "12345123451234512345" );
}

// Tests des opérateurs d'addition et de multiplication
TEST( TestNumber, TestNumberAddAssignInt )
{
    Number n{ 100 };
    n += 100;
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "200" );
}

TEST ( TestNumber, TestNumberAddInt )
{
    Number n{ 100 };
    Number m{ n + 100 };
    std::ostringstream os;
    os << m;
    EXPECT_EQ( os.str(), "200" );
}

TEST( TestNumber, TestNumberMultiplyAssignInt )
{
    Number n{ 100 };
    n *= 4;
    std::ostringstream os;
    os << n;
    EXPECT_EQ( os.str(), "400" );
}

TEST ( TestNumber, TestNumberMultiplyInt )
{
    Number n{ 100 };
    Number m{ n * 4 };
    std::ostringstream os;
    os << m;
    EXPECT_EQ( os.str(), "400" );
}

// Tests de la construction par déplacement
TEST( TestNumber, TestNumberMove0 )
{
    Number n{ 0 };
    Number m{ std::move( n ) };
    std::ostringstream os;
    os << m;
    EXPECT_EQ( os.str(), "0" );
}

TEST( TestNumber, TestNumberMove12345678 )
{
    Number n{ 12345678 };
    Number m{ std::move( n ) };
    std::ostringstream os;
    os << m;
    EXPECT_EQ( os.str(), "12345678" );
}

TEST( TestNumber, TestNumberMoveBig )
{
    Number n{ 12345123451234512345UL };
    Number m{ std::move( n ) };
    std::ostringstream os;
    os << m;
    EXPECT_EQ( os.str(), "12345123451234512345" );
}

// Tests de l'opérateur d'affectation par déplacement
TEST( TestNumber, TestNumberMoveAssign0 )
{
    Number n{ 0 };
    Number m{ 12345678 };
    m = std::move( n );
    std::ostringstream os;
    os << m;
    EXPECT_EQ( os.str(), "0" );
}

TEST( TestNumber, TestNumberMoveAssign12345678 )
{
    Number n{ 12345678 };
    Number m{ 0 };
    m = std::move( n );
    std::ostringstream os;
    os << m;
    EXPECT_EQ( os.str(), "12345678" );
}

TEST( TestNumber, TestNumberMoveAssignBig )
{
    Number n{ 12345123451234512345UL };
    Number m = std::move( n );
    std::ostringstream os;
    os << m;
    EXPECT_EQ( os.str(), "12345123451234512345" );
}

int main( int argc, char * argv[] )
{
    std::srand( std::time( nullptr ));
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
