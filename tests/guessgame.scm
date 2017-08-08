;;; 猜数字
;;; author: problue
(define (display-num msg)
  (string->number (prompt msg)))
(define (confirm s)
  (display (string-append s " y/n"))
  (equal? (read) 'y))
(define times 0)
(define (guess-loop)
  (define g (display-num "数"))
  (set! times (+ times 1))
  (let ((result #t))
    (cond ((< g answer) (set! result "小了~"))
          ((> g answer) (set! result "大了~"))
          (else (display (string-append "恭喜,你猜对了!尝试次数:"
                                       (number->string times)))
                (set! result #f)))
    (if result
      (if (confirm (string-append result "是否继续?"))
          (guess-loop)))))
(display "欢迎进入猜数字游戏~")
(define max (display-num "请输入游戏的最大数:"))
(if (> max 1)
  (begin
    (define answer (rand 1 max))
    (display "ok，我已生成一个数。现在开始，请猜猜看")
    (guess-loop)))