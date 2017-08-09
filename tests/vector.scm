#() ;#()
#(1 2) ;#(1 2)
#(1 #\a) ;#(1 #\a)
(vector-ref #(1 '(2 3)) 1) ;'(2 3)
(vector? 1) ;#f
(vector? #(1)) ;#t
(vector? #()) ;#t
(vector-ref #(1 2) 1) ;2
(vector-length #(1 2)) ;2
(vector #\a 1 2) ;#(#\a 1 2)
(define v (make-vector 3))
v ;#(0 0 0)
(vector-set! v 0 1)
v ;#(1 0 0)
(vector-set! v 1 2)
v ;#(1 2 0)
(list->vector '(1 2 3)) ;#(1 2 3)
(vector->list v) ;(1 2 0)
(vector-fill! v 3)
v ;#(3 3 3)