(procedure? car) ; #t
(procedure? 'car) ; #f
(procedure? (lambda (x) (* x x))) ; #t
(procedure? '(lambda (x) (* x x))) ; #f

(apply + (list 3 4)) ; 7
(define compose
  (lambda (f g)
    (lambda args
      (f (apply g args)))))
((compose square  *) 12 75) ; 810000


(map cadr '((a b) (d e) (g h))) ; (b e h)
(map + '(1 2 3) '(4 5 6)) ; (5 7 9)

(for-each display '(1 2 3)) ; (1 2 3)



(define (f x y . z)
  (display x)
  (display y)
  (display z)
  (newline))
(f 1 2)  ;12()(1 2)
(f 1 2 3 4)  ;12(3 4)(1 2 3 4)


(void? 1) ;#f
(void? (void)) ;#t
(void? (begin)) ;#t
(void? (define a 2)) ;#t
(void? (set! a 3)) ;#t