/*  WOL : word level solver
 *
 */

#ifndef WOLEXP_H_INCLUDED
#define WOLEXP_H_INCLUDED

#include <vector>
#include <array>
#include <iostream>

namespace wolver
{

using namespace std;

class WolNode {

public:  //Types
	enum WolNodeType {
		WOL_INVALID_NODE = 0,

		WOL_BV_CONST_NODE = 1,
		WOL_BV_VAR_NODE = 2,
		WOL_SLICE_NODE = 3,
		WOL_AND_NODE = 4,
		WOL_BEQ_NODE = 5,             /* equality on bit vectors */
		WOL_ADD_NODE = 6,
		WOL_MUL_NODE = 7,
		WOL_ULT_NODE = 8,
      WOL_SLT_NODE = 9,
		WOL_SLL_NODE = 10,
		WOL_SRL_NODE = 11,
		WOL_UDIV_NODE = 12,
      WOL_SDIV_NODE = 13,
	   WOL_UREM_NODE = 14,
      WOL_SREM_NODE = 15,
      WOL_SMOD_MODE = 16,
		WOL_CONCAT_NODE = 17,
		WOL_BCOND_NODE = 18,          /* conditional on bit vectors */
		WOL_PROXY_NODE = 19,          /* simplified expression without children */
      WOL_BV_NOT_NODE = 20,
		WOL_NUM_OPS_NODE = 21
	};
public :
   WolNode(WolNodeType type , int precision);
   virtual ~WolNode() {}

public: //Methods
	bool wol_is_bv_const_node() {return (_type == WOL_BV_CONST_NODE);}
	bool wol_is_bv_var_node() {return (_type == WOL_BV_VAR_NODE);} 
   bool wol_is_leaf_node() { return ((_type == WOL_BV_CONST_NODE) ||
                                     (_type == WOL_BV_VAR_NODE));}
   bool wol_is_slice_node () {return (_type = WOL_SLICE_NODE);}
   bool wol_is_and_node () {return (_type = WOL_AND_NODE);}
	bool wol_is_beq_node () {return (_type = WOL_BEQ_NODE);}
	bool wol_is_add_node () {return (_type = WOL_ADD_NODE);}
	bool wol_is_mul_node () {return (_type = WOL_MUL_NODE);}
	bool wol_is_ult_node () {return (_type = WOL_ULT_NODE);}
	bool wol_is_sll_node () {return (_type = WOL_SLL_NODE);}
	bool wol_is_srl_node () {return (_type = WOL_SRL_NODE);}
	bool wol_is_udiv_node () {return (_type = WOL_UDIV_NODE);}
	bool wol_is_urem_node () {return (_type = WOL_UREM_NODE);}
	bool wol_is_concat_node () {return (_type = WOL_CONCAT_NODE);}
	bool wol_is_bcond_node () {return (_type = WOL_BCOND_NODE);}
	bool wol_is_proxy_node () {return (_type = WOL_PROXY_NODE);}
   void addParent(WolNode *node);
   void incRefCount() {_refCount++;}
   void setName(std::string name) {_name = name;}
   std::string getName() {return _name;}
   bool isArray() {return false;}
   int getWidth() const {return _precision;}
   WolNodeType getType() const {return _type;}
   int getArity() const {return _arity;}
   void setId (int id) {_id = id;}
   int getId() const {return _id;}  
   int getPrecision() const {return _precision;}

public: // virtual methods

   virtual void release()
   {
      _refCount--;
      if (_refCount <= 0)
         delete this;
   }

   virtual void print()
   {
      cout << _name;
   }
 
private: //Methods


protected: // data
	WolNodeType _type;  	// type of expression
   int _precision;  		// number of bits
	int _arity;		// arity of operator
   int _refCount;  //reference count
      int _id;  
   std::string _name;
   std::vector<WolNode *> _parents;
};

class WolComplexNode : public WolNode {

public :
   WolComplexNode(WolNodeType type, int precision, WolNode *child1);
   WolComplexNode(WolNodeType type, int precision, WolNode *child1, WolNode *child2);
   WolComplexNode(WolNodeType type, int precision, WolNode *child1, WolNode *child2, WolNode *child3);
   WolComplexNode(WolNodeType type, int precision, int high_prec, int low_prec, WolNode *child1);  
   ~WolComplexNode() {}


public: //methods

std::array<WolNode *, 3> getChildren() const {return _children;}
int getHighPrecision() const {return _highPrec;}
int getLowPrecision() const {return _lowPrec;}

void setChildren(WolNode *child, int index);
void setHighPrecision(int high_prec) {_highPrec = high_prec;}
void setLowPrecision(int low_prec) {_lowPrec = low_prec;}    
void release();
void print();

private: //methods

private: //data
  int _highPrec;
  int _lowPrec;
  std::array<WolNode *, 3> _children;
};

}


#endif
