(pcc-clearText)                     ; 清空屏幕
(pcc-fixConsoleSize 50 20)          ; 设置窗口尺寸
(pcc-setTextColor pcc-LIGHT_GREEN)  ; 设置文本颜色
(pcc-gotoTextPos 10 10)             ; 光标定位
(display "Hello world!\n")          ; 输出字符串
(pcc-jkGetKey)                      ; 等待用户按键,以暂停窗口
(exit)                              ; 退出