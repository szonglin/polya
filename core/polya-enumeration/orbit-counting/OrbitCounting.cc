#include "core/polya-enumeration/orbit-counting/OrbitCounting.hh"

#include "core/polya-enumeration/rational/Rational.hh"
#include "core/util/Power.hh"

#include <range/v3/all.hpp>

namespace polya::orbits
{
namespace views = ranges::views;

auto countOrbits(const PermutationGroup& aGroup, ColourCount aColourCount) -> OrbitCount
{
    const auto myGroupOrderFactor =
        Rational{Rational::Numerator{1}, Rational::Denominator{aGroup.order().get()}};
    const auto myOrbitCount = ranges::accumulate(
        aGroup.elements().get()
            | views::transform(
                [aColourCount, myGroupOrderFactor](const Permutation& aPermutation)
                {
                    const auto myNumerator = mathutil::power(
                        mathutil::Base{aColourCount.get()},
                        mathutil::Exponent{aPermutation.asCycles().size()}
                    );
                    return Rational{myNumerator.get()} * myGroupOrderFactor;
                }
            ),
        Rational{0}
    );
    return OrbitCount{static_cast<std::uint64_t>(myOrbitCount.asInteger())};
}
} // namespace polya::orbits