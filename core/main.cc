#include "core/api/Polya.hh"
#include "core/util/Exception.hh"

#include <iostream>
#include <vector>

using namespace polya;

auto main() -> int
{
    const auto myOrbitCount = orbits::countOrbits(groups::tetrahedron(), orbits::ColourCount{3});
    const auto myCycleIndex = cycleIndexPolynomial(
        groups::tetrahedron(),
        std::vector{
            Polynomial::VariableName{"u"}, Polynomial::VariableName{"v"},
            Polynomial::VariableName{"w"}, Polynomial::VariableName{"x"}}
    );
    const auto myPolyaPolynomial = evaluateColours(
        myCycleIndex, orbits::ColourCount{3},
        std::vector{
            Polynomial::VariableName{"r"}, Polynomial::VariableName{"g"},
            Polynomial::VariableName{"b"}}
    );

    std::cout << "Orbits: " << myOrbitCount.get() << '\n'
              << "Cycle index polynomial: " << myCycleIndex.get().toString() << '\n'
              << "Polya polynomial with three colours: " << myPolyaPolynomial.toString() << '\n';

    ensure(
        myOrbitCount == evaluateUniform(myCycleIndex, orbits::ColourCount{3}),
        "Orbit counting theorem should match cycle index polynomial evaluated at a constant"
    );

    return 0;
}