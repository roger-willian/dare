#include "cester.h"
#include "dare.h"

CESTER_BODY(
  Exception throw_directly() {
    try (
      throw("Thrown directly", 10);
      return SUCCESS;
    ) catch (
      return EVAR;
    )
  }

  Exception rethrow_directly() {
    try (
      check(throw_directly())
      return SUCCESS;
    ) catch (
      return EVAR;
    )
  }
)

CESTER_TEST(throw_catch, ti,
  int fail = 0;
  try (
    throw("Message", 20);
    fail = 1;
  ) catch (
    cester_assert_not_equal(SUCCESS, EVAR);
    cester_assert_str_equal("Message", get_msg(EVAR));
    cester_assert_equal(20, get_code(EVAR));
    cancel(EVAR);
  )
  cester_assert_false(fail);
)

CESTER_TEST(dont_throw, ti,
  int fail = 1;
  try (
    fail = 0;
  ) catch (
    fail = 1;
  )
  cester_assert_false(fail);
)

CESTER_TEST(check_directly, ti,
  int fail = 0;
  try (
    check(throw_directly())
    fail = 1;
  ) catch (
    cester_assert_not_equal(SUCCESS, EVAR);
    cester_assert_str_equal("Thrown directly", get_msg(EVAR));
    cester_assert_equal(10, get_code(EVAR));
    cancel(EVAR);
  )
  cester_assert_false(fail);
)

CESTER_TEST(rethrow_directly, ti,
  int fail = 0;
  try (
    check(rethrow_directly())
    fail = 1;
  ) catch (
    cester_assert_not_equal(SUCCESS, EVAR);
    cester_assert_str_equal("Thrown directly", get_msg(EVAR));
    cester_assert_equal(10, get_code(EVAR));
    cancel(EVAR);
  )
  cester_assert_false(fail);
)

CESTER_TEST(check_cause, ti,
  int fail = 0;
  try (
    check_cause(throw_directly(), "Thrown with cause", 30)
    fail = 1;
  ) catch (
    cester_assert_not_equal(SUCCESS, EVAR);
    cester_assert_str_equal("Thrown with cause", get_msg(EVAR));
    cester_assert_equal(30, get_code(EVAR));
    Exception cause = get_cause(EVAR);
    cester_assert_not_null(cause);
    cester_assert_str_equal("Thrown directly", get_msg(cause));
    cester_assert_equal(10, get_code(cause));
    cancel(EVAR);
  )
  cester_assert_false(fail);
)

CESTER_TEST(stacktrace, ti,
  try (
    check_cause(throw_directly(), "Thrown with cause", 30);
  ) catch (
    char *expected = ""
    "Exception: (30) Thrown with cause\n"
    "  at basic_test.c:96\n"
    "Caused by: (10) Thrown directly\n"
    "  at basic_test.c:7\n";
    CESTER_CAPTURE_STDOUT();
    print_stacktrace(EVAR);
    cester_assert_stdout_stream_content_equal(expected);
    CESTER_RELEASE_STDOUT();
    cancel(EVAR);
  )
)
