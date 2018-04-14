#ifndef MAJOR_PRE_PROCESSED_FILE_H
#define MAJOR_PRE_PROCESSED_FILE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class PreProcessedFile {
private:
	//this is the file name of the main file
	//TODO: return to static memory. This was only made dynamic due to issues with an early version of c++ now not being used.
	std::string* fileName_;
	std::string* filePath_;

	std::string* allCode_;
	//list of line numbers that are the first line in a file
	std::vector<int> fileStarts_;
	//this is the files that have been included into the main file
	std::vector<std::string> fileNames_;
	std::vector<int> fileLineEnds_;

	int currentIndex_;
	int currentLine_;
	int currentFile_;
	int fileLength_;
public:
	/*
	  This takes a source file and checks the pre-process statements.
	  The pre-process statements are used to include other files source code.
	  Files that are included are not able to include files themselves.
	*/
	PreProcessedFile(std::string fileName) noexcept(false);
	~PreProcessedFile();

	/*
	  This returns a string in the format "file:lineNumber" of where the character of a particular index lies
	  in the source files. This is useful for the compiler to throw error messages.
	*/
	std::string getFilePosition(int characterIndex);
	
	char operator[](int i);
	std::string *operator->();

	/*
	  Return all the code in a single string
	*/
	std::string allCode() {
	  return *allCode_;
	};

	/*
	  Return the length of all the files combined
	*/
	int fileLength() {
	  return fileLength_;
	};
};

#endif
