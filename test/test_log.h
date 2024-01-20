/**
 ******************************************************************************
 * @file           : test_log.h
 * @brief          : Debug macros from, "Learn C The Hard Way"
 * @author         : Zed Shaw
 ******************************************************************************
 */

#ifndef FASTLED_TEST_LOG_H
#define FASTLED_TEST_LOG_H

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef HIDE_TEST_LOGS
#define test_debug(M, ...)
#define test_log_err(M, ...)
#define test_log_warn(M, ...)
#define test_log_info(M, ...)
#else
#define test_debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", \
    __FILE__, __LINE__, ##__VA_ARGS__)

#define test_log_err(M, ...) fprintf(stderr, \
    "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, \
    clean_errno(), ##__VA_ARGS__)

#define test_log_warn(M, ...) fprintf(stdout, \
    "[WARN] (%s:%d: errno: %s) " M "\n", \
    __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define test_log_info(M, ...) fprintf(stdout, "[INFO] (%s:%d) " M "\n", \
    __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_fatal(M, ...) fprintf(stderr, \
    "[FATAL] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, \
    clean_errno(), ##__VA_ARGS__)

#define log_err_minunit(M, ...) fprintf(stderr, \
    "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, \
    clean_errno(), ##__VA_ARGS__)

#define check(A, M, ...) if (!(A)) {\
  test_log_err(M, ##__VA_ARGS__); errno = 0; goto error; }

#define sentinel(M, ...) { test_log_err(M, ##__VA_ARGS__);\
  errno = 0; goto error; }

#define check_mem(A) check((A), "Out of memory.")

#define check_debug(A, M, ...) if (!(A)) { test_debug(M, ##__VA_ARGS__);\
  errno = 0; goto error; }

#endif // FASTLED_TEST_LOG_H
