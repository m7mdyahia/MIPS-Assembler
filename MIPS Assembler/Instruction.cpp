#include "Instruction.h"
#include "sourcefile.h"

// Testing constructors
#ifdef _DEBUG
Instruction::Instruction(unsigned long l)
{
	bitset<32> binstr(instr_value);
	binstr = bitset<32>(l);
	//op = bitset<6>(binstr.to_string().substr(0, 6));

}

Instruction::Instruction(const bitset<32>& allinstr)
{
	bitset<32> binstr(instr_value);
	binstr = bitset<32>(allinstr);
	//op = bitset<6>(binstr.to_string().substr(0, 6));

}

Instruction::Instruction(const string& str)
{
	bitset<32> binstr(instr_value);
		binstr = bitset<32>(str);
		//op = bitset<6>(binstr.to_string().substr(0, 6));
}

Instruction::Instruction(int opcode_int_par, const string & instr_str_par) :instr_str(instr_str_par), op(opcode_int_par)
{
}

#endif
Instruction::Instruction()
{
	op = 0;

}

//output methods

//newline terminated 8 bits string
string Instruction::str_bin() const
{
	bitset<32> binstr(instr_value);
	string s;//char[33] s
	s = binstr.to_string();
	string res;
	for (int i = 0; i < 32; i+=8)
	{
		res += s.substr(i, 8) + "\n";;
	}
	
	return res;

}

string Instruction::str_hex() const
{
	
	stringstream ss;
	string str;
	ss.width(8);
	ss.fill('0');
	ss << hex << instr_value;
	str = ss.str();
	string res;
	for (int i = 0; i < 8; i+=2)
	{
		res += str.substr(i,2) + "\n";
		
	}
	
	return res;

}

string Instruction::str_bin_32() const
{
	bitset<32> binstr(instr_value);
	return binstr.to_string()+"\n";

}

string Instruction::str_hex_32() const
{

	stringstream ss;
	ss.width(8);
	ss.fill('0');
	ss << hex << instr_value;
	
	return ss.str() + "\n";

}


ostream & operator<<(ostream & os, const Instruction & instr)
{
	os << instr.str_bin();
	return os;
}

 ostream & operator<<(ostream & os, const Instruction * instr_p)
{
	 os << instr_p->str_bin();
	 return os;
}

//Factory
 Instruction* Instruction::make_instr(const string & str,  SourceFile * caller)
{
	istringstream ss(str);
	string opcode_str;
	
	ss >> opcode_str;
	int opcode_int = op_map.at(opcode_str);
	
	try
	{
	
		if (opcode_int == 0) return new RInstr(opcode_str, str,caller);
		else if (opcode_int == 0x2 || opcode_int == 0x3) return new JInstr(opcode_int, str,caller);
		else return new IInstr(opcode_int, str,caller);
	}
	catch (out_of_range& e)
	{
		//TODO 

	}

	//setting the opcode
	
}

//Derived classes' construvtors

RInstr::RInstr(const string & fn_str_par, const string & instr_str_par, SourceFile * caller)
{
	op = 0;
	resolved = true;
	unsigned int rs=0, rt=0, rd=0, shamt=0, fn=0;
	string opcode_, r1, r2, r3,sh;

	stringstream ss(instr_str_par);
	//ss >> opcode_;
	ss.ignore(numeric_limits<streamsize>::max(), ' ');
	try
	{
		fn = fn_map.at(fn_str_par);

		switch (fn)
		{
			//three register case
		case 0x20:
		case 0x21:
		case 0x24:
		case 0x25:
		case 0x27:
		case 0x2a:
		case 0xb:
		case 0x22:
		case 0x23:
		{

					 getline(ss, r1, ',');
					 getline(ss, r2, ',');
					 getline(ss, r3);
					 rs = register_number(r2); rt = register_number(r3); rd = register_number(r1);
					 break;
		}



			//two registers and shift
		case 0x00:
		case 0x02:


			getline(ss, r1, ',');
			getline(ss, r2, ',');
			getline(ss, sh);
			rs = 0; rt = register_number(r2); rd = register_number(r3); shamt = static_cast<int>(stoul(sh, 0, 0)) & 0x1F;
			break;
			//one register //jr
		case 0x08:
			getline(ss, r1);
			rs = register_number(r1);

			break;
		default:
			break;

		}
	}

	catch (out_of_range& e)
	{
		//TODO unkwon function
	}


	instr_value = (op << 26) | (rs << 21) | (rt << 16) | (rd << 11) | (shamt << 6) | fn;
	
}

IInstr::IInstr(int opcode_int_par, const string & instr_str_par,  SourceFile * caller)
{
	op = opcode_int_par;
	unsigned int rs = 0, rt = 0;
	signed int c = 0;
	string  r1, r2, sc;
	stringstream ss(instr_str_par);
	//ss >> opcode_;
	ss.ignore(numeric_limits<streamsize>::max(),' ');

	switch (opcode_int_par)
	{
		//2 registers and constant
	case 0x8:
	case 0x9:
	case 0xc:
	case 0xd:
	case 0xa:
	case 0xb:
	{

		getline(ss, r1, ',');
		getline(ss, r2, ',');
		getline(ss, sc);

		rs = register_number(r2); rt = register_number(r1); c = stoi(sc,0,0);
		resolved = true;
		break;
	}
	case 0xf://lui
	{
		getline(ss, r1, ',');
		getline(ss, sc);
		rs = 0; rt = register_number(r1);  c = stoi(sc, 0, 0);
		resolved = true;
		break;
	}
		//load store lw r1,c(r2)
	case 0x24:
	case 0x25:
	case 0x23:
	case 0x2b:
	case 0x28:
	case 0x29:
	{
		getline(ss, r1, ',');
		getline(ss, sc, '(');
		getline(ss, r2, ')');
		rs = register_number(r2); rt = register_number(r1); c = stoi(sc, 0, 0);
		resolved = true;
		break;
	}
		//beq r1,r2,label
	case 0x4:
	case 0x5:
	{
		getline(ss, r1, ',');
		getline(ss, r2, ',');
		getline(ss, sc);
		
		size_t current_pc, next_pc;
		if (caller->label_address(sc, current_pc, next_pc))
		{
			
			rs = register_number(r1); rt = register_number(r2); c = next_pc - (current_pc + 1);
			resolved = true;
		}
		else
		{
			resolved = false;
		}
		break;
	}
	default:
	
		break;
	}


	
	instr_value = (op << 26) | (rs << 21) | (rt << 16) | (c & 0x0000FFFF) ;
}

JInstr::JInstr(int opcode_int_par, const string & instr_str_par,  SourceFile * caller)
{
	op = opcode_int_par;
	signed int address = 0;
	string  label_str;
	stringstream ss(instr_str_par);
	ss.ignore(numeric_limits<streamsize>::max(), ' ');
	getline(ss, label_str);
	
	size_t current_pc, next_pc;
	if (caller->label_address(label_str, current_pc, next_pc))
	{
		address = next_pc;
		instr_value = (op << 26) | (address & 0x03FFFFFF);
		resolved = true;
	}
	else
	{
		resolved = false;
		instr_value = 0;
	}

	
}

//intializing op code map Using c++11 
const map<string, unsigned char > Instruction::op_map{
	{ "add", 0 },
	{ "addi", 0x8 },
	{ "addiu", 0x9 },
	{ "addu", 0 },
	{ "and", 0 },
	{ "andi", 0xc },
	{ "beq", 0x4 },
	{ "bne", 0x5 },
	{ "j", 0x2 },
	{ "jal", 0x3 },
	{ "jr", 0 },
	{ "lbu", 0x24 },
	{ "lhu", 0x25 },
	{ "ll", 0x30 },
	{ "lui", 0xf },
	{ "lw", 0x23 },
	{ "nor", 0 },
	{ "or", 0 },
	{ "ori", 0xd },
	{ "slt", 0 },
	{ "slti", 0xa },
	{ "sltiu", 0xb },
	{ "sltu", 0 },
	{ "sll", 0 },
	{ "srl", 0 },
	{ "sb", 0x28 },
	{ "sc", 0x38 },
	{ "sh", 0x29 },
	{ "sw", 0x2b },
	{ "sub", 0 },
	{ "subu", 0 }
};

unsigned char Instruction::register_number(string& str)
{


	size_t first = str.find_first_not_of(" \t");
	size_t last = str.find_last_not_of(" \t");
	str = str.substr(first, (last - first + 1));

	if (str[0] != '$') throw(logic_error(""));

	unsigned char r = r_map.at(str.substr(1));
	
	return r;
}

//intializing register map Using c++11 

const map<string,unsigned char > Instruction::r_map{
	{ "zero", 0 },
	{ "0", 0 },
	{ "at", 1 },
	{ "1", 1 },
	{ "v0", 2 },
	{ "2", 2 },
	{ "v1", 3 },
	{ "3", 3 },
	{ "a0", 4 },
	{ "4", 4 },
	{ "a1", 5 },
	{ "5", 5 },
	{ "a2", 6 },
	{ "6", 6 },
	{ "a3", 7 },
	{ "7", 7 },
	{ "t0", 8 },
	{ "8", 8 },
	{ "t1", 9 },
	{ "9", 9 },
	{ "t2", 10 },
	{ "10", 10 },
	{ "t3", 11 },
	{ "11", 11 },
	{ "t4", 12 },
	{ "12", 12 },
	{ "t5", 13 },
	{ "13", 13 },
	{ "t6", 14 },
	{ "14", 14 },
	{ "t7", 15 },
	{ "15", 15 },
	{ "s0", 16 },
	{ "16", 16 },
	{ "s1", 17 },
	{ "17", 17 },
	{ "s2", 18 },
	{ "18", 18 },
	{ "s3", 19 },
	{ "19", 19 },
	{ "s4", 20 },
	{ "20", 20 },
	{ "s5", 21 },
	{ "21", 21 },
	{ "s6", 22 },
	{ "22", 22 },
	{ "s7", 23 },
	{ "23", 23 },
	{ "t8", 24 },
	{ "24", 24 },
	{ "t9", 25 },
	{ "25", 25 },
	{ "k0", 26 },
	{ "26", 26 },
	{ "k1", 27 },
	{ "27", 27 },
	{ "gp", 28 },
	{ "28", 28 },
	{ "sp", 29 },
	{ "29", 29 },
	{ "fp", 30 },
	{ "30", 30 },
	{ "ra", 30 },
	{ "31", 30 }
};


const map<string, unsigned char > Instruction::fn_map{
	{ "add", 0x20 },
	{ "addu", 0x21 },
	{ "and", 0x24 },
	{ "or", 0x25 },
	{ "nor", 0x27 },
	{ "slt", 0x2a },
	{ "sltu", 0xb },
	{ "sub", 0x22 },
	{ "subu", 0x23 },

	{ "sll", 0x00 },
	{ "srl", 0x02 },

	{ "jr", 0x08 }
};

 string& Instruction::trim(string& str)
{
	size_t first = str.find_first_not_of(" \t");
	size_t last = str.find_last_not_of(" \t");
	str = str.substr(first, (last - first + 1));
	return str;
}
