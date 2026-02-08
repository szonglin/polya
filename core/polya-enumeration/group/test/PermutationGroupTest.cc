#include "core/polya-enumeration/group/PermutationGroup.hh"
#include "core/polya-enumeration/permutation/Permutation.hh"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <sstream>

namespace polya::test
{
using namespace ::testing;
using Degree = Permutation::Degree;
using Element = Permutation::Element;
using Cycle = Permutation::Cycle;
using Order = PermutationGroup::Order;
using Elements = PermutationGroup::Elements;
using Generators = PermutationGroup::Generators;

class PermutationGroupTest : public ::testing::Test
{
};

TEST_F(PermutationGroupTest, ElementsConstructor)
{
    const auto myGroup = PermutationGroup{
        "S_2", Elements{std::vector{
                   Permutation{Degree{2}}, Permutation{std::vector{Element{1}, Element{0}}}}}};
    EXPECT_THAT(myGroup.order(), Eq(Order{2}));
    EXPECT_THAT(myGroup.degree(), Eq(Degree{2}));
    EXPECT_THAT(myGroup.name(), Eq("S_2"));
}

TEST_F(PermutationGroupTest, NoElementsThrows)
{
    EXPECT_THROW(
        PermutationGroup("empty", Elements{std::vector<Permutation>{}}), std::runtime_error
    );
}

TEST_F(PermutationGroupTest, GeneratorsConstructor)
{
    const auto myGroup = PermutationGroup{
        "S_3", Degree{3},
        Generators{std::vector{
            Permutation{std::vector{Element{1}, Element{0}, Element{2}}},
            Permutation{std::vector{Element{1}, Element{2}, Element{0}}}}}};
    EXPECT_THAT(myGroup.order(), Eq(Order{6}));
}

TEST_F(PermutationGroupTest, InvalidDegreeInGeneratorThrows)
{
    EXPECT_THROW(
        PermutationGroup("bad", Degree{3}, Generators{std::vector{Permutation{Degree{4}}}}),
        std::runtime_error
    );
}

TEST_F(PermutationGroupTest, Name)
{
    const auto myGroup = groups::cyclic(Degree{3});
    EXPECT_THAT(myGroup.name(), Eq("C_3"));
}

TEST_F(PermutationGroupTest, ToString)
{
    const auto myGroup = groups::cyclic(Degree{3});
    EXPECT_THAT(
        myGroup.toString(), Eq("C_3(order: 3, degree: 3) {\n\t(0)(1)(2)\n\t(0 1 2)\n\t(0 2 1)\n}")
    );
}

TEST_F(PermutationGroupTest, OutputStream)
{
    std::ostringstream myStream;
    myStream << groups::trivial(Degree{2});
    EXPECT_THAT(myStream.str(), Eq("Trivial_2(order: 1, degree: 2) {\n\t(0)(1)\n}"));
}

TEST_F(PermutationGroupTest, Cyclic)
{
    EXPECT_THAT(groups::cyclic(Degree{1}).order(), Eq(Order{1}));
    EXPECT_THAT(groups::cyclic(Degree{4}).order(), Eq(Order{4}));
    EXPECT_THAT(groups::cyclic(Degree{6}).order(), Eq(Order{6}));
}

TEST_F(PermutationGroupTest, Dihedral)
{
    EXPECT_THAT(groups::dihedral(Degree{3}).order(), Eq(Order{6}));
    EXPECT_THAT(groups::dihedral(Degree{4}).order(), Eq(Order{8}));
    EXPECT_THAT(groups::dihedral(Degree{6}).order(), Eq(Order{12}));
}

TEST_F(PermutationGroupTest, Symmetric)
{
    EXPECT_THAT(groups::symmetric(Degree{1}).order(), Eq(Order{1}));
    EXPECT_THAT(groups::symmetric(Degree{2}).order(), Eq(Order{2}));
    EXPECT_THAT(groups::symmetric(Degree{3}).order(), Eq(Order{6}));
    EXPECT_THAT(groups::symmetric(Degree{4}).order(), Eq(Order{24}));
}

TEST_F(PermutationGroupTest, Trivial)
{
    const auto myGroup = groups::trivial(Degree{5});
    EXPECT_THAT(myGroup.order(), Eq(Order{1}));
    EXPECT_THAT(myGroup.degree(), Eq(Degree{5}));
}

TEST_F(PermutationGroupTest, Tetrahedron)
{
    const auto myGroup = groups::tetrahedron();
    EXPECT_THAT(myGroup.order(), Eq(Order{12}));
    EXPECT_THAT(myGroup.degree(), Eq(Degree{4}));
    EXPECT_THAT(myGroup.name(), Eq("Tetrahedron"));
}

TEST_F(PermutationGroupTest, Cube)
{
    const auto myGroup = groups::cube();
    EXPECT_THAT(myGroup.order(), Eq(Order{24}));
    EXPECT_THAT(myGroup.degree(), Eq(Degree{6}));
    EXPECT_THAT(myGroup.name(), Eq("Cube"));
}

TEST_F(PermutationGroupTest, ContainsPermutation)
{
    const auto myGroup = groups::cyclic(Degree{3});
    EXPECT_THAT(myGroup.contains(Permutation{Degree{3}}), IsTrue());
    EXPECT_THAT(
        myGroup.contains(Permutation{std::vector{Element{1}, Element{2}, Element{0}}}), IsTrue()
    );
}

TEST_F(PermutationGroupTest, DoesNotContainPermutation)
{
    const auto myGroup = groups::cyclic(Degree{3});
    EXPECT_THAT(
        myGroup.contains(Permutation{std::vector{Element{1}, Element{0}, Element{2}}}), IsFalse()
    );
}

TEST_F(PermutationGroupTest, ValidGroups)
{
    EXPECT_THAT(groups::cyclic(Degree{4}).isValidGroup(), IsTrue());
    EXPECT_THAT(groups::dihedral(Degree{4}).isValidGroup(), IsTrue());
    EXPECT_THAT(groups::symmetric(Degree{4}).isValidGroup(), IsTrue());
    EXPECT_THAT(groups::tetrahedron().isValidGroup(), IsTrue());
    EXPECT_THAT(groups::cube().isValidGroup(), IsTrue());
}

TEST_F(PermutationGroupTest, InvalidGroup)
{
    const auto myNotAGroup = PermutationGroup{
        "bad", Elements{std::vector{
                   Permutation{std::vector{Element{1}, Element{2}, Element{0}}},
                   Permutation{std::vector{Element{0}, Element{2}, Element{1}}}}}};
    EXPECT_THAT(myNotAGroup.isValidGroup(), IsFalse());
}

} // namespace polya::test
