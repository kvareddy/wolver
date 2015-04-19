#include "wolmgr.h"
#include "wolexp.h"
#include "wolvalue.h"
#include "wolexpfactory.h"
#include "wolevalfactory.h"
#include "wolvaluefactory.h"
#include <sstream>
#include <bitset>
#include <assert.h>
#include <string.h>
#include <memory>

namespace wolver {

using namespace std;

//-----------------------------------------------------------------------------
static const char* hex_char_to_bin(char c) {

    // TODO handle default / error
    switch(toupper(c))
    {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
    }
   return "0000";
}

WolNodeSptr WolExpFactory::makeConstExpr(std::string name, int denom) {

   // convert name in to binary string
   if (denom == 16) {

      std::string bin;
      for(unsigned i = 0; i != name.length(); ++i)
         bin += hex_char_to_bin(name[i]);
      name = bin;
   }
   else 
      assert (denom == 2);

   WolNodeSptr constExpr;

   // check if the constEpxr is already present
   if ((constExpr = WolMgr::getInstance().findConstExpr(name)) == nullptr) {

      // build the constexpr
      constExpr = WolNodeSptr(
          new WolNode(WolNodeType::WOL_BV_CONST_NODE, name.length()));
      constExpr->setName(name);
      constExpr->setValue(WolMgr::getInstance().
                          getValueFactory()->makeConstValue(name));
      WolMgr::getInstance().insertConstExpr(constExpr);
      WolMgr::getInstance().insertIdExpr(constExpr);
   }

   return constExpr;
}

WolNodeSptr WolExpFactory::makeZeroExpr(int len) {

   assert(len > 0);
   string name(len, '0');
   return makeConstExpr(name, 2);   
}

WolNodeSptr WolExpFactory::makeOneExpr(int len) {

   assert(len > 0);

   if (len == 1) 
      return makeConstExpr("1", 2);
   else {

      string name(len-1, '0');
      name.push_back('1');

      return makeConstExpr(name, 2);
   }
}

WolNodeSptr WolExpFactory::makeFalseExpr() {

   return makeZeroExpr(1);
}

WolNodeSptr WolExpFactory::makeOnesExpr(int len) {

   assert(len > 0);

   string name(len, '1');
   return makeConstExpr(name, 2);
}

WolNodeSptr WolExpFactory::makeTrueExpr() {

   return makeOnesExpr(1);
}

WolNodeSptr WolExpFactory::makeVarExpr(int len, std::string symbol) {

   WolNodeSptr varExpr(new WolNode(WolNodeType::WOL_BV_VAR_NODE, len));
   varExpr->setName(symbol);
   WolMgr::getInstance().insertIdExpr(varExpr);

   return varExpr;
}

static WolNodeSptr findExprInt(WolNodeSptr exp, std::string name) {

   WolNodeSptr retExp;
   if ((retExp = WolMgr::getInstance().findUniqueExpr(exp)) == nullptr) {

      retExp = exp;
      retExp->setName(name);
      WolMgr::getInstance().insertUniqueExpr(retExp);
      WolMgr::getInstance().insertIdExpr(retExp);
   
      int arity = retExp->getArity();   

      for(int i = 0; i < arity; i++) {
                                                 
        dynamic_pointer_cast<WolComplexNode>(retExp)->
            getChildren()[i]->addParent(
            retExp);
      }
   }

   return retExp; 
}

static WolNodeSptr findSliceExprInt(WolNodeSptr exp, std::string name) {

   WolNodeSptr retExp;
   if ((retExp = WolMgr::getInstance().findSliceExpr(exp)) == nullptr) {

      retExp = exp;
      retExp->setName(name);
      WolMgr::getInstance().insertUniqueSliceExpr(retExp);
      WolMgr::getInstance().insertIdExpr(retExp);
  
      int arity = retExp->getArity();   

      for(int i = 0; i < arity; i++) {

        dynamic_pointer_cast<WolComplexNode>(retExp)->getChildren()[i]->
            addParent(retExp);
      }
   }

   return retExp;
}

WolNodeSptr WolExpFactory::makeNotExprInt(WolNodeSptr expr) {
   
   WolNodeSptr notExpr(new WolComplexNode(WolNodeType::WOL_BV_NOT_NODE,
                                          expr->getPrecision(), expr));

   return findExprInt(notExpr, "not");
}

WolNodeSptr WolExpFactory::makeNotExpr(WolNodeSptr expr) {

   return rewriteNotExpr(expr);   
}


WolNodeSptr WolExpFactory::makeNegExpr(WolNodeSptr expr) {

   WolNodeSptr oneExpr = makeOneExpr(expr->getPrecision());
   WolNodeSptr negExpr = makeNotExpr(expr);
   WolNodeSptr addExpr = makeAddExpr(negExpr, oneExpr);

   return addExpr;
}
 
WolNodeSptr WolExpFactory::makeRedorExpr(WolNodeSptr expr) {
// TODO   
   
   return expr;
}

WolNodeSptr WolExpFactory::makeRedxorExpr(WolNodeSptr expr){
// TODO

   return expr;
}

WolNodeSptr WolExpFactory::makeRedandExpr(WolNodeSptr expr) {
// TODO

   return expr;
}

WolNodeSptr WolExpFactory::makeSliceExprInt(WolNodeSptr expr, int upper, 
                                            int lower) {
   
   assert(upper >= lower);
   assert(expr);

   WolNodeSptr sliceExpr(
       new WolComplexNode(WolNodeType::WOL_SLICE_NODE, upper-lower+1, upper,
           lower, expr));
   ostringstream os;
   os << "[" << upper << "," << lower << "]";
   string name = os.str(); 

   return findSliceExprInt(sliceExpr, name);
}

WolNodeSptr WolExpFactory::makeSliceExpr(WolNodeSptr expr,
                                         int upper,
                                         int lower) {
   
   return rewriteSliceExpr(expr, upper, lower);
}

WolNodeSptr WolExpFactory::makeUextExpr(WolNodeSptr expr,
                                        int len) {
   
   WolNodeSptr result;

   if (len == 0) 
      result = expr;
   else {

      assert(len>0);

      WolNodeSptr zeroExpr = makeZeroExpr(len);
      result = makeConcatExpr(zeroExpr, expr);
   }

   return result;
}

WolNodeSptr WolExpFactory::makeSextExpr(WolNodeSptr expr,
                                        int len) {
   
   WolNodeSptr result;
   
   if (len == 0)
      result = expr;
   else {

      assert(len>0);

      WolNodeSptr zeroExpr = makeZeroExpr(len);
      WolNodeSptr onesExpr = makeOnesExpr(len);
      int precision = expr->getPrecision();
      WolNodeSptr negExpr = makeSliceExpr(expr, precision - 1, precision - 1);
      WolNodeSptr iteExpr = makeCondExpr(negExpr, onesExpr, zeroExpr);
      result = makeConcatExpr(iteExpr, expr);
   }

   return result;
}

WolNodeSptr WolExpFactory::makeRepeatExpr(WolNodeSptr expr,
                                          int param) {

   WolNodeSptr oldExpr;
   WolNodeSptr result = expr;
   
   for (int i = 1; i < param; i++){

      oldExpr = result;
      result = makeConcatExpr(oldExpr, expr);
   }
   
   return result;
}

WolNodeSptr WolExpFactory::makeImpliesExpr(WolNodeSptr expr1,
                                           WolNodeSptr expr2) {

   WolNodeSptr notExpr2 = makeNotExpr(expr2);
   WolNodeSptr andExpr = makeAndExpr(expr1, notExpr2);
   WolNodeSptr result = makeNotExpr(andExpr);

   return result;
}

WolNodeSptr WolExpFactory::makeIffExpr(WolNodeSptr expr1,
                                       WolNodeSptr expr2) {

   return makeEqExpr(expr1, expr2);
}

WolNodeSptr WolExpFactory::makeXorExpr(WolNodeSptr expr1,
                                       WolNodeSptr expr2) {

   WolNodeSptr orExpr = makeOrExpr(expr1, expr2);
   WolNodeSptr andExpr = makeAndExpr(expr1, expr2);
   WolNodeSptr negExpr = makeNotExpr(andExpr);
   WolNodeSptr result = makeAndExpr(orExpr, negExpr);

   return result;
}

WolNodeSptr WolExpFactory::makeXnorExpr(WolNodeSptr expr1,
                                        WolNodeSptr expr2) {

   WolNodeSptr xorExpr = makeXorExpr(expr1, expr2);
   WolNodeSptr result = makeNotExpr(xorExpr);
   return result;
}

WolNodeSptr WolExpFactory::makeAndExprInt(WolNodeSptr expr1,
                                          WolNodeSptr expr2) {

   WolNodeSptr andExpr(
       new WolComplexNode(WolNodeType::WOL_AND_NODE, expr1->getPrecision(),
                          expr1, expr2));

   return findExprInt(andExpr, "and");
}

WolNodeSptr WolExpFactory::makeAndExpr(WolNodeSptr expr1,
                                       WolNodeSptr expr2) {

   return rewriteAndExpr(expr1, expr2);
}

WolNodeSptr WolExpFactory::makeNandExpr(WolNodeSptr expr1,
                                        WolNodeSptr expr2) {

   WolNodeSptr andExpr = makeAndExpr(expr1, expr2);
   WolNodeSptr result = makeNotExpr(andExpr);

   return result;
}

WolNodeSptr WolExpFactory::makeOrExpr(WolNodeSptr expr1,
                                      WolNodeSptr expr2) {

   WolNodeSptr negExpr1 = makeNotExpr(expr1);
   WolNodeSptr negExpr2 = makeNotExpr(expr2);
   WolNodeSptr andExpr = makeAndExpr(negExpr1, negExpr2);
   WolNodeSptr result = makeNotExpr(andExpr);

   return result;
}

WolNodeSptr WolExpFactory::makeNorExpr(WolNodeSptr expr1,
                                       WolNodeSptr expr2) {

   WolNodeSptr orExpr = makeOrExpr(expr1, expr2);
   WolNodeSptr result = makeNotExpr(orExpr);

   return result;
}

WolNodeSptr WolExpFactory::makeEqExprInt(WolNodeSptr expr1,
                                         WolNodeSptr expr2) {

   WolNodeSptr eqExpr(new WolComplexNode(WolNodeType::WOL_BEQ_NODE, 1,
                                         expr1, expr2));

   return findExprInt(eqExpr, "==");
}

WolNodeSptr WolExpFactory::makeEqExpr(WolNodeSptr expr1,
                                      WolNodeSptr expr2) {

   return rewriteEqExpr(expr1, expr2);
}

WolNodeSptr WolExpFactory::makeNeExpr(WolNodeSptr expr1,
                                      WolNodeSptr expr2) {

   WolNodeSptr eqExpr = makeEqExpr(expr1, expr2);
   WolNodeSptr result = makeNotExpr(eqExpr);

   return result;
}

WolNodeSptr WolExpFactory::makeAddExprInt(WolNodeSptr expr1,
                                          WolNodeSptr expr2) {

   WolNodeSptr addExpr(
       new WolComplexNode(WolNodeType::WOL_ADD_NODE, expr1->getPrecision(),
                          expr1, expr2));

   return findExprInt(addExpr, "+");
}

WolNodeSptr WolExpFactory::makeAddExpr(WolNodeSptr expr1,
                                       WolNodeSptr expr2) {

   return rewriteAddExpr(expr1, expr2);
}

WolNodeSptr WolExpFactory::makeUaddoExpr(WolNodeSptr expr1,
                                         WolNodeSptr expr2) {

   WolNodeSptr uextExpr1 = makeUextExpr(expr1, 1);
   WolNodeSptr uextExpr2 = makeUextExpr(expr2, 1);
   WolNodeSptr addExpr = makeAddExpr(uextExpr1, uextExpr2);
   WolNodeSptr result = makeSliceExpr(addExpr, expr1->getPrecision(), 
                                      expr1->getPrecision());
  
   return result;
}

WolNodeSptr WolExpFactory::makeSaddoExpr(WolNodeSptr expr1,
                                         WolNodeSptr expr2) {

   int len = expr1->getPrecision();
   WolNodeSptr sign_e1 = makeSliceExpr(expr1, len - 1, len - 1);
   WolNodeSptr sign_e2 = makeSliceExpr(expr2, len - 1, len - 1);
   WolNodeSptr add = makeAddExpr(expr1, expr2);
   WolNodeSptr sign_result = makeSliceExpr(add, len - 1, len - 1);
   WolNodeSptr and1 = makeAndExpr(sign_e1, sign_e2);
   WolNodeSptr neg1 = makeNotExpr(sign_result);
   WolNodeSptr or1 = makeAndExpr(and1, neg1);
   WolNodeSptr neg2 = makeNotExpr(sign_e1);
   WolNodeSptr neg3 = makeNotExpr(sign_e2);
   WolNodeSptr and2 = makeAndExpr(neg2, neg3);
   WolNodeSptr or2 = makeAndExpr(and2, sign_result);
   WolNodeSptr result = makeAndExpr(or1, or2);

   return result;
}

WolNodeSptr WolExpFactory::makeMulExprInt(WolNodeSptr expr1,
                                          WolNodeSptr expr2) {

   WolNodeSptr mulExpr(
       new WolComplexNode(WolNodeType::WOL_MUL_NODE, expr1->getPrecision(),
                          expr1, expr2));

   return findExprInt(mulExpr, "*");
}

WolNodeSptr WolExpFactory::makeMulExpr(WolNodeSptr expr1,
                                       WolNodeSptr expr2) {

   return rewriteMulExpr(expr1, expr2);
}


WolNodeSptr WolExpFactory::makeUltExprInt(WolNodeSptr expr1,
                                          WolNodeSptr expr2) {
   
   WolNodeSptr ultExpr(new WolComplexNode(WolNodeType::WOL_ULT_NODE, 1,
                                          expr1, expr2));

   return findExprInt(ultExpr, "<");
}

WolNodeSptr WolExpFactory::makeUltExpr(WolNodeSptr expr1,
                                       WolNodeSptr expr2) {

   return rewriteUltExpr(expr1, expr2);   
}


WolNodeSptr WolExpFactory::makeSltExpr(WolNodeSptr expr1,
                                       WolNodeSptr expr2) {

   int len = expr1->getPrecision();
   if (len == 1) {
        WolNodeSptr neg1 = makeNotExpr(expr2);
        return makeAndExpr(expr1, neg1);         
   } 
   WolNodeSptr s0 = makeSliceExpr(expr1, len - 1, len - 1);
   WolNodeSptr s1 = makeSliceExpr(expr2, len - 1, len - 1);   
   WolNodeSptr r0 = makeSliceExpr(expr1, len - 2, 0);
   WolNodeSptr r1 = makeSliceExpr(expr2, len - 2, 0);
   WolNodeSptr ult = makeUltExpr(r0, r1);
   WolNodeSptr deterBySign = makeAndExpr(s0, makeNotExpr(s1));
   WolNodeSptr l = deterBySign;
   WolNodeSptr r = makeAndExpr(makeNotExpr(s0), s1);
   WolNodeSptr eqSign = makeAndExpr(makeNotExpr(l), makeNotExpr(r));
   WolNodeSptr eqSignAndUlt = makeAndExpr(eqSign, ult);
   WolNodeSptr result = makeOrExpr(deterBySign, eqSignAndUlt);

   return result;
}

WolNodeSptr WolExpFactory::makeUlteExpr(WolNodeSptr expr1,
                                        WolNodeSptr expr2) {

   WolNodeSptr ultExpr = makeUltExpr(expr2, expr1);
   WolNodeSptr result = makeNotExpr(ultExpr);
     
   return result;
}

WolNodeSptr WolExpFactory::makeSlteExpr(WolNodeSptr expr1,
                                        WolNodeSptr expr2) {

   WolNodeSptr sltExpr = makeSltExpr(expr2, expr1);
   WolNodeSptr result = makeNotExpr(sltExpr);

   return result;
}

WolNodeSptr WolExpFactory::makeUgtExpr(WolNodeSptr expr1,
                                       WolNodeSptr expr2) {

   return makeUltExpr(expr2, expr1);
}

WolNodeSptr WolExpFactory::makeSgtExpr(WolNodeSptr expr1,
                                       WolNodeSptr expr2) {
   
   return makeSltExpr(expr2, expr1);
}

WolNodeSptr WolExpFactory::makeUgteExpr(WolNodeSptr expr1,
                                        WolNodeSptr expr2) {
   
   WolNodeSptr ultExpr = makeUltExpr(expr1, expr2);
   WolNodeSptr result = makeNotExpr(ultExpr);

   return result;

}

WolNodeSptr WolExpFactory::makeSgteExpr(WolNodeSptr expr1,
                                        WolNodeSptr expr2) {

   WolNodeSptr sltExpr = makeSltExpr(expr1, expr2);
   WolNodeSptr result = makeNotExpr(sltExpr);

   return result;
}

WolNodeSptr WolExpFactory::makeSllExprInt(WolNodeSptr expr1,
                                          WolNodeSptr expr2) {

    WolNodeSptr sllExpr(
        new WolComplexNode(WolNodeType::WOL_SLL_NODE, expr1->getPrecision(),
                           expr1, expr2));

   return findExprInt(sllExpr, "sll");
}

WolNodeSptr WolExpFactory::makeSllExpr(WolNodeSptr expr1,
                                       WolNodeSptr expr2) {
   return rewriteSllExpr(expr1, expr2);   
}

WolNodeSptr WolExpFactory::makeSrlExprInt(WolNodeSptr expr1,
                                          WolNodeSptr expr2) {
   
   WolNodeSptr srlExpr(
       new WolComplexNode(WolNodeType::WOL_SRL_NODE, expr1->getPrecision(),
                          expr1, expr2));

   return findExprInt(srlExpr, "srl");
}

WolNodeSptr WolExpFactory::makeSrlExpr(WolNodeSptr expr1,
                                       WolNodeSptr expr2) {

   return rewriteSrlExpr(expr1, expr2);   
}


WolNodeSptr WolExpFactory::makeSraExpr(WolNodeSptr expr1,
                                       WolNodeSptr expr2) {

   int len = expr1->getPrecision();
   WolNodeSptr sign_e1 = makeSliceExpr(expr1, len - 1, len - 1);
   WolNodeSptr srl1 = makeSrlExpr(expr1, expr2);
   WolNodeSptr neg1 = makeNotExpr(expr1);
   WolNodeSptr srl2 = makeSrlExpr(neg1, expr2);
   WolNodeSptr neg2 = makeNotExpr(srl2);
   WolNodeSptr result = makeCondExpr(sign_e1, neg2, srl1);
      
   return result; 
}

WolNodeSptr WolExpFactory::makeRolExpr(WolNodeSptr expr1,
                                       int shift) {

   int len = expr1->getWidth();
   shift %= len;
   WolNodeSptr result;

   if (shift) {

      shift = len - shift;
      assert(1 <= shift && shift < len);
   
      WolNodeSptr l = makeSliceExpr(expr1, shift - 1, 0);
      WolNodeSptr r = makeSliceExpr(expr1, len - 1, shift);

      result = makeConcatExpr(l, r);
   }
   else {

      result = expr1;
   }

   assert(result->getPrecision() == len);

   return result;
}

WolNodeSptr WolExpFactory::makeRorExpr(WolNodeSptr expr1, int shift) {

   int len = expr1->getWidth();
   shift %= len;
   WolNodeSptr result;

   if (shift) {

      assert(1 <= shift && shift < len);

      WolNodeSptr l = makeSliceExpr(expr1, shift - 1, 0);
      WolNodeSptr r = makeSliceExpr(expr1, len - 1, shift);

      result = makeConcatExpr(l, r);
   }
   else {

      result = expr1;
   }

   assert(result->getPrecision() == len);

   return result;
}

WolNodeSptr WolExpFactory::makeSubExpr(WolNodeSptr expr1,
                                       WolNodeSptr expr2) {

   WolNodeSptr negExpr = makeNegExpr(expr2);
   WolNodeSptr result = makeAddExpr(expr1, negExpr);

   return result;
}

WolNodeSptr WolExpFactory::makeUdivExprInt(WolNodeSptr expr1,
                                           WolNodeSptr expr2) {

   WolNodeSptr udivExpr(
       new WolComplexNode(WolNodeType::WOL_UDIV_NODE, expr1->getPrecision(),
                          expr1, expr2));

   return findExprInt(udivExpr, "udiv");
}

WolNodeSptr WolExpFactory::makeUdivExpr(WolNodeSptr expr1,
                                        WolNodeSptr expr2) {

   return rewriteUdivExpr(expr1, expr2);
}

WolNodeSptr WolExpFactory::makeSdivExpr(WolNodeSptr expr1,
                                        WolNodeSptr expr2) {

   int len = expr1->getPrecision();

   if (len == 1)
       return makeNotExpr(makeAndExpr(makeNotExpr(expr1), expr2));
   
   WolNodeSptr signE1 = makeSliceExpr(expr1, len - 1, len - 1);
   WolNodeSptr signE2 = makeSliceExpr(expr2, len - 1, len - 1);
   WolNodeSptr xorExp = makeXorExpr(signE1, signE2);
   WolNodeSptr negE1 = makeNegExpr(expr1);
   WolNodeSptr negE2 = makeNegExpr(expr2);
   WolNodeSptr condE1 = makeCondExpr(signE1, negE1, expr1);
   WolNodeSptr condE2 = makeCondExpr(signE2, negE2, expr2);
   WolNodeSptr udiv = makeUdivExpr(condE1, condE2);
   WolNodeSptr negUdiv = makeNegExpr(udiv);
   WolNodeSptr result = makeCondExpr(xorExp, negUdiv, udiv); 

   return result;
}

WolNodeSptr WolExpFactory::makeUremExprInt(WolNodeSptr expr1,
                                           WolNodeSptr expr2) {

   WolNodeSptr uremExpr(
      new WolComplexNode(WolNodeType::WOL_UREM_NODE, expr1->getPrecision(),
                         expr1, expr2));

   return findExprInt(uremExpr, "urem");
}

WolNodeSptr WolExpFactory::makeUremExpr(WolNodeSptr expr1,
                                        WolNodeSptr expr2) {

   return rewriteUremExpr(expr1, expr2);
}

WolNodeSptr WolExpFactory::makeSremExpr(WolNodeSptr expr1,
                                        WolNodeSptr expr2) {

   int len = expr1->getPrecision();
   
   if (len == 1)
       return makeAndExpr(expr1, makeNotExpr(expr2));
   
   WolNodeSptr signE1 = makeSliceExpr(expr1, len - 1, len - 1);
   WolNodeSptr signE2 = makeSliceExpr(expr2, len - 1, len - 1);
   WolNodeSptr negE1 = makeNegExpr(expr1);
   WolNodeSptr negE2 = makeNegExpr(expr2);
   WolNodeSptr condE1 = makeCondExpr(signE1, negE1, expr1);
   WolNodeSptr condE2 = makeCondExpr(signE2, negE2, expr2);
   WolNodeSptr urem = makeUremExpr(condE1, condE2);
   WolNodeSptr negUrem = makeNegExpr(urem);
   WolNodeSptr result = makeCondExpr(signE1, negUrem, urem);

   return result;
}

WolNodeSptr WolExpFactory::makeSmodExpr(WolNodeSptr expr1,
                                        WolNodeSptr expr2) {
   
   int len = expr1->getPrecision();
   WolNodeSptr zero = makeZeroExpr(len);
   WolNodeSptr e1Zero = makeEqExpr(zero, expr1);
   WolNodeSptr signE1 = makeSliceExpr(expr1, len - 1, len - 1);
   WolNodeSptr signE2 = makeSliceExpr(expr2, len - 1, len - 1);
   WolNodeSptr negE1 = makeNegExpr(expr1);
   WolNodeSptr negE2 = makeNegExpr(expr2);
   WolNodeSptr e1AndE2 = makeAndExpr(makeNotExpr(signE1), makeNotExpr(signE2));
   WolNodeSptr e1AndNegE2 = makeAndExpr(makeNotExpr(signE1), signE2);
   WolNodeSptr negE1AndE2 = makeAndExpr(signE1, makeNotExpr(signE2));
   WolNodeSptr negE1AndNegE2 = makeAndExpr(signE1, signE2);
   WolNodeSptr condE1 = makeCondExpr(signE1,negE1, expr1);
   WolNodeSptr condE2 = makeCondExpr(signE2, negE2, expr2);
   WolNodeSptr urem = makeUremExpr(condE1, condE2);
   WolNodeSptr uRemZero = makeEqExpr(urem, expr2);
   WolNodeSptr negUrem = makeNegExpr(urem);
   WolNodeSptr add1 = makeAddExpr(negUrem, expr2);
   WolNodeSptr add2 = makeAddExpr(urem, expr2);
   WolNodeSptr gadd1 = makeCondExpr(uRemZero, zero, add1);
   WolNodeSptr gadd2 = makeCondExpr(uRemZero, zero, add2);
   WolNodeSptr condCase1 = makeCondExpr(e1AndE2, urem, zero);
   WolNodeSptr condCase2 = makeCondExpr(negE1AndE2, gadd1, zero);
   WolNodeSptr condCase3 = makeCondExpr(e1AndNegE2, gadd2, zero);
   WolNodeSptr condCase4 = makeCondExpr(negE1AndNegE2, negUrem, zero);
   WolNodeSptr or1 = makeOrExpr(condCase1, condCase2);
   WolNodeSptr or2 = makeOrExpr(condCase3, condCase4);
   WolNodeSptr result = makeOrExpr(or1, or2);

   return result;
}

WolNodeSptr WolExpFactory::makeConcatExprInt(WolNodeSptr expr1,
                                             WolNodeSptr expr2) {
   
   WolNodeSptr concatExpr(new WolComplexNode(
                              WolNodeType::WOL_CONCAT_NODE,
                              expr1->getPrecision() + expr2->getPrecision(),
                              expr1, 
                              expr2));

   return findExprInt(concatExpr, "concat");
}

WolNodeSptr WolExpFactory::makeConcatExpr(WolNodeSptr expr1,
                                          WolNodeSptr expr2) {

   return rewriteConcatExpr(expr1, expr2);
}

WolNodeSptr WolExpFactory::makeCondExprInt(WolNodeSptr expr1,
                                           WolNodeSptr expr2,
                                           WolNodeSptr expr3) {

   WolNodeSptr condExpr(
       new WolComplexNode(WolNodeType::WOL_BCOND_NODE, expr2->getPrecision(),
                          expr1, expr2, expr3));

   return findExprInt(condExpr, "cond");
}

WolNodeSptr WolExpFactory::makeCondExpr(WolNodeSptr expr1,
                                        WolNodeSptr expr2,
                                        WolNodeSptr expr3) {

   return rewriteCondExpr(expr1, expr2, expr3);
}

WolNodeSptr WolExpFactory::rewriteBinaryExpr(WolNodeType kind,
                                             WolNodeSptr expr1, 
                                             WolNodeSptr expr2) {

   WolNodeSptr result = nullptr;
   
   if (expr1->wol_is_bv_const_node() && expr2->wol_is_bv_const_node()) {

      WolValueSptr value = nullptr;
      WolEvalFactory *evalFactory = WolMgr::getInstance().getEvalFactory();
   
      switch (kind) {

         case WolNodeType::WOL_AND_NODE:
            value = evalFactory->evalAnd(expr1->getValue(), expr2->getValue());
            break;

         case WolNodeType::WOL_BEQ_NODE:
            value = evalFactory->evalBeq(expr1->getValue(), expr2->getValue());
            break;

         case WolNodeType::WOL_ADD_NODE:
            value = evalFactory->evalAdd(expr1->getValue(), expr2->getValue());
            break;

         case WolNodeType::WOL_MUL_NODE:
            value = evalFactory->evalMul(expr1->getValue(), expr2->getValue());
            break;

         case WolNodeType::WOL_ULT_NODE:
            value = evalFactory->evalUlt(expr1->getValue(), expr2->getValue());
            break;

         case WolNodeType::WOL_SLL_NODE:
            value = evalFactory->evalSll(expr1->getValue(), expr2->getValue());
            break;

         case WolNodeType::WOL_SRL_NODE:
            value = evalFactory->evalSrl(expr1->getValue(), expr2->getValue());
            break;

         case WolNodeType::WOL_UDIV_NODE:
            value = evalFactory->evalUdiv(expr1->getValue(), expr2->getValue());
            break;

         case WolNodeType::WOL_UREM_NODE:
            value = evalFactory->evalUrem(expr1->getValue(), expr2->getValue());
            break;

         default:
            assert(kind == WolNodeType::WOL_CONCAT_NODE);
            value = evalFactory->evalConcat(expr1->getValue(),
                                            expr2->getValue());
            break;
      }

      result = makeConstExpr(value->getStringRep(), 2);      

   }
   else if (expr1->wol_is_bv_const_node() && !expr2->wol_is_bv_const_node()) {
      
      WolValue::WolConstType type = expr1->getValue()->getConstType(); 

      switch (type) {

         case WolValue::WolConstType::WOL_SPECIAL_CONST_ZERO:

            switch (kind) {
            
               case WolNodeType::WOL_BEQ_NODE:

                  if(expr2->getPrecision() == 1) 
                     result = makeNotExpr(expr2);
                  else if (expr2->wol_is_xor_expr()){  
                     // 0 == (a ^ b)   -->  a = b */
                     // 0 == !(!a && !b) && !(a && b)          
                     WolNodeSptr temp1 = 
                         expr2->getChild(1)->getChild(0)->getChild(0);
                     WolNodeSptr temp2 = 
                         expr2->getChild(1)->getChild(0)->getChild(1);
                     
                     result = rewriteEqExpr(temp1, temp2);            
                  }                  
                  else if (expr2->wol_is_not_node() && 
                           expr2->getChild(0)->wol_is_and_node()){
                     
                     // 0 == a | b   --> a == 0 && b == 0
                     WolNodeSptr left = rewriteEqExpr(
                         rewriteNotExpr(expr2->getChild(0)->getChild(0)),
                                        expr1);
                     WolNodeSptr right = rewriteEqExpr(
                         rewriteNotExpr(expr2->getChild(0)->getChild(1)),
                                         expr1);

                     result = rewriteAndExpr(left, right);
                  }
                  break;

               case WolNodeType::WOL_ULT_NODE:   // 0 < a ---> a != 0
                  result = rewriteNotExpr(rewriteEqExpr(expr1, expr2));
                  break;

               case WolNodeType::WOL_ADD_NODE:
                  result = expr2;
                  break;

               case WolNodeType::WOL_MUL_NODE:
               case WolNodeType::WOL_SLL_NODE:
               case WolNodeType::WOL_SRL_NODE:
               case WolNodeType::WOL_UREM_NODE:
               case WolNodeType::WOL_AND_NODE:
                  result = makeZeroExpr(expr2->getPrecision());
                  break;

               case WolNodeType::WOL_UDIV_NODE:
                  {
                     WolNodeSptr temp1 = makeZeroExpr(expr2->getPrecision());
                     WolNodeSptr temp2 = makeOnesExpr(expr2->getPrecision());
                     WolNodeSptr eq = rewriteEqExpr(expr2, temp1);
                     
                     result = rewriteCondExpr(eq, temp2, temp1);
                  }
                  break;

               default:
                  break;

            }
            break;

         case WolValue::WOL_SPECIAL_CONST_ONE:

            if (kind == WolNodeType::WOL_MUL_NODE)
               result = expr2;
            break;

         case WolValue::WolConstType::WOL_SPECIAL_CONST_ONES:

            if (kind == WolNodeType::WOL_BEQ_NODE) {

               if (expr2->wol_is_xnor_expr()) {   
                  // 1+ == (a xnor b)   ---> a == b
                  // 1+ == !(!(!a && !b) && !(a && b))
                  WolNodeSptr temp1 = 
                      expr2->getChild(0)->getChild(1)->getChild(0)->getChild(0);
                  WolNodeSptr temp2 = 
                      expr2->getChild(0)->getChild(1)->getChild(0)->getChild(1);
                  
                  result = rewriteEqExpr(temp1, temp2);            
               }
               else if (expr2->wol_is_and_node()) { 
                  // 1+ == a & b ---> a == 1+ && b == 1+
                  
                  WolNodeSptr left = rewriteEqExpr(expr2->getChild(0), expr1);
                  WolNodeSptr right = rewriteEqExpr(expr2->getChild(1), expr1);
                  
                  result = rewriteAndExpr(left, right);
               }
            }
            else if (kind == WolNodeType::WOL_AND_NODE)
               result = expr2;
            else if (kind == WolNodeType::WOL_ULT_NODE)  // UNSIGNED_MAX < x
               result = makeFalseExpr();
            break;
         
         case WolValue::WolConstType::WOL_SPECIAL_CONST_ONE_ONES:
         
            assert(expr1->getPrecision() == 1);
         
            if(kind == WolNodeType::WOL_AND_NODE || kind == WolNodeType::WOL_BEQ_NODE ||
               kind == WolNodeType::WOL_MUL_NODE)
               result = expr2;
            else if(kind == WolNodeType::WOL_ULT_NODE)
               result = makeFalseExpr();
            break;
   
         case WolValue::WolConstType::WOL_SPECIAL_CONST_NONE:
     
            //TODO handle const = a | b or const = a & b
            // wolvalue for a and b can be inferred from here.
            break;

         default:
            assert(0);
            break;

      }     

   }
   else if (!expr1->wol_is_bv_const_node() && expr2->wol_is_bv_const_node()) {  

      WolValue::WolConstType type = expr2->getValue()->getConstType(); 

      switch (type) {

         case WolValue::WolConstType::WOL_SPECIAL_CONST_ZERO:

            switch (kind) {

               case WolNodeType::WOL_BEQ_NODE:

                  if(expr1->getPrecision() == 1) 
                     result = makeNotExpr(expr1);
                  else if (expr1->wol_is_xor_expr()){  
                     // (a ^ b) == 0   -->  a = b */
                     // 0 == !(!a && !b) && !(a && b)          
                     WolNodeSptr temp1 = 
                         expr1->getChild(1)->getChild(0)->getChild(0);
                     WolNodeSptr temp2 = 
                         expr1->getChild(1)->getChild(0)->getChild(1);
                     
                     result = rewriteEqExpr(temp1, temp2);            
                  }                  
                  else if (expr1->wol_is_not_node() && 
                           expr1->getChild(0)->wol_is_and_node()){
                     
                     // a | b == 0   --> a == 0 && b == 0
                     WolNodeSptr left = rewriteEqExpr(
                         rewriteNotExpr(expr1->getChild(0)->getChild(0)),
                                        expr1);
                     WolNodeSptr right = rewriteEqExpr(
                         rewriteNotExpr(expr1->getChild(0)->getChild(1)),expr1);
                     
                     result = rewriteAndExpr(left, right);
                  }
                  break;

               case WolNodeType::WOL_ULT_NODE:   // a < 0 ---> a != 0
                  result = makeFalseExpr();
                  break;

               case WolNodeType::WOL_ADD_NODE:
               case WolNodeType::WOL_SLL_NODE:
               case WolNodeType::WOL_UREM_NODE:
               case WolNodeType::WOL_SRL_NODE:
                  result = expr1;
                  break;

               case WolNodeType::WOL_MUL_NODE:
               case WolNodeType::WOL_AND_NODE:
                  result = makeZeroExpr(expr1->getPrecision());
                  break;

               case WolNodeType::WOL_UDIV_NODE:
                  result = makeOnesExpr(expr1->getPrecision());
                  break;

               default:
                  break;

            }
            break;
         case WolValue::WOL_SPECIAL_CONST_ONE:

            if (kind == WolNodeType::WOL_MUL_NODE || kind == WolNodeType::WOL_UDIV_NODE)
               result = expr1;
            else if(kind == WolNodeType::WOL_UREM_NODE)
               result = makeZeroExpr(expr1->getPrecision());
            else if(kind == WolNodeType::WOL_ULT_NODE) {

               WolNodeSptr temp = makeZeroExpr(expr1->getPrecision());

               result = rewriteEqExpr(expr1, temp);
            }
            break;
         case WolValue::WolConstType::WOL_SPECIAL_CONST_ONES:

            if (kind == WolNodeType::WOL_BEQ_NODE) {

               if (expr1->wol_is_xnor_expr()) {   
                  //  (a xnor b) == 1+   ---> a == b
                  // 1+ == !(!(!a && !b) && !(a && b))
                  WolNodeSptr temp1 = 
                      expr1->getChild(0)->getChild(1)->getChild(0)->getChild(0);
                  WolNodeSptr temp2 = 
                      expr1->getChild(0)->getChild(1)->getChild(0)->getChild(1);

                  result = rewriteEqExpr(temp1, temp2);            
               }
               else if (expr2->wol_is_and_node()) { 
               
                 // a & b == 1+ ---> a == 1+ && b == 1+
                  WolNodeSptr left = rewriteEqExpr(expr1->getChild(0), expr1);
                  WolNodeSptr right = rewriteEqExpr(expr1->getChild(1), expr1);

                  result = rewriteAndExpr(left, right);
               }
            }
            else if (kind == WolNodeType::WOL_AND_NODE)
               result = expr1;
            else if (kind == WolNodeType::WOL_ULT_NODE) {  // x < UNSIGNED_MAX
               result = rewriteNotExpr(rewriteEqExpr(expr1, expr2));
            }
            break;

         case WolValue::WolConstType::WOL_SPECIAL_CONST_ONE_ONES:

            assert(expr2->getPrecision() == 1);

            if(kind == WolNodeType::WOL_AND_NODE || kind == WolNodeType::WOL_BEQ_NODE ||
               kind == WolNodeType::WOL_MUL_NODE || kind == WolNodeType::WOL_UDIV_NODE)
               result = expr1;
            break;

         case WolValue::WolConstType::WOL_SPECIAL_CONST_NONE:
            //TODO handle a | b == const or a & b == const
            // wolvalue for a and b can be inferred from here.
            break;

         default:
            assert(0);
            break;

      }     

   }
   else if (expr1 == expr2 && (
            kind == WolNodeType::WOL_BEQ_NODE || kind == WolNodeType::WOL_ADD_NODE)) {

      if (kind == WolNodeType::WOL_BEQ_NODE)   // x == x ---> True
         result = makeTrueExpr();
      else if (expr1->getPrecision() >= 2) {   // x+x ---> 2*x

            std::string tempstr(expr1->getPrecision(), '0');
            tempstr[expr1->getPrecision() -2] = '1';
            WolNodeSptr tempexpr = makeConstExpr(tempstr, 2);

            result = rewriteMulExpr(expr1, tempexpr);
      }   
   } 
   else if (((expr1->wol_is_not_node() && expr1->getChild(0) == expr2) ||
            (expr2->wol_is_not_node() && expr2->getChild(0) == expr1)) && 
            (kind == WolNodeType::WOL_BEQ_NODE || kind == WolNodeType::WOL_ADD_NODE)) {
      
       if (kind == WolNodeType::WOL_BEQ_NODE) // x == ~x ---> False
         result = makeFalseExpr();
       else     // x + ~x == -1
         result = makeOnesExpr(expr1->getPrecision());
   }
   else if (expr1 == expr2 && (kind == WolNodeType::WOL_ULT_NODE
            || kind == WolNodeType::WOL_UREM_NODE
            || kind == WolNodeType::WOL_UDIV_NODE)) {
      
      int len = expr1->getPrecision();
      WolNodeSptr temp2, temp3, temp4, eq;  

      switch(kind) {
      
         case WolNodeType::WOL_ULT_NODE:  // u < u ---> false
            result = makeFalseExpr();
            break;
      
         case WolNodeType::WOL_UDIV_NODE:
            // v/v is 1 if v != 0 and UINT_MAX otherwise
            temp2 = makeZeroExpr(len);
            temp3 = makeOneExpr(len);
            temp4 = makeOnesExpr(len);
            eq = rewriteEqExpr(expr1, temp2);
            result = rewriteCondExpr(eq, temp4, temp3);
            break;

         default: // u rem u ---> 0
            result = makeZeroExpr(len);
            break;
      }
   }    
   else if (expr1->wol_is_bcond_node() && expr2->wol_is_bcond_node() && 
            (expr1->getChild(0) == expr2->getChild(0)) && 
            ( kind == WolNodeType::WOL_ULT_NODE
        	|| kind == WolNodeType::WOL_BEQ_NODE
        	|| kind == WolNodeType::WOL_ADD_NODE
        	|| kind == WolNodeType::WOL_UDIV_NODE)) {

      // ITE(c, a1, b1) op ITE (c, a2, b2) ----> ITE (c, a1 op b1, a2 op b2)
      switch (kind) {
      
         case WolNodeType::WOL_ULT_NODE:
            result = rewriteCondExpr(
                     expr1->getChild(0), 
                     rewriteUltExpr(expr1->getChild(1), expr2->getChild(1)),
                     rewriteUltExpr(expr1->getChild(2), expr2->getChild(2)));
            break;

         case WolNodeType::WOL_BEQ_NODE:
            result = rewriteCondExpr(
                     expr1->getChild(0), 
                     rewriteEqExpr(expr1->getChild(1), expr2->getChild(1)),  
                     rewriteEqExpr(expr1->getChild(2), expr2->getChild(2)));
            break;

         case WolNodeType::WOL_ADD_NODE:
            result = rewriteCondExpr(
                     expr1->getChild(0),
                     rewriteAddExpr(expr1->getChild(1), expr2->getChild(1)), 
                     rewriteAddExpr(expr1->getChild(2), expr2->getChild(2)));
            break;

         default:
            result = rewriteCondExpr(
                     expr1->getChild(0),
                     rewriteUdivExpr(expr1->getChild(1), expr2->getChild(1)),
                     rewriteUdivExpr(expr1->getChild(2), expr2->getChild(2)));
            break;

      }   
   }  
   else if (kind == WolNodeType::WOL_AND_NODE && expr1->wol_is_concat_node() &&
            expr2->wol_is_concat_node() && 
            expr1->getChild(0)->getPrecision() == 
                expr2->getChild(0)->getPrecision()) {

         // (0::X) & (Y::0) ---> 0 ; (0::X) & (Y::1)  ---> 0::X ; 
         // (1::X) & (Y::1) ---> Y::X        
         // (X::0) & (0::Y) ---> 0 ; (X::0) & (1::Y)  ---> X::0 ; 
         //(X::1) & (1::Y) ---> X::Y 

         if((expr1->getChild(0)->wol_is_const_zero_or_ones_expr() &&  
              expr2->getChild(1)->wol_is_const_zero_or_ones_expr()) ||
            (expr1->getChild(1)->wol_is_const_zero_or_ones_expr() && 
              expr2->getChild(0)->wol_is_const_zero_or_ones_expr()) ) {

            WolNodeSptr left = rewriteAndExpr(expr1->getChild(0) , 
                                              expr2->getChild(0));
            WolNodeSptr right = rewriteAndExpr(expr1->getChild(1), 
                                               expr2->getChild(1));

            result = rewriteConcatExpr(left, right);
         }
   }

   /* TODO: lots of word level simplifications:
   * a[7:4] == b[7:4] && a[3:0] == b[3:0] <=> a == b
   * {a,b} == {c,d} with |a|=|c| <=> a == c && b == d
   * ...
   */
  /* TODO a + 2 * a <=> 3 * a <=> and see below */
  /* TODO strength reduction: a * 2 == a << 1 (really ?) */
  /* TODO strength reduction: a * 3 == (a << 1) + a (really ?) */
  /* TODO strength reduction: a / 2 == (a >> 1) (yes!) */
  /* TODO strength reduction: a / 3 =>  higher bits zero (check!) */
  /* TODO MAX-1 < a <=> a == MAX */

  /* TODO (x < ~x) <=> !msb(x) */
  /* TODO (~x < x) <=> msb(x) */

  /* TODO to support GAUSS bubble up odd terms:
   * (2 * a + 3 * y) + 4 * x => 3 * y + (2 * a + 4 * x)
   * or alternatively normalize arithmetic terms/polynomials
   * or simply always replace by equation.
   */

  /* TODO simplify (c * x + 2 * y) + x == 5 at GAUSS application
   * by first (c + 1) * x + 2 * y == 5 and then check whether 'c'
   * is even.
   */

  /* TODO Howto handle 2 * x == 4 && 4 * x + 8 * y == 0 ?
   * Maybe: x[30:0] == 2 && 4 * {x[31],2[30:0]} + 8 * y == 0?
   * Then: x[30:0] == 2 && 8[31:0] + 8 *y == 0?
   * Then: x[30:0] = 2 && 8 * y = -8
   * Finally:  x[30:0] = 2 && y[29:0] = -1
   * etc.
   */

   return result;
}



WolNodeSptr WolExpFactory::rewriteSliceExpr(WolNodeSptr expr, int upper, 
                                            int lower) {

   int precision = expr->getPrecision();
   WolNodeSptr result = nullptr;   

   if (precision == upper - lower + 1) {

      result = expr;
   }
   else if (expr->wol_is_not_node()) {
      
      // (!a)[l:m] ---> !(a[l:m])
      result = rewriteNotExpr(rewriteSliceExpr(expr->getChild(0),
                                               upper, lower));
   }
   else if (expr->wol_is_bv_const_node()) { 
      
      // c[l:m] ---> c'
      WolValueSptr value = WolMgr::getInstance().getEvalFactory()->evalSplice(
          expr->getValue(),
          upper,
          lower);
      result = makeConstExpr(value->getStringRep(), 2);       
   }
   else if (expr->wol_is_slice_node()) {

      // a[b:c][l:m] -----> a[b+l:b+m]
      WolComplexNodeSptr exprc = dynamic_pointer_cast<WolComplexNode>(expr);

      result = rewriteSliceExpr(expr->getChild(0), 
                                exprc->getLowPrecision() + upper,
                                exprc->getLowPrecision() + lower);
   }
   else if (expr->wol_is_concat_node()) {

      // a::b[l:m] ----> a if (l = 0 && m = len(a) - 1)
      // a::b[l:m]-----> b if (len(a) + len(b) == m + 1 && l = len(a))
      if (lower == 0 &&
          expr->getChild(1)->getPrecision() == upper - lower + 1) {
      
         result = expr->getChild(1);
      }
      else if ((upper == precision - 1) &&
               (expr->getChild(0)->getPrecision() == (upper - lower + 1))) {
         
         result = expr->getChild(0);
      }
      //TODO: Further rewrites are possible if concats are normalized.
   }
   else if (expr->wol_is_and_node() && 
            (expr->getChild(0)->wol_is_slice_simplifiable_node() ||
             (expr->getChild(0)->wol_is_not_node() && 
               expr->getChild(0)->getChild(0)->wol_is_slice_simplifiable_node()))
         &&  (expr->getChild(1)->wol_is_slice_simplifiable_node() ||
              (expr->getChild(1)->wol_is_not_node() &&
               expr->getChild(1)->getChild(0)->wol_is_slice_simplifiable_node()))) {             
      // (a && b)[l:m] ----> a[l:m] && b[l:m]
      WolNodeSptr left = rewriteSliceExpr(expr->getChild(0), upper, lower);
      WolNodeSptr right = rewriteSliceExpr(expr->getChild(1), upper, lower);
    
      result = makeAndExpr(left, right);     
   }
   else if (expr->wol_is_bcond_node() &&
            (expr->getChild(1)->wol_is_slice_simplifiable_node() ||
             (expr->getChild(1)->wol_is_not_node() && 
               expr->getChild(1)->getChild(0)->wol_is_slice_simplifiable_node()))
               && (expr->getChild(2)->wol_is_slice_simplifiable_node() ||
             (expr->getChild(2)->wol_is_not_node() && 
               expr->getChild(2)->getChild(0)->wol_is_slice_simplifiable_node()))) {            
      // ITE(c, a, b)[l:m] ----> ITE(c, a[l:m], b[l:m])
      WolNodeSptr t = rewriteSliceExpr(expr->getChild(1), upper, lower);
      WolNodeSptr e = rewriteSliceExpr(expr->getChild(2), upper, lower);

      result = makeCondExpr(expr->getChild(0), t, e);
   }

   if(!result)
      result = makeSliceExprInt(expr, upper, lower);

   return result;
}

static bool check_inversion(WolNodeSptr expr1, WolNodeSptr expr2) {

   bool result = (expr1->wol_is_not_node() && (expr1->getChild(0) == expr2)) ||
                 (expr2->wol_is_not_node() && (expr2->getChild(0) == expr1));

   return result;   
}

static bool check_const(WolNodeSptr expr){

   bool result = (expr->wol_is_bv_const_node() || 
                  (expr->wol_is_not_node() && 
                   expr->getChild(0)->wol_is_bv_const_node()));
   return result;
}

static bool find_and_contradiction_expr(WolNodeSptr expr, WolNodeSptr expr1, 
                                        WolNodeSptr expr2, int& calls) {
   
   if (calls >= WOL_FIND_AND_NODE_CONTRADICTION_LIMT)
      return false;

   if (!expr->wol_is_not_node() && expr->wol_is_and_node()) {
      
      if (check_inversion(expr->getChild(0), expr1) || 
          check_inversion(expr->getChild(0), expr2) ||
          check_inversion(expr->getChild(1), expr1) ||
          check_inversion(expr->getChild(1), expr2))
         return false;
      calls++;
      return find_and_contradiction_expr(expr->getChild(0), expr1, expr2, calls) 
          || find_and_contradiction_expr(expr->getChild(1), expr1, expr2, calls);
   }
   return false;
}

WolNodeSptr WolExpFactory::rewriteAndExpr(WolNodeSptr expr0, WolNodeSptr expr1){
   
   WolNodeSptr result = nullptr;
   int precision = expr1->getPrecision();

   if (expr0 == expr1)  // x & x == x
      result = expr1;
   else if (check_inversion(expr0, expr1)) {
   
      // x & ~x ----> 0
      result = makeZeroExpr(precision);  
   }
   else if ((expr0->wol_is_and_node() || 
             (expr0->wol_is_not_node() && expr0->getChild(0)->wol_is_and_node()))
         && (expr1->wol_is_and_node() || 
            (expr1->wol_is_not_node() && expr1->getChild(0)->wol_is_and_node()))) {


      if (!expr0->wol_is_not_node() && !expr1->wol_is_not_node()) {

         WolNodeSptr child00 = expr0->getChild(0);
         WolNodeSptr child01 = expr0->getChild(1);
         WolNodeSptr child10 = expr1->getChild(0);
         WolNodeSptr child11 = expr1->getChild(1);
 
         // second rule of contradiction
         if (check_inversion(child00, child10) ||
             check_inversion(child00, child11) ||
             check_inversion(child01, child10) ||
             check_inversion(child01, child11) )
             result = makeZeroExpr(precision);

         //symmetric rule of idempotency     
         if (child00 == child10 || child01 == child10)
            result = rewriteAndExpr(expr0, child11);
      
         // use commutativity
         if (child00 == child11 || child01 == child11)
            result = rewriteAndExpr(expr0, child10);         
     
      }
      else if (!expr0->wol_is_not_node() && expr1->wol_is_not_node()) {

         WolNodeSptr child00 = expr0->getChild(0);
         WolNodeSptr child01 = expr0->getChild(1);
         WolNodeSptr child10 = expr1->getChild(0)->getChild(0);
         WolNodeSptr child11 = expr1->getChild(0)->getChild(1);
 
         // second rule of subsumption
         if (check_inversion(child00, child10) ||
             check_inversion(child00, child11) ||
             check_inversion(child01, child10) ||
             check_inversion(child01, child11) )
            result = expr0;
         
         //symmetric rule of substitution
         if (child00 == child10 || child01 == child10) {
    
            WolNodeSptr notExpr = makeNotExpr(child11);
            result = rewriteAndExpr(expr0, notExpr);
         }

         // symmetric rule of substitution
         if (child00 == child11 || child01 == child11) {

            WolNodeSptr notExpr = makeNotExpr(child10);
            result = rewriteAndExpr(expr0, notExpr);
         }
  
      }
      else if (expr0->wol_is_not_node() && !expr1->wol_is_not_node()) {

         WolNodeSptr child00 = expr0->getChild(0)->getChild(0);
         WolNodeSptr child01 = expr0->getChild(0)->getChild(1);
         WolNodeSptr child10 = expr1->getChild(0);
         WolNodeSptr child11 = expr1->getChild(1);

         // second rule of subsumption
         if (check_inversion(child00, child10) ||
             check_inversion(child00, child11) ||
             check_inversion(child01, child10) ||
             check_inversion(child01, child11) )
            result = expr1;
      
         // symmetric rule of substitution
         if (child01 == child10 || child01 == child11) {
         
            WolNodeSptr notExpr = makeNotExpr(child00);
            result = rewriteAndExpr(notExpr, expr1);    
         }

         // symmetric rule of substitution
         if (child00 == child10 || child00 == child11) {

            WolNodeSptr notExpr = makeNotExpr(child01);
            result = rewriteAndExpr(notExpr, expr1);
         }
   
      }
      else {
   
         WolNodeSptr child00 = expr0->getChild(0)->getChild(0);
         WolNodeSptr child01 = expr0->getChild(0)->getChild(1);
         WolNodeSptr child10 = expr1->getChild(0)->getChild(0);
         WolNodeSptr child11 = expr1->getChild(0)->getChild(1);
          
         // a XNOR b simplifies to a == b for the boolean case
         if (precision == 1 &&
             child00->wol_is_not_node() != child01->wol_is_not_node() &&
             child10->wol_is_not_node() != child11->wol_is_not_node() &&
             ((check_inversion(child00, child10) && 
                check_inversion(child01, child11)) ||
              (check_inversion(child00, child11) && 
                check_inversion(child01, child10)))) {

            result = rewriteEqExpr(child00->wol_is_not_node() 
                                   ? child00->getChild(0) : child00,
                                   child01->wol_is_not_node() 
                                   ? child01->getChild(0): child01);
         }

         // rule of resolution
         if ((child00 == child10 && check_inversion(child01, child11)) ||
             (child00 == child11 && check_inversion(child01, child10)))
            result = makeNotExpr(child00);

         // rule of resolution
         if ((child11 == child01 && check_inversion(child10, child00)) ||
             (child11 == child00 && check_inversion(child10, child01)))
            result = makeNotExpr(child11);
      }
    
   }
   else if (expr0->wol_is_and_node() || 
            (expr0->wol_is_not_node() && expr0->getChild(0)->wol_is_and_node())) { 
      
      if (expr0->wol_is_not_node()) {
      
         WolNodeSptr child00 = expr0->getChild(0)->getChild(0);
         WolNodeSptr child01 = expr0->getChild(0)->getChild(1);

         // first rule of subsumption
         if (check_inversion(child00, expr1) || check_inversion(child01, expr1))
            result = expr1;

         // asymmetric rule of substitution
         if (child01 == expr1) {
          
            WolNodeSptr notExpr = makeNotExpr(child00);
            result = rewriteAndExpr(notExpr, expr1);
         }

         // asymmetric rule of substitution
         if (child00 == expr1) {

            WolNodeSptr notExpr = makeNotExpr(child01);
            result = rewriteAndExpr(notExpr, expr1);
         }

      }
      else {

         WolNodeSptr child00 = expr0->getChild(0);
         WolNodeSptr child01 = expr0->getChild(1);

         // first rule of contradiction 
         if (check_inversion(child00, expr1) || check_inversion(child01, expr1))
            result = makeZeroExpr(precision);
         
         // asymmetric rule of idempotency
         if (child00 == expr1 || child01 == expr1)
            result = expr0;

         if (expr1->wol_is_bv_const_node()) {

            if (child00->wol_is_bv_const_node()) {

               WolNodeSptr temp = rewriteAndExpr(expr1, child00);
               result = rewriteAndExpr(temp, child01);
            }
            else if (child01->wol_is_bv_const_node()) {
      
               WolNodeSptr temp = rewriteAndExpr(expr1, child01);
               result = rewriteAndExpr(temp, child00);
            }

         }

      }      

   }  
   else if (expr1->wol_is_and_node() ||  
            (expr1->wol_is_not_node() && expr1->getChild(0)->wol_is_and_node())) {

      if (expr1->wol_is_not_node()) {
      
         WolNodeSptr child10 = expr1->getChild(0)->getChild(0);
         WolNodeSptr child11 = expr1->getChild(0)->getChild(1);

         //first rule of subsumption
         if (check_inversion(child10, expr0) || check_inversion(child11, expr0)) 
            return expr0;  
         
         // asymmetric rule of substiution
         if (child10 == expr0) {
      
            WolNodeSptr notExpr = makeNotExpr(child11);
            result = rewriteAndExpr(expr0, notExpr);
         }

         //asymmetric rule of substitution
         if (child11 == expr0) {

            WolNodeSptr notExpr = makeNotExpr(child10);
            result = rewriteAndExpr(expr0, notExpr);
         }

      }
      else {

         WolNodeSptr child10 = expr1->getChild(0);
         WolNodeSptr child11 = expr1->getChild(1);
 
         // first rule of contradiction
         if (check_inversion(child10, expr0) || check_inversion(child11, expr0))
            result = makeZeroExpr(precision);

         // asymmetric rule of idempotency
         if (child10 == expr0 || child11 == expr0)
            result = expr1;

         if(expr0->wol_is_bv_const_node()) {

            if (child10->wol_is_bv_const_node()) {
         
               WolNodeSptr temp = rewriteAndExpr(expr0, child10);
               result = rewriteAndExpr(temp, child11);
            }
            else if (child11->wol_is_bv_const_node()) {

               WolNodeSptr temp = rewriteAndExpr(expr0, child11);
               result = rewriteAndExpr(temp, child10);
            }

         }

      }
    
   }
   else if ((expr0->wol_is_ult_node() || 
             (expr0->wol_is_not_node() && expr0->getChild(0)->wol_is_ult_node())) &&
            (expr1->wol_is_ult_node() || 
             (expr1->wol_is_not_node() && expr1->getChild(0)->wol_is_ult_node()))) {

      // a < b && b < a simplifies to FALSE
      if (!expr0->wol_is_not_node() && !expr1->wol_is_not_node() &&
          expr0->getChild(0) == expr1->getChild(1) && 
          expr0->getChild(1) == expr1->getChild(0))
         result = makeFalseExpr();

      // NOT(a < b) && NOT (b < a) -----> a == b
      if (expr0->wol_is_not_node() && expr1->wol_is_not_node() &&
          expr0->getChild(0)->getChild(0) == expr1->getChild(0)->getChild(1) &&
          expr0->getChild(0)->getChild(1) == expr1->getChild(0)->getChild(0))
         result = rewriteEqExpr(expr0->getChild(0)->getChild(0), 
                                expr0->getChild(0)->getChild(1));   
   }   

   int calls = 0;
   if (find_and_contradiction_expr(expr0, expr0, expr1, calls) ||
       find_and_contradiction_expr(expr1, expr0, expr1, calls))
      result = makeZeroExpr(precision);  

   
   if (expr0->wol_is_not_node() && expr0->getType() == expr1->getType()) {

      if (expr0->wol_is_add_node()) {

        //TODO: implement normalize_adds_expr 
//        std::pair<WolNodeSptr, WolNodeSptr> pr =  normalize_adds_expr(expr0, 
//                                                                      expr1);
//        expr0 = pr.first();
//        expr1 = pr.second();
      }
      else if (expr0->wol_is_mul_node()) {
   
        //TODO: implement normalize_muls_expr 
//        std::pair<WolNodeSptr, WolNodeSptr> pr =  normalize_muls_expr(expr0,
//                                                                      expr1);
//        expr0 = pr.first();
//        expr1 = pr.second();
      }
   }
   
   if (result == nullptr)
      result = rewriteBinaryExpr(WolNodeType::WOL_AND_NODE, expr0, expr1);
   
   if (result == nullptr)
      result = makeAndExprInt(expr0, expr1);


   return result;
}

 
WolNodeSptr 
WolExpFactory::normalize_negated_add(WolNodeSptr expr) {
   
   if (!expr->wol_is_not_node())
      return expr;
   
   if (!expr->getChild(0)->wol_is_add_node())
      return expr;

   WolNodeSptr tmp = makeAddExpr(expr->getChild(0)->getChild(0),
                                 expr->getChild(0)->getChild(1));
   WolNodeSptr one = makeOneExpr(expr->getPrecision());
   WolNodeSptr result = makeAddExpr(tmp, one);

   return result;
}

static 
bool is_always_unequal(WolNodeSptr expr0, WolNodeSptr expr1) {

   return false;
}

WolNodeSptr WolExpFactory::rewriteEqExpr(WolNodeSptr expr0, WolNodeSptr expr1) {

   WolNodeSptr result = nullptr;
   
   expr0 = normalize_negated_add(expr0);
   expr1 = normalize_negated_add(expr1);

   // ~e0 == ~e1 -----> e0 == e1
   if (expr0->wol_is_not_node() && expr1->wol_is_not_node()) {
   
      expr0 = expr0->getChild(0);
      expr1 = expr1->getChild(0);
   }
   
   if (expr0 == expr1) {
    
      return makeTrueExpr();  
   } 

   if (is_always_unequal(expr0, expr1)) {
      
      return makeFalseExpr(); 
   }

   if (!expr0->wol_is_not_node()) {
      
      // a + b = a ----> b = 0.
      // This rule does not lead to less subtitutions. 
      // 'a' cannot be substituted as
      // the occurrence check would fail.

      if (expr0->wol_is_add_node()) {

         if (expr0->getChild(0) == expr1) {
            WolNodeSptr zero = makeZeroExpr(expr0->getPrecision());
            return rewriteEqExpr(zero, expr0->getChild(1));  //RECURSIVE      
         }

         if (expr0->getChild(1) == expr1) {
            WolNodeSptr zero = makeZeroExpr(expr0->getPrecision());
            return rewriteEqExpr(zero, expr0->getChild(0));  //RECURSIVE    
         }
      }
 
      // b ? a :t = d ----> a != d => !b AND d = t      
      if(expr0->wol_is_bcond_node()) {
      
         if (is_always_unequal(expr0->getChild(1), expr1)) {
            WolNodeSptr temp = rewriteEqExpr(expr0->getChild(2), expr1);
            return rewriteAndExpr(rewriteNotExpr(expr0->getChild(0)), temp); // RECURSIVE
         }

         if (is_always_unequal(expr0->getChild(2), expr1)) { 
            WolNodeSptr temp = rewriteEqExpr(expr0->getChild(1), expr1);  
            return rewriteAndExpr(expr0->getChild(0), temp); // RECURSIVE      
         }
      }      
   }

   if (!expr1->wol_is_not_node()) {

      // a = a + b ----> b = 0.
      // This rule does not lead to less subtitutions. 
      //'a' cannot be substituted as
      // the occurrence check would fail.

      if (expr1->wol_is_add_node()) {

         if (expr1->getChild(0) == expr0) {
            WolNodeSptr zero = makeZeroExpr(expr1->getPrecision());
            return rewriteEqExpr(zero, expr1->getChild(1));  //RECURSIVE      
         }

         if (expr1->getChild(1) == expr0) {
            WolNodeSptr zero = makeZeroExpr(expr1->getPrecision());
            return rewriteEqExpr(zero, expr1->getChild(0));  //RECURSIVE    
         }
      }

      // d = b ? a :t ----> a != d => !b AND d = t      
      if(expr1->wol_is_bcond_node()) {

         if (is_always_unequal(expr1->getChild(1), expr0)) {
            WolNodeSptr temp = rewriteEqExpr(expr1->getChild(2), expr0);
            return rewriteAndExpr(rewriteNotExpr(expr1->getChild(0)), temp); // RECURSIVE
         }

         if (is_always_unequal(expr1->getChild(2), expr0)) {     
            WolNodeSptr temp = rewriteEqExpr(expr1->getChild(1), expr0);
            return rewriteAndExpr(expr1->getChild(0), temp); // RECURSIVE      
         }
      }
   }

   if (!expr0->wol_is_not_node() && !expr1->wol_is_not_node()) {

      // a + b = a + c ----> b = c      
      if (expr0->wol_is_add_node() && expr1->wol_is_add_node()) {
         
         if (expr0->getChild(0) == expr1->getChild(0)) 
            return rewriteEqExpr(expr0->getChild(1), expr1->getChild(1));  // RECURSIVE
            
         if (expr0->getChild(0) == expr1->getChild(1)) 
            return rewriteEqExpr(expr0->getChild(1), expr1->getChild(0));  // RECURSIVE

         if (expr0->getChild(1) == expr1->getChild(0)) 
            return rewriteEqExpr(expr0->getChild(0), expr1->getChild(1));  // RECURSIVE

         if (expr0->getChild(1) == expr1->getChild(1)) 
            return rewriteEqExpr(expr0->getChild(0), expr1->getChild(0));  // RECURSIVE
      }
      
      // commutative operators are normalized ignoring signs. so we do not have //REMEMBER
      // to check cases like a & b == ~b & a as they are represented as a & b == a& ~b
      if (expr0->wol_is_and_node() && expr1->wol_is_and_node()) {
         
         WolNodeSptr child00 = expr0->getChild(0);
         WolNodeSptr child01 = expr0->getChild(1);
         WolNodeSptr child10 = expr1->getChild(1);
         WolNodeSptr child11 = expr1->getChild(1);

         if(check_inversion(child00, child10) &&
            check_inversion(child01, child11)) {

            // a & b == ~a & ~b   ----> a == ~b
            if (child00->wol_is_not_node() == child01->wol_is_not_node()) 
               return rewriteEqExpr(child00, rewriteNotExpr(child01)); //RECURSIVE

            //~a & b == a & ~b ---> a == b
            if (child00->wol_is_not_node() != child01->wol_is_not_node()) 
               return rewriteEqExpr(child10 , child01); // RECURSIVE
         }

         // a & b == a & ~b ----> a == 0
         if (child00 == child10 && check_inversion(child01, child11)) {
            WolNodeSptr temp = makeZeroExpr(expr0->getPrecision());
            return rewriteEqExpr(child00, temp);  // RECURSIVE
         }

         // a & b == ~a & b ---> b == 0
         if (child01 == child11 && check_inversion(child00, child10)) {
            WolNodeSptr temp = makeZeroExpr(expr0->getPrecision());
            return rewriteEqExpr(child01, temp); // RECURSIVE
         }
      }   
   }

   // a == b ? a : c is rewritten to b OR a == c
   // a == ~(b ? a : c) is rewritten to !b AND a == ~c
   if (expr0->wol_is_bcond_node() || 
      (expr0->wol_is_not_node() && expr0->getChild(0)->wol_is_bcond_node())) {

      WolNodeSptr real0 = expr0->wol_is_not_node() ? expr0->getChild(0) : expr0;
      
      if (real0->getChild(1) == expr1) {
         
         if (expr0->wol_is_not_node()) {
            WolNodeSptr temp = rewriteEqExpr(rewriteNotExpr(real0->getChild(2)), 
                                             expr1);
            result = rewriteAndExpr(rewriteNotExpr(real0->getChild(0)), temp);
         }
         else {
            WolNodeSptr temp = rewriteEqExpr(real0->getChild(2), expr1);
            result = makeOrExpr(real0->getChild(0), temp);
         }
         return result;
      }

      if (real0->getChild(2) == expr1) {
         
         if (expr0->wol_is_not_node()) {
            WolNodeSptr temp = rewriteEqExpr(rewriteNotExpr(real0->getChild(1)), 
                                             expr1);
            result = rewriteAndExpr(rewriteNotExpr(real0->getChild(0)), temp);
         }
         else {
            WolNodeSptr temp = rewriteEqExpr(real0->getChild(1), expr1);
            result = makeOrExpr(real0->getChild(0), temp);
         }
         return result;
      }
   }

   if (expr1->wol_is_bcond_node() || 
      (expr1->wol_is_not_node() && expr1->getChild(0)->wol_is_bcond_node())) {

      WolNodeSptr real1 = expr1->wol_is_not_node() ? expr1->getChild(0) : expr1;
      
      if (real1->getChild(1) == expr0) {
         
         if (expr1->wol_is_not_node()) {
            WolNodeSptr temp = rewriteEqExpr(rewriteNotExpr(real1->getChild(2)), 
                                             expr0);
            result = rewriteAndExpr(rewriteNotExpr(real1->getChild(0)), temp);
         }
         else {
            WolNodeSptr temp = rewriteEqExpr(real1->getChild(2), expr0);
            result = makeOrExpr(real1->getChild(0), temp);
         }
         return result;
      }

      if (real1->getChild(2) == expr0) {

         if (expr1->wol_is_not_node()) {
            WolNodeSptr temp = rewriteEqExpr(rewriteNotExpr(real1->getChild(1)), 
                                             expr0);
            result = rewriteAndExpr(rewriteNotExpr(real1->getChild(0)), temp);
         }
         else {
            WolNodeSptr temp = rewriteEqExpr(real1->getChild(0), expr0);
            result = makeOrExpr(real1->getChild(0), temp);
         }
         return result;
      }
   }

/* TODO: Normalization
   // normalize adds and muls on demand
   if (!expr0->wol_is_not_node() && !expr1->wol_is_not_node() && 
       (((expr0->wol_is_add_node() || expr0->wol_is_mul_node()) &&
          expr0->getType() == expr1->getType()) ||
          (expr0->wol_is_add_node() && expr1->wol_is_mul_node()) ||
          (expr1->wol_is_add_node() && expr0->wol_is_mul_node()))) {

      if ((expr0->wol_is_add_node() || expr0->wol_is_mul_node()) && 
          expr0->getType() == exp1->getType()) {

         WolNodeStpr temp1 = nullptr;
         WolNodeSptr temp2 = nullptr;

         if (expr0->wol_is_add_node()) 
            normalize_adds_exp(expr0, expr1, temp1, temp2);
         else 
            normalize_muls_exp(expr0, expr1, temp1, temp2);
         
         expr0 = temp1;
         expr1 = temp2;   
      }
      
   }   

*/
    
   result = rewriteBinaryExpr(WolNodeType::WOL_BEQ_NODE, expr0, expr1);

   if (!result)
      result = makeEqExprInt(expr0, expr1);

   return result;
}

WolNodeSptr WolExpFactory::rewriteAddExpr(WolNodeSptr expr0, WolNodeSptr expr1){
   
   int precision = expr0->getPrecision();
   WolNodeSptr result = nullptr;   

   if (precision == 1) 
      return makeXorExpr(expr0, expr1);

   // a - a -----> 0
   if (expr1->wol_is_and_node() &&
       check_inversion(expr0, expr1->getChild(0)) && 
       expr1->getChild(1)->wol_is_const_one_expr())
      return makeZeroExpr(precision);

   // const1 + (const2 + a) ----> (const1 + const2) + a
   if (check_const(expr0) &&
        expr1->wol_is_and_node()) {
   
      
      if (check_const(expr1->getChild(0))) {
         
         WolNodeSptr temp = rewriteAddExpr(expr0, expr1->getChild(0));
         return rewriteAddExpr(temp, expr1->getChild(1));
      }

      if (check_const(expr1->getChild(1))) {
         
         WolNodeSptr temp = rewriteAddExpr(expr0, expr1->getChild(1));
         return rewriteAddExpr(temp, expr1->getChild(0));
      }
   }   
   // (a + cons1) + const2 ----> a + (const1 + const2)
   else if (check_const(expr1) &&
           expr0->wol_is_and_node()) {


      if (check_const(expr0->getChild(0))) {
         
         WolNodeSptr temp = rewriteAddExpr(expr1, expr0->getChild(0));
         return rewriteAddExpr(temp, expr0->getChild(1));
      }

      if (check_const(expr0->getChild(1))) {

         WolNodeSptr temp = rewriteAddExpr(expr1, expr0->getChild(1));
         return rewriteAddExpr(temp, expr0->getChild(0));
      }
   } 
 
   // a + !(b + c) ---> 1 + (!b + !c) TODO: Check the validity of this rewrite   
         

   //TODO: Normalizing multiplication

   result = rewriteBinaryExpr(WolNodeType::WOL_ADD_NODE, expr0, expr1);

   if (!result)
      result = makeAddExprInt(expr0, expr1);

   return result;
}

WolNodeSptr WolExpFactory::rewriteMulExpr(WolNodeSptr expr0, WolNodeSptr expr1) {

   WolNodeSptr result = nullptr;
   int precision = expr0->getPrecision();
   
   if (precision == 1) 
      return rewriteAndExpr(expr0, expr1);
   
   // const1 * (const2 * a) ----> (const1 * const2) * a
   if (check_const(expr0) && expr1->wol_is_mul_node()) {

      if (check_const(expr1->getChild(0))) {

         WolNodeSptr temp = rewriteMulExpr(expr0, expr1->getChild(0));
         return rewriteMulExpr(temp, expr1->getChild(1));
      }

      if (check_const(expr1->getChild(1))) {

         WolNodeSptr temp = rewriteMulExpr(expr0, expr1->getChild(1));
         return rewriteMulExpr(temp, expr1->getChild(0));
      }
   }
   // (a * cons1) * const2 ----> a * (const1 * const2)
   else if (check_const(expr1)) {


      if (check_const(expr0->getChild(0))) {

         WolNodeSptr temp = rewriteMulExpr(expr1, expr0->getChild(0));
         return rewriteMulExpr(temp, expr0->getChild(1));
      }

      if (check_const(expr0->getChild(1))) {

         WolNodeSptr temp = rewriteMulExpr(expr1, expr0->getChild(1));
         return rewriteMulExpr(temp, expr0->getChild(0));
      }
   }

   //const * (t + const) =recursively= const * t + const * const 
   if (check_const(expr0) && expr1->wol_is_and_node() &&
       (check_const(expr1->getChild(0)) || check_const(expr1->getChild(1)))) {
      
      WolNodeSptr left = rewriteMulExpr(expr0, expr1->getChild(0));
      WolNodeSptr right = rewriteMulExpr(expr0, expr1->getChild(1));
      WolNodeSptr result = rewriteAddExpr(left, right);
      return result;
   }

  if (check_const(expr1) && expr0->wol_is_and_node() &&
       (check_const(expr0->getChild(0)) || check_const(expr0->getChild(1)))) {
      
      WolNodeSptr left = rewriteMulExpr(expr1, expr0->getChild(0));
      WolNodeSptr right = rewriteMulExpr(expr1, expr0->getChild(1));
      WolNodeSptr result = rewriteAddExpr(left, right);
      return result;
   }

   if (expr0->wol_is_and_node() && expr1->wol_is_and_node()) {
      
      //TODO: Normalise adds on demand
   }
 

   result = rewriteBinaryExpr(WolNodeType::WOL_MUL_NODE, expr0, expr1);
   if (!result)
      result = makeMulExprInt(expr0, expr1);

   return result;
}

WolNodeSptr WolExpFactory::rewriteUltExpr(WolNodeSptr expr0, WolNodeSptr expr1){

   WolNodeSptr result = nullptr;
   int precision = expr0->getPrecision();

   // ~a < ~b ----> b < a
   if (expr0->wol_is_not_node() && expr1->wol_is_not_node())   
      return rewriteUltExpr(expr1->getChild(0), expr0->getChild(0));
   
   if (precision == 1)
      return rewriteAndExpr(rewriteNotExpr(expr0), expr1);

   if (!expr0->wol_is_not_node() && (expr0->getType() == expr1->getType())) {

      switch (expr0->getType()) {
   
         case WolNodeType::WOL_CONCAT_NODE:

            if (expr0->getChild(0) == expr1->getChild(0)) 
               return rewriteUltExpr(expr0->getChild(1), expr1->getChild(1));
            else if (expr0->getChild(1) == expr1->getChild(1))
               return rewriteUltExpr(expr0->getChild(0), expr1->getChild(0));
            break;

         case WolNodeType::WOL_ADD_NODE:
            //TODO: Normalize adds expr

            break;
         case WolNodeType::WOL_MUL_NODE:
            //TODO: Normalize muls expr
            break;
         default:
            break;
      } 

   }
        
   result = rewriteBinaryExpr(WolNodeType::WOL_ULT_NODE, expr0, expr1);
   
   if (!result)
      result = makeUltExprInt(expr0, expr1);   

   return result;
}

WolNodeSptr WolExpFactory::rewriteSllExpr(WolNodeSptr expr0, WolNodeSptr expr1) {

   WolNodeSptr result = rewriteBinaryExpr(WolNodeType::WOL_SLL_NODE, expr0, expr1);
   if (!result)
      result = makeSllExprInt(expr0, expr1);

   return result;
}

WolNodeSptr WolExpFactory::rewriteSrlExpr(WolNodeSptr expr0, WolNodeSptr expr1){

   WolNodeSptr result = rewriteBinaryExpr(WolNodeType::WOL_SRL_NODE, expr0, expr1);
   if (!result)
      result = makeSrlExprInt(expr0, expr1);

   return result;
}

WolNodeSptr WolExpFactory::rewriteUdivExpr(WolNodeSptr expr0, WolNodeSptr expr1){

   WolNodeSptr result = nullptr;
   int precision = expr0->getPrecision();

   if (precision == 1) 
      return rewriteAndExpr(rewriteNotExpr(expr0), expr1);

   if (!expr0->wol_is_not_node() && (expr0->getType() == expr1->getType())) {
   
      if (expr0->wol_is_add_node()) {
         //TODO normailize adds exp
      }
      else if (expr0->wol_is_mul_node()) {
         //TODO normalize muls exp
      }
   }
      
   result = rewriteBinaryExpr(WolNodeType::WOL_UDIV_NODE, expr0, expr1);
   if (!result)
      result = makeUdivExprInt(expr0, expr1);

   return result;
}

WolNodeSptr WolExpFactory::rewriteUremExpr(WolNodeSptr expr0, WolNodeSptr expr1) {

   WolNodeSptr result = nullptr;
   int precision = expr0->getPrecision();

   if (precision == 1)
      return rewriteAndExpr(expr0, rewriteNotExpr(expr1));

   if (!expr0->wol_is_not_node() && (expr0->getType() == expr1->getType())) {

      if (expr0->wol_is_add_node()) {
         //TODO normailize adds exp
      }
      else if (expr0->wol_is_mul_node()) {
         //TODO normalize muls exp
      }
   }
    
   result = rewriteBinaryExpr(WolNodeType::WOL_UREM_NODE, expr0, expr1);
   if (!result)
      result = makeUremExprInt(expr0, expr1);

   return result;
}

WolNodeSptr WolExpFactory::rewriteConcatExpr(WolNodeSptr expr0, WolNodeSptr expr1) {
   
   WolNodeSptr result = nullptr;
   
   //TODO: rewrite code
   result = rewriteBinaryExpr(WolNodeType::WOL_CONCAT_NODE, expr0, expr1);
   if (!result)
      result = makeConcatExprInt(expr0, expr1);

   return result;
}

WolNodeSptr WolExpFactory::rewriteCondExpr(WolNodeSptr expr_cond, 
                                           WolNodeSptr expr_if, 
                                           WolNodeSptr expr_else) {

   WolNodeSptr result = nullptr;
   assert(expr_cond->getPrecision() == 1);   
   
   if (expr_cond->wol_is_not_node()) {
      expr_cond = expr_cond->getChild(0);
      WolNodeSptr tmp = expr_if;
      expr_if = expr_else;
      expr_else = tmp;
   }

   if (expr_if == expr_else) {
      result = expr_if;
      return result;
   }

   if (expr_cond->wol_is_bv_const_node()) {
       if (expr_cond->getValue()->getConstType() == 
            WolValue::WolConstType::WOL_SPECIAL_CONST_ZERO)
           result = expr_else;
       else 
           result = expr_if;   
       return result;  
   }

   if (!result)
      result = makeCondExprInt(expr_cond, expr_if, expr_else);

   return result;
}

WolNodeSptr WolExpFactory::rewriteNotExpr(WolNodeSptr expr) {

   //TODO: handle !(expr); expr could be anything.

   WolNodeSptr result = nullptr;
   if (expr->wol_is_not_node())
       return expr->getChild(0);    
    
   if (expr->wol_is_bv_const_node()){
        return makeConstExpr(expr->getValue()->getNotValue()->getStringRep(), 2);
   }

   result = makeNotExprInt(expr);     
   return result;
}
}
