/*
 * wolimplengine.cpp
 *
 *  Created on: Sep 24, 2014
 *      Author: user
 */

#include <cassert>
#include "wolimplengine.h"
#include "wolexp.h"
#include "wolworklist.h"
#include "wollog.h"
#include "wolmgr.h"

namespace wolver {


  void
  WolImplEngine::revertChanges(nodeL &changes) {

    for(WolNodeSptr i : changes) {
      WolValueSptr svalue = i->getSValue();
      assert(i->getSValue() != nullptr);

      i->setValue(svalue);
      i->setSValue(nullptr);
    }

  }

  std::pair<bool, nodeL>
  WolWLImplEngine::performImplication(WolNodeSptr node) {
    //construct the work list
    WolWorkList initialNodes;
    initialNodes.addNode(node);

    // call the internal function
    return performImplicationInt(initialNodes);

  }

  std::pair<bool, nodeL>
  WolWLImplEngine::performImplication(nodeL &inputs, nodeL &outputs) {

    WolWorkList initialNodes;
    initialNodes.addNodes(inputs);
    initialNodes.addNodes(outputs);

    return performImplicationInt(initialNodes);

  }

  std::pair<bool, nodeL>
  WolWLImplEngine::performImplicationInt(WolWorkList &wl){

    nodeL changes;
    WolWorkList prev = wl;
    WolWorkList curr = prev.getNeighbors();
    prev.setImplyFlag();

    while(true) {
      WolMgr::getInstance().printgv();
      WolWorkList next;
      DEBUG_MSG << "Performing Implication on " << curr.str();
      bool output = curr.performImplication(changes, next);
      prev.unsetImplyFlag();

      if (!output) {
    	  revertChanges(changes);
    	  return std::make_pair(false, changes);
      }
      if (next.isEmpty()) break;

      curr.setImplyFlag();
      prev = curr;
      curr = next;
    }

    return std::make_pair(true, changes);
  }
}



