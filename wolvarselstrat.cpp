/*
 * wolvarselstrat.cpp
 *
 *  Created on: Nov 9, 2014
 *      Author: Vijay Anand Korthikanti
 */

#include "common.h"
#include "wolexp.h"
#include "wolvalue.h"
#include "wolvarselstrat.h"

namespace wolver {


 WolNodeSptr
 WolVarSelRandStrategy::pickNextVariable() {

   for (auto i : _nodePool) {
     if (!i->getValue()->isSingleTon())
       return i;
   }
   return nullptr;
 }

}



