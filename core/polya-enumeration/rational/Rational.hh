#pragma once

#include "core/util/Type.hh"

#include <compare>
#include <concepts>
#include <cstdint>
#include <ostream>
#include <string>

namespace polya
{
class Rational
{
public:
    using Numerator = Type<std::int64_t, struct NumeratorTag>;
    using Denominator = Type<std::int64_t, struct DenominatorTag>;

    template <std::integral T>
    explicit Rational(T anInteger) : theNumerator{anInteger}, theDenominator{1}
    {
    }

    explicit Rational(Numerator aNumerator, Denominator aDenominator);

    auto reduce() -> void;

    auto operator+=(const Rational& aRational) -> Rational&;
    auto operator-=(const Rational& aRational) -> Rational&;
    auto operator*=(const Rational& aRational) -> Rational&;
    auto operator/=(const Rational& aRational) -> Rational&;
    [[nodiscard]] auto operator+(const Rational& aRational) const -> Rational;
    [[nodiscard]] auto operator-(const Rational& aRational) const -> Rational;
    [[nodiscard]] auto operator*(const Rational& aRational) const -> Rational;
    [[nodiscard]] auto operator/(const Rational& aRational) const -> Rational;

    [[nodiscard]] auto operator==(const Rational& aRational) const -> bool;
    [[nodiscard]] auto operator<=>(const Rational& aRational) const -> std::strong_ordering;

    [[nodiscard]] auto isIntegral() const -> bool;
    [[nodiscard]] auto asInteger() const -> std::int64_t;

    [[nodiscard]] auto toString() const -> std::string;
    friend auto operator<<(std::ostream& aStream, const Rational& aRational) -> std::ostream&;

private:
    Numerator theNumerator;
    Denominator theDenominator;
};
} // namespace polya
