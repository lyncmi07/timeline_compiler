#include "PreProcessedFile.h"
#include <string>

#include "../Exceptions/CompilerExceptions.h"

using std::cout;
using std::endl;

PreProcessedFile::PreProcessedFile(std::string file) noexcept(false){
  cout << "PRE PROCESSING FILE: " << file << endl;
  
  int lastDirectoryIndex = file.rfind("/");

  fileName_ = new std::string(file.substr(lastDirectoryIndex+1));
  filePath_ = new std::string(file.substr(0, lastDirectoryIndex+1));
  fileLength_ = 0;
  allCode_ = new std::string();


  std::ifstream infile(file.c_str());

  int currentSize = 0;
  std::string line;
  while(std::getline(infile, line)) {
    int includeDefIndex = line.find("#include ");
    if(includeDefIndex != -1) {
      //there is an include pre processor statement on this line
      //include statement takes on the form #include "filename"

      int fileNameIndex = line.find("\"") + 1;

      if(fileNameIndex == -1) {
        cout << "\033[1;41mERROR:\033[0m include statement implemented incorrectly" << endl;
        break;
      }

      std::string sourceFileName = line.substr(includeDefIndex + 10);
      int fileNameEndIndex = sourceFileName.find("\"");
      sourceFileName = sourceFileName.substr(0,fileNameEndIndex);

      cout << "INCLUDING: " << sourceFileName << endl;

      fileNames_.push_back(sourceFileName);
      //fileNames_.insert(fileNames_.begin(), sourceFileName);
      
    } else {
      currentSize += line.length();
      fileLineEnds_.push_back(currentSize);
      *allCode_ += line;
    }
  }

  fileLength_ += currentSize;
  fileStarts_.push_back(0);

  infile.close();

  for(int i = fileNames_.size()-1; i >= 0; i--) {
    //now include the other files
    std::string fullFilename = *filePath_ + fileNames_[i];
    std::ifstream includeFile(fullFilename.c_str());
    std::string includeString;
    currentSize = 0;
    std::vector<int> currentLineEnds;
    while(std::getline(includeFile, line)) {
      currentSize += line.length();
      currentLineEnds.push_back(currentSize);
      includeString += line;
    }
    if(currentSize == 0) {
     std::cout << "\033[1;41mERROR:\033[0m There is no file '" << fileNames_[i] << "' or it is empty" << std::endl;
     throw comp_except::PreProcessException();
    }
    includeFile.close();

    //increase the file size to include the extra file
    fileLength_ += currentSize;

    //update the file line starts to be offset by the number of lines in the new file
    for(int i = 0; i < fileStarts_.size(); i++) {
      fileStarts_[i] = fileStarts_[i] + currentLineEnds.size();
    }
    fileStarts_.insert(fileStarts_.begin(), 0);
    //fileStarts_.push_back(0);

    //update the line endings to be offset by the number of characters in the new file
    for(int i = 0; i < fileLineEnds_.size(); i++) {
      currentLineEnds.push_back(fileLineEnds_[i] + currentSize);
    }
    fileLineEnds_ = currentLineEnds;
    
    

    //add this new code into the current code string.
    std::string* newCode = new std::string(includeString);
    *newCode += *allCode_;
    delete allCode_;
    
    allCode_ = newCode;
  }

}

PreProcessedFile::~PreProcessedFile() {
  delete fileName_;
  delete filePath_;
  delete allCode_;
}

std::string PreProcessedFile::getFilePosition(int characterIndex) {
  if(characterIndex > fileLength_) {
    return "(EOF)";
  }

  int linePosition = -1;

  for(int i = 0; i < fileLineEnds_.size(); i++) {
    if(characterIndex < fileLineEnds_[i]) {
	linePosition = i;
	break;
	//return ":" + std::to_string(i);
    }
  }

  if(linePosition == -1) {
    return "(NOT FOUND)";
  }
  int fileNumber = -1;
  for(int i = 1; i < fileStarts_.size(); i++) {
    if(linePosition < fileStarts_[i]) {
      fileNumber = i-1;
      break;
    }
  }

  if(fileNumber == -1) {
    //line is in the main file
 
    //get rid of the offset of the lines in the other files
    linePosition -= fileStarts_[fileStarts_.size()-1];
    //add back in the pre-process lines that have been removed. And add 1 because file lines start at 1.
    linePosition += fileNames_.size() + 1;
    std::string returningString = *fileName_;
    returningString += ":";
    returningString += std::to_string(linePosition);
    return returningString;
  } else {
    //line is in one of the included files
 
    //get rid of the offset of the lines in the other files. And add 1 because file lines start at 1
    linePosition -= fileStarts_[fileNumber];
    linePosition++;
    return fileNames_[fileNumber] + ":" + std::to_string(linePosition);
  }

  return "(NOT FOUND)";
}

char PreProcessedFile::operator[](int i) {
  return allCode_->c_str()[i];
}
std::string* PreProcessedFile::operator->() {
  return allCode_;
}
