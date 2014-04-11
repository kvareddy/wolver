#ifndef WOLMGR_H_INCLUDED
#define WOLMGR_H_INCLUDED

#include "wolexp.h"
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

namespace wolver {

using namespace std;

class WolMgr {

public:
   WolMgr() {}
   ~WolMgr() {}

public:  // methods
   void insertUniqueSliceExpr(WolNode *exp){ _uniqSliceExpTable.insert(exp); }
   WolNode *findSliceExpr(WolNode *exp);
   void insertUniqueExpr(WolNode *exp) {_uniqExpTable.insert(exp);}
   WolNode *findUniqueExpr(WolNode *exp);
   void insertConstExpr(WolNode *exp) {_uniqConstTable[exp->getName()] = exp;}
   WolNode *findConstExpr(std::string string);
   void insertIdExpr(WolNode *exp);
   WolNode *findExpr(int id);

private: // methods


private: // data

struct sliceExpEqualTo
    : std::binary_function<WolNode *, WolNode *, bool>
{
   bool operator()(const WolNode* const& x, const WolNode* const& y) const
   {
      const WolComplexNode *x_comp = dynamic_cast<const WolComplexNode *>(x);
      const WolComplexNode *y_comp = dynamic_cast<const WolComplexNode *>(y);
      
      if(((x_comp->getChildren()[0]) != (y_comp->getChildren())[0]) ||
         (x_comp->getHighPrecision() != y_comp->getHighPrecision()) ||
         (y_comp->getLowPrecision() != y_comp->getLowPrecision()))
         return false;      
       
      return true;
   }
};

struct sliceExpHash
    : std::unary_function<WolNode *, std::size_t>
{
   std::size_t operator()(const WolNode* const& x) const
   {
      std::size_t seed = 0;
      const WolComplexNode *x_comp = dynamic_cast<const WolComplexNode *>(x);
      seed = (size_t)((x_comp->getChildren())[0]->getId() + x_comp->getHighPrecision() +
              x_comp->getLowPrecision());   
      return seed;
   }
};

struct expEqualTo
    : std::binary_function<WolNode *, WolNode *, bool>
{
   bool operator()(const WolNode* const& x, const WolNode* const& y) const
   {
      const WolComplexNode *x_comp = dynamic_cast<const WolComplexNode *>(x);
      const WolComplexNode *y_comp = dynamic_cast<const WolComplexNode *>(y);
      
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
    : std::unary_function<WolNode *, std::size_t>
{
   std::size_t operator()(const WolNode* const& x) const
   {
      std::size_t seed = 0;
      const WolComplexNode *x_comp = dynamic_cast<const WolComplexNode *>(x);

      int arity = x_comp->getArity();
      for (int i = 0; i < arity; i++) 
      {
         seed += (size_t)(x_comp->getChildren())[i]->getId();
      }
      return seed;
   }
};

std::unordered_map<std::string, WolNode *> _uniqConstTable;
std::unordered_set<WolNode *, sliceExpHash, sliceExpEqualTo> _uniqSliceExpTable;
std::unordered_set<WolNode *, expHash, expEqualTo> _uniqExpTable;
std::unordered_map<int, WolNode *> _idToExpMap;
int _globalId;
};

}

#endif
