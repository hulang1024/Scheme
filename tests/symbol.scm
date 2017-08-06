(symbol? 'foo) ; #t
(symbol? (car '(a b))) ; #t
(symbol? "bar") ; #f
(symbol? 'nil) ; #t
(symbol? '()) ; #f
(symbol? #f) ; #f

(symbol->string 'flying-fish) ; "flying-fish"
(symbol->string 'Martin) ; "martin"
(symbol->string-ci 'Martin) ;"Martin"
(symbol->string (string->symbol "Malvina")) ; "Malvina"

(string->symbol "mISSISSIppi") ; 名字为"mISSISSIppi" 的符号