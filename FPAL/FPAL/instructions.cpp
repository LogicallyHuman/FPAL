#include "instructions.h"

//DEFINITION OF OPERAND BITS FOR EACH INSTRUCTON


const char NoOperation::operands[] =    {none, none, none, none, none, none, none, none, none, none, none, none};

const char StoreImmediate::operands[] = {immediate, immediate, immediate, immediate, immediate, immediate, immediate, immediate, none, none, none, none};

const char ALUOperation::operands[] =   {aluOP, aluOP, aluOP, aluOP, aluModeSelect, aluForceCarrySelect, aluCarryFromPrevSelect, none, none, none, none, none};

const char StoreToRAM::operands[] =     {ramAddr, ramAddr, ramAddr, ramAddr, ramAddr, ramAddr, ramAddr, LsbReadSelect, none, none, none, none};

const char LoadFromRAM::operands[] =    {ramAddr, ramAddr, ramAddr, ramAddr, ramAddr, ramAddr, ramAddr, LsbReadSelect, none, none, none, none};

const char Move::operands[] =           {reg1, reg1, reg1, reg1, reg2, reg2, reg2, reg2, none, none, none, none};

const char Jump::operands[] =           {promAddr, promAddr, promAddr, promAddr, promAddr, promAddr, promAddr, promAddr, promAddr, promAddr, promAddr, LsbReadSelect};

const char JumpIfEqual::operands[] =    {promAddr, promAddr, promAddr, promAddr, promAddr, promAddr, promAddr, promAddr, promAddr, promAddr, promAddr, LsbReadSelect};

const char JumpIfZero::operands[] =     {promAddr, promAddr, promAddr, promAddr, promAddr, promAddr, promAddr, promAddr, promAddr, promAddr, promAddr, LsbReadSelect};


//BUILD INSTRUCTION FUNCTION DEFINITIONS FOR EACH INSTRUCTION

uint16_t NoOperation::buildInstruction(){
    return 0x00;//No Operation only returns 0x00
}

uint16_t StoreImmediate::buildInstruction(){
    return (uint16_t)(this->opcode << 12) | (uint16_t)(this->immediateToSet<< 4);
}

uint16_t ALUOperation::buildInstruction(){
    switch (this->aluFunctionToExecute){
        case alu_add:  return (uint16_t)(this->opcode << 12) | 0x0900 | carryFromPrevOp << 5; break;//wall of code
        case alu_sub:  return (uint16_t)(this->opcode << 12) | 0x0640 | carryFromPrevOp << 5; break;
        case alu_inc:  return (uint16_t)(this->opcode << 12) | 0x0040 | carryFromPrevOp << 5; break;
        case alu_dec:  return (uint16_t)(this->opcode << 12) | 0x0f00 | carryFromPrevOp << 5; break;
        case alu_lsh:  return (uint16_t)(this->opcode << 12) | 0x0c40 | carryFromPrevOp << 5; break;
        case alu_rsh:  return (uint16_t)(this->opcode << 12) | 0x0000 | carryFromPrevOp << 5; break;//TODO: DECIDE A OP FOR RIGHT SHIFT AND IMPLEMENT HARDWARE
        case alu_and:  return (uint16_t)(this->opcode << 12) | 0x0b80 | carryFromPrevOp << 5; break;
        case alu_nand: return (uint16_t)(this->opcode << 12) | 0x0480 | carryFromPrevOp << 5; break;
        case alu_or:   return (uint16_t)(this->opcode << 12) | 0x0e80 | carryFromPrevOp << 5; break;
        case alu_nor:  return (uint16_t)(this->opcode << 12) | 0x0180 | carryFromPrevOp << 5; break;
        case alu_xor:  return (uint16_t)(this->opcode << 12) | 0x0680 | carryFromPrevOp << 5; break;
        case alu_xnor: return (uint16_t)(this->opcode << 12) | 0x0980 | carryFromPrevOp << 5; break;
        case alu_not:  return (uint16_t)(this->opcode << 12) | 0x0180 | carryFromPrevOp << 5; break;
        case alu_notb: return (uint16_t)(this->opcode << 12) | 0x0580 | carryFromPrevOp << 5; break;
        default: throw(EXCEPTION_OPERAND_NOT_FOUND); break;
    }
}

uint16_t StoreToRAM::buildInstruction(){
    if(this->useLsbRegister){
        return (uint16_t)(this->opcode << 12) | 1;
    }
    else{
        if(this->LsbAddress > 0x7ff){
            throw(EXCEPTION_OPERAND_OUT_OF_RANGE);
        }
        else{
            return (uint16_t)(this->opcode << 12) | (uint16_t)(this->LsbAddress << 1) | this->useLsbRegister;
        }
    }
}

uint16_t LoadFromRAM::buildInstruction(){
    if(this->useLsbRegister){
        return (uint16_t)(this->opcode << 12) | 1;
    }
    else{
        if(this->LsbAddress > 0x7ff){
            throw(EXCEPTION_OPERAND_OUT_OF_RANGE);
        }
        else{
            return (uint16_t)(this->opcode << 12) | (uint16_t)(this->LsbAddress << 1) | this->useLsbRegister;
        }
    }
}

uint16_t Move::buildInstruction(){
    int16_t returnInstruction = this->opcode << 12;
    switch (this->regSrc){
        case(acc): returnInstruction     |= 0x0 << 8; break;
        case(b): returnInstruction       |= 0x1 << 8; break;
        case(mar_lsb): returnInstruction |= 0x2 << 8; break;
        case(mar_msb): returnInstruction |= 0x3 << 8; break;
        default: throw(EXCEPTION_OPERAND_NOT_FOUND); break;
    }
    switch (this->regDst){
        case(acc): returnInstruction     |= 0x0 << 4; break;
        case(b): returnInstruction       |= 0x1 << 4; break;
        case(mar_lsb): returnInstruction |= 0x2 << 4; break;
        case(mar_msb): returnInstruction |= 0x3 << 4; break;
        default: throw(EXCEPTION_OPERAND_NOT_FOUND); break;
    }
    return returnInstruction;
}

uint16_t Jump::buildInstruction(){
    if(this->useLsbRegister){
        return (this->opcode << 12) | (0x0001);
    }
    else{
        if(this->promLsbTarget < 0x400){
            return (this->opcode << 12) | (promLsbTarget << 1);
        }
        else throw(EXCEPTION_OPERAND_OUT_OF_RANGE);
    }
}

uint16_t JumpIfEqual::buildInstruction(){
    if(this->useLsbRegister){
        return (this->opcode << 12) | (0x0001);
    }
    else{
        if(this->promLsbTarget < 0x400){
            return (this->opcode << 12) | (promLsbTarget << 1);
        }
        else throw(EXCEPTION_OPERAND_OUT_OF_RANGE);
    }
}

uint16_t JumpIfZero::buildInstruction(){
    if(this->useLsbRegister){
        return (this->opcode << 12) | (0x0001);
    }
    else{
        if(this->promLsbTarget < 0x400){
            return (this->opcode << 12) | (promLsbTarget << 1);
        }
        else throw(EXCEPTION_OPERAND_OUT_OF_RANGE);
    }
}

