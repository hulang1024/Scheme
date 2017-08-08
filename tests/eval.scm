(eval '(* 7 3) (interaction-environment))  ; 21
(let ((f (eval '(lambda (f x) (f x x))
               (interaction-environment))))
  (f + 10)) ; 20