;author: http://www.paulgraham.com/rootsoflisp.html
(define (atom x)
  (not (pair? x)))
(define null. null?)
(define eq equal?)
(define (pair. x y)
  (cond ((or (atom x) (atom y)) '())
        ((cons (list (car x) (car y))
               (pair. (cdr x) (cdr y))))))
(define (append. x y)
  (cond ((atom x) y)
        ((cons (car x)
               (append. (cdr x) y)))))
;(append. '(a b) '(c d))
(define (assoc. x y)
  (cond ((null? y) '())
        ((equal? x (caar y)) (cadar y))
        ((assoc. x (cdr y)))))
;(assoc. 'x '((x 1) (y 4)))
;(assoc. 'y '((x 1) (y 4)))



;---------------------------------------
;e 要求值的表达式
;a 环境
(define (eval. e a)
  (cond
    ;cond#1.e是原子,查找它在环境中的值
    ((atom e) (assoc. e a))
    ;e是表
    ;cond#2.(a...),a是原子,包括所有的原始操作符
    ((atom (car e))
     (cond 
       ((eq (car e) 'quote) (cadr e))
       ((eq (car e) 'atom)  (atom   (eval. (cadr e) a)))
       ((eq (car e) 'eq)    (eq     (eval. (cadr e) a)
                                    (eval. (caddr e) a)))
       ((eq (car e) 'car)   (car    (eval. (cadr e) a)))
       ((eq (car e) 'cdr)   (cdr    (eval. (cadr e) a)))
       ((eq (car e) 'cons)  (cons   (eval. (cadr e) a)
                                    (eval. (caddr e) a)))
       ((eq (car e) 'cond)  (evcon. (cdr e) a))
       ('t (eval. (cons (assoc. (car e) a)
                        (cdr e))
                  a))))
    ;cond#3.
    ((eq (caar e) 'label)
     (eval. (cons (caddar e) (cdr e))
            (cons (list (cadar e) (car e)) a)))
    ;cond#4.((lambda (p1...pn) e) a1...an)
    ((eq (caar e) 'lambda)
     (eval. (caddar e);e
            (append. (pair. (cadar e) (evlis. (cdr  e) a));((p1 a1)...(pn an))
                     a)))))

(define (evcon. c a)
  (cond ((eval. (caar c) a)
         (eval. (cadar c) a))
        ('t (evcon. (cdr c) a))))

(define (evlis. m a)
  (cond ((null. m) '())
        ('t (cons (eval.  (car m) a)
                  (evlis. (cdr m) a)))))
;---------------------------------------


;test cond#1
(eval. 'y '((x 3) (y 2)))
;test cond#2.1
(eval. '(quote y) '())
(eval. (cadr '(eq '(a b) '(a b))) '())
;test cond#2.2
(eval. '(atom '(a b)) '())
(eval. '(atom '()) '())
;test cond#2.3
(eval. '(eq 'a 'a) '())
;test cond#2.4
(eval. '(car '(a b c)) '())
;test cond#2.6
(eval. '(cons x '(b c)) '((x a) (y b)))
;test cond#2.7
(eval. '(cond ((atom x) 'atom)
              ('t 'list))
       '((x '(a b))))
;test cond#2.8
(eval. '(f '(b c))
       '((f (lambda (x) (cons 'a x)))))
(eval. '(f '(b c))
       '((x 1) (f (lambda (x) (cons 'a x)))))
;test cond#3
(eval. '((label firstatom (lambda (x)
                            (cond ((atom x) x)
                                  ('t (firstatom (car x))))))
        y)
      '((y ((a b) (c d)))))
;;test cond#4
(eval. '((lambda (x y) (cons x (cdr y))) 'a '(b c d))
       '())


(eval. '((lambda (x y) (cons x (cdr y)))
         'a
         '(b c d))
       '())

(eval. '(cons x (cdr y))
       '((x a) (y (b c d))))
