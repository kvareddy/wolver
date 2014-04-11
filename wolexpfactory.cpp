#include "wolmgr.h"
#include "wolexp.h"
#include "wolexpfactory.h"
#include <sstream>
#include <bitset>
#include <assert.h>
#include <string.h>

namespace wolver {

using namespace std;

WolExpFactory::WolExpFactory(WolMgr *mgr)
               : _mgr(mgr)
{
}

static const char* hex_char_to_bin(char c)
{
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

WolNode *WolExpFactory::makeConstExpr(std::string name, int denom) {

   // convert name in to binary strig   
   if (denom == 16) {
      std::string bin;
      for(unsigned i = 0; i != name.length(); ++i)
         bin += hex_char_to_bin(name[i]);
      name = bin;
   }
   else 
      assert (denom == 2);

   WolNode *constExpr = NULL;
   // check if the constEpxr is already present
   if ((constExpr = _mgr->findConstExpr(name)) != NULL) {
      constExpr->incRefCount();
   }
   else {
      // build the constexpr
      constExpr = new WolNode(WolNode::WOL_BV_CONST_NODE, name.length());
      constExpr->setName(name);
      _mgr->insertConstExpr(constExpr);
      _mgr->insertIdExpr(constExpr);
   }

   return constExpr;
}

WolNode *WolExpFactory::makeZeroExpr(int len) {
   assert(len > 0);
   string name(len, '0');
   return makeConstExpr(name, 2);   
}

WolNode *WolExpFactory::makeOneExpr(int len) {
   assert(len > 0);
   if (len == 1) 
      return makeConstExpr("1", 2);
   else {
      string name(len-1, '0');
      name.push_back('1');
      return makeConstExpr(name, 2);
   }
}

WolNode *WolExpFactory::makeFalseExpr() {
   return makeZeroExpr(1);
}

WolNode *WolExpFactory::makeOnesExpr(int len) {
   assert(len > 0);
   string name(len, '1');
   return makeConstExpr(name, 2);
}

WolNode *WolExpFactory::makeTrueExpr() {
   return makeOnesExpr(1);
}

WolNode *WolExpFactory::makeVarExpr(int len, std::string symbol) {
   WolNode *varExpr = new WolNode(WolNode::WOL_BV_VAR_NODE, len);
   varExpr->setName(symbol);
   return varExpr;
}

static WolNode *findExprInt(WolMgr *mgr, WolNode *exp, std::string name) {

   WolNode *retExp = NULL;
   if ((retExp = mgr->findUniqueExpr(exp)) != NULL) {
      retExp->incRefCount();
      exp->release();
   }
   else {
      retExp = exp;
      retExp->setName(name);
      mgr->insertUniqueExpr(exp);
      mgr->insertIdExpr(retExp);
   }

   return retExp; 
}

static WolNode *findSliceExprInt(WolMgr *mgr, WolNode *exp, std::string name) {

   WolNode *retExp = NULL;
   if ((retExp = mgr->findSliceExpr(exp)) != NULL) {
      retExp->incRefCount();
      exp->release();
   }
   else {
      retExp = exp;
      retExp->setName(name);
      mgr->insertUniqueSliceExpr(exp);
      mgr->insertIdExpr(retExp);
   }

   return retExp;
}

WolNode *WolExpFactory::makeNotExpr(WolNode *expr) {
   
   WolNode *notExpr = new WolComplexNode(WolNode::WOL_BV_NOT_NODE, expr->getPrecision(), expr);

   return findExprInt(_mgr, notExpr, "not");
}

WolNode *WolExpFactory::makeNegExpr(WolNode *expr) {

   WolNode *oneExpr = makeOneExpr(expr->getPrecision());
   WolNode *negExpr = makeNotExpr(expr);
   WolNode *addExpr = makeAddExpr(negExpr, oneExpr);
   negExpr->release();
   oneExpr->release();

   return addExpr;
}
 
WolNode *WolExpFactory::makeRedorExpr(WolNode *expr) {
// TODO   
   
   return expr;
}

WolNode *WolExpFactory::makeRedxorExpr(WolNode *expr){
// TODO

   return expr;
}

WolNode *WolExpFactory::makeRedandExpr(WolNode *expr) {
// TODO

   return expr;
}

WolNode *WolExpFactory::makeSliceExpr(WolNode *expr, int upper, int lower) {
   
   assert(upper >= lower);
   assert(expr);
   WolNode *sliceExpr = new WolComplexNode(WolNode::WOL_SLICE_NODE, upper-lower+1,
                                           upper, lower, expr);
   ostringstream os;
   os << "[" << upper << "," << lower << "]";
   string name = os.str(); 
   return findSliceExprInt(_mgr, sliceExpr, name);
}

WolNode *WolExpFactory::makeUextExpr(WolNode *expr, int len) {
   
   WolNode *result;
   if (len == 0) {
      expr->incRefCount();
      result = expr;
   }
   else {
      assert(len>0);
      WolNode *zeroExpr = makeZeroExpr(len);
      result = makeConcatExpr(zeroExpr, expr);
      zeroExpr->release();
   }
   return result;
}

WolNode *WolExpFactory::makeSextExpr(WolNode *expr, int len) {
   WolNode *result;
   if (len == 0){
      expr->incRefCount();
      result = expr;
   }
   else {
      assert(len>0);
      WolNode *zeroExpr = makeZeroExpr(len);
      WolNode *onesExpr = makeOnesExpr(len);
      int precision = expr->getPrecision();
      WolNode *negExpr = makeSliceExpr(expr, precision - 1, precision - 1);
      WolNode *iteExpr = makeCondExpr(negExpr, onesExpr, zeroExpr);
      result = makeConcatExpr(iteExpr, expr);
      negExpr->release();
      onesExpr->release();
      zeroExpr->release();
      iteExpr->release();
   }
   return result;
}

WolNode *WolExpFactory::makeRepeatExpr(WolNode *expr, int param) {

   WolNode *oldExpr;
   WolNode *result = expr;
   result->incRefCount();
   
   for (int i = 1; i < param; i++)
   {
      oldExpr = result;
      result = makeConcatExpr(oldExpr, expr);
      oldExpr->release();
   }
   expr->release();
   
   return result;
}

WolNode *WolExpFactory::makeImpliesExpr(WolNode *expr1, WolNode *expr2) {
   WolNode *notExpr2 = makeNotExpr(expr2);
   WolNode *andExpr = makeAndExpr(expr1, notExpr2);
   WolNode *result = makeNotExpr(andExpr);
   notExpr2->release();
   andExpr->release();
   return result;
}

WolNode *WolExpFactory::makeIffExpr(WolNode *expr1, WolNode *expr2) {
   return makeEqExpr(expr1, expr2);
}

WolNode *WolExpFactory::makeXorExpr(WolNode *expr1, WolNode *expr2) {
   WolNode *orExpr = makeOrExpr(expr1, expr2);
   WolNode *andExpr = makeAndExpr(expr1, expr2);
   WolNode *negExpr = makeNegExpr(andExpr);
   WolNode *result = makeAndExpr(orExpr, negExpr);

   orExpr->release();
   andExpr->release();
   negExpr->release();
   return result;
}

WolNode *WolExpFactory::makeXnorExpr(WolNode *expr1, WolNode *expr2) {
   WolNode *xorExpr = makeXorExpr(expr1, expr2);
   WolNode *result = makeNotExpr(xorExpr);
   xorExpr->release();
   return result;
}

WolNode *WolExpFactory::makeAndExpr(WolNode *expr1, WolNode *expr2) {
   WolNode *andExpr = new WolComplexNode(WolNode::WOL_AND_NODE, expr1->getPrecision(), 
                                         expr1, expr2);

   return findExprInt(_mgr, andExpr, "and");

}

WolNode *WolExpFactory::makeNandExpr(WolNode *expr1, WolNode *expr2) {
   WolNode *andExpr = makeAndExpr(expr1, expr2);
   WolNode *result = makeNotExpr(andExpr);
   andExpr->release();

   return result;
}

WolNode *WolExpFactory::makeOrExpr(WolNode *expr1, WolNode *expr2) {
   WolNode *negExpr1 = makeNotExpr(expr1);
   WolNode *negExpr2 = makeNotExpr(expr2);
   WolNode *andExpr = makeAndExpr(negExpr1, negExpr2);
   WolNode *result = makeNotExpr(andExpr);
   negExpr1->release();
   negExpr2->release();
   andExpr->release();

   return result;
}

WolNode *WolExpFactory::makeNorExpr(WolNode *expr1, WolNode *expr2) {
   WolNode *orExpr = makeOrExpr(expr1, expr2);
   WolNode *result = makeNotExpr(orExpr);
   orExpr->release();

   return result;
}

WolNode *WolExpFactory::makeEqExpr(WolNode *expr1, WolNode *expr2) {
   WolNode *eqExpr = new WolComplexNode(WolNode::WOL_BEQ_NODE, 1, 
                                         expr1, expr2);

   return findExprInt(_mgr, eqExpr, "==");
}

WolNode *WolExpFactory::makeNeExpr(WolNode *expr1, WolNode *expr2) {
   WolNode *eqExpr = makeEqExpr(expr1, expr2);
   WolNode *result = makeNotExpr(eqExpr);
   eqExpr->release();
   return result;
}

WolNode *WolExpFactory::makeAddExpr(WolNode *expr1, WolNode *expr2) {
   WolNode *addExpr = new WolComplexNode(WolNode::WOL_ADD_NODE, expr1->getPrecision(), 
                                         expr1, expr2);

   return findExprInt(_mgr, addExpr, "+");
}

WolNode *WolExpFactory::makeUaddoExpr(WolNode *expr1, WolNode *expr2) {
   WolNode *uextExpr1 = makeUextExpr(expr1, 1);
   WolNode *uextExpr2 = makeUextExpr(expr2, 1);
   WolNode *addExpr = makeAddExpr(uextExpr1, uextExpr2);
   WolNode *result = makeSliceExpr(addExpr, expr1->getPrecision(), expr1->getPrecision());
   uextExpr1->release();
   uextExpr2->release();
   addExpr->release();
  
   return result;
}

WolNode *WolExpFactory::makeSaddoExpr(WolNode *expr1, WolNode *expr2) {
   int len = expr1->getPrecision();
   WolNode *sign_e1 = makeSliceExpr(expr1, len - 1, len - 1);
   WolNode *sign_e2 = makeSliceExpr(expr2, len - 1, len - 1);
   WolNode *add = makeAddExpr(expr1, expr2);
   WolNode *sign_result = makeSliceExpr(add, len - 1, len - 1);
   WolNode *and1 = makeAndExpr(sign_e1, sign_e2);
   WolNode *neg1 = makeNotExpr(sign_result);
   WolNode *or1 = makeAndExpr(and1, neg1);
   WolNode *neg2 = makeNotExpr(sign_e1);
   WolNode *neg3 = makeNotExpr(sign_e2);
   WolNode *and2 = makeAndExpr(neg2, neg3);
   WolNode *or2 = makeAndExpr(and2, sign_result);
   WolNode *result = makeAndExpr(or1, or2);

   sign_e1->release();
   sign_e2->release();
   add->release();
   sign_result->release();
   and1->release();
   neg1->release();
   or1->release();
   neg2->release();
   neg3->release();
   and2->release();
   or2->release();
   
   return result;
}

WolNode *WolExpFactory::makeMulExpr(WolNode *expr1, WolNode *expr2) {
   WolNode *mulExpr = new WolComplexNode(WolNode::WOL_MUL_NODE, expr1->getPrecision(), 
                                         expr1, expr2);

   return findExprInt(_mgr, mulExpr, "*");
}

WolNode *WolExpFactory::makeUltExpr(WolNode *expr1, WolNode *expr2) {
   
   WolNode *ultExpr = new WolComplexNode(WolNode::WOL_ULT_NODE, 1,
                                         expr1, expr2);

   return findExprInt(_mgr, ultExpr, "<");

}

WolNode *WolExpFactory::makeSltExpr(WolNode *expr1, WolNode *expr2) {
   //TODO Complex modeling
   WolNode *sltExpr = new WolComplexNode(WolNode::WOL_SLT_NODE, 1,
                                         expr1, expr2);

   return findExprInt(_mgr, sltExpr, "s<");

}

WolNode *WolExpFactory::makeUlteExpr(WolNode *expr1, WolNode *expr2) {

   WolNode *ultExpr = makeUltExpr(expr2, expr1);
   WolNode *result = makeNotExpr(ultExpr);
     
   ultExpr->release();
   return result;

}

WolNode *WolExpFactory::makeSlteExpr(WolNode *expr1, WolNode *expr2) { 

   WolNode *sltExpr = makeUltExpr(expr2, expr1);
   WolNode *result = makeNotExpr(sltExpr);

   sltExpr->release();
   return result;

}

WolNode *WolExpFactory::makeUgtExpr(WolNode *expr1, WolNode *expr2) {

   return makeUltExpr(expr2, expr1);
}

WolNode *WolExpFactory::makeSgtExpr(WolNode *expr1, WolNode *expr2) {
   
   return makeSltExpr(expr2, expr1);
}

WolNode *WolExpFactory::makeUgteExpr(WolNode *expr1, WolNode *expr2) {
   
   WolNode *ultExpr = makeUltExpr(expr1, expr2);
   WolNode *result = makeNotExpr(ultExpr);

   ultExpr->release();
   return result;

}

WolNode *WolExpFactory::makeSgteExpr(WolNode *expr1, WolNode *expr2) {

   WolNode *sltExpr = makeUltExpr(expr1, expr2);
   WolNode *result = makeNotExpr(sltExpr);

   sltExpr->release();
   return result;
}

WolNode *WolExpFactory::makeSllExpr(WolNode *expr1, WolNode *expr2) {

    //TODO : modeling
    WolNode *sllExpr = new WolComplexNode(WolNode::WOL_SLL_NODE, expr1->getPrecision(),
                                         expr1, expr2);

   return findExprInt(_mgr, sllExpr, "sll");

}

WolNode *WolExpFactory::makeSrlExpr(WolNode *expr1, WolNode *expr2) {
   
   //TODO :modeling
   WolNode *srlExpr = new WolComplexNode(WolNode::WOL_SRL_NODE, expr1->getPrecision(),
                                         expr1, expr2);

   return findExprInt(_mgr, srlExpr, "srl");

}

WolNode *WolExpFactory::makeSraExpr(WolNode *expr1, WolNode *expr2) {

   //TODO: modeling
   int len = expr1->getPrecision();
   WolNode *sign_e1 = makeSliceExpr(expr1, len - 1, len - 1);
   WolNode *srl1 = makeSrlExpr(expr1, expr2);
   WolNode *neg1 = makeNotExpr(expr1);
   WolNode *srl2 = makeSrlExpr(neg1, expr2);
   WolNode *neg2 = makeNotExpr(srl2);
   WolNode *result = makeCondExpr(sign_e1, neg2, srl1);
   sign_e1->release();
   srl1->release();
   neg1->release();
   srl2->release();
   neg2->release();
      
   return result; 
}

WolNode *WolExpFactory::makeRolExpr(WolNode *expr1, int shift) {
   int len = expr1->getWidth();
   shift %= len;
   WolNode *result;

   if (shift) {
      shift = len - shift;
      assert(1 <= shift && shift < len);
   
      WolNode *l = makeSliceExpr(expr1, shift - 1, 0);
      WolNode *r = makeSliceExpr(expr1, len - 1, shift);

      result = makeConcatExpr(l, r);
      l->release();
      r->release();
   }
   else {
      result = expr1;
      result->incRefCount();
   }
   assert(result->getPrecision() == len);
   return result;

}

WolNode *WolExpFactory::makeRorExpr(WolNode *expr1, int shift) {

   int len = expr1->getWidth();
   shift %= len;
   WolNode *result;

   if (shift) {
      assert(1 <= shift && shift < len);

      WolNode *l = makeSliceExpr(expr1, shift - 1, 0);
      WolNode *r = makeSliceExpr(expr1, len - 1, shift);

      result = makeConcatExpr(l, r);
      l->release();
      r->release();
   }
   else {
      result = expr1;
      result->incRefCount();
   }
   assert(result->getPrecision() == len);
   return result;
}

WolNode *WolExpFactory::makeSubExpr(WolNode *expr1, WolNode *expr2) {
   WolNode *negExpr = makeNotExpr(expr2);
   WolNode *result = makeAddExpr(expr1, negExpr);
   negExpr->release();

   return result;
}

WolNode *WolExpFactory::makeUdivExpr(WolNode *expr1, WolNode *expr2) {
   WolNode *udivExpr = new WolComplexNode(WolNode::WOL_UDIV_NODE, expr1->getPrecision(),
                                           expr1, expr2);

   return findExprInt(_mgr, udivExpr, "udiv");
}

WolNode *WolExpFactory::makeSdivExpr(WolNode *expr1, WolNode *expr2) {
 //TODO : modeling
   WolNode *sdivExpr = new WolComplexNode(WolNode::WOL_SDIV_NODE, expr1->getPrecision(),
                                           expr1, expr2);

   return findExprInt(_mgr, sdivExpr, "sdiv");
}

WolNode *WolExpFactory::makeUremExpr(WolNode *expr1, WolNode *expr2) {

   WolNode *uremExpr = new WolComplexNode(WolNode::WOL_UREM_NODE, expr1->getPrecision(),
                                           expr1, expr2);

   return findExprInt(_mgr, uremExpr, "urem");

}

WolNode *WolExpFactory::makeSremExpr(WolNode *expr1, WolNode *expr2) {
   // TODO modeling
   WolNode *sremExpr = new WolComplexNode(WolNode::WOL_SREM_NODE, expr1->getPrecision(),
                                           expr1, expr2);

   return findExprInt(_mgr, sremExpr, "srem");
}

WolNode *WolExpFactory::makeSmodExpr(WolNode *expr1, WolNode *expr2) {
   //TODO modeling
   WolNode *smodExpr = new WolComplexNode(WolNode::WOL_SMOD_MODE, expr1->getPrecision(),
                                           expr1, expr2);

   return findExprInt(_mgr, smodExpr, "smod");
}

WolNode *WolExpFactory::makeConcatExpr(WolNode *expr1, WolNode *expr2) {
   WolNode *concatExpr = new WolComplexNode(WolNode::WOL_CONCAT_NODE, expr1->getPrecision() + expr2->getPrecision(),
                                           expr1, expr2);

   return findExprInt(_mgr, concatExpr, "concat");
}

WolNode *WolExpFactory::makeCondExpr(WolNode *expr1, WolNode *expr2, WolNode *expr3) {
   WolNode *condExpr = new WolComplexNode(WolNode::WOL_BCOND_NODE, expr2->getPrecision(),
                                           expr1, expr2, expr3);

   return findExprInt(_mgr, condExpr, "cond");
}


}
