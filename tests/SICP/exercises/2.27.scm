(define (deep-reverse lst)
  (define (iter items result)
    (if (null? items)
        result
        (iter (cdr items)
              (cons
               (if (pair? (car items))
                   (iter (car items) '())
                   (car items))
               result))))
  (iter lst '()))

(define x (list (list 1 2) (list 3 4) 5))
(deep-reverse x) 
  