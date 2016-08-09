#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include <string>
#include "exceptions.h"
enum OperandType {none, aluOP, aluModeSelect, aluForceCarrySelect, aluCarryFromPrevSelect, reg1, reg2, immediate, promAddr, ramAddr, LsbReadSelect};

enum ALUOperations {alu_add, alu_sub, alu_inc, alu_dec, alu_lsh, alu_rsh, alu_and, alu_nand, alu_or, alu_nor, alu_xor, alu_xnor, alu_not, alu_notb};

enum Registers {acc, b, mar_lsb, mar_msb};

class Instruction{
    public:
        virtual uint16_t buildInstruction() = 0;
        static char getOpCodeForInstruction(std::string instructionMnemonic);
};

class NoOperation : public Instruction{
public:
    uint16_t buildInstruction();
private:
    static const char opcode = 0x00;
    const static char operands[];
};

class StoreImmediate : public Instruction{
public:
    uint8_t immediateToSet;
    uint16_t buildInstruction();
private:
    static const char opcode = 0x01;
    static const char operands[];
};

class ALUOperation : public Instruction{
public:
    uint8_t aluFunctionToExecute;
    bool carryFromPrevOp;
    uint16_t buildInstruction();
private:
    static const char opcode = 0x02;
    static const char operands[];
};


class StoreToRAM : public Instruction{
public:
    uint16_t LsbAddress;
    bool useLsbRegister;
    uint16_t buildInstruction();
private:
    static const char opcode = 0x03;
    static const char operands[];
};

class LoadFromRAM : public Instruction{
public:
    uint8_t LsbAddress;
    bool useLsbRegister;
    uint16_t buildInstruction();
private:
    static const char opcode = 0x04;
    static const char operands[];
};

class Move : public Instruction{
public:
    uint8_t regSrc;
    uint8_t regDst;
    uint16_t buildInstruction();
private:
    static const char opcode = 0x05;
    static const char operands[];
};


class Jump : public Instruction{
public:
    uint16_t promLsbTarget;
    bool useLsbRegister;
    uint16_t buildInstruction();
private:
    static const char opcode = 0x06;
    static const char operands[];
};

class JumpIfEqual : public Instruction{
public:
    uint16_t promLsbTarget;
    bool useLsbRegister;
    uint16_t buildInstruction();
private:
    static const char opcode = 0x07;
    static const char operands[];
};

class JumpIfZero : public Instruction{
public:
    uint16_t promLsbTarget;
    bool useLsbRegister;
    uint16_t buildInstruction();
private:
    static const char opcode = 0x08;
    static const char operands[];
};



#endif // INSTRUCTIONS_H
