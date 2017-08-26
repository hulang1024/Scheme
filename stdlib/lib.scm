;;; list selectors
(define (caar x) (car (car x))) 
(define (cadr x) (car (cdr x))) 
(define (cddr x) (cdr (cdr x))) 
(define (cdar x) (cdr (car x))) 

(define (caaar x) (car (car (car x)))) 
(define (caadr x) (car (car (cdr x)))) 
(define (cadar x) (car (cdr (car x)))) 
(define (caddr x) (car (cdr (cdr x)))) 
(define (cdaar x) (cdr (car (car x)))) 
(define (cdadr x) (cdr (car (cdr x)))) 
(define (cddar x) (cdr (cdr (car x)))) 
(define (cdddr x) (cdr (cdr (cdr x)))) 

(define (caaaar x) (car (car (car (car x))))) 
(define (caaadr x) (car (car (car (cdr x))))) 
(define (caadar x) (car (car (cdr (car x))))) 
(define (caaddr x) (car (car (cdr (cdr x))))) 
(define (cadaar x) (car (cdr (car (car x))))) 
(define (cadadr x) (car (cdr (car (cdr x))))) 
(define (caddar x) (car (cdr (cdr (car x))))) 
(define (cadddr x) (car (cdr (cdr (cdr x))))) 
(define (cdaaar x) (cdr (car (car (car x))))) 
(define (cdaadr x) (cdr (car (car (cdr x))))) 
(define (cdadar x) (cdr (car (cdr (car x))))) 
(define (cdaddr x) (cdr (car (cdr (cdr x))))) 
(define (cddaar x) (cdr (cdr (car (car x))))) 
(define (cddadr x) (cdr (cdr (car (cdr x))))) 
(define (cdddar x) (cdr (cdr (cdr (car x))))) 
(define (cddddr x) (cdr (cdr (cdr (cdr x))))) 

(define nil '())


;;; bool

(define true #t)
(define false #f)


;;; write
(define (newline) (display "\n"))
(define print display)
(define (println obj) (display obj)(newline))
  
;;; math
(define (square x) (* x x))
(define (cube x)(* x x x))
(define (inc x) (+ x 1))
(define (dec x) (- x 1))
(define (abs x) (if (> x 0) x (- x)))
(define max (lambda (x y) (if (> x y) x y)))
(define min (lambda (x y) (if (< x y) x y)))

(define (expt b n)
  (define (expt-iter b counter product)
    (if (= counter 0)
        product
        (expt-iter b
                   (- counter 1)
                   (* b product))))
  (expt-iter b n 1))
 
(define (gcd a b) (if (= b 0) a (gcd b (remainder a b))))
 
 

;;; list
(define (atom? x)
  (not (pair? x)))


(define (assoc x y)
  (cond ((null? y) '())
        ((equal? x (caar y)) (car y))
        ((assoc x (cdr y)))))

(define (assv x y)
  (cond ((null? y) '())
        ((eqv? x (caar y)) (car y))
        ((assv x (cdr y)))))

(define (assq x y)
  (cond ((null? y) '())
        ((eq? x (caar y)) (car y))
        ((assq x (cdr y)))))



(define subst
  (lambda (new old slist)
    (if (null? slist)
      '()
       (cons
	     (subst-in-s-exp new old (car slist)) 
	     (subst new old (cdr slist))))))

(define subst-in-s-exp
  (lambda (new old sexp)
    (cond ((symbol? sexp)
          (if (eqv? sexp old) new sexp))
          ((pair? sexp) (subst new old sexp))
          (else sexp))))