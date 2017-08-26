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
        
(define (rand-list n)
  (let loop [[i 0] [l '()]]
    (if (>= i n) l
      (loop (+ i 1) (cons i l)))))
      
(define (rand-vector n)
  (let loop [[i 0] [v (make-vector n)]]
    (if (>= i n) v
        (begin
          (vector-set! v i i)
          (loop (+ i 1) v)))))
      

(define N 50000)
(define list-sum 0)
(define vector-sum 0)

(cost-eval
  '(let [[n 0] [l (rand-list N)]]
     (while (< n N)
       (set! list-sum (+ list-sum (list-ref l n)))
       (set! n (+ n 1)))))
       
(cost-eval
  '(let [[n 0] [v (rand-vector N)]]
     (while (< n N)
       (set! vector-sum (+ vector-sum (vector-ref v n)))
       (set! n (+ n 1)))))
       
(= 1249975000 list-sum vector-sum)