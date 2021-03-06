(set-logic QF_BV)
(set-info :source |
	Constructed by Trevor Hansen to test bvnor nesting.
|)
(set-info :smt-lib-version 2.0)
(set-info :category "check")
(set-info :status unsat)
(declare-fun v0 () (_ BitVec 1))
(declare-fun v1 () (_ BitVec 1))
(declare-fun v2 () (_ BitVec 1))
(declare-fun v3 () (_ BitVec 1))
;(assert (= v2 #b1))
;(assert (or (= v0 #b1) (= v1 #b1)))
;(assert (= v0 #b0))
(assert (not (= (concat v3 v3) ((_ repeat 2) v0))))
(check-sat)
(exit)
