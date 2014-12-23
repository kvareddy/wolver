#ifndef WOLMGR_H_INCLUDED
#define WOLMGR_H_INCLUDED

#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include "wolexp.h"
#include "wolstack.h"
#include "common.h"

namespace wolver {

using namespace std;

class WolValueFactory;
class WolEvalFactory;
class WolVarSelStrategy;

class WolMgr {

public: //singleton
   static WolMgr& getInstance()
   {
      static WolMgr instance;
      return instance;
   }

private:
   WolMgr()
    : _globalId(1),
     _valueFactory(NULL),
     _evalFactory(NULL){}
   ~WolMgr() {}
   WolMgr(WolMgr const&);
   void operator= (WolMgr const&);

public:  // methods
   void insertUniqueSliceExpr(WolNodeSptr exp){ _uniqSliceExpTable.insert(exp); }
   WolNodeSptr findSliceExpr(WolNodeSptr exp);
   void insertUniqueExpr(WolNodeSptr exp) {_uniqExpTable.insert(exp);}
   WolNodeSptr findUniqueExpr(WolNodeSptr exp);
   void insertConstExpr(WolNodeSptr exp) {_uniqConstTable[exp->getName()] = exp;}
   WolNodeSptr findConstExpr(std::string string);
   void insertIdExpr(WolNodeSptr exp);
   WolNodeSptr findExpr(int id);   
   void setWolValueFactory(WolValueFactory *valueFactory) {_valueFactory = valueFactory;}
   void setWolEvalFactory(WolEvalFactory *evalFactory) {_evalFactory = evalFactory;}
   WolValueFactory *getValueFactory() {return _valueFactory;}
   WolEvalFactory  *getEvalFactory() {return _evalFactory;}
   bool solve(nodeVec inputs, nodeVec outputs);

private: // methods
   void initializeOutputNodes(nodeVec outputs);

private: // data

struct sliceExpEqualTo
    : std::binary_function<WolNodeSptr, WolNodeSptr, bool>
{
   bool operator()(WolNodeSptr const& x, WolNodeSptr const& y) const
   {
      WolComplexNodeSptr x_comp = dynamic_pointer_cast<WolComplexNode>(x);
      WolComplexNodeSptr y_comp = dynamic_pointer_cast<WolComplexNode>(y);
      
      if(((x_comp->getChildren()[0]) != (y_comp->getChildren())[0]) ||
         (x_comp->getHighPrecision() != y_comp->getHighPrecision()) ||
         (y_comp->getLowPrecision() != y_comp->getLowPrecision()))
         return false;      
       
      return true;
   }
};

struct sliceExpHash
    : std::unary_function<WolNodeSptr, std::size_t>
{
   std::size_t operator()(WolNodeSptr const& x) const
   {
      std::size_t seed = 0;
      WolComplexNodeSptr x_comp = dynamic_pointer_cast<WolComplexNode>(x);
      seed = (size_t)((x_comp->getChildren())[0]->getId() + x_comp->getHighPrecision() +
              x_comp->getLowPrecision());   
      return seed;
   }
};

struct expEqualTo
    : std::binary_function<WolNodeSptr, WolNodeSptr, bool>
{
   bool operator()(WolNodeSptr const& x, WolNodeSptr const& y) const
   {
      WolComplexNodeSptr x_comp = dynamic_pointer_cast<WolComplexNode>(x);
      WolComplexNodeSptr y_comp = dynamic_pointer_cast<WolComplexNode>(y);
      
      if ((x_comp->getType() != y_comp->getType()) ||
          (x_comp->getArity() != y_comp->getArity()))
          return false;

      int arity = x_comp->getArity();
      for (int i = 0; i < arity; i++) 
      {
         if ((x_comp->getChildren())[i] != (y_comp->getChildren())[i])
             return false;
      }
      return true; 
   }
};

struct expHash
    : std::unary_function<WolNodeSptr, std::size_t>
{
   std::size_t operator()(WolNodeSptr const& x) const
   {
      std::size_t seed = 0;
      WolComplexNodeSptr x_comp = dynamic_pointer_cast<WolComplexNode>(x);

      int arity = x_comp->getArity();
      for (int i = 0; i < arity; i++) 
      {
         seed += (size_t)(x_comp->getChildren())[i]->getId();
      }
      return seed;
   }
};

std::unordered_map<std::string, WolNodeSptr> _uniqConstTable;
std::unordered_set<WolNodeSptr, sliceExpHash, sliceExpEqualTo> _uniqSliceExpTable;
std::unordered_set<WolNodeSptr, expHash, expEqualTo> _uniqExpTable;
std::unordered_map<int, WolNodeSptr> _idToExpMap;
int _globalId;
WolValueFactory *_valueFactory; 
WolEvalFactory  *_evalFactory;
WolStack _backtrackStack;


};

}

#endif
