/*
 * wolgenengine.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: user
 */

#include <cassert>
#include "wolimplengine.h"
#include "wolexp.h"
#include "wolvalue.h"
#include "wolgenengine.h"


namespace wolver {

 /*
  * for op in node->parents
  * 	E_op = Fwd_Impl(value, R_c) /\ R_op
  * 	Ec = Back_Impl(E_op, value) /\ R_c
  * 	U1 = Back_Impl(R_op - E_op, Rc)
  * 	U2 = Back_Impl(E_op, R_c - E_c)
  * 	U_op = U1 \/ U2
  * 	U_total = U_total \/ U_op
  *
  * gen_value = R_node - U_total
  * assert(value in gen_value)
  *
  */
  WolValueSptr
  WolSimpleGenEngine::performGenerlization(WolNodeSptr node, WolValueSptr value) {

    WolEvalFactory *evalFactory = WolMgr::getInstance().getEvalFactory();
    auto parents = node->getParents();

    WolValueSptr U_total = nullptr;
    for (auto i : parents) {

      auto parent = dynamic_pointer_cast<WolComplexNode>(i.lock());
      assert(parent);

      auto R_op = parent->getValue();

      if (parent->getArity() == 2) {
	auto children = parent->getChildren();
	auto fNode =  (children[0] == node) ? children[1] : children[0];
	auto R_c = fNode->getValue();

	auto E_op = evalFactory->evalIntersection(
	                         parent->performForwardImplication(value, R_c),
	                         R_op);
	auto E_c = evalFactory->evalIntersection(
	                         fNode->performBackwardImplication(parent, E_op, value),
	                         R_c);
	auto U1 = node->performBackwardImplication(
	                         evalFactory->evalDiff(R_op, E_op),
	                         R_c);
	auto U2 = node->performBackwardImplication(
	                         E_op,
	                         evalFactory->evalDiff(R_c , E_c));
	auto U_op = evalFactory->evalUnion(U1, U2);
	U_total = evalFactory->evalUnion(U_total, U_op);

      }
      else if (parent->getArity() == 3) {
	auto children = parent->getChildren();
	WolNodeSptr fNode1 = nullptr;
	WolNodeSptr fNode2 = nullptr;
	WolValueSptr E_op = nullptr;
	WolValueSptr R_c1 = nullptr;
	WolValueSptr R_c2 = nullptr;
	WolValueSptr E_c1 = nullptr;
	WolValueSptr E_c2 = nullptr;

	if (children[0] == node) {
	  fNode1 = children[1];
	  fNode2 = children[2];
	  R_c1 = children[1]->getValue();
	  R_c2 = children[2]->getValue();
	  E_op = evalFactory->evalIntersection(
	            	      parent->performForwardImplication(value, R_c1, R_c2),
	  	              R_op);
	  E_c1 = evalFactory->evalIntersection(
		    fNode1->performBackwardImplication(parent, E_op, value, R_c2),
		    R_c1);
	  E_c2 = evalFactory->evalIntersection(
		    fNode2->performBackwardImplication(parent, E_op, value, R_c1),
		    R_c2);
	}
	else if (children[1] == node) {
	  fNode1 = children[0];
	  fNode2 = children[2];
	  R_c1 = children[0]->getValue();
	  R_c2 = children[2]->getValue();
	  E_op = evalFactory->evalIntersection(
	  	              parent->performForwardImplication(R_c1, value, R_c2),
	  	  	      R_op);
	  E_c1 = evalFactory->evalIntersection(
	 		    fNode1->performBackwardImplication(parent, E_op, value, R_c2),
	 		    R_c1);
	  E_c2 = evalFactory->evalIntersection(
	 		    fNode2->performBackwardImplication(parent, E_op, R_c1, value),
	 		    R_c2);
	}
	else {
	  assert(children[2] == node);
	  fNode1 = children[0];
	  fNode2 = children[1];
	  R_c1 = children[0]->getValue();
	  R_c2 = children[1]->getValue();
	  E_op = evalFactory->evalIntersection(
	  	  	      parent->performForwardImplication(R_c1, R_c2, value),
	  	  	      R_op);
	  E_c1 = evalFactory->evalIntersection(
	      fNode1->performBackwardImplication(parent, E_op, R_c2, value),
	      R_c1);
	  E_c2 = evalFactory->evalIntersection(
	      fNode2->performBackwardImplication(parent, E_op, R_c1, value),
	      R_c2);
	}


	auto U1 = node->performBackwardImplication(
	    evalFactory->evalDiff(R_op, E_op),
	    R_c1, R_c2);
	auto U2 = node->performBackwardImplication(
	    E_op,
	    evalFactory->evalDiff(R_c1, E_c1),
	    evalFactory->evalDiff(R_c2, E_c2));
	auto U_op = evalFactory->evalUnion(U1, U2);
	U_total = evalFactory->evalUnion(U_total, U_op);

      }
      else {
	assert(parent->getArity() == 1);

	auto E_op = evalFactory->evalIntersection(
	    parent->performForwardImplication(value),
	    R_op);
	auto U_op = node->performBackwardImplication(
	    evalFactory->evalDiff(R_op, E_op));
	U_total = evalFactory->evalUnion(U_total, U_op);
      }

    }
    auto gen_value = evalFactory->evalDiff(node->getValue(), U_total);

    return gen_value;
  }

}
