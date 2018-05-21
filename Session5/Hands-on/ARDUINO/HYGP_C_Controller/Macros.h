#ifndef MACROS_H
#define MACROS_H

#define STR(x) #x // Change the defined variable into string
//#define SHOW_DEFINE(x) printf("%s=%s\n", #x, STR(x))
#define STR2(x) STR(x)
/*
 * Concatenate preprocessor tokens A and B without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#define PPCAT_NX(A, B) A ##_## B
/*
 * Concatenate preprocessor tokens A and B after macro-expanding them.
 * Then, e.g., PCONCAT_(s, 1) produces the identifier s1.
 */
#define PCONCAT_(A, B) PPCAT_NX(A, B)
#define FUNCTION_(A, B) PPCAT_NX(A, B)

#endif
