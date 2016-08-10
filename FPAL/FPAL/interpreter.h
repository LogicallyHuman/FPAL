#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include <string>
#include "exceptions.h"
#include "instructions.h"

enum lineType {blank, define, function, endfunction, instruction, declare, label};


struct Variable {
	std::string name;
	int address;
};

struct Function {
	std::string name;
	int size = 0;
	char * instructions;
};

struct Instruction {
	char opcode;
	char operand1;
	char operand2;
};

class SourceFile{
private:

    char * originalFile;
    int originalFileLength;
	int lineCount;
	Function * functions;
	std::string * lines;
	lineType * lineTypes;
	int getOpCode(int line);
	

public:
	int defineCount = 0;
	std::string defines;
	int globalVariableCount = 0;
	int localFileVariableCount = 0;
	Variable * variables;
	int getLineCount();
	int getLineType(int lineNum);
	std::string getLine(int lineNum);
    void setOriginalFile(char * originalFile, int originalFileLength);
    void generateLines();
	void identifyLines();
	void allocateVariables();

};

#endif // PARSER_H_INCLUDED
