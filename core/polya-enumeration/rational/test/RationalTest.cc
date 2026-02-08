#include "core/polya-enumeration/rational/Rational.hh"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <sstream>

namespace polya::test
{
using namespace ::testing;
using Numerator = Rational::Numerator;
using Denominator = Rational::Denominator;

class RationalTest : public ::testing::Test
{
};

TEST_F(RationalTest, ReduceSimplifies)
{
    auto myRational = Rational{Numerator{4}, Denominator{6}};
    myRational.reduce();
    EXPECT_THAT(myRational, Eq(Rational(Numerator{2}, Denominator{3})));
}

TEST_F(RationalTest, ReduceNormalizesDenominatorSign)
{
    auto myRational = Rational{Numerator{1}, Denominator{-2}};
    myRational.reduce();
    EXPECT_THAT(myRational, Eq(Rational(Numerator{-1}, Denominator{2})));
}

TEST_F(RationalTest, ReduceZeroNumerator)
{
    auto myRational = Rational{Numerator{0}, Denominator{5}};
    myRational.reduce();
    EXPECT_THAT(myRational, Eq(Rational{0}));
}

TEST_F(RationalTest, Addition)
{
    EXPECT_THAT(
        Rational(Numerator{1}, Denominator{2}) + Rational(Numerator{1}, Denominator{3}),
        Eq(Rational(Numerator{5}, Denominator{6}))
    );
    EXPECT_THAT(
        Rational(Numerator{1}, Denominator{2}) + Rational(Numerator{-1}, Denominator{2}),
        Eq(Rational{0})
    );
    EXPECT_THAT(
        Rational(Numerator{1}, Denominator{2}) + Rational{0},
        Eq(Rational(Numerator{1}, Denominator{2}))
    );
}

TEST_F(RationalTest, Subtraction)
{
    EXPECT_THAT(
        Rational(Numerator{1}, Denominator{2}) - Rational(Numerator{1}, Denominator{3}),
        Eq(Rational(Numerator{1}, Denominator{6}))
    );
    EXPECT_THAT(
        Rational(Numerator{1}, Denominator{3}) - Rational(Numerator{1}, Denominator{2}),
        Eq(Rational(Numerator{-1}, Denominator{6}))
    );
    EXPECT_THAT(
        Rational(Numerator{1}, Denominator{2}) - Rational(Numerator{1}, Denominator{2}),
        Eq(Rational{0})
    );
}

TEST_F(RationalTest, Multiplication)
{
    EXPECT_THAT(
        Rational(Numerator{1}, Denominator{2}) * Rational(Numerator{2}, Denominator{3}),
        Eq(Rational(Numerator{1}, Denominator{3}))
    );
    EXPECT_THAT(Rational(Numerator{1}, Denominator{2}) * Rational{0}, Eq(Rational{0}));
    EXPECT_THAT(
        Rational(Numerator{1}, Denominator{2}) * Rational{-1},
        Eq(Rational(Numerator{-1}, Denominator{2}))
    );
}

TEST_F(RationalTest, Division)
{
    EXPECT_THAT(
        Rational(Numerator{1}, Denominator{2}) / Rational(Numerator{2}, Denominator{3}),
        Eq(Rational(Numerator{3}, Denominator{4}))
    );
    EXPECT_THAT(
        Rational(Numerator{1}, Denominator{2}) / Rational{-1},
        Eq(Rational(Numerator{-1}, Denominator{2}))
    );
}

TEST_F(RationalTest, DivisionByZeroThrows)
{
    EXPECT_THROW(
        [[maybe_unused]] const auto myResult = Rational(Numerator{1}, Denominator{2}) / Rational{0},
        std::runtime_error
    );
}

TEST_F(RationalTest, CompoundAssignment)
{
    auto myRational = Rational{Numerator{1}, Denominator{2}};
    myRational += Rational{Numerator{1}, Denominator{3}};
    EXPECT_THAT(myRational, Eq(Rational(Numerator{5}, Denominator{6})));

    myRational -= Rational{Numerator{1}, Denominator{6}};
    EXPECT_THAT(myRational, Eq(Rational(Numerator{2}, Denominator{3})));

    myRational *= Rational{Numerator{3}, Denominator{4}};
    EXPECT_THAT(myRational, Eq(Rational(Numerator{1}, Denominator{2})));

    myRational /= Rational{Numerator{1}, Denominator{2}};
    EXPECT_THAT(myRational, Eq(Rational{1}));
}

TEST_F(RationalTest, Comparison)
{
    EXPECT_THAT(Rational(Numerator{1}, Denominator{2}), Eq(Rational(Numerator{2}, Denominator{4})));
    EXPECT_THAT(Rational(Numerator{1}, Denominator{2}), Ne(Rational(Numerator{1}, Denominator{3})));
    EXPECT_THAT(Rational(Numerator{1}, Denominator{3}), Lt(Rational(Numerator{1}, Denominator{2})));
    EXPECT_THAT(Rational(Numerator{-1}, Denominator{2}), Lt(Rational{0}));
    EXPECT_THAT(Rational(Numerator{1}, Denominator{2}), Gt(Rational(Numerator{1}, Denominator{3})));
}

TEST_F(RationalTest, IsIntegral)
{
    EXPECT_THAT(Rational{1}.isIntegral(), IsTrue());
    EXPECT_THAT(Rational{0}.isIntegral(), IsTrue());
    EXPECT_THAT(Rational(Numerator{1}, Denominator{2}).isIntegral(), IsFalse());
}

TEST_F(RationalTest, AsInteger)
{
    EXPECT_THAT(Rational{5}.asInteger(), Eq(5));
    EXPECT_THAT(Rational{-3}.asInteger(), Eq(-3));
}

TEST_F(RationalTest, AsIntegerThrowsForNonIntegral)
{
    EXPECT_THROW(
        [[maybe_unused]] const auto myInteger = Rational(Numerator{1}, Denominator{2}).asInteger(),
        std::runtime_error
    );
}

TEST_F(RationalTest, ToString)
{
    EXPECT_THAT(Rational(Numerator{-1}, Denominator{2}).toString(), Eq("(-1/2)"));
    EXPECT_THAT(Rational{3}.toString(), Eq("(3/1)"));
}

TEST_F(RationalTest, OutputStream)
{
    std::ostringstream myStream;
    myStream << Rational{Numerator{-1}, Denominator{2}};
    EXPECT_THAT(myStream.str(), Eq("(-1/2)"));
}
} // namespace polya::test
