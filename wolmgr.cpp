#include "wolmgr.h"
#include "wolexp.h"
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <string>

namespace wolver {

using namespace std;


WolNode *WolMgr::findSliceExpr(WolNode *exp) {
   std::unordered_set<WolNode *>::iterator get = _uniqSliceExpTable.find(exp);
   if (get == _uniqSliceExpTable.end()) return NULL;
   return *get;
}

WolNode *WolMgr::findUniqueExpr(WolNode *exp) {
   std::unordered_set<WolNode *>::iterator get = _uniqExpTable.find(exp);
   if (get == _uniqExpTable.end()) return NULL;
   return *get;
}

WolNode *WolMgr::findConstExpr(std::string name) {
   std::unordered_map<std::string, WolNode *>::iterator get = _uniqConstTable.find(name);
   if (get == _uniqConstTable.end()) return NULL;
   return get->second;
}

void WolMgr::insertIdExpr(WolNode *exp) {
   _globalId++;
   _idToExpMap[_globalId] = exp;
   exp->setId(_globalId);
}

WolNode *WolMgr::findExpr(int id) {
   std::unordered_map<int, WolNode *>::iterator get = _idToExpMap.find(id);
   if (get == _idToExpMap.end()) return NULL;
   return get->second;
}


}
