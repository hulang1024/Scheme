(define add3
  (lambda (x) (+ x 3)))
(add3 3) ; 6
(define first car)
(first '(1 2)) ; 1

(let ((x 5))
  (define foo (lambda (y) (bar x y)))
  (define bar (lambda (a b) (+ (* a b) a)))
  (foo (+ x 3))) ; 45