(set-logic QF_BV)
(set-info :source |
Bit-reversal rev(x)
From the book "Hacker's delight" by Henry S. Warren, Jr., page 101
We verify rev(rev(x)) = x

Contributed by Robert Brummayer (robert.brummayer@gmail.com)
|)
(set-info :smt-lib-version 2.0)
(set-info :category "crafted")
(set-info :status unsat)
(declare-fun x () (_ BitVec 32))
(assert (let ((?v_0 ((_ zero_extend 27) (_ bv1 5)))) (let ((?v_1 (bvor (bvshl (bvand (_ bv10 32) (_ bv1431655765 32)) ?v_0) (bvlshr (bvand (_ bv10 32) (_ bv2863311530 32)) ?v_0))) (?v_2 ((_ zero_extend 27) (_ bv2 5)))) (let ((?v_3 (bvor (bvshl (bvand ?v_1 (_ bv858993459 32)) ?v_2) (bvlshr (bvand ?v_1 (_ bv3435973836 32)) ?v_2))) (?v_4 ((_ zero_extend 27) (_ bv4 5)))) (let ((?v_5 (bvor (bvshl (bvand ?v_3 (_ bv252645135 32)) ?v_4) (bvlshr (bvand ?v_3 (_ bv4042322160 32)) ?v_4))) (?v_6 ((_ zero_extend 27) (_ bv8 5)))) (let ((?v_7 (bvor (bvshl (bvand ?v_5 (_ bv16711935 32)) ?v_6) (bvlshr (bvand ?v_5 (_ bv4278255360 32)) ?v_6))) (?v_8 ((_ zero_extend 27) (_ bv16 5)))) (let ((?v_9 (bvor (bvshl (bvand ?v_7 (_ bv65535 32)) ?v_8) (bvlshr (bvand ?v_7 (_ bv4294901760 32)) ?v_8)))) (let ((?v_10 (bvor (bvshl (bvand ?v_9 (_ bv1431655765 32)) ?v_0) (bvlshr (bvand ?v_9 (_ bv2863311530 32)) ?v_0)))) (let ((?v_11 (bvor (bvshl (bvand ?v_10 (_ bv858993459 32)) ?v_2) (bvlshr (bvand ?v_10 (_ bv3435973836 32)) ?v_2)))) (let ((?v_12 (bvor (bvshl (bvand ?v_11 (_ bv252645135 32)) ?v_4) (bvlshr (bvand ?v_11 (_ bv4042322160 32)) ?v_4)))) (let ((?v_13 (bvor (bvshl (bvand ?v_12 (_ bv16711935 32)) ?v_6) (bvlshr (bvand ?v_12 (_ bv4278255360 32)) ?v_6)))) (not (= (bvnot (ite (= (bvor (bvshl (bvand ?v_13 (_ bv65535 32)) ?v_8) (bvlshr (bvand ?v_13 (_ bv4294901760 32)) ?v_8)) (_ bv10 32)) (_ bv1 1) (_ bv0 1))) (_ bv0 1))))))))))))))
(check-sat)
(exit)
