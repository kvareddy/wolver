#include "wolvaluefactoryimpl.h"
#include "wolvalueimpl.h"
#include "wolvalue.h"
#include "common.h"

namespace wolver
{

  WolValueSptr
  WolValueFactoryImpl::makeConstValue (std::string name) {

    WolConstValueImplSptr retValue (new WolConstValueImpl (name));
    return retValue;
  }

  WolValueSptr
  WolValueFactoryImpl::makeComplexValue(int precision) {

    WolRangeValueImplSptr retValue (new WolRangeValueImpl(precision));
    return nullptr;
  }

}
