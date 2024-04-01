/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Concepts des langages de programmation - TP n°3
 *
 * Expression.cpp
 */

#include <iostream>
#include <string>
#include <utility>

#include "Expression.hpp"

ostream& operator<<(std::ostream& os, Expression const& e) {
    e.print(os);
    return os;
}

//============================ Expression ============================
// Compteur d'instances
int Expression::compteur = 0;

// Constructeur par défaut
Expression::Expression() {
    compteur++;
}

// Destructeur
Expression::~Expression() {
    compteur--;
}

//============================ Nombre ============================
// Constructeur par défaut
Nombre::Nombre(double n) {
    this->n_ = n;
}

// Destructeur
Nombre::~Nombre() {}

// Affichage
ostream& Nombre::print(std::ostream& os) const {
    return os << this->n_;
}

// Dérivation
Expression* Nombre::derive(std::string const& s) const {
    return new Nombre(0);
}

// Clonage
Expression* Nombre::clone() const {
    return new Nombre(*this);
}

// Simplification
Expression* Nombre::simplify() const {
    return clone();
}

// Getter
double Nombre::getN() const {
    return this->n_;
}

//============================ Variable ============================
// Constructeur par défaut
Variable::Variable(std::string const& s) {
    this->s_ = s;
}

// Destructeur
Variable::~Variable() {}

// Affichage
ostream& Variable::print(std::ostream& os) const {
    return os << this->s_;
}

// Dérivation
Expression* Variable::derive(std::string const& s) const {
    if (this->s_ == s) {
        return new Nombre(1);
    } else {
        return new Nombre(0);
    }
}

// Clonage
Expression* Variable::clone() const {
    return new Variable(*this);
}

// Simplification
Expression* Variable::simplify() const {
    return clone();
}

//============================ Operation ============================
// Constructeur par défaut
Operation::Operation(Expression* gauche, Expression* droite) {
    this->gauche_ = gauche;
    this->droite_ = droite;
}

// Constructeur par copie
Operation::Operation(Operation const& o) {
    this->gauche_ = o.gauche_->clone();
    this->droite_ = o.droite_->clone();
}

// Destructeur
Operation::~Operation() {
    delete this->gauche_;
    delete this->droite_;
}

//============================ Addition ============================
// Affichage
ostream& Addition::print(std::ostream& os) const {
    os << "(";
    this->gauche_->print(os);
    os << " + ";
    this->droite_->print(os);
    os << ")";
    return os;
}

// Dérivation
Expression* Addition::derive(std::string const& s) const {
    return new Addition(this->gauche_->derive(s), this->droite_->derive(s));
}

// Clonage
Expression* Addition::clone() const {
    return new Addition(this->gauche_->clone(), this->droite_->clone());
}

// Simplification
Expression* Addition::simplify() const {
    Expression* gauche = this->gauche_->simplify();
    Expression* droite = this->droite_->simplify();

    Nombre* gaucheNombre = dynamic_cast<Nombre*>(gauche);
    Nombre* droiteNombre = dynamic_cast<Nombre*>(droite);

    if (gaucheNombre && droiteNombre) {
        double result = gaucheNombre->getN() + droiteNombre->getN();
        delete gauche;
        delete droite;
        return new Nombre( result );
    } else if (gaucheNombre && gaucheNombre->getN() == 0) {
        delete gauche;
        return droite;
    } else if (droiteNombre && droiteNombre->getN() == 0) {
        delete droite;
        return gauche;
    } else {
        return new Addition(gauche, droite);
    }
}

//============================ Multiplication ============================
// Affichage
ostream& Multiplication::print(std::ostream& os) const {
    os << "(";
    this->gauche_->print(os);
    os << " * ";
    this->droite_->print(os);
    os << ")";
    return os;
}

// Dérivation 
Expression* Multiplication::derive(std::string const& s) const {
    // Cloner les opérandes pour éviter des références invalides
    Expression* gauche = this->gauche_->clone();
    Expression* droite = this->droite_->clone();

    return new Addition(
        new Multiplication(gauche->derive(s), droite),
        new Multiplication(gauche, droite->derive(s))
    );
}

// Clonage
Expression* Multiplication::clone() const {
    return new Multiplication(this->gauche_->clone(), this->droite_->clone());
}

// Simplification
Expression* Multiplication::simplify() const {
    Expression* gauche = this->gauche_->simplify();
    Expression* droite = this->droite_->simplify();

    Nombre* gaucheNombre = dynamic_cast<Nombre*>(gauche);
    Nombre* droiteNombre = dynamic_cast<Nombre*>(droite);

    if (gaucheNombre && droiteNombre) {
        double result = gaucheNombre->getN() * droiteNombre->getN();
        delete gauche;
        delete droite;
        return new Nombre( result );
    } else if (gaucheNombre && gaucheNombre->getN() == 0 || droiteNombre && droiteNombre->getN() == 0) {
        delete gauche;
        delete droite;
        return new Nombre(0);
    } else if (gaucheNombre && gaucheNombre->getN() == 1) {
        delete gauche;
        return droite;
    } else if (droiteNombre && droiteNombre->getN() == 1) {
        delete droite;
        return gauche;
    } else {  // Si aucune simplification n'est possible
        return new Multiplication(gauche, droite);
    }
}