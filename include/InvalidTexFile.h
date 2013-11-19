#ifndef INVALIDTEXFILE_H_
#define INVALIDTEXFILE_H_

#include <string>

using namespace std;

class InvalidTexFile {
	string filepath;
public:
	InvalidTexFile(string filepath);
	virtual ~InvalidTexFile();

	string getFilePath();
};

#endif /* INVALIDTEXFILE_H_ */
