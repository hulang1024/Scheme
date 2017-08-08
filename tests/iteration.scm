(let ((x '(1 3 5 7 9)))
     (let iter ((x x) (sum 0))
         (if (null? x)
             (begin sum)
             (begin (iter (cdr x) (+ sum (car x))))))) ; 25
             
(define (fact-let n)
  (let loop ((n1 n)(p n))           
    (if(= n1 1)                  
        p
        (let ((m (- n1 1)))
          (loop m (* p m))))))
(fact-let 6)  ; 720

(let ((x '(1 3 5 7 9)))
  (do ((x x (cdr x))
       (sum 0 (+ sum (car x))))
      ((null? x) sum)))  ; 25
      
(let ((i 10))
    (while (>= i 0)
           (display i)
           (display " ")
           (set! i (- i 1))))  ; 10 9 8 7 6 5 4 3 2 1 0 
           
(for i in (10 to 20)
       (when (even? i)
             (display i) (display " "))) ;10 12 14 16 18


;; memoization version
(define fibonacci
  (let ((memo '()))
    (define (fib n)
      (let ((ret (assoc n memo)))
        (if (null? ret)
            (begin
             (if (>= n 2)
                 (begin
                  (set! ret (+ (fib (- n 1)) (fib (- n 2))))
                  (set! memo (append memo (list (list n ret))))
                  ret)
                 n))
            (cadr ret))))
    fib))
           
(for i in (10 to 100)
     (println (fibonacci i)))
