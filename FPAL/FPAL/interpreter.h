#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include <string>
#include "exceptions.h"


enum lineType {nop, define, function, endfunction, instruction, declare, label};


struct variable {
	std::string name;
	int address;
};



class SourceFile{
private:

    char * originalFile;
    int originalFileLength;
	int lineCount;
	std::string * lines;
	lineType * lineTypes;
	int getOpCode(int line);
	

public:
	int defineCount = 0;
	std::string defines;
	int globalVariableCount = 0;
	int localFileVariableCount = 0;
	variable * variables;
	int getLineCount();
	int getLineType(int lineNum);
	std::string getLine(int lineNum);
    void setOriginalFile(char * originalFile, int originalFileLength);
    void generateLines();
	void identifyLines();
	void allocateVariables();

};

#endif // PARSER_H_INCLUDED
