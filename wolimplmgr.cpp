/*
 * wolimplmgr.cpp
 *
 *  Created on: Sep 24, 2014
 *      Author: user
 */

#include "wolimplmgr.h"
#include "common.h"
#include "wolexp.h"
#include "wolimplengine.h"
#include "wolgenengine.h"
#include "wolvalue.h"

namespace wolver {

void
WolImplMgr::initializeImplicationEngine() {
  _impEngine = new WolWLImplEngine();
  _genEngine = new WolSimpleGenEngine();
}

WolImplMgr::~WolImplMgr() {
   delete _impEngine;
   delete _genEngine;
}

std::pair<bool, nodeL>
WolImplMgr::performImplications(WolNodeSptr node) {
    return _impEngine->performImplication(node);
}

std::pair<bool, nodeL>
WolImplMgr::performImplications(nodeL &inputs, nodeL &outputs) {
  implResult res = _impEngine->performImplication(inputs, outputs);
  if (res.first == false) {
    // cleanup the stored values at the nodes
    for( auto i : res.second) i->setSValue(nullptr);
  }
  return res;
}

WolValueSptr
WolImplMgr::performGenarilization(WolNodeSptr node, WolValueSptr value) {
  return _genEngine->performGenerlization(node, value);
}


}
