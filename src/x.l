
; #lang problue-lisp/scm/lisp/psl/faiz/p-scm/pbs
(load "../stdlib/lib.scm")
; (load "lib.scm")
; log
;; roots:
;;; load,disp,exit,time; cons,car,cdr,quote,pair?,eq?,cond; set!,def,lam,apply,eval; '();
;;; begin,length;
;; (void) nil `#f #t '() 0 1
;new: or/and when/unless def(asd a s d);case cond


; c-code
;; exit
;; system.c
;; static scm_object* exit_prim (int, scm_object* []);
;; void scm_add_prim
;;; (scm_env *env, const char *name, scm_prim prim, int min_arity, int max_arity)
;; void scm_init_..(){
;;; Scm_add_prim(env, exit, 0, 1);
;; static scm_object* exit_prim (int argc, scm_object *argv[]) {
 ;; exit( argc>0? argv[0]): 0); //int?
 ;; return scm_void;
;; }


;std-ex
;let~=lam cond~=if let*
;push pop delete cls cd echo echol
;time call/cc delay
;c
;(lam(x y z)z) bignum/gmp
;;||; #||# {-xx-}
(define eq? eqv?)
(define nil? null?)
(define (quit) (quote)) ;<-(define)
(define (refresh) (load "x.l")) ;wo delete


;faiz-dsl
;def ?
;nth rest
;not ;if
(define *v (void))
(define *n nil)
;(define *f *n)
(define *f #f)
(define *t #t)
;
(define ev eval)
(define len length)
(define (last li)
    (if(= 1(len li))
      (car li)
      (last (cdr li)) 
)   )
(define (f . xs) [if[nil? xs] *v (last xs)])
(define eq eq?)
;
; (define (not x)
        ; (if (eqv? x *f) *t *f))
(define (true? x) (not(not x))) ;;->bool
(define tru? true?)
; (define (and . xs)
    ; (if(pair? xs)
      ; (if(not(car xs))
        ; *f
        ; (ev(cons 'and (cdr xs)))
      ; )
      ; *t
; )   )
; (define (or . xs) ;1x=1 00=0
    ; (if(pair? xs)
      ; (if(car xs)
        ; *t
        ; (ev(cons 'or (cdr xs)))
      ; )
      ; *f
; )   )
(define (xor x y)
;_xor ;x=x ;xx=0 xy=1 ;xyz=(x^y)^z ;xor x y z 图像上不满足结合律, bool里面满足结合律
    (if(eq?(not x)(not y))
      *f *t
)   )
;xor by and or not
; (define (xor2 x y)
    ; (or (and(not y)x) (and(not x)y) )
; )
;(define ) ;(asd '(1 2 3 4 5)) -> '((1 2) (3 4) '(5))
(define (xor* . xs)
    *v
)

(define (append x y)
    (if
      (nil? x) y
      [cons (car x) (append(cdr x)y)]
)   )
(define (pair x y) ;
    (if [and(nil? x)(nil? y)]
      nil
      (cons
        [list(car x)(car y)]
        (pair(cdr x)(cdr y))
)   ) )
(define (map1 g li) ;(map (lam(x)(+ x 1)) li) ;rmap
  (if(nil? li)
    nil
    (cons
      (g (car li))
      (map1 g (cdr li))
) ) )
(define (map2 . xs)
  ( (lambda(g l1)  
      (if (>(len xs)2)
        ( (lambda(l2)        
            (if (or(nil? l1)(nil? l2))
              nil
              (cons
                (g (car l1) (car l2))
                [map2 g (cdr l1) (cdr l2)]
          ) ) )
          [caddr xs]
        )
        (if (nil? l1)
          nil
          (cons
            (g (car l1))
            [map2 g (cdr l1)]
    ) ) ) )
    [car xs] [cadr xs]
) )
(define map map2)
;
(define (reverse li)
    (define (cons-rev li ans) ;g_
      (if (eq? li '())  
          ans ;
          (cons-rev
            (cdr li)
            (append (list (car li)) ans)
    ) )   )
    (cons-rev li '())
)


;map 1 xs ;apply n xs ;l2r.each.n->lis ;l2r.i2o ;l2r.o2i ;
;asdf=(a^s)^(d^f)=...?
; (define (xor . xs)
    ; (apply xor xs)
; )

; (define (apply-each n g li)
    ; (map (lambda()) (list-each 2 li))
; )
; (define (xor . xs) ;211,121
    ; (apply-each 2 _xor xs)
; )

;
(define (*1+ . xs) (apply + (cons 1 xs)) )
(define *+1 *1+)
(define (*1- . xs) (apply - (cons 1 xs)) ) ;
(define (*-1 . xs) (- (apply + xs) 1) )
;
(define disp display)
(define newl newline)
;
(define (mk-cps g)
  (lambda args.
    (
      (lambda (r)
        ((car r)
          (apply g       ;;f.->nil?
            (reverse (cdr r))
      ) ) )
      (reverse args.)
) ) )
(define +& (mk-cps +))
;let/lam
        
;mutiple values version
; (define (call/cc* g)
  ; (shift k.
    ; (call-with-values
      ; (lambda ()
        ; (g
          ; (lambda xs
            ; (shift _. (apply k. xs))
      ; ) ) )
      ; k.
; ) ) )
;shift k. ;call-with-values
     
(define (echo . xs) ;;echo 1 '() ;
    (if [pair? xs]
        (f (disp (car xs))
           (ev (cons 'echo (cdr xs))) ;
)   )   )

(define echol println)
;(define (echo . xs) (map disp xs))
;(define (echol . xs) (map disp xs) (newl))
(define refr refresh)
(define q quit)

;test
(define (fib-test n)
  (if (> n 2)
    (+ (fib-test (- n 1)) (fib-test (- n 2)))
    1
) )
(define (fib-tail-hlp n) ;def(asd a b c.)... ;no-gmp,so-n=0~46
    [lambda (x y)
      (if (< n 1)
        y
        ([fib-tail-hlp (- n 1)] (+ x y) x)    
)   ] )
(define (fib-tail n) ([fib-tail-hlp n] 1 0)
)
(define fib fib-tail)


;main
(define a *v)
(set! a '(7 11))
;(echol a)


;(quit)