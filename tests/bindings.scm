(let ((x 2) (y 3))
  (* x y)) ; 6
(let ((x 2) (y 3))
  (let ((x 7)
        (z (+ x y)))
    (* z x))) ; 35