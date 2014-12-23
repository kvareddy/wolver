/*
 * wolutil.h
 *
 *  Created on: Jul 31, 2014
 *      Author: user
 */

#ifndef WOLUTIL_H_
#define WOLUTIL_H_

#include "common.h"
#include <boost/dynamic_bitset.hpp>

namespace wolver
{

  dbitset
  createSubsetOfDynamicBitSet (dbitset &source, int low, int high);
  bool
  fullAdder (bool b1, bool b2, bool& carry);
  dbitset
  bitSetAdd (dbitset val1, dbitset val2);
  dbitset
  bitSetMul (dbitset val1, dbitset val2);
  dbitset
  bitSetSub (dbitset val1, dbitset val2);
  dbitset
  bitSetDiv (dbitset val1, dbitset val2, bool remainder);
  dbitset
  bitSetInc (dbitset val);
  dbitset
  bitSetDec (dbitset val);
  dbitset
  bitSetRand (dbitset lower, dbitset upper);

}
#endif /* WOLUTIL_H_ */
