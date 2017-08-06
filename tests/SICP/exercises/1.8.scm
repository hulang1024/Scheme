;求立方 
(define (cute x)
  (* x x x))
;求平方
(define (square x)
  (* x x))
;求立方根
(define (curt x)
  ;迭代
  (define (iter guess x)
    (if (good-enough? guess x)
        guess
        (iter (improve guess x)
              x)))
  ;猜测（近似）是否足够好
  (define (good-enough? guess x)
    (< (abs (- (cute guess) x)) 0.001))
  ;改进
  (define (improve guess x)
    (/ (+ (/ x (square guess))
          (* 2 guess))
       3))

  (iter 1 x))

(curt 27)