/*
 * wolworklist.cpp
 *
 *  Created on: Sep 24, 2014
 *      Author: user
 */

#include "wolworklist.h"
#include "wollog.h"
#include "wolexp.h"
#include <functional>


namespace wolver {

std::size_t
expHashW::operator()(WolNodeSptr const& x) const
{
   return x->getId();
}

bool
expEqualToW::operator()(WolNodeSptr const& x, WolNodeSptr const& y) const
{
   return x->getId() == y->getId();
}


std::string
WolWorkList::str() {
	std::stringstream sstm;
	for (auto i : _workList) {
		sstm << " " << i->getId();
	}
	return sstm.str();
}

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
    DEBUG1_MSG << "Peforming implication on node " << i->getId();
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


