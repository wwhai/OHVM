# OHVM 虚拟机设计规范
## 简介
这是我为了学习虚拟机原理和计算机程序运行原理，写的一个很简单的虚拟机demo，可以运行简单的字节码，当然如果我有时间和精力写出来相对完整点的版本，说不定还能搞个小玩具。
## 基础规范

### 内存范围
默认内存设定大小为 0-4KB，可以调整参数来扩大。

### 寄存器分布

一共有 293 个个寄存器.

### 栈
一共有8层栈空间，每一层有32个寄存器。

## 虚拟机模型

![](res/model.png)

### 内存分布

1. 程序地址：
程序从 0x0000H 开始。
2. 入口地址：
程序入口地址默认是 0x0000H。

##### 注意

> **_0x0H_** 是一个绝对地址, 真实地址是：**_1025_**.

### 寄存器分布

Those address can't be accessed by user program.

| Name                 | Symbol    | Size       | Address(Decimal) |
| -------------------- | --------- | ---------- | ---------------- |
| NullPtr Register     | NULL      | 1byte      | ROM: 0 - 1       |
| VM Flag Register     | FLAG      | 1byte      | ROM: 1 - 2       |
| Addressing Register  | A         | 4byte      | ROM: 3 - 5       |
| ACCumulator          | ACC       | 4byte      | ROM: 6 - 9       |
| Program counter      | PC        | 4byte      | ROM: 10 - 13     |
| Stack index pointer  | SP        | 4byte      | ROM: 14 - 17     |
| Stack deepth pointer | SD        | 4byte      | ROM: 18 - 21     |
| Exception Register   | EXCPT     | 4byte      | ROM: 22 - 25     |
| General Register     | R{0...31} | 4byte(per) | ROM: 25 - 152    |
| Stack Register       | S{0...7}  | 4byte(per) | ROM: 153 - 378   |

### 标志位寄存器

| Name      | Symbol | Size |
| --------- | ------ | ---- |
| Zero      | ZE     | 1bit |
| Equal     | EQ     | 1bit |
| Execption | EX     | 1bit |
| Reserved  | --     | 1bit |
| Reserved  | --     | 1bit |
| Reserved  | --     | 1bit |
| Reserved  | --     | 1bit |
| Reserved  | --     | 1bit |


## 指令集

| Instruction Code | Symbol                | Example                               | Function                                             |
| ---------------- | --------------------- | ------------------------------------- | ---------------------------------------------------- |
| 0X0000           | NOP                   | NOP                                   | No operation, used for delay                         |
| 0X0001           | START                 | START                                 | Start ohvm                                           |
| 0X0002           | STOP                  | STOP                                  | Stop ohvm                                            |
| 0X0003           | RSTR                  | RSTR                                  | Reset all Register                                   |
| 0X0004           | TIMER [Rn]            | MOVR R0 #5<br>TIMER R0                | Start Timer                                          |
| 0X0005           | GOTO [Rn]/[Label]     | GOTO LOOP<br>GOTO R0                  | GOTO ADDress                                         |
| 0X0006           | CALL [Rn]/[Label]     | CALL LOOP<br>CALL R0                  | CALL SUB process                                     |
| 0X0007           | BACK                  | BACK                                  | RETURN from SUB process                              |
| 0X0008           | JMP [Rn]/[Label]      | JMP LOOP                              | JUMP Address With No Condition                       |
| 0X0009           | JZ [ACC] [Rn]/[Label] | JZ LOOP                               | Compare with ACC, Jump Address if Zero Flag == 1     |
| 0X000A           | JE [ACC] [Rn]/[Label] | JE LOOP                               | Compare with ACC, Jump Address if Equal Flag == 1    |
| 0X000B           | JX [ACC] [Rn]/[Label] | JX LOOP                               | Compare with ACC, Jump Address if Excepion Flag == 1 |
| 0X000C           | CMRAE [Rn]            | MOV R0 #1<br>CMRAE                    | COMPARE R{x} if equal ACC                            |
| 0X000D           | CMRAG [Rn]            | MOV R0 #1<br>CMRAG                    | COMPARE R{x} if greater than ACC                     |
| 0X000E           | CMRAL [Rn]            | MOV R0 #1<br>CMRAL                    | COMPARE R{x} if little than ACC                      |
| 0X000F           | CMRRE [Rn] [Rn]       | MOV R0 #1<br>MOV R1 #1<br>CMRRE R0 R1 | COMPARE R{x} if equal R{x}                           |
| 0X0010           | CMRRG [Rn] [Rn]       | MOV R0 #1<br>MOV R1 #1<br>CMRRG R0 R1 | COMPARE R{x} if greater than R{x}                    |
| 0X0011           | CMRRL [Rn] [Rn]       | MOV R0 #1<br>MOV R1 #1<br>CMRRL R0 R1 | COMPARE R{x} if little than R{x}                     |
| 0X0012           | CMASE [Rn] [Sn]       | CMASE R0 S0                           | COMPARE ACC if equal than Stack                      |
| 0X0013           | CMASG [Rn] [Sn]       | CMASG R0 S0                           | COMPARE ACC if greater than Stack                    |
| 0X0014           | CMASL [Rn] [Sn]       | CMASL R0 S0                           | COMPARE ACC if little than Stack                     |
| 0X0015           | CMRSE [Rn] [Sn]       | CMRSE R0 S0                           | COMPARE R{x} if little than Stack                    |
| 0X0016           | CMRSG [Rn] [Sn]       | CMRSG R0 S0                           | COMPARE R{x} if greater than Stack                   |
| 0X0017           | CMRSL [Rn] [Sn]       | CMRSL R0 S0                           | COMPARE R{x} if little than Stack                    |
| 0X0018           | INCA                  | INCA                                  | INCREASE ACC                                         |
| 0X0019           | DECA                  | DECA                                  | DECREASE ACC                                         |
| 0X001A           | INCR [Rn]             | INCR R0                               | INCREASE Register{x}                                 |
| 0X001B           | DECR [Rn]             | DECR R0                               | DECREASE Register{x}                                 |
| 0X001C           | ADDAR [Rn]            | MOV R0 #1<br>ADDAR R0                 | ADD ACC and Register{x}                              |
| 0X001D           | SUBAR [Rn]            | MOV R0 #1<br>SUBAR R0                 | SUB ACC and Register{x}                              |
| 0X001E           | INCS                  | INCS                                  | INCREASE Stack                                       |
| 0X001F           | DECS                  | DECS                                  | DECREASE Stack                                       |
| 0X0020           | ADDAS [Sn]            | ADDAS S0                              | ADD ACC and Stack                                    |
| 0X0021           | SUBAS [Sn]            | SUBAS S0                              | SUB ACC and Stack                                    |
| 0X0022           | ANDR [Rn]             | MOV R0 #1<br>ANDR R0                  | ACC AND Register{x}                                  |
| 0X0023           | AOR [Rn]              | MOV R0 #1<br>AOR R0                   | ACC OR Register{x}                                   |
| 0X0024           | AXR [Rn]              | MOV R0 #1<br>AXR R0                   | ACC XOR Register{x}                                  |
| 0X0025           | BSLR [Rn]             | MOV R0 #1<br>BSLR R0                  | BIT Shift left in Register{x}                        |
| 0X0026           | BSRR [Rn]             | MOV R0 #1<br>BSRR R0                  | BIT Shift right in Register{x}                       |
| 0X0027           | BSLLR [Rn]            | MOV R0 #1<br>BSLLR R0                 | BIT Shift left loop in Register{x}                   |
| 0X0028           | BSRLR [Rn]            | MOV R0 #1<br>BSRLR R0                 | BIT Shift right loop in Register{x}                  |
| 0X0029           | ANDS [Sn]             | ANDS S0                               | ACC AND Stack                                        |
| 0X002A           | AOS [Sn]              | AOS S0                                | ACC OR Stack                                         |
| 0X002B           | AXS [Sn]              | AXS S0                                | ACC XOR Stack                                        |
| 0X002C           | BSLS [Sn]             | BSLS S0                               | BIT Shift left in Stack                              |
| 0X002D           | BSRS [Sn]             | BSRS S0                               | BIT Shift right in Stack                             |
| 0X002E           | BSLLS [Sn]            | BSLLS S0                              | BIT Shift left loop in Stack                         |
| 0X002F           | BSRLS [Sn]            | BSRLS S0                              | BIT Shift right loop in Stack                        |
| 0X0030           | IMA #[Hex]            | IMA #1                                | Immediately value to ACC                             |
| 0X0031           | IMR [Rn] #[Hex]       | IMR R0 #1                             | Immediately value to Register                        |
| 0X0032           | IMS [Sn] #[Hex]       | IMS S1 #1                             | Immediately value to Stack                           |
| 0X0033           | GET [Rn]              | GET R1                                | Get value from address                               |
| 0X0034           | MVRR [Rn] [Rn]        | MVRR R0 R0                            | MOVE Register value to another Register{x}           |
| 0X0035           | MVRS [Rn] [Sn]        | MVRS R0 S0                            | MOVE Register value to Stack                         |
| 0X0036           | MVSR [Rn] [Rn]        | MVRS S0 R0                            | MOVE Stack value to Register                         |
| 0X0037           | MVAR [Rn]             | MVAR R0                               | MOVE ACC value to Register                           |
| 0X0038           | INTK                  | INTK                                  | Wait key interupt                                    |
| 0X0039           | KEY                   | KEY                                   | Get key                                              |
| 0X003A           | PLY [Rn]              | MOV R0 #1<br>PLY R0                   | Play dididi sound                                    |
| 0X003B           | PLYS [Rn]             | MOV R0 #1<br>PLYS R0                  | Play a series of sound.Usually have a frequence      |
| 0X003C           | GSET [Rn]             | MOV R0 #1<br>GSET R0                  | Graphics resolution setting, [64X32 - 640X320]       |
| 0X003D           | CLS                   | CLS                                   | Clear screen                                         |
| 0X003E           | DPXY [Rn]             | MOV R0 #1<br>DPXY R0                  | Draw 8 \* x pixels at (x, y)                         |
| 0X003F           | DXY [Rn]              | MOV R0 #1<br>DXY R0                   | Draw point at (x, y)                                 |
| 0X0040           | DCXY [Rn]             | MOV R0 #1<br>DCXY R0                  | Draw char at (x, y)                                  |
| 0X0041           | DLINE [Rn]            | MOV R0 #1<br>DCXY R0                  | Draw line                                            |
| 0X0042           | DRECT [Rn]            | MOV R0 #1<br>DCXY R0                  | Draw rectangle                                       |
| 0X0043           | SCRU                  | SCRU                                  | Screen scrool up x pixel                             |
| 0X0044           | SCRD                  | SCRD                                  | Screen scrool down x pixel                           |
| 0X0045           | SCRL                  | SCRL                                  | Screen scrool left x pixel                           |
| 0X0046           | SCRR                  | SCRR                                  | Screen scrool right x pixel                          |
| 0X0047           | SET [Rn]              | SET R0                                | Set value to ram                                     |



## 汇编规范

### 关键字

- **_MAIN_**: 入口标识符
- **_STOP_**: 直接退出虚拟机
- **_BEGIN_**: 子程序声明标识符
- **_END_**: 子程序标签

##### _注意:_

> <div style="color:red">关键字不可用于其他地方!</div>

### 汇编格式
1. 入口
   用 `MAIN` 标记入口，一个程序必须有一个 `MAIN`。
2. 子程序
   用 `BEGIN {子程序名}: [statement] END` 来声明一个子程序，例如声明一个输出字符的子程序：
```assembly

BEGIN OUTPUT_CHAR:
   ;;
END

```
3. 退出
   退出用 `STOP` 标签标记，程序只要到 `STOP`，立即结束所有运行。
```assembly
STOP
```
4. 注释
   使用双分号注释: `;;`
```
    ;;这是一行注释
```

## 汇编案例

```assembly
;; Example program
;; main
MAIN:
    IA 0              ;; ACC = 0
    IR R0 #1          ;; R0 = 1
    ADDAR R0          ;; ACC = ACC + R0
    CALL DISPLAY      ;; Call display
    STOP              ;;
END
;; display sub process
BEGIN DISPLAY:
    DCXY 40, 50, ACC ;; Display value in ACC
END
```

## 异常处理

按理来说汇编不应该有异常处理这种设计的，但是我想了下异常处理的本质也是一个 `JMP`,于是我自作聪明的设计了这个语法：`TRY ... CATCH`.

```
TRY
    ;; ....
    ;; ....
    ;; ....
CATCH ${Address}
```

捕获异常时，将设置 `EX` 寄存器设置为1，并且跳转到 `CATCH` 指定的代码位置处。

## 字节码规范
如图：

![](res/bc-format.png)

## 编译器
我现在技术不到家，写不出来高级语言编译器，本来打算设计一个简单的编译器，把 C 或者是 BASIC 这种基础语法编译成字节码，但是我水平太菜了，搞不定，于是这里只能手动来写字节码了。

### 如何编译
编译流程：
```
┌───────────────────────┐
│    BASIC Source code  │
│                       │
└──────────┬────────────┘
           │
           ▼
┌───────────────────────┐
│   OHVM ASM code       │
│                       │
└──────────┬────────────┘
           │
┌──────────▼────────────┐
│                       │
│   OHVM Byte code      │
└───────────────────────┘
```
假设编译器已经实现好了，应该按照下面的方式来使用：
```shell
# 要编译这个文件：hello_world.oohvms
oohvmcc hello_world.oohvms
# 编译完成后，应该有下面这几个文件：
# hello_world.oohvmasm hello_world.oohvmbc
```
> oohvms 为源代码文件；oohvmasm 为汇编代码；oohvmbc 为字节码文件
## 任务规划

- [x] 基础指令集设计
- [ ] 项目结构设计
- [ ] 虚拟机内核设计
- [ ] 汇编器设计
- [ ] 高级程序语言设计
- [ ] 程序编译器设计

## 资源链接

- Blog: https://openlab.ezlinker.cn
- Mail: cnwwhai@gmail.com

## 参考

- 《计算机程序运行原理》

- 《C语言核心技术》

- 《Linux内核开发》
- YACC，LEX 等基础官方文档（实现编译器相关）