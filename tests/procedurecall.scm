(+ 3 4) ;7
((if #f + *) 3 4) ;12

;() ;error


(lambda (x) (+ x x)); 一个过程
((lambda (x) (+ x x)) 4); 8

(define reverse-subtract
    (lambda (x y) (- y x)))
(reverse-subtract 7 10)
;3

(define add4
    (let ((x 4))
        (lambda (y) (+ x y))))
(add4 6)
;10


((lambda x x) 3 4 5 6) ;(3 4 5 6)
((lambda (x y . z) z)
 3 4 5 6)
;(5 6)