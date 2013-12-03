#include "InvalidPreAttrException.h"

using namespace std;

InvalidPreAttrException::InvalidPreAttrException() {
}

InvalidPreAttrException::InvalidPreAttrException(string attr) {
	error_message = "Invalid \"" + attr + "\" defined. Exiting...\n";
}

InvalidPreAttrException::~InvalidPreAttrException() {
}

string InvalidPreAttrException::getErrMessg() {
	return error_message;
}
