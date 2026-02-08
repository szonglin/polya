#pragma once

#include "core/util/Type.hh"

#include <compare>
#include <cstddef>
#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

namespace polya
{
// A permutation of a set S is a bijection S -> S. We use S = {0, 1, ..., d - 1}.
class Permutation
{
public:
    using Degree = Type<std::size_t, struct DegreeTag>;
    using Element = Type<std::uint32_t, struct ElementTag>;
    using Cycle = Type<std::vector<Element>, struct CycleTag>;

    struct CycleStructure
    {
        using Count = Type<std::size_t, struct CountTag>;
        std::vector<Count> theCycleLengthDistribution;

        [[nodiscard]] auto totalCycleCount() const -> Count;
        [[nodiscard]] auto degree() const -> Degree;
    };
    Permutation() = default;
    explicit Permutation(Degree aDegree);
    explicit Permutation(std::vector<Element> aBijection);
    explicit Permutation(Degree aDegree, const std::vector<Cycle>& aCycles);

    [[nodiscard]] auto operator()(Element anElement) const -> Element;
    auto operator*=(const Permutation& aPermutation) -> Permutation&; // Permutation composition
    auto operator*(const Permutation& aPermutation) const -> Permutation;

    [[nodiscard]] auto operator==(const Permutation& aPermutation) const -> bool;
    [[nodiscard]] auto operator<=>(const Permutation& aPermutation) const -> std::strong_ordering;

    [[nodiscard]] auto inverse() const -> Permutation;
    [[nodiscard]] auto power(std::int32_t aPower) const -> Permutation;

    [[nodiscard]] auto degree() const noexcept -> Degree;

    [[nodiscard]] auto isIdentity() const noexcept -> bool;
    [[nodiscard]] auto asCycles() const -> std::vector<Cycle>;
    [[nodiscard]] auto cycleStructure() const -> CycleStructure;

    [[nodiscard]] auto toString() const -> std::string;
    friend auto operator<<(std::ostream& aStream, const Permutation& aPermutation) -> std::ostream&;

private:
    [[nodiscard]] auto isValidElement(Element anElement) const noexcept -> bool;

    std::vector<Element> theBijection;
};

namespace permutations
{
auto rotation(Permutation::Degree aDegree) -> Permutation;
auto reflection(Permutation::Degree aDegree) -> Permutation;
auto transposition(
    Permutation::Degree aDegree, Permutation::Element aFirst, Permutation::Element aSecond
) -> Permutation;
} // namespace permutations

} // namespace polya
