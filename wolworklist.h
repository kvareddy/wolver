/*
 * wolworklist.h
 *
 *  Created on: Sep 22, 2014
 *      Author: user
 */

#ifndef WOLWORKLIST_H_
#define WOLWORKLIST_H_

#include <unordered_set>
#include "common.h"

namespace wolver {

class WolNode;

struct expHashW
      : std::unary_function<WolNodeSptr, std::size_t>
  {
     std::size_t operator()(WolNodeSptr const& x) const;
  };

  struct expEqualToW
      : std::binary_function<WolNodeSptr, WolNodeSptr, bool>
  {
    bool operator()(WolNodeSptr const& x, WolNodeSptr const& y) const;

  };

class WolWorkList {

public:
  typedef std::unordered_set<WolNodeSptr>::iterator Iterator;
public:
  WolWorkList() {}
  ~WolWorkList() {}

public: //methods
   void addNode(WolNodeSptr node);
   void addNodes(nodeL &nodes);
   void addNodes(std::vector<WolNodeSptr> &nodes);
   void setImplyFlag();
   void unsetImplyFlag();
   bool isEmpty() {return _workList.empty();}
   bool performImplication(nodeL &changes, WolWorkList &wl);
   WolWorkList getNeighbors();
   std::string str();
//   Iterator begin() { return _workList.begin();}
//   Iterator end() {return _workList.end();}

private: // methods


private: //data
  std::unordered_set<WolNodeSptr, expHashW, expEqualToW > _workList;

};

}


#endif /* WOLWORKLIST_H_ */
