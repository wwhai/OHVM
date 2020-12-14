# OPEN VM Design Document

## VM Specific

### RAM

16MB - 4GB rom

### Register

293 Registers.

### Stack

8(layer) \* 32(Register) deepth Stack.

## VM Model

### Basic Structure

The open vm is a virtual CPU simulator,It contains RAM,ROM,CPU,External Interface,like picture at bellow:

![](res/model.png)

### Memory Distributed

1. Program address
   Program address default is `0x0H`
2. MAIN address
   MAIN address default is `0x0H`
   You can use `START` label assign main address

```assembly
START:
    MAIN 0H
END
```

##### _Notice:_

> 0x0H is a abslute address, it's real address is 1025.

### Register Distributed

Register use `0-65536` , ` 2 bytes` address.Those address can't be accessed by user program.

| Name                 | Symbol    | Size       | Address |
| -------------------- | --------- | ---------- | ------- |
| Addressing Register  | A         | 4byte      | 1       |
| Accumulator          | ACC       | 4byte      | 2       |
| Program counter      | PC        | 4byte      | 3       |
| Stack index pointer  | SP        | 4byte      | 4       |
| Stack deepth pointer | SD        | 4byte      | 5       |
| Exception Register   | EX        | 4byte      | 6       |
| General Register     | R{0...31} | 4byte(per) | 7-38    |
| Stack Register       | S{0...7}  | 4byte(per) | 39-47   |

### Instructions Specific

Instruction have fixed header: 1 bit be splited two content,high 4bit is **Instruction Code**,low 4bit is **Function Code**,least is other segment,it maybe address or register.

| Instruction Code | Function Code | UNFIXED LENGTH segment |
| ---------------- | ------------- | ---------------------- |
| 4bit             | 4bit          | Other segment          |

### Instructions Preview

| Instruction Code | Function Code | Symbol | Function                                   | Byte Size |
| ---------------- | ------------- | ------ | ------------------------------------------ | --------- |
| F                | 0             | GOTO   | GOTO ADDress                               | 1+4       |
| F                | 1             | JNOC   | JUMP no condition                          | 1+4       |
| F                | 2             | CALL   | CALL SUB process                           | 1+4       |
| F                | 3             | BACK   | RETURN from SUB process                    | 1+4       |
| E                | 0             | CMRAE  | COMPARE R{x} if equal Acc                  | 1+1       |
| E                | 1             | CMRAG  | COMPARE R{x} if greater than Acc           | 1+1       |
| E                | 2             | CMRAL  | COMPARE R{x} if little than Acc            | 1+1       |
| E                | 3             | CMRRE  | COMPARE R{x} if equal R{x}                 | 1+2       |
| E                | 4             | CMRRG  | COMPARE R{x} if greater than R{x}          | 1+2       |
| E                | 5             | CMRRL  | COMPARE R{x} if little than R{x}           | 1+2       |
| E                | 6             | CMASE  | COMPARE Acc if equal than Stack            | 1+2       |
| E                | 7             | CMASG  | COMPARE Acc if greater than Stack          | 1+2       |
| E                | 8             | CMASL  | COMPARE Acc if little than Stack           | 1+2       |
| E                | 9             | CMRSE  | COMPARE R{x} if little than Stack          | 1+3       |
| E                | A             | CMRSG  | COMPARE R{x} if greater than Stack         | 1+3       |
| E                | B             | CMRSL  | COMPARE R{x} if little than Stack          | 1+3       |
| D                | 0             | INCA   | INCREASE Acc                               | 1         |
| D                | 1             | DECA   | DECREASE Acc                               | 1         |
| D                | 2             | INCR   | INCREASE Register{x}                       | 1+1       |
| D                | 3             | DECR   | DECREASE Register{x}                       | 1+1       |
| D                | 4             | ADDAR  | ADD Acc and Register{x}                    | 1+1       |
| D                | 5             | SUBAR  | SUB Acc and Register{x}                    | 1+1       |
| D                | 6             | INCS   | INCREASE Stack                             | 1+2       |
| D                | 7             | DECS   | DECREASE Stack                             | 1+2       |
| D                | 8             | ADDAS  | ADD Acc and Stack                          | 1+2       |
| D                | 9             | SUBAS  | SUB Acc and Stack                          | 1+2       |
| C                | 0             | ANDR   | Acc AND Register{x}                        | 1+1       |
| C                | 1             | AOR    | Acc OR Register{x}                         | 1+1       |
| C                | 2             | AXR    | Acc XOR Register{x}                        | 1+1       |
| C                | 3             | BSLR   | bit shift left in Register{x}              | 1+1       |
| C                | 4             | BSRR   | bit shift right in Register{x}             | 1+1       |
| C                | 5             | BSLLR  | bit shift left loop in Register{x}         | 1+1       |
| C                | 6             | BSRLR  | bit shift right loop in Register{x}        | 1+1       |
| C                | 7             | ANDS   | Acc AND Stack                              | 1+2       |
| C                | 8             | AOS    | Acc OR Stack                               | 1+2       |
| C                | 9             | AXS    | Acc XOR Stack                              | 1+2       |
| C                | A             | BSLS   | bit shift left in Stack                    | 1+2       |
| C                | B             | BSRS   | bit shift right in Stack                   | 1+2       |
| C                | C             | BSLLS  | bit shift left loop in Stack               | 1+2       |
| C                | D             | BSRLS  | bit shift right loop in Stack              | 1+2       |
| B                | 0             | IMA    | Immediately value to Acc                   | 1+4       |
| B                | 1             | IMR    | Immediately value to Register              | 1+5       |
| B                | 2             | IMS    | Immediately value to Stack                 | 1+6       |
| B                | 3             | GET    | Get value from address                     | 1+4       |
| B                | 4             | MVRR   | MOVE Register value to another Register{x} | 1+2       |
| B                | 5             | MVRS   | MOVE Register value to Stack               | 1+2       |
| B                | 6             | MVSR   | MOVE Stack value to Register               |           |
| B                | 7             | MVAR   | MOVE Acc value to Register                 |           |
| A                | 0             | INTK   | wait key interupt                          |           |
| A                | 1             | KEY    | get key                                    |           |
| A                | 2             | PLY    | play sound                                 |           |
| A                | 3             | PLYS   | play a series of sound                     |           |
| A                | 4             | DPXY   | draw 8 \* x pixels at (x, y)               |           |
| A                | 5             | SCRU   | Screen scrool up x pixel                   |           |
| A                | 6             | SCRD   | Screen scrool down x pixel                 |           |
| A                | 7             | SCRL   | Screen scrool left x pixel                 |           |
| A                | 8             | SCRR   | Screen scrool right x pixel                |           |
| A                | 9             | DXY    | draw point at (x, y)                       |           |
| A                | A             | DCXY   | draw char at (x, y)                        |           |
| 5                | 0             | TIMER  | timer                                      |           |
| 5                | 1             | CLS    | clear screen                               |           |
| 5                | 2             | RSTR   | reset all Register                         |           |
| 5                | 3             | START  | Start vm                                   |           |

##### _Notice:_

> All instruction return value is store in passtive side. Such `MOVSR {Rx}` means move stack value to register ,passtive side is register.But in `INCA` means Acc increase 1 and return value to Acc.

## Instructions Set

### F0 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### F1 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### F2 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### F3 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### E1 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### E2 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### E3 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### E4 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### E5 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### E6 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### E7 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### E8 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### E9 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### EA : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### EB : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### D0 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### D1 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### D2 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### D3 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### D4 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### D5 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### D6 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### D7 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### D8 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### D9 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### CA : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### CB : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### CC : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### CD : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### B0 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### B1 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### B2 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### B3 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### B4 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### B5 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### A0 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### A1 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### A2 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### A3 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### A4 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### A5 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### A6 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### A7 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### A8 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### A9 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### AA : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### 50 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### 51 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

### 52 : GOTO

#### Define

| Instruction Code | Function Code | Symbol | Size | Description |
| ---------------- | ------------- | ------ | ---- | ----------- |
| F                | 0             | GOTO   | 4    | description |

#### Example

- HEX:

  ```assembly
  F00xFFFF
  ```

- ASM:

  ```assembly
  GOTO 0xFFF
  ```

##### _Notice:_

> some notice....

## VM Program Specific

### Reserved Keywords

- START: used for start vm
- MAIN: main like c
- END: sub process end label
- STOP: stop vm

##### _Notice:_

> Can't use those words in user program!

### Program text format

```assembly
START:
    MAIN 0H
END

MAIN:
    ;;
END

SUB1:
   ;;
END

SUB2:
   ;;
END
```

## Program Example

```assembly
;; Example program
START:
    MAIN 00H ;; program start at 0H
END
;; main
MAIN:
    IA 0              ;; ACC = 0
    IR R0 #1          ;; R0 = 1
    ADDAR R0          ;; ACC = ACC + R0
    CALL DISPLAY      ;; Call display
    STOP              ;;
END
;; display sub process
DISPLAY:
    DCXY 40, 50, ACC ;; Display value in ACC
END

```

## VM Byte Code Specific

Byte Code header contains 4byte file type,and 3byte version,4byte address:

| Type |     |     |     | Version |     |     | Start Address |     |     |     |
| ---- | --- | --- | --- | ------- | --- | --- | ------------- | --- | --- | --- |
| V    | M   | B   | C   | 0       | 0   | 1   | 0             | 0   | 0   | 0   |
