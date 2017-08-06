#\a ; 小写字母
#\A ; 大写字母

(char? #\c) ; #t

(char=? #\a #\b) ;#f
(char=? #\a #\a) ;#t

(char-upper-case? #\A) ;#t
(char-upper-case? #\a) ;#f
(char-lower-case? #\A) ;#f
(char-lower-case? #\a) ;#t

(char-upcase #\a) ;#\A
(char-downcase #\A) ;#\a