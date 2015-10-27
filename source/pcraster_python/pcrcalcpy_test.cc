#define BOOST_TEST_MODULE pcrcalcpy test suite
#ifndef INCLUDED_STDDEFX
#include "stddefx.h"
#define INCLUDED_STDDEFX
#endif

#ifndef INCLUDED_PCRDLL
#include "pcrdll.h"
#define INCLUDED_PCRDLL
#endif


#ifndef INCLUDED_BOOST_TEST_INCLUDED_UNIT_TEST
#include <boost/test/included/unit_test.hpp>
#define INCLUDED_BOOST_TEST_INCLUDED_UNIT_TEST
#endif

#ifndef INCLUDED_PCRCALCPY_PYTHONUNITTEST
#include "pcrcalcpy_pythonunittest.h"
#define INCLUDED_PCRCALCPY_PYTHONUNITTEST
#endif



 boost::unit_test::test_suite* init_unit_test_suite(int /*argc*/, char ** const /*argv*/) {
  boost::unit_test::test_suite* test = BOOST_TEST_SUITE(__FILE__);
  test->add(pcrcalcpy::PythonUnitTest().suite());

  return test;
}
