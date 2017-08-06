(define (make-accumulator sum)
  (lambda (n)
    (set! sum (+ sum n))
    sum))

(define A (make-accumulator 5))
(A 10);15
(A 10);25

(define B (make-accumulator 6))
(A 5);30
(A 5);35