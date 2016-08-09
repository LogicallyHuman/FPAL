
#include <iostream>
#include <bitset>
#include <string>
#include <iomanip>
#include "instructions.h"
#include "reader.h"
#include "interpreter.h"
/*
    Command line arguments:
    fenasm <assembly file>

    Assembler Steps:                                        | Code finished
    1)Read File                                             |   YES
    2)Interpret each line                                   |   NO
    3)Locate Other Files                                    |   NO
    4)Go to step 1 until no files remain                    |
    5)Index Variables                                       |   NO
    6)Index Defines                                         |   NO
    7)Index Functions                                       |   NO
    8)Allocate addresses for variables                      |   NO
    9)Place functions                                       |   NO
    10)Replace calls with function addresses                |   NO
    11)Replace defines                                      |   NO
    12)Build instructions                                   |   YES
    13)Output to binary file                                |   NO
  */


    /*
    Arguments:
    -main        Folowed by file name to build
    -logisim     Builds binary for logisim
    */
//enum lineType { nop, define, func, endfunc, instruction, declare };

std::string foo(int a) {
	switch (a)
	{
	case nop:
		return "nop        ";
	case define:
		return "define     ";
	case function:
		return "function   ";
	case endfunction:
		return "endfunction";
	case instruction:
		return "instruction";
	case declare:
		return "declare    ";
	case label:
		return "label      ";
	default:
		return "error      ";
	}
}

int main(){
    int fileSize;
    char * fileContents;
    fileSize = readEntireFile("testFPAL.txt", &fileContents);
    SourceFile file;
	file.setOriginalFile(fileContents, fileSize);

	file.generateLines();
	file.identifyLines();
	file.allocateVariables();

    for(int i = 0; i < file.getLineCount(); i++){
        std::cout << "Line " << std::setfill('0') << std::setw(log(file.getLineCount()) - 1) << i << " : (" << foo(file.getLineType(i)) << ") : " << file.getLine(i) << std::endl;
    }
	std::cout << std::endl << std::endl;
	for (int i = 0; i < file.localFileVariableCount; i++) {
		std::cout << "Variable " << i << ": " << file.variables[i].name << " at " << file.variables[i].address << std::endl;
	}
	getchar();
    return 0;

}
