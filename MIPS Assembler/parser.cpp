#include "sourcefile.h"

int main()
{
	cout << "MIPS Assembler\n";
	char outputmode;
	string src_file, obj_file;

	/*
	cout << "Enter the source file name : ";

	cin >> src_file;
	
	cout << "Enter the object file name : ";

	cin >> obj_file;
			
	*/
	cout << "Assembling the file ...\n";
	try
	{
		SourceFile src("assembly.asm", "bin.txt");
		cout << "Finished Assembling the file choose the ";

		cout << endl << "output modes:\n"
			<< "0:Binary 8 bits per line\n"
			<< "1:Hexadicimal 8 bits per line\n"
			<< "2:Binary 32 bits per line\n"
			<< "3:Hexadicimal 32 bits per line\n";

		bool c;
		do
		{
			cin >> outputmode;
			c = src.save_output(outputmode);
			if (!c) cout << "invalid output mode\nre-enter the output mode\n";

		} while (!c);

		cout << "successfully saved\n";

	}

	catch (...)
	{

	}


		
}