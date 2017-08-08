(begin)
(define x 0)
(begin (set! x 5)
       (+ x 1)) ; 6
(begin (display "4 plus 1 equals ")
       (display (+ 4 1))) ; 未定义，并打印输出4 plus 1 equals 5