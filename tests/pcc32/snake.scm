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
(define myColors [vector pcc-CYAN pcc-MAGENTA pcc-RED pcc-GREEN pcc-YELLOW])

; 定义各对象的形状,注意是宽字符[◆◇□●★¤~◎＋],顺序同上.
(define mySharps [vector "□" "◆" "■" "＋" "★"])

; 定义蛇的运动方向[上|下|左|右]
(define DIR_UP    1)
(define DIR_DOWN  2)
(define DIR_LEFT  3)
(define DIR_RIGHT 4)

(define point2d-new cons)
(define point2d-x car)
(define point2d-y cdr)
(define point2d-set-x! set-car!)
(define point2d-set-y! set-cdr!)

(define mySnake (make-vector (* MAP_WIDTH MAP_HEIGHT) (point2d-new 0 0)))
(define myFood (point2d-new 0 0))
(define snakeLength 0)
(define snakeDir DIR_RIGHT)
(define isFood 0)
(define isOver 0)

(define (initSnake)
  (set! snakeLength SNAKE_MIN_LEN)
  (for i in (0 to snakeLength)
    (display (vector-ref mySnake i))
    (point2d-set-x! [vector-ref mySnake i] (- 10 i))
    (point2d-set-y! [vector-ref mySnake i] 3))
    (display (vector-ref mySnake 0))
    (display (vector-ref mySnake 1))
)
    
(define (drawSnake)
  (pcc-setTextColor [vector-ref myColors BS_SHEAD])
  (pcc-gotoTextPos (* (+ MAP_BASE_X (point2d-x [vector-ref mySnake 0])) 2)
                   (+ MAP_BASE_Y (point2d-y [vector-ref mySnake 0])))
  (display (vector-ref mySharps BS_SHEAD))
  (pcc-setTextColor (vector-ref myColors BS_SBODY))
  (for i in (0 to (- snakeLength 1))
    ;(pcc-gotoTextPos (* (+ MAP_BASE_X (point2d-x [vector-ref mySnake i])) 2)
     ;                (+ MAP_BASE_Y (point2d-y [vector-ref mySnake i])))
    ;(display (vector-ref mySharps BS_SBODY))
    )
  #|
  (pcc-setTextColor (vector-ref myColors BS_STAIL))
  (pcc-gotoTextPos (* (+ MAP_BASE_X (point2d-x [vector-ref mySnake (- snakeLength 1)])) 2)
                   (+ MAP_BASE_Y (point2d-y [vector-ref mySnake (- snakeLength 1)])))
  (display (vector-ref mySharps BS_STAIL))|#
  )
  
(define (drawMap)
  (pcc-setTextColor [vector-ref myColors BS_SPACE])
  (for i in (0 to MAP_HEIGHT)
    (pcc-gotoTextPos (* MAP_BASE_X 2) (+ MAP_BASE_Y i))
    (for j in (0 to MAP_WIDTH)
      (display [vector-ref mySharps BS_SPACE]))))
  
  
(define (main)
  (pcc-clearText)
  (pcc-fixConsoleSize (* (+ MAP_WIDTH 2) 2) (+ MAP_HEIGHT 5))
  (pcc-setCursorVisible 0)
  (pcc-setConsoleTitle "Snake by Hu Lang. 2017.08.09")
  
  (initSnake)
  ;(drawMap)
  ;(drawSnake)
    (display (vector-ref mySnake 0))
    (display (vector-ref mySnake 1))
  (pcc-jkGetKey))
  
(main)
;(define (loop) (display (pcc-jkGetKey)) (loop))
;(loop)
