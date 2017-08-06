#t ; #t
#f ; #f
'#f ; #f

(boolean? #t);#t
(boolean? #f);#t
(boolean? 0) ; #f
(boolean? '());#f
(boolean? (> 1 2));#t
(boolean? 6);#f
(boolean? (boolean? *));#t

(not 3) ; #f
(not (list 3)) ; #f
(not #f) ; #t
(not '()) ; #f
(not (list)) ; #f

(not 0);#f
(not 1.2);#f
(not "");#f
(not "s");#f
(not +);#f