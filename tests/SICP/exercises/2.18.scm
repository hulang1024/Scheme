(define (reverse1 lst)
  (define (iter items result)
    (if (null? items)
        result
        (iter (cdr items)
              (cons (car items) result))))
  (iter lst '()))

(reverse1 (list 1 4 9 16 25)) 