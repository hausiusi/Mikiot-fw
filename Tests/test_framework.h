/*
 * test_framework.h
 *
 *  Created on: Jul 14, 2020
 *      Author: Zviad
 */

#ifndef TEST_FRAMEWORK_H_
#define TEST_FRAMEWORK_H_

#include <string.h>
#include "debug.h"
#include "mikiot_config.h"
#include "mw_timebase.h"

typedef struct {
	uint32_t passed;
	uint32_t warning;
	uint32_t failed;
	uint32_t start;
	uint32_t end;
	uint32_t elapsed;
} tf_result;

tf_result tf_current_results, tf_total_results;

#define tf_write(frmt, vargs...)              do { debugf(frmt, ##vargs); mw_uart1_dma_string_transmit(tf_debug_array); } while(0)

#define tf_init_test_results()                                                             \
	do {                                                                                   \
		   tf_total_results.passed += tf_current_results.passed;                           \
		   tf_total_results.failed += tf_current_results.failed;                           \
		   tf_total_results.warning += tf_current_results.warning;                         \
		   tf_total_results.elapsed += tf_current_results.elapsed;                         \
		   tf_total_results.end = tf_current_results.end;                                  \
		   if (!tf_total_results.start) tf_total_results.start = tf_current_results.start; \
		   memset((uint8_t*)&tf_current_results, 0, sizeof(tf_result));                    \
		   tf_current_results.start = mw_timebase_ticks_get();                             \
	} while(0)

#define tf_write_pass(frmt, vargs...)       \
	do {                                    \
           tf_write("PASS: "frmt, vargs);   \
           tf_current_results.passed++;     \
}while(0)

#define tf_write_warning(frmt, vargs...)     \
	do {                                     \
		   tf_write("WARNING: "frmt, vargs); \
		   tf_current_results.warning++;     \
	}while(0)

#define tf_write_fail(frmt, vargs...)         \
		do {                                  \
			   tf_write("FAIL: "frmt, vargs); \
			   tf_current_results.failed++;   \
		}while(0)

#define _flood_symbols(symbol, times) do { for(int i=times; i--;) tf_write(symbol); } while(0)

#define tf_write_title(tc_f)                                    \
	do {                                                        \
		   _flood_symbols("-", sizeof("Test case: "#tc_f) + 1); \
		   tf_write("\n|Test case: "#tc_f"|\n");                \
		   _flood_symbols("-", sizeof("Test case: "#tc_f) + 1); \
		   tf_write("\n");                                      \
}while(0)

#define tf_begin(tc_f)                        \
	do {                                      \
		   tf_write_title(tc_f);              \
		   tf_init_test_results();            \
}while(0)

#define tf_end(tc_f)                                                                                                                               \
	do {                                                                                                                                           \
		 tf_current_results.end = mw_timebase_ticks_get();                                                                                         \
		 tf_current_results.elapsed = tf_current_results.end - tf_current_results.start;                                                           \
         _flood_symbols("=", sizeof(#tc_f": ") + 6);                                                                                               \
         tf_write("\n"#tc_f": %s\n", tf_current_results.failed == 0 ? "PASSED" : "FAILED");                                                        \
         tf_write("\nPasses: %u\nWarnings: %u\nFailures: %u\n", tf_current_results.passed, tf_current_results.warning, tf_current_results.failed); \
         tf_write("Elapsed: %ums\n", tf_current_results.elapsed);                                                                                  \
         _flood_symbols("=", sizeof(#tc_f": ") + 6);                                                                                               \
         tf_write("\n");                                                                                                                           \
} while(0)

#define tf_run(tc_f)                   	             \
	do {                                             \
	       tf_begin(tc_f);                           \
	       tc_f;                       	             \
           tf_end(tc_f);                             \
}while(0)

#define tf_run_all(group)

#define tf_equals(a, b)                                                  \
	do {                                                                 \
		   if (a == b) tf_write_pass("%s==%s: (%u=%u)\n", #a, #b, a, b); \
           else tf_write_fail("%s==%s: (%u!=%u)\n", #a, #b, a, b);       \
	} while (0)

#define tf_not_equals(a, b) \
		do {                                                                  \
			   if (a != b) tf_write_pass("%s!=%s: (%u!=%u)\n", #a, #b, a, b); \
	           else tf_write_fail("%s!=%s: (%u=%u)\n", #a, #b, a, b);         \
		} while (0)

#define tf_string_equals(str1, str2)                                              \
		do {                                                                      \
			   if (strcmp(str1, str2) == 0) tf_write_pass("%s=%s\n", str1, str2); \
	           else tf_write_fail("%s!=%s\n", str1, str2);                        \
		} while (0)

#define tf_string_contains(str, substr)                                                      \
		do {                                                                                 \
			   if (strstr(str, substr) != 0) tf_write_pass("%s contains %s\n", str, substr); \
	           else tf_write_fail("%s doesn't contain %s\n", str, substr);                   \
		} while (0)

#endif /* TEST_FRAMEWORK_H_ */
