#include "interpreter.h"
#include <iostream>

void SourceFile::setOriginalFile(char * originalFile, int originalFileLength){
    this->originalFile = originalFile;
    this->originalFileLength = originalFileLength;
}

void SourceFile::generateLines(){
    char currentChar;
    int position = 0;
    this->lineCount = 1;
    do{//Count lines
        currentChar = this->originalFile[position];
        if(currentChar == '\n'){
            lineCount++;
        }
        position++;
    }
    while(position < this->originalFileLength);
    this->lines = new std::string[this->lineCount];
    position = 0;
    int i = 0;
    do{//Read lines
        currentChar = this->originalFile[position];
        if(currentChar == '\n'){
            i++;
        }
        else{
            this->lines[i] += currentChar;
        }
        position++;
    }while(position < this->originalFileLength);

	i = 0;
	do {//Remove \r character from every line
		if (this->lines[i][this->lines[i].length() - 1] == '\r')this->lines[i].pop_back();
		i++;
	} while (i < this->lineCount);
}

void SourceFile::identifyLines(){//IT IDENTIFIES POSSIBLE LINE MEANING, IT DOES NOT CHECK IF SYNTAX IS CORRECT. TODO
	int pos = 0;
	std::string word;
	bool inFunction = false;
	this->lineTypes = new lineType[this->lineCount];
	for (int i = 0; i < this->lineCount; i++) {//For each line
		pos = 0;
		word = "";
		if (this->lines[i].length() == 0) {//If line is empty, it's a nop
			this->lineTypes[i] = nop;
			continue;
		}
		while ((this->lines[i][pos] == ' ') || (this->lines[i][pos] == '\t')){
			pos++;//Skip first spaces
		}
		if (this->lines[i][pos] == '.') {//If it starts with '.'
			pos++;//Skip point
			do {//Read word after '.'
				word += lines[i][pos];
				pos++;
			} while ((pos < this->lines[i].length()) && (this->lines[i][pos] != ' ' && this->lines[i][pos] != '\t'));
			if (word == "endfunc") {//If it's "endfunc", it's a endfunc line type
				if (!inFunction)throw EXCEPTION_ENDFUNC_OUT_OF_FUNCTION;
				this->lineTypes[i] = endfunction;//Unless it was in a define block
				inFunction = false;
			}
			else if (word == "def") {//If it's "define", it's nop type and start of define block
				if (inFunction)throw EXCEPTION_KEYWORD_IN_FUNCTION;
				this->lineTypes[i] = define;
			}
			else if (word == "dec") {//If it's "declare", it's a variable delcare type
				if (inFunction)throw EXCEPTION_KEYWORD_IN_FUNCTION;
				this->lineTypes[i] = declare;
				localFileVariableCount++;
			}
			else {//If it isn't a keyword, it's a function
				if (inFunction)throw EXCEPTION_FUNCTION_DEFINE_IN_FUNCTION;
				this->lineTypes[i] = function;
				inFunction = true;
			}
		}
		else if (this->lines[i][pos] == ';') {//If it starts with a ';', it's a comment line, (nop)
			this->lineTypes[i] = nop;
		}
		else if (this->lines[i][pos] == ':') {
			if (!inFunction)throw EXCEPTION_LABEL_OUT_OF_FUNCTION;
			this->lineTypes[i] = label;
		}
		
		else {//If it doesn't identify it as anything else, it's an instruction
			if (!inFunction)throw EXCEPTION_INSTRUCTION_OUT_OF_FUNCTION;
			this->lineTypes[i] = instruction;
		}
	}
}

void SourceFile::allocateVariables() {
	this->variables = new variable[this->localFileVariableCount];
	int pos = 0;
	this->globalVariableCount = 0;
	int vari = 0;
	for (int i = 0; i < this->lineCount; i++) {
		if (this->lineTypes[i] == declare) {
			while ((this->lines[i][pos] == ' ') || (this->lines[i][pos] == '\t')) {
				pos++;//Skip first spaces
			}
			pos += 4; //Skip .dec
			while ((this->lines[i][pos] == ' ') || (this->lines[i][pos] == '\t')) {
				pos++;//Skip spaces
			}
			std::string name;
			while (!((this->lines[i][pos] == ' ') || (this->lines[i][pos] == '\t') || (this->lines[i].length() <= pos))) {
				name += this->lines[i][pos];
				pos++;
			}
			this->variables[vari].name = name;
			this->variables[vari].address = this->globalVariableCount;
			globalVariableCount++;
			vari++;
			pos = 0;
		}
	}
}


int SourceFile::getLineCount() {
	return this->lineCount;
}

int SourceFile::getLineType(int lineNum) {
	return this->lineTypes[lineNum];
}

std::string SourceFile::getLine(int lineNum) {
	return this->lines[lineNum];
}