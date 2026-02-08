#pragma once

#include <format>
#include <source_location>
#include <stdexcept>

#define UTIL_DEBUG

namespace polya
{
inline auto throw_runtime_error(const char* aString, std::source_location aLocation) -> void
{
    throw std::runtime_error(std::format(
        "Runtime error at {} {} {}:\n\t{}", aLocation.file_name(), aLocation.line(),
        aLocation.function_name(), aString
    ));
}

template <typename... Args>
auto throw_runtime_error(std::format_string<Args...> aFormatString, Args&&... args) -> void
{
    throw std::runtime_error(std::format(
        "Runtime error at {} {} {}:\n\t{}", std::source_location::current().file_name(),
        std::source_location::current().line(), std::source_location::current().function_name(),
        std::format(aFormatString, std::forward<Args>(args)...)
    ));
}

inline auto ensure(bool expression, const char* aString, std::source_location aLocation) -> void
{
    if (not expression) [[unlikely]]
    {
        throw_runtime_error(aString, aLocation);
    }
}

template <typename... Args>
auto ensure(bool expression, std::format_string<Args...> aFormatString, Args&&... args) -> void
{
    if (not expression) [[unlikely]]
    {
        throw_runtime_error(aFormatString, std::forward<Args>(args)...);
    }
}

inline auto ensure_debug(
    [[maybe_unused]] bool expression, [[maybe_unused]] const char* aString,
    [[maybe_unused]] std::source_location aLocation
) -> void
{
#ifdef UTIL_DEBUG
    if (not expression) [[unlikely]]
    {
        throw_runtime_error(aString, aLocation);
    }
#endif
}

template <typename... Args>
auto ensure_debug(
    [[maybe_unused]] bool expression, [[maybe_unused]] std::format_string<Args...> aFormatString,
    [[maybe_unused]] Args&&... args
) -> void
{
#ifdef UTIL_DEBUG
    if (not expression) [[unlikely]]
    {
        throw_runtime_error(aFormatString, std::forward<Args>(args)...);
    }
#endif
}
} // namespace polya
