#include "cester.h"
#include "dare.h"

CESTER_TEST(assert_true, ti,
  try (
    assert_true(1, "Should not happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_false, ti,
  try (
    assert_false(0, "Should not happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_null, ti,
  try (
    assert_null(NULL, "Should not happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_not_null, ti,
  try (
    assert_not_null(NULL + 1, "Should not happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_equal, ti,
  try (
    assert_equal(7, 7, "Should not happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_not_equal, ti,
  try (
    assert_not_equal(7, -7, "Should not happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_lt, ti,
  try (
    assert_lt(1, 2, "Should not happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_gt, ti,
  try (
    assert_gt(2, 1, "Should not happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_le, ti,
  try (
    assert_le(7, 8, "Should not happen", 0);
    assert_le(7, 7, "Should not happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_ge, ti,
  try (
    assert_ge(8, 7, "Should not happen", 0);
    assert_ge(7, 7, "Should not happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_str_equal, ti,
  try (
    assert_str_equal("you dare", "you dare", "Should not happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_str_not_equal, ti,
  try (
    assert_str_not_equal("you dare", "how dare you", "Should not happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_OPTIONS(
  CESTER_TEST_SHOULD_FAIL(assert_true_fail)
  CESTER_TEST_SHOULD_FAIL(assert_false_fail)
  CESTER_TEST_SHOULD_FAIL(assert_null_fail)
  CESTER_TEST_SHOULD_FAIL(assert_not_null_fail)
  CESTER_TEST_SHOULD_FAIL(assert_equal_fail)
  CESTER_TEST_SHOULD_FAIL(assert_not_equal_fail)
  CESTER_TEST_SHOULD_FAIL(assert_lt_fail)
  CESTER_TEST_SHOULD_FAIL(assert_gt_fail)
  CESTER_TEST_SHOULD_FAIL(assert_le_fail)
  CESTER_TEST_SHOULD_FAIL(assert_ge_fail)
  CESTER_TEST_SHOULD_FAIL(assert_str_equal_fail)
  CESTER_TEST_SHOULD_FAIL(assert_str_not_equal_fail)
)

CESTER_TEST(assert_true_fail, ti,
  try (
    assert_true(0, "Should happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_false_fail, ti,
  try (
    assert_false(1, "Should happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_null_fail, ti,
  try (
    assert_null(NULL + 1, "Should happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_not_null_fail, ti,
  try (
    assert_not_null(NULL, "Should happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_equal_fail, ti,
  try (
    assert_equal(7, 8, "Should happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_not_equal_fail, ti,
  try (
    assert_not_equal(7, 7, "Should happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_lt_fail, ti,
  try (
    assert_lt(7, 7, "Should happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_gt_fail, ti,
  try (
    assert_gt(7, 7, "Should happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_le_fail, ti,
  try (
    assert_le(8, 7, "Should happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_ge_fail, ti,
  try (
    assert_ge(7, 8, "Should happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_str_equal_fail, ti,
  try (
    assert_str_equal("you dare", "how dare you", "Should happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)

CESTER_TEST(assert_str_not_equal_fail, ti,
  try (
    assert_str_not_equal("you dare", "you dare", "Should happen", 0);
  ) catch (
    cester_assert_null(EVAR);
    cancel(EVAR);
  )
)