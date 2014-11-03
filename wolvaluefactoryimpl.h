#ifndef WOLVALUEFACTORYIMPL_H_INCLUDED
#define WOLVALUEFACTORYIMPL_H_INCLUDED

#include <memory>
#include <string>
#include "wolvaluefactory.h"
#include "common.h"
#include "wolvalue.h"

namespace wolver {

using namespace std;

class WolValue;
class WolValueImpl;
class WolMgr;


class WolValueFactoryImpl: public WolValueFactory {

public: 
   WolValueFactoryImpl () {}
   virtual ~WolValueFactoryImpl () {}

public :
   WolValueSptr makeConstValue(std::string name);
   WolValueSptr makeComplexValue(int precision);

private:


};

}
#endif
