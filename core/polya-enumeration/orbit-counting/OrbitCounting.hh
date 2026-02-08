#pragma once

#include "core/polya-enumeration/group/PermutationGroup.hh"
#include "core/util/Type.hh"

#include <cstdint>

namespace polya::orbits
{
using OrbitCount = Type<std::uint64_t, struct ResultTag>;
using ColourCount = Type<std::uint32_t, struct ColourCountTag>;

// Orbit-Counting Theorem
auto countOrbits(const PermutationGroup& aGroup, ColourCount aColourCount) -> OrbitCount;
} // namespace polya::orbits