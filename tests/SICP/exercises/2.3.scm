; abstraction
(define (rect-area rect)
  (* (rect-l rect) (rect-w rect)))

(define (rect-circumference rect)
  (/ (* (rect-l rect) (rect-w rect)) 2))

; represent 1
;(define (make-rect l w) (cons l w))
;(define (rect-l rect) (car rect))
;(define (rect-w rect) (cdr rect))
;(define rect (make-rect 4 5))

; represent 2
;(load "2.2.rkt")
(define (make-point x y) (cons x y))
(define (x-point p) (car p))
(define (y-point p) (cdr p))
(define (make-rect p1 p2) (cons p1 p2))
(define (rect-p1 rect) (car rect))
(define (rect-p2 rect) (cdr rect))
(define (rect-l rect)
  (- (x-point (rect-p1 rect)) (x-point (rect-p2 rect))))
(define (rect-w rect)
  (- (y-point (rect-p1 rect)) (y-point (rect-p2 rect))))
(define rect (make-rect (make-point 0 4) (make-point 4 9)))

(rect-area rect)
(rect-circumference rect)

