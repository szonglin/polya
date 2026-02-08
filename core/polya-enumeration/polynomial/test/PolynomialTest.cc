#include "core/polya-enumeration/polynomial/Polynomial.hh"
#include "core/polya-enumeration/rational/Rational.hh"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <sstream>

namespace polya::test
{
using namespace ::testing;
using VariableName = Polynomial::VariableName;
using Exponent = Polynomial::Exponent;
using Term = Polynomial::Term;

class PolynomialTest : public ::testing::Test
{
};

TEST_F(PolynomialTest, Constructor)
{
    const auto myPoly = Polynomial{std::vector{VariableName{"x"}, VariableName{"y"}}};
    EXPECT_THAT(myPoly.isZero(), IsTrue());
    EXPECT_THAT(myPoly.variables(), Eq(std::vector{VariableName{"x"}, VariableName{"y"}}));
}

TEST_F(PolynomialTest, SetAndGetCoefficient)
{
    auto myPoly = Polynomial{std::vector{VariableName{"x"}, VariableName{"y"}}};
    const auto myTerm = Term{std::vector{Exponent{2}, Exponent{1}}};
    myPoly.set(myTerm, Rational{3});
    EXPECT_THAT(myPoly.coefficient(myTerm), Eq(Rational{3}));
    EXPECT_THAT(myPoly.isZero(), IsFalse());
}

TEST_F(PolynomialTest, ZeroCoefficientsAreNotStored)
{
    auto myPoly = Polynomial{std::vector{VariableName{"x"}}};
    const auto myTerm = Term{std::vector{Exponent{1}}};
    myPoly.set(myTerm, Rational{0});
    EXPECT_THAT(myPoly.isZero(), IsTrue());
    EXPECT_THAT(myPoly.terms().size(), Eq(0UZ));
}

TEST_F(PolynomialTest, UnsetCoefficient)
{
    const auto myPoly = Polynomial{std::vector{VariableName{"x"}}};
    const auto myTerm = Term{std::vector{Exponent{1}}};
    EXPECT_THAT(myPoly.coefficient(myTerm), Eq(Rational{0}));
}

TEST_F(PolynomialTest, AdditionSameVariables)
{
    auto myPoly1 = Polynomial{std::vector{VariableName{"x"}, VariableName{"y"}}};
    myPoly1.set(Term{std::vector{Exponent{1}, Exponent{0}}}, Rational{2});
    myPoly1.set(Term{std::vector{Exponent{0}, Exponent{1}}}, Rational{3});

    auto myPoly2 = Polynomial{std::vector{VariableName{"x"}, VariableName{"y"}}};
    myPoly2.set(Term{std::vector{Exponent{1}, Exponent{0}}}, Rational{1});
    myPoly2.set(Term{std::vector{Exponent{0}, Exponent{0}}}, Rational{5});

    const auto myResult = myPoly1 + myPoly2;
    EXPECT_THAT(myResult.coefficient(Term{std::vector{Exponent{1}, Exponent{0}}}), Eq(Rational{3}));
    EXPECT_THAT(myResult.coefficient(Term{std::vector{Exponent{0}, Exponent{1}}}), Eq(Rational{3}));
    EXPECT_THAT(myResult.coefficient(Term{std::vector{Exponent{0}, Exponent{0}}}), Eq(Rational{5}));
}

TEST_F(PolynomialTest, AdditionInPlace)
{
    auto myPoly1 = Polynomial{std::vector{VariableName{"x"}}};
    myPoly1.set(Term{std::vector{Exponent{1}}}, Rational{2});

    auto myPoly2 = Polynomial{std::vector{VariableName{"x"}}};
    myPoly2.set(Term{std::vector{Exponent{1}}}, Rational{3});

    myPoly1 += myPoly2;
    EXPECT_THAT(myPoly1.coefficient(Term{std::vector{Exponent{1}}}), Eq(Rational{5}));
}

TEST_F(PolynomialTest, Subtraction)
{
    auto myPoly1 = Polynomial{std::vector{VariableName{"x"}}};
    myPoly1.set(Term{std::vector{Exponent{1}}}, Rational{5});

    auto myPoly2 = Polynomial{std::vector{VariableName{"x"}}};
    myPoly2.set(Term{std::vector{Exponent{1}}}, Rational{2});

    const auto myResult = myPoly1 - myPoly2;
    EXPECT_THAT(myResult.coefficient(Term{std::vector{Exponent{1}}}), Eq(Rational{3}));
}

TEST_F(PolynomialTest, SubtractionInPlace)
{
    auto myPoly1 = Polynomial{std::vector{VariableName{"x"}}};
    myPoly1.set(Term{std::vector{Exponent{1}}}, Rational{5});

    auto myPoly2 = Polynomial{std::vector{VariableName{"x"}}};
    myPoly2.set(Term{std::vector{Exponent{1}}}, Rational{2});

    myPoly1 -= myPoly2;
    EXPECT_THAT(myPoly1.coefficient(Term{std::vector{Exponent{1}}}), Eq(Rational{3}));
}

TEST_F(PolynomialTest, MultiplicationSimple)
{
    auto myPoly1 = Polynomial{std::vector{VariableName{"x"}}};
    myPoly1.set(Term{std::vector{Exponent{1}}}, Rational{2});

    auto myPoly2 = Polynomial{std::vector{VariableName{"x"}}};
    myPoly2.set(Term{std::vector{Exponent{1}}}, Rational{3});

    const auto myResult = myPoly1 * myPoly2;
    EXPECT_THAT(myResult.coefficient(Term{std::vector{Exponent{2}}}), Eq(Rational{6}));
    EXPECT_THAT(myResult.coefficient(Term{std::vector{Exponent{1}}}), Eq(Rational{0}));
}

TEST_F(PolynomialTest, MultiplicationDistributive)
{
    auto myPoly1 = Polynomial{std::vector{VariableName{"x"}}};
    myPoly1.set(Term{std::vector{Exponent{1}}}, Rational{1});
    myPoly1.set(Term{std::vector{Exponent{0}}}, Rational{1});

    auto myPoly2 = Polynomial{std::vector{VariableName{"x"}}};
    myPoly2.set(Term{std::vector{Exponent{1}}}, Rational{1});
    myPoly2.set(Term{std::vector{Exponent{0}}}, Rational{2});

    const auto myResult = myPoly1 * myPoly2;
    EXPECT_THAT(myResult.coefficient(Term{std::vector{Exponent{2}}}), Eq(Rational{1}));
    EXPECT_THAT(myResult.coefficient(Term{std::vector{Exponent{1}}}), Eq(Rational{3}));
    EXPECT_THAT(myResult.coefficient(Term{std::vector{Exponent{0}}}), Eq(Rational{2}));
}

TEST_F(PolynomialTest, MultiplicationMultivariate)
{
    auto myPoly1 = Polynomial{std::vector{VariableName{"x"}, VariableName{"y"}}};
    myPoly1.set(Term{std::vector{Exponent{1}, Exponent{0}}}, Rational{2});

    auto myPoly2 = Polynomial{std::vector{VariableName{"x"}, VariableName{"y"}}};
    myPoly2.set(Term{std::vector{Exponent{0}, Exponent{1}}}, Rational{3});

    const auto myResult = myPoly1 * myPoly2;
    EXPECT_THAT(myResult.coefficient(Term{std::vector{Exponent{1}, Exponent{1}}}), Eq(Rational{6}));
}

TEST_F(PolynomialTest, MultiplicationInPlace)
{
    auto myPoly1 = Polynomial{std::vector{VariableName{"x"}}};
    myPoly1.set(Term{std::vector{Exponent{1}}}, Rational{2});

    auto myPoly2 = Polynomial{std::vector{VariableName{"x"}}};
    myPoly2.set(Term{std::vector{Exponent{1}}}, Rational{3});

    myPoly1 *= myPoly2;
    EXPECT_THAT(myPoly1.coefficient(Term{std::vector{Exponent{2}}}), Eq(Rational{6}));
}

TEST_F(PolynomialTest, ToStringZero)
{
    const auto myPoly = Polynomial{std::vector{VariableName{"x"}}};
    EXPECT_THAT(myPoly.toString(), Eq("0"));
}

TEST_F(PolynomialTest, ToStringConstant)
{
    auto myPoly = Polynomial{std::vector{VariableName{"x"}}};
    myPoly.set(Term{std::vector{Exponent{0}}}, Rational{5});
    EXPECT_THAT(myPoly.toString(), Eq("+(5/1)"));
}

TEST_F(PolynomialTest, ToStringSingleVariable)
{
    auto myPoly = Polynomial{std::vector{VariableName{"x"}}};
    myPoly.set(Term{std::vector{Exponent{1}}}, Rational{1});
    EXPECT_THAT(myPoly.toString(), Eq("+(1/1)x^1"));
}

TEST_F(PolynomialTest, ToStringWithCoefficient)
{
    auto myPoly = Polynomial{std::vector{VariableName{"x"}}};
    myPoly.set(Term{std::vector{Exponent{2}}}, Rational{3});
    EXPECT_THAT(myPoly.toString(), Eq("+(3/1)x^2"));
}

TEST_F(PolynomialTest, ToStringNegativeCoefficient)
{
    auto myPoly = Polynomial{std::vector{VariableName{"x"}}};
    myPoly.set(Term{std::vector{Exponent{1}}}, Rational{-1});
    EXPECT_THAT(myPoly.toString(), Eq("+(-1/1)x^1"));
}

TEST_F(PolynomialTest, ToStringMultipleTerms)
{
    auto myPoly = Polynomial{std::vector{VariableName{"x"}}};
    myPoly.set(Term{std::vector{Exponent{2}}}, Rational{1});
    myPoly.set(Term{std::vector{Exponent{1}}}, Rational{2});
    myPoly.set(Term{std::vector{Exponent{0}}}, Rational{3});
    EXPECT_THAT(myPoly.toString(), Eq("+(3/1) +(2/1)x^1 +(1/1)x^2"));
}

TEST_F(PolynomialTest, ToStringMultivariate)
{
    auto myPoly = Polynomial{std::vector{VariableName{"x"}, VariableName{"y"}}};
    myPoly.set(Term{std::vector{Exponent{2}, Exponent{0}}}, Rational{1});
    myPoly.set(Term{std::vector{Exponent{1}, Exponent{1}}}, Rational{-2});
    myPoly.set(Term{std::vector{Exponent{0}, Exponent{0}}}, Rational{3});
    EXPECT_THAT(myPoly.toString(), Eq("+(3/1) +(-2/1)x^1*y^1 +(1/1)x^2"));
}

TEST_F(PolynomialTest, OutputStream)
{
    auto myPoly = Polynomial{std::vector{VariableName{"x"}}};
    myPoly.set(Term{std::vector{Exponent{1}}}, Rational{2});
    std::ostringstream myStream;
    myStream << myPoly;
    EXPECT_THAT(myStream.str(), Eq("+(2/1)x^1"));
}

TEST_F(PolynomialTest, MultiplyByRational)
{
    auto myPoly = Polynomial{std::vector{VariableName{"x"}}};
    myPoly.set(
        Term{std::vector{Exponent{1}}}, Rational{Rational::Numerator{3}, Rational::Denominator{4}}
    );
    myPoly.set(
        Term{std::vector{Exponent{0}}}, Rational{Rational::Numerator{-1}, Rational::Denominator{8}}
    );
    const auto myResult = Rational{16} * myPoly;
    EXPECT_THAT(myResult.toString(), Eq("+(-2/1) +(12/1)x^1"));
    EXPECT_THAT(myResult, Eq(myPoly * Rational{16}));
}

TEST_F(PolynomialTest, InvalidTermThrows)
{
    auto myPoly = Polynomial{std::vector{VariableName{"x"}, VariableName{"y"}}};
    EXPECT_THROW(myPoly.set(Term{std::vector{Exponent{1}}}, Rational{1}), std::runtime_error);
}

TEST_F(PolynomialTest, OperationsWithDifferentVariablesThrow)
{
    auto myPoly1 = Polynomial{std::vector{VariableName{"x"}}};
    auto myPoly2 = Polynomial{std::vector{VariableName{"y"}}};
    EXPECT_THROW([[maybe_unused]] const auto myValue = myPoly1 + myPoly2, std::runtime_error);
    EXPECT_THROW([[maybe_unused]] const auto myValue = myPoly1 - myPoly2, std::runtime_error);
    EXPECT_THROW([[maybe_unused]] const auto myValue = myPoly1 * myPoly2, std::runtime_error);
}
} // namespace polya::test
