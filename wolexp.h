/*  WOL : word level solver
 *
 */

#ifndef WOLEXP_H_INCLUDED
#define WOLEXP_H_INCLUDED

namespace wolver
{

class WolNode {

    
public:  //Types
	enum WolNodeType {
		WOL_INVALID_NODE = 0,

		WOL_BV_CONST_NODE = 1,
		WOL_BV_VAR_NODE = 2,
		WOL_ARRAY_VAR_NODE = 3,
		WOL_PARAM_NODE = 4,      /* parameter for lambda expressions */
		WOL_SLICE_NODE = 5,
		WOL_AND_NODE = 6,
		WOL_BEQ_NODE = 7,             /* equality on bit vectors */
		WOL_AEQ_NODE = 8,             /* equality on arrays */
		WOL_ADD_NODE = 9,
		WOL_MUL_NODE = 10,
		WOL_ULT_NODE = 11,
		WOL_SLL_NODE = 12,
		WOL_SRL_NODE = 13,
		WOL_UDIV_NODE = 14,
		WOL_UREM_NODE = 15,
		WOL_CONCAT_NODE = 16,
		WOL_READ_NODE = 17,
		WOL_APPLY_NODE = 18,
		WOL_LAMBDA_NODE = 19,         /* lambda expression */
		WOL_WRITE_NODE = 20,
		WOL_BCOND_NODE = 21,          /* conditional on bit vectors */
		WOL_ACOND_NODE = 22,          /* conditional on arrays */
		WOL_ARGS_NODE = 23,
		WOL_PROXY_NODE = 24,          /* simplified expression without children */
		WOL_NUM_OPS_NODE = 25
	};
public :
   WolNode();
   ~WolNode();

public: //Methods
	bool wol_is_bv_const_node() {return (_type == WOL_BV_CONST_NODE);}
	bool wol_is_bv_var_node() {return (_type == WOL_BV_VAR_NODE);} 
	bool wol_is_array_var_node() {return (_type == WOL_ARRAY_VAR_NODE);}
        bool wol_is_param_node () {return (_type = WOL_PARAM_NODE);}
        bool wol_is_slice_node () {return (_type = WOL_SLICE_NODE);}
     	bool wol_is_and_node () {return (_type = WOL_AND_NODE);}
	bool wol_is_beq_node () {return (_type = WOL_BEQ_NODE);}
	bool wol_is_aeq_node () {return (_type = WOL_AEQ_NODE);}
	bool wol_is_add_node () {return (_type = WOL_ADD_NODE);}
	bool wol_is_mul_node () {return (_type = WOL_MUL_NODE);}
	bool wol_is_ult_node () {return (_type = WOL_ULT_NODE);}
	bool wol_is_sll_node () {return (_type = WOL_SLL_NODE);}
	bool wol_is_srl_node () {return (_type = WOL_SRL_NODE);}
	bool wol_is_udiv_node () {return (_type = WOL_UDIV_NODE);}
	bool wol_is_urem_node () {return (_type = WOL_UREM_NODE);}
	bool wol_is_concat_node () {return (_type = WOL_CONCAT_NODE);}
	bool wol_is_read_node () {return (_type = WOL_READ_NODE);}
	bool wol_is_apply_node () {return (_type = WOL_APPLY_NODE);}
	bool wol_is_lambda_node () {return (_type = WOL_LAMBDA_NODE);}
	bool wol_is_write_node () {return (_type = WOL_WRITE_NODE);}
	bool wol_is_bcond_node () {return (_type = WOL_BCOND_NODE);}
	bool wol_is_acond_node () {return (_type = WOL_ACOND_NODE);}
	bool wol_is_args_node () {return (_type = WOL_ARGS_NODE);}
	bool wol_is_proxy_node () {return (_type = WOL_PROXY_NODE);}

private: //Methods


private: // data
	WolNodeType _type;  	// type of expression
        int _length;  		// number of bits
        int _parents;	 	// number of parents
	int _arity;		// arity of operator
};

}


#endif
