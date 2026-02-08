#include "core/polya-enumeration/orbit-counting/OrbitCounting.hh"
#include "core/polya-enumeration/group/PermutationGroup.hh"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace polya::test
{
using namespace ::testing;
using OrbitCount = orbits::OrbitCount;
using ColourCount = orbits::ColourCount;

class OrbitCountingTest : public ::testing::Test
{
};

TEST_F(OrbitCountingTest, TrivialGroup)
{
    const auto myGroup = groups::trivial(Permutation::Degree{3});
    EXPECT_THAT(orbits::countOrbits(myGroup, ColourCount{2}), Eq(OrbitCount{8}));
}

TEST_F(OrbitCountingTest, SingleColour)
{
    const auto myGroup = groups::cyclic(Permutation::Degree{4});
    EXPECT_THAT(orbits::countOrbits(myGroup, ColourCount{1}), Eq(OrbitCount{1}));
}

TEST_F(OrbitCountingTest, Necklace4Beads2Colours)
{
    const auto myGroup = groups::cyclic(Permutation::Degree{4});
    EXPECT_THAT(orbits::countOrbits(myGroup, ColourCount{2}), Eq(OrbitCount{6}));
}

TEST_F(OrbitCountingTest, Necklace4Beads3Colours)
{
    const auto myGroup = groups::cyclic(Permutation::Degree{4});
    EXPECT_THAT(orbits::countOrbits(myGroup, ColourCount{3}), Eq(OrbitCount{24}));
}

TEST_F(OrbitCountingTest, Bracelet4Beads2Colours)
{
    const auto myGroup = groups::dihedral(Permutation::Degree{4});
    EXPECT_THAT(orbits::countOrbits(myGroup, ColourCount{2}), Eq(OrbitCount{6}));
}

TEST_F(OrbitCountingTest, Bracelet4Beads3Colours)
{
    const auto myGroup = groups::dihedral(Permutation::Degree{4});
    EXPECT_THAT(orbits::countOrbits(myGroup, ColourCount{3}), Eq(OrbitCount{21}));
}

TEST_F(OrbitCountingTest, SymmetricGroup4Elements3Colours)
{
    const auto myGroup = groups::symmetric(Permutation::Degree{4});
    EXPECT_THAT(orbits::countOrbits(myGroup, ColourCount{3}), Eq(OrbitCount{15}));
}

TEST_F(OrbitCountingTest, TetrahedronFaces2Colours)
{
    const auto myGroup = groups::tetrahedron();
    EXPECT_THAT(orbits::countOrbits(myGroup, ColourCount{2}), Eq(OrbitCount{5}));
}

TEST_F(OrbitCountingTest, TetrahedronFaces3Colours)
{
    const auto myGroup = groups::tetrahedron();
    EXPECT_THAT(orbits::countOrbits(myGroup, ColourCount{3}), Eq(OrbitCount{15}));
}

TEST_F(OrbitCountingTest, CubeFaces2Colours)
{
    const auto myGroup = groups::cube();
    EXPECT_THAT(orbits::countOrbits(myGroup, ColourCount{2}), Eq(OrbitCount{10}));
}

TEST_F(OrbitCountingTest, CubeFaces3Colours)
{
    const auto myGroup = groups::cube();
    EXPECT_THAT(orbits::countOrbits(myGroup, ColourCount{3}), Eq(OrbitCount{57}));
}

TEST_F(OrbitCountingTest, CubeFaces20Colours)
{
    const auto myGroup = groups::cube();
    EXPECT_THAT(orbits::countOrbits(myGroup, ColourCount{20}), Eq(OrbitCount{2'690'800}));
}

TEST_F(OrbitCountingTest, CubeFaces50Colours)
{
    const auto myGroup = groups::cube();
    EXPECT_THAT(orbits::countOrbits(myGroup, ColourCount{50}), Eq(OrbitCount{651'886'250}));
}

} // namespace polya::test
