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

class PermutationTest : public ::testing::Test
{
};

// Construction

TEST_F(PermutationTest, DefaultConstructor)
{
    const auto myPermutation = Permutation{};
    EXPECT_THAT(myPermutation.degree(), Eq(Degree{0}));
}

TEST_F(PermutationTest, IdentityConstructor)
{
    const auto myPermutation = Permutation{Degree{4}};
    EXPECT_THAT(myPermutation.degree(), Eq(Degree{4}));
    EXPECT_THAT(myPermutation.isIdentity(), IsTrue());
}

TEST_F(PermutationTest, BijectionConstructor)
{
    const auto myPermutation = Permutation{std::vector{Element{1}, Element{0}, Element{2}}};
    EXPECT_THAT(myPermutation(Element{0}), Eq(Element{1}));
    EXPECT_THAT(myPermutation(Element{1}), Eq(Element{0}));
    EXPECT_THAT(myPermutation(Element{2}), Eq(Element{2}));
}

TEST_F(PermutationTest, CycleConstructor)
{
    const auto myPermutation =
        Permutation{Degree{4}, {Cycle{{Element{0}, Element{1}, Element{2}}}}};
    EXPECT_THAT(myPermutation(Element{0}), Eq(Element{1}));
    EXPECT_THAT(myPermutation(Element{1}), Eq(Element{2}));
    EXPECT_THAT(myPermutation(Element{2}), Eq(Element{0}));
    EXPECT_THAT(myPermutation(Element{3}), Eq(Element{3}));
}

TEST_F(PermutationTest, CycleConstructorWithMultipleCycles)
{
    const auto myPermutation =
        Permutation{Degree{4}, {Cycle{{Element{0}, Element{1}}}, Cycle{{Element{2}, Element{3}}}}};
    EXPECT_THAT(myPermutation(Element{0}), Eq(Element{1}));
    EXPECT_THAT(myPermutation(Element{1}), Eq(Element{0}));
    EXPECT_THAT(myPermutation(Element{2}), Eq(Element{3}));
    EXPECT_THAT(myPermutation(Element{3}), Eq(Element{2}));
}

TEST_F(PermutationTest, NotInDomainThrows)
{
    const auto myPermutation = Permutation{Degree{3}};
    EXPECT_THROW(
        [[maybe_unused]] const auto myValue = myPermutation(Element{3}), std::runtime_error
    );
}

TEST_F(PermutationTest, NotInDomainCycleElementThrows)
{
    EXPECT_THROW(Permutation(Degree{3}, {Cycle{{Element{0}, Element{5}}}}), std::runtime_error);
}

TEST_F(PermutationTest, CompositionWithIdentity)
{
    const auto myPermutation = Permutation{std::vector{Element{1}, Element{2}, Element{0}}};
    const auto myIdentity = Permutation{Degree{3}};
    EXPECT_THAT(myPermutation * myIdentity, Eq(myPermutation));
    EXPECT_THAT(myIdentity * myPermutation, Eq(myPermutation));
}

TEST_F(PermutationTest, Composition)
{
    const auto myOuterPermutation = Permutation{std::vector{Element{1}, Element{0}, Element{2}}};
    const auto myInnerPermutation = Permutation{std::vector{Element{0}, Element{2}, Element{1}}};
    const auto myComposed = myOuterPermutation * myInnerPermutation;
    EXPECT_THAT(myComposed(Element{0}), Eq(Element{1}));
    EXPECT_THAT(myComposed(Element{1}), Eq(Element{2}));
    EXPECT_THAT(myComposed(Element{2}), Eq(Element{0}));
}

TEST_F(PermutationTest, InverseOfIdentityIsIdentity)
{
    const auto myIdentity = Permutation{Degree{3}};
    EXPECT_THAT(myIdentity.inverse(), Eq(myIdentity));
}

TEST_F(PermutationTest, Inverse)
{
    const auto myPermutation = Permutation{std::vector{Element{1}, Element{2}, Element{0}}};
    const auto myIdentity = Permutation{Degree{3}};
    EXPECT_THAT(myPermutation * myPermutation.inverse(), Eq(myIdentity));
    EXPECT_THAT(myPermutation.inverse() * myPermutation, Eq(myIdentity));
}

TEST_F(PermutationTest, PowerZeroIsIdentity)
{
    const auto myPermutation = Permutation{std::vector{Element{1}, Element{2}, Element{0}}};
    EXPECT_THAT(myPermutation.power(0), Eq(Permutation{Degree{3}}));
}

TEST_F(PermutationTest, PowerOneIsSelf)
{
    const auto myPermutation = Permutation{std::vector{Element{1}, Element{2}, Element{0}}};
    EXPECT_THAT(myPermutation.power(1), Eq(myPermutation));
}

TEST_F(PermutationTest, Power)
{
    const auto myPermutation = Permutation{std::vector{Element{1}, Element{2}, Element{0}}};
    EXPECT_THAT(myPermutation.power(2), Eq(myPermutation * myPermutation));
}

TEST_F(PermutationTest, PowerNegative)
{
    const auto myPermutation = Permutation{std::vector{Element{1}, Element{2}, Element{0}}};
    EXPECT_THAT(myPermutation.power(-1), Eq(myPermutation.inverse()));
    EXPECT_THAT(myPermutation.power(-2), Eq(myPermutation.inverse().power(2)));
}

TEST_F(PermutationTest, IsIdentity)
{
    EXPECT_THAT(Permutation{}.isIdentity(), IsTrue());
    EXPECT_THAT(Permutation{Degree{4}}.isIdentity(), IsTrue());
}

TEST_F(PermutationTest, IsNotIdentity)
{
    EXPECT_THAT(Permutation(std::vector{Element{1}, Element{0}}).isIdentity(), IsFalse());
}

TEST_F(PermutationTest, IdentityCycles)
{
    const auto myCycles = Permutation{Degree{3}}.asCycles();
    EXPECT_THAT(myCycles.size(), Eq(3));
}

TEST_F(PermutationTest, SingleCycle)
{
    const auto myCycles = Permutation(std::vector{Element{1}, Element{2}, Element{0}}).asCycles();
    EXPECT_THAT(myCycles.size(), Eq(1));
    EXPECT_THAT(myCycles[0].get().size(), Eq(3));
}

TEST_F(PermutationTest, MultipleCycles)
{
    const auto myPermutation =
        Permutation{std::vector{Element{1}, Element{0}, Element{3}, Element{2}, Element{4}}};
    const auto myCycles = myPermutation.asCycles();
    EXPECT_THAT(myCycles.size(), Eq(3));
}

TEST_F(PermutationTest, CycleStructureIdentity)
{
    const auto myStructure = Permutation{Degree{4}}.cycleStructure();
    EXPECT_THAT(myStructure.totalCycleCount(), Eq(Permutation::CycleStructure::Count{4}));
    EXPECT_THAT(myStructure.degree(), Eq(Degree{4}));
}

TEST_F(PermutationTest, CycleStructureThreeCycle)
{
    const auto myStructure =
        Permutation(std::vector{Element{1}, Element{2}, Element{0}, Element{3}}).cycleStructure();
    EXPECT_THAT(myStructure.totalCycleCount(), Eq(Permutation::CycleStructure::Count{2}));
    EXPECT_THAT(myStructure.degree(), Eq(Degree{4}));
    // 1 fixed point (length 1), 1 cycle of length 3
    EXPECT_THAT(myStructure.theCycleLengthDistribution[1].get(), Eq(1));
    EXPECT_THAT(myStructure.theCycleLengthDistribution[3].get(), Eq(1));
}

TEST_F(PermutationTest, EqualPermutations)
{
    const auto myA = Permutation{std::vector{Element{1}, Element{0}}};
    const auto myB = Permutation{std::vector{Element{1}, Element{0}}};
    EXPECT_THAT(myA, Eq(myB));
}

TEST_F(PermutationTest, UnequalPermutations)
{
    const auto myA = Permutation{std::vector{Element{1}, Element{0}}};
    const auto myB = Permutation{Degree{2}};
    EXPECT_THAT(myA, Ne(myB));
}

TEST_F(PermutationTest, LexicographicOrdering)
{
    const auto myA = Permutation{Degree{2}};
    const auto myB = Permutation{std::vector{Element{1}, Element{0}}};
    EXPECT_THAT(myA, Lt(myB));
}

TEST_F(PermutationTest, ToStringIdentity)
{
    EXPECT_THAT(Permutation{Degree{3}}.toString(), Eq("(0)(1)(2)"));
}

TEST_F(PermutationTest, ToStringSingleCycle)
{
    EXPECT_THAT(
        Permutation(std::vector{Element{1}, Element{2}, Element{0}}).toString(), Eq("(0 1 2)")
    );
}

TEST_F(PermutationTest, ToStringMultipleCycles)
{
    EXPECT_THAT(
        Permutation(std::vector{Element{1}, Element{0}, Element{3}, Element{2}}).toString(),
        Eq("(0 1)(2 3)")
    );
}

TEST_F(PermutationTest, OutputStream)
{
    std::ostringstream myStream;
    myStream << Permutation(std::vector{Element{1}, Element{2}, Element{0}});
    EXPECT_THAT(myStream.str(), Eq("(0 1 2)"));
}

TEST_F(PermutationTest, Rotation)
{
    const auto myRotation = permutations::rotation(Degree{4});
    EXPECT_THAT(myRotation(Element{0}), Eq(Element{1}));
    EXPECT_THAT(myRotation(Element{1}), Eq(Element{2}));
    EXPECT_THAT(myRotation(Element{2}), Eq(Element{3}));
    EXPECT_THAT(myRotation(Element{3}), Eq(Element{0}));
    EXPECT_THAT(myRotation.power(4).isIdentity(), IsTrue());
}

TEST_F(PermutationTest, Reflection)
{
    const auto myReflection = permutations::reflection(Degree{4});
    EXPECT_THAT(myReflection(Element{0}), Eq(Element{0}));
    EXPECT_THAT(myReflection(Element{1}), Eq(Element{3}));
    EXPECT_THAT(myReflection(Element{3}), Eq(Element{1}));
    EXPECT_THAT(myReflection(Element{2}), Eq(Element{2}));
    EXPECT_THAT(myReflection.power(2).isIdentity(), IsTrue());
}

TEST_F(PermutationTest, Transposition)
{
    const auto myTransposition = permutations::transposition(Degree{4}, Element{1}, Element{3});
    EXPECT_THAT(myTransposition(Element{0}), Eq(Element{0}));
    EXPECT_THAT(myTransposition(Element{2}), Eq(Element{2}));
    EXPECT_THAT(myTransposition(Element{1}), Eq(Element{3}));
    EXPECT_THAT(myTransposition(Element{3}), Eq(Element{1}));
    EXPECT_THAT(myTransposition.power(2).isIdentity(), IsTrue());
}

} // namespace polya::test
