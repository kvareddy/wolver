#include "wolvalueimpl.h"
#include "wolevalfactoryimpl.h"
#include <boost/dynamic_bitset.hpp>
#include "common.h"
#include "wolutil.h"
#include "wolmgr.h"
#include <memory>
#include <iostream>

namespace wolver
{

  using namespace std;

  typedef pair<WolValueImpl::WolValueImplType,
               WolValueImpl::WolValueImplType> typePair;

//-----------------------------------------------------------------------------
// TOP LEVEL EVALUATION FUNCTIONS

  WolValueSptr
  WolEvalFactoryImpl::evalOp (evalMap &map, WolValueSptr val1,
			      WolValueSptr val2) {

    WolValueImplSptr val_impl1 = dynamic_pointer_cast<WolValueImpl> (val1);
    WolValueImplSptr val_impl2 = dynamic_pointer_cast<WolValueImpl> (val2);

    return evalOpInt(map, val_impl1, val_impl2);
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalOpInt (evalMap &map, WolValueImplSptr val1,
                                 WolValueImplSptr val2) {

    WolValueImpl::WolValueImplType type1 = val1->getImplType ();
    WolValueImpl::WolValueImplType type2 = val2->getImplType ();

    evalFunctionPtr evalfun = lookup (map, type1, type2);

    return (this->*evalfun) (val1, val2);
  }

  WolValueSptr
  WolEvalFactoryImpl::evalAnd (WolValueSptr val1, WolValueSptr val2) {
    return evalOp (_andEvalMap, val1, val2);
  }

  WolValueSptr
  WolEvalFactoryImpl::evalBeq (WolValueSptr val1, WolValueSptr val2) {
    return evalOp (_beqEvalMap, val1, val2);
  }

  WolValueSptr
  WolEvalFactoryImpl::evalAdd (WolValueSptr val1, WolValueSptr val2) {
    return evalOp (_addEvalMap, val1, val2);
  }

  WolValueSptr
  WolEvalFactoryImpl::evalMul (WolValueSptr val1, WolValueSptr val2) {
    return evalOp (_mulEvalMap, val1, val2);
  }

  WolValueSptr
  WolEvalFactoryImpl::evalUlt (WolValueSptr val1, WolValueSptr val2) {
    WolValueImplSptr val_impl1 = dynamic_pointer_cast<WolValueImpl> (val1);
    WolValueImplSptr val_impl2 = dynamic_pointer_cast<WolValueImpl> (val2);

    return evalUltInt(val_impl1, val_impl2);
  }


  WolValueSptr
  WolEvalFactoryImpl::evalSll (WolValueSptr val1, WolValueSptr val2) {
    return evalOp (_sllEvalMap, val1, val2);
  }

  WolValueSptr
  WolEvalFactoryImpl::evalSrl (WolValueSptr val1, WolValueSptr val2) {
    return evalOp (_srlEvalMap, val1, val2);
  }

  WolValueSptr
  WolEvalFactoryImpl::evalUdiv (WolValueSptr val1, WolValueSptr val2) {
    return evalOp (_udivEvalMap, val1, val2);
  }

  WolValueSptr
  WolEvalFactoryImpl::evalUrem (WolValueSptr val1, WolValueSptr val2) {
    return evalOp (_uremEvalMap, val1, val2);
  }

  WolValueSptr
  WolEvalFactoryImpl::evalConcat (WolValueSptr val1, WolValueSptr val2) {
    return evalOp (_concatEvalMap, val1, val2);
  }

  WolValueSptr
  WolEvalFactoryImpl::evalSplice (WolValueSptr val, int upper, int lower) {
    WolValueImplSptr val_impl = dynamic_pointer_cast<WolValueImpl> (val);

    return evalSpliceInt(val_impl, upper, lower);
  }

  WolValueSptr
  WolEvalFactoryImpl::evalUnion (WolValueSptr val1, WolValueSptr val2) {
    return evalOp (_unionEvalMap, val1, val2);
  }

  WolValueSptr
  WolEvalFactoryImpl::evalIntersection (WolValueSptr val1, WolValueSptr val2) {
    return evalOp (_intersectionEvalMap, val1, val2);
  }

  WolValueSptr
  WolEvalFactoryImpl::evalCompliment (WolValueSptr val) {
    WolValueImplSptr val_impl = dynamic_pointer_cast<WolValueImpl> (val);

    return evalComplimentInt(val_impl);
  }

  WolValueSptr
  WolEvalFactoryImpl::evalDiff (WolValueSptr val1, WolValueSptr val2) {
	  WolValueSptr compliment = evalCompliment(val2);
	  if (compliment)
		  return evalIntersection(val1, compliment);
	  else
		  return nullptr;
  }

  WolValueSptr
  WolEvalFactoryImpl::evalCond(WolValueSptr cond, WolValueSptr val1, WolValueSptr val2) {
    WolValueImplSptr cond_impl = dynamic_pointer_cast<WolValueImpl> (cond);
    WolValueImplSptr val1_impl = dynamic_pointer_cast<WolValueImpl> (val1);
    WolValueImplSptr val2_impl = dynamic_pointer_cast<WolValueImpl> (val2);
    return evalCondInt(cond_impl, val1_impl, val2_impl);
  }

  WolValueSptr
  WolEvalFactoryImpl::evalUltB(WolValueSptr op_val, WolValueSptr operand_val, bool implyLeft){

    WolValueImplSptr op_val_impl = dynamic_pointer_cast<WolValueImpl> (op_val);
    WolValueImplSptr operand_val_impl = dynamic_pointer_cast<WolValueImpl> (operand_val);

    return evalUltBInt(op_val_impl, operand_val_impl, implyLeft);
  }

  WolValueSptr
  WolEvalFactoryImpl::evalAndB(WolValueSptr op_val, WolValueSptr operand_val) {
     return evalOp(_andBEvalMap, op_val, operand_val);
  }

  WolValueSptr
  WolEvalFactoryImpl::evalBeqB(WolValueSptr op_val, WolValueSptr operand_val){

    WolValueImplSptr op_val_impl = dynamic_pointer_cast<WolValueImpl> (op_val);
    WolValueImplSptr operand_val_impl = dynamic_pointer_cast<WolValueImpl> (operand_val);

    return evalBeqBInt(op_val_impl, operand_val_impl);
  }

  WolValueSptr
  WolEvalFactoryImpl::evalConcatB(WolValueSptr op_val, WolValueSptr operand_val, bool implyLeft){

    WolValueImplSptr op_val_impl = dynamic_pointer_cast<WolValueImpl> (op_val);
    WolValueImplSptr operand_val_impl = dynamic_pointer_cast<WolValueImpl> (operand_val);

    return evalConcatBInt(op_val_impl, operand_val_impl, implyLeft);
  }

  WolValueSptr
  WolEvalFactoryImpl::evalSpliceB(WolValueSptr op_val, int upper, int lower, int width) {

    WolValueImplSptr op_val_impl = dynamic_pointer_cast<WolValueImpl> (op_val);
    return evalSpliceBInt(op_val_impl, upper, lower, width);
  }

  // position = 0 => backward implication for bool condition
  // position = 1 => backward implication for if branch
  // position = 2 => backward implication for else branch
  WolValueSptr
  WolEvalFactoryImpl::evalCondB(WolValueSptr op_val, WolValueSptr operand_val1,
                                WolValueSptr operand_val2, int position) {

    WolValueImplSptr op_impl = dynamic_pointer_cast<WolValueImpl> (op_val);
    WolValueImplSptr operand1_impl = dynamic_pointer_cast<WolValueImpl> (operand_val1);
    WolValueImplSptr operand2_impl = dynamic_pointer_cast<WolValueImpl> (operand_val2);

    return evalCondBInt(op_impl, operand1_impl, operand2_impl, position);

  }

  WolValueImplSptr
   WolEvalFactoryImpl::evalAndInt (WolValueImplSptr val1, WolValueImplSptr val2) {
     return evalOpInt (_andEvalMap, val1, val2);
   }

   WolValueImplSptr
   WolEvalFactoryImpl::evalBeqInt (WolValueImplSptr val1, WolValueImplSptr val2) {
     return evalOpInt (_beqEvalMap, val1, val2);
   }

   WolValueImplSptr
   WolEvalFactoryImpl::evalAddInt (WolValueImplSptr val1, WolValueImplSptr val2) {
     return evalOpInt (_addEvalMap, val1, val2);
   }

   WolValueImplSptr
   WolEvalFactoryImpl::evalMulInt (WolValueImplSptr val1, WolValueImplSptr val2) {
     return evalOpInt (_mulEvalMap, val1, val2);
   }

   WolValueImplSptr
   WolEvalFactoryImpl::evalUltInt (WolValueImplSptr val1, WolValueImplSptr val2) {

     if (val1->getHighValue() < val2->getLowValue()) {
       WolBoolValueImplSptr retValue = makeBoolImpl(1);
       return retValue;
     }
     else if (val2->getHighValue() <= val1->getLowValue()) {
       WolBoolValueImplSptr retValue = makeBoolImpl(0);
       return retValue;
     }
     else {
       WolRangeValueImplSptr retValue = makeRangeImpl(1);
       return retValue;
     }

     assert(0);
     return nullptr;
   }

   WolValueImplSptr
   WolEvalFactoryImpl::evalSllInt (WolValueImplSptr val1, WolValueImplSptr val2) {
     return evalOpInt (_sllEvalMap, val1, val2);
   }

   WolValueImplSptr
   WolEvalFactoryImpl::evalSrlInt (WolValueImplSptr val1, WolValueImplSptr val2) {
     return evalOpInt (_srlEvalMap, val1, val2);
   }

   WolValueImplSptr
   WolEvalFactoryImpl::evalUdivInt (WolValueImplSptr val1, WolValueImplSptr val2) {
     return evalOpInt (_udivEvalMap, val1, val2);
   }

   WolValueImplSptr
   WolEvalFactoryImpl::evalUremInt (WolValueImplSptr val1, WolValueImplSptr val2) {
     return evalOpInt (_uremEvalMap, val1, val2);
   }

   WolValueImplSptr
   WolEvalFactoryImpl::evalConcatInt (WolValueImplSptr val1, WolValueImplSptr val2) {
     return evalOpInt (_concatEvalMap, val1, val2);
   }

   WolValueImplSptr
   WolEvalFactoryImpl::evalSpliceInt (WolValueImplSptr val, int upper, int lower) {

     evalSpliceFunctionPtr evalfun = lookup (_spliceEvalMap,
 					    val->getImplType ());

     return (this->*evalfun) (val, upper, lower);
   }

   WolValueImplSptr
   WolEvalFactoryImpl::evalUnionInt (WolValueImplSptr val1, WolValueImplSptr val2) {
     return evalOpInt (_unionEvalMap, val1, val2);
   }

   WolValueImplSptr
   WolEvalFactoryImpl::evalIntersectionInt (WolValueImplSptr val1, WolValueImplSptr val2) {
     return evalOpInt (_intersectionEvalMap, val1, val2);
   }

   WolValueImplSptr
   WolEvalFactoryImpl::evalComplimentInt (WolValueImplSptr val) {

     eval1FunctionPtr evalfun = lookup (_complimentEvalMap,
 				       val->getImplType ());

     return (this->*evalfun) (val);
   }

   WolValueImplSptr
   WolEvalFactoryImpl::evalCondInt(WolValueImplSptr cond, WolValueImplSptr val1,
                                   WolValueImplSptr val2) {

     assert(cond->getPrecision() == 1);

     if (cond->isFull()) return evalUnionInt(val1, val2);

     assert(cond->isSingleTon());
     assert(cond->isConst());

     if (cond->getConstType() == WolValue::WOL_SPECIAL_CONST_ZERO)
       return val2;
     else
       return val1;

     assert(0);
     return nullptr;
   }

   WolValueImplSptr
   WolEvalFactoryImpl::evalUltBInt(WolValueImplSptr op_val, WolValueImplSptr operand_val,
                                   bool implyLeft){

     int prec = operand_val->getPrecision();

     if (op_val->isRange()) {
       WolRangeValueImplSptr retValue = makeRangeImpl(prec);
       return retValue;
     }

     dbitset lowValue = operand_val->getLowValue();
     dbitset highValue = operand_val->getHighValue();
     if (implyLeft) {
        if (op_val->getConstType() == WolValueImpl::WOL_SPECIAL_CONST_ZERO) {
           WolRangeValueImplSptr retValue = makeRangeImpl2(lowValue, dbitset(prec, (unsigned) -1));
           return retValue;
        }
        else {
          assert(op_val->getConstType() == WolValueImpl::WOL_SPECIAL_CONST_ONE);
          WolRangeValueImplSptr retValue = makeRangeImpl2(dbitset(prec, 0), bitSetDec(highValue));
          return retValue;
        }
     }
     else {
       if (op_val->getConstType() == WolValueImpl::WOL_SPECIAL_CONST_ZERO) {
	 WolRangeValueImplSptr retValue = makeRangeImpl2(dbitset(prec, 0), highValue);
	 return retValue;
       }
       else {
	 assert(op_val->getConstType() == WolValueImpl::WOL_SPECIAL_CONST_ONE);
	 WolRangeValueImplSptr retValue = makeRangeImpl2(bitSetInc(lowValue), dbitset(prec, (unsigned)-1));
	 return retValue;
       }
     }

     assert(0);
     return nullptr;
   }

   WolValueImplSptr
   WolEvalFactoryImpl::evalAndBInt(WolValueImplSptr op_val, WolValueImplSptr operand_val) {

     return evalOpInt(_andBEvalMap, op_val, operand_val);
   }

   WolValueImplSptr
   WolEvalFactoryImpl::evalBeqBInt(WolValueImplSptr op_val, WolValueImplSptr operand_val) {

     int prec = operand_val->getPrecision();

     if (op_val->isRange()) {
       WolRangeValueImplSptr retValue = makeRangeImpl(prec);
       return retValue;
     }
     if (op_val->getConstType() == WolValueImpl::WOL_SPECIAL_CONST_ZERO) {
       if (operand_val->isConst()) {
	   return evalComplimentInt(operand_val);
       }
       else {
	 WolRangeValueImplSptr retValue = makeRangeImpl(prec);
	 return retValue;
       }
     }
     else {
       assert(op_val->getConstType() == WolValueImpl::WOL_SPECIAL_CONST_ONE);
       return operand_val;
     }

     assert(0);
     return nullptr;
   }

   WolValueImplSptr
   WolEvalFactoryImpl::evalConcatBInt(WolValueImplSptr op_val, WolValueImplSptr operand_val,
                                      bool implyLeft) {

     int op_prec = op_val->getPrecision();
     int operand_prec = operand_val->getPrecision();

     assert(op_prec > operand_prec);
     if (implyLeft) {
         WolValueImplSptr splitRightValue =  evalSpliceInt(op_val, operand_prec -1, 0);
         if (evalIntersectionInt(splitRightValue, operand_val) == nullptr) return nullptr;
         else return evalSpliceInt(op_val, op_prec -  1 , operand_prec);
     }
     else {
         WolValueImplSptr splitLeftValue = evalSpliceInt(op_val, op_prec - 1, operand_prec);
         if (evalIntersectionInt(splitLeftValue, operand_val) == nullptr) return nullptr;
         else return evalSpliceInt(op_val, operand_prec - 1, 0);
     }

     assert(0);
     return nullptr;
   }

   WolValueImplSptr
   WolEvalFactoryImpl::evalSpliceBInt(WolValueImplSptr op_val, int upper, int lower, int width) {

     if (upper - lower + 1 == width) return op_val;

     WolConcatValueImplSptr retValue = makeConcatImpl();
     if (lower != 0) {
       WolRangeValueImplSptr left = makeRangeImpl(lower);
       retValue->addValue(left);
     }

     retValue->addValue(op_val);

     if (upper != width - 1) {
       WolRangeValueImplSptr right = makeRangeImpl(width - upper -1);
       retValue->addValue(right);
     }

     assert(retValue->getPrecision() == width);
     return retValue;
   }

   // position = 0 => backward implication for Bool condition
   // position = 1 => backward implication for if branch
   // position = 2 => backward implication for else branch
   WolValueImplSptr
   WolEvalFactoryImpl::evalCondBInt(WolValueImplSptr op_val, WolValueImplSptr operand_val1,
                                    WolValueImplSptr operand_val2, int position) {

     if (position == 0) {
        WolValueImplSptr int1 = evalIntersectionInt(op_val, operand_val1);
        WolValueImplSptr int2 = evalIntersectionInt(op_val, operand_val2);

        if (!int1 && !int2) return nullptr;
        else if (int1 && !int2) {
          WolValueImplSptr retValue = makeConstImpl(dbitset(1, 1));
          return retValue;
        }
        else if (!int1 && int2) {
          WolValueImplSptr retValue = makeConstImpl(dbitset(1,0));
          return retValue;
        }
        else {
          WolValueImplSptr retValue = makeRangeImpl(1);
          return retValue;
        }
     }
     else if (position == 1) {
       assert(operand_val1->getPrecision() == 1);

       if (operand_val1->getConstType() == WolValue::WOL_SPECIAL_CONST_ZERO) {
	 int precision = operand_val2->getPrecision();
	 WolValueImplSptr retValue = makeRangeImpl(precision);
	 return retValue;
       }
       else {
	 return op_val;
       }
     }
     else {
       assert(position == 2);
       assert(operand_val1->getPrecision() == 1);

       if (operand_val1->getConstType() == WolValue::WOL_SPECIAL_CONST_ONE) {
	 int precision = operand_val2->getPrecision();
	 WolValueImplSptr retValue = makeRangeImpl(precision);
	 return retValue;
       }
       else {
	 return op_val;
       }
     }

     assert(0);
     return nullptr;
   }

//-------------------------------------------------------------------------
// LOOK UP FUNCTIONS

  evalFunctionPtr
  WolEvalFactoryImpl::lookup (evalMap &emap,
			      WolValueImpl::WolValueImplType type1,
			      WolValueImpl::WolValueImplType type2) {

    evalMap::iterator mapEntry = emap.find (std::make_pair (type1, type2));

    if (mapEntry == emap.end ())
      assert(0);

    return mapEntry->second;
  }

  eval1FunctionPtr
  WolEvalFactoryImpl::lookup (eval1Map &emap,
			      WolValueImpl::WolValueImplType type) {

    eval1Map::iterator mapEntry = emap.find (type);

    if (mapEntry == emap.end ())
      assert(0);

    return mapEntry->second;
  }

  evalSpliceFunctionPtr
  WolEvalFactoryImpl::lookup (evalSpliceMap &emap,
			      WolValueImpl::WolValueImplType type) {

    evalSpliceMap::iterator mapEntry = emap.find (type);

    if (mapEntry == emap.end ())
      assert(0);

    return mapEntry->second;
  }

//-----------------------------------------------------------------------------
// Map Intialization routines

  void
  WolEvalFactoryImpl::initializeAllMaps () {
    initializeAndEvalMap ();
    initializeBeqEvalMap ();
    initializeAddEvalMap ();
    initializeMulEvalMap ();
    initializeSllEvalMap ();
    initializeSrlEvalMap ();
    initializeUdivEvalMap ();
    initializeUremEvalMap ();
    initializeConcatEvalMap ();
    initializeSpliceEvalMap ();
    initializeUnionEvalMap ();
    initializeIntersectionEvalMap ();
    initializeComplimentMap ();

    initializeAndBEvalMap();

  }

  void
  WolEvalFactoryImpl::initializeAndEvalMap () {

    _andEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE),
	    &WolEvalFactoryImpl::evalBoolAndBool));

    _andEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalConstAndConst));
    _andEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalConstAndRange));
    _andEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalConstAndUnion));
    _andEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalConstAndConcat));
    _andEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalRangeAndConst));
    _andEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalRangeAndRange));
    _andEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalRangeAndUnion));
    _andEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalRangeAndConcat));
    _andEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalUnionAndConst));
    _andEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalUnionAndRange));
    _andEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalUnionAndUnion));
    _andEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalUnionAndConcat));
    _andEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalConcatAndConst));
    _andEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalConcatAndRange));
    _andEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalConcatAndUnion));
    _andEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalConcatAndConcat));
  }

  void
  WolEvalFactoryImpl::initializeBeqEvalMap () {
    _beqEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE),
	    &WolEvalFactoryImpl::evalBoolBeqBool));
    _beqEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalConstBeqConst));
    _beqEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalNonConstBeq));
    _beqEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalNonConstBeq));
    _beqEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalNonConstBeq));
    _beqEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalNonConstBeq));
    _beqEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalNonConstBeq));
    _beqEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalNonConstBeq));
    _beqEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalNonConstBeq));
    _beqEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalNonConstBeq));
    _beqEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalNonConstBeq));
    _beqEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalNonConstBeq));
    _beqEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalNonConstBeq));
    _beqEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalNonConstBeq));
    _beqEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalNonConstBeq));
    _beqEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalNonConstBeq));
    _beqEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalNonConstBeq));
  }

  void
  WolEvalFactoryImpl::initializeAddEvalMap () {

    _addEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE),
	    &WolEvalFactoryImpl::evalBoolAddBool));
    _addEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalConstAddConst));
    _addEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalConstAddRange));
    _addEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalConstAddUnion));
    _addEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalConstAddConcat));
    _addEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalRangeAddConst));
    _addEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalRangeAddRange));
    _addEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalRangeAddUnion));
    _addEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalRangeAddConcat));
    _addEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalUnionAddConst));
    _addEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalUnionAddRange));
    _addEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalUnionAddUnion));
    _addEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalUnionAddConcat));
    _addEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalConcatAddConst));
    _addEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalConcatAddRange));
    _addEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalConcatAddUnion));
    _addEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalConcatAddConcat));

  }

  void
  WolEvalFactoryImpl::initializeMulEvalMap () {
    _mulEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE),
	    &WolEvalFactoryImpl::evalBoolMulBool));
    _mulEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalConstMulConst));
    _mulEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalConstMulRange));
    _mulEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalConstMulUnion));
    _mulEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalConstMulConcat));
    _mulEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalRangeMulConst));
    _mulEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalRangeMulRange));
    _mulEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalRangeMulUnion));
    _mulEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalRangeMulConcat));
    _mulEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalUnionMulConst));
    _mulEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalUnionMulRange));
    _mulEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalUnionMulUnion));
    _mulEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalUnionMulConcat));
    _mulEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalConcatMulConst));
    _mulEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalConcatMulRange));
    _mulEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalConcatMulUnion));
    _mulEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalConcatMulConcat));

  }

  void
  WolEvalFactoryImpl::initializeSllEvalMap () {

    _sllEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE),
	    &WolEvalFactoryImpl::evalBoolSllBool));
    _sllEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalConstSllConst));
    _sllEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalConstSllRange));
    _sllEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalConstSllUnion));
    _sllEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalConstSllConcat));
    _sllEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalRangeSllConst));
    _sllEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalRangeSllRange));
    _sllEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalRangeSllUnion));
    _sllEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalRangeSllConcat));
    _sllEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalUnionSllConst));
    _sllEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalUnionSllRange));
    _sllEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalUnionSllUnion));
    _sllEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalUnionSllConcat));
    _sllEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalConcatSllConst));
    _sllEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalConcatSllRange));
    _sllEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalConcatSllUnion));
    _sllEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalConcatSllConcat));

  }

  void
  WolEvalFactoryImpl::initializeSrlEvalMap () {
    _srlEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE),
	    &WolEvalFactoryImpl::evalBoolSrlBool));
    _srlEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalConstSrlConst));
    _srlEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalConstSrlRange));
    _srlEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalConstSrlUnion));
    _srlEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalConstSrlConcat));
    _srlEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalRangeSrlConst));
    _srlEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalRangeSrlRange));
    _srlEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalRangeSrlUnion));
    _srlEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalRangeSrlConcat));
    _srlEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalUnionSrlConst));
    _srlEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalUnionSrlRange));
    _srlEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalUnionSrlUnion));
    _srlEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalUnionSrlConcat));
    _srlEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalConcatSrlConst));
    _srlEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalConcatSrlRange));
    _srlEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalConcatSrlUnion));
    _srlEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalConcatSrlConcat));

  }

  void
  WolEvalFactoryImpl::initializeUdivEvalMap () {
    _udivEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE),
	    &WolEvalFactoryImpl::evalBoolUdivBool));
    _udivEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalConstUdivConst));
    _udivEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalConstUdivRange));
    _udivEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalConstUdivUnion));
    _udivEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalConstUdivConcat));
    _udivEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalRangeUdivConst));
    _udivEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalRangeUdivRange));
    _udivEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalRangeUdivUnion));
    _udivEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalRangeUdivConcat));
    _udivEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalUnionUdivConst));
    _udivEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalUnionUdivRange));
    _udivEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalUnionUdivUnion));
    _udivEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalUnionUdivConcat));
    _udivEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalConcatUdivConst));
    _udivEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalConcatUdivRange));
    _udivEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalConcatUdivUnion));
    _udivEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalConcatUdivConcat));

  }

  void
  WolEvalFactoryImpl::initializeUremEvalMap () {
    _uremEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE),
	    &WolEvalFactoryImpl::evalBoolUremBool));
    _uremEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalConstUremConst));
    _uremEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalConstUremRange));
    _uremEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalConstUremUnion));
    _uremEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalConstUremConcat));
    _uremEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalRangeUremConst));
    _uremEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalRangeUremRange));
    _uremEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalRangeUremUnion));
    _uremEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalRangeUremConcat));
    _uremEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalUnionUremConst));
    _uremEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalUnionUremRange));
    _uremEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalUnionUremUnion));
    _uremEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalUnionUremConcat));
    _uremEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalConcatUremConst));
    _uremEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalConcatUremRange));
    _uremEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalConcatUremUnion));
    _uremEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalConcatUremConcat));

  }

  void
  WolEvalFactoryImpl::initializeConcatEvalMap () {
    _concatEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE),
	    &WolEvalFactoryImpl::evalBoolConcatBool));
    _concatEvalMap.insert (
	evalMap::value_type (
	    std::make_pair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
			    WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalConstConcatConst));
    _concatEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalNonConstConcat));
    _concatEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalNonConstConcat));
    _concatEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalNonConstConcat));
    _concatEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalNonConstConcat));
    _concatEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalNonConstConcat));
    _concatEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalNonConstConcat));
    _concatEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalNonConstConcat));
    _concatEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalNonConstConcat));
    _concatEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalNonConstConcat));
    _concatEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalNonConstConcat));
    _concatEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalNonConstConcat));
    _concatEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalNonConstConcat));
    _concatEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalNonConstConcat));
    _concatEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalNonConstConcat));
    _concatEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalNonConstConcat));

  }

  void
  WolEvalFactoryImpl::initializeSpliceEvalMap () {
    _spliceEvalMap.insert (
	evalSpliceMap::value_type (WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
				   &WolEvalFactoryImpl::evalBoolSplice));
    _spliceEvalMap.insert (
	evalSpliceMap::value_type (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
				   &WolEvalFactoryImpl::evalConstSplice));
    _spliceEvalMap.insert (
	evalSpliceMap::value_type (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
				   &WolEvalFactoryImpl::evalRangeSplice));
    _spliceEvalMap.insert (
	evalSpliceMap::value_type (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
				   &WolEvalFactoryImpl::evalUnionSplice));
    _spliceEvalMap.insert (
	evalSpliceMap::value_type (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
				   &WolEvalFactoryImpl::evalConcatSplice));
  }

  void
  WolEvalFactoryImpl::initializeUnionEvalMap () {
    _unionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE),
	    &WolEvalFactoryImpl::evalBoolUnionBool));
    _unionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalConstUnionConst));
    _unionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalConstUnionRange));
    _unionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalConstUnionUnion));
    _unionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalConstUnionConcat));
    _unionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalRangeUnionConst));
    _unionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalRangeUnionRange));
    _unionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalRangeUnionUnion));
    _unionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalRangeUnionConcat));
    _unionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalUnionUnionConst));
    _unionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalUnionUnionRange));
    _unionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalUnionUnionUnion));
    _unionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalUnionUnionConcat));
    _unionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalConcatUnionConst));
    _unionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalConcatUnionRange));
    _unionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalConcatUnionUnion));
    _unionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalConcatUnionConcat));

  }

  void
  WolEvalFactoryImpl::initializeIntersectionEvalMap () {
    _intersectionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE),
	    &WolEvalFactoryImpl::evalBoolIntersectionBool));
    _intersectionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalConstIntersectionConst));
    _intersectionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalConstIntersectionRange));
    _intersectionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalConstIntersectionUnion));
    _intersectionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalConstIntersectionConcat));
    _intersectionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalRangeIntersectionConst));
    _intersectionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalRangeIntersectionRange));
    _intersectionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalRangeIntersectionUnion));
    _intersectionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalRangeIntersectionConcat));
    _intersectionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalUnionIntersectionConst));
    _intersectionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalUnionIntersectionRange));
    _intersectionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalUnionIntersectionUnion));
    _intersectionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalUnionIntersectionConcat));
    _intersectionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	    &WolEvalFactoryImpl::evalConcatIntersectionConst));
    _intersectionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	    &WolEvalFactoryImpl::evalConcatIntersectionRange));
    _intersectionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	    &WolEvalFactoryImpl::evalConcatIntersectionUnion));
    _intersectionEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
		      WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	    &WolEvalFactoryImpl::evalConcatIntersectionConcat));
  }

  void
  WolEvalFactoryImpl::initializeComplimentMap () {
    _complimentEvalMap.insert (
	eval1Map::value_type (WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
			      &WolEvalFactoryImpl::evalBoolCompliment));
    _complimentEvalMap.insert (
	eval1Map::value_type (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
			      &WolEvalFactoryImpl::evalConstCompliment));
    _complimentEvalMap.insert (
	eval1Map::value_type (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
			      &WolEvalFactoryImpl::evalRangeCompliment));
    _complimentEvalMap.insert (
	eval1Map::value_type (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
			      &WolEvalFactoryImpl::evalUnionCompliment));
    _complimentEvalMap.insert (
	eval1Map::value_type (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
			      &WolEvalFactoryImpl::evalConcatCompliment));
  }

  void
  WolEvalFactoryImpl::initializeAndBEvalMap () {

    _andBEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
	              WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE),
	              &WolEvalFactoryImpl::evalBackBoolAndBool));

    _andBEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
	              WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	              &WolEvalFactoryImpl::evalBackConstAndConst));
    _andBEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
	              WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	              &WolEvalFactoryImpl::evalBackConstAndRange));
    _andBEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
	              WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	              &WolEvalFactoryImpl::evalBackConstAndUnion));
    _andBEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
	              WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	              &WolEvalFactoryImpl::evalBackConstAndConcat));
    _andBEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
	              WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	              &WolEvalFactoryImpl::evalBackRangeAndConst));
    _andBEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
	              WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	              &WolEvalFactoryImpl::evalBackRangeAndRange));
    _andBEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
	              WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	              &WolEvalFactoryImpl::evalBackRangeAndUnion));
    _andBEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
	              WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	              &WolEvalFactoryImpl::evalBackRangeAndConcat));
    _andBEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
	              WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	              &WolEvalFactoryImpl::evalBackUnionAndConst));
    _andBEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
	              WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	              &WolEvalFactoryImpl::evalBackUnionAndRange));
    _andBEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
	              WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	              &WolEvalFactoryImpl::evalBackUnionAndUnion));
    _andBEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
	              WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	              &WolEvalFactoryImpl::evalBackUnionAndConcat));
    _andBEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
	              WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE),
	              &WolEvalFactoryImpl::evalBackConcatAndConst));
    _andBEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
	              WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE),
	              &WolEvalFactoryImpl::evalBackConcatAndRange));
    _andBEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
	              WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE),
	              &WolEvalFactoryImpl::evalBackConcatAndUnion));
    _andBEvalMap.insert (
	evalMap::value_type (
	    typePair (WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
	              WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE),
	              &WolEvalFactoryImpl::evalBackConcatAndConcat));
  }


//------------------------------------------------------------------------------------------------
// AND EVAlUATION ROUTINES

  WolValueImplSptr
  WolEvalFactoryImpl::evalBoolAndBool (WolValueImplSptr val1,
				       WolValueImplSptr val2) {

    WolBoolValueImplSptr bool_val1 = dBoolCast(val1);
    WolBoolValueImplSptr bool_val2 = dBoolCast(val2);

    bool result = bool_val1->_value & bool_val2->_value;

    auto retValue = makeBoolImpl(result);
    return retValue;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstAndConst (WolValueImplSptr val1,
					 WolValueImplSptr val2) {

    WolConstValueImplSptr const_val1 = dConstCast(val1);
    WolConstValueImplSptr const_val2 = dConstCast(val2);

    boost::dynamic_bitset<> result = const_val1->_value & const_val2->_value;

    auto retValue = makeConstImpl(result);
    return retValue;
  }

  //TODO: Graphical Representation of data structures for debugging purpose.
  WolValueImplSptr
  WolEvalFactoryImpl::evalConstAndRange (WolValueImplSptr val1,
					 WolValueImplSptr val2) {

    WolConstValueImplSptr const_val = dConstCast(val1);
    WolRangeValueImplSptr range_val = dRangeCast(val2);

    int size = const_val->_value.size();
    if (!range_val->isFull()){
      int position = range_val->OptimalSplitPosition();
      if (position == 0) position = size/2;
      auto rangeSplit = range_val->split(position);
      auto constSplit = const_val->split(position);

      return evalAndInt(constSplit, rangeSplit);
    }
    else {
      // 110011 ---> xx00xx
      dbitset val = const_val->_value;
      if (val.none()) {
	auto result = makeConstImpl(val);
	return result ;
      }
      else {
	WolConcatValueImplSptr concatVal = makeConcatImpl();
	int i = 0, j = 0;
	int element = val[0];
        for (int k = 1; k < size; k++) {
          if ((val[k] == element)) {
            j++;
            if (k < (size - 1)) continue;
          }

            //[i,j]
          if (element == 0) {
            WolConstValueImplSptr constVal = makeConstImpl(dbitset(j - i + 1, 0));
            concatVal->addValue(constVal);
          }
          else {
            WolRangeValueImplSptr rangeVal = makeRangeImpl(j- i + 1);
            concatVal->addValue(rangeVal);
          }

          i = k;
          j = k;
          element = val[k];

        }
        return concatVal;
      }
    }

    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstAndUnion (WolValueImplSptr val1,
					 WolValueImplSptr val2) {

    WolConstValueImplSptr const_val = dConstCast(val1);
    WolUnionValueImplSptr union_val = dUnionCast(val2);

    WolUnionValueImplSptr result = makeUnionImpl ();
    for (auto it = union_val->begin(); it != union_val->end(); it++) {
      WolValueImplSptr andValue = evalAndInt(const_val, *it);
      result->addValue(andValue);
    }

    return result;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstAndConcat (WolValueImplSptr val1,
					  WolValueImplSptr val2) {

    WolConstValueImplSptr const_val = dConstCast(val1);
    WolConcatValueImplSptr concat_val = dConcatCast(val2);

    WolConcatValueImplSptr result = makeConcatImpl();
    dbitset value = const_val->_value;

    int offset = const_val->getPrecision();
    for (auto it = concat_val->begin(); it != concat_val->end(); it++) {
      int prec = (*it)->getPrecision();
      dbitset subValue = createSubsetOfDynamicBitSet(value, offset - prec, offset - 1);

      WolConstValueImplSptr subConst = makeConstImpl(subValue);
      WolValueImplSptr andValue = evalAndInt(subConst, *it);
      result->addValue(andValue);
      offset -= prec;
    }

    return result;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeAndRange (WolValueImplSptr val1,
					 WolValueImplSptr val2) {

    assert(val1->getPrecision() >= 1);
    assert(val2->getPrecision() >= 1);


    WolRangeValueImplSptr range1 = dRangeCast(val1);
    WolRangeValueImplSptr range2 = dRangeCast(val2);

    assert(range1->getPrecision() != 1 || range1->isFull());
    assert(range2->getPrecision() != 2 || range2->isFull());

    if (range1->isFull() && range2->isFull())
    	return val1;

    int position = min(range1->OptimalSplitPosition(),
                       range2->OptimalSplitPosition());
    if (position == 0) position = range1->getPrecision()/2;

    auto range1Split = range1->split(position);
    auto range2Split = range2->split(position);

    return evalAndInt(range1Split, range2Split);
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeAndUnion (WolValueImplSptr val1,
					 WolValueImplSptr val2) {

    WolRangeValueImplSptr range_val = dRangeCast(val1);
    WolUnionValueImplSptr union_val = dUnionCast(val2);

    WolUnionValueImplSptr result = makeUnionImpl ();
    for (auto it = union_val->begin(); it != union_val->end(); it++) {
      WolValueImplSptr andValue = evalAndInt(range_val, *it);
      result->addValue(andValue);
    }

    return result;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeAndConcat (WolValueImplSptr val1,
					  WolValueImplSptr val2) {

    WolRangeValueImplSptr range_val = dRangeCast(val1);
    WolConcatValueImplSptr concat_val = dConcatCast(val2);

    WolConcatValueImplSptr result = makeConcatImpl();

    assert(concat_val->begin() != concat_val->end());
    int position = (*concat_val->begin())->getPrecision();
    WolValueImplSptr rangeSplit = range_val->split(position);

    return evalAndInt(rangeSplit, concat_val);

  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionAndUnion (WolValueImplSptr val1,
					 WolValueImplSptr val2) {

    WolUnionValueImplSptr union1 = dUnionCast(val1);
    WolUnionValueImplSptr union2 = dUnionCast(val2);

    WolUnionValueImplSptr result = makeUnionImpl();
    for (auto it1 = union1->begin(); it1 != union1->end(); it1++) {
      for (auto it2 = union2->begin(); it2 != union2->end(); it2++) {
         WolValueImplSptr andValue = evalAndInt(*it1, *it2);
         result->addValue(andValue);
      }
    }

    return result;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionAndConcat (WolValueImplSptr val1,
					  WolValueImplSptr val2) {

    WolUnionValueImplSptr union_val = dUnionCast(val1);
    WolConcatValueImplSptr concat_val = dConcatCast(val2);
    WolUnionValueImplSptr result = makeUnionImpl ();

    for (auto it = union_val->begin(); it != union_val->end(); it++) {
      WolValueImplSptr andValue = evalAndInt(*it, concat_val);
      result->addValue(andValue);
    }

    return result;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatAndConcat (WolValueImplSptr val1,
					   WolValueImplSptr val2) {
    WolConcatValueImplSptr concat1 = dConcatCast(val1);
    WolConcatValueImplSptr concat2 = dConcatCast(val2);

    assert(concat1->numValues() > 1);
    assert(concat2->numValues() > 1);

    int firstPrecision1 = concat1->getValue(0)->getPrecision();
    int firstPrecision2 = concat2->getValue(0)->getPrecision();


    if (firstPrecision1 < firstPrecision2) {
      WolValueImplSptr split = concat2->split(firstPrecision1);
      return evalAndInt(concat1, split);
    }
    else if (firstPrecision1 > firstPrecision2) {
      WolValueImplSptr split = concat1->split(firstPrecision2);
      return evalAndInt(split, concat2);

    }
    else {
      WolValueImplSptr left1 = concat1->getValue(0);
      WolValueImplSptr left2 = concat2->getValue(0);
      WolValueImplSptr leftResult = evalAndInt(left1, left2);

      WolValueImplSptr rightResult = nullptr;
      if (concat1->numValues() == 2) {
	WolValueImplSptr right1 = concat1->getValue(1);
	WolValueImplSptr right2 = concat2->getValue(1);

	rightResult = evalAndInt(right1, right2);
      }
      else {
	WolConcatValueImplSptr right1 = makeConcatImpl();
	for (int i = 1; i < concat1->numValues(); i++) {
	  right1->addValue(concat1->getValue(i));
	}

	WolConcatValueImplSptr right2 = makeConcatImpl();
	  for (int i = 1; i < concat2->numValues(); i++) {
	    right1->addValue(concat2->getValue(i));
	}

	rightResult = evalAndInt(right1, right2);
      }

      WolConcatValueImplSptr result = makeConcatImpl();
      result->addValue(leftResult);
      result->addValue(rightResult);
      return result;
    }

    assert(0);
    return nullptr;
  }

// primary functions for BEQ operations
  WolValueImplSptr
  WolEvalFactoryImpl::evalBoolBeqBool (WolValueImplSptr val1,
				       WolValueImplSptr val2) {

    WolBoolValueImplSptr bool_val1 = dBoolCast (val1);
    WolBoolValueImplSptr bool_val2 = dBoolCast (val2);

    bool result = (bool_val1->_value == bool_val2->_value);

    WolBoolValueImplSptr retValue = makeBoolImpl(result);
    return retValue;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstBeqConst (WolValueImplSptr val1,
					 WolValueImplSptr val2) {

    WolConstValueImplSptr const_val1 = dConstCast (val1);
    WolConstValueImplSptr const_val2 = dConstCast (val2);

    bool result = (const_val1->_value == const_val2->_value);

    WolBoolValueImplSptr retValue = makeBoolImpl(result);
    return retValue;
  }

  //TODO: add not singleton check for all data types.
    // assumption: range, union, concat cannot have single value;
  WolValueImplSptr
  WolEvalFactoryImpl::evalNonConstBeq (WolValueImplSptr val1,
    				       WolValueImplSptr val2) {
      WolRangeValueImplSptr retValue = makeRangeImpl(1);
      return retValue;
  }

// primary functions for ADD operator WolEvalFactoryImpl::evaluation
  WolValueImplSptr
  WolEvalFactoryImpl::evalBoolAddBool (WolValueImplSptr val1,
				       WolValueImplSptr val2) {

    WolBoolValueImplSptr bool_val1 = dynamic_pointer_cast<WolBoolValueImpl> (
	val1);
    WolBoolValueImplSptr bool_val2 = dynamic_pointer_cast<WolBoolValueImpl> (
	val2);

    bool result = (bool_val1->_value ^ bool_val2->_value);

    WolBoolValueImplSptr retValue (new WolBoolValueImpl (result));
    return static_pointer_cast<WolValueImpl> (retValue);
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstAddConst (WolValueImplSptr val1,
					 WolValueImplSptr val2) {

    WolConstValueImplSptr const_val1 = dConstCast (
	val1);
    WolConstValueImplSptr const_val2 = dConstCast (
	val2);

    boost::dynamic_bitset<> result = bitSetAdd (const_val1->_value,
						const_val2->_value);

    WolConstValueImplSptr retValue (new WolConstValueImpl (result));
    return static_pointer_cast<WolValueImpl> (retValue);
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstAddRange (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstAddUnion (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstAddConcat (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeAddRange (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeAddUnion (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeAddConcat (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionAddUnion (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionAddConcat (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatAddConcat (WolValueImplSptr val1,
					   WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

  // primary functions for MUL operator WolEvalFactoryImpl::evaluation
  WolValueImplSptr
  WolEvalFactoryImpl::evalBoolMulBool (WolValueImplSptr val1,
				       WolValueImplSptr val2) {

    WolBoolValueImplSptr bool_val1 = dynamic_pointer_cast<WolBoolValueImpl> (
	val1);
    WolBoolValueImplSptr bool_val2 = dynamic_pointer_cast<WolBoolValueImpl> (
	val2);

    bool result = bool_val1->_value & bool_val2->_value;

    WolBoolValueImplSptr retValue (new WolBoolValueImpl (result));
    return static_pointer_cast<WolValueImpl> (retValue);
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstMulConst (WolValueImplSptr val1,
					 WolValueImplSptr val2) {

    WolConstValueImplSptr const_val1 = dConstCast (
	val1);
    WolConstValueImplSptr const_val2 = dConstCast (
	val2);

    boost::dynamic_bitset<> result = bitSetMul (const_val1->_value,
						const_val2->_value);

    WolConstValueImplSptr retValue (new WolConstValueImpl (result));
    return static_pointer_cast<WolValueImpl> (retValue);
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstMulRange (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstMulUnion (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstMulConcat (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeMulRange (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeMulUnion (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeMulConcat (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionMulUnion (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionMulConcat (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatMulConcat (WolValueImplSptr val1,
					   WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

  // primary functions for SLL operator WolEvalFactoryImpl::evaluation
  WolValueImplSptr
  WolEvalFactoryImpl::evalBoolSllBool (WolValueImplSptr val1,
				       WolValueImplSptr val2) {

    WolBoolValueImplSptr bool_val1 = dynamic_pointer_cast<WolBoolValueImpl> (
	val1);
    WolBoolValueImplSptr bool_val2 = dynamic_pointer_cast<WolBoolValueImpl> (
	val2);

    bool result = (bool_val1->_value << bool_val2->_value);

    WolBoolValueImplSptr retValue (new WolBoolValueImpl (result));
    return static_pointer_cast<WolValueImpl> (retValue);
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstSllConst (WolValueImplSptr val1,
					 WolValueImplSptr val2) {

    WolConstValueImplSptr const_val1 = dConstCast (
	val1);
    WolConstValueImplSptr const_val2 = dConstCast (
	val2);

    boost::dynamic_bitset<> result = const_val1->_value
	<< const_val2->_value.to_ulong ();

    WolConstValueImplSptr retValue (new WolConstValueImpl (result));
    return static_pointer_cast<WolValueImpl> (retValue);

  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstSllRange (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConstSllUnion (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConstSllConcat (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeSllRange (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeSllUnion (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeSllConcat (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionSllUnion (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionSllConcat (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatSllConcat (WolValueImplSptr val1,
					   WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeSllConst (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionSllConst (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatSllConst (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionSllRange (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatSllRange (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatSllUnion (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

// primary functions for SRL operator WolEvalFactoryImpl::evaluation
  WolValueImplSptr
  WolEvalFactoryImpl::evalBoolSrlBool (WolValueImplSptr val1,
				       WolValueImplSptr val2) {

    WolBoolValueImplSptr bool_val1 = dynamic_pointer_cast<WolBoolValueImpl> (
	val1);
    WolBoolValueImplSptr bool_val2 = dynamic_pointer_cast<WolBoolValueImpl> (
	val2);

    bool result = bool_val1->_value >> bool_val2->_value;

    WolBoolValueImplSptr retValue (new WolBoolValueImpl (result));
    return static_pointer_cast<WolValueImpl> (retValue);
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstSrlConst (WolValueImplSptr val1,
					 WolValueImplSptr val2) {

    WolConstValueImplSptr const_val1 = dConstCast (
	val1);
    WolConstValueImplSptr const_val2 = dConstCast (
	val2);

    boost::dynamic_bitset<> result = const_val1->_value
	>> const_val2->_value.to_ulong ();

    WolConstValueImplSptr retValue (new WolConstValueImpl (result));
    return static_pointer_cast<WolValueImpl> (retValue);
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstSrlRange (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConstSrlUnion (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConstSrlConcat (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeSrlRange (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeSrlUnion (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeSrlConcat (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionSrlUnion (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionSrlConcat (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatSrlConcat (WolValueImplSptr val1,
					   WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeSrlConst (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionSrlConst (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatSrlConst (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionSrlRange (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatSrlRange (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatSrlUnion (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

  // primary functions for UDIV operator WolEvalFactoryImpl::evaluation
  WolValueImplSptr
  WolEvalFactoryImpl::evalBoolUdivBool (WolValueImplSptr val1,
					WolValueImplSptr val2) {
    WolBoolValueImplSptr bool_val1 = dynamic_pointer_cast<WolBoolValueImpl> (
	val1);
    WolBoolValueImplSptr bool_val2 = dynamic_pointer_cast<WolBoolValueImpl> (
	val2);

    if (bool_val2->_value == 0)
      assert(0);
    bool result = bool_val1->_value;

    WolBoolValueImplSptr retValue (new WolBoolValueImpl (result));
    return static_pointer_cast<WolValueImpl> (retValue);
  }



  WolValueImplSptr
  WolEvalFactoryImpl::evalConstUdivConst (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    WolConstValueImplSptr const_val1 = dConstCast (
	val1);
    WolConstValueImplSptr const_val2 = dConstCast (
	val2);

    boost::dynamic_bitset<> result = bitSetDiv (const_val1->_value,
						const_val2->_value, false);

    WolConstValueImplSptr retValue (new WolConstValueImpl (result));
    return static_pointer_cast<WolValueImpl> (retValue);

  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstUdivRange (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConstUdivUnion (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConstUdivConcat (WolValueImplSptr val1,
					   WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeUdivRange (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeUdivUnion (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeUdivConcat (WolValueImplSptr val1,
					   WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionUdivUnion (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionUdivConcat (WolValueImplSptr val1,
					   WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatUdivConcat (WolValueImplSptr val1,
					    WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeUdivConst (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionUdivConst (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatUdivConst (WolValueImplSptr val1,
					   WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionUdivRange (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatUdivRange (WolValueImplSptr val1,
					   WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatUdivUnion (WolValueImplSptr val1,
					   WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

  // primary functions for UREM operator WolEvalFactoryImpl::evaluation
  WolValueImplSptr
  WolEvalFactoryImpl::evalBoolUremBool (WolValueImplSptr val1,
					WolValueImplSptr val2) {

    WolBoolValueImplSptr bool_val2 = dynamic_pointer_cast<WolBoolValueImpl> (
	val2);

    if (bool_val2->_value == 0)
      assert(0);
    bool result = false;

    WolBoolValueImplSptr retValue (new WolBoolValueImpl (result));
    return static_pointer_cast<WolValueImpl> (retValue);
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstUremConst (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    WolConstValueImplSptr const_val1 = dConstCast (
	val1);
    WolConstValueImplSptr const_val2 = dConstCast (
	val2);

    boost::dynamic_bitset<> result = bitSetDiv (const_val1->_value,
						const_val2->_value, true);

    WolConstValueImplSptr retValue (new WolConstValueImpl (result));
    return static_pointer_cast<WolValueImpl> (retValue);
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstUremRange (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConstUremUnion (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConstUremConcat (WolValueImplSptr val1,
					   WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeUremRange (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeUremUnion (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeUremConcat (WolValueImplSptr val1,
					   WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionUremUnion (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionUremConcat (WolValueImplSptr val1,
					   WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatUremConcat (WolValueImplSptr val1,
					    WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeUremConst (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionUremConst (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatUremConst (WolValueImplSptr val1,
					   WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionUremRange (WolValueImplSptr val1,
					  WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatUremRange (WolValueImplSptr val1,
					   WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }
  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatUremUnion (WolValueImplSptr val1,
					   WolValueImplSptr val2) {
    assert(0);
    return nullptr;
  }

// primary functions for CONCAT operator WolEvalFactoryImpl::evaluation
  WolValueImplSptr
  WolEvalFactoryImpl::evalBoolConcatBool (WolValueImplSptr val1,
					  WolValueImplSptr val2) {

    WolBoolValueImplSptr bool_val1 = dBoolCast (val1);
    WolBoolValueImplSptr bool_val2 = dBoolCast (val2);

    boost::dynamic_bitset<> result (2);
    result[0] = bool_val1->_value;
    result[1] = bool_val2->_value;

    WolConstValueImplSptr retValue (new WolConstValueImpl (result));
    return static_pointer_cast<WolValueImpl> (retValue);
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstConcatConst (WolValueImplSptr val1,
					    WolValueImplSptr val2) {
    WolConstValueImplSptr const_val1 = dConstCast (val1);
    WolConstValueImplSptr const_val2 = dConstCast (val2);

    int length1 = const_val1->_value.size ();
    int length2 = const_val2->_value.size ();

    boost::dynamic_bitset<> result (length1 + length2);
    int index = 0;

    for (int i = 0; i < length2; i++) {
      result[index] = const_val2->_value[i];
      index++;
    }

    for (int i = 0; i < length1; i++) {
      result[index] = const_val1->_value[i];
      index++;
    }

    WolConstValueImplSptr retValue (new WolConstValueImpl (result));
    return static_pointer_cast<WolValueImpl> (retValue);
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalNonConstConcat (WolValueImplSptr val1,
					  WolValueImplSptr val2) {

    WolConcatValueImplSptr retValue = makeConcatImpl();
    retValue->addValue(val1);
    retValue->addValue(val2);
    return retValue;
  }


  // primary functions for UNION operator WolEvalFactoryImpl::evaluation
  WolValueImplSptr
  WolEvalFactoryImpl::evalBoolUnionBool (WolValueImplSptr val1,
					 WolValueImplSptr val2) {
    WolBoolValueImplSptr bool_val1 = dBoolCast (val1);
    WolBoolValueImplSptr bool_val2 = dBoolCast (val2);

    if (bool_val1->_value == bool_val2->_value)
       return bool_val1;
    else
       return makeRangeImpl(1);

    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstUnionConst (WolValueImplSptr val1,
					   WolValueImplSptr val2) {
    WolConstValueImplSptr const_val1 = dConstCast (val1);
    WolConstValueImplSptr const_val2 = dConstCast (val2);

    //TODO: Check to see if range can be formed
    if (const_val1->containsValue(const_val2->getLowValue()))
      return const_val1;
    else {
      WolUnionValueImplSptr retValue = makeUnionImpl();
      retValue->addValue(val1);
      retValue->addValue(val2);
      return retValue;
    }

    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstUnionRange (WolValueImplSptr val1,
                                           WolValueImplSptr val2) {

    WolConstValueImplSptr const_val = dConstCast(val1);
    WolRangeValueImplSptr range_val = dRangeCast(val2);

    if (range_val->containsValue(const_val->getLowValue()))
      return val2;
    else {
      WolUnionValueImplSptr retValue = makeUnionImpl();
      retValue->addValue(val1);
      retValue->addValue(val2);
      return retValue;
    }

    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstUnionUnion (WolValueImplSptr val1,
					   WolValueImplSptr val2) {
    WolConstValueImplSptr const_val = dConstCast(val1);
    WolUnionValueImplSptr union_val = dUnionCast(val2);

    //TODO : check to see if const is at the boundaries of range elements.
    for (auto i : union_val->_values) {
      if(i->containsValue(const_val->getLowValue()))
	return val2;
    }

    WolUnionValueImplSptr retValue = makeUnionImpl();
    retValue->addValue(val1);
    retValue->addValue(val2);
    return retValue;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstUnionConcat (WolValueImplSptr val1,
					    WolValueImplSptr val2) {

    WolConstValueImplSptr const_val = dConstCast(val1);
    if (val2->containsValue(const_val->getLowValue()))
      return val2;

    WolUnionValueImplSptr retValue = makeUnionImpl();
    retValue->addValue(val1);
    retValue->addValue(val2);
    return retValue;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeUnionRange (WolValueImplSptr val1,
					   WolValueImplSptr val2) {

    WolRangeValueImplSptr range_val1 = dRangeCast(val1);
    WolRangeValueImplSptr range_val2 = dRangeCast(val2);

    bool merge = false;

     if (range_val1->containsValue(range_val2->getLowValue()) ||
	 range_val2->containsValue(range_val1->getLowValue()) ||
	 bitSetInc(range_val1->getHighValue()) == range_val2->getLowValue() ||
	 bitSetInc(range_val2->getHighValue()) == range_val1->getLowValue())
        merge = true;

    if (merge) {
      WolRangeValueImplSptr retValue = makeRangeImpl2(std::min(range_val1->getLowValue(),
                                                           range_val2->getLowValue()),
                                                     std::max(range_val1->getHighValue(),
                                                           range_val2->getHighValue()));
      return retValue;
    }
    else {
      WolUnionValueImplSptr retValue = makeUnionImpl();
      retValue->addValue(val1);
      retValue->addValue(val2);
      return retValue;

    }
    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeUnionUnion (WolValueImplSptr val1,
					   WolValueImplSptr val2) {
    //TODO: lots of inclusion cases are possible: handle all cases

    WolRangeValueImplSptr range_val = dRangeCast(val1);
    WolUnionValueImplSptr union_val = dUnionCast(val2);


    if (range_val->containsValue(union_val->getLowValue()) &&
	range_val->containsValue(union_val->getHighValue()))
       return val1;
    else {
      WolUnionValueImplSptr retValue = makeUnionImpl();
      retValue->addValue(val1);
      retValue->addValue(val2);
      return retValue;
    }

    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeUnionConcat (WolValueImplSptr val1,
					    WolValueImplSptr val2) {

    WolUnionValueImplSptr retValue = makeUnionImpl();
    retValue->addValue(val1);
    retValue->addValue(val2);
    return retValue;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionUnionUnion (WolValueImplSptr val1,
					   WolValueImplSptr val2) {

    //TODO: scope for optimizations
    WolUnionValueImplSptr union_val1 = dUnionCast(val1);
    WolUnionValueImplSptr union_val2 = dUnionCast(val2);

    WolUnionValueImplSptr retValue = makeUnionImpl();
    for (auto i : union_val1->_values) {
       retValue->addValue(i);
    }
    for (auto i : union_val2->_values) {
       retValue->addValue(i);
    }

    return retValue;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionUnionConcat (WolValueImplSptr val1,
					    WolValueImplSptr val2) {

    WolUnionValueImplSptr union_val = dUnionCast(val1);
    WolConcatValueImplSptr concat_val = dConcatCast(val2);

    WolUnionValueImplSptr retValue = makeUnionImpl();
    for (auto i : union_val->_values)
	retValue->addValue(i);
    retValue->addValue(val2);

    return retValue;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatUnionConcat (WolValueImplSptr val1,
					     WolValueImplSptr val2) {

    WolUnionValueImplSptr retValue = makeUnionImpl();
    retValue->addValue(val1);
    retValue->addValue(val2);
    return retValue;
  }

  // primary functions for INTERSECTION operator WolEvalFactoryImpl::evaluation
  WolValueImplSptr
  WolEvalFactoryImpl::evalBoolIntersectionBool (WolValueImplSptr val1,
						WolValueImplSptr val2) {
    WolBoolValueImplSptr bool_val1 = dBoolCast(val1);
    WolBoolValueImplSptr bool_val2 = dBoolCast(val2);

    if (bool_val1->_value == bool_val1->_value) return val1;
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstIntersectionConst (WolValueImplSptr val1,
						  WolValueImplSptr val2) {
    WolConstValueImplSptr const_val1 = dConstCast(val1);
    WolConstValueImplSptr const_val2 = dConstCast(val2);

    if (const_val1->_value == const_val2->_value) return val1;
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstIntersectionRange (WolValueImplSptr val1,
						  WolValueImplSptr val2) {
    WolConstValueImplSptr const_val = dConstCast(val1);
    WolRangeValueImplSptr range_val = dRangeCast(val2);

    if (range_val->containsValue(const_val->getLowValue())) return val1;
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstIntersectionUnion (WolValueImplSptr val1,
						  WolValueImplSptr val2) {
    WolConstValueImplSptr const_val = dConstCast(val1);
    WolUnionValueImplSptr union_val = dUnionCast(val2);

    if (union_val->containsValue(const_val->getLowValue())) return val1;
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstIntersectionConcat (WolValueImplSptr val1,
						   WolValueImplSptr val2) {
    WolConstValueImplSptr const_val = dConstCast(val1);
    WolConcatValueImplSptr concat_val = dConcatCast(val2);

    if(concat_val->containsValue(const_val->getLowValue())) return val1;
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeIntersectionRange (WolValueImplSptr val1,
						  WolValueImplSptr val2) {

    WolRangeValueImplSptr range_val1 = dRangeCast(val1);
    WolRangeValueImplSptr range_val2 = dRangeCast(val2);

    if (range_val1->isFull()) return range_val2;
    if (range_val2->isFull()) return range_val1;
    if (range_val1->_highValue == range_val2->_lowValue) {
      WolConstValueImplSptr retValue = makeConstImpl(range_val1->_highValue);
      return retValue;
    }
    else if (range_val2->_lowValue == range_val1->_highValue) {
      WolConstValueImplSptr retValue = makeConstImpl(range_val2->_lowValue);
    }
    else  if (range_val1->containsValue(range_val2->getLowValue()) ||
	      range_val2->containsValue(range_val1->getLowValue())) {

      WolRangeValueImplSptr retValue =
	  makeRangeImpl2(std::max(range_val1->getLowValue(),
	                         range_val2->getLowValue()),
	                std::min(range_val1->getHighValue(),
	                         range_val2->getHighValue()));
      return retValue;
    }
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeIntersectionUnion (WolValueImplSptr val1,
						  WolValueImplSptr val2) {
    WolRangeValueImplSptr range_val = dRangeCast(val1);
    WolUnionValueImplSptr union_val = dUnionCast(val2);

    if (range_val->isFull()) return val2;

    WolUnionValueImplSptr retValue = makeUnionImpl();
    bool null = true;
    for(auto i : union_val->_values) {
      WolValueImplSptr temp = evalIntersectionInt(i, range_val);
      if (!temp) {
	retValue->addValue(temp);
	null = false;
      }
    }

    return null? nullptr : retValue;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeIntersectionConcat (WolValueImplSptr val1,
						   WolValueImplSptr val2) {

    WolRangeValueImplSptr range_val = dRangeCast(val1);
    WolConcatValueImplSptr concat_val = dConcatCast(val2);

    if (range_val->isFull()) return val2;

    WolConcatValueImplSptr result = makeConcatImpl();
    assert(concat_val->begin() != concat_val->end());
    int position = (*concat_val->begin())->getPrecision();
    WolValueImplSptr rangeSplit = range_val->split(position);

    return evalIntersectionInt(rangeSplit, concat_val);
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionIntersectionUnion (WolValueImplSptr val1,
						  WolValueImplSptr val2) {
    WolUnionValueImplSptr union_val1 = dUnionCast(val1);
    WolUnionValueImplSptr union_val2 = dUnionCast(val2);

    WolUnionValueImplSptr retValue = makeUnionImpl();
    bool null = true;
    for (auto i : union_val1->_values) {
      for (auto j : union_val2->_values) {
	WolValueImplSptr temp = evalIntersectionInt(i, j);
	if (!temp){
	  retValue->addValue(temp);
	  null = false;
	}
      }
    }

    return null? nullptr: retValue;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionIntersectionConcat (WolValueImplSptr val1,
						   WolValueImplSptr val2) {

    WolUnionValueImplSptr union_val = dUnionCast(val1);
    WolConcatValueImplSptr concat_val = dConcatCast(val2);

    WolUnionValueImplSptr retValue = makeUnionImpl();
    bool null = true;
    for(auto i : union_val->_values) {
      WolValueImplSptr temp = evalIntersectionInt(i, concat_val);
      if (!temp) {
	retValue->addValue(temp);
	null = false;
      }
    }
    return null? nullptr: retValue;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatIntersectionConcat (WolValueImplSptr val1,
						    WolValueImplSptr val2) {
    WolConcatValueImplSptr concat1 = dConcatCast(val1);
    WolConcatValueImplSptr concat2 = dConcatCast(val2);

    assert(concat1->numValues() > 1);
    assert(concat2->numValues() > 1);

    int firstPrecision1 = concat1->getValue(0)->getPrecision();
    int firstPrecision2 = concat2->getValue(0)->getPrecision();


    if (firstPrecision1 < firstPrecision2) {
      WolValueImplSptr split = concat2->split(firstPrecision1);
      return evalIntersectionInt(concat1, split);
    }
    else if (firstPrecision1 > firstPrecision2) {
      WolValueImplSptr split = concat1->split(firstPrecision2);
      return evalIntersectionInt(split, concat2);

    }
    else {
      WolValueImplSptr left1 = concat1->getValue(0);
      WolValueImplSptr left2 = concat2->getValue(0);
      WolValueImplSptr leftResult = evalIntersectionInt(left1, left2);
      if(!leftResult) return nullptr;

      WolValueImplSptr rightResult = nullptr;
      if (concat1->numValues() == 2) {
	WolValueImplSptr right1 = concat1->getValue(1);
	WolValueImplSptr right2 = concat2->getValue(1);

	rightResult = evalIntersectionInt(right1, right2);
      }
      else {
	WolConcatValueImplSptr right1 = makeConcatImpl();
	for (int i = 1; i < concat1->numValues(); i++) {
	  right1->addValue(concat1->getValue(i));
	}

	WolConcatValueImplSptr right2 = makeConcatImpl();
	for (int i = 1; i < concat2->numValues(); i++) {
	  right1->addValue(concat2->getValue(i));
	}

	rightResult = evalIntersectionInt(right1, right2);
      }
      if (!rightResult) return nullptr;

      WolConcatValueImplSptr result = makeConcatImpl();
      result->addValue(leftResult);
      result->addValue(rightResult);
      return result;
    }

    assert(0);
    return nullptr;
  }

  // primary functions for Splice operator WolEvalFactoryImpl::evaluation
  WolValueImplSptr
  WolEvalFactoryImpl::evalBoolSplice (WolValueImplSptr val, int upper,
				      int lower) {
    assert(upper == lower);
    return val;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstSplice (WolValueImplSptr val, int upper,
				       int lower) {
    WolConstValueImplSptr const_val = dConstCast (
	val);
    int length = const_val->_value.size ();

    assert(length >= upper - lower + 1);
    boost::dynamic_bitset<> result (upper - lower + 1);
    int index = 0;

    for (int i = lower; i <= upper; i++) {
      result[index] = const_val->_value[i];
      index++;
    }

    WolConstValueImplSptr retValue (new WolConstValueImpl (result));
    return static_pointer_cast<WolValueImpl> (retValue);
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeSplice (WolValueImplSptr val, int upper,
				       int lower) {

    WolRangeValueImplSptr range_val = dRangeCast(val);
    int precision = val->getPrecision();
    if (lower == 0 && upper == precision - 1)
      return range_val;

    WolValueImplSptr tempSplit = range_val;
    if (lower > 0)
       tempSplit = range_val->split(lower);

    WolValueImplSptr finalSplit = tempSplit;
    if (upper < precision - 1)
       finalSplit = tempSplit->split(upper);


    WolValueImplSptr retValue = finalSplit->getSlices(upper, lower);
    return retValue;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionSplice (WolValueImplSptr val, int upper,
				       int lower) {

    WolUnionValueImplSptr union_val = dUnionCast(val);
    WolUnionValueImplSptr retValue = makeUnionImpl();

    for (auto it = union_val->begin(); it != union_val->end(); it++) {
      retValue->addValue(evalSpliceInt((*it), upper, lower));
    }
    retValue->optimize();

    return retValue;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatSplice (WolValueImplSptr val, int upper,
					int lower) {

    WolConcatValueImplSptr concat_val = dConcatCast(val);
    int precision = val->getPrecision();

    if (lower == 0 && upper == precision - 1)
      return concat_val;

    WolValueImplSptr tempSplit = concat_val;
    if (lower > 0)
      tempSplit = concat_val->split(lower);

    WolValueImplSptr finalSplit = tempSplit;
    if (upper < precision - 1)
      finalSplit = tempSplit->split(upper);

    WolValueImplSptr retValue = finalSplit->getSlices(upper, lower);
    return retValue;
  }

  // primary functions for compliment operator WolEvalFactoryImpl::evaluation
  WolValueImplSptr
  WolEvalFactoryImpl::evalBoolCompliment (WolValueImplSptr val) {

    WolBoolValueImplSptr bool_val = dBoolCast(val);
    WolBoolValueImplSptr retValue = makeBoolImpl(!bool_val->_value);

    return retValue;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConstCompliment (WolValueImplSptr val) {


	  WolConstValueImplSptr const_val = dConstCast(val);
	  int prec = const_val->getPrecision();

	  if (const_val->getConstType() == WolValueImpl::WOL_SPECIAL_CONST_ZERO) {

		  if (prec == 1) {
			  WolConstValueImplSptr retValue = makeConstImpl(dbitset(1,1));
			  return retValue;
		  }
		  else {
			  dbitset incValue = bitSetInc(const_val->_value);
			  WolRangeValueImplSptr retValue = makeRangeImpl2(incValue, ~dbitset(prec, 0));
			  return retValue;
		  }
	  }
	  else if (const_val->getConstType() == WolValueImpl::WOL_SPECIAL_CONST_ONES ||
			  const_val->getConstType() == WolValueImpl::WOL_SPECIAL_CONST_ONE_ONES) {
		  if (prec == 1) {
			  WolConstValueImplSptr retValue = makeConstImpl(dbitset(1,0));
			  return retValue;
		  }
		  else {
			  dbitset decValue = bitSetDec(const_val->_value);
			  WolRangeValueImplSptr retValue = makeRangeImpl2(dbitset(prec, 0), decValue);
			  return retValue;
		  }
	  }
	  else {
		  dbitset decValue = bitSetDec(const_val->_value);
		  dbitset incValue = bitSetInc(const_val->_value);

		  WolUnionValueImplSptr retValue = makeUnionImpl();
		  if (dbitset(prec, 0) != decValue) {
			  WolRangeValueImplSptr one = makeRangeImpl2(dbitset(prec, 0), decValue);
			  retValue->addValue(one);
		  }
		  else {
			  WolConstValueImplSptr one = makeConstImpl(decValue);
			  retValue->addValue(one);
		  }

		  if (dbitset(prec, unsigned(-1)) != incValue) {
			  WolRangeValueImplSptr two = makeRangeImpl2(incValue, dbitset(prec, (unsigned)-1));
			  retValue->addValue(two);
		  }
		  else {
			  WolConstValueImplSptr two = makeConstImpl(incValue);
			  retValue->addValue(two);
		  }

		  return retValue;

	  }
	  assert(0);
	  return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalRangeCompliment (WolValueImplSptr val) {

    WolRangeValueImplSptr range_val = dRangeCast(val);
    dbitset lowValue = range_val->_lowValue;
    dbitset highValue = range_val->_highValue;
    int prec = range_val->getPrecision();


    if(range_val->isFull()) return nullptr;

    if (lowValue == dbitset(prec, 0)) {

      dbitset incValue = bitSetInc(highValue);
      if (dbitset(prec, unsigned(-1)) != incValue) {
	WolRangeValueImplSptr retValue = makeRangeImpl2(incValue, dbitset(prec, (unsigned)-1));
	return retValue;
      }
      else {
	WolConstValueImplSptr retValue = makeConstImpl(incValue);
	return retValue;
      }
    }
    else if (highValue == dbitset(prec, (unsigned)-1)) {
      dbitset decValue = bitSetDec(lowValue);
      if (dbitset(prec, 0) != decValue) {
	WolRangeValueImplSptr retValue = makeRangeImpl2(dbitset(prec, 0), decValue);
	return retValue;
      }
      else {
	WolConstValueImplSptr retValue = makeConstImpl(decValue);
	return retValue;
      }
    }
    else {
      dbitset decValue = bitSetDec(lowValue);
      dbitset incValue = bitSetInc(highValue);

      WolUnionValueImplSptr retValue = makeUnionImpl();
      if (dbitset(prec, 0) != decValue) {
	WolRangeValueImplSptr one = makeRangeImpl2(dbitset(prec, 0), decValue);
	retValue->addValue(one);
      }
      else {
	WolConstValueImplSptr one = makeConstImpl(decValue);
	retValue->addValue(one);
      }

      if (dbitset(prec, unsigned(-1)) != incValue) {
	WolRangeValueImplSptr two = makeRangeImpl2(incValue, dbitset(prec, (unsigned)-1));
	retValue->addValue(two);
      }
      else {
	WolConstValueImplSptr two = makeConstImpl(incValue);
	retValue->addValue(two);
      }

      return retValue;

    }
    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalUnionCompliment (WolValueImplSptr val) {

    WolUnionValueImplSptr union_val = dUnionCast(val);

    WolValueImplSptr retValue = makeRangeImpl(union_val->getPrecision());
    for (auto i : union_val->_values) {
      retValue = evalIntersectionInt(retValue, evalComplimentInt(i));
    }

    return retValue;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalConcatCompliment (WolValueImplSptr val) {

    WolConcatValueImplSptr concat_val = dConcatCast(val);
    int size = concat_val->numValues();
    int total_prec = concat_val->getPrecision();

    WolUnionValueImplSptr retValue = makeUnionImpl();
    for (int i = 0; i < size ; i++) {
      WolConcatValueImplSptr temp = makeConcatImpl();
      int temp_prec = 0;
      for (int j = 0; j < i; j++) {
	temp_prec += concat_val->getValue(j)->getPrecision();
	temp->addValue(concat_val->getValue(j));
      }
      temp_prec += concat_val->getValue(i)->getPrecision();
      temp->addValue(evalComplimentInt(concat_val->getValue(i)));

      WolRangeValueImplSptr temp_full_range = makeRangeImpl(total_prec - temp_prec - 1);
      temp->addValue(temp_full_range);
      retValue->addValue(temp);
    }

    return retValue;
  }

  // functions for  Backward AND evaluation
  WolValueImplSptr
  WolEvalFactoryImpl::evalBackBoolAndBool(WolValueImplSptr op_val,
                                       WolValueImplSptr operand_val) {
   WolBoolValueImplSptr op_bool_val = dBoolCast(op_val);
   WolBoolValueImplSptr operand_bool_val = dBoolCast(operand_val);

   if (op_bool_val->_value == 1 && operand_bool_val == 0) {
     return nullptr;
   }
   else if (op_bool_val->_value == 1 && operand_bool_val->_value == 1) {
     return makeBoolImpl(true);
   }
   else if (op_bool_val->_value == 0 && operand_bool_val->_value == 1) {
     return makeBoolImpl(false);
   }
   else {
     return makeRangeImpl(1);
   }

    assert(0);
    return nullptr;
  }


  /**
   *  op operand res
   *  0     0     X
   *  0     1     0
   *  1     1     1
   *  1     0     NA
   */
  WolValueImplSptr
  WolEvalFactoryImpl::evalBackConstAndConst(WolValueImplSptr op_val,
		  WolValueImplSptr operand_val){

	  WolConstValueImplSptr op_const_val = dConstCast(op_val);
	  WolConstValueImplSptr operand_const_val = dConstCast(operand_val);

	  if (((op_const_val->_value) & (~operand_const_val->_value)) != dbitset(op_val->getPrecision(), 0))
		  return nullptr;

	  if (((~op_const_val->_value) & (~operand_const_val->_value)) == dbitset(op_val->getPrecision(), 0))
		  return op_val;

	  WolConcatValueImplSptr retValue = makeConcatImpl();
	  dbitset temp;
	  int numXcount = 0;
	  for (unsigned i = 0; i < op_const_val->_value.size(); i++) {
		  if (op_const_val->_value[i] == 0 && operand_const_val->_value[i] == 0) {
			  numXcount++;
			  if (numXcount == 1 && i > 1) {
				  WolConstValueImplSptr c = makeConstImpl(temp);
				  retValue->addValue(c);
				  temp.clear();
			  }
		  }
		  else {
			  temp.resize(temp.size() + 1, op_const_val->_value[i]);
			  if (numXcount > 0) {
				  WolRangeValueImplSptr r = makeRangeImpl(numXcount);
				  retValue->addValue(r);
				  numXcount = 0;
			  }
		  }
	  }
	  if (numXcount == 0) {
		  WolConstValueImplSptr c = makeConstImpl(temp);
		  retValue->addValue(c);
	  }
	  else {
		  WolRangeValueImplSptr r = makeRangeImpl(numXcount);
		  retValue->addValue(r);
	  }

	  assert(retValue->getPrecision() == op_val->getPrecision());
	  if (retValue->numValues() == 1)
		  return retValue->getValue(0);

	  return retValue;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalBackConstAndRange(WolValueImplSptr op_val,
		  WolValueImplSptr operand_val){

	  WolConstValueImplSptr const_val = dConstCast(op_val);
	  WolRangeValueImplSptr range_val = dRangeCast(operand_val);

	  int size = const_val->_value.size();
	  if (!range_val->isFull()){
		  int position = range_val->OptimalSplitPosition();
		  if (position == 0) position = size/2;
		  auto rangeSplit = range_val->split(position);
		  auto constSplit = const_val->split(position);

		  return evalAndBInt(constSplit, rangeSplit);
	  }
	  else {
		  // 110011 ---> 11XX11
		  dbitset val = const_val->_value;
		  if (val.none()) {
			  auto result = makeRangeImpl(size);
			  return result ;
		  }
		  else if ((~val).none()) {
			  return const_val;
		  }
		  else {
			  WolConcatValueImplSptr concatVal = makeConcatImpl();
			  int i = 0, j = 0;
			  int element = val[0];
			  for (int k = 1; k < size; k++) {
				  if ((val[k] == element)) {
					  j++;
					  if (k < (size - 1)) continue;
				  }

				  //[i,j]
				  if (element == 1) {
					  WolConstValueImplSptr constVal = makeConstImpl(dbitset(j - i + 1, 0));
					  concatVal->addValue(constVal);
				  }
				  else {
					  WolRangeValueImplSptr rangeVal = makeRangeImpl(j- i + 1);
					  concatVal->addValue(rangeVal);
				  }

				  i = k;
				  j = k;
				  element = val[k];

			  }
			  if (concatVal->numValues() == 1)
				  return concatVal->getValue(0);
			  else
				  return concatVal;
		  }
	  }

	  assert(0);
	  return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalBackConstAndUnion(WolValueImplSptr op_val,
                                         WolValueImplSptr operand_val){
    WolConstValueImplSptr const_val = dConstCast(op_val);
    WolUnionValueImplSptr union_val = dUnionCast(operand_val);

    WolUnionValueImplSptr result = makeUnionImpl ();
    for (auto it = union_val->begin(); it != union_val->end(); it++) {
      WolValueImplSptr andValue = evalAndBInt(const_val, *it);
      if (andValue) result->addValue(andValue);
    }

    if (result->numValues() == 0)
      return nullptr;
    return result;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalBackConstAndConcat(WolValueImplSptr op_val,
                                          WolValueImplSptr operand_val){
    WolConstValueImplSptr const_val = dConstCast(op_val);
    WolConcatValueImplSptr concat_val = dConcatCast(operand_val);

    WolConcatValueImplSptr result = makeConcatImpl();
    dbitset value = const_val->_value;

    int offset = const_val->getPrecision();
    for (auto it = concat_val->begin(); it != concat_val->end(); it++) {
      int prec = (*it)->getPrecision();
      dbitset subValue = createSubsetOfDynamicBitSet(value, offset - prec, offset - 1);

      WolConstValueImplSptr subConst = makeConstImpl(subValue);
      WolValueImplSptr andValue = evalAndBInt(subConst, *it);
      if (andValue) result->addValue(andValue);
      offset -= prec;
    }

    if (result->numValues() == 0) return nullptr;
    return result;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalBackRangeAndRange(WolValueImplSptr op_val,
                                            WolValueImplSptr operand_val){

    WolRangeValueImplSptr range1 = dRangeCast(op_val);
    WolRangeValueImplSptr range2 = dRangeCast(operand_val);

    int position = min(range1->OptimalSplitPosition(),
                       range2->OptimalSplitPosition());
    if (position == 0) position = range1->getPrecision()/2;

    auto range1Split = range1->split(position);
    auto range2Split = range2->split(position);

    return evalAndBInt(range1Split, range2Split);
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalBackRangeAndUnion(WolValueImplSptr op_val,
                                         WolValueImplSptr operand_val){

    WolRangeValueImplSptr range_val = dRangeCast(op_val);
    WolUnionValueImplSptr union_val = dUnionCast(operand_val);

    WolUnionValueImplSptr result = makeUnionImpl ();
    for (auto it = union_val->begin(); it != union_val->end(); it++) {
      WolValueImplSptr andValue = evalAndBInt(range_val, *it);
      if (andValue) result->addValue(andValue);
    }

    if (result->numValues() == 0) return nullptr;
    return result;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalBackRangeAndConcat(WolValueImplSptr op_val,
                                          WolValueImplSptr operand_val){

    WolRangeValueImplSptr range_val = dRangeCast(op_val);
    WolConcatValueImplSptr concat_val = dConcatCast(operand_val);

    WolConcatValueImplSptr result = makeConcatImpl();

    assert(concat_val->begin() != concat_val->end());
    int position = (*concat_val->begin())->getPrecision();
    WolValueImplSptr rangeSplit = range_val->split(position);

    return evalAndBInt(rangeSplit, concat_val);
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalBackUnionAndUnion(WolValueImplSptr op_val,
                                         WolValueImplSptr operand_val){

    WolUnionValueImplSptr union1 = dUnionCast(op_val);
    WolUnionValueImplSptr union2 = dUnionCast(operand_val);

    WolUnionValueImplSptr result = makeUnionImpl();
    for (auto it1 = union1->begin(); it1 != union1->end(); it1++) {
      for (auto it2 = union2->begin(); it2 != union2->end(); it2++) {
	WolValueImplSptr andValue = evalAndBInt(*it1, *it2);
	if (andValue) result->addValue(andValue);
      }
    }

    if (result->numValues() == 0) return nullptr;
    return result;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalBackUnionAndConcat(WolValueImplSptr op_val,
                                          WolValueImplSptr operand_val){

    WolUnionValueImplSptr union_val = dUnionCast(op_val);
    WolConcatValueImplSptr concat_val = dConcatCast(operand_val);
    WolUnionValueImplSptr result = makeUnionImpl ();

    for (auto it = union_val->begin(); it != union_val->end(); it++) {
      WolValueImplSptr andValue = evalAndBInt(*it, concat_val);
      if (andValue) result->addValue(andValue);
    }

    if (result->numValues() == 0) return nullptr;
    return result;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalBackConcatAndConcat(WolValueImplSptr op_val,
                                           WolValueImplSptr operand_val){

    WolConcatValueImplSptr concat1 = dConcatCast(op_val);
    WolConcatValueImplSptr concat2 = dConcatCast(operand_val);

    assert(concat1->numValues() > 1);
    assert(concat2->numValues() > 1);

    int firstPrecision1 = concat1->getValue(0)->getPrecision();
    int firstPrecision2 = concat2->getValue(0)->getPrecision();

    if (firstPrecision1 < firstPrecision2) {
      WolValueImplSptr split = concat2->split(firstPrecision1);
      return evalAndBInt(concat1, split);
    }
    else if (firstPrecision1 > firstPrecision2) {
      WolValueImplSptr split = concat1->split(firstPrecision2);
      return evalAndBInt(split, concat2);

    }
    else {
      WolValueImplSptr left1 = concat1->getValue(0);
      WolValueImplSptr left2 = concat2->getValue(0);
      WolValueImplSptr leftResult = evalAndBInt(left1, left2);

      if (!leftResult) return nullptr;

      WolValueImplSptr rightResult = nullptr;
      if (concat1->numValues() == 2) {
	WolValueImplSptr right1 = concat1->getValue(1);
	WolValueImplSptr right2 = concat2->getValue(1);

	rightResult = evalAndBInt(right1, right2);
      }
      else {
	WolConcatValueImplSptr right1 = makeConcatImpl();
	for (int i = 1; i < concat1->numValues(); i++) {
	  right1->addValue(concat1->getValue(i));
	}

	WolConcatValueImplSptr right2 = makeConcatImpl();
	for (int i = 1; i < concat2->numValues(); i++) {
	  right1->addValue(concat2->getValue(i));
	}

	rightResult = evalAndBInt(right1, right2);
      }

      if (!rightResult) return nullptr;

      WolConcatValueImplSptr result = makeConcatImpl();
      result->addValue(leftResult);
      result->addValue(rightResult);
      return result;
    }

    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalBackRangeAndConst(WolValueImplSptr op_val,
                                         WolValueImplSptr operand_val){

    WolRangeValueImplSptr range_val = dRangeCast(op_val);
    WolConstValueImplSptr const_val = dConstCast(operand_val);

    int size = const_val->_value.size();
    if (!range_val->isFull()){
      int position = range_val->OptimalSplitPosition();
      if (position == 0) position = size/2;
      auto rangeSplit = range_val->split(position);
      auto constSplit = const_val->split(position);

      return evalAndBInt(rangeSplit, constSplit);
    }
    else {
      return range_val;
    }

    assert(0);
    return nullptr;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalBackUnionAndConst(WolValueImplSptr op_val,
                                            WolValueImplSptr operand_val){

    WolUnionValueImplSptr union_val = dUnionCast(op_val);
    WolConstValueImplSptr const_val = dConstCast(operand_val);

    WolUnionValueImplSptr result = makeUnionImpl ();
    for (auto it = union_val->begin(); it != union_val->end(); it++) {
      WolValueImplSptr andValue = evalAndBInt(*it, const_val);

      if (andValue) {
	if (andValue->isFull()) return andValue;
	result->addValue(andValue);
      }
    }

    if (result->numValues() == 0)
      return nullptr;
    return result;
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalBackConcatAndConst(WolValueImplSptr op_val,
                                             WolValueImplSptr operand_val){

    WolConcatValueImplSptr concat_val = dConcatCast(op_val);
    WolConstValueImplSptr const_val = dConstCast(operand_val);

    return evalBackConstAndConcat(operand_val, op_val);
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalBackUnionAndRange(WolValueImplSptr op_val,
                                         WolValueImplSptr operand_val){
    return evalBackRangeAndUnion(operand_val, op_val);
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalBackConcatAndRange(WolValueImplSptr op_val,
                                          WolValueImplSptr operand_val){
    return evalBackRangeAndConcat(operand_val, op_val);
  }

  WolValueImplSptr
  WolEvalFactoryImpl::evalBackConcatAndUnion(WolValueImplSptr op_val,
                                          WolValueImplSptr operand_val){
    return evalBackUnionAndConcat(operand_val, op_val);
  }

}
