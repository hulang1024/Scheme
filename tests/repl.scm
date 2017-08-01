(define (loop)
  (display (eval (read)))
  (loop))
(loop)