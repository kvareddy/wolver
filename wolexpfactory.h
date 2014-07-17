#ifndef WOLEXPFACTORY_H_INCLUDED
#define WOLEXPFACTORY_H_INCLUDED

#include <memory>
namespace wolver {

using namespace std;

class WolNode;
class WolMgr;
class WolEvalFactory;


class WolExpFactory {

public: //singleton
   static WolExpFactory& getInstance()
   {
      static WolExpFactory instance;
      return instance;
   }

private :
	WolExpFactory() {};
   WolExpFactory (WolExpFactory const&);
   void operator=(WolExpFactory const&);


public :
	WolNodeSptr makeConstExpr(std::string name, int denom);	// Binary Constant
	WolNodeSptr makeZeroExpr(int len);		// Binary Constant representing 'len' zeros
	WolNodeSptr makeFalseExpr();		// Constant representing false
	WolNodeSptr makeOnesExpr(int len);		// Binary Constant representing 'len' Ones
	WolNodeSptr makeTrueExpr();			// Constant representing true
	WolNodeSptr makeOneExpr(int len);		// Binary constant representing 1 with 'len' bits
	WolNodeSptr makeVarExpr(int len, std::string symbol);
	WolNodeSptr makeNotExpr(WolNodeSptr expr);
	WolNodeSptr makeNegExpr(WolNodeSptr expr);
	WolNodeSptr makeRedorExpr(WolNodeSptr expr);
	WolNodeSptr makeRedxorExpr(WolNodeSptr expr);
	WolNodeSptr makeRedandExpr(WolNodeSptr expr);
	WolNodeSptr makeSliceExpr(WolNodeSptr expr, int upper, int lower);
	WolNodeSptr makeUextExpr(WolNodeSptr expr, int len);
	WolNodeSptr makeSextExpr(WolNodeSptr expr, int len);
   WolNodeSptr makeRepeatExpr(WolNodeSptr expr, int param);
	WolNodeSptr makeImpliesExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeIffExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeXorExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeXnorExpr(WolNodeSptr expr1, WolNodeSptr expr2);
   WolNodeSptr makeAndExpr(WolNodeSptr expr1, WolNodeSptr expr2); 	
	WolNodeSptr makeNandExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeOrExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeNorExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeEqExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeNeExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeAddExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeUaddoExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeSaddoExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeMulExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeUltExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeSltExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeUlteExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeSlteExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeUgtExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeSgtExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeUgteExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeSgteExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeSllExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeSrlExpr(WolNodeSptr expr1, WolNodeSptr expr2);
 	WolNodeSptr makeSraExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeRolExpr(WolNodeSptr expr1, int param);
	WolNodeSptr makeRorExpr(WolNodeSptr expr1, int param);
	WolNodeSptr makeSubExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeUdivExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeSdivExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeUremExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeSremExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeSmodExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeConcatExpr(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeCondExpr(WolNodeSptr expr1, WolNodeSptr expr2, WolNodeSptr expr3);

private :   // functions
	WolNodeSptr makeSliceExprInt(WolNodeSptr expr, int upper, int lower);
	WolNodeSptr makeAndExprInt(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeEqExprInt(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeAddExprInt(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeMulExprInt(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeUltExprInt(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeSllExprInt(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeSrlExprInt(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeUdivExprInt(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeUremExprInt(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeConcatExprInt(WolNodeSptr expr1, WolNodeSptr expr2);
	WolNodeSptr makeCondExprInt(WolNodeSptr expr1, WolNodeSptr expr2, WolNodeSptr expr3);
   WolNodeSptr makeNotExprInt(WolNodeSptr expr1);

   WolNodeSptr rewriteNotExpr(WolNodeSptr expr);
   WolNodeSptr rewriteSliceExpr(WolNodeSptr expr, int upper, int lower);
   WolNodeSptr rewriteAndExpr(WolNodeSptr expr1, WolNodeSptr expr2);
   WolNodeSptr rewriteEqExpr(WolNodeSptr expr1, WolNodeSptr expr2);
   WolNodeSptr rewriteAddExpr(WolNodeSptr expr1, WolNodeSptr expr2);
   WolNodeSptr rewriteMulExpr(WolNodeSptr expr1, WolNodeSptr expr2);
   WolNodeSptr rewriteUltExpr(WolNodeSptr expr1, WolNodeSptr expr2);
   WolNodeSptr rewriteSllExpr(WolNodeSptr expr1, WolNodeSptr expr2);
   WolNodeSptr rewriteSrlExpr(WolNodeSptr expr1, WolNodeSptr expr2);
   WolNodeSptr rewriteUdivExpr(WolNodeSptr expr1, WolNodeSptr expr2);
   WolNodeSptr rewriteUremExpr(WolNodeSptr expr1, WolNodeSptr expr2);
   WolNodeSptr rewriteConcatExpr(WolNodeSptr expr1, WolNodeSptr expr2);
   WolNodeSptr rewriteCondExpr(WolNodeSptr expr1, WolNodeSptr expr2, WolNodeSptr expr3);

   WolNodeSptr rewriteBinaryExpr(WolNode::WolNodeType type, WolNodeSptr expr1, WolNodeSptr expr);
  WolNodeSptr normalize_negated_add(WolNodeSptr expr);

private :

};


}

#endif
