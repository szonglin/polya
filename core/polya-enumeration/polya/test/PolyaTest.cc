#include "core/polya-enumeration/polya/Polya.hh"
#include "core/polya-enumeration/group/PermutationGroup.hh"
#include "core/polya-enumeration/orbit-counting/OrbitCounting.hh"
#include "core/polya-enumeration/polynomial/Polynomial.hh"
#include "core/polya-enumeration/rational/Rational.hh"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace polya::test
{
using namespace ::testing;
using VariableName = Polynomial::VariableName;
using Exponent = Polynomial::Exponent;
using Term = Polynomial::Term;
using OrbitCount = orbits::OrbitCount;
using ColourCount = orbits::ColourCount;

class PolyaTest : public ::testing::Test
{
};

TEST_F(PolyaTest, CycleIndexTrivialGroup)
{
    const auto myGroup = groups::trivial(Permutation::Degree{3});
    const auto myZ = cycleIndexPolynomial(myGroup);

    const auto myTerm = Term{std::vector{Exponent{3}, Exponent{0}, Exponent{0}}};
    EXPECT_THAT(myZ.get().coefficient(myTerm), Eq(Rational{1}));
}

TEST_F(PolyaTest, CycleIndexCyclic4)
{
    const auto myGroup = groups::cyclic(Permutation::Degree{4});
    const auto myZ = cycleIndexPolynomial(myGroup);

    EXPECT_THAT(
        myZ.get().coefficient(Term{std::vector{Exponent{4}, Exponent{0}, Exponent{0}, Exponent{0}}}
        ),
        Eq(Rational{Rational::Numerator{1}, Rational::Denominator{4}})
    );
    EXPECT_THAT(
        myZ.get().coefficient(Term{std::vector{Exponent{0}, Exponent{2}, Exponent{0}, Exponent{0}}}
        ),
        Eq(Rational{Rational::Numerator{1}, Rational::Denominator{4}})
    );
    EXPECT_THAT(
        myZ.get().coefficient(Term{std::vector{Exponent{0}, Exponent{0}, Exponent{0}, Exponent{1}}}
        ),
        Eq(Rational{Rational::Numerator{1}, Rational::Denominator{2}})
    );
}

TEST_F(PolyaTest, CycleIndexCustomVariables)
{
    const auto myGroup = groups::cyclic(Permutation::Degree{2});
    const auto myVariables = std::vector{VariableName{"a"}, VariableName{"b"}};
    const auto myZ = cycleIndexPolynomial(myGroup, myVariables);

    EXPECT_THAT(myZ.get().toString(), Eq("+(1/2)b^1 +(1/2)a^2"));
}

TEST_F(PolyaTest, EvaluateUniformTrivialGroup)
{
    const auto myGroup = groups::trivial(Permutation::Degree{3});
    const auto myZ = cycleIndexPolynomial(myGroup);

    for (const auto myNumberOfColours : {1uz, 2uz, 3uz, 5uz, 10uz})
    {
        const auto myPolyaResult = evaluateUniform(myZ, ColourCount{myNumberOfColours});
        const auto myBurnsideResult = orbits::countOrbits(myGroup, ColourCount{myNumberOfColours});
        EXPECT_THAT(myPolyaResult, Eq(myBurnsideResult));
    }
}

TEST_F(PolyaTest, EvaluateUniformCyclic4)
{
    const auto myGroup = groups::cyclic(Permutation::Degree{4});
    const auto myZ = cycleIndexPolynomial(myGroup);

    // Test with various color counts
    for (const auto myNumberOfColours : {1uz, 2uz, 3uz, 5uz, 10uz})
    {
        const auto myPolyaResult = evaluateUniform(myZ, ColourCount{myNumberOfColours});
        const auto myBurnsideResult = orbits::countOrbits(myGroup, ColourCount{myNumberOfColours});
        EXPECT_THAT(myPolyaResult, Eq(myBurnsideResult));
    }
}

TEST_F(PolyaTest, EvaluateUniformDihedral4)
{
    const auto myGroup = groups::dihedral(Permutation::Degree{4});
    const auto myZ = cycleIndexPolynomial(myGroup);

    for (const auto myNumberOfColours : {1uz, 2uz, 3uz, 5uz, 10uz})
    {
        const auto myPolyaResult = evaluateUniform(myZ, ColourCount{myNumberOfColours});
        const auto myBurnsideResult = orbits::countOrbits(myGroup, ColourCount{myNumberOfColours});
        EXPECT_THAT(myPolyaResult, Eq(myBurnsideResult));
    }
}

TEST_F(PolyaTest, EvaluateUniformTetrahedron)
{
    const auto myGroup = groups::tetrahedron();
    const auto myZ = cycleIndexPolynomial(myGroup);

    for (const auto myNumberOfColours : {1uz, 2uz, 3uz, 5uz, 10uz})
    {
        const auto myPolyaResult = evaluateUniform(myZ, ColourCount{myNumberOfColours});
        const auto myBurnsideResult = orbits::countOrbits(myGroup, ColourCount{myNumberOfColours});
        EXPECT_THAT(myPolyaResult, Eq(myBurnsideResult));
    }
}

TEST_F(PolyaTest, EvaluateUniformCube)
{
    const auto myGroup = groups::cube();
    const auto myZ = cycleIndexPolynomial(myGroup);

    for (const auto myNumberOfColours : {1uz, 2uz, 3uz, 5uz, 50uz})
    {
        const auto myPolyaResult = evaluateUniform(myZ, ColourCount{myNumberOfColours});
        const auto myBurnsideResult = orbits::countOrbits(myGroup, ColourCount{myNumberOfColours});
        EXPECT_THAT(myPolyaResult, Eq(myBurnsideResult));
    }
}

TEST_F(PolyaTest, EvaluateUniformSymmetric4)
{
    const auto myGroup = groups::symmetric(Permutation::Degree{4});
    const auto myZ = cycleIndexPolynomial(myGroup);

    for (const auto myNumberOfColours : {1uz, 2uz, 3uz, 5uz, 10uz})
    {
        const auto myPolyaResult = evaluateUniform(myZ, ColourCount{myNumberOfColours});
        const auto myBurnsideResult = orbits::countOrbits(myGroup, ColourCount{myNumberOfColours});
        EXPECT_THAT(myPolyaResult, Eq(myBurnsideResult));
    }
}

TEST_F(PolyaTest, EvaluateColoursCyclic2With2Colours)
{
    const auto myGroup = groups::cyclic(Permutation::Degree{2});
    const auto myZ = cycleIndexPolynomial(myGroup);
    const auto myResult = evaluateColours(myZ, ColourCount{2});

    EXPECT_THAT(myResult.coefficient(Term{std::vector{Exponent{2}, Exponent{0}}}), Eq(Rational{1}));
    EXPECT_THAT(myResult.coefficient(Term{std::vector{Exponent{1}, Exponent{1}}}), Eq(Rational{1}));
    EXPECT_THAT(myResult.coefficient(Term{std::vector{Exponent{0}, Exponent{2}}}), Eq(Rational{1}));
}

TEST_F(PolyaTest, EvaluateColoursCyclic4With2Colours)
{
    const auto myGroup = groups::cyclic(Permutation::Degree{4});
    const auto myZ = cycleIndexPolynomial(myGroup);
    const auto myResult = evaluateColours(myZ, ColourCount{2});

    EXPECT_THAT(myResult.coefficient(Term{std::vector{Exponent{4}, Exponent{0}}}), Eq(Rational{1}));
    EXPECT_THAT(myResult.coefficient(Term{std::vector{Exponent{3}, Exponent{1}}}), Eq(Rational{1}));
    EXPECT_THAT(myResult.coefficient(Term{std::vector{Exponent{2}, Exponent{2}}}), Eq(Rational{2}));
    EXPECT_THAT(myResult.coefficient(Term{std::vector{Exponent{1}, Exponent{3}}}), Eq(Rational{1}));
    EXPECT_THAT(myResult.coefficient(Term{std::vector{Exponent{0}, Exponent{4}}}), Eq(Rational{1}));
}

TEST_F(PolyaTest, EvaluateColoursTetrahedronWith3Colours)
{
    const auto myGroup = groups::tetrahedron();
    const auto myZ = cycleIndexPolynomial(myGroup);
    const auto myResult = evaluateColours(
        myZ, ColourCount{3}, std::vector{VariableName{"r"}, VariableName{"g"}, VariableName{"b"}}
    );
    EXPECT_THAT(myResult.terms().size(), Eq(15uz));
    EXPECT_THAT(
        myResult.coefficient(Term{std::vector{Exponent{4}, Exponent{0}, Exponent{0}}}),
        Eq(Rational{1})
    );
    EXPECT_THAT(
        myResult.coefficient(Term{std::vector{Exponent{0}, Exponent{4}, Exponent{0}}}),
        Eq(Rational{1})
    );
    EXPECT_THAT(
        myResult.coefficient(Term{std::vector{Exponent{0}, Exponent{0}, Exponent{4}}}),
        Eq(Rational{1})
    );
    EXPECT_THAT(
        myResult.coefficient(Term{std::vector{Exponent{3}, Exponent{1}, Exponent{0}}}),
        Eq(Rational{1})
    );
    EXPECT_THAT(
        myResult.coefficient(Term{std::vector{Exponent{3}, Exponent{0}, Exponent{1}}}),
        Eq(Rational{1})
    );
    EXPECT_THAT(
        myResult.coefficient(Term{std::vector{Exponent{0}, Exponent{3}, Exponent{1}}}),
        Eq(Rational{1})
    );
    EXPECT_THAT(
        myResult.coefficient(Term{std::vector{Exponent{1}, Exponent{3}, Exponent{0}}}),
        Eq(Rational{1})
    );
    EXPECT_THAT(
        myResult.coefficient(Term{std::vector{Exponent{0}, Exponent{1}, Exponent{3}}}),
        Eq(Rational{1})
    );
    EXPECT_THAT(
        myResult.coefficient(Term{std::vector{Exponent{1}, Exponent{0}, Exponent{3}}}),
        Eq(Rational{1})
    );
    EXPECT_THAT(
        myResult.coefficient(Term{std::vector{Exponent{2}, Exponent{2}, Exponent{0}}}),
        Eq(Rational{1})
    );
    EXPECT_THAT(
        myResult.coefficient(Term{std::vector{Exponent{2}, Exponent{0}, Exponent{2}}}),
        Eq(Rational{1})
    );
    EXPECT_THAT(
        myResult.coefficient(Term{std::vector{Exponent{0}, Exponent{2}, Exponent{2}}}),
        Eq(Rational{1})
    );
    EXPECT_THAT(
        myResult.coefficient(Term{std::vector{Exponent{2}, Exponent{1}, Exponent{1}}}),
        Eq(Rational{1})
    );
    EXPECT_THAT(
        myResult.coefficient(Term{std::vector{Exponent{1}, Exponent{2}, Exponent{1}}}),
        Eq(Rational{1})
    );
    EXPECT_THAT(
        myResult.coefficient(Term{std::vector{Exponent{1}, Exponent{1}, Exponent{2}}}),
        Eq(Rational{1})
    );
}

} // namespace polya::test
