# OPEN VM Design Document

## VM Specific
### RAM

16MB - 4GB rom 

### ROM

Min 256MB

### Register

293 Registers.

### Stack

8(layer)*32(Register) deepth Stack.

## VM Model

![](res/model.png)



### Register

|Name         | Symbol   |  Description  |
|  ----  | ----  | ----|
|1.Addressing Register       | A      |8bit|
|2.Accumulator       | ACC     |32bit|
|3.Program counter    | PC      |32bit|
|4.Stack index pointer  | SP      |8bit |
|5.Stack deepth pointer  | SD      |8bit |
|6.General Register    | R{0...31}   |32 * 32bit|
|7.Stack Register      | S{0...7}    |8 * 32bit|
|8.Exception Register | EX |8bit|
>  Total count：6 + 32 + 8 * 32 = 294


### Instructions Specific
- 4bit Instruction Code
- 4bit Function Code
- UNFIXED LENGTH segment

### Instructions Preview Table
| Instruction Code | Function Code | Symbol   |  Function  | Byte Size |
|  ----  | ----  | ------- | ------------ |  ----  |
| F      |   0   | GOTO    | GOTO ADDress |  |
| F      |   1   | JNOC    | JUMP no condition |  |
| F      |   2   | CALL    | CALL SUB process |  |
| F      |   3   | BACK    | RETURN from SUB process |  |
| E      |   0   | CMRAE | COMPARE R{x} if equal Acc |  |
| E      |   1   | CMRAG | COMPARE R{x} if greater than Acc |  |
| E      |   2   | CMRAL | COMPARE R{x} if little than Acc |  |
| E      |   3   | CMRRE | COMPARE R{x} if equal R{x} |  |
| E      |   4   | CMRRG | COMPARE R{x} if greater than R{x}          |  |
| E      |   5   | CMRRL | COMPARE R{x} if little than R{x}           |  |
| E      |   6   | CMASE | COMPARE Acc if equal than Stack |  |
| E      |   7   | CMASG | COMPARE Acc if greater than Stack |  |
| E      |   8   | CMASL | COMPARE Acc if little than Stack |  |
| E      |   9   | CMRSE | COMPARE R{x} if little than Stack |  |
| E      |   A   | CMRSG | COMPARE R{x} if greater than Stack |  |
| E      |   B   | CMRSL | COMPARE R{x} if little than Stack |  |
| D      |   0   | INCA | INCREASE Acc |  |
| D      |   1   | DECA | DECREASE Acc |  |
| D      |   2   | INCR | INCREASE Register{x} |  |
| D      |   3   | DECR | DECREASE Register{x} |  |
| D      |   4   | ADDAR | ADD Acc and Register{x} |  |
| D      |   5   | SUBAR | SUB Acc and Register{x} |  |
| D      |   6   | INCS | INCREASE Stack |  |
| D      |   7   | DECS | DECREASE Stack |  |
| D      |   8   | ADDAS | ADD Acc and Stack |  |
| D      |   9   | SUBAS | SUB Acc and Stack |  |
| C | 0 | ANDR | Acc AND Register{x} |  |
| C | 1 | AOR | Acc OR Register{x} |  |
| C | 2 | AXR | Acc XOR Register{x} |  |
| C | 3 | BSLR | bit shift left in Register{x} |  |
| C | 4 | BSRR | bit shift right in Register{x} |  |
| C | 5 | BSLLR | bit shift left loop in Register{x} |  |
| C | 6 | BSRLR | bit shift right loop in Register{x} |  |
| C | 7 | ANDS | Acc AND Stack |  |
| C | 8 | AOS | Acc OR Stack |  |
| C | 9 | AXS | Acc XOR Stack |  |
| C | A | BSLS | bit shift left in Stack |  |
| C | B | BSRS | bit shift right in Stack |  |
| C | C | BSLLS | bit shift left loop in Stack |  |
| C | D | BSRLS | bit shift right loop in Stack |  |
| B | 0 | # | immediately value to Acc |  |
| B | 1 | $ | immediately value to Register |  |
| B | 2 | % | immediately value to Stack |  |
| B | 3 | MVRR | MOVE Register value to another Register{x} |  |
| B | 4 | MVRS | MOVE Register value to Stack |  |
| B | 5 | MVSR | MOVE Stack value to Register |  |
| A | 0 | INTK | wait key interupt |  |
| A | 1 | KEY | get key |  |
| A | 2 | PLY | play sound |  |
| A | 3 | PLYS | play a series of sound |  |
| A | 4 | DPXY | draw 8 * x pixels at (x, y) |  |
| A | 5 | SCRU | screen scrool up x pixel |  |
| A | 6 | SCRD | screen scrool down x pixel |  |
| A | 7 | SCRL | screen scrool left x pixel |  |
| A | 8 | SCRR | screen scrool right x pixel |  |
| A | 9 | DXY | draw pointer at (x, y) |  |
| A | A | DCXY | draw char at (x, y) |  |
| 5 | 0 | TIMER | timer |  |
| 5 | 1 | CLS | clear screen |  |
| 5 | 2 | RSTR | reset all Register |  |

## Instructions Set
### F0 : GOTO
| Instruction Code | Function Code | Symbol   |  Function  |
|  ----  | ----  | ---- | ----|
| F      |   0   | GOTO | goto address |

#### Example
Hex:
```
F00xFFFF
```
ASM:
```
GOTO 0xFFF
```
