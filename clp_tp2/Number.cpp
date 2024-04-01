/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Concepts des langages de programmation - TP n°2
 *
 * Number.cpp
 */

#include <exception>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>

#include "Number.hpp"

//================================ NUMBER ================================

// Construction par défaut
Number::Number( unsigned long l ) {
    first_ = new Digit(l);
}

// Construction par copie
Number::Number( const Number & n ) {
    first_ = new Digit(*n.first_);
}

// Construction par string
Number::Number( std::string s) {
    first_ = new Digit(s);
}

// Construction par déplacement
Number::Number( Number && n ) noexcept {
    first_ = n.first_;
    n.first_ = nullptr;
}

// Destruction
Number::~Number() {
    delete first_;
}

// Opérateur d'affectation
Number & Number::operator=( const Number & n ) {
    if ( this != &n ) {
        delete first_;
        first_ = new Digit(*n.first_);
    }
    return *this;
}

// Opérateur d'affectation par déplacement
Number & Number::operator=( Number && n ) noexcept {
    if ( this != &n ) {
        delete first_;
        first_ = n.first_;
        n.first_ = nullptr;
    }
    return *this;
}

// Affichage d'un nombre
void Number::print( std::ostream & out ) const {
    first_->print(out);
}

// Addition
void Number::add ( unsigned int i ) {
    first_->add(i);
}

// Multiplication
void Number::multiply ( unsigned int i ) {
    first_->multiply(i);
}

//================================ DIGIT ================================

// Construction par défaut
Number::Digit::Digit( unsigned long l ) {
    digit_ = l % number_base;
    if ( l / number_base != 0 ) {
        next_ = new Digit(l / number_base); // On crée le prochain maillon avec le reste de la division
    } else {
        next_ = nullptr;
    }
}

// Construction par copie
Number::Digit::Digit( const Digit & d ) {
    digit_ = d.digit_;
    if ( d.next_ != nullptr ) {
        next_ = new Digit(*d.next_);
    } else {
        next_ = nullptr;
    }
}

// Construction par string
Number::Digit::Digit( std::string s ) {
    digit_ = static_cast< unsigned int >( s.back() - '0' );

    if ( !(isdigit( s.back()) ) ) {
        throw std::invalid_argument( "Le string contient un caractère non numérique" );
    } else if ( s.empty() ) {
        throw std::invalid_argument( "Le string est vide" );
    }

    if ( s.length() > 1 ) {              // Si la chaine de caractère est plus longue que 1...
        next_ = new Digit( s.substr( 0, s.length() - 1 ) ); // On enlève le dernier caractère pour le prochain maillon
    } else {
        next_ = nullptr;
    }
}

// Destruction
Number::Digit::~Digit() {
    if ( next_ != nullptr ) {
        delete next_;
    }
}

// Opérateur d'affectation
Number::Digit & Number::Digit::operator=( const Digit & d ) {
    if ( this != &d ) {
        digit_ = d.digit_;
        if ( d.next_ != nullptr ) {
            next_ = new Digit(*d.next_);
        } else {
            next_ = nullptr;
        }
    }
    return *this;
}

// Affichage d'un chiffre
void Number::Digit::print( std::ostream & out ) const {
    if ( next_ != nullptr ) {
        next_->print(out);
    }
    out << digit_;
}

// Addition
void Number::Digit::add(unsigned int i) {
    unsigned int retenue = (static_cast<unsigned int>(digit_) + i) / number_base; // Calcul de la retenue après l'addition
    digit_ = (static_cast<unsigned int>(digit_) + i) % number_base;               // Calcul du modulo après l'addition

    if ( next_ != nullptr ) {
        next_->add(retenue);
    } else if ( retenue != 0 ) {
        next_ = new Digit(retenue);
    }
}

// Multiplication
void Number::Digit::multiply ( unsigned int i ) {
    if ( i == 0 ) { // Si i est zéro, le résultat est simplement zéro
        digit_ = 0;
        if ( next_ != nullptr ) {
            delete next_;
            next_ = nullptr;
        }
    } else {
        digit_ *= i;
        if ( next_ != nullptr ) { // Si le prochain maillon existe, on le multiplie aussi
            next_->multiply(i);
        }
        if ( digit_ >= number_base ) {  // Si le chiffre est supérieur à 10, on doit le diviser par 10
            if ( next_ == nullptr ) {   // Si le prochain maillon n'existe pas encore, on le crée
                next_ = new Digit(0); 
            }
            next_->add( digit_ / number_base );  // On ajoute le quotient de la division au prochain maillon
            digit_ %= number_base;               // On garde le reste de la division par 10 dans le maillon actuel
        }
    }
}

//================================ OPERATEURS ================================

Number factorial( unsigned int i ) {
    if ( i == 0 ) {
        return Number{1};
    } else {
        Number n = factorial( i - 1 );
        n.multiply( i );
        return n;
    }
}

std::istream & operator>>( std::istream & in, Number & n )
{
    in >> std::ws;
    std::string s = "";
    while ( in.good() ) {
        int c{ in.get() };
        if( std::isdigit( c )) {
            s += c;
        } else {
            in.putback( c );
            break;
        }
    }
    n = Number{ s };
    return in;
}

Number & operator+=( Number & n, unsigned int i )
{
    n.add( i );
    return n;
}

Number & operator*=( Number & n, unsigned int i )
{
    n.multiply( i );
    return n;
}

Number operator+( Number n, unsigned int i )
{
    return n += i;
}

Number operator*( Number n, unsigned int i )
{
    return n *= i;
}