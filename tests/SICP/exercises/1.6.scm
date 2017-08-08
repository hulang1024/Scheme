(define (new-if predicate then-clause else-clause)
  (cond (predicate then-clause)
        (else else-clause)))

(define (eval-seq? arg1 arg2)
  ((arg1)(arg2)))

(if (= 2 3) 0 5)
;5
(new-if (= 2 3) 0 5)
;5
(if (= 1 1) 0 5)
;0
(new-if (= 1 1) 0 5)
;0
(if (= 2 3) (display "2") (display "1"))
;1
(new-if (= 2 3) (display "2") (display "1"))
;21

