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
   ~WolEvalFactory() {}
   
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

private : //data


};

}

#endif
