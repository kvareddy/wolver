#ifndef WOLVALUE_H_INCLUDED
#define WOLVALUE_H_INCLUDED

#include<iostream>
#include "common.h"

namespace wolver
{

using namespace std;

class WolValue;

class WolValue {

friend class WolEvalFactory;

public: //Types

  enum WolValueType {
      WOL_VALUE_BOOL_TYPE,
      WOL_VALUE_CONST_TYPE,
      WOL_VALUE_NO_CONST_TYPE
  };   

   enum WolConstType {
      WOL_SPECIAL_CONST_ZERO = 0,
      WOL_SPECIAL_CONST_ONE = 1,
      WOL_SPECIAL_CONST_ONES = 2,
      WOL_SPECIAL_CONST_ONE_ONES = 3,
      WOL_SPECIAL_CONST_NONE = 4
   };

public:
   WolValue() {}
   virtual ~WolValue() {}
   
public: // functions
   virtual void print() = 0;
   virtual std::string getStringRep() = 0;
   virtual bool isConst() = 0;
   virtual WolConstType getConstType() = 0;
   virtual WolValueSptr getNotOfExpr() = 0;
   virtual WolValueType getType() = 0; 
   
private: //data

};



}

#endif
