#include "wolvalueimpl.h"
#include "wolevalfactoryimpl.h"
#include <boost/dynamic_bitset.hpp>
#include "common.h"
#include <memory>
#include <iostream>

namespace wolver {

using namespace std;

typedef pair<WolValueImpl::WolValueImplType, WolValueImpl::WolValueImplType> typePair;


//------------------------------------------------------------------------------------------
// TOP LEVEL EVALUATION FUNCTIONS

WolValueSptr
WolEvalFactoryImpl::evalOp(evalMap &map, WolValueSptr val1, WolValueSptr val2) {

   WolValueImplSptr val_impl1 = dynamic_pointer_cast<WolValueImpl>(val1);
   WolValueImplSptr val_impl2 = dynamic_pointer_cast<WolValueImpl>(val2);

   WolValueImpl::WolValueImplType type1 = val_impl1->getImplType();
   WolValueImpl::WolValueImplType type2 = val_impl2->getImplType();

   evalFunctionPtr evalfun = lookup(map, type1, type2);

   return (this->*evalfun)(val_impl1, val_impl2);
}

WolValueSptr 
WolEvalFactoryImpl::evalAnd(WolValueSptr val1, WolValueSptr val2) {
   return evalOp(_andEvalMap, val1, val2);
}

WolValueSptr 
WolEvalFactoryImpl::evalBeq(WolValueSptr val1, WolValueSptr val2) {
   return evalOp(_beqEvalMap, val1, val2);
}

WolValueSptr 
WolEvalFactoryImpl::evalAdd(WolValueSptr val1, WolValueSptr val2) {
   return evalOp(_addEvalMap, val1, val2);
}

WolValueSptr 
WolEvalFactoryImpl::evalMul(WolValueSptr val1, WolValueSptr val2) {
   return evalOp(_mulEvalMap, val1, val2);
}

WolValueSptr 
WolEvalFactoryImpl::evalUlt(WolValueSptr val1, WolValueSptr val2) {
   return evalOp(_ultEvalMap, val1, val2);
}

WolValueSptr 
WolEvalFactoryImpl::evalSll(WolValueSptr val1, WolValueSptr val2) {
   return evalOp(_sllEvalMap, val1, val2);
}

WolValueSptr 
WolEvalFactoryImpl::evalSrl(WolValueSptr val1, WolValueSptr val2) {
   return evalOp(_srlEvalMap, val1, val2);
}

WolValueSptr 
WolEvalFactoryImpl::evalUdiv(WolValueSptr val1, WolValueSptr val2) {
   return evalOp(_udivEvalMap, val1, val2);
}

WolValueSptr 
WolEvalFactoryImpl::evalUrem(WolValueSptr val1, WolValueSptr val2) {
   return evalOp(_uremEvalMap, val1, val2);
}

WolValueSptr 
WolEvalFactoryImpl::evalConcat(WolValueSptr val1, WolValueSptr val2) {
   return evalOp(_concatEvalMap, val1, val2);
}

WolValueSptr 
WolEvalFactoryImpl::evalSplice(WolValueSptr val, int upper, int lower) {
   WolValueImplSptr val_impl = dynamic_pointer_cast<WolValueImpl>(val);

   evalSpliceFunctionPtr evalfun = lookup(_spliceEvalMap, val_impl->getImplType());

   return (this->*evalfun)(val_impl, upper, lower);
}

WolValueSptr 
WolEvalFactoryImpl::evalUnion(WolValueSptr val1, WolValueSptr val2) {
   return evalOp(_unionEvalMap, val1, val2);
}

WolValueSptr 
WolEvalFactoryImpl::evalIntersection(WolValueSptr val1, WolValueSptr val2) {
   return evalOp(_intersectionEvalMap, val1, val2);
}

WolValueSptr 
WolEvalFactoryImpl::evalCompliment(WolValueSptr val) {
   WolValueImplSptr val_impl = dynamic_pointer_cast<WolValueImpl>(val);

   eval1FunctionPtr evalfun = lookup(_complimentEvalMap, val_impl->getImplType());

   return (this->*evalfun)(val_impl);
}

//------------------------------------------------------------------------------------
// LOOK UP FUNCTIONS

evalFunctionPtr 
WolEvalFactoryImpl::lookup(evalMap &emap, WolValueImpl::WolValueImplType type1, 
                           WolValueImpl::WolValueImplType type2) {
   
   evalMap::iterator mapEntry = emap.find(std::make_pair(type1, type2));
 
   if(mapEntry == emap.end()) assert(0);
   
   return mapEntry->second;
}

eval1FunctionPtr 
WolEvalFactoryImpl::lookup(eval1Map &emap, WolValueImpl::WolValueImplType type) {

   eval1Map::iterator mapEntry = emap.find(type);

   if(mapEntry == emap.end()) assert(0);

   return mapEntry->second;
}

evalSpliceFunctionPtr 
WolEvalFactoryImpl::lookup(evalSpliceMap &emap, WolValueImpl::WolValueImplType type) {

   evalSpliceMap::iterator mapEntry = emap.find(type);

   if(mapEntry == emap.end()) assert(0);

   return mapEntry->second;
}

//---------------------------------------------------------------------------------------- 
// Map Intialization routines

void
WolEvalFactoryImpl::initializeAllMaps(){
   initializeAndEvalMap();
   initializeBeqEvalMap();
   initializeAddEvalMap();
   initializeMulEvalMap();
   initializeUltEvalMap();
   initializeSllEvalMap();
   initializeSrlEvalMap();
   initializeUdivEvalMap();
   initializeUremEvalMap();
   initializeConcatEvalMap();
   initializeSpliceEvalMap();
   initializeUnionEvalMap();
   initializeIntersectionEvalMap();
   initializeComplimentMap();
}

void
WolEvalFactoryImpl::initializeAndEvalMap() {

   _andEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,  
                                                   WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE), 
                                         &WolEvalFactoryImpl::evalBoolAndBool));    

   _andEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,  
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConstAndConst));
   _andEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,  
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConstAndRange));
   _andEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,  
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConstAndUnion));
   _andEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,  
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConstAndConcat));
   _andEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,  
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalRangeAndConst));
   _andEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,  
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalRangeAndRange));
   _andEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,  
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalRangeAndUnion));
   _andEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,  
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalRangeAndConcat));
   _andEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,  
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalUnionAndConst));
   _andEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalUnionAndRange));
   _andEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalUnionAndUnion));
   _andEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalUnionAndConcat));
   _andEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE, 
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConcatAndConst));
   _andEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConcatAndRange));
   _andEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConcatAndUnion));
   _andEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConcatAndConcat));
}


void 
WolEvalFactoryImpl::initializeBeqEvalMap(){
   _beqEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE), &WolEvalFactoryImpl::evalBoolBeqBool));
   _beqEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConstBeqConst));
   _beqEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConstBeqRange));
   _beqEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConstBeqUnion));
   _beqEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConstBeqConcat));
   _beqEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalRangeBeqConst));
   _beqEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalRangeBeqRange));
   _beqEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalRangeBeqUnion));
   _beqEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalRangeBeqConcat));
   _beqEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalUnionBeqConst));
   _beqEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalUnionBeqRange));
   _beqEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalUnionBeqUnion));
   _beqEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalUnionBeqConcat));
   _beqEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConcatBeqConst));
   _beqEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConcatBeqRange));
   _beqEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConcatBeqUnion));
   _beqEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConcatBeqConcat));
}


void 
WolEvalFactoryImpl::initializeAddEvalMap(){

   _addEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE), &WolEvalFactoryImpl::evalBoolAddBool));
   _addEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConstAddConst));
   _addEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConstAddRange));
   _addEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConstAddUnion));
   _addEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConstAddConcat));
   _addEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalRangeAddConst));
   _addEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalRangeAddRange));
   _addEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalRangeAddUnion));
   _addEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalRangeAddConcat));
   _addEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalUnionAddConst));
   _addEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalUnionAddRange));
   _addEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalUnionAddUnion));
   _addEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalUnionAddConcat));
   _addEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConcatAddConst));
   _addEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConcatAddRange));
   _addEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConcatAddUnion));
   _addEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConcatAddConcat));

}

void 
WolEvalFactoryImpl::initializeMulEvalMap() {
   _mulEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE), &WolEvalFactoryImpl::evalBoolMulBool));
   _mulEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConstMulConst));
   _mulEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConstMulRange));
   _mulEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConstMulUnion));
   _mulEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConstMulConcat));
   _mulEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalRangeMulConst));
   _mulEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalRangeMulRange));
   _mulEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalRangeMulUnion));
   _mulEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalRangeMulConcat));
   _mulEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalUnionMulConst));
   _mulEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalUnionMulRange));
   _mulEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalUnionMulUnion));
   _mulEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalUnionMulConcat));
   _mulEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConcatMulConst));
   _mulEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConcatMulRange));
   _mulEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConcatMulUnion));
   _mulEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConcatMulConcat));

}
 
void 
WolEvalFactoryImpl::initializeUltEvalMap() {

   _ultEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE), &WolEvalFactoryImpl::evalBoolUltBool));
   _ultEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConstUltConst));
   _ultEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConstUltRange));
   _ultEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConstUltUnion));
   _ultEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConstUltConcat));
   _ultEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalRangeUltConst));
   _ultEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalRangeUltRange));
   _ultEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalRangeUltUnion));
   _ultEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalRangeUltConcat));
   _ultEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalUnionUltConst));
   _ultEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalUnionUltRange));
   _ultEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalUnionUltUnion));
   _ultEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalUnionUltConcat));
   _ultEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConcatUltConst));
   _ultEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConcatUltRange));
   _ultEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConcatUltUnion));
   _ultEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConcatUltConcat));

}

void 
WolEvalFactoryImpl::initializeSllEvalMap() {

   _sllEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE), &WolEvalFactoryImpl::evalBoolSllBool));
   _sllEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConstSllConst));
   _sllEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConstSllRange));
   _sllEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConstSllUnion));
   _sllEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConstSllConcat));
   _sllEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalRangeSllConst));
   _sllEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalRangeSllRange));
   _sllEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalRangeSllUnion));
   _sllEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalRangeSllConcat));
   _sllEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalUnionSllConst));
   _sllEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalUnionSllRange));
   _sllEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalUnionSllUnion));
   _sllEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalUnionSllConcat));
   _sllEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConcatSllConst));
   _sllEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConcatSllRange));
   _sllEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConcatSllUnion));
   _sllEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConcatSllConcat));

}

void 
WolEvalFactoryImpl::initializeSrlEvalMap() {
   _srlEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE), &WolEvalFactoryImpl::evalBoolSrlBool));
   _srlEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConstSrlConst));
   _srlEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConstSrlRange));
   _srlEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConstSrlUnion));
   _srlEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConstSrlConcat));
   _srlEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalRangeSrlConst));
   _srlEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalRangeSrlRange));
   _srlEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalRangeSrlUnion));
   _srlEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalRangeSrlConcat));
   _srlEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalUnionSrlConst));
   _srlEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalUnionSrlRange));
   _srlEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalUnionSrlUnion));
   _srlEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalUnionSrlConcat));
   _srlEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConcatSrlConst));
   _srlEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConcatSrlRange));
   _srlEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConcatSrlUnion));
   _srlEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConcatSrlConcat));

}

void 
WolEvalFactoryImpl::initializeUdivEvalMap() {
   _udivEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE), &WolEvalFactoryImpl::evalBoolUdivBool));
   _udivEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConstUdivConst));
   _udivEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConstUdivRange));
   _udivEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConstUdivUnion));
   _udivEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConstUdivConcat));
   _udivEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalRangeUdivConst));
   _udivEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalRangeUdivRange));
   _udivEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalRangeUdivUnion));
   _udivEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalRangeUdivConcat));
   _udivEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalUnionUdivConst));
   _udivEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalUnionUdivRange));
   _udivEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalUnionUdivUnion));
   _udivEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalUnionUdivConcat));
   _udivEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConcatUdivConst));
   _udivEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConcatUdivRange));
   _udivEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConcatUdivUnion));
   _udivEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConcatUdivConcat));

}

void 
WolEvalFactoryImpl::initializeUremEvalMap() {
   _uremEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE), &WolEvalFactoryImpl::evalBoolUremBool));
   _uremEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConstUremConst));
   _uremEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConstUremRange));
   _uremEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConstUremUnion));
   _uremEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConstUremConcat));
   _uremEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalRangeUremConst));
   _uremEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalRangeUremRange));
   _uremEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalRangeUremUnion));
   _uremEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalRangeUremConcat));
   _uremEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalUnionUremConst));
   _uremEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalUnionUremRange));
   _uremEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalUnionUremUnion));
   _uremEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalUnionUremConcat));
   _uremEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConcatUremConst));
   _uremEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConcatUremRange));
   _uremEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConcatUremUnion));
   _uremEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConcatUremConcat));

}

void 
WolEvalFactoryImpl::initializeConcatEvalMap(){
   _concatEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE), &WolEvalFactoryImpl::evalBoolConcatBool));
   _concatEvalMap.insert(evalMap::value_type(std::make_pair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConstConcatConst));
   _concatEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConstConcatRange));
   _concatEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConstConcatUnion));
   _concatEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConstConcatConcat));
   _concatEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalRangeConcatConst));
   _concatEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalRangeConcatRange));
   _concatEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalRangeConcatUnion));
   _concatEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalRangeConcatConcat));
   _concatEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalUnionConcatConst));
   _concatEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalUnionConcatRange));
   _concatEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalUnionConcatUnion));
   _concatEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalUnionConcatConcat));
   _concatEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConcatConcatConst));
   _concatEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConcatConcatRange));
   _concatEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConcatConcatUnion));
   _concatEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConcatConcatConcat));

}

void 
WolEvalFactoryImpl::initializeSpliceEvalMap(){
  _spliceEvalMap.insert(evalSpliceMap::value_type(WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
                                                  &WolEvalFactoryImpl::evalBoolSplice));
  _spliceEvalMap.insert(evalSpliceMap::value_type(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE, 
                                                  &WolEvalFactoryImpl::evalConstSplice));
  _spliceEvalMap.insert(evalSpliceMap::value_type(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE, 
                                                  &WolEvalFactoryImpl::evalRangeSplice));
  _spliceEvalMap.insert(evalSpliceMap::value_type(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE, 
                                                  &WolEvalFactoryImpl::evalUnionSplice));
  _spliceEvalMap.insert(evalSpliceMap::value_type(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE, 
                                                  &WolEvalFactoryImpl::evalConcatSplice)); 
}

void 
WolEvalFactoryImpl::initializeUnionEvalMap(){
   _unionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE), &WolEvalFactoryImpl::evalBoolUnionBool));
   _unionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConstUnionConst));
   _unionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConstUnionRange));
   _unionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConstUnionUnion));
   _unionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConstUnionConcat));
   _unionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalRangeUnionConst));
   _unionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalRangeUnionRange));
   _unionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalRangeUnionUnion));
   _unionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalRangeUnionConcat));
   _unionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalUnionUnionConst));
   _unionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalUnionUnionRange));
   _unionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalUnionUnionUnion));
   _unionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalUnionUnionConcat));
   _unionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConcatUnionConst));
   _unionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConcatUnionRange));
   _unionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConcatUnionUnion));
   _unionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConcatUnionConcat));

}

void 
WolEvalFactoryImpl::initializeIntersectionEvalMap(){
   _intersectionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE), &WolEvalFactoryImpl::evalBoolIntersectionBool));
   _intersectionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConstIntersectionConst));
   _intersectionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConstIntersectionRange));
   _intersectionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConstIntersectionUnion));
   _intersectionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConstIntersectionConcat));
   _intersectionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalRangeIntersectionConst));
   _intersectionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalRangeIntersectionRange));
   _intersectionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalRangeIntersectionUnion));
   _intersectionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalRangeIntersectionConcat));
   _intersectionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalUnionIntersectionConst));
   _intersectionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalUnionIntersectionRange));
   _intersectionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalUnionIntersectionUnion));
   _intersectionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalUnionIntersectionConcat));
   _intersectionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE), &WolEvalFactoryImpl::evalConcatIntersectionConst));
   _intersectionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE), &WolEvalFactoryImpl::evalConcatIntersectionRange));
   _intersectionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE), &WolEvalFactoryImpl::evalConcatIntersectionUnion));
   _intersectionEvalMap.insert(evalMap::value_type(typePair(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                        WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE), &WolEvalFactoryImpl::evalConcatIntersectionConcat));
}

void 
WolEvalFactoryImpl::initializeComplimentMap(){
  _complimentEvalMap.insert(eval1Map::value_type(WolValueImpl::WOL_VALUE_IMPL_BOOL_TYPE, 
                                                  &WolEvalFactoryImpl::evalBoolCompliment));   
  _complimentEvalMap.insert(eval1Map::value_type(WolValueImpl::WOL_VALUE_IMPL_CONST_TYPE,
                                                  &WolEvalFactoryImpl::evalConstCompliment));
  _complimentEvalMap.insert(eval1Map::value_type(WolValueImpl::WOL_VALUE_IMPL_RANGE_TYPE,
                                                  &WolEvalFactoryImpl::evalRangeCompliment));
  _complimentEvalMap.insert(eval1Map::value_type(WolValueImpl::WOL_VALUE_IMPL_UNION_TYPE,
                                                  &WolEvalFactoryImpl::evalUnionCompliment));
  _complimentEvalMap.insert(eval1Map::value_type(WolValueImpl::WOL_VALUE_IMPL_CONCAT_TYPE,
                                                  &WolEvalFactoryImpl::evalConcatCompliment));
}
//------------------------------------------------------------------------------------------------
// AND EVAlUATION ROUTINES

WolValueImplSptr 
WolEvalFactoryImpl::evalBoolAndBool(WolValueImplSptr val1,
                                    WolValueImplSptr val2){
   
   WolBoolValueImplSptr bool_val1 = dynamic_pointer_cast<WolBoolValueImpl> (val1);
   WolBoolValueImplSptr bool_val2 = dynamic_pointer_cast<WolBoolValueImpl> (val2);

   bool result = bool_val1->_value & bool_val2->_value;

   WolBoolValueImplSptr retValue(new WolBoolValueImpl(result));
   return static_pointer_cast<WolValueImpl>(retValue);
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstAndConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2){
 
   WolConstValueImplSptr const_val1 = dynamic_pointer_cast<WolConstValueImpl> (val1);
   WolConstValueImplSptr const_val2 = dynamic_pointer_cast<WolConstValueImpl> (val2);
 
   boost::dynamic_bitset<> result = const_val1->_value & const_val2->_value;  
 
   WolConstValueImplSptr retValue(new WolConstValueImpl(result));
   return static_pointer_cast<WolValueImpl>(retValue);
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstAndRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2){
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstAndUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2){
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstAndConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2){
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeAndRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2){
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeAndUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2){
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeAndConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2){
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalUnionAndUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2){
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalUnionAndConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2){
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConcatAndConcat(WolValueImplSptr val1,
                                        WolValueImplSptr val2){
   assert(0);
   return nullptr;
}


// primary functions for BEQ operations
WolValueImplSptr 
WolEvalFactoryImpl::evalBoolBeqBool(WolValueImplSptr val1,
                                    WolValueImplSptr val2) {

   WolBoolValueImplSptr bool_val1 = dynamic_pointer_cast<WolBoolValueImpl> (val1);
   WolBoolValueImplSptr bool_val2 = dynamic_pointer_cast<WolBoolValueImpl> (val2);

   bool result = (bool_val1->_value == bool_val2->_value);

   WolBoolValueImplSptr retValue(new WolBoolValueImpl(result));
   return static_pointer_cast<WolValueImpl>(retValue);
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstBeqConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {

   WolConstValueImplSptr const_val1 = dynamic_pointer_cast<WolConstValueImpl> (val1);
   WolConstValueImplSptr const_val2 = dynamic_pointer_cast<WolConstValueImpl> (val2);

   bool result = (const_val1->_value == const_val2->_value);

   WolBoolValueImplSptr retValue(new WolBoolValueImpl(result));
   return static_pointer_cast<WolValueImpl>(retValue);
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstBeqRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstBeqUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstBeqConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2){
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeBeqRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeBeqUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeBeqConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalUnionBeqUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalUnionBeqConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConcatBeqConcat(WolValueImplSptr val1,
                                        WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

// primary functions for ADD operator WolEvalFactoryImpl::evaluation
WolValueImplSptr 
WolEvalFactoryImpl::evalBoolAddBool(WolValueImplSptr val1,
                                    WolValueImplSptr val2) {
   
   WolBoolValueImplSptr bool_val1 = dynamic_pointer_cast<WolBoolValueImpl> (val1);
   WolBoolValueImplSptr bool_val2 = dynamic_pointer_cast<WolBoolValueImpl> (val2);

   bool result = (bool_val1->_value ^ bool_val2->_value);

   WolBoolValueImplSptr retValue(new WolBoolValueImpl(result));
   return static_pointer_cast<WolValueImpl>(retValue);
}

static
bool fullAdder(bool b1, bool b2, bool& carry) {
 
   bool sum = (b1 ^ b2) ^ carry;
   carry = (b1 && b2) || (b1 && carry) || (b2 && carry);
   return sum;
}

static 
boost::dynamic_bitset<> bitSetAdd(boost::dynamic_bitset<> val1, 
                                  boost::dynamic_bitset<> val2) {
   
   assert(val1.size() == val2.size());
   
   int length = val1.size();
   boost::dynamic_bitset<> result (length);
   bool carry = false;

   for (int i = 0; i < length; ++i)
      result[i] = fullAdder((bool)val1[i], (bool)val2[i], carry);

   return result;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstAddConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2){

   WolConstValueImplSptr const_val1 = dynamic_pointer_cast<WolConstValueImpl> (val1);
   WolConstValueImplSptr const_val2 = dynamic_pointer_cast<WolConstValueImpl> (val2);
   
   boost::dynamic_bitset<> result = bitSetAdd(const_val1->_value, const_val2->_value);

   WolConstValueImplSptr retValue(new WolConstValueImpl(result));
   return static_pointer_cast<WolValueImpl>(retValue); 
}


WolValueImplSptr 
WolEvalFactoryImpl::evalConstAddRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2){
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstAddUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstAddConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2){
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeAddRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeAddUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeAddConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalUnionAddUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalUnionAddConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConcatAddConcat(WolValueImplSptr val1,
                                        WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}


   // primary functions for MUL operator WolEvalFactoryImpl::evaluation
WolValueImplSptr 
WolEvalFactoryImpl::evalBoolMulBool(WolValueImplSptr val1,
                                    WolValueImplSptr val2) {

   WolBoolValueImplSptr bool_val1 = dynamic_pointer_cast<WolBoolValueImpl> (val1);
   WolBoolValueImplSptr bool_val2 = dynamic_pointer_cast<WolBoolValueImpl> (val2);

   bool result = bool_val1->_value & bool_val2->_value;

   WolBoolValueImplSptr retValue(new WolBoolValueImpl(result));
   return static_pointer_cast<WolValueImpl>(retValue);
}

static
boost::dynamic_bitset<> bitSetMul(boost::dynamic_bitset<> val1,
                                boost::dynamic_bitset<> val2) {

   assert(val1.size() == val2.size());

   int length = val1.size();
   boost::dynamic_bitset<>result(length);

   // we want to minimize the number of times we shift and add
   if (val1.count() < val2.count()) {
      for (int i = 0; i < length; i++)
         if (val1[i]) result = bitSetAdd(result, val2 << i);
   }
   else {
      for (int i = 0; i < length; i++)
         if (val2[i]) result = bitSetAdd(result, val1 << i);
   }

   return result;
}


WolValueImplSptr 
WolEvalFactoryImpl::evalConstMulConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
  
   WolConstValueImplSptr const_val1 = dynamic_pointer_cast<WolConstValueImpl> (val1);
   WolConstValueImplSptr const_val2 = dynamic_pointer_cast<WolConstValueImpl> (val2);

   boost::dynamic_bitset<> result = bitSetMul(const_val1->_value,const_val2->_value);

   WolConstValueImplSptr retValue(new WolConstValueImpl(result));
   return static_pointer_cast<WolValueImpl>(retValue);
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstMulRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2){
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstMulUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2){
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstMulConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2){
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeMulRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2){
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeMulUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2){
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeMulConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalUnionMulUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalUnionMulConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConcatMulConcat(WolValueImplSptr val1,
                                        WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}


// primary functions for ULT operator WolEvalFactoryImpl::evaluation
WolValueImplSptr 
WolEvalFactoryImpl::evalBoolUltBool(WolValueImplSptr val1,
                                    WolValueImplSptr val2) {

   WolBoolValueImplSptr bool_val1 = dynamic_pointer_cast<WolBoolValueImpl> (val1);
   WolBoolValueImplSptr bool_val2 = dynamic_pointer_cast<WolBoolValueImpl> (val2);

   bool result = bool_val1->_value < bool_val2->_value;

   WolBoolValueImplSptr retValue(new WolBoolValueImpl(result));
   return static_pointer_cast<WolValueImpl>(retValue);
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstUltConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
 
   WolConstValueImplSptr const_val1 = dynamic_pointer_cast<WolConstValueImpl> (val1);
   WolConstValueImplSptr const_val2 = dynamic_pointer_cast<WolConstValueImpl> (val2);

   bool result = const_val1->_value < const_val2->_value;

   WolBoolValueImplSptr retValue(new WolBoolValueImpl(result));
   return static_pointer_cast<WolValueImpl>(retValue);
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstUltRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstUltUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstUltConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeUltRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeUltUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeUltConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalUnionUltUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalUnionUltConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConcatUltConcat(WolValueImplSptr val1,
                                        WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeUltConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalUnionUltConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConcatUltConst(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalUnionUltRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConcatUltRange(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConcatUltUnion(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {
   assert(0);
   return nullptr;
}
 
     // primary functions for SLL operator WolEvalFactoryImpl::evaluation
WolValueImplSptr 
WolEvalFactoryImpl::evalBoolSllBool(WolValueImplSptr val1,
                                    WolValueImplSptr val2) {
   
   WolBoolValueImplSptr bool_val1 = dynamic_pointer_cast<WolBoolValueImpl> (val1);
   WolBoolValueImplSptr bool_val2 = dynamic_pointer_cast<WolBoolValueImpl> (val2);

   bool result = (bool_val1->_value << bool_val2->_value);

   WolBoolValueImplSptr retValue(new WolBoolValueImpl(result));
   return static_pointer_cast<WolValueImpl>(retValue);
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstSllConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {

   WolConstValueImplSptr const_val1 = dynamic_pointer_cast<WolConstValueImpl> (val1);
   WolConstValueImplSptr const_val2 = dynamic_pointer_cast<WolConstValueImpl> (val2);

   boost::dynamic_bitset<> result = const_val1->_value << const_val2->_value.to_ulong();

   WolConstValueImplSptr retValue(new WolConstValueImpl(result));
   return static_pointer_cast<WolValueImpl>(retValue);

}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstSllRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConstSllUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConstSllConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalRangeSllRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalRangeSllUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalRangeSllConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalUnionSllUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalUnionSllConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConcatSllConcat(WolValueImplSptr val1,
                                        WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalRangeSllConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalUnionSllConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConcatSllConst(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalUnionSllRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConcatSllRange(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConcatSllUnion(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}


// primary functions for SRL operator WolEvalFactoryImpl::evaluation
WolValueImplSptr 
WolEvalFactoryImpl::evalBoolSrlBool(WolValueImplSptr val1,
                                    WolValueImplSptr val2) {

   WolBoolValueImplSptr bool_val1 = dynamic_pointer_cast<WolBoolValueImpl> (val1);
   WolBoolValueImplSptr bool_val2 = dynamic_pointer_cast<WolBoolValueImpl> (val2);

   bool result = bool_val1->_value >> bool_val2->_value;

   WolBoolValueImplSptr retValue(new WolBoolValueImpl(result));
   return static_pointer_cast<WolValueImpl>(retValue);
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstSrlConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2) {

   WolConstValueImplSptr const_val1 = dynamic_pointer_cast<WolConstValueImpl> (val1);
   WolConstValueImplSptr const_val2 = dynamic_pointer_cast<WolConstValueImpl> (val2);

   boost::dynamic_bitset<> result = const_val1->_value >> const_val2->_value.to_ulong();

   WolConstValueImplSptr retValue(new WolConstValueImpl(result));
   return static_pointer_cast<WolValueImpl>(retValue);
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstSrlRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConstSrlUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConstSrlConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalRangeSrlRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalRangeSrlUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalRangeSrlConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalUnionSrlUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalUnionSrlConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConcatSrlConcat(WolValueImplSptr val1,
                                        WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalRangeSrlConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalUnionSrlConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConcatSrlConst(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalUnionSrlRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConcatSrlRange(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConcatSrlUnion(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

   // primary functions for UDIV operator WolEvalFactoryImpl::evaluation
WolValueImplSptr 
WolEvalFactoryImpl::evalBoolUdivBool(WolValueImplSptr val1,
                                    WolValueImplSptr val2) {
   WolBoolValueImplSptr bool_val1 = dynamic_pointer_cast<WolBoolValueImpl> (val1);
   WolBoolValueImplSptr bool_val2 = dynamic_pointer_cast<WolBoolValueImpl> (val2);

   if (bool_val2->_value == 0) assert(0);
   bool result = bool_val1->_value;

   WolBoolValueImplSptr retValue(new WolBoolValueImpl(result));
   return static_pointer_cast<WolValueImpl>(retValue);
}

static
boost::dynamic_bitset<> bitSetSub(boost::dynamic_bitset<> val1,
                                       boost::dynamic_bitset<> val2) {

   assert(val1.size() == val2.size());

   int length = val1.size();
   boost::dynamic_bitset<>result(length);
   bool borrow = false;
   for (int i = 0; i < length; i++) {
      if (borrow) {
         if (val1[i]) {
            result[i] = val2[i];
            borrow = val2[i];
         }
         else {
            result[i] = 1 - val2[i];
            borrow = true;
         }
 
      }
      else {
         if (val1[i]) {
            result[i] = 1 - val2[i];
            borrow = false;
         }
         else {
            result[i] = val2[i];
            borrow = val2[i];
         }
      }
   }
   return result;
}

static
boost::dynamic_bitset<> bitSetDiv(boost::dynamic_bitset<> val1,
                                  boost::dynamic_bitset<> val2,
                                  bool remainder) {

   assert(val1.size() == val2.size());

   int length = val1.size();
   boost::dynamic_bitset<>q(length);
   boost::dynamic_bitset<>r(length);

   q.reset();
   r.reset();

   if (val2 == q) {
       assert(0); return q;
   }

   if (val1 == q) {
       return q;
   }

   if (val1 == val2) {
     if (!remainder) q[0] = 1;
     return q;
   }

   r = val1;
   if (val1 < val2)
   {
      if (!remainder) return q;
      else return r;
   }
 
   // count significant digits in divisor and dividend
   unsigned int sig_val1 = 0;
   for (int i=length-1; i>=0; i--) {
      sig_val1 = i;
      if (val1[i]) break;
   }

   unsigned int sig_val2 = 0;
   for (int i=length-1; i>=0; i--) {
      sig_val2 = i;
      if (val2[i]) break;
   }

   // align the divisor with the dividend
   unsigned int n = (sig_val1 - sig_val2);
   val2 <<= n;
   // make sure the loop executes the right number of times
   n += 1;
   // long division algorithm, shift, and subtract
   while (n--){
      // shift the quotient to the left
      if (val2 <= r) {
         // add a new digit to quotient
         q[n] = true;
         r = bitSetSub(r, val2);
      }
      // shift the divisor to the right
      val2 >>= 1;
   }

   if (remainder) return r;
   else return  q;
}
  


WolValueImplSptr 
WolEvalFactoryImpl::evalConstUdivConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   WolConstValueImplSptr const_val1 = dynamic_pointer_cast<WolConstValueImpl> (val1);
   WolConstValueImplSptr const_val2 = dynamic_pointer_cast<WolConstValueImpl> (val2);

   boost::dynamic_bitset<> result = bitSetDiv(const_val1->_value,
                                              const_val2->_value,
                                              false);

   WolConstValueImplSptr retValue(new WolConstValueImpl(result));
   return static_pointer_cast<WolValueImpl>(retValue);
    
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstUdivRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConstUdivUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConstUdivConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalRangeUdivRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalRangeUdivUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalRangeUdivConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalUnionUdivUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalUnionUdivConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConcatUdivConcat(WolValueImplSptr val1,
                                        WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalRangeUdivConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalUnionUdivConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConcatUdivConst(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalUnionUdivRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConcatUdivRange(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConcatUdivUnion(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

       // primary functions for UREM operator WolEvalFactoryImpl::evaluation
WolValueImplSptr 
WolEvalFactoryImpl::evalBoolUremBool(WolValueImplSptr val1,
                                     WolValueImplSptr val2)
{

   WolBoolValueImplSptr bool_val2 = dynamic_pointer_cast<WolBoolValueImpl> (val2);

   if (bool_val2->_value == 0) assert(0);
   bool result = false;

   WolBoolValueImplSptr retValue(new WolBoolValueImpl(result));
   return static_pointer_cast<WolValueImpl>(retValue);
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstUremConst(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   WolConstValueImplSptr const_val1 = dynamic_pointer_cast<WolConstValueImpl> (val1);
   WolConstValueImplSptr const_val2 = dynamic_pointer_cast<WolConstValueImpl> (val2);

   boost::dynamic_bitset<> result = bitSetDiv(const_val1->_value,
                                              const_val2->_value,
                                              true);

   WolConstValueImplSptr retValue(new WolConstValueImpl(result));
   return static_pointer_cast<WolValueImpl>(retValue);
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstUremRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConstUremUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConstUremConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalRangeUremRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalRangeUremUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalRangeUremConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalUnionUremUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalUnionUremConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConcatUremConcat(WolValueImplSptr val1,
                                        WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalRangeUremConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalUnionUremConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConcatUremConst(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalUnionUremRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConcatUremRange(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConcatUremUnion(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}


// primary functions for CONCAT operator WolEvalFactoryImpl::evaluation
WolValueImplSptr 
WolEvalFactoryImpl::evalBoolConcatBool(WolValueImplSptr val1,
                                       WolValueImplSptr val2) {

   WolBoolValueImplSptr bool_val1 = dynamic_pointer_cast<WolBoolValueImpl> (val1);
   WolBoolValueImplSptr bool_val2 = dynamic_pointer_cast<WolBoolValueImpl> (val2);

   boost::dynamic_bitset<> result(2);
   result[0] = bool_val1->_value;
   result[1] = bool_val2->_value;

   WolConstValueImplSptr retValue(new WolConstValueImpl(result));
   return static_pointer_cast<WolValueImpl>(retValue);
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstConcatConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   WolConstValueImplSptr const_val1 = dynamic_pointer_cast<WolConstValueImpl> (val1);
   WolConstValueImplSptr const_val2 = dynamic_pointer_cast<WolConstValueImpl> (val2);

   int length1 = const_val1->_value.size();
   int length2 = const_val2->_value.size();

   boost::dynamic_bitset<> result(length1 + length2);
   int index = 0;
  
   for (int i = 0; i < length2; i++) {
      result[index] = const_val2->_value[i]; 
      index++;
   }
   
   for (int i = 0; i < length1; i++) {
      result[index] = const_val1->_value[i];
      index++;
   }

   WolConstValueImplSptr retValue(new WolConstValueImpl(result));
   return static_pointer_cast<WolValueImpl>(retValue);
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstConcatRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConstConcatUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConstConcatConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalRangeConcatRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalRangeConcatUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalRangeConcatConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalUnionConcatUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalUnionConcatConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConcatConcatConcat(WolValueImplSptr val1,
                                        WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalRangeConcatConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalUnionConcatConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalConcatConcatConst(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}
WolValueImplSptr 
WolEvalFactoryImpl::evalUnionConcatRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConcatConcatRange(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConcatConcatUnion(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}


   // primary functions for UNION operator WolEvalFactoryImpl::evaluation
WolValueImplSptr 
WolEvalFactoryImpl::evalBoolUnionBool(WolValueImplSptr val1,
                                    WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstUnionConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstUnionRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstUnionUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstUnionConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeUnionRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeUnionUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeUnionConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalUnionUnionUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalUnionUnionConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConcatUnionConcat(WolValueImplSptr val1,
                                        WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

    // primary functions for INTERSECTION operator WolEvalFactoryImpl::evaluation
WolValueImplSptr 
WolEvalFactoryImpl::evalBoolIntersectionBool(WolValueImplSptr val1,
                                    WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstIntersectionConst(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstIntersectionRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstIntersectionUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstIntersectionConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeIntersectionRange(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeIntersectionUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeIntersectionConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalUnionIntersectionUnion(WolValueImplSptr val1,
                                      WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalUnionIntersectionConcat(WolValueImplSptr val1,
                                       WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConcatIntersectionConcat(WolValueImplSptr val1,
                                        WolValueImplSptr val2)
{
   assert(0);
   return nullptr;
}


   // primary functions for Splice operator WolEvalFactoryImpl::evaluation
WolValueImplSptr 
WolEvalFactoryImpl::evalBoolSplice(WolValueImplSptr val, int upper, int lower)
{
   assert(upper == lower);
   return val;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstSplice(WolValueImplSptr val, int upper, int lower)
{
   WolConstValueImplSptr const_val = dynamic_pointer_cast<WolConstValueImpl> (val);
   int length = const_val->_value.size();

   assert(length >= upper - lower + 1);
   boost::dynamic_bitset<> result(upper- lower + 1);
   int index = 0;

   for (int i = lower; i <= upper; i++) {
      result[index] = const_val->_value[i];
      index++;
   }
  
   WolConstValueImplSptr retValue(new WolConstValueImpl(result));
   return static_pointer_cast<WolValueImpl>(retValue);
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeSplice(WolValueImplSptr val, int upper, int lower)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalUnionSplice(WolValueImplSptr val, int upper, int lower)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConcatSplice(WolValueImplSptr val, int upper, int lower)
{
   assert(0);
   return nullptr;
}


   // primary functions for compliment operator WolEvalFactoryImpl::evaluation
WolValueImplSptr 
WolEvalFactoryImpl::evalBoolCompliment(WolValueImplSptr val)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalConstCompliment(WolValueImplSptr val)
{


   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalRangeCompliment(WolValueImplSptr val)
{
   assert(0);
   return nullptr;
}

WolValueImplSptr 
WolEvalFactoryImpl::evalUnionCompliment(WolValueImplSptr val)
{
   assert(0);
   return nullptr;
}


WolValueImplSptr 
WolEvalFactoryImpl::evalConcatCompliment(WolValueImplSptr val) {
   assert(0);
   return nullptr;
}


}
