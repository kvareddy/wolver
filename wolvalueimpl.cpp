#include "wolvalueimpl.h"
#include "wolvalue.h"
#include <boost/dynamic_bitset.hpp>

namespace wolver {

WolValue::WolConstType 
WolConstValueImpl::getConstType(){

   bool is_zero_present = false;
   bool is_one_present = false;
   int num_ones = 0;

   for (boost::dynamic_bitset<>::size_type i = 0; i < _value.size(); ++i) {
        if ((bool)_value[i] == true ) {
            is_one_present = true;
            num_ones++;
        }  
        if ((bool)_value[i] == false) {
            is_zero_present = true; 
        } 
   }
   
   if (_value.size() == 1 && is_one_present)
       return WolValue::WOL_SPECIAL_CONST_ONE_ONES;
   else if (!is_zero_present)
       return WolValue::WOL_SPECIAL_CONST_ONES;
   else if (!is_one_present)
       return WolValue::WOL_SPECIAL_CONST_ZERO;
   else if (num_ones == 1 && (bool)_value[0] == true)
       return WOL_SPECIAL_CONST_ONE; 
   else
       return WOL_SPECIAL_CONST_NONE;
}

WolValueSptr
WolBoolValueImpl::getNotOfExpr() {

   WolBoolValueImplSptr retValue(new WolBoolValueImpl(!_value));
   return retValue;
}

WolValueSptr
WolConstValueImpl::getNotOfExpr() {
   
   WolConstValueImplSptr retValue(new WolConstValueImpl(~_value));
   return retValue;

}

}
