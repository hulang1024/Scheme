"The word \"recursion\" has many meanings."  ; "The word "recursion" has many meanings."
(string? "ss");#t
(make-string 6);"" or "\u0000\u0000\u0000\u0000\u0000\u0000"
(make-string 6 #\c)
(string-length (make-string 6));6
(string #\a #\b #\c);"abc"
(define s1 (string #\a #\b #\c #\d))
(string-ref s1 3);#\d
(string-set! s1 3 #\c)
(define s2 "abcc")
(string=? s1 s2);#t
(string=? s1 "ABCC");#f
(string-ci=? s1 "ABCC");#t
(substring s2 0 3);"abc"
(string->list s1);(#\a #\b #\c #\c)
(string-copy s2);"abcc"
(string-fill! s1 #\e)
(string-append s2 s1);"abcceeee"
