#ifndef COMPAT_H
#define COMPAT_H

#ifdef _MSC_VER
#define LGL_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
#define LGL_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

#ifdef _WIN32
// Support unicode command-line args on Windows
#define LGL_TMAIN wmain
using LGL_TCHAR = wchar_t;
#else
#define LGL_TMAIN main
using LGL_TCHAR = char;
#endif

#endif // COMPAT_H
