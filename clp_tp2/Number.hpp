/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Concepts des langages de programmation - TP n°2
 *
 * Number.hpp
 */

#ifndef NUMBER_HPP_INCLUDED
#define NUMBER_HPP_INCLUDED

#include <iostream>
#include <string>
#include <utility>

class Number {
public:

    Number( unsigned long l );
    Number( const Number & n );
    Number( std::string s);
    Number( Number && n ) noexcept;
    ~Number();
    Number & operator=( const Number & n );
    Number & operator=( Number && n ) noexcept;
    void print( std::ostream & out ) const;
    void add ( unsigned int i );
    void multiply ( unsigned int i );

private:
    using DigitType = unsigned int;
    // Un seul chiffre décimal par maillon : l'objectif ici n'est pas la performance
    static const DigitType number_base{ 10u };

    struct Digit {
        DigitType digit_;
        Digit * next_;

        Digit( unsigned long l );
        Digit( const Digit & d );
        Digit( std::string s );
        ~Digit();
        Digit & operator=( const Digit & d );
        void print( std::ostream & out ) const;
        void add( unsigned int i );
        void multiply ( unsigned int i );
    };
    Digit * first_;
};

inline std::ostream & operator<<( std::ostream & out, const Number & n )
{
    n.print( out );
    return out;
}

Number factorial( unsigned int i );
std::istream & operator>>( std::istream & in, Number & n );
Number & operator+=( Number & n, unsigned int i );
Number & operator*=( Number & n, unsigned int i );
Number operator+( Number n, unsigned int i );
Number operator*( Number n, unsigned int i );

#endif