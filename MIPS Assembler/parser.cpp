#include "sourcefile.h"

int main()
{
	char outputmode;
	string src_file, obj_file;
	cout << "MIPS Assembler\n";
	/*
	cout << "Enter the source file name";
	cin >> src_file;
	cout << "Enter the object file name";
	cin >> obj_file;
	SourceFile src(src_file, obj_file);
	*/
	SourceFile src("assembly.asm", "bin.txt");

	
		cout << "output modes:\n"
		<< "0:Binary 8 bits per line\n"
		<< "1:Hexadicimal 8 bits per line\n"
		<< "2:Binary 32 bits per line\n"
		<< "3:Hexadicimal 32 bits per line\n";
		

		do
		{
			cin >> outputmode;
		} while (!(src.save_output(outputmode)));
		
	
}