/*  WOL : word level solver
 *
 */

#ifndef WOLEXP_H_INCLUDED
#define WOLEXP_H_INCLUDED

#include <vector>
#include <array>
#include <iostream>
#include <memory>
#include "common.h"

namespace wolver
{

using namespace std;

class WolNode;
class WolComplexNode;
class WolValue;




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
		WOL_SLL_NODE = 10,
		WOL_SRL_NODE = 11,
		WOL_UDIV_NODE = 12,
	        WOL_UREM_NODE = 14,
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
   bool wol_is_slice_node () {return (_type == WOL_SLICE_NODE);}
   bool wol_is_and_node () {return (_type == WOL_AND_NODE);}
   bool wol_is_beq_node () {return (_type == WOL_BEQ_NODE);}
   bool wol_is_add_node () {return (_type == WOL_ADD_NODE);}
   bool wol_is_mul_node () {return (_type == WOL_MUL_NODE);}
   bool wol_is_ult_node () {return (_type == WOL_ULT_NODE);}
   bool wol_is_sll_node () {return (_type == WOL_SLL_NODE);}
   bool wol_is_srl_node () {return (_type == WOL_SRL_NODE);}
   bool wol_is_udiv_node () {return (_type == WOL_UDIV_NODE);}
   bool wol_is_urem_node () {return (_type == WOL_UREM_NODE);}
   bool wol_is_concat_node () {return (_type == WOL_CONCAT_NODE);}
   bool wol_is_bcond_node () {return (_type == WOL_BCOND_NODE);}
   bool wol_is_not_node() {return (_type == WOL_BV_NOT_NODE);}
   bool wol_is_proxy_node () {return (_type == WOL_PROXY_NODE);}
   bool wol_is_slice_simplifiable_node () {return (_type == WOL_BV_CONST_NODE) ||
       (_type == WOL_BV_CONST_NODE) ||
                                                  (_type == WOL_SLICE_NODE);}
   void addParent(WolNodeSptr node);
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
   std::vector<WolNodeWptr> getParents();
   WolValueSptr getValue() {return _value;}
   void setValue(WolValueSptr value) {_value = value;}
   WolValueSptr getSValue() {return _svalue;}
   void setSValue(WolValueSptr svalue) {_svalue = svalue;}
   void setImplyFlag() {_implyFlag = true;}
   void unsetImplyFlag() {_implyFlag = false;}
   bool getImplyFlag() {return _implyFlag;}
   WolValueSptr getRandomValue();


public: // virtual methods

   virtual void print()
   {
      cout << _name;
   }
   virtual bool wol_is_xor_expr() {return false;}
   virtual bool wol_is_xnor_expr() {return false;} 
   virtual bool wol_is_const_zero_or_ones_expr();
   virtual bool wol_is_const_one_expr();
   virtual WolNodeSptr getChild(int i) {return nullptr;}
   virtual bool hasChildren() {return false;}

   // result = -1; conflict
   // result = 0 ; nothing happened
   // result = 1 ; enable backward implication
   // result = 2 ; enable both forward and backward implication
   virtual int performImplication();
   virtual WolValueSptr performBackwardImplication();
   virtual WolValueSptr performBackwardImplication(WolNodeSptr parent,
                                                   WolValueSptr parentValue = {},
                                                   WolValueSptr operand1 = {},
                                                   WolValueSptr operand2 = {});
   virtual std::vector<WolNodeSptr> getNeighbors();

protected: //Methods
  void deleteInvalidParents();


protected: // data
  WolNodeType _type;  	// type of expression
  int _precision;  		// number of bits
  int _arity;		// arity of operator
  int _refCount;  //reference count
  int _id;
  bool _implyFlag;
  std::string _name;
  std::vector<WolNodeWptr> _parents;
  WolValueSptr _value;
  WolValueSptr _svalue; //for storing purpose
};

class WolComplexNode : public WolNode  {

public :
   WolComplexNode(WolNodeType type, 
                  int precision, 
                  WolNodeSptr child1);
   WolComplexNode(WolNodeType type,
                  int precision,
                  WolNodeSptr child1, 
                  WolNodeSptr child2);
   WolComplexNode(WolNodeType type,
                  int precision, 
                  WolNodeSptr child1, 
                  WolNodeSptr child2,
                  WolNodeSptr child3);
   WolComplexNode(WolNodeType type,
                  int precision,
                  int high_prec,
                  int low_prec, 
                  WolNodeSptr child1);  
   ~WolComplexNode();

public: //methods

WolNodeSptr getChild(int i) {return _children[i];}
std::array<WolNodeSptr, 3> getChildren() const {return _children;}
int getHighPrecision() const {return _highPrec;}
int getLowPrecision() const {return _lowPrec;}
bool hasChildren() {return true;}
WolValueSptr performForwardImplication();
WolValueSptr performForwardImplication(WolValueSptr operand1,
                                      WolValueSptr operand2 = {},
                                      WolValueSptr operand3 = {});
virtual int performImplication();
void setChildren(WolNodeSptr child, int index);
void setHighPrecision(int high_prec) {_highPrec = high_prec;}
void setLowPrecision(int low_prec) {_lowPrec = low_prec;}    
void print();
bool wol_is_xor_expr();
bool wol_is_xnor_expr();
bool wol_is_const_zero_or_ones_expr() {return false;}
std::vector<WolNodeSptr> getNeighbors();

private: //methods

private: //data
  int _highPrec;
  int _lowPrec;
  std::array<WolNodeSptr, 3> _children;
};

//operators
inline bool operator ==(const WolNodeSptr &lhs, const WolNodeSptr &rhs) {
  return lhs->getId() == rhs->getId(); }
inline bool operator <(const WolNodeSptr &lhs, const WolNodeSptr &rhs) {
  return lhs->getId() < rhs->getId();}

}


#endif
