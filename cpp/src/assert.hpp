#pragma once

// Painless assertions. Greatly inspired from rsg and netorcai.

#include <cstdarg>
#include <cstdio>
#include <exception>

namespace batprotocol
{

/// batprotocol's exception class
class Error : public std::exception
{
private:
    char text[1024]; //!< The error message

public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    /// Builds an Error with a printf-like format
    explicit Error(char const* fmt, ...) __attribute__((format(printf,2,3)))
    {
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(text, sizeof text, fmt, ap);
        va_end(ap);
    }
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

    /// Gets Error's message
    /// @return The Error's message.
    char const* what() const throw() { return text; }
};

} // end of batprotocol namespace

/// Define an ENFORCE macro similar to D's enforce.
#define BAT_ENFORCE(pred, fmt, ...) \
    do { \
        if (false == (pred)) { \
            throw batprotocol::Error(fmt, ##__VA_ARGS__); \
        } \
    } while (0)

/// Define an ASSERT macro similar to D's assert.
#ifndef NDEBUG
    #define BAT_ASSERT BAT_ENFORCE
#else
    #define BAT_ASSERT(pred, fmt, ...) do {} while (0)
#endif
