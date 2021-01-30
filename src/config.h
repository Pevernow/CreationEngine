#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <map>
#include <string>
using namespace std;

#define COMMENT_CHAR '#'

bool ReadConfig(const string& filename, map<string, string>& m);
bool WriteConfig(const string& filename, map<string, string>& m);
void PrintConfig(const map<string, string>& m);
#endif