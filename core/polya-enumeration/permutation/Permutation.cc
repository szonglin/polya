#include "core/polya-enumeration/permutation/Permutation.hh"

#include "core/util/Exception.hh"

#include <algorithm>
#include <cmath>
#include <range/v3/all.hpp>
#include <utility>

namespace polya
{
namespace views = ranges::views;

auto Permutation::CycleStructure::totalCycleCount() const -> Count
{
    const auto myTotal =
        ranges::accumulate(theCycleLengthDistribution | views::transform(Count::underlying), 0uz);
    return Count{myTotal};
}

auto Permutation::CycleStructure::degree() const -> Degree
{
    const auto myDegree = ranges::inner_product(
        views::iota(0uz), theCycleLengthDistribution | views::transform(&Count::underlying), 0uz
    );
    return Degree{myDegree};
}

Permutation::Permutation(Degree aDegree)
    : theBijection{
        views::iota(0uz, aDegree.get())
        | views::transform([](const auto aValue) { return Element{aValue}; })
        | ranges::to<std::vector>()}
{
}

Permutation::Permutation(std::vector<Element> aBijection) : theBijection{std::move(aBijection)} {}

Permutation::Permutation(Degree aDegree, const std::vector<Cycle>& aCycles) : Permutation{aDegree}
{
    for (const auto& myCycle : aCycles)
    {
        const auto& myElements = myCycle.get();
        for (const auto myIndex : views::iota(0uz, myElements.size()))
        {
            ensure(
                isValidElement(myElements[myIndex]),
                "Cycle element {} is not in the domain of permutation with degree {}",
                myElements[myIndex].get(), aDegree.get()
            );
            theBijection[myElements[myIndex].get()] = myElements[(myIndex + 1) % myElements.size()];
        }
    }
}

auto Permutation::operator()(Element anElement) const -> Element
{
    ensure(
        isValidElement(anElement), "Element {} is not in the domain of permutation with degree {}",
        anElement.get(), degree().get()
    );

    return theBijection[anElement.get()];
}

auto Permutation::operator*=(const Permutation& aPermutation) -> Permutation&
{
    ensure(degree() == aPermutation.degree(), "Cannot compose permutations of different degrees");
    theBijection =
        aPermutation.theBijection
        | views::transform([&](const Element& anElement) { return theBijection[anElement.get()]; })
        | ranges::to<std::vector>();
    return *this;
}

auto Permutation::operator*(const Permutation& aPermutation) const -> Permutation
{
    auto myResult = *this;
    myResult *= aPermutation;
    return myResult;
}

auto Permutation::operator==(const Permutation& aPermutation) const -> bool
{
    return theBijection == aPermutation.theBijection;
}

auto Permutation::operator<=>(const Permutation& aPermutation) const -> std::strong_ordering
{
    return std::lexicographical_compare_three_way(
        theBijection.begin(), theBijection.end(), aPermutation.theBijection.begin(),
        aPermutation.theBijection.end(),
        [](const Element& aLhs, const Element& aRhs) { return aLhs.get() <=> aRhs.get(); }
    );
}

auto Permutation::inverse() const -> Permutation
{
    auto myInverseBijection = std::vector<Element>(degree().get(), Element{0});
    for (const auto [myIndex, myValue] : theBijection | views::enumerate)
    {
        myInverseBijection[myValue.get()] = Element{myIndex};
    }
    return Permutation{myInverseBijection};
}

auto Permutation::power(std::int32_t aPower) const -> Permutation
{
    auto myBase = aPower < 0 ? inverse() : *this;
    auto myResult = Permutation{degree()};
    aPower = std::abs(aPower);

    for (; aPower > 0; aPower /= 2)
    {
        if (aPower % 2 == 1)
        {
            myResult *= myBase;
        }
        myBase *= myBase;
    }
    return myResult;
}

auto Permutation::degree() const noexcept -> Degree
{
    return Degree{theBijection.size()};
}

auto Permutation::isIdentity() const noexcept -> bool
{
    return ranges::equal(
        theBijection | views::transform(&Element::underlying), views::iota(0uz, theBijection.size())
    );
}

auto Permutation::asCycles() const -> std::vector<Cycle>
{
    auto myVisited = std::vector<bool>(degree().get(), false);
    auto myCycles = std::vector<Cycle>{};
    for (const auto myElement : views::iota(0uz, degree().get()))
    {
        if (myVisited[myElement])
        {
            continue;
        }
        auto myCycleElements = std::vector<Element>{};
        auto myCurrent = myElement;
        while (not myVisited[myCurrent])
        {
            myVisited[myCurrent] = true;
            myCycleElements.emplace_back(myCurrent);
            myCurrent = theBijection[myCurrent].get();
        }
        myCycles.emplace_back(std::move(myCycleElements));
    }
    return myCycles;
}

auto Permutation::cycleStructure() const -> CycleStructure
{
    const auto myCycles = asCycles();
    auto myDistribution =
        std::vector<CycleStructure::Count>(degree().get() + 1, CycleStructure::Count{0});
    for (const auto& myLength : myCycles | views::transform(&Cycle::underlying)
                                    | views::transform(&std::vector<Element>::size))
    {
        myDistribution[myLength] = CycleStructure::Count{myDistribution[myLength].get() + 1};
    }
    return CycleStructure{std::move(myDistribution)};
}

auto Permutation::toString() const -> std::string
{
    const auto myElementToString = [](const Element& anElement)
    { return std::to_string(anElement.get()); };
    const auto myCycles = asCycles();
    const auto myFormatCycle = [&](const Cycle& aCycle)
    {
        const auto myElements = aCycle.get() | views::transform(myElementToString)
                                | views::join(std::string_view(" ")) | ranges::to<std::string>();
        return '(' + myElements + ')';
    };
    return myCycles | views::transform(myFormatCycle) | views::join | ranges::to<std::string>();
}

auto operator<<(std::ostream& aStream, const Permutation& aPermutation) -> std::ostream&
{
    return aStream << aPermutation.toString();
}

auto Permutation::isValidElement(Element anElement) const noexcept -> bool
{
    return anElement.get() < degree().get();
}

namespace permutations
{
using Degree = Permutation::Degree;
using Element = Permutation::Element;

// Creates a rotation by one permutation of the given degree
auto rotation(Degree aDegree) -> Permutation
{
    const auto myDegreeValue = aDegree.get();
    auto myBijection = views::iota(0uz, myDegreeValue)
                       | views::transform([&](const auto anIndex)
                                          { return Element{(anIndex + 1) % myDegreeValue}; })
                       | ranges::to<std::vector>();
    return Permutation{std::move(myBijection)};
}

// Creates a reflection over index 0 permutation of the given degree
auto reflection(Degree aDegree) -> Permutation
{
    const auto myDegreeValue = aDegree.get();
    auto myBijection =
        views::iota(0uz, myDegreeValue)
        | views::transform([&](const auto anIndex)
                           { return Element{(myDegreeValue - anIndex) % myDegreeValue}; })
        | ranges::to<std::vector>();
    return Permutation{std::move(myBijection)};
}

// Creates a permutation of the given degree with a transposition of the given elements
auto transposition(Degree aDegree, Element aFirst, Element aSecond) -> Permutation
{
    return Permutation{aDegree, {Permutation::Cycle{{aFirst, aSecond}}}};
}
} // namespace permutations

} // namespace polya
