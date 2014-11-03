/*
 * wolimplmgr.h
 *
 *  Created on: Sep 22, 2014
 *      Author: Vijay Anand Korthikanti
 *  Interface for the Implication Manager
 */

#ifndef WOLIMPLMGR_H_
#define WOLIMPLMGR_H_

#include <utility>
#include "common.h"

namespace wolver {

class WolGenEngine;
class WolImplEngine;
class WolValue;

class WolImplMgr {

public: //singleton
   static WolImplMgr& getInstance()
   {
      static WolImplMgr instance;
      return instance;
   }

private:
   WolImplMgr() { initializeImplicationEngine();}
   ~WolImplMgr();
   WolImplMgr(WolImplMgr const&);
   void operator= (WolImplMgr const&);

public: // methods
   std::pair<bool, nodeL> performImplications(WolNodeSptr node);
   std::pair<bool, nodeL> performImplications(nodeL &inputs, nodeL &outputs);
   WolValueSptr performGenarilization(WolNodeSptr node, WolValueSptr value);


private: // methods
  void initializeImplicationEngine();
  void collectStatistics();

private:
  WolImplEngine *_impEngine;
  WolGenEngine  *_genEngine;
  // statistics data for all implications

};


}



#endif /* WOLIMPLMGR_H_ */
