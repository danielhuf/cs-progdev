/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Concepts des langages de programmation - TP n°3
 *
 * SmartExpression.cpp
 */

#include <iostream>
#include <string>
#include <utility>
#include <memory>

#include "SmartExpression.hpp"

ostream& operator<<(std::ostream& os, SmartExpression const& e) {
    e.print(os);
    return os;
}  

//============================ Expression ============================
int SmartExpression::compteur = 0;

SmartExpression::SmartExpression() {
    compteur++;
}

SmartExpression::~SmartExpression() {
    compteur--;
}

//============================ Nombre ============================
std::shared_ptr<Nombre> Nombre::zero = std::make_shared<Nombre>(0);
std::shared_ptr<Nombre> Nombre::one = std::make_shared<Nombre>(1);

Nombre::Nombre(double n) : n_(n) {}

ostream& Nombre::print(ostream& os) const {
    return os << n_;
}

unique_ptr<SmartExpression> Nombre::derive(string const&) const {
    return make_unique<Nombre>(*zero);
}

unique_ptr<SmartExpression> Nombre::clone() const {
    return make_unique<Nombre>(*this);
}

unique_ptr<SmartExpression> Nombre::simplify() const {
    return clone();
}

double Nombre::getN() const {
    return n_;
}

std::shared_ptr<Nombre> Nombre::Zero() {
    return zero;
}

std::shared_ptr<Nombre> Nombre::One() {
    return one;
}

//============================ Variable ============================
Variable::Variable(string const& s) : s_(s) {}

ostream& Variable::print(ostream& os) const {
    return os << s_;
}

unique_ptr<SmartExpression> Variable::derive(string const& s) const {
    return make_unique<Nombre>(s == s_ ? *Nombre::One() : *Nombre::Zero());
}

unique_ptr<SmartExpression> Variable::clone() const {
    return make_unique<Variable>(*this);
}

unique_ptr<SmartExpression> Variable::simplify() const {
    return clone();
}

//============================ Operation ============================
Operation::Operation(unique_ptr<SmartExpression> gauche, unique_ptr<SmartExpression> droite)
    : gauche_(move(gauche)), droite_(move(droite)) {}

Operation::Operation(const Operation& o)
    : gauche_(o.gauche_ ? o.gauche_->clone() : nullptr), 
      droite_(o.droite_ ? o.droite_->clone() : nullptr) {}

//============================ Addition ============================
ostream& Addition::print(ostream& os) const {
    os << "(";
    gauche_->print(os);
    os << " + ";
    droite_->print(os);
    os << ")";
    return os;
}

unique_ptr<SmartExpression> Addition::derive(string const& s) const {
    return make_unique<Addition>(gauche_->derive(s), droite_->derive(s));
}

unique_ptr<SmartExpression> Addition::clone() const {
    return make_unique<Addition>(gauche_->clone(), droite_->clone());
}

unique_ptr<SmartExpression> Addition::simplify() const {
    auto gauche = gauche_->simplify();
    auto droite = droite_->simplify();

    Nombre* gaucheNombre = dynamic_cast<Nombre*>(gauche.get());
    Nombre* droiteNombre = dynamic_cast<Nombre*>(droite.get());

    if (gaucheNombre && droiteNombre) {
        return make_unique<Nombre>(gaucheNombre->getN() + droiteNombre->getN());
    } else if (gaucheNombre && gaucheNombre->getN() == 0) {
        return move(droite);
    } else if (droiteNombre && droiteNombre->getN() == 0) {
        return move(gauche);
    } else {
        return make_unique<Addition>(move(gauche), move(droite));
    }
}

//============================ Multiplication ============================
ostream& Multiplication::print(ostream& os) const {
    os << "(";
    gauche_->print(os);
    os << " * ";
    droite_->print(os);
    os << ")";
    return os;
}

unique_ptr<SmartExpression> Multiplication::derive(string const& s) const {
    auto gauche = gauche_->clone();
    auto droite = droite_->clone();

    return make_unique<Addition>(
        make_unique<Multiplication>(gauche->derive(s), move(droite)),
        make_unique<Multiplication>(move(gauche), droite->derive(s))
    );
}

unique_ptr<SmartExpression> Multiplication::clone() const {
    return make_unique<Multiplication>(gauche_->clone(), droite_->clone());
}

unique_ptr<SmartExpression> Multiplication::simplify() const {
    auto gauche = gauche_->simplify();
    auto droite = droite_->simplify();

    Nombre* gaucheNombre = dynamic_cast<Nombre*>(gauche.get());
    Nombre* droiteNombre = dynamic_cast<Nombre*>(droite.get());

    if (gaucheNombre && droiteNombre) {
        return make_unique<Nombre>(gaucheNombre->getN() * droiteNombre->getN());
    } else if (gaucheNombre && gaucheNombre->getN() == 0 || droiteNombre && droiteNombre->getN() == 0) {
        return make_unique<Nombre>(0);
    } else if (gaucheNombre && gaucheNombre->getN() == 1) {
        return move(droite);
    } else if (droiteNombre && droiteNombre->getN() == 1) {
        return move(gauche);
    } else {
        return make_unique<Multiplication>(move(gauche), move(droite));
    }
}

/* Réponse à la question : Pourquoi est-il nécessaire de continuer à créer des instances dans Nombre::simplifie() et Variable::simplifie() ? Proposer une solution pour remédier à ce point.
 * Actuellement, ces deux méthodes créent des nouvelles instances à chaque appel, ce qui peut être inefficace, surtout si les objets sont simples et ne changent pas lors de la simplification.
 * Pour remedier à ce problème, une approche qui maintient l'utilisation de 'unique_ptr' mais minimise la création d'instances est d'utiliser des méthodes statiques, comme c'etait le cas pour les méthodes 'Zero' et 'One' de la classe 'Nombre'.
 * On peut écrire par exemple :
 
unique_ptr<SmartExpression> Nombre::simplify() const {
    // Retourner une instance statique pour éviter la création d'une nouvelle instance
    return Nombre::getStaticInstance(n_);
}

unique_ptr<SmartExpression> Variable::simplify() const {
    // Supposant l'existence d'une méthode similaire à getStaticInstance pour Variable
    return Variable::getStaticInstance(s_);
}

 * Où 'getStaticInstance' serait une méthode statique qui gère une collection d'instances uniques et retourne un pointeur vers l'instance correspondante.
 * Cependant, implémenter une telle méthode peut être trop complexe et peut introduire des problèmes inattendus liés à la gestion de la mémoire et au partage d'instances entre différents parties du code.
*/