;;author: problue
;;反转(镜像)二叉树，如:
;;  1            1
;; / \          / \
;;2   3   =>   3   2
;;   /          \
;;  4            4
;;

;二叉树的构造器和选择器
(define binary-tree-entry car)
(define binary-tree-brance cdr)
(define binary-tree-left cadr)
(define binary-tree-right cddr)
(define (binary-tree-set-left! t val)
  (set-car! (binary-tree-brance t) val))
(define (binary-tree-set-right! t val)
  (set-cdr! (binary-tree-brance t) val))
(define (tree-leaf? x) (atom? x))
(define (make-binary-tree entry l r)
  (cons entry (cons l r)))

;打印二叉树
(define (binary-tree-print t)
  (define (rec t level)
    (cond ((null? t) (display "[null]"))
          ((tree-leaf? t) (display t))
          (else (display (binary-tree-entry t))
                (define indent (make-string (* level 2)))
                (string-fill! indent #\space)
                (newline)
                (display indent)
                (rec (binary-tree-left t) (inc level))
                (newline)
                (display indent)
                (rec (binary-tree-right t) (inc level)))))
  (rec t 1)
  (newline))

;反转二叉树
(define (binary-tree-invert t)
  (cond ((tree-leaf? t) #t)
        (else
         (let ((left (binary-tree-left t))
               (right (binary-tree-right t))
               (temp nil))
           (set! temp left)
           (set! left right)
           (set! right temp)
           (binary-tree-set-left! t left)
           (binary-tree-set-right! t right))
         (binary-tree-invert (binary-tree-left t))
         (binary-tree-invert (binary-tree-right t)))))


;问题描述
(define t1 (make-binary-tree 1 2 (make-binary-tree 3 4 nil)))
(binary-tree-print t1)
(binary-tree-invert t1)
(println "反转之后:")
(binary-tree-print t1)