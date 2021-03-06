#ifndef WOLEVALFACTORYIMPL_H_INCLUDED
#define WOLEVALFACTORYIMPL_H_INCLUDED

#include <iostream>
#include <map>
#include "wolvalueimpl.h"
#include "../common.h"
#include "../wolevalfactory.h"

namespace wolver
{

using namespace std;

class WolValue;
class WolConstValue;
class WolMgr;
class WolValueFactoryImpl;
class WolValueImpl;
class WolEvalFactoryImpl;

typedef WolValueImplSptr (WolEvalFactoryImpl::*evalFunctionPtr)(WolValueImplSptr, WolValueImplSptr);
typedef std::map<std::pair<WolValueImpl::WolValueImplType, WolValueImpl::WolValueImplType> , evalFunctionPtr> evalMap;
typedef WolValueImplSptr (WolEvalFactoryImpl::*eval1FunctionPtr)(WolValueImplSptr);
typedef std::map<WolValueImpl::WolValueImplType, eval1FunctionPtr> eval1Map;
typedef WolValueImplSptr (WolEvalFactoryImpl::*evalSpliceFunctionPtr)(WolValueImplSptr, int, int);
typedef std::map<WolValueImpl::WolValueImplType, evalSpliceFunctionPtr> evalSpliceMap;


class WolEvalFactoryImpl: public WolEvalFactory {

friend class WolRangeValueImpl;
public:
   WolEvalFactoryImpl() {
       initializeAllMaps();      
   }
   ~WolEvalFactoryImpl() {}
   
public : //methods

   WolValueSptr evalOp(evalMap &emap, WolValueSptr val1, WolValueSptr val2);
   WolValueSptr evalOp(evalMap &emap, WolValueSptr val1, WolValueSptr val2, bool flag);
   WolValueSptr evalAnd(WolValueSptr val1, WolValueSptr val2);
   WolValueSptr evalBeq(WolValueSptr val1, WolValueSptr val2);
   WolValueSptr evalAdd(WolValueSptr val1, WolValueSptr val2);
   WolValueSptr evalMul(WolValueSptr val1, WolValueSptr val2);
   WolValueSptr evalUlt(WolValueSptr val1, WolValueSptr val2);
   WolValueSptr evalSll(WolValueSptr val1, WolValueSptr val2);
   WolValueSptr evalSrl(WolValueSptr val1, WolValueSptr val2);
   WolValueSptr evalUdiv(WolValueSptr val1, WolValueSptr val2);
   WolValueSptr evalUrem(WolValueSptr val1, WolValueSptr val2);
   WolValueSptr evalConcat(WolValueSptr val1, WolValueSptr val2);
   WolValueSptr evalSplice(WolValueSptr val, int upper, int lower);
   WolValueSptr evalUnion(WolValueSptr val1, WolValueSptr val2);
   WolValueSptr evalIntersection(WolValueSptr val1, WolValueSptr val2);
   WolValueSptr evalDiff(WolValueSptr val1, WolValueSptr val2);
   WolValueSptr evalCompliment(WolValueSptr val);
   WolValueSptr evalCond(WolValueSptr cond, WolValueSptr val1, WolValueSptr val2);


   WolValueSptr evalUltB(WolValueSptr op_val, WolValueSptr operand_val, bool implyLeft);
   WolValueSptr evalAndB(WolValueSptr op_val, WolValueSptr operand_val);
   WolValueSptr evalBeqB(WolValueSptr op_val, WolValueSptr operand_val);
   WolValueSptr evalConcatB(WolValueSptr op_val, WolValueSptr operand_val, bool implyLeft);
   WolValueSptr evalSpliceB(WolValueSptr op_val, int upper, int lower, int width);
   // position = 0 => backward implication for bool condition
   // position = 1 => backward implication for if branch
   // position = 2 => backward implication for else branch
   WolValueSptr evalCondB(WolValueSptr op_val, WolValueSptr operand_val1,
                          WolValueSptr operand_val2, int position);


private : //methods

   WolValueImplSptr evalOpInt(evalMap &emap, WolValueImplSptr val1, WolValueImplSptr val2);
   WolValueImplSptr evalOpInt(evalMap &emap, WolValueImplSptr val1, WolValueImplSptr val2, bool flag);
   WolValueImplSptr evalAndInt(WolValueImplSptr val1, WolValueImplSptr val2);
   WolValueImplSptr evalBeqInt(WolValueImplSptr val1, WolValueImplSptr val2);
   WolValueImplSptr evalAddInt(WolValueImplSptr val1, WolValueImplSptr val2);
   WolValueImplSptr evalMulInt(WolValueImplSptr val1, WolValueImplSptr val2);
   WolValueImplSptr evalUltInt(WolValueImplSptr val1, WolValueImplSptr val2);
   WolValueImplSptr evalSllInt(WolValueImplSptr val1, WolValueImplSptr val2);
   WolValueImplSptr evalSrlInt(WolValueImplSptr val1, WolValueImplSptr val2);
   WolValueImplSptr evalUdivInt(WolValueImplSptr val1, WolValueImplSptr val2);
   WolValueImplSptr evalUremInt(WolValueImplSptr val1, WolValueImplSptr val2);
   WolValueImplSptr evalConcatInt(WolValueImplSptr val1, WolValueImplSptr val2);
   WolValueImplSptr evalSpliceInt(WolValueImplSptr val, int upper, int lower);
   WolValueImplSptr evalUnionInt(WolValueImplSptr val1, WolValueImplSptr val2);
   WolValueImplSptr evalIntersectionInt(WolValueImplSptr val1, WolValueImplSptr val2);
   WolValueImplSptr evalComplimentInt(WolValueImplSptr val);
   WolValueImplSptr evalCondInt(WolValueImplSptr cond, WolValueImplSptr val1, WolValueImplSptr val2);


   WolValueImplSptr evalUltBInt(WolValueImplSptr op_val, WolValueImplSptr operand_val, bool implyLeft);
   WolValueImplSptr evalAndBInt(WolValueImplSptr op_val, WolValueImplSptr operand_val);
   WolValueImplSptr evalBeqBInt(WolValueImplSptr op_val, WolValueImplSptr operand_val);
   WolValueImplSptr evalConcatBInt(WolValueImplSptr op_val, WolValueImplSptr operand_val, bool implyLeft);
   WolValueImplSptr evalSpliceBInt(WolValueImplSptr op_val, int upper, int lower, int width);
   // position = 0 => backward implication for Bool condition
   // position = 1 => backward implication for if branch
   // position = 2 => backward implication for else branch
   WolValueImplSptr evalCondBInt(WolValueImplSptr op_val, WolValueImplSptr operand_val1,
                                 WolValueImplSptr operand_val2, int position);

   evalFunctionPtr lookup(evalMap &emap, WolValueImpl::WolValueImplType type1, 
                          WolValueImpl::WolValueImplType type2);
   eval1FunctionPtr lookup(eval1Map &emap, WolValueImpl::WolValueImplType type);
   evalSpliceFunctionPtr lookup(evalSpliceMap &emap, WolValueImpl::WolValueImplType type);

   // function maps initialization for all operation
   void initializeAllMaps();
   void initializeAndEvalMap();
   void initializeBeqEvalMap();
   void initializeAddEvalMap();
   void initializeMulEvalMap();
   void initializeSllEvalMap();
   void initializeSrlEvalMap();
   void initializeUdivEvalMap();
   void initializeUremEvalMap();
   void initializeConcatEvalMap();
   void initializeSpliceEvalMap();
   void initializeUnionEvalMap();
   void initializeIntersectionEvalMap();
   void initializeComplimentMap();

   void initializeAndBEvalMap();;

   // primary functions for AND operator evaluation
   WolValueImplSptr evalBoolAndBool(WolValueImplSptr val1, 
                                    WolValueImplSptr val2);
   WolValueImplSptr evalConstAndConst(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstAndRange(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstAndUnion(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstAndConcat(WolValueImplSptr val1, 
                                       WolValueImplSptr val2);
   WolValueImplSptr evalRangeAndRange(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalRangeAndUnion(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalRangeAndConcat(WolValueImplSptr val1, 
                                       WolValueImplSptr val2);
   WolValueImplSptr evalUnionAndUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2); 
   WolValueImplSptr evalUnionAndConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2);
   WolValueImplSptr evalConcatAndConcat(WolValueImplSptr val1,
                                        WolValueImplSptr val2);
   
   // symmetric functions for AND operator
   WolValueImplSptr evalRangeAndConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
      return evalConstAndRange(val2, val1);
   }
   WolValueImplSptr evalUnionAndConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
      return evalConstAndUnion(val2, val1);
   }
   WolValueImplSptr evalConcatAndConst(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
      return evalConstAndConcat(val2, val1);
   }
   WolValueImplSptr evalUnionAndRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
      return evalRangeAndUnion(val2, val1);
   }
   WolValueImplSptr evalConcatAndRange(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
      return evalRangeAndConcat(val2, val1);
   }
   WolValueImplSptr evalConcatAndUnion(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
      return evalUnionAndConcat(val2, val1);
   }

   // primary functions for BEQ operator evaluation
   WolValueImplSptr evalBoolBeqBool(WolValueImplSptr val1, 
                                    WolValueImplSptr val2);
   WolValueImplSptr evalConstBeqConst(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalNonConstBeq(WolValueImplSptr val1,
                                   WolValueImplSptr val2);
   
   // primary functions for ADD operator evaluation
   WolValueImplSptr evalBoolAddBool(WolValueImplSptr val1, 
                                    WolValueImplSptr val2);
   WolValueImplSptr evalConstAddConst(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstAddRange(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstAddUnion(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstAddConcat(WolValueImplSptr val1, 
                                       WolValueImplSptr val2);
   WolValueImplSptr evalRangeAddRange(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalRangeAddUnion(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalRangeAddConcat(WolValueImplSptr val1, 
                                       WolValueImplSptr val2);
   WolValueImplSptr evalUnionAddUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2); 
   WolValueImplSptr evalUnionAddConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2);
   WolValueImplSptr evalConcatAddConcat(WolValueImplSptr val1,
                                        WolValueImplSptr val2);
   
   // symmetric functions for ADD operator
   WolValueImplSptr evalRangeAddConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
      return evalConstAddRange(val2, val1);
   }
   WolValueImplSptr evalUnionAddConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
      return evalConstAddUnion(val2, val1);
   }
   WolValueImplSptr evalConcatAddConst(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
      return evalConstAddConcat(val2, val1);
   }
   WolValueImplSptr evalUnionAddRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
      return evalRangeAddUnion(val2, val1);
   }
   WolValueImplSptr evalConcatAddRange(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
      return evalRangeAddConcat(val2, val1);
   }
   WolValueImplSptr evalConcatAddUnion(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
      return evalUnionAddConcat(val2, val1);
   }

   // primary functions for MUL operator evaluation
   WolValueImplSptr evalBoolMulBool(WolValueImplSptr val1, 
                                    WolValueImplSptr val2);
   WolValueImplSptr evalConstMulConst(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstMulRange(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstMulUnion(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstMulConcat(WolValueImplSptr val1, 
                                       WolValueImplSptr val2);
   WolValueImplSptr evalRangeMulRange(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalRangeMulUnion(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalRangeMulConcat(WolValueImplSptr val1, 
                                       WolValueImplSptr val2);
   WolValueImplSptr evalUnionMulUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2); 
   WolValueImplSptr evalUnionMulConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2);
   WolValueImplSptr evalConcatMulConcat(WolValueImplSptr val1,
                                        WolValueImplSptr val2);
   
   // symmetric functions for MUL operator
   WolValueImplSptr evalRangeMulConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
      return evalConstMulRange(val2, val1);
   }
   WolValueImplSptr evalUnionMulConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
      return evalConstMulUnion(val2, val1);
   }
   WolValueImplSptr evalConcatMulConst(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
      return evalConstMulConcat(val2, val1);
   }
   WolValueImplSptr evalUnionMulRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
      return evalRangeMulUnion(val2, val1);
   }
   WolValueImplSptr evalConcatMulRange(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
      return evalRangeMulConcat(val2, val1);
   }
   WolValueImplSptr evalConcatMulUnion(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
      return evalUnionMulConcat(val2, val1);
   }

     // primary functions for SLL operator evaluation
   WolValueImplSptr evalBoolSllBool(WolValueImplSptr val1, 
                                    WolValueImplSptr val2);
   WolValueImplSptr evalConstSllConst(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstSllRange(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstSllUnion(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstSllConcat(WolValueImplSptr val1, 
                                       WolValueImplSptr val2);
   WolValueImplSptr evalRangeSllRange(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalRangeSllUnion(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalRangeSllConcat(WolValueImplSptr val1, 
                                       WolValueImplSptr val2);
   WolValueImplSptr evalUnionSllUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2); 
   WolValueImplSptr evalUnionSllConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2);
   WolValueImplSptr evalConcatSllConcat(WolValueImplSptr val1,
                                        WolValueImplSptr val2);
   WolValueImplSptr evalRangeSllConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2);
   WolValueImplSptr evalUnionSllConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConcatSllConst(WolValueImplSptr val1,
                                       WolValueImplSptr val2);
   WolValueImplSptr evalUnionSllRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConcatSllRange(WolValueImplSptr val1,
                                       WolValueImplSptr val2);
   WolValueImplSptr evalConcatSllUnion(WolValueImplSptr val1,
                                       WolValueImplSptr val2);

      // primary functions for SRL operator evaluation
   WolValueImplSptr evalBoolSrlBool(WolValueImplSptr val1, 
                                    WolValueImplSptr val2);
   WolValueImplSptr evalConstSrlConst(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstSrlRange(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstSrlUnion(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstSrlConcat(WolValueImplSptr val1, 
                                       WolValueImplSptr val2);
   WolValueImplSptr evalRangeSrlRange(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalRangeSrlUnion(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalRangeSrlConcat(WolValueImplSptr val1, 
                                       WolValueImplSptr val2);
   WolValueImplSptr evalUnionSrlUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2); 
   WolValueImplSptr evalUnionSrlConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2);
   WolValueImplSptr evalConcatSrlConcat(WolValueImplSptr val1,
                                        WolValueImplSptr val2);
   WolValueImplSptr evalRangeSrlConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2);
   WolValueImplSptr evalUnionSrlConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConcatSrlConst(WolValueImplSptr val1,
                                       WolValueImplSptr val2);
   WolValueImplSptr evalUnionSrlRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConcatSrlRange(WolValueImplSptr val1,
                                       WolValueImplSptr val2);
   WolValueImplSptr evalConcatSrlUnion(WolValueImplSptr val1,
                                       WolValueImplSptr val2);

    // primary functions for UDIV operator evaluation
   WolValueImplSptr evalBoolUdivBool(WolValueImplSptr val1, 
                                    WolValueImplSptr val2);
   WolValueImplSptr evalConstUdivConst(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstUdivRange(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstUdivUnion(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstUdivConcat(WolValueImplSptr val1, 
                                       WolValueImplSptr val2);
   WolValueImplSptr evalRangeUdivRange(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalRangeUdivUnion(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalRangeUdivConcat(WolValueImplSptr val1, 
                                       WolValueImplSptr val2);
   WolValueImplSptr evalUnionUdivUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2); 
   WolValueImplSptr evalUnionUdivConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2);
   WolValueImplSptr evalConcatUdivConcat(WolValueImplSptr val1,
                                        WolValueImplSptr val2);
   WolValueImplSptr evalRangeUdivConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2);
   WolValueImplSptr evalUnionUdivConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConcatUdivConst(WolValueImplSptr val1,
                                       WolValueImplSptr val2);
   WolValueImplSptr evalUnionUdivRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConcatUdivRange(WolValueImplSptr val1,
                                       WolValueImplSptr val2);
   WolValueImplSptr evalConcatUdivUnion(WolValueImplSptr val1,
                                       WolValueImplSptr val2);

   // primary functions for UREM operator evaluation
   WolValueImplSptr evalBoolUremBool(WolValueImplSptr val1, 
                                    WolValueImplSptr val2);
   WolValueImplSptr evalConstUremConst(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstUremRange(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstUremUnion(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstUremConcat(WolValueImplSptr val1, 
                                       WolValueImplSptr val2);
   WolValueImplSptr evalRangeUremRange(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalRangeUremUnion(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalRangeUremConcat(WolValueImplSptr val1, 
                                       WolValueImplSptr val2);
   WolValueImplSptr evalUnionUremUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2); 
   WolValueImplSptr evalUnionUremConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2);
   WolValueImplSptr evalConcatUremConcat(WolValueImplSptr val1,
                                        WolValueImplSptr val2);
   WolValueImplSptr evalRangeUremConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2);
   WolValueImplSptr evalUnionUremConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConcatUremConst(WolValueImplSptr val1,
                                       WolValueImplSptr val2);
   WolValueImplSptr evalUnionUremRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConcatUremRange(WolValueImplSptr val1,
                                       WolValueImplSptr val2);
   WolValueImplSptr evalConcatUremUnion(WolValueImplSptr val1,
                                       WolValueImplSptr val2);

    // primary functions for CONCAT operator evaluation
   WolValueImplSptr evalBoolConcatBool(WolValueImplSptr val1, 
                                    WolValueImplSptr val2);
   WolValueImplSptr evalConstConcatConst(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalNonConstConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2);

   // primary functions for UNION operator evaluation
   WolValueImplSptr evalBoolUnionBool(WolValueImplSptr val1, 
                                    WolValueImplSptr val2);
   WolValueImplSptr evalConstUnionConst(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstUnionRange(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstUnionUnion(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstUnionConcat(WolValueImplSptr val1, 
                                       WolValueImplSptr val2);
   WolValueImplSptr evalRangeUnionRange(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalRangeUnionUnion(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalRangeUnionConcat(WolValueImplSptr val1, 
                                       WolValueImplSptr val2);
   WolValueImplSptr evalUnionUnionUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2); 
   WolValueImplSptr evalUnionUnionConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2);
   WolValueImplSptr evalConcatUnionConcat(WolValueImplSptr val1,
                                        WolValueImplSptr val2);
   
   // symmetric functions for UNION operator
   WolValueImplSptr evalRangeUnionConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
      return evalConstUnionRange(val2, val1);
   }
   WolValueImplSptr evalUnionUnionConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
      return evalConstUnionUnion(val2, val1);
   }
   WolValueImplSptr evalConcatUnionConst(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
      return evalConstUnionConcat(val2, val1);
   }
   WolValueImplSptr evalUnionUnionRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
      return evalRangeUnionUnion(val2, val1);
   }
   WolValueImplSptr evalConcatUnionRange(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
      return evalRangeUnionConcat(val2, val1);
   }
   WolValueImplSptr evalConcatUnionUnion(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
      return evalUnionUnionConcat(val2, val1);
   }

   // primary functions for INTERSECTION operator evaluation
   WolValueImplSptr evalBoolIntersectionBool(WolValueImplSptr val1, 
                                    WolValueImplSptr val2);
   WolValueImplSptr evalConstIntersectionConst(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstIntersectionRange(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstIntersectionUnion(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalConstIntersectionConcat(WolValueImplSptr val1, 
                                       WolValueImplSptr val2);
   WolValueImplSptr evalRangeIntersectionRange(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalRangeIntersectionUnion(WolValueImplSptr val1, 
                                      WolValueImplSptr val2);
   WolValueImplSptr evalRangeIntersectionConcat(WolValueImplSptr val1, 
                                       WolValueImplSptr val2);
   WolValueImplSptr evalUnionIntersectionUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2); 
   WolValueImplSptr evalUnionIntersectionConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2);
   WolValueImplSptr evalConcatIntersectionConcat(WolValueImplSptr val1,
                                        WolValueImplSptr val2);
   
   // symmetric functions for INTERSECTION operator
   WolValueImplSptr evalRangeIntersectionConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
      return evalConstIntersectionRange(val2, val1);
   }
   WolValueImplSptr evalUnionIntersectionConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
      return evalConstIntersectionUnion(val2, val1);
   }
   WolValueImplSptr evalConcatIntersectionConst(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
      return evalConstIntersectionConcat(val2, val1);
   }
   WolValueImplSptr evalUnionIntersectionRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
      return evalRangeIntersectionUnion(val2, val1);
   }
   WolValueImplSptr evalConcatIntersectionRange(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
      return evalRangeIntersectionConcat(val2, val1);
   }
   WolValueImplSptr evalConcatIntersectionUnion(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
      return evalUnionIntersectionConcat(val2, val1);
   }

   // primary functions for Splice operator evaluation
   WolValueImplSptr evalBoolSplice(WolValueImplSptr val , int upper, int lower); 
   WolValueImplSptr evalConstSplice(WolValueImplSptr val, int upper, int lower); 
   WolValueImplSptr evalRangeSplice(WolValueImplSptr val, int upper, int lower);
   WolValueImplSptr evalUnionSplice(WolValueImplSptr val, int upper, int lower);
   WolValueImplSptr evalConcatSplice(WolValueImplSptr val, int upper, int lower);


   // primary functions for compliment operator evaluation
   WolValueImplSptr evalBoolCompliment(WolValueImplSptr val); 
   WolValueImplSptr evalConstCompliment(WolValueImplSptr val); 
   WolValueImplSptr evalRangeCompliment(WolValueImplSptr val);
   WolValueImplSptr evalUnionCompliment(WolValueImplSptr val);
   WolValueImplSptr evalConcatCompliment(WolValueImplSptr val);


   // functions for  Backward AND evaluation
   WolValueImplSptr evalBackBoolAndBool(WolValueImplSptr op_val,
                                    WolValueImplSptr operand_val);
   WolValueImplSptr evalBackConstAndConst(WolValueImplSptr op_val,
                                      WolValueImplSptr operand_val);
   WolValueImplSptr evalBackConstAndRange(WolValueImplSptr op_val,
                                      WolValueImplSptr operand_val);
   WolValueImplSptr evalBackConstAndUnion(WolValueImplSptr op_val,
                                      WolValueImplSptr operand_val);
   WolValueImplSptr evalBackConstAndConcat(WolValueImplSptr op_val,
                                       WolValueImplSptr operand_val);
   WolValueImplSptr evalBackRangeAndRange(WolValueImplSptr op_val,
                                      WolValueImplSptr operand_val);
   WolValueImplSptr evalBackRangeAndUnion(WolValueImplSptr op_val,
                                      WolValueImplSptr operand_val);
   WolValueImplSptr evalBackRangeAndConcat(WolValueImplSptr op_val,
                                       WolValueImplSptr operand_val);
   WolValueImplSptr evalBackUnionAndUnion(WolValueImplSptr op_val,
                                      WolValueImplSptr operand_val);
   WolValueImplSptr evalBackUnionAndConcat(WolValueImplSptr op_val,
                                       WolValueImplSptr operand_val);
   WolValueImplSptr evalBackConcatAndConcat(WolValueImplSptr op_val,
                                        WolValueImplSptr operand_val);
   WolValueImplSptr evalBackRangeAndConst(WolValueImplSptr op_val,
                                      WolValueImplSptr operand_val);
   WolValueImplSptr evalBackUnionAndConst(WolValueImplSptr op_val,
                                      WolValueImplSptr operand_val);
   WolValueImplSptr evalBackConcatAndConst(WolValueImplSptr op_val,
                                       WolValueImplSptr operand_val);
   WolValueImplSptr evalBackUnionAndRange(WolValueImplSptr op_val,
                                      WolValueImplSptr operand_val);
   WolValueImplSptr evalBackConcatAndRange(WolValueImplSptr op_val,
                                       WolValueImplSptr operand_val);
   WolValueImplSptr evalBackConcatAndUnion(WolValueImplSptr op_val,
                                       WolValueImplSptr operand_val);

private : //data
  
   // map for each operator.
   evalMap _andEvalMap;
   evalMap _beqEvalMap;
   evalMap _addEvalMap;
   evalMap _mulEvalMap;
   evalMap _sllEvalMap;
   evalMap _srlEvalMap;
   evalMap _udivEvalMap;
   evalMap _uremEvalMap;
   evalMap _concatEvalMap;
   evalSpliceMap _spliceEvalMap;
   evalMap _unionEvalMap;
   evalMap _intersectionEvalMap;
   eval1Map _complimentEvalMap; 

   evalMap _andBEvalMap;

};

}

#endif
