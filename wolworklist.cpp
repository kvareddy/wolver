/*
 * wolworklist.cpp
 *
 *  Created on: Sep 24, 2014
 *      Author: user
 */

#include "wolworklist.h"
#include <functional>


namespace wolver {

void
WolWorkList::addNode(WolNodeSptr node) {
  _workList.insert(node);
}

void
WolWorkList::addNodes(nodeL &nodes) {
  _workList.insert(nodes.begin(), nodes.end());

}

void
WolWorkList::addNodes(std::vector<WolNodeSptr> &nodes) {
  _workList.insert(nodes.begin(), nodes.end());
}

void
WolWorkList::setImplyFlag() {
  std::for_each(_workList.begin(), _workList.end(),
                std::mem_fn(&WolNode::setImplyFlag));

}

void
WolWorkList::unsetImplyFlag() {
  std::for_each(_workList.begin(), _workList.end(),
                  std::mem_fn(&WolNode::unsetImplyFlag));

}

bool
WolWorkList::performImplication(nodeL &changes, WolWorkList &wl) {

  for(auto i : _workList) {
    int output = i->performImplication();

    // conflict
    if (output == -1) return false;

    if (output == 1) {
      // enable backward implication
      if (i->getChild(0)) wl.addNode(i->getChild(0));
      if (i->getChild(1)) wl.addNode(i->getChild(1));
      if (i->getChild(2)) wl.addNode(i->getChild(2));
    }
    else if (output == 2) {
      // enable forward and backward implication
      std::vector<WolNodeSptr> neighbors;
      neighbors = i->getNeighbors();
      wl.addNodes(neighbors);
      changes.insert(i);
    }
    else {
      assert(output == 0);
    }
  }
  return true;
}

WolWorkList
WolWorkList::getNeighbors() {
  WolWorkList retValue;
  std::vector<WolNodeSptr> neighbors;
  for(auto i : _workList) {
    neighbors = i->getNeighbors();
    retValue.addNodes(neighbors);
  }
  return retValue;
}

}


