; 定义地图的尺寸及坐标(均使用双字符长度)
(define MAP_WIDTH  24)
(define MAP_HEIGHT 16)
(define MAP_BASE_X 1)
(define MAP_BASE_Y 1)

; 定义蛇的相关参数
(define SNAKE_MIN_LEN 5)
(define SNAKE_MAX_LEN (* MAP_WIDTH MAP_HEIGHT))

; 定义地图块的状态,分别为[空格|蛇头|蛇身|食物]
(define BS_SPACE 0)
(define BS_SHEAD 1)
(define BS_SBODY 2)
(define BS_STAIL 3)
(define BS_FOOD  4)

; 定义各对象的颜色,顺序同上.
(define myColors (vector pcc-CYAN pcc-MAGENTA pcc-RED pcc-GREEN pcc-YELLOW))

; 定义各对象的形状,注意是宽字符[◆◇□●★¤~◎＋],顺序同上.
(define mySharps (vector "□" "◆" "■" "＋" "★"))

; 定义蛇的运动方向(上|下|左|右]
(define DIR_UP    1)
(define DIR_DOWN  2)
(define DIR_LEFT  3)
(define DIR_RIGHT 4)

(define point2d-new cons)
(define point2d-x car)
(define point2d-y cdr)
(define point2d-set-x! set-car!)
(define point2d-set-y! set-cdr!)

(define mySnake (make-vector SNAKE_MAX_LEN))
(define myFood (point2d-new 0 0))
(define snakeLength 0)
(define snakeDir DIR_RIGHT)
(define isFood #f)
(define isOver #f)
(define isPause #t)

(define (initSnake)
  (for i in (0 to SNAKE_MAX_LEN)
    (vector-set! mySnake i (point2d-new 0 0)))
  (set! snakeLength SNAKE_MIN_LEN)
  (for i in (0 to snakeLength)
    (point2d-set-x! (vector-ref mySnake i) (- 10 i))
    (point2d-set-y! (vector-ref mySnake i) 3)))
    
(define (drawSnake)
  (pcc-setTextColor (vector-ref myColors BS_SHEAD))
  (pcc-gotoTextPos (* (+ MAP_BASE_X (point2d-x (vector-ref mySnake 0))) 2)
                   (+ MAP_BASE_Y (point2d-y (vector-ref mySnake 0))))
  (display (vector-ref mySharps BS_SHEAD))
  
  (pcc-setTextColor (vector-ref myColors BS_SBODY))
  (for i in (1 to (- snakeLength 1))
    (pcc-gotoTextPos (* (+ MAP_BASE_X (point2d-x (vector-ref mySnake i))) 2)
                        (+ MAP_BASE_Y (point2d-y (vector-ref mySnake i))))
    (display (vector-ref mySharps BS_SBODY)))
    
  (pcc-setTextColor (vector-ref myColors BS_STAIL))
  (pcc-gotoTextPos (* (+ MAP_BASE_X (point2d-x (vector-ref mySnake (- snakeLength 1)))) 2)
                   (+ MAP_BASE_Y (point2d-y (vector-ref mySnake (- snakeLength 1)))))
  (display (vector-ref mySharps BS_STAIL)))
  
(define (drawMap)
  (pcc-setTextColor (vector-ref myColors BS_SPACE))
  (for i in (0 to MAP_HEIGHT)
    (pcc-gotoTextPos (* MAP_BASE_X 2) (+ MAP_BASE_Y i))
    (for j in (0 to MAP_WIDTH)
      (display (vector-ref mySharps BS_SPACE)))))
  
  
(define (drawBlock x y bs)
  (when (and (>= x 0) (< x MAP_WIDTH) (>= y 0) (< y MAP_HEIGHT))
    (pcc-gotoTextPos (* (+ MAP_BASE_X x) 2) (+ MAP_BASE_Y y))
    (pcc-setTextColor (vector-ref myColors bs))
    (display (vector-ref mySharps bs))))

(define (moveSnake)
  (define hx (point2d-x (vector-ref mySnake 0)))
  (define hy (point2d-y (vector-ref mySnake 0)))

  (when (>= snakeLength SNAKE_MIN_LEN)
    (cond
      ((= snakeDir DIR_UP) (set! hy (- hy 1)))
      ((= snakeDir DIR_DOWN) (set! hy (+ hy 1)))
      ((= snakeDir DIR_LEFT) (set! hx (- hx 1)))
      ((= snakeDir DIR_RIGHT) (set! hx (+ hx 1))))

      (if (or (< hx 0) (>= hx MAP_WIDTH) (< hy 0) (>= hy MAP_HEIGHT) (isInSnake hx hy))
          (set! isOver #t)
          
          (begin
            (if (and (= hx (point2d-x myFood)) (= hy (point2d-y myFood)))
                (begin
                  (set! snakeLength (+ snakeLength 1))
                  (set! isFood #f))
                (drawBlock (point2d-x (vector-ref mySnake (- snakeLength 1)))
                           (point2d-y (vector-ref mySnake (- snakeLength 1)))
                           BS_SPACE))
            (let ((i (- snakeLength 1)))
              (while (>= i 0)
                (point2d-set-x! (vector-ref mySnake (+ i 1)) (point2d-x (vector-ref mySnake i)))
                (point2d-set-y! (vector-ref mySnake (+ i 1)) (point2d-y (vector-ref mySnake i)))
                (set! i (- i 1))))
            (point2d-set-x! (vector-ref mySnake 0) hx)
            (point2d-set-y! (vector-ref mySnake 0) hy)
            (drawSnake)))))

(define (isInSnake x y)
  (define b #f)
  (for i in (0 to snakeLength)
    (if (and (= x (point2d-x (vector-ref mySnake i)))
             (= y (point2d-y (vector-ref mySnake i))))
        (set! b #t)))
  b)

(define (drawFood)
  (define x 0)
  (define y 0)
  (define (randFood)
    (set! x (remainder (rand) MAP_WIDTH))
    (set! y (remainder (rand) MAP_HEIGHT))
    (if (isInSnake x y)
      (randFood)))
  (randFood)

  (point2d-set-x! myFood x)
  (point2d-set-y! myFood y)
  (drawBlock (point2d-x myFood) (point2d-y myFood) BS_FOOD)
  (set! isFood #t))

  
(define (main)
  (pcc-clearText)
  (pcc-fixConsoleSize (* (+ MAP_WIDTH 2) 2) (+ MAP_HEIGHT 2))
  (pcc-setCursorVisible 0)
  (pcc-setConsoleTitle "Snake by Hu Lang. 2017.08.09")
  
  (initSnake)
  (drawMap)
  (drawSnake)
  
  (while (not isOver)
    (when (not isPause)
      (moveSnake)
      (if (not isFood)
        (drawFood)))

    (pcc-delayMS (- 200 (* snakeLength 2)))

    (if (pcc-jkHasKey)
      (let ((key (pcc-jkGetKey)))
        (cond
          ((= key pcc-JK_UP)
           (if (not (= snakeDir DIR_DOWN))
             (set! snakeDir DIR_UP)))
          ((= key pcc-JK_DOWN)
           (if (not (= snakeDir DIR_UP))
             (set! snakeDir DIR_DOWN)))
          ((= key pcc-JK_LEFT)
           (if (not (= snakeDir DIR_RIGHT))
             (set! snakeDir DIR_LEFT)))
          ((= key pcc-JK_RIGHT)
           (if (not (= snakeDir DIR_LEFT))
             (set! snakeDir DIR_RIGHT)))
          ((memv key (list pcc-JK_ESC pcc-JK_ENTER pcc-JK_SPACE))
           (set! isPause (not isPause)))
          (else (pcc-simpleRing 300 100))))))
            
  (pcc-gotoTextPos (+ (/ (+ MAP_BASE_X MAP_WIDTH) 2) 2) (/ (+ MAP_BASE_Y MAP_HEIGHT) 2))
  (pcc-setTextColor pcc-YELLOW)
  (display "Game Over! Score: ")
  (display (- snakeLength SNAKE_MIN_LEN))
  (pcc-jkGetKey))
  
(main)
