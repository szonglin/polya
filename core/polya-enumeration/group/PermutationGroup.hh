#pragma once

#include "core/polya-enumeration/permutation/Permutation.hh"

#include "core/util/Type.hh"

#include <cstdint>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

namespace polya
{
class PermutationGroup
{
public:
    using Elements = Type<std::vector<Permutation>, struct ElementsTag>;
    using Generators = Type<std::vector<Permutation>, struct GeneratorsTag>;
    using Order = Type<std::uint32_t, struct OrderTag>;
    using Degree = Permutation::Degree;

    explicit PermutationGroup(std::string_view aName, Elements anElements);
    explicit PermutationGroup(
        std::string_view aName, Degree aDegree, const Generators& aGenerators
    );

    [[nodiscard]] auto name() const -> std::string_view;
    [[nodiscard]] auto order() const -> Order;
    [[nodiscard]] auto degree() const -> Degree;
    [[nodiscard]] auto elements() const -> const Elements&;
    [[nodiscard]] auto contains(const Permutation& aPermutation) const -> bool;

    [[nodiscard]] auto toString() const -> std::string;
    friend auto operator<<(std::ostream& aStream, const PermutationGroup& aGroup) -> std::ostream&;

    [[nodiscard]] auto isValidGroup() const -> bool; // Checks the group properties

private:
    [[nodiscard]] auto hasClosure() const -> bool;
    [[nodiscard]] auto hasIdentity() const -> bool;
    [[nodiscard]] auto hasInverse() const -> bool;

    std::string theName;
    Elements theElements; // Guaranteed to be ordered
};

namespace groups
{
auto cyclic(Permutation::Degree aDegree) -> PermutationGroup;
auto dihedral(Permutation::Degree aDegree) -> PermutationGroup;
auto symmetric(Permutation::Degree aDegree) -> PermutationGroup;
auto trivial(Permutation::Degree aDegree) -> PermutationGroup;
auto tetrahedron() -> PermutationGroup;
auto cube() -> PermutationGroup;
} // namespace groups

} // namespace polya
