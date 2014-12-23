/*
 * wolvarselstrat.h
 *
 *  Created on: Nov 9, 2014
 *      Author: user
 */

#ifndef WOLVARSELSTRAT_H_
#define WOLVARSELSTRAT_H_

#include "common.h"
#include <vector>

namespace wolver {

class WolVarSelStrategy {

public:
  WolVarSelStrategy(std::vector<WolNodeSptr> &pool)
  : _nodePool(pool) {}
  virtual ~WolVarSelStrategy() {};

public: //methods
  virtual WolNodeSptr pickNextVariable() = 0;

protected: //data
   std::vector<WolNodeSptr> _nodePool;
};


class WolVarSelRandStrategy : public WolVarSelStrategy {

public: // constructor and destructor
  WolVarSelRandStrategy(std::vector<WolNodeSptr> &pool)
  : WolVarSelStrategy(pool) {}
  ~WolVarSelRandStrategy() {}

public: // methods
  WolNodeSptr pickNextVariable();

private: //data

};

}

#endif /* WOLVARSELSTRAT_H_ */
