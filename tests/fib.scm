
(define (cost-eval exp)
    (let [[ret #f] [t (clock)]]
        (set! ret (eval exp))
        (set! t (- (clock) t))
        (display "result: ")
        (display ret)
        (display "\n")
        (display "elapse: ")
        (display t)
        (display " ms")
        (display "\n")
        t))
        
(define (fib n)
  (if (> n 2)
      (+ (fib (- n 1))
         (fib (- n 2)))
      1))
        
(cost-eval '(fib 30))