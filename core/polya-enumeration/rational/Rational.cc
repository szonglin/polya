#include "core/polya-enumeration/rational/Rational.hh"

#include "core/util/Exception.hh"

#include <numeric>

namespace polya
{
Rational::Rational(Numerator aNumerator, Denominator aDenominator)
    : theNumerator{aNumerator}, theDenominator{aDenominator}
{
}

auto Rational::reduce() -> void
{
    ensure(theDenominator.get() != 0, "Cannot reduce fraction with zero denominator");
    if (theNumerator.get() == 0)
    {
        theDenominator = Denominator{1};
    }
    if (theDenominator.get() < 0)
    {
        theNumerator = Numerator{-theNumerator.get()};
        theDenominator = Denominator{-theDenominator.get()};
    }
    const auto myGcd = std::gcd(std::abs(theNumerator.get()), theDenominator.get());
    theNumerator = Numerator{theNumerator.get() / myGcd};
    theDenominator = Denominator{theDenominator.get() / myGcd};
}

auto Rational::operator+=(const Rational& aRational) -> Rational&
{
    const auto myNumerator = theNumerator.get() * aRational.theDenominator.get()
                             + aRational.theNumerator.get() * theDenominator.get();
    const auto myDenominator = theDenominator.get() * aRational.theDenominator.get();
    theNumerator = Numerator{myNumerator};
    theDenominator = Denominator{myDenominator};
    reduce();
    return *this;
}

auto Rational::operator-=(const Rational& aRational) -> Rational&
{
    const auto myNumerator = theNumerator.get() * aRational.theDenominator.get()
                             - aRational.theNumerator.get() * theDenominator.get();
    const auto myDenominator = theDenominator.get() * aRational.theDenominator.get();
    theNumerator = Numerator{myNumerator};
    theDenominator = Denominator{myDenominator};
    reduce();
    return *this;
}

auto Rational::operator*=(const Rational& aRational) -> Rational&
{
    theNumerator = Numerator{theNumerator.get() * aRational.theNumerator.get()};
    theDenominator = Denominator{theDenominator.get() * aRational.theDenominator.get()};
    reduce();
    return *this;
}

auto Rational::operator/=(const Rational& aRational) -> Rational&
{
    ensure(aRational.theNumerator.get() != 0, "Cannot divide by zero");
    theNumerator = Numerator{theNumerator.get() * aRational.theDenominator.get()};
    theDenominator = Denominator{theDenominator.get() * aRational.theNumerator.get()};
    reduce();
    return *this;
}

auto Rational::operator+(const Rational& aRational) const -> Rational
{
    auto myResult = *this;
    myResult += aRational;
    return myResult;
}

auto Rational::operator-(const Rational& aRational) const -> Rational
{
    auto myResult = *this;
    myResult -= aRational;
    return myResult;
}

auto Rational::operator*(const Rational& aRational) const -> Rational
{
    auto myResult = *this;
    myResult *= aRational;
    return myResult;
}

auto Rational::operator/(const Rational& aRational) const -> Rational
{
    auto myResult = *this;
    myResult /= aRational;
    return myResult;
}

auto Rational::operator==(const Rational& aRational) const -> bool
{
    return theNumerator.get() * aRational.theDenominator.get()
           == aRational.theNumerator.get() * theDenominator.get();
}

auto Rational::operator<=>(const Rational& aRational) const -> std::strong_ordering
{
    const auto myLhs = theNumerator.get() * aRational.theDenominator.get();
    const auto myRhs = aRational.theNumerator.get() * theDenominator.get();
    return myLhs <=> myRhs;
}

[[nodiscard]] auto Rational::isIntegral() const -> bool
{
    return theDenominator.get() == 1;
}

[[nodiscard]] auto Rational::asInteger() const -> std::int64_t
{
    ensure(
        isIntegral(), "Cannot convert fraction with denominator {} to an integer",
        theDenominator.get()
    );
    return theNumerator.get();
}

auto Rational::toString() const -> std::string
{
    return '(' + std::to_string(theNumerator.get()) + '/' + std::to_string(theDenominator.get())
           + ')';
}

auto operator<<(std::ostream& aStream, const Rational& aRational) -> std::ostream&
{
    return aStream << aRational.toString();
}

} // namespace polya
