/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Concepts des langages de programmation - TP n°3
 *
 * Expression.hpp
 */

#ifndef EXPRESSION_HPP_INCLUDED
#define EXPRESSION_HPP_INCLUDED

#include <iostream>
#include <string>
#include <utility>

using namespace std;

class Expression {
public:
    
    Expression();
    virtual ~Expression();
    friend ostream& operator<<(ostream& os, Expression const& e);
    virtual ostream& print(ostream& os) const = 0;

    // Dérivation
    /* Cette méthode prend en argument une variable par rapport à laquelle on dérive l'expression.
     * La méthode retourne un pointeur vers une nouvelle instance d'Expression qui réprésente la dérivée de l'expression courante.
     * La mémoire est allouée dynamiquement pour cette nouvelle instance à cause de l'opérateur new.
     * L'objet récepteur de l'appel de la méthode n'est pas modifié à cause de son définition en tant que const.
    */
    virtual Expression* derive(string const& s) const = 0;

    static int compteur;
    virtual Expression* clone() const = 0;
    virtual Expression* simplify() const = 0;

private:
};

class Nombre : public Expression {
public:
    
    Nombre(double n);
    virtual ~Nombre();
    ostream& print(ostream& os) const override;
    Expression* derive(string const& s) const override;
    Expression* clone() const override;
    Expression* simplify() const override;
    double getN() const;

private:
    double n_;
};

class Variable : public Expression {
public:
    
    Variable(string const& s);
    virtual ~Variable();
    ostream& print(ostream& os) const override;
    Expression* derive(string const& s) const override;
    Expression* clone() const override;
    Expression* simplify() const override;

private:
    string s_;
};

/* Pour déterminer le lien entre un opérateur et ses opérandes, on peut utiliser une classe abstraite 'Operation' et ses sous-classes spécifiques pour chaque type d'opération.
 * Pour le constructeur défaut de la classe Operation, on utilise des pointeurs vers les opérandes. Ce choix évite de faire des copies des opérandes, mais il faut faire attention à la durée de vie des opérandes pour éviter des références invalides.
 * Les opérandes passées en argument du constructeur sont allouées dynamiquement dehors de la classe et libérées dans le destructeur de la classe.
 * Pour le constructeur par copie, on utilise une référence vers une instance de Operation et on fait une copie de ses opérandes.
 * Les sous-classes de Operation ont des constructeurs par défaut et par copie qui appellent les constructeurs correspondants de la super-classe, ce qui évite le besoin de constructeurs dans les sous-classes.
 * Cependant, les sous-classes implémentent les méthodes print et derive de manière spécifique à chaque type d'opération.
*/
class Operation : public Expression {
public:
    
    Operation(Expression* gauche, Expression* droite);
    Operation(Operation const& o);
    virtual ~Operation();
    ostream& print(ostream& os) const = 0;
    Expression* derive(string const& s) const = 0;

protected:
    Expression* gauche_;
    Expression* droite_;
};

class Addition : public Operation {
public:
    
    using Operation::Operation; // Héritage des constructeurs de Operation
    ostream& print(ostream& os) const override;
    Expression* derive(string const& s) const override;
    Expression* clone() const override;
    Expression* simplify() const override;
};

class Multiplication : public Operation {
public:
    
    using Operation::Operation;
    ostream& print(ostream& os) const override;
    Expression* derive(string const& s) const override;
    Expression* clone() const override;
    Expression* simplify() const override;
};

#endif