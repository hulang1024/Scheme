(define (smooth f)
  (define dx 0.00001)
  (lambda (x) (/ 3
                 (+ (f (- x dx))
                    (f x)
                    (f (+ x dx))))))
(((repeated smooth 2) square) 2)