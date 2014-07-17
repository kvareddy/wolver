#ifndef WOLVALUEIMPL_H_INCLUDED
#define WOLVALUEIMPL_H_INCLUDED

#include<iostream>
#include "wolvalue.h"
#include "common.h"
#include <boost/dynamic_bitset.hpp>

namespace wolver
{

using namespace std;

class WolValueImpl : public WolValue {

friend class WolEvalFactoryImpl;

public: //Types

   enum WolValueImplType {
      WOL_VALUE_IMPL_INVALID_TYPE = 0,
      WOL_VALUE_IMPL_CONST_TYPE = 1,
      WOL_VALUE_IMPL_BOOL_TYPE = 2,
      WOL_VALUE_IMPL_RANGE_TYPE = 3,
      WOL_VALUE_IMPL_UNION_TYPE = 4,
      WOL_VALUE_IMPL_CONCAT_TYPE = 5
   }; 


public:
   WolValueImpl(WolValueImplType type)
         : _type(type) {}
   virtual ~WolValueImpl() {}
   
public: // functions
   virtual void print() = 0;
   virtual std::string getStringRep() = 0;
   virtual bool isConst() {return _type  == WOL_VALUE_IMPL_CONST_TYPE;} 
   virtual WolConstType getConstType() = 0;
   virtual WolValueImplType getImplType() = 0;
   virtual WolValueType getType() = 0;
   virtual WolValueSptr getNotOfExpr() {assert(0); return nullptr;}  
 
private: //data
   WolValueImplType _type;

};


class WolBoolValueImpl: public WolValueImpl {

friend class WolEvalFactoryImpl;

public:
   WolBoolValueImpl(bool value) 
      : WolValueImpl(WOL_VALUE_IMPL_BOOL_TYPE) 
      , _value(value) {}
   ~WolBoolValueImpl() {}

public: 
  void print() {cout << _value;}
  std::string getStringRep() {return to_string(_value);}
  WolConstType getConstType() {assert(0); return (WolConstType)0;}
  WolValueType getType() {return WOL_VALUE_BOOL_TYPE;}
  WolValueImplType getImplType() {return WOL_VALUE_IMPL_BOOL_TYPE;}
  WolValueSptr getNotOfExpr(); 

private: 
  bool _value;
};


class WolConstValueImpl : public WolValueImpl {

friend class WolEvalFactoryImpl;

public: //enums

public:
   WolConstValueImpl(std::string value)
      : WolValueImpl(WOL_VALUE_IMPL_CONST_TYPE)
      , _value(boost::dynamic_bitset<>(value)) {}
   WolConstValueImpl(boost::dynamic_bitset<> value)
      : WolValueImpl(WOL_VALUE_IMPL_CONST_TYPE)
      , _value(value) {}
   ~WolConstValueImpl() {}

public: // functions
   void print() {cout<< _value;}
   std::string getStringRep() { std::string retValue; boost::to_string(_value, retValue); return retValue;}
   WolConstType getConstType();
   WolValueType getType() {return WOL_VALUE_CONST_TYPE;}
   WolValueImplType getImplType() {return WOL_VALUE_IMPL_CONST_TYPE;}
   WolValueSptr getNotOfExpr();

private: //data
   boost::dynamic_bitset<> _value;
};


class WolRangeValueImpl: public WolValueImpl {

friend class WolEvalFactoryImpl;

public:
   WolRangeValueImpl() 
       : WolValueImpl(WOL_VALUE_IMPL_RANGE_TYPE) {}
   ~WolRangeValueImpl() {}

public: // funcitons
   void print();
   std::string getStringRep();
   WolConstType getConstType(){ assert(0); return (WolConstType)0; }
   WolValueType getType() {return WOL_VALUE_NO_CONST_TYPE;}
   WolValueImplType getImplType() {return WOL_VALUE_IMPL_RANGE_TYPE;}

private: //data

};


class WolUnionValueImpl: public WolValueImpl {

friend class WolEvalFactoryImpl;

public:
   WolUnionValueImpl()
       : WolValueImpl(WOL_VALUE_IMPL_UNION_TYPE) {}
   ~WolUnionValueImpl() {}

public: // funcitons
   void print();
   std::string getStringRep();
   WolConstType getConstType(){assert(0); return (WolConstType)0; }
   WolValueType getType() {return WOL_VALUE_NO_CONST_TYPE;}
   WolValueImplType getImplyType() {return WOL_VALUE_IMPL_UNION_TYPE;}

private: //data

};

class WolConcatValueImpl: public WolValueImpl {

friend class WolEvalFactoryImpl;

public:
   WolConcatValueImpl()
       : WolValueImpl(WOL_VALUE_IMPL_CONCAT_TYPE) {}
   ~WolConcatValueImpl() {}

public: // functions
   void print();
   std::string getStringRep();
   WolConstType getConstType(){ assert(0); return (WolConstType)0; }
   WolValueType getType() {return WOL_VALUE_NO_CONST_TYPE;}
   WolValueImplType getImplyType() {return WOL_VALUE_IMPL_CONCAT_TYPE;}


private: //data

};


}
#endif
