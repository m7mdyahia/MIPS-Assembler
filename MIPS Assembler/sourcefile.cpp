#include "sourcefile.h"

SourceFile::SourceFile() :source("assembly.asm"), object_str("bin.txt")
{
	
	outputmode = 0;
	set_instr_list();
	
}
SourceFile::SourceFile(const  string & src, const string & obj) : source(src), object_str(obj)
{

	outputmode = 0;
	set_instr_list();

	
}

void SourceFile::set_instr_list()
{
	string instructionstring_;
//Firist pass
	while (getline((source >> ws), instructionstring_))//neglects leading whitespaces including empty lines
	{
		string::size_type semicolon_pos = instructionstring_.find(':');
		if (semicolon_pos != string::npos)//
		{
			
			//string label_str = string(instructionstring_, 0, instructionstring_.find(':'));
			add_label(instructionstring_.substr( 0, semicolon_pos));
			if (!(semicolon_pos + 1 > instructionstring_.size() - 1)) assemble_line( instructionstring_.substr(semicolon_pos + 1));

		}
		else //instruction line
		{
			assemble_line(instructionstring_);
		}
	}//end firist pass

//second pass
map <size_t, string>::iterator ui;
for (ui = unresolved_instr.begin(); ui != unresolved_instr.end(); ui++)
{
	instr_list[ui->first] = Instruction::make_instr(ui->second, this);
}

}

void SourceFile::add_label(const string &label_str_)
{
	map <string, size_t> ::iterator pos = label_list.find(label_str_);
	if (pos == label_list.end()) // Check if there was a match
	{
		label_list[label_str_] = (instr_list.size());
	}
	else//the label already exist
	{
		throw logic_error("Repeated reference");

	}

}

void SourceFile::assemble_line(const string & instructionstring)
{

	size_t first = instructionstring.find_first_not_of(" \t");
	size_t last = instructionstring.find_last_not_of(" \t");
//	instructionstring = instructionstring.substr(first, (last - first + 1));

	string trimed_instructionstring(instructionstring, first, last - first + 1);

	Instruction* Ip = Instruction::make_instr(instructionstring, this);
	if (Ip->is_resolved())
	{
		instr_list.push_back(Ip); //calling the factory method
	}
	else //unresolvedlabel
	{
		instr_list.push_back(nullptr);
		unresolved_instr[instr_list.size() - 1] = instructionstring;//size=lastelement+1
	}

}

bool SourceFile::label_address(const string & label_str, size_t& current_address, size_t& next_address)
{
	current_address = (instr_list.size());//size = last element+1

	map <string, size_t> ::iterator pos = label_list.find(label_str);
	
	if (pos == label_list.end()) // label not found
		
	{
		next_address = current_address; //don't know why :)
		return false;
	}
	else
	{
		next_address = pos->second;
		return true;
	}
	
	
}


bool SourceFile::save_output()

{
	ofstream object(object_str);
	switch (outputmode)
	{
	case '0'://Binary 8 bits per line
		for (unsigned int i = 0; i<instr_list.size(); i++)
		{

			object << instr_list[i]->str_bin();
		}
		break;
	case '1'://1:Hexadicimal 8 bits per line
		for (unsigned int i = 0; i<instr_list.size(); i++)
		{

			object << instr_list[i]->str_hex();
		}
		break;
	case '2'://2:Binary 32 bits per line
		for (unsigned int i = 0; i<instr_list.size(); i++)
		{

			object << instr_list[i]->str_bin_32();
		}
		break;
	case '3'://3:Hexadicimal 32 bits per line
		for (unsigned int i = 0; i<instr_list.size(); i++)
		{

			object << instr_list[i]->str_hex_32();
		}
		break;
	default:
		return false;
		break;
	}

	return true;

}
bool SourceFile::save_output(char omode, const string & obj)
{
	outputmode = omode;
	object_str = obj;
	return save_output();
}

/// <param name="omode">Used to indicate status.</param>
bool SourceFile::save_output(char omode)
{
	outputmode = omode;
	return save_output();
}
bool SourceFile::save_output(const string & obj)
{
	object_str = obj;
	return save_output();
}