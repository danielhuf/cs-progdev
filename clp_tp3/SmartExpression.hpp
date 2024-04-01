/*
 * Cursus CentraleSupélec - Dominante Informatique et numérique
 * 3IF1020 - Concepts des langages de programmation - TP n°3
 *
 * SmartExpression.hpp
 */

#ifndef SMART_EXPRESSION_HPP_INCLUDED
#define SMART_EXPRESSION_HPP_INCLUDED

#include <iostream>
#include <string>
#include <utility>
#include <memory>

using namespace std;

class SmartExpression {
public:

    SmartExpression();
    virtual ~SmartExpression();
    friend ostream& operator<<(ostream& os, SmartExpression const& e);

    virtual ostream& print(ostream& os) const = 0;
    virtual unique_ptr<SmartExpression> derive(string const& s) const = 0;
    virtual unique_ptr<SmartExpression> clone() const = 0;
    virtual unique_ptr<SmartExpression> simplify() const = 0;
    static int compteur;

private:
};

class Nombre : public SmartExpression {
public:

    explicit Nombre(double n);
    ostream& print(ostream& os) const override;
    unique_ptr<SmartExpression> derive(string const& s) const override;
    unique_ptr<SmartExpression> clone() const override;
    unique_ptr<SmartExpression> simplify() const override;
    double getN() const;
    static std::shared_ptr<Nombre> Zero();
    static std::shared_ptr<Nombre> One();

private:
    double n_;
    static std::shared_ptr<Nombre> zero;
    static std::shared_ptr<Nombre> one;
};

class Variable : public SmartExpression {
public:

    explicit Variable(string const& s);
    ostream& print(ostream& os) const override;
    unique_ptr<SmartExpression> derive(string const& s) const override;
    unique_ptr<SmartExpression> clone() const override;
    unique_ptr<SmartExpression> simplify() const override;

private:
    string s_;
};

class Operation : public SmartExpression {
public:

    Operation(unique_ptr<SmartExpression> gauche, unique_ptr<SmartExpression> droite);
    Operation(const Operation& o);
    virtual ~Operation() = default;

protected:

    unique_ptr<SmartExpression> gauche_;
    unique_ptr<SmartExpression> droite_;
};

class Addition : public Operation {
public:

    using Operation::Operation;
    ostream& print(ostream& os) const override;
    unique_ptr<SmartExpression> derive(string const& s) const override;
    unique_ptr<SmartExpression> clone() const override;
    unique_ptr<SmartExpression> simplify() const override;
};

class Multiplication : public Operation {
public:

    using Operation::Operation;
    ostream& print(ostream& os) const override;
    unique_ptr<SmartExpression> derive(string const& s) const override;
    unique_ptr<SmartExpression> clone() const override;
    unique_ptr<SmartExpression> simplify() const override;
};

#endif