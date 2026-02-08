#pragma once

#include "core/util/Type.hh"

#include <cstdint>

namespace polya::mathutil
{
using Base = Type<std::uint32_t, struct BaseTag>;
using Exponent = Type<std::uint32_t, struct ExponentTag>;
using PowerResult = Type<std::uint64_t, struct ResultTag>;

auto power(Base aBase, Exponent anExponent) -> PowerResult
{
    auto myResult = PowerResult{1};
    while (anExponent.get() > 0)
    {
        if (anExponent.get() % 2 == 1)
        {
            myResult.get() *= aBase.get();
        }
        aBase.get() *= aBase.get();
        anExponent.get() /= 2;
    }
    return myResult;
}

} // namespace polya::mathutil