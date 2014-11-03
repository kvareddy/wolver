/*
 * wolgenengine.h
 *
 *  Created on: Oct 22, 2014
 *      Author: user
 */

#ifndef WOLGENENGINE_H_
#define WOLGENENGINE_H_

#include "common.h"

namespace wolver {


class WolGenEngine {

class WolValue;
class WolNode;

public:
  WolGenEngine() {}
  virtual ~WolGenEngine() {}

public: //methods
virtual WolValueSptr performGenerlization(WolNodeSptr node, WolValueSptr Value) = 0;

private: //methods


private: // data


};


class WolSimpleGenEngine : public WolGenEngine{

public:
  WolSimpleGenEngine() {}
  ~WolSimpleGenEngine() {}

public: //methods
  WolValueSptr performGenerlization(WolNodeSptr node, WolValueSptr Value);

private: //methods


private: // data


};

}

#endif /* WOLGENENGINE_H_ */
