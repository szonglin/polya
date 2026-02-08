#pragma once

#include <utility>

namespace polya
{
template <typename ValueT, typename Tag>
class Type
{
public:
    using UnderlyingT = ValueT;

    explicit Type(const ValueT& aValue) : theValue{aValue} {}
    explicit Type(ValueT&& aValue) : theValue{std::move(aValue)} {}

    ValueT& get()
    {
        return theValue;
    }

    const ValueT& get() const
    {
        return theValue;
    }

    auto operator==(const Type& anOther) const -> bool
    {
        return theValue == anOther.theValue;
    }

    static auto underlying(const Type& aType) -> const ValueT&
    {
        return aType.theValue;
    }

private:
    ValueT theValue;
};
} // namespace polya