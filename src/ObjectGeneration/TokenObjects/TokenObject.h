#ifndef MAJOR_TOKEN_OBJECT_H
#define MAJOR_TOKEN_OBJECT_H

class TokenObject {
protected:
  int sourceFileIndex_;
public:
  TokenObject(int sourceFileIndex_);
  int sourceFileIndex();
};

#endif 
