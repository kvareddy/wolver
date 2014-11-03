#include "wolmgr.h"
#include "wolexp.h"
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <string>

namespace wolver
{

  using namespace std;

  WolNodeSptr
  WolMgr::findSliceExpr (WolNodeSptr exp) {
    std::unordered_set<WolNodeSptr>::iterator get = _uniqSliceExpTable.find (
	exp);
    if (get == _uniqSliceExpTable.end ())
      return NULL;
    return *get;
  }

  WolNodeSptr
  WolMgr::findUniqueExpr (WolNodeSptr exp) {
    std::unordered_set<WolNodeSptr>::iterator get = _uniqExpTable.find (exp);
    if (get == _uniqExpTable.end ())
      return NULL;
    return *get;
  }

  WolNodeSptr
  WolMgr::findConstExpr (std::string name) {
    std::unordered_map<std::string, WolNodeSptr>::iterator get =
	_uniqConstTable.find (name);
    if (get == _uniqConstTable.end ())
      return NULL;
    return get->second;
  }

  void
  WolMgr::insertIdExpr (WolNodeSptr exp) {
    _globalId++;
    _idToExpMap[_globalId] = exp;
    exp->setId (_globalId);
  }

  WolNodeSptr
  WolMgr::findExpr (int id) {
    std::unordered_map<int, WolNodeSptr>::iterator get = _idToExpMap.find (id);
    if (get == _idToExpMap.end ())
      return NULL;
    return get->second;
  }

}
