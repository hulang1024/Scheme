# scheme

[Scheme](https://en.wikipedia.org/wiki/Scheme_(programming_language))编程语言的一个实现。  


## 特性
目前的支持:
* 变量引用: `<variable>`
* 常量引用: `quote`,`'`  
* 过程调用: `(operator operand ...)`
* 过程: `lambda`  
* 第一级函数  
* 静态(词法)作用域
* 闭包
* 动态类型
* 传值调用
* 定义: `define`
* 赋值: `set!`
* 顺序结构: `begin`
* 条件表达式: `if`
* 递归
* 尾递归

* 标准过程
    + 相等谓词  
        `eq?`,`equal?`
    + 数值运算和数值输入/输出  
        `number?`,`=`,`<`,`<`,`>`,`<=`,`>=`,`+`,`*`,`-`,`/`,  
        `zero?`,`positive?`,`negative?`,`odd?`,`even?`,`abs`,`remainder`  
       1. 整数: `integer?`
    + 其它数据类型  
       1. 布尔  
        `boolean?`,`not`
       2. 序对和表  
        `pair?`,`null?`,`list?`,`cons`,`car`,`cdr`,`set-car!`,`set-cdr!`,  
        `caar`,`cadr` `...` `cdddar`,`cddddr` (`car`和`cdr`的组合,定义到第四层),  
        `list`,`length`,`memq`,`memv`
       3. 符号  
        `symbol?`
       4. 字符  
        `char?`
       5. 字符串  
        `string?`
    + 控制特征  
        `procedure?`,`apply`,`void`,`void?`
    + 求值  
        `eval`v
    + 输入/输出  
        `read`,`write`,`newline`,`display`
    + 系统接口  
        `load`
* 注释  
  单行注释：`;line comment`


## 用法
执行`scheme`将会开始并给你一个REPL(Read-Eval-Print-Loop)环境。  
执行一个源文件:  
`scheme -file file1.scm`