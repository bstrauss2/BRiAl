// -*- c++ -*-
//*****************************************************************************
/** @file BooleSetTest.cc
 *
 * @author Ket Kalda, Alexander Dreyer
 * @date 2010-10-07
 *
 * boost/test-driven unit test
 * 
 * @par Copyright:
 *   (c) 2010 by The PolyBoRi Team
 *
 **/
//*****************************************************************************

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
using boost::test_tools::output_test_stream;

#include "pbori_defs.h"
#include "BooleVariable.h"
#include "BooleMonomial.h"
#include "BoolePolynomial.h"
#include "BooleExponent.h"
#include "BoolePolyRing.h"

USING_NAMESPACE_PBORI

struct Fset {
  typedef BooleSet set_type;
  Fset():
    ring(5) {
      x = BooleVariable(0);
      y = BooleVariable(1);
      z = BooleVariable(2);
      v = BooleVariable(3);
      w = BooleVariable(4);
      BOOST_TEST_MESSAGE( "setup fixture" );
      ring.setVariableName(0, "x");
      ring.setVariableName(1, "y");
      ring.setVariableName(2, "z");
      ring.setVariableName(3, "v");
      ring.setVariableName(4, "w");
      poly = x*y*z + v*z - x*v + y;
    }

    ~Fset() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

    BoolePolyRing ring;
    BooleVariable x, y, z, v, w;
    BoolePolynomial poly;
};

BOOST_FIXTURE_TEST_SUITE(BooleSetTestSuite, Fset )

BOOST_AUTO_TEST_CASE(test_constructors) {
  set_type set;
  output_test_stream output;
  output << set;
  BOOST_CHECK(output.is_equal("{}"));
  set = set.add(x);
  set_type set2;
  BOOST_CHECK_THROW(set_type(0,set,set2), PBoRiError);
  set_type set1;
  set1 = set1.add(y);
  set_type set3 = set_type(0,set1,set2);
  output << set3;
  BOOST_CHECK(output.is_equal("{{x,y}}"));
  set = poly.set();
  output << set;
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,v}, {y}, {z,v}}"));
}

BOOST_AUTO_TEST_CASE(test_variables) {

  BOOST_TEST_MESSAGE( "usedVariables, usedVariablesExp" );
  output_test_stream output;
  set_type set = poly.set();
  BOOST_CHECK_EQUAL(set.usedVariables(),x*y*z*v);
  set_type empty;
  BOOST_CHECK_EQUAL(empty.usedVariables(),BooleMonomial());
  BooleExponent exp = set.usedVariablesExp();
  output << exp;
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3)"));
  exp = empty.usedVariablesExp();
  output << exp;
  BOOST_CHECK(output.is_equal("()"));

  BOOST_TEST_MESSAGE( "lastLexicographicalTerm" );
  BOOST_CHECK_EQUAL(set.lastLexicographicalTerm(), BooleMonomial(v*z));
  //BOOST_CHECK_EQUAL(empty.lastLexicographicalTerm(), BooleMonomial());//memory access violation at address 0x00000000

  BOOST_TEST_MESSAGE( "minimalElements" );
  set_type set2 = set.minimalElements();
  output << set2;
  BOOST_CHECK(output.is_equal("{{x,v}, {y}, {z,v}}"));
  set2 = empty.minimalElements();
  output << set2;
  BOOST_CHECK(output.is_equal("{}"));

  BOOST_TEST_MESSAGE( "hasTermOfVariables" );
  BOOST_CHECK(set.hasTermOfVariables(x*v));
  BOOST_CHECK(set.hasTermOfVariables(x*y*z));
  BOOST_CHECK(set.hasTermOfVariables(x*z*y));
  BOOST_CHECK(set.hasTermOfVariables(y));
  BOOST_CHECK(set.hasTermOfVariables(z*v));
  BOOST_CHECK(!set.hasTermOfVariables(x));
  BOOST_CHECK(!set.hasTermOfVariables(z));
  BOOST_CHECK(!set.hasTermOfVariables(v));
  BOOST_CHECK(!set.hasTermOfVariables(w));
  BOOST_CHECK(set.hasTermOfVariables(x*y));
  BOOST_CHECK(set.hasTermOfVariables(y*x));
  BOOST_CHECK(set.hasTermOfVariables(y*z));
  BOOST_CHECK(set.hasTermOfVariables(z*y));

  BOOST_TEST_MESSAGE( "contains" );
  BOOST_CHECK(!set.contains(empty));
  BOOST_CHECK(set.contains(set));
  BOOST_CHECK(empty.contains(empty));
  set2 = empty.add(x*y*z);
  
  std::cout << set<<" "<<set.ring().hash() <<std::endl;
  std::cout << set2<<" "<<set2.ring().hash() <<std::endl;
  
  //BOOST_CHECK(set.contains(set2)); // memory access violation at address: 0x00000004: no mapping at fault address

  BOOST_TEST_MESSAGE( "emptyElement" );
  set_type emptyEl = empty.emptyElement();
  output << emptyEl;
  BOOST_CHECK(output.is_equal("{}"));
  emptyEl = set.emptyElement();
  output << emptyEl;
  BOOST_CHECK(output.is_equal("{}"));

  BOOST_TEST_MESSAGE( "countIndex" );
  BOOST_CHECK_EQUAL(set.countIndex(0),2);
  BOOST_CHECK_EQUAL(set.countIndex(1),2);
  BOOST_CHECK_EQUAL(set.countIndex(2),2);
  BOOST_CHECK_EQUAL(set.countIndex(3),2);
  BOOST_CHECK_EQUAL(set.countIndex(4),0);
  BOOST_CHECK_EQUAL(set.countIndex(5),0);
  BOOST_CHECK_EQUAL(empty.countIndex(0),0);
  BOOST_CHECK_EQUAL(empty.countIndex(1),0);
  BOOST_CHECK_EQUAL(empty.countIndex(2),0);
  BOOST_CHECK_EQUAL(empty.countIndex(3),0);
  BOOST_CHECK_EQUAL(empty.countIndex(4),0);
  BOOST_CHECK_EQUAL(empty.countIndex(5),0);

  BOOST_TEST_MESSAGE( "countIndexDouble" );
  BOOST_CHECK_EQUAL(set.countIndexDouble(0),2);
  BOOST_CHECK_EQUAL(set.countIndexDouble(1),2);
  BOOST_CHECK_EQUAL(set.countIndexDouble(2),2);
  BOOST_CHECK_EQUAL(set.countIndexDouble(3),2);
  BOOST_CHECK_EQUAL(set.countIndexDouble(4),0);
  BOOST_CHECK_EQUAL(set.countIndexDouble(5),0);
  BOOST_CHECK_EQUAL(empty.countIndexDouble(0),0);
  BOOST_CHECK_EQUAL(empty.countIndexDouble(1),0);
  BOOST_CHECK_EQUAL(empty.countIndexDouble(2),0);
  BOOST_CHECK_EQUAL(empty.countIndexDouble(3),0);
  BOOST_CHECK_EQUAL(empty.countIndexDouble(4),0);
  BOOST_CHECK_EQUAL(empty.countIndexDouble(5),0);

  BOOST_TEST_MESSAGE( "containsDivisorsOfDecDeg" );
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(y));
}

BOOST_AUTO_TEST_CASE(test_properties) {

  BOOST_TEST_MESSAGE( "isSingleton, isSingletonOrPair, isPair" );

  set_type set = poly.set();
  BOOST_CHECK(!set.isSingleton());
  BOOST_CHECK(!set.isSingletonOrPair());
  BOOST_CHECK(!set.isPair());

  set_type empty;
  BOOST_CHECK(empty.isSingleton());
  BOOST_CHECK(empty.isSingletonOrPair());
  BOOST_CHECK(!empty.isPair());
  empty = empty.add(BooleMonomial());
  BOOST_CHECK(empty.isSingleton());
  BOOST_CHECK(empty.isSingletonOrPair());
  BOOST_CHECK(!empty.isPair());

  set_type single;
  single = single.add(x);
  BOOST_CHECK(single.isSingleton());
  BOOST_CHECK(single.isSingletonOrPair());
  BOOST_CHECK(!single.isPair());
  single = single.add(BooleMonomial());
  BOOST_CHECK(!single.isSingleton());
  BOOST_CHECK(single.isSingletonOrPair());
  BOOST_CHECK(single.isPair());

  set_type pair;
  pair = pair.add(x);
  pair = pair.add(y);
  BOOST_CHECK(!pair.isSingleton());
  BOOST_CHECK(pair.isSingletonOrPair());
  BOOST_CHECK(pair.isPair());
  pair = pair.add(BooleMonomial());
  BOOST_CHECK(!pair.isSingleton());
  BOOST_CHECK(!pair.isSingletonOrPair());
  BOOST_CHECK(!pair.isPair());
}

BOOST_AUTO_TEST_CASE(test_add_own) {

  set_type set = poly.set();
  set_type empty;
  BOOST_CHECK(set.owns(x*y*z));
  BOOST_CHECK(set.owns(v*z));
  BOOST_CHECK(set.owns(x*v));
  BOOST_CHECK(set.owns(y));
  BOOST_CHECK(!set.owns(x));
  BOOST_CHECK(!set.owns(z));
  BOOST_CHECK(!set.owns(v));
  BOOST_CHECK(!set.owns(w));
  BOOST_CHECK(!set.owns(x*v*z));
  BOOST_CHECK(!set.owns(BooleMonomial()));
  BOOST_CHECK(!empty.owns(x));
  BOOST_CHECK(!empty.owns(y));
  BOOST_CHECK(!empty.owns(z));
  BOOST_CHECK(!empty.owns(v));
  BOOST_CHECK(!empty.owns(w));

  set_type addedset = set.add(x);
  addedset = addedset.add(x*v*z);
  BOOST_CHECK(addedset.owns(x*y*z));
  BOOST_CHECK(addedset.owns(v*z));
  BOOST_CHECK(addedset.owns(x*v));
  BOOST_CHECK(addedset.owns(y));
  BOOST_CHECK(addedset.owns(x));
  BOOST_CHECK(!addedset.owns(z));
  BOOST_CHECK(!addedset.owns(v));
  BOOST_CHECK(!addedset.owns(w));
  BOOST_CHECK(addedset.owns(x*v*z));
  addedset = empty.add(x);
  addedset = addedset.add(y);
  addedset = addedset.add(v*w);
  BOOST_CHECK(addedset.owns(x));
  BOOST_CHECK(addedset.owns(y));
  BOOST_CHECK(addedset.owns(v*w));
  BOOST_CHECK(!addedset.owns(v));
  BOOST_CHECK(!addedset.owns(w));

  set_type empty2;
  empty2 = empty2.add(x);
  BOOST_CHECK(empty2.owns(x));

  BOOST_CHECK(!set.ownsOne());
  addedset = set.add(BooleMonomial());
  BOOST_CHECK(addedset.ownsOne());
  BOOST_CHECK(!empty.ownsOne());
  addedset = empty.add(BooleMonomial());
  BOOST_CHECK(addedset.ownsOne());
}

BOOST_AUTO_TEST_CASE(test_division) {

  BOOST_TEST_MESSAGE( "divisors" );

  output_test_stream output;
  set_type set = poly.set();
  set_type result = set.divisorsOf(x);
  output << result;
  BOOST_CHECK(output.is_equal("{}"));
  result = set.divisorsOf(y);
  output << result;
  BOOST_CHECK(output.is_equal("{{y}}"));
  result = set.divisorsOf(z);
  output << result;
  BOOST_CHECK(output.is_equal("{}"));
  result = set.divisorsOf(v);
  output << result;
  BOOST_CHECK(output.is_equal("{}"));
  result = set.divisorsOf(w);
  output << result;
  BOOST_CHECK(output.is_equal("{}"));

  BOOST_TEST_MESSAGE( "divide" );

  result = set.divide(x);
  output << result;
  BOOST_CHECK(output.is_equal("{{y,z}, {v}}"));
  result = set.divide(y);
  output << result;
  BOOST_CHECK(output.is_equal("{{x,z}, {}}"));
  result = set.divide(z);
  output << result;
  BOOST_CHECK(output.is_equal("{{x,y}, {v}}"));
  result = set.divide(v);
  output << result;
  BOOST_CHECK(output.is_equal("{{x}, {z}}"));
  result = set.divide(w);
  output << result;
  BOOST_CHECK(output.is_equal("{}"));
  result = set.divide(x*v);
  output << result;
  BOOST_CHECK(output.is_equal("{{}}"));
  result = set.divide(x*y);
  output << result;
  BOOST_CHECK(output.is_equal("{{z}}"));
  result = set.divide(x*v*y);
  output << result;
  BOOST_CHECK(output.is_equal("{}"));
  result = set.divide(BooleMonomial());
  output << result;
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,v}, {y}, {z,v}}"));
  set_type empty;
  result = empty.divide(x*v*y);
  output << result;
  BOOST_CHECK(output.is_equal("{}"));
  result = empty.divide(BooleMonomial());
  output << result;
  BOOST_CHECK(output.is_equal("{}"));

  BOOST_TEST_MESSAGE( "firstDivisorsOf" );
  BoolePolynomial poly2 = x*y*z + x;
  set_type set2 = poly2.set();
  result = set.firstDivisorsOf(set2);
  output << result;
  BOOST_CHECK(output.is_equal("{{x,y,z}, {y}}"));
}

BOOST_AUTO_TEST_CASE(test_size_values) {

  set_type set = poly.set();
  set = set.add(x);
  set = set.add(x*y);
  std::cout << set << std::endl;
  set_type empty;

  BOOST_TEST_MESSAGE( "size" );
  BOOST_CHECK_EQUAL(set.size(),6);
  BOOST_CHECK_EQUAL(empty.size(),0);

  BOOST_TEST_MESSAGE( "length" );
  BOOST_CHECK_EQUAL(set.length(),6);
  BOOST_CHECK_EQUAL(empty.length(),0);

  BOOST_TEST_MESSAGE( "nVariables" );
  BOOST_CHECK_EQUAL(set.nVariables(),5);
  BOOST_CHECK_EQUAL(empty.nVariables(),5);

  BOOST_TEST_MESSAGE( "sizeDouble" );
  BOOST_CHECK_EQUAL(set.sizeDouble(),6);
  BOOST_CHECK_EQUAL(empty.sizeDouble(),0);
}

BOOST_AUTO_TEST_SUITE_END()