#include "core/polya-enumeration/polynomial/Polynomial.hh"

#include "core/util/Exception.hh"

#include <range/v3/all.hpp>
#include <string>
#include <utility>

namespace polya
{
namespace views = ranges::views;
using VariableName = Polynomial::VariableName;
using Exponent = Polynomial::Exponent;
using Term = Polynomial::Term;

auto Polynomial::TermComparator::operator()(const Term& aFirstTerm, const Term& aSecondTerm)
    const noexcept -> bool
{
    const auto myFirstSumOfExponents = ranges::accumulate(
        aFirstTerm.get(), 0,
        [](const auto& anAccumulator, const auto& aValue) { return anAccumulator + aValue.get(); }
    );
    const auto mySecondSumOfExponents = ranges::accumulate(
        aSecondTerm.get(), 0,
        [](const auto& anAccumulator, const auto& aValue) { return anAccumulator + aValue.get(); }
    );
    if (myFirstSumOfExponents == mySecondSumOfExponents)
    {
        return ranges::lexicographical_compare(
            aFirstTerm.get() | views::transform(&Exponent::underlying),
            aSecondTerm.get() | views::transform(&Exponent::underlying)
        );
    }
    return myFirstSumOfExponents < mySecondSumOfExponents;
}

Polynomial::Polynomial(std::vector<VariableName> theVariableNames)
    : theVariableNames{std::move(theVariableNames)}
{
}

auto Polynomial::coefficient(const Term& aTerm) const -> Rational
{
    return theCoefficientMap.contains(aTerm) ? theCoefficientMap.at(aTerm) : Rational{0};
}

auto Polynomial::set(const Term& aTerm, const Rational& aCoefficient) -> void
{
    ensure(
        aTerm.get().size() == theVariableNames.size(),
        "Expected term with {} exponents, but received {}", theVariableNames.size(),
        aTerm.get().size()
    );
    if (aCoefficient == Rational{0})
    {
        return;
    }
    theCoefficientMap.insert_or_assign(aTerm, aCoefficient);
}

auto Polynomial::isZero() const -> bool
{
    return theCoefficientMap.empty();
}

auto Polynomial::variables() const -> const std::vector<VariableName>&
{
    return theVariableNames;
}

auto Polynomial::terms() const -> const std::map<Term, Rational, TermComparator>&
{
    return theCoefficientMap;
}

auto Polynomial::operator+=(const Polynomial& aPolynomial) -> Polynomial&
{
    ensure(
        variables() == aPolynomial.variables(), "Cannot add polynomials with different variables"
    );

    for (const auto& [myTerm, myCoefficient] : aPolynomial.terms())
    {
        set(myTerm, coefficient(myTerm) + myCoefficient);
    }
    return *this;
}

auto Polynomial::operator-=(const Polynomial& aPolynomial) -> Polynomial&
{
    ensure(
        variables() == aPolynomial.variables(),
        "Cannot subtract polynomials with different variables"
    );

    for (const auto& [myTerm, myCoefficient] : aPolynomial.terms())
    {
        set(myTerm, coefficient(myTerm) - myCoefficient);
    }
    return *this;
}

auto Polynomial::operator*=(const Polynomial& aPolynomial) -> Polynomial&
{
    ensure(
        variables() == aPolynomial.variables(),
        "Cannot multiply polynomials with different variables"
    );

    auto myResult = Polynomial{theVariableNames};
    for (const auto& [myFirst, mySecond] :
         views::cartesian_product(theCoefficientMap, aPolynomial.terms()))
    {
        const auto& [myTerm, myCoefficient] = myFirst;
        const auto& [myOtherTerm, myOtherCoefficient] = mySecond;
        const auto myNewExponents = views::zip(myTerm.get(), myOtherTerm.get())
                                    | views::transform(
                                        [](const auto& aPair)
                                        {
                                            const auto& [myFirst, mySecond] = aPair;
                                            return Exponent{myFirst.get() + mySecond.get()};
                                        }
                                    )
                                    | ranges::to<std::vector>();
        const auto myNewTerm = Term{myNewExponents};
        myResult.set(
            myNewTerm, myResult.coefficient(myNewTerm) + myCoefficient * myOtherCoefficient
        );
    }
    *this = std::move(myResult);
    return *this;
}

auto Polynomial::operator+(const Polynomial& aPolynomial) const -> Polynomial
{
    auto myResult = *this;
    myResult += aPolynomial;
    return myResult;
}

auto Polynomial::operator-(const Polynomial& aPolynomial) const -> Polynomial
{
    auto myResult = *this;
    myResult -= aPolynomial;
    return myResult;
}

auto Polynomial::operator*(const Polynomial& aPolynomial) const -> Polynomial
{
    auto myResult = *this;
    myResult *= aPolynomial;
    return myResult;
}

auto Polynomial::operator==(const Polynomial& aPolynomial) const -> bool
{
    return variables() == aPolynomial.variables() and terms() == aPolynomial.terms();
}

auto Polynomial::toString() const -> std::string
{
    if (isZero())
    {
        return "0";
    }

    return terms() | views::keys
           | views::transform([&](const auto& aTerm) { return formatTerm(aTerm); })
           | views::join(' ') | ranges::to<std::string>();
}

auto Polynomial::formatTerm(const Term& aTerm) const -> std::string
{
    auto myString = std::string{};
    const auto myCoefficient = coefficient(aTerm);
    if (myCoefficient == Rational{0})
    {
        return myString;
    }
    myString += '+';
    myString += myCoefficient.toString();
    myString += views::zip(theVariableNames, aTerm.get())
                | views::filter(
                    [](const auto& aPair)
                    {
                        const auto& [aName, anIndex] = aPair;
                        return anIndex.get() != 0;
                    }
                )
                | views::transform(
                    [](const auto& aPair)
                    {
                        const auto& [aName, anIndex] = aPair;
                        return aName.get() + '^' + std::to_string(anIndex.get());
                    }
                )
                | views::join('*') | ranges::to<std::string>();
    return myString;
}

auto operator<<(std::ostream& aStream, const Polynomial& aPolynomial) -> std::ostream&
{
    return aStream << aPolynomial.toString();
}

auto operator*(const Rational& aRational, const Polynomial& aPolynomial) -> Polynomial
{
    auto myPolynomial = aPolynomial;
    for (const auto& [myTerm, myCoefficient] : myPolynomial.terms())
    {
        myPolynomial.set(myTerm, aRational * myCoefficient);
    }
    return myPolynomial;
}

auto operator*(const Polynomial& aPolynomial, const Rational& aRational) -> Polynomial
{
    return aRational * aPolynomial;
}

} // namespace polya
