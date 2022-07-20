#pragma once

// Some standard libraries provide forward declaration for std::string.

#include <version>

#ifdef __GLIBCXX__
#include <bits/stringfwd.h>
#elif defined(_LIBCPP_VERSION)
#include <iosfwd>
#else
#include <string>
#endif
