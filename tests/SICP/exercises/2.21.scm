(define (square-list items)
  (if (null? items)
      items
      (cons ((lambda (x) (* x x)) (car items))
            (square-list (cdr items)))))
(define (square-list items)
  (map1 (lambda (x) (* x x)) items))

(square-list (list 1 2 3 4))