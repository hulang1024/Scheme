(define (make-monitored f)
  (let ((calls 0))
    (lambda (x)
      (cond ((eq? x 'how-many-calls) calls)
            ((eq? x 'reset-count) (set! calls 0))
            (else
              (let ((ret (f x)))
                (set! calls (+ calls 1))
                ret))))))

(define s (make-monitored square))
(s 10);100
(s 'how-many-calls);1
(s 'how-many-calls);1
(s 5);25
(s 'how-many-calls);2
(s 'reset-count)
(s 'how-many-calls);0