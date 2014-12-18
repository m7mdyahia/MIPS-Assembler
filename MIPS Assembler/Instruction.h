#ifndef Instruction_H_
#define Instruction_H_

#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include <cstdio>
#include <sstream>
#include <map>
#include <utility>
#include <stdexcept>


//#include <iomanip>
using namespace std;

class SourceFile;//forward declaration

class Instruction
{
public:
	
	//Factory Method
	static Instruction* make_instr(const string & opcode_str,   SourceFile* caller);
	bool resolve_label(signed int target_address);
	bool is_resolved(){ return resolved; };

	//Return string containing the binary object code in binary 8-bits per line
	string str_bin() const;

	//Return string containing the binary object code in hex 8-bits per line
	string str_hex() const;

	string str_hex_32() const;

	string str_bin_32() const;

	//for testing
#ifdef _DEBUG
	Instruction(unsigned long l);
	Instruction(const string& str);
	Instruction(const bitset<32>& allinstr);
#endif
	Instruction(void);
	//end for testing

	Instruction(int opcode_int_par, const string & instr_str_par);

	

//	virtual void assemble();//TODO make it pure virtual

	friend ostream & operator<<(ostream & os, const Instruction & instr);
	friend ostream & operator<<(ostream & os, const Instruction * instr_p);


protected:
	//bitset<32> binstr;
	unsigned int instr_value;
	unsigned int op;
	string instr_str;//not good

	bool resolved;
	

	static const map<string, unsigned char > op_map;
	static const map<string, unsigned char > fn_map;
	static const map<string, unsigned char > r_map;

	static string& trim(string& str);
	
	static unsigned char register_number(string& str);
	

};

class RInstr : public Instruction
{
public:
	RInstr(const string & fn_str_par, const string & instr_str_par,  SourceFile *);
private:

};

class IInstr : public Instruction
{
private:
	
public:
	IInstr(int opcode_int_par, const string & instr_str_par,  SourceFile *);
};

class JInstr : public Instruction
{
private:
	

public:
	JInstr(int opcode_int_par, const string & instr_str_par,  SourceFile *);
};



#endif