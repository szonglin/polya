#pragma once

#include "core/polya-enumeration/rational/Rational.hh"
#include "core/util/Type.hh"

#include <cstdint>
#include <map>
#include <ostream>
#include <string>
#include <vector>

namespace polya
{
// A multivariate polynomial with a sparse representation
class Polynomial
{
public:
    using VariableName = Type<std::string, struct VariableNameTag>;
    using VariableCount = Type<std::uint32_t, struct VariableCountTag>;
    using Exponent = Type<std::uint32_t, struct ExponentTag>;
    using Term = Type<std::vector<Exponent>, struct TermTag>;

private:
    struct TermComparator
    {
        auto operator()(const Term& aFirstTerm, const Term& aSecondTerm) const noexcept -> bool;
    };

public:
    explicit Polynomial(std::vector<VariableName> theVariableNames);

    [[nodiscard]] auto coefficient(const Term& aTerm) const -> Rational;
    auto set(const Term& aTerm, const Rational& aCoefficient) -> void;

    [[nodiscard]] auto isZero() const -> bool;
    [[nodiscard]] auto variables() const -> const std::vector<VariableName>&;
    [[nodiscard]] auto terms() const -> const std::map<Term, Rational, TermComparator>&;

    auto operator+=(const Polynomial& aPolynomial) -> Polynomial&;
    auto operator-=(const Polynomial& aPolynomial) -> Polynomial&;
    auto operator*=(const Polynomial& aPolynomial) -> Polynomial&;
    [[nodiscard]] auto operator+(const Polynomial& aPolynomial) const -> Polynomial;
    [[nodiscard]] auto operator-(const Polynomial& aPolynomial) const -> Polynomial;
    [[nodiscard]] auto operator*(const Polynomial& aPolynomial) const -> Polynomial;

    [[nodiscard]] auto operator==(const Polynomial& aPolynomial) const -> bool;

    [[nodiscard]] auto toString() const -> std::string;
    friend auto operator<<(std::ostream& aStream, const Polynomial& aPolynomial) -> std::ostream&;

    friend auto operator*(const Rational& aRational, const Polynomial& aPolynomial) -> Polynomial;
    friend auto operator*(const Polynomial& aPolynomial, const Rational& aRational) -> Polynomial;

private:
    [[nodiscard]] auto formatTerm(const Term& aTerm) const -> std::string;

    std::vector<VariableName> theVariableNames;
    std::map<Term, Rational, TermComparator> theCoefficientMap;
};
} // namespace polya