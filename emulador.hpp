#include <iostream>
namespace cpu{
extern unsigned char acc, stat, pc;
extern unsigned char memory[];
extern unsigned char binary[];
extern std::string instuction_asm[];

struct Stat {
	unsigned char s;
	Stat(unsigned char st) : s(st) {}
	bool overflow() { return s & 0b100; }
	bool carry() { return s & 0b10; }
	bool zero() { return s & 0b1; }
};

bool load_program(const std::string & FilePath);
bool load_program();
void reset();
void step();
struct disass_instruction {
	unsigned char opcode, operando, mem_operando;
	bool use_memory, use_operando, use_address;
};
disass_instruction disass(unsigned char address);
std::string disassString(unsigned char address);
}