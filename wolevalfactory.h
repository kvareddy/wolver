#ifndef WOLEVALFACTORY_H_INCLUDED
#define WOLEVALFACTORY_H_INCLUDED

#include<iostream>

namespace wolver
{

using namespace std;

class WolValue;
class WolMgr;

class WolEvalFactory {

public : 
   WolEvalFactory() {}
   virtual ~WolEvalFactory() {}
   
public : //methods

   virtual WolValueSptr evalAnd(WolValueSptr val1, WolValueSptr val2) = 0;
   virtual WolValueSptr evalBeq(WolValueSptr val1, WolValueSptr val2) = 0;
   virtual WolValueSptr evalAdd(WolValueSptr val1, WolValueSptr val2) = 0;
   virtual WolValueSptr evalMul(WolValueSptr val1, WolValueSptr val2) = 0;
   virtual WolValueSptr evalUlt(WolValueSptr val1, WolValueSptr val2) = 0;
   virtual WolValueSptr evalSll(WolValueSptr val1, WolValueSptr val2) = 0;
   virtual WolValueSptr evalSrl(WolValueSptr val1, WolValueSptr val2) = 0;
   virtual WolValueSptr evalUdiv(WolValueSptr val1, WolValueSptr val2) = 0;
   virtual WolValueSptr evalUrem(WolValueSptr val1, WolValueSptr val2) = 0;
   virtual WolValueSptr evalConcat(WolValueSptr val1, WolValueSptr val2) = 0;
   virtual WolValueSptr evalSplice(WolValueSptr val, int upper, int lower) = 0;
   virtual WolValueSptr evalUnion(WolValueSptr val1, WolValueSptr val2) = 0;
   virtual WolValueSptr evalIntersection(WolValueSptr val1, WolValueSptr val2) = 0;
   virtual WolValueSptr evalDiff(WolValueSptr val1, WolValueSptr val2) = 0;
   virtual  WolValueSptr evalCompliment(WolValueSptr val) = 0;
   virtual  WolValueSptr evalCond(WolValueSptr cond, WolValueSptr val1, WolValueSptr val2) = 0;

   virtual WolValueSptr evalUltB(WolValueSptr op_val, WolValueSptr operand_val, bool implyLeft) = 0;
   virtual WolValueSptr evalAndB(WolValueSptr op_val, WolValueSptr operand_val) = 0;
   virtual WolValueSptr evalBeqB(WolValueSptr op_val, WolValueSptr operand_val) = 0;
   virtual WolValueSptr evalConcatB(WolValueSptr op_val, WolValueSptr operand_val, bool implyLeft) = 0;
   virtual WolValueSptr evalSpliceB(WolValueSptr op_val, int upper, int lower, int width) = 0;
   // position = 0 => backward implication for bool condition
   // position = 1 => backward implication for if branch
   // position = 2 => backward implication for else branch
   virtual WolValueSptr evalCondB(WolValueSptr op_val, WolValueSptr operand_val1,
                                  WolValueSptr operand_val2, int position) = 0;

private : //data


};

}

#endif
