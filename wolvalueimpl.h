#ifndef WOLVALUEIMPL_H_INCLUDED
#define WOLVALUEIMPL_H_INCLUDED

#include <iostream>
#include <sstream>
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
   WolValueImpl(WolValueImplType type, int precision = 0)
         : _type(type)
	 , _prec(precision){}
   virtual ~WolValueImpl() {}
   
public: // functions
   virtual void print() = 0;
   virtual std::string getStringRep() = 0;
   virtual bool isConst() {return _type  == WOL_VALUE_IMPL_CONST_TYPE;}
   virtual bool isRange() {return _type == WOL_VALUE_IMPL_RANGE_TYPE;}
   virtual WolConstType getConstType() = 0;
   virtual WolValueImplType getImplType() {return _type;}
   virtual WolValueType getType() = 0;
   virtual WolValueSptr getNotValue() {return getNotValueInt();}
   virtual WolValueImplSptr getNotValueInt() = 0;
   virtual int getPrecision() {return _prec;}
   virtual WolValueImplSptr split(int position) = 0;
   virtual bool isSingleTon() = 0;
   virtual dbitset getLowValue() = 0;
   virtual dbitset getHighValue() = 0;
   virtual bool isFull() {assert(0); return false;}
   virtual bool containsValue(dbitset value) = 0;
   // assumption data has already been split at lower and upper positions
   virtual WolValueImplSptr getSlices(int upper, int lower) {assert(0); return nullptr;}
   virtual WolValueSptr getRandomValue() = 0;


private: //data
   WolValueImplType _type;

protected:
  int _prec;

};


class WolBoolValueImpl: public WolValueImpl,
			public std::enable_shared_from_this<WolBoolValueImpl>{

friend class WolEvalFactoryImpl;

public:
   WolBoolValueImpl(bool value) 
      : WolValueImpl(WOL_VALUE_IMPL_BOOL_TYPE, 1)
      , _value(value) {}
   ~WolBoolValueImpl() {}

public: 
  void print() {cout << _value;}
  std::string getStringRep() {return to_string(_value);}
  WolConstType getConstType() {assert(0); return (WolConstType)0;}
  WolValueType getType() {return WOL_VALUE_BOOL_TYPE;}
  WolValueImplSptr getNotValueInt();
  WolValueImplSptr split(int position) { assert(0); return nullptr;}
  bool isSingleTon() {return true;}
  dbitset getLowValue() {return dbitset(1,_value);}
  dbitset getHighValue() {return dbitset(1, _value);}
  bool containsValue(dbitset const_val) {assert(0); return false;}
  WolValueSptr getRandomValue() {return shared_from_this();}
  bool _value;
};


class WolConstValueImpl : public WolValueImpl,
			  public std::enable_shared_from_this<WolConstValueImpl> {

friend class WolEvalFactoryImpl;

public: //enums

public:
   WolConstValueImpl(std::string value)
      : WolValueImpl(WOL_VALUE_IMPL_CONST_TYPE, value.size())
      , _value(boost::dynamic_bitset<>(value)) {}
   WolConstValueImpl(boost::dynamic_bitset<> value)
      : WolValueImpl(WOL_VALUE_IMPL_CONST_TYPE, value.size())
      , _value(value) {}
   ~WolConstValueImpl() {}

public: // functions
   void print() {cout<< _value;}
   std::string getStringRep();
   WolConstType getConstType();
   WolValueType getType() {return WOL_VALUE_CONST_TYPE;}
   WolValueImplSptr split(int position);
   bool isSingleTon() {return true;}
   dbitset getLowValue() {return _value;}
   dbitset getHighValue() {return _value;}
   WolValueImplSptr getNotValueInt();
   bool containsValue(dbitset const_val)
     {return const_val == _value;}
   WolValueSptr getRandomValue() { return shared_from_this();}
private: //data
   dbitset _value;
};

class WolRangeValueImpl: public WolValueImpl {

friend class WolEvalFactoryImpl;

public:
   WolRangeValueImpl(int precision)
       : WolValueImpl(WOL_VALUE_IMPL_RANGE_TYPE, precision)
       , _prec(precision)
       , _lowValue(dbitset(_prec, 0))
       , _highValue(dbitset(_prec, (unsigned)-1)){}
   WolRangeValueImpl(dbitset low, dbitset high)
       : WolValueImpl(WOL_VALUE_IMPL_RANGE_TYPE, low.size())
       , _prec(low.size())
       , _lowValue(low)
       , _highValue(high) {}
   ~WolRangeValueImpl() {}

public: // functions
   void print() { cout << getStringRep(); }
   std::string getStringRep();

   WolConstType getConstType(){ assert(0); return (WolConstType)0; }
   WolValueType getType() {return WOL_VALUE_NO_CONST_TYPE;}
   WolValueImplSptr split(int position);
   int OptimalSplitPosition();
   bool isFull();
   bool checkConsistency();
   bool isSingleTon() { assert(checkConsistency()); return false;}
   dbitset getLowValue() {return _lowValue;}
   dbitset getHighValue() {return _highValue;}
   WolValueImplSptr getNotValueInt();
   bool containsValue(dbitset const_val) {
     return (_lowValue <= const_val) && (_highValue >= const_val);
   }
   WolValueSptr getRandomValue();
private: //data
   int _prec;
   dbitset _lowValue;
   dbitset _highValue;
};

class WolUnionValueImpl: public WolValueImpl {

friend class WolEvalFactoryImpl;

public:
  typedef std::vector<WolValueImplSptr>::iterator Iterator;

public:
   WolUnionValueImpl()
       : WolValueImpl(WOL_VALUE_IMPL_UNION_TYPE) {}
   ~WolUnionValueImpl() {}

public: // functions
   void print() {cout << getStringRep(); }
   std::string getStringRep();
   WolConstType getConstType(){assert(0); return (WolConstType)0; }
   WolValueType getType() {return WOL_VALUE_NO_CONST_TYPE;}
   WolValueImplSptr split(int position);
   void addValue(WolValueImplSptr value);
   WolValueImplSptr getValue(int index) {return _values[index];}
   int numValues() {return _values.size();}
   Iterator begin() {return _values.begin();}
   Iterator end() {return _values.end();}
   bool isSingleTon() { return false;}
   dbitset getLowValue();
   dbitset getHighValue();
   WolValueImplSptr getSlices(int upper, int lower);
   void optimize() {} // check if full range is present in the list
   WolValueImplSptr getNotValueInt();
   bool containsValue(dbitset const_val);
   WolValueSptr getRandomValue();

private: //data
   std::vector<WolValueImplSptr> _values;
};

class WolConcatValueImpl: public WolValueImpl,
                          public std::enable_shared_from_this<WolConcatValueImpl> {

friend class WolEvalFactoryImpl;

public:
  typedef std::vector<WolValueImplSptr>::iterator Iterator;
public:
   WolConcatValueImpl()
       : WolValueImpl(WOL_VALUE_IMPL_CONCAT_TYPE) {}
   ~WolConcatValueImpl() {}

public: // functions
   void print() { cout << getStringRep();}
   std::string getStringRep();
   WolConstType getConstType(){ assert(0); return (WolConstType)0; }
   WolValueType getType() {return WOL_VALUE_NO_CONST_TYPE;}
   WolValueImplSptr split(int position);
   void addValue(WolValueImplSptr value);
   WolValueImplSptr getValue(int index) {return _values[index];}
   int numValues() {return _values.size();}
   Iterator begin() {return _values.begin();}
   Iterator end() {return _values.end();}
   bool isSingleTon() {return false;}
   dbitset getLowValue();
   dbitset getHighValue();
   WolValueImplSptr getSlices(int upper, int lower);
   WolValueImplSptr getNotValueInt();
   bool containsValue(dbitset const_val);
   WolValueSptr getRandomValue();
private: //data
   std::vector<WolValueImplSptr> _values;
};


}
#endif
