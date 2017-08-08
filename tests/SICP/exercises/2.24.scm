(list 1 (list 2 (list 3 4))) ; (1 (2 (3 4)))
;代换为: (cons 1 (cons (cons 2 (cons (cons 3 (cons 4 '())) '())) '()))
