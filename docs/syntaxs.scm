(define-syntax case
  (syntax-rules (else)
    ((_ key ((datum ...) exp ...) ...)
     (let ((v key))
       (cond ((memv v '(datum ...)) exp ...)
             ...)))
    ((_ key ((datum ...) exp1 ...) ... (else exp2 ...))
     (let ((v key))
       (cond ((memv v '(datum ...)) exp1 ...)
             ...
             (else exp2 ...))))))

(define-syntax when
  (syntax-rules ()
    ((_ test body ...)
     (if test
         (begin body ...)))))

(define-syntax unless
  (syntax-rules ()
   ((_ test body ...)
     (if (not test)
         (begin body ...)))))

(define-syntax or
  (syntax-rules ()
    ((or) #f)
    ((or e) e)
    ((or e1 e2 ...)
     (let ((temp e1))
       (if temp
           temp
           (or e2 ...))))))

(define-syntax do
  (syntax-rules (variable init)
    ((do ((variable init step) ...)
         (test expression ...)
         command ...)
     (let iter ((variable init))
         (if test
             (begin expression ...)
             (begin command ...
                    (iter step ...)))))))

;C like style's while
(define-syntax while
  (syntax-rules ()
    ((_ test body ...)
     (let loop ()
       (when test
             body ...
             (loop))))))

(define-syntax for
  (syntax-rules (to)
    ;; loop in sequence
    ;; (for i (0 to 10) do something...)
    ((_ i in (from to end) body ...)
     (let loop ((i from))
       (when (< i end)
             body ...
             (loop (+ i 1)))))
    ;; loop in list
    ;; (for i in '(a b c) do something...)
    ((_ i in lst body ...)
     (let loop ((l lst))
       (unless (null? l)
               (let ((i (car l)))
                 body ...
                 (loop (cdr l))))))))
