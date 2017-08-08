; 定义地图的尺寸及坐标(均使用双字符长度)
(define MAP_WIDTH    24)
(define MAP_HEIGHT   16)
(define MAP_BASE_X   1)
(define MAP_BASE_Y   1)

; 定义蛇的相关参数
(define SNAKE_MIN_LEN   5)

; 定义地图块的状态,分别为[空格|蛇头|蛇身|食物]
(define BS_SPACE     0)
(define BS_SHEAD     1)
(define BS_SBODY     2)
(define BS_STAIL     3)
(define BS_FOOD      4)

; 定义各对象的颜色,颜色定义详见[pcc32.h],顺序同上.
(define myColors (list pcc-CYAN pcc-MAGENTA pcc-RED pcc-GREEN pcc-YELLOW))

; 定义各对象的形状,注意是宽字符[◆◇□●★¤~◎＋],顺序同上.
(define mySharps (list "□" "◆" "■" "＋" "★"))

; 定义蛇的运动方向[上|下|左|右]
(define DIR_UP    1)
(define DIR_DOWN  2)
(define DIR_LEFT  3)
(define DIR_RIGHT 4)

(define make-snake-body cons)
(define snake-x car)
(define snake-y cdr)

(define mySnake '())

(define (initSnake)
  (set! mySnake (list (make-snake-body 10 3))))
  
(define (drawMap)
  )
  
(define (drawSnake)
  (pcc-setTextColor (list-ref myColors BS_SHEAD))
  (pcc-gotoTextPos (* (+ MAP_BASE_X (snake-x (list-ref mySnake 0)) 2))
                   (+ MAP_BASE_Y (snake-y (list-ref mySnake 0))))
  (display (list-ref mySharps BS_SHEAD)))
  
(define (main)
  (pcc-clearText)
  (pcc-fixConsoleSize (* (+ MAP_WIDTH 2) 2) (+ MAP_HEIGHT 5))
  (pcc-setCursorVisible 0)
  (pcc-setConsoleTitle "Snake by Hu Lang. 2017.08.09")
  
  (initSnake)
  (drawMap)
  (drawSnake)
  
  (pcc-jkGetKey))
  
(main)
;(define (loop) (display (pcc-jkGetKey)) (loop))
;(loop)
