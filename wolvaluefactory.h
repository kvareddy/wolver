#ifndef WOLVALUEFACTORY_H_INCLUDED
#define WOLVALUEFACTORY_H_INCLUDED

#include <memory>
#include "common.h"

namespace wolver {

using namespace std;

class WolValue;
class WolMgr;


class WolValueFactory {

public:
   WolValueFactory() {}
   virtual ~WolValueFactory() {}

public:
   virtual WolValueSptr makeConstValue(std::string name) = 0;
   virtual WolValueSptr makeComplexValue(int precision) = 0;
   

private:


};

}
#endif
