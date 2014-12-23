/*
 * wolutil.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: user
 */

#include "wolutil.h"
#include "common.h"
#include <boost/dynamic_bitset.hpp>

namespace wolver
{

  dbitset
    createSubsetOfDynamicBitSet (dbitset &source, int low, int high) {

      assert(low <= high);
      assert(low >= 0);
      dbitset result(high- low + 1);

      for (int i = low; i <= high; i++) {
        result[i - low] = source[i];
      }

      return result;
    }

  bool
  fullAdder (bool b1, bool b2, bool& carry) {

    bool sum = (b1 ^ b2) ^ carry;
    carry = (b1 && b2) || (b1 && carry) || (b2 && carry);
    return sum;
  }

  dbitset
  bitSetAdd (dbitset val1, dbitset val2) {

    assert(val1.size () == val2.size ());

    int length = val1.size ();
    dbitset result (length);
    bool carry = false;

    for (int i = 0; i < length; ++i)
      result[i] = fullAdder ((bool) val1[i], (bool) val2[i], carry);

    return result;
  }

  dbitset
  bitSetInc (dbitset val) {

    int length = val.size ();
    dbitset result (length);
    bool carry = false;

    for (int i = 0; i < length; ++i) {
      if (i == 0)
	result[i] = fullAdder ((bool) val[i], 1, carry);
      else if (carry)
	result[i] = fullAdder ((bool) val[i], 0, carry);
      else
	result[i] = val[i];
    }

    return result;
  }

  dbitset
  bitSetMul (dbitset val1, dbitset val2) {

    assert(val1.size () == val2.size ());

    int length = val1.size ();
    dbitset result (length);

    // we want to minimize the number of times we shift and add
    if (val1.count () < val2.count ()) {
      for (int i = 0; i < length; i++)
	if (val1[i])
	  result = bitSetAdd (result, val2 << i);
    }
    else {
      for (int i = 0; i < length; i++)
	if (val2[i])
	  result = bitSetAdd (result, val1 << i);
    }

    return result;
  }

  dbitset
  bitSetSub (dbitset val1, dbitset val2) {

    assert(val1.size () == val2.size ());

    int length = val1.size ();
    dbitset result (length);
    bool borrow = false;
    for (int i = 0; i < length; i++) {
      if (borrow) {
	if (val1[i]) {
	  result[i] = val2[i];
	  borrow = val2[i];
	}
	else {
	  result[i] = 1 - val2[i];
	  borrow = true;
	}

      }
      else {
	if (val1[i]) {
	  result[i] = 1 - val2[i];
	  borrow = false;
	}
	else {
	  result[i] = val2[i];
	  borrow = val2[i];
	}
      }
    }
    return result;
  }

  dbitset
  bitSetDec (dbitset val) {

    assert(val.any ());

    int length = val.size ();
    dbitset result (length);
    bool borrow = false;
    bool temp = false;
    for (int i = 0; i < length; i++) {
      temp = i == 0 ? 1 : 0;
      if (borrow) {
	if (val[i]) {
	  result[i] = temp;
	  borrow = temp;
	}
	else {
	  result[i] = 1 - temp;
	  borrow = true;
	}

      }
      else {
	if (val[i]) {
	  result[i] = 1 - temp;
	  borrow = false;
	}
	else {
	  result[i] = temp;
	  borrow = temp;
	}
      }
    }
    return result;
  }

  dbitset
  bitSetDiv (dbitset val1, dbitset val2, bool remainder) {

    assert(val1.size () == val2.size ());

    int length = val1.size ();
    dbitset q (length);
    dbitset r (length);

    q.reset ();
    r.reset ();

    if (val2 == q) {
      assert(0);
      return q;
    }

    if (val1 == q) {
      return q;
    }

    if (val1 == val2) {
      if (!remainder)
	q[0] = 1;
      return q;
    }

    r = val1;
    if (val1 < val2) {
      if (!remainder)
	return q;
      else
	return r;
    }

    // count significant digits in divisor and dividend
    unsigned int sig_val1 = 0;
    for (int i = length - 1; i >= 0; i--) {
      sig_val1 = i;
      if (val1[i])
	break;
    }

    unsigned int sig_val2 = 0;
    for (int i = length - 1; i >= 0; i--) {
      sig_val2 = i;
      if (val2[i])
	break;
    }

    // align the divisor with the dividend
    unsigned int n = (sig_val1 - sig_val2);
    val2 <<= n;
    // make sure the loop executes the right number of times
    n += 1;
    // long division algorithm, shift, and subtract
    while (n--) {
      // shift the quotient to the left
      if (val2 <= r) {
	// add a new digit to quotient
	q[n] = true;
	r = bitSetSub (r, val2);
      }
      // shift the divisor to the right
      val2 >>= 1;
    }

    if (remainder)
      return r;
    else
      return q;
  }


  /*
   * picking a random bitvector between lower and upper
   * algorithm:
   * diff = upper - lower
   * compute active_precision
   * pick a random vector with suff_precision
   * while (r_vector > diff) r_vector <- r_vector - diff;
   * return r_vector
   */
  dbitset
  bitSetRand (dbitset lower, dbitset upper) {

   assert(lower < upper);
   assert(lower.size () == upper.size ());
   int length = lower.size();

   dbitset diff = bitSetSub(upper, lower);
   int numPrefixZeros = 0;
   for (int i = length - 1; i >= 0; i--) {
     if (diff[i]) break;
     numPrefixZeros++;
   }


   int active_precision = length - numPrefixZeros;
   dbitset r_vector(length);
   std::default_random_engine generator;
   std::uniform_int_distribution<int> dist(0, 1);
   for (int i = 0; i < active_precision; i++) {
      r_vector[i] = dist(generator);
   }

   while(r_vector > diff)
     r_vector = bitSetSub(r_vector, diff);

   dbitset retValue = bitSetAdd(lower, r_vector);

    return retValue;
  }

}

