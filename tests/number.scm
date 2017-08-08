3 ; 3
+3 ; 3
-3 ; -3
3.1415 ;3.1415

(integer? 3) ;#t
(real? 3.2)  ;#t
(number? 5)  ;#t

(zero? 0)      ; #t
(positive? 0)  ; #f
(positive? 1)  ; #t
(negative? 0)  ; #f
(negative? -1) ; #t
(odd? 7)       ; #t
(odd? 6)       ; #f
(even? 7)      ; #f
(even? 6)      ; #t
