

#ifndef TEST_MACROS_H
#define TEST_MACROS_H

#include <stdio.h>

#define STRINGIFY(val) #val
#define STR(val) STRINGIFY(val)


/**
 * @brief Check to see if an expression is true.
 *
 * @param expr The expression to test.
 */
#define expect(expr) {\
	if(!(expr)) {\
		fprintf(stderr, "ERROR: !%s at line: %d\n",\
				STR(expr), __LINE__);\
		exit(-1);\
	}\
}

/**
 * @brief Check to see if a and be are equal.
 *
 * @param a The first value.
 * @param b The second value.
 */
#define expect_eq(a, b) {\
	if(a != b) {\
		fprintf(stderr, "ERROR: %s != %s at line: %d\n",\
				STR(a), STR(b), __LINE__);\
		exit(-1);\
	}\
}

/**
 * @brief Check to see if a and be are ot nequal.
 *
 * @param a The first value.
 * @param b The second value.
 */
#define expect_neq(a, b) {\
	if(a == b) {\
		fprintf(stderr, "ERROR: %s == %s at line: %d\n",\
				STR(a), STR(b), __LINE__);\
		exit(-1);\
	}\
}

#endif /* TEST_MACROS_H */
