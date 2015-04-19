#ifndef WOLMGR_H_CLUDED
#define WOLMGR_H_INCLUDED

#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include <sstream>
#include "common.h"

namespace wolver {

using namespace std;

class WolValueFactory;
class WolEvalFactory;
class WolVarSelStrategy;
class WolStack;
class WolNode;

struct sliceExpHash
    : std::unary_function<WolNodeSptr, std::size_t>
{
   std::size_t operator()(WolNodeSptr const& x) const;
};

struct sliceExpEqualTo
    : std::binary_function<WolNodeSptr, WolNodeSptr, bool>
{
    bool operator()(WolNodeSptr const& x, WolNodeSptr const& y) const;
};

struct expEqualTo
    : std::binary_function<WolNodeSptr, WolNodeSptr, bool>
{
   bool operator()(WolNodeSptr const& x, WolNodeSptr const& y) const;
};

struct expHash
    : std::unary_function<WolNodeSptr, std::size_t>
{
   std::size_t operator()(WolNodeSptr const& x) const;
};

class WolMgr {

public: //singleton
   static WolMgr& getInstance()
   {
      static WolMgr instance;
      return instance;
   }

private:
   WolMgr();
   ~WolMgr();
   WolMgr(WolMgr const&);
   void operator= (WolMgr const&);

public:  // methods
   void insertUniqueSliceExpr(WolNodeSptr exp){ _uniqSliceExpTable.insert(exp); }
   WolNodeSptr findSliceExpr(WolNodeSptr exp);
   void insertUniqueExpr(WolNodeSptr exp) {_uniqExpTable.insert(exp);}
   WolNodeSptr findUniqueExpr(WolNodeSptr exp);
   void insertConstExpr(WolNodeSptr exp);
   WolNodeSptr findConstExpr(std::string string);
   void insertIdExpr(WolNodeSptr exp);
   WolNodeSptr findExpr(int id);   
   void setWolValueFactory(WolValueFactory *valueFactory) {_valueFactory = valueFactory;}
   void setWolEvalFactory(WolEvalFactory *evalFactory) {_evalFactory = evalFactory;}
   WolValueFactory *getValueFactory() {return _valueFactory;}
   WolEvalFactory  *getEvalFactory() {return _evalFactory;}
   bool solve(nodeVec inputs, nodeVec outputs);
   std::ostringstream& getOutputStream() { return _os;}
   void printgv();
   void setOutputNodes(nodeVec outputsVec);

private: // methods
   bool initializeOutputNodes(nodeVec outputs);

private: // data





std::unordered_map<std::string, WolNodeSptr> _uniqConstTable;
std::unordered_set<WolNodeSptr, sliceExpHash, sliceExpEqualTo> _uniqSliceExpTable;
std::unordered_set<WolNodeSptr, expHash, expEqualTo> _uniqExpTable;
std::unordered_map<int, WolNodeSptr> _idToExpMap;
int _globalId;
WolValueFactory *_valueFactory; 
WolEvalFactory  *_evalFactory;
WolStack *_backtrackStack;
nodeVec _outputs;
std::ostringstream  _os;


};

#define OUTPUT_MSG WolMgr::getInstance().getOutputStream()
//#define OUTPUT_MSG std::cout
}

#endif
