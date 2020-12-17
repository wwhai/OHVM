# OPEN VM Design Document

## VM Specific

### RAM

**_4KB_** - **_4GB_**

### Register

**_293_** Registers.

### Stack

**_8(layer)_** , **_32(Register)_** deepth Stack.

## VM Model

### Basic Structure

The open vm is a virtual CPU simulator,It contains RAM,ROM,CPU,External Interface,like picture at bellow:

![](res/model.png)

### Memory Distributed

1. Program address
   Program address default is **_0x0H_**
2. MAIN address
   MAIN address default is **_0x0H_**,You can use **_START_** label assign main address

```assembly
START:
    MAIN 0H
END
```

##### _Notice:_

> **_0x0H_** is a abslute address, it's real address is **_1025_**.

### Register Distributed

Those address can't be accessed by user program.

| Name                 | Symbol    | Size       | Address |
| -------------------- | --------- | ---------- | ------- |
| Addressing Register  | A         | 4byte      | 1       |
| ACCumulator          | ACC       | 4byte      | 2       |
| Program counter      | PC        | 4byte      | 3       |
| Stack index pointer  | SP        | 4byte      | 4       |
| Stack deepth pointer | SD        | 4byte      | 5       |
| Exception Register   | EX        | 4byte      | 6       |
| General Register     | R{0...31} | 4byte(per) | 7-38    |
| Stack Register       | S{0...7}  | 4byte(per) | 39-47   |

### Instructions Specific

Instruction have fixed header: 1 BIT be splited two content,high 4BIT is **Instruction Code**,low 4BIT is **Function Code**,least is other segment,it maybe address or register.

| Instruction Code | UNFIXED LENGTH segment |
| ---------------- | ---------------------- |
| 1 Byte           | Other segment          |

### Instructions Preview

| Instruction Code | Symbol                      | Example                               | Function                                        |
| ---------------- | --------------------------- | ------------------------------------- | ----------------------------------------------- |
| 0x0000           | NOP                         | NOP                                   | No operation, used for delay                    |
| 0x0001           | START                       | START                                 | Start vm                                        |
| 0x0002           | STOP                        | STOP                                  | Stop vm                                         |
| 0x0003           | RSTR                        | RSTR                                  | Reset all Register                              |
| 0x0004           | TIMER [Register]            | MOVR R0 #5<br>TIMER R0                | Start Timer                                     |
| 0x0005           | GOTO [Register]/[Label]     | GOTO LOOP<br>GOTO R0                  | GOTO ADDress                                    |
| 0x0006           | JNOC [Register]/[Label]     | JNOC LOOP<br>JNOC R0                  | JUMP no condition                               |
| 0x0007           | CALL [Register]/[Label]     | CALL LOOP<br>CALL R0                  | CALL SUB process                                |
| 0x0008           | BACK                        |                                       | RETURN from SUB process                         |
| 0x0009           | CMRAE [Register]            | MOV R0 #1<br>CMRAE                    | COMPARE R{x} if equal ACC                       |
| 0x000A           | CMRAG [Register]            | MOV R0 #1<br>CMRAG                    | COMPARE R{x} if greater than ACC                |
| 0x000B           | CMRAL [Register]            | MOV R0 #1<br>CMRAL                    | COMPARE R{x} if little than ACC                 |
| 0x000C           | CMRRE [Register] [Register] | MOV R0 #1<br>MOV R1 #1<br>CMRRE R0 R1 | COMPARE R{x} if equal R{x}                      |
| 0x000D           | CMRRG [Register] [Register] | MOV R0 #1<br>MOV R1 #1<br>CMRRG R0 R1 | COMPARE R{x} if greater than R{x}               |
| 0x000E           | CMRRL [Register] [Register] | MOV R0 #1<br>MOV R1 #1<br>CMRRL R0 R1 | COMPARE R{x} if little than R{x}                |
| 0x000F           | CMASE [Register] [Stack]    |                                       | COMPARE ACC if equal than Stack                 |
| 0x0010           | CMASG [Register] [Stack]    |                                       | COMPARE ACC if greater than Stack               |
| 0x0011           | CMASL [Register] [Stack]    |                                       | COMPARE ACC if little than Stack                |
| 0x0012           | CMRSE [Register] [Stack]    |                                       | COMPARE R{x} if little than Stack               |
| 0x0013           | CMRSG [Register] [Stack]    |                                       | COMPARE R{x} if greater than Stack              |
| 0x0014           | CMRSL [Register] [Stack]    |                                       | COMPARE R{x} if little than Stack               |
| 0x0015           | INCA                        |                                       | INCREASE ACC                                    |
| 0x0016           | DECA                        |                                       | DECREASE ACC                                    |
| 0x0017           | INCR                        |                                       | INCREASE Register{x}                            |
| 0x0018           | DECR                        |                                       | DECREASE Register{x}                            |
| 0x0019           | ADDAR [Register]            | MOV R0 #1<br>ADDAR R0                 | ADD ACC and Register{x}                         |
| 0x001A           | SUBAR [Register]            | MOV R0 #1<br>SUBAR R0                 | SUB ACC and Register{x}                         |
| 0x001B           | INCS                        |                                       | INCREASE Stack                                  |
| 0x001C           | DECS                        |                                       | DECREASE Stack                                  |
| 0x001D           | ADDAS [Stack]               |                                       | ADD ACC and Stack                               |
| 0x001E           | SUBAS [Stack]               |                                       | SUB ACC and Stack                               |
| 0x001F           | ANDR [Register]             | MOV R0 #1<br>ANDR R0                  | ACC AND Register{x}                             |
| 0x0020           | AOR [Register]              | MOV R0 #1<br>AOR R0                   | ACC OR Register{x}                              |
| 0x0021           | AXR [Register]              | MOV R0 #1<br>AXR R0                   | ACC XOR Register{x}                             |
| 0x0022           | BSLR [Register]             | MOV R0 #1<br>BSLR R0                  | BIT Shift left in Register{x}                   |
| 0x0023           | BSRR [Register]             | MOV R0 #1<br>BSRR R0                  | BIT Shift right in Register{x}                  |
| 0x0024           | BSLLR [Register]            | MOV R0 #1<br>BSLLR R0                 | BIT Shift left loop in Register{x}              |
| 0x0025           | BSRLR [Register]            | MOV R0 #1<br>BSRLR R0                 | BIT Shift right loop in Register{x}             |
| 0x0026           | ANDS [Stack]                |                                       | ACC AND Stack                                   |
| 0x0027           | AOS [Stack]                 |                                       | ACC OR Stack                                    |
| 0x0028           | AXS [Stack]                 |                                       | ACC XOR Stack                                   |
| 0x0029           | BSLS [Stack]                |                                       | BIT Shift left in Stack                         |
| 0x002A           | BSRS [Stack]                |                                       | BIT Shift right in Stack                        |
| 0x002B           | BSLLS [Stack]               |                                       | BIT Shift left loop in Stack                    |
| 0x002C           | BSRLS [Stack]               |                                       | BIT Shift right loop in Stack                   |
| 0x002D           | IMA #[Hex]                  |                                       | Immediately value to ACC                        |
| 0x002E           | IMR #[Hex]                  |                                       | Immediately value to Register                   |
| 0x002F           | IMS #[Hex]                  |                                       | Immediately value to Stack                      |
| 0x0030           | GET [Register]              |                                       | Get value from address                          |
| 0x0031           | MVRR [Register] [Register]  | MOV R0 #1<br>MOV R1 #1<br>MVRR R0 R1  | MOVE Register value to another Register{x}      |
| 0x0032           | MVRS [Register] [Stack]     |                                       | MOVE Register value to Stack                    |
| 0x0033           | MVSR [Register] [Register]  | MOV R0 #1<br>MOV R1 #1<br>MVRS R0 R1  | MOVE Stack value to Register                    |
| 0x0034           | MVAR [Register]             |                                       | MOVE ACC value to Register                      |
| 0x0035           | INTK                        |                                       | Wait key interupt                               |
| 0x0036           | KEY                         |                                       | Get key                                         |
| 0x0037           | PLY [Register]              | MOV R0 #1<br>PLY R0                   | Play dididi sound                               |
| 0x0038           | PLYS [Register]             | MOV R0 #1<br>PLYS R0                  | Play a series of sound.Usually have a frequence |
| 0x0039           | DPXY [Register]             | MOV R0 #1<br>DPXY R0                  | Draw 8 \* x pixels at (x, y)                    |
| 0x003A           | SCRU                        |                                       | Screen scrool up x pixel                        |
| 0x003B           | SCRD                        |                                       | Screen scrool down x pixel                      |
| 0x003C           | SCRL                        |                                       | Screen scrool left x pixel                      |
| 0x003D           | SCRR                        |                                       | Screen scrool right x pixel                     |
| 0x003E           | DXY [Register]              | MOV R0 #1<br>DXY R0                   | Draw point at (x, y)                            |
| 0x003F           | DCXY [Register]             | MOV R0 #1<br>DCXY R0                  | Draw char at (x, y)                             |
| 0x0040           | CLS                         |                                       | Clear screen                                    |
| 0x0041           | GSET [Register]             | MOV R0 #1<br>GSET R0                  | Graphics resolution setting, [64X32 - 640X320]  |

##### _Notice:_

> All instruction return value is store in passtive side. Such `MOVSR {Rx}` means move stack value to register ,passtive side is register.But in `INCA` means ACC increase 1 and return value to ACC.

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

## VM Program Specific

### Reserved Keywords

- **_START_**: used for start vm
- **_MAIN_**: main like c
- **_END_**: sub process end label
- **_STOP_**: stop vm

##### _Notice:_

> <div style="color:red">Can't use those words in user program!</div>

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

![](res/bc-format.png)
