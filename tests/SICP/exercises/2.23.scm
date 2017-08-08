(define (for-each1 p l)
  (if (null? l)
      '()
      (begin
        (p (car l))
        (for-each1 p (cdr l)))))

(for-each1 display (list 1 2 3 4))