(define (a-plus-abs-b a b)
  ((if (> b 0) + -) a b))

(a-plus-abs-b 3 4)
(a-plus-abs-b 3 -4)
(a-plus-abs-b -3 4)
(a-plus-abs-b -3 -4)