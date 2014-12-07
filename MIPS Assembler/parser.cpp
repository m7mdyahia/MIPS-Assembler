#include "Instruction.h"
#include <vector>


int main()
{
	char outputmode;
	cout << "MIPS Assembler\n"
		"output modes:\n"
		"1:Binary 8 bits per line\n"
		"2:Hexadicimal 8 bits per line\n"
		"3:Binary 32 bits per line\n"
		"4:Hexadicimal 32 bits per line\n";


	vector <Instruction*> instr_list; //list contains all programm structure
	//list<Instruction*>::iterator pos;

	ifstream source("assembly.asm");
	ofstream object("bin.txt");

	cin >> outputmode;
	string str;
	
	while (getline(source, str))
	{
		instr_list.push_back(Instruction::make_instr(str)); //calling the factory method
		
	}


	//Assembling each instruction
	for (unsigned int i = 0;i<instr_list.size();i++)
	{
	
		object << instr_list[i]->str_hex();
		
	}


	return 0;
}