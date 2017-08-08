(define (sum-of-two-max a b c)
  (cond ((and (>= a c) (>= b c)) (+ a b))
        ((and (>= a b) (>= c b)) (+ a c))
        ((and (>= c a) (>= b a)) (+ b c))))

(sum-of-two-max 1 1 1)
;2
(sum-of-two-max 1 2 3)
(sum-of-two-max 1 3 2)
(sum-of-two-max 2 1 3)
(sum-of-two-max 2 3 1)
(sum-of-two-max 3 2 1)
(sum-of-two-max 3 1 2)
;5