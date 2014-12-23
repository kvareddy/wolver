/*
 * wolstack.cpp
 *
 *  Created on: Dec 14, 2014
 *      Author: user
 */

#include "wolstack.h"
#include "wolexp.h"
#include "wolevalfactory.h"
#include "wolmgr.h"
#include <cassert>

namespace wolver {


  void
  WolStackEntry::populateImplications(nodeL &list) {

    for (WolNodeSptr i : list) {
      WolValueSptr storedValue = i->getSValue();
      i->setSValue(nullptr);

      _implications.push_back(nodeValue(i, storedValue));
    }
  }


  void
  WolStackEntry::restoreImplications() {

    for (nodeValue i : _implications) {
      assert(i.first->getSValue() == nullptr);
      i.first->setValue(i.second);
    }

  }

  void
  WolStack::pushDecisionNode(WolNodeSptr node) {

    WolStackEntry newEntry(node->getValue(), node);
    _stack.push(newEntry);
  }

  void
  WolStack::storeImplications(nodeL &list) {
    _stack.top().populateImplications(list);
  }


  WolValueSptr
  WolStack::backtrackOnCurrentVariable(WolValueSptr adjustValue) {

    WolEvalFactory *evalFactory = WolMgr::getInstance().getEvalFactory();

    // restore Implications (if done)
    _stack.top().restoreImplications();

    // subtraction : activeValue = activeValue - adjustValue
    WolValueSptr activeValue = _stack.top().getActiveValue();
    WolValueSptr newValue = evalFactory->evalDiff(activeValue, adjustValue);
    _stack.top().updateActiveValue(newValue);

    return newValue;
  }

  void
  WolStack::backtrackToNextVariable() {
    WolStackEntry top = _stack.top();
    top.getNode()->setValue(top.getSotredValue());
    _stack.pop();
  }

}

