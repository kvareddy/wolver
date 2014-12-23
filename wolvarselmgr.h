/*
 * wolvarselmgr.h
 *
 *  Created on: Nov 16, 2014
 *      Author: user
 */

#ifndef WOLVARSELMGR_H_
#define WOLVARSELMGR_H_

#include "common.h"
#include <vector>

namespace wolver {

class WolVarSelStrategy;

class WolVarSelMgr {

public : //enums
  enum StrategyType
  {
    Random,
    NewStrategy
  };


public: //singleton
  static WolVarSelMgr& getInstance() {
    static WolVarSelMgr instance;
    return instance;
  }

private:
  WolVarSelMgr()
   : _strategy(NULL) {}
  ~WolVarSelMgr();
  WolVarSelMgr(WolVarSelMgr const&);
  void operator= (WolVarSelMgr const&);

public: //methods
 void setStrategy(StrategyType type, std::vector<WolNodeSptr> pool);
 WolNodeSptr pickNextVariable();


private: //methods


private: // data
 WolVarSelStrategy *_strategy;


};

}



#endif /* WOLVARSELMGR_H_ */
