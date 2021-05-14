## 关于 OHVM-Lang
这是我自己的另一个想法，前段时间写了个很简单的虚拟机（其实就是OHVM），但是没有一个高级语言作为APP开发工具，于是我绞尽脑汁在找一些脚本语言。其他的比如Lua，JS等都是成熟的语言，语法规则实在是太庞大了，我看到那些规范几乎亮眼黑，一点不利于新手学习。于是我找到了一个大佬写的博客，他这个是自己设计一个很简单的编程语言，很适合作为新手学习，于是我就给抄过来了。本项目主要目的是把高级程序 OHVM-Lang 编译成 OHVM 的字节码。

## 其他
本人不懂编译技术，全靠谷歌，写出来的东西估计很垃圾，但是学习价值还是有。
## OHVM-Lang 设计规范
### 变量
变量赋值使用符号 `:=` , 例如我要定义一个变量存储一个数，程序应该如下写：
```BASIC
var0 := 1
var1 := 2
....

```
* 变量赋值不支持多行，一行一个，和 C 有区别！

> 也许你会好奇为何变量预定义？其实这是上古时代的编程语言通用的做法，因为本设计参考了 BASIC，所以也继承了这种非常简单的变量形式,这个变量的具体实现形式是个数组:

### 注释

OHVM-Lang 使用 `##`注释，和 python，shell 不太一样，注意是两个`#`。

### 运算符
- 数学运算符

  | 运算符 | 含义       | 示例                                        |
  | ------ | ---------- | ------------------------------------------- |
  | +      | 数学加法   | var0 := 1 + 1                               |
  | -      | 数学减法   | var0 := 1 - 1                               |
  | *      | 数学乘法   | var0 := 1 * 1                               |
  | ^      | 数学幂运算 | var0 := 1 ^ 1                               |
  | /      | 数学整除法 | var0 := 1 / 1                               |
  | %      | 数学模运算 | var0 := 1 % 1                               |
  | <      | 数学大于   | var0 := 1 > 1 ,## 逻辑运算返回 0，表示false |
  | >      | 数学小于   | var0 := 1 < 1 ,## 逻辑运算返回 0，表示false |
  | =      | 数学等于   | var0 := 1 = 1 ,## 逻辑运算返回 1，表示true  |
  | >:     | IO输出     | >: 1 + 1                                    |
  | <<     | IO输入     | :> var0                                     |

- 逻辑运算符
  | 运算符 | 含义 | 示例           |
  | ------ | ---- | -------------- |
  | &      | AND  | var0 := 1 & 1  |
  | \|     | OR   | var0 := 1 \| 1 |
  | !      | NOT  | var0 := 1 ! 1  |
  | ^      | XOR  | var0 := 1 ^ 1  |

- 字符

  字符用` " `起来的串表示，如下示例:

  ```basic
  var0 := "hello world"
  >: var0
  ```

  其中 `$`前置的单个字符值会被直接转成 ASCII 码:

  ```basic
  var0 := $A
  var1 := $1
  ```

  其中 var0 的值为 A 的 ASCII 码为 65；var1 的值为 49。
### 条件

条件语句的格式如下:

```c
if(boolean){
  ##
}
else{
  ##
}
```

看起来是不是很简单，所以为了省事，目前只有这一种条件语句，来个 demo：

```c
var0 := 1
var1 := 2
if(var0 > var1){
    >: var0
}
else{
    >: var1
}
```


### 循环

循环语句的格式如下：

```javascript
loop(boolean){
    ##
    goout
}
```

- 跳出循环使用 `goout` , 这其实是汇编的 `POP` 命令。当到这里的时候，发生了退栈的行为。

### 函数
函数概念和任意一个其他编程语言的相同，其定义如下：
```
fun [名称](参数表) ->
   函数体
end
```

例如定义一个计算两数之和的demo：
- 有返回值
```
fun add(a, b) ->
    return a + b
end
```
- 无返回值：
```
fun echo(a) ->
    >: a
end
```

### 简单案例

1. hello world

   ```
   var0 := "hello ohvm"
   >: var0
   ```
2. 计算两个数之和
   ```
   var0 := 1
   var1 := 2
   >: var0 + var1
   ```
3. 比较两个数大小
   ```
   var0 := 1
   var1 := 2
   if(var0 > var1){
       >: 1
   }else{
       >: 0
   }
   ```

### 其他

#### 内置函数

desining

#### 绘图函数

desining

## 参考
- [Turing Complete Languages] [https://www.cs.drexel.edu/~kschmidt/CS360/Lectures/1.html]
- [ Writing an Interpreter ] [http://memphis.compilertools.net/interpreter.html]
- [ BASIC Interpreter ] [http://zzp.me/archive/basic%E8%A7%A3%E9%87%8A%E5%99%A8.html]
- [ C8C ] [https://github.com/glouw/c8c.git]