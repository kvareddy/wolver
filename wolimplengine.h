/*
 * wolimplengine.h
 *
 *  Created on: Sep 22, 2014
 *      Author: user
 */

#ifndef WOLIMPLENGINE_H_
#define WOLIMPLENGINE_H_

#include <utility>
#include "common.h"
#include "wolworklist.h"

namespace wolver {


class WolImplEngine {

public:
  WolImplEngine() {}
  virtual ~WolImplEngine() {}

public: //methods
   virtual std::pair<bool,nodeL> performImplication(WolNodeSptr node) = 0;
   virtual std::pair<bool,nodeL> performImplication(nodeL &inputs, nodeL &outputs) = 0;
   void revertChanges(nodeL &changes);

private: //methods


private: // data
   // common statistics

};


class WolWLImplEngine : public WolImplEngine{

public:
  WolWLImplEngine() {}
  ~WolWLImplEngine() {}

public: //methods
   std::pair<bool, nodeL> performImplication(WolNodeSptr node);
   std::pair<bool, nodeL> performImplication(nodeL &inputs, nodeL &outputs);

private: //methods
   std::pair<bool, nodeL> performImplicationInt(WolWorkList &wl);


private: // data
   // worklist engine specific data

};












}
#endif /* WOLIMPLENGINE_H_ */
