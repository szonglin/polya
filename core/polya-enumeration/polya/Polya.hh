#pragma once

#include "core/polya-enumeration/group/PermutationGroup.hh"
#include "core/polya-enumeration/orbit-counting/OrbitCounting.hh"
#include "core/polya-enumeration/polynomial/Polynomial.hh"
#include "core/util/Type.hh"

#include <optional>

namespace polya
{
using CycleIndexPolynomial = Type<Polynomial, struct CycleIndexPolynomialTag>;

// Generate the cycle index polynomial
auto cycleIndexPolynomial(
    const PermutationGroup& aGroup,
    const std::optional<std::vector<Polynomial::VariableName>>& aVariableNames = std::nullopt
) -> CycleIndexPolynomial;

// Evaluate the polynomial at a constant (equivalent to Orbit Counting)
auto evaluateUniform(const CycleIndexPolynomial& aCycleIndex, orbits::ColourCount aColourCount)
    -> orbits::OrbitCount;

// Polya Enumeration Theorem
auto evaluateColours(
    const CycleIndexPolynomial& aCycleIndex,
    orbits::ColourCount aColourCount,
    const std::optional<std::vector<Polynomial::VariableName>>& aColourNames = std::nullopt
) -> Polynomial;

} // namespace polya