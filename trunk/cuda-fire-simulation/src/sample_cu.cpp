#include "stdafx.h"
#include "sample_cu.h"
#include "ocuutil/float_routines.h"

UnitTest::UnitTest(const char *name) :
_name(name), _forge_ahead(false), _failed(false)
{
  UnitTestDriver::s_driver().register_test(this);
}

void 
UnitTest::assert_equal_double(double a, double b, double tol, const char *filename, int lineno)
{
  if (fabs(a-b) <= tol) return;

  printf("[ASSERT] %s::assert_equal_double(%.10f, %.10f, %.10f) at %s line %d\n", _name.c_str(), a, b, tol, filename, lineno);

  if (!_forge_ahead)
    throw UnitTestFailedException();
}

void 
UnitTest::assert_equal_float(float a, float b, float tol, const char *filename, int lineno)
{
  if (fabsf(a-b) <= tol) return;

  printf("[ASSERT] %s::assert_equal_float(%.10f, %.10f, %.10f) at %s line %d\n", _name.c_str(), a, b, tol, filename, lineno);

  if (!_forge_ahead)
    throw UnitTestFailedException();
}

void 
UnitTest::assert_equal_int(int a, int b, const char *filename, int lineno)
{
  if (a == b) return;

  printf("[ASSERT] %s::assert_equal_int(%d, %d) at %s line %d\n", _name.c_str(), a, b, filename, lineno);

  if (!_forge_ahead)
    throw UnitTestFailedException();
}

void 
UnitTest::assert_finite(double a, const char *filename, int lineno)
{
  if (ocu::check_float(a)) return;

  printf("[ASSERT] %s::assert_finite at %s line %d\n", _name.c_str(), filename, lineno);

  if (!_forge_ahead)
    throw UnitTestFailedException();
}

void 
UnitTest::assert_true(bool a, const char *filename, int lineno)
{
  if (a) return;

  printf("[ASSERT] %s::assert_true at %s line %d\n", _name.c_str(), filename, lineno);

  if (!_forge_ahead)
    throw UnitTestFailedException();
}


void UnitTestDriver::register_test(UnitTest *test)
{
  _test_list.push_back(test);
}

bool UnitTestDriver::run_tests(const std::vector<UnitTest *> &tests_to_run)
{
  bool any_failed = false;
  int i;
  printf("Running tests: ");
  for (i=0; i < tests_to_run.size(); i++) {
    if (i != 0)
      printf(", ");
    printf("%s", tests_to_run[i]->name());
  }
  printf("\n");

  for (i=0; i < tests_to_run.size(); i++) {
    bool ok = true;
    try {
      printf("running %s\n", tests_to_run[i]->name());
      tests_to_run[i]->start_test();
    }
    catch(...) {
      ok = false;
    }

    if (tests_to_run[i]->failed())
      ok = false;

    if (!ok) {
      any_failed = true;
      printf("[FAILED] %s\n", tests_to_run[i]->name());
    }
  }
  printf("\n");

  if (!any_failed) {
    printf("[PASSED]\n");
    return true;
  }
  else {
    printf("There were failures.\n");
    return false;
  }
}

void UnitTestDriver::print_tests()
{
  for (int i=0; i < _test_list.size(); i++) {
    if (i != 0)
      printf(", ");
    printf("%s", _test_list[i]->name());
  }
  printf("\n");
}



bool UnitTestDriver::run_all_tests()
{
  return run_tests(_test_list);
}

bool 
UnitTestDriver::run_tests(const std::vector<std::string> &tests)
{
  int i, j;
  std::vector<UnitTest *> tests_to_run;

  for (j=0; j < tests.size(); j++) {

    bool found = false;
    for (i = 0; !found && i < _test_list.size(); i++)
      if (tests[j] == _test_list[i]->name()) {

        tests_to_run.push_back(_test_list[i]);
        found = true;
      }

      if (!found) {
        printf("[WARNING] UnitTestDriver::run_tests - test %s not found\n", tests[j].c_str());
      }
  }

  return run_tests(tests_to_run);
}

UnitTestDriver &
UnitTestDriver::s_driver()
{
  static UnitTestDriver s_instance;
  return s_instance;
}