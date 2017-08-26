(define (fib n)
  (if (> n 2)
      (+ (fib (- n 1))
         (fib (- n 2)))
      1))
        
;(cost-eval '(fib 30))