(if (> 3 2) 'yes 'no) ; yes
(if (> 2 3) 'yes 'no) ; no
(if (> 3 2)
    (- 3 2)
    (+ 3 2)) ; 1


;;;派生表达式
(cond ((> 3 2) 'greater)
      ((< 3 2) 'less)) ; greater
(cond ((> 3 3) 'greater)
      ((< 3 3) 'less)
      (else 'equal)) ; equal
(cond)
(cond (#t 1 2 (display 3) 4)) ; 34
(case (* 2 3)
      ((2 3 5 7) 'prime)
      ((1 4 6 8 9) 'composite)) ;composite
(case 1)
(case (begin (display 'c) 4)
      ((3) 'prime)
      ((4) 'composite)) ;ccomposite
(case (* 2 3)
      (else 2)) ;2
(case 1
      ((1) (display 2) 'b '(c d))) ;2(c d)
(and (= 2 2) (> 2 1)) ; #t
(and (= 2 2) (< 2 1)) ; #f
(and 1 2 'c '(f g)) ; (f g)
(and) ; #t

(or (= 2 2) (> 2 1)) ; #t
(or (= 2 2) (< 2 1)) ; #t
(or #f #f #f) ; #f
(or (memq 'b '(a b c))
    (/ 3 0)) ; (b c)

(unless (> 1 3)
        (display 4) 5)  ; 45
(when (< 1 3)
        (display 4) 6)  ; 46
