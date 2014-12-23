/*
 * wolvarselmgr.cpp
 *
 *  Created on: Nov 16, 2014
 *      Author: user
 */


#include "common.h"
#include "wolvarselmgr.h"
#include "wolvarselstrat.h"
#include "wolexp.h"
#include "wolvalue.h"
#include <cassert>

namespace wolver {


WolVarSelMgr::~WolVarSelMgr() {
  if (_strategy)
  delete _strategy;
}

void
WolVarSelMgr::setStrategy( StrategyType type,
                           std::vector<WolNodeSptr> pool) {

  if (type == WolVarSelMgr::Random) {
    _strategy = new WolVarSelRandStrategy(pool);
  }
  else {
    assert(0);
  }

}

WolNodeSptr
WolVarSelMgr::pickNextVariable() {
  return _strategy->pickNextVariable();
}


}
