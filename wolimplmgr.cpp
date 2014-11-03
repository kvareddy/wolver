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
    return _impEngine->performImplication(inputs, outputs);

}

WolValueSptr
WolImplMgr::performGenarilization(WolNodeSptr node, WolValueSptr value) {
  return _genEngine->performGenerlization(node, value);
}


}
