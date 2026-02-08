#include "core/polya-enumeration/polya/Polya.hh"

#include "core/util/Exception.hh"
#include "core/util/Power.hh"

#include <cstdint>
#include <range/v3/all.hpp>
#include <string>
#include <utility>

namespace polya
{
namespace views = ranges::views;

namespace
{
auto generatingFunction(
    const std::vector<Polynomial::VariableName>& aColourVariables, Polynomial::Exponent aPower
) -> Polynomial
{
    const auto myColourCount = aColourVariables.size();
    auto myResult = Polynomial{aColourVariables};

    for (const auto myColour : views::iota(0uz, myColourCount))
    {
        auto myExponents =
            std::vector<Polynomial::Exponent>(myColourCount, Polynomial::Exponent{0});
        myExponents[myColour] = aPower;
        myResult.set(Polynomial::Term{std::move(myExponents)}, Rational{1});
    }

    return myResult;
}
} // namespace

auto cycleIndexPolynomial(
    const PermutationGroup& aGroup,
    const std::optional<std::vector<Polynomial::VariableName>>& aVariableNames
) -> CycleIndexPolynomial
{
    const auto myDegreeValue = aGroup.degree().get();
    auto myVariableNames = aVariableNames.value_or(
        views::iota(1uz, myDegreeValue + 1)
        | views::transform([](const auto& aValue)
                           { return Polynomial::VariableName{"x_" + std::to_string(aValue)}; })
        | ranges::to<std::vector<Polynomial::VariableName>>()
    );
    ensure(
        myVariableNames.size() == myDegreeValue,
        "Expected the number of variable names ({}) to match the degree ({})",
        myVariableNames.size(), myDegreeValue
    );

    auto myPolynomial = Polynomial(myVariableNames);
    const auto myGroupFactor = Rational{
        Rational::Numerator{1},
        Rational::Denominator{static_cast<std::int64_t>(aGroup.order().get())}};

    for (const auto& myElement : aGroup.elements().get())
    {
        const auto& myDistribution = myElement.cycleStructure().theCycleLengthDistribution;
        auto myExponents =
            views::iota(1uz, myDegreeValue + 1)
            | views::transform(
                [&myDistribution](const auto& aLength)
                {
                    return Polynomial::Exponent{
                        aLength < myDistribution.size() ? myDistribution.at(aLength).get() : 0uz};
                }
            )
            | ranges::to<std::vector<Polynomial::Exponent>>();
        const auto myTerm = Polynomial::Term{std::move(myExponents)};
        myPolynomial.set(myTerm, myPolynomial.coefficient(myTerm) + myGroupFactor);
    }
    return CycleIndexPolynomial{myPolynomial};
}

auto evaluateUniform(const CycleIndexPolynomial& aCycleIndex, orbits::ColourCount aColourCount)
    -> orbits::OrbitCount
{
    const auto substituteIntoTerm = [aColourCount](const auto& aPair)
    {
        const auto& [myTerm, myCoefficient] = aPair;
        const auto myExponent = mathutil::Exponent{ranges::accumulate(
            myTerm.get() | views::transform(&Polynomial::Exponent::underlying), 0uz
        )};
        return myCoefficient
               * Rational{mathutil::power(mathutil::Base{aColourCount.get()}, myExponent).get()};
    };
    const auto myResult = ranges::accumulate(
        aCycleIndex.get().terms() | views::transform(substituteIntoTerm), Rational{0}
    );
    return orbits::OrbitCount{myResult.asInteger()};
}

auto evaluateColours(
    const CycleIndexPolynomial& aCycleIndex, orbits::ColourCount aColourCount,
    const std::optional<std::vector<Polynomial::VariableName>>& aColourNames
) -> Polynomial
{

    auto myColourVariables = aColourNames.value_or(
        views::iota(1uz, aColourCount.get() + 1)
        | views::transform([](const auto& aValue)
                           { return Polynomial::VariableName{"c_" + std::to_string(aValue)}; })
        | ranges::to<std::vector<Polynomial::VariableName>>()
    );
    ensure(
        myColourVariables.size() == aColourCount.get(),
        "Expected the number of colour names ({}) to match the colour count ({})",
        myColourVariables.size(), aColourCount.get()
    );

    auto myResult = Polynomial{myColourVariables};

    for (const auto& [myTerm, myCoefficient] : aCycleIndex.get().terms())
    {
        auto myProduct = Polynomial{myColourVariables};
        myProduct.set(
            Polynomial::Term{
                std::vector<Polynomial::Exponent>(aColourCount.get(), Polynomial::Exponent{0})},
            myCoefficient
        );

        for (const auto myCycleLength : views::iota(1uz, myTerm.get().size() + 1))
        {
            const auto myExponent = myTerm.get()[myCycleLength - 1];
            if (myExponent == Polynomial::Exponent{0})
            {
                continue;
            }
            const auto myPowerSum =
                generatingFunction(myColourVariables, Polynomial::Exponent{myCycleLength});

            for ([[maybe_unused]] const auto myPowerIndex : views::iota(0uz, myExponent.get()))
            {
                myProduct *= myPowerSum;
            }
        }
        myResult += myProduct;
    }

    return myResult;
}
} // namespace polya