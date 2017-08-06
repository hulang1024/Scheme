;;;;
;;;; A stack implementation.
;;;;
 
(define (make-stack) 
  (define stack '()) 
  (define (empty?) (null? stack)) 
 
  (define (top) 
    (if (null? stack) 
        (error "Stack is empty -- TOP"
               stack) 
        (car stack))) 
 
  (define (push! object) 
    (set! stack (cons object stack)) 
    object) 
 
  (define (pop!) 
    (if (null? stack) 
        (error "Stack underflow -- POP!"
               stack) 
        (let ((object (car stack))) 
          (set! stack (cdr stack)) 
          object))) 
  (define (print)
    (display stack)
    (newline))
  (define (dispatch op . args) 
    (case op
      ((empty?) (apply empty? args)) 
      ((top) (apply top args)) 
      ((push!) (apply push! args)) 
      ((pop!) (apply pop! args))
      ((print) (apply print args))
      (else
        (error "Unknown stack operation -- DISPATCH" op)))) 
  dispatch)

 ;;test
(define s (make-stack))
(s 'push! 1)  ;1
(s 'print)    ;(1)
(s 'push! 2)  ;2
(s 'print)    ;(2 1)
(s 'top)      ;2
(s 'pop!)     ;2
(s 'print)    ;(1)
(s 'top)      ;1
(s 'empty?)   ;#f
(s 'pop!)     ;1
(s 'print)    ;()
(s 'empty?)   ;#t
(s 'pop!)     ;Stack underflow -- POP!