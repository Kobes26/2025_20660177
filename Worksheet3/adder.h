// begin = adder.h =============================================

// Prevent adder.h from being included more than once
#ifndef MATHSLIB_ADDER_H
#define MATHSLIB_ADDER_H

// We need to prefix our function declarations with an additional
// keyword which is different depending on the operating system
// and whether we are building or using the library.
//
// The variable maths_EXPORTS must be defined at build time
// if we are building the library, but not if we are using it.

#if defined(WIN32) || defined(_WIN32)

    #ifdef maths_STATIC
        // Building or using a static library
        #define MATHSLIB_API
    #else
        #ifdef maths_EXPORTS
            // Building the DLL
            #define MATHSLIB_API __declspec(dllexport)
        #else
            // Using the DLL
            #define MATHSLIB_API __declspec(dllimport)
        #endif
    #endif

#else
    // Non-Windows platforms
    #define MATHSLIB_API
#endif

// Function prototype including export/import specifier
MATHSLIB_API int add(int a, int b);

#endif
// end = adder.h ===============================================