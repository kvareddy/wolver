#ifndef WOLEXPFACTORY_H_INCLUDED
#define WOLEXPFACTORY_H_INCLUDED

#include "wolmgr.h"
#include "wolexp.h"

namespace wolver {

class WolExpFactory {

public :
	WolExpFactory(WolMgr *mgr);
	~WolExpFactory();

	WolNode *makeConstExpr(const char *bits)	// Binary Constant
	WolNode *makeZeroExpr(int len);		// Binary Constant representing 'len' zeros
	WolNode *makeFalseExpr();		// Constant representing false
	WolNode *makeOnesExpr(int len);		// Binary Constant representing 'len' Ones
	WolNode *makeTrueExpr();			// Constant representing true
	WolNode *makeOneExpr(int len);		// Binary constant representing 1 with 'len' bits
	WolNode *makeUnsignedExpr(unsigned u, int len);
	WolNode *makeIntExpr(int i , int len);
	WolNode *makeVarExpr(int len, const char *symbol);
	WolNode *makeParamExpr(int len, const char *symbol);
	WolNode *makeNotExpr(WolNode *expr);
	WolNode *makeNegExpr(WolNode *expr);
	WolNode *makeRedorExpr(WolNode *expr);
	WolNode *makeRedxorExpr(WolNode *expr);
	WolNode *makeRedandExpr(WolNode *expr);
	WolNode *makeSliceExpr(WolNode *expr, int upper, int lower);
	WolNode *makeUextExpr(WolNode *expr, int len);
	WolNode *makeSextExpr(WolNode *expr, int len);
	WolNode *makeImpliesExpr(WolNode *expr1, WoleNode *expr2);
	WolNode *makeIffExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeXorExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeXnorExpr(WolNode *expr1, WolNode *expr2);
        WolNode *makeAndExpr(WolNode *expr1, WolNode *expr2); 	
	WolNode *makeNandExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeOrExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeNorExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeEqExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeNeExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeAddExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeUaddoExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeSaddoExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeMulExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeUmuloExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeSmuloExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeUltExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeSltExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeUlteExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeSlteExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeUgtExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeSgtExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeUgteExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeSgteExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeSllExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeSrlExpr(WolNode *expr1, WolNode *expr2);
 	WolNode *makeSraExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeRolExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeRorExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeSubExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeUsuboExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeSsuboExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeUdivExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeSdivExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeSdivoExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeUremExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeSremExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeSmodExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeConcatExpr(WolNode *expr1, WolNode *expr2);
	WolNode *makeCondExpr(WolNode *expr1, WolNode *expr2, WolNode *expr3);
	WolNode *makeIncExpr(WolNode *expr);
	WolNode *makeDecExpr(WolNode *expr);

private :   // functions
	WolNode *makeSliceExprInt(WolNode *expr, int upper, int lower);
	WolNode *makeAndExprInt(WolNode *expr1, WolNode *expr2);
	WolNode *makeEqExprInt(WolNode *expr1, WolNode *expr2);
	WolNode *makeAddExprInt(WolNode *expr1, WolNode *expr2);
	WolNode *makeMulExprInt(WolNode *expr1, WolNode *expr2);
	WolNode *makeUltExprInt(WolNode *expr1, WolNode *expr2);
	WolNode *makeSllExprInt(WolNode *expr1, WolNode *expr2);
	WolNode *makeSrlExprInt(WolNode *expr1, WolNode *expr2);
	WolNode *makeUdivExprInt(WolNode *expr1, WolNode *expr2);
	WolNode *makeUremExprInt(WolNode *expr1, WolNode *expr2);
	WolNode *makeConcatExprInt(WolNode *expr1, WolNode *expr2);
	WolNode *makeCondExprInt(WolNode *expr1, WolNode *expr2, WolNode *expr3);

        WolNode *rewriteSliceExpr(WolNode *expr, int upper, int lower);
        WolNode *rewriteAndExpr(WolNode *expr1, WolNode *expr2);
        WolNode *rewriteEqExpr(WolNode *expr1, WolNode *expr2);
        WolNode *rewriteAddExpr(WolNode *expr1, WolNode *expr2);
        WolNode *rewriteMulExpr(WolNode *expr1, WolNode *expr2);
        WolNode *rewriteUltExpr(WolNode *expr1, WolNode *expr2);
        WolNode *rewriteSllExpr(WolNode *expr1, WolNode *expr2);
        WolNode *rewriteSrlExpr(WolNode *expr1, WolNode *expr2);
        WolNode *rewriteUdivExpr(WolNode *expr1, WolNode *expr2);
        WolNode *rewriteUremExpr(WolNode *expr1, WolNode *expr2);
        WolNode *rewriteConcatExpr(WolNode *expr1, WolNode *expr2);
        WolNode *rewriteCondExpr(WolNode *expr1, WolNode *expr2, WolNode *expr3);

private :
	WolMgr *wolmgr;


};


}

#endif
