(define (make-account balance password)
  (define (withdraw amount)
    (if (>= balance amount)
        (begin (set! balance (- balance amount))
               balance)
        "Insufficient funds"))
  (define (deposit amount)
    (set! balance (+ balance amount))
    balance)
  (define (call-the-cops)
    (lambda x "call-the-cops"))
  (define error-count 0)
  (define (dispatch p m)
    (if (eq? p password)
        (begin
          (set! error-count 0)
          (cond ((eq? m 'withdraw) withdraw)
                ((eq? m 'deposit) deposit)
                (else "Unknow request -- MAKE-ACCOUNT")))
        (begin
          (set! error-count (+ error-count 1))
          (if (= error-count 7)
              (call-the-cops)
              (lambda x "Incorrect password")))))
  dispatch)

(define acc (make-account 100 'secret-password))
((acc 'secret-password 'withdraw) 40) ;60
(define (dotimes f n)
  (if (> n 0)
      (begin
        (f)
        (dotimes f (- n 1)))))
(dotimes
  (lambda ()
    (display ((acc 'some-other-password 'deposit) 50))
    (newline))
  7) ; 输出6次Incorrect password和一次call-the-cops