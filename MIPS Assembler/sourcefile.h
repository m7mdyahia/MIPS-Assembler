#ifndef SOURCEFILE
#define SOURCEFILE

#include "Instruction.h"

#include <vector>
#include <map>
#include <string>
#include <fstream>

using namespace std;



class SourceFile
{
public:
	SourceFile();
	SourceFile(const  string & src, const string & obj="");
	bool save_output(char omode, const string & obj);
	bool save_output(char omode);
	bool save_output(const string & obj);
	bool save_output();
	bool label_address(const string & label_str, size_t& current_address, size_t& next_address);
private:
	
	ifstream source;
	string object_str;
	vector <Instruction*> instr_list; //list contains all programm instructions
	void set_instr_list();
	void add_label(const string &);
	void assemble_line(const string &);
	map <string,size_t> label_list;
	map <size_t, string> unresolved_instr;
	//vector<size_t> unresolved_list;
	char outputmode;
	
};
#endif