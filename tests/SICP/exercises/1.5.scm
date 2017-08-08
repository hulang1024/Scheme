(define (p) (p))
(define (test x y)
  (if (= x 0)
      0
      y))
(test 0 (p));如果解释器的求值顺序采用的是应用序,解释器会死，否则输出0