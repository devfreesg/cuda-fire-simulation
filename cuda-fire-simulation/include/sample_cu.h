/********************************************************************
*  sample_cu.h
*  This is a example of the CUDA program.
*********************************************************************/
#ifndef __SAMPLE_H__
#define __SAMPLE_H__

#include <string>
#include <vector>
#include "ocuutil/defines.h"
#include "ocuutil/timer.h"

class UnitTestFailedException {
};

class UnitTest {
  std::string _name;
  bool        _forge_ahead;
  bool        _failed;
  ocu::CPUTimer _timer;

protected:

  UnitTest(const char *name);

  void assert_equal_double(double, double, double tol, const char *filename, int lineno);
  void assert_equal_float(float, float, float tol, const char *filename, int lineno);
  void assert_equal_int(int, int, const char *filename, int lineno);
  void assert_finite(double, const char *filename, int lineno);
  void assert_true(bool, const char *filename, int lineno);

  void set_forge_ahead(bool onoff) { _forge_ahead = onoff; }
  void set_failed() { _failed = true; }

  float elapsed_ms() { ocu::CPUTimer temp = _timer; temp.stop(); return temp.elapsed_ms(); }

  virtual void run() = 0;

public:

  bool failed() const { return _failed; }
  const char *name() const { return _name.c_str(); }

  void start_test() {
    _timer.start();
    run();
  }
};

#define UNITTEST_ASSERT_EQUAL_DOUBLE(a,b,t) this->assert_equal_double((a),(b),(t),__FILE__, __LINE__)
#define UNITTEST_ASSERT_EQUAL_FLOAT(a,b,t)  this->assert_equal_float((a),(b),(t),__FILE__, __LINE__)
#define UNITTEST_ASSERT_EQUAL_INT(a,b)      this->assert_equal_int((a),(b),__FILE__, __LINE__)
#define UNITTEST_ASSERT_FINITE(a)           this->assert_finite((a),__FILE__, __LINE__)
#define UNITTEST_ASSERT_TRUE(a)             this->assert_true((a),__FILE__, __LINE__)


#define DECLARE_UNITTEST_BEGIN(TEST) \
  template<typename DUMMY_TYPE> \
  class TEST : public UnitTest { \
  public: \
   TEST() : UnitTest(#TEST) { } \
   int dummy_so_semi_colon_will_be_parsed


#define DECLARE_UNITTEST_DOUBLE_BEGIN(TEST) \
  template<typename DUMMY_TYPE> \
  class TEST : public UnitTest { \
  public: \
   TEST() : UnitTest(#TEST) { } \
   int dummy_so_semi_colon_will_be_parsed


#define DECLARE_UNITTEST_END(TEST) \
  }; \
  TEST<int> TEST##_instance

#ifdef OCU_DOUBLESUPPORT
#define DECLARE_UNITTEST_DOUBLE_END(TEST) \
  }; \
  TEST<int> TEST##_instance
#else
#define DECLARE_UNITTEST_DOUBLE_END(TEST) \
  }; \
  int TEST##_instance
#endif

class UnitTestDriver {
  std::vector<UnitTest *> _test_list;

  bool run_tests(const std::vector<UnitTest *> &tests);

public:

  void register_test(UnitTest *);
  bool run_all_tests();
  bool run_tests(const std::vector<std::string> &tests);
  void print_tests();

  static UnitTestDriver &s_driver();
};


#endif // __SAMPLE_H__