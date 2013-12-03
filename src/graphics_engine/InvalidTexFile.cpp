#include "InvalidTexFile.h"

InvalidTexFile::InvalidTexFile(string filepath) {
	this->filepath = filepath;
}

InvalidTexFile::~InvalidTexFile() {
}

string InvalidTexFile::getFilePath() {
	return filepath;
}
