#include "emulador.hpp"

#include <string.h>

#include <fstream>
#include <set>
namespace cpu {
unsigned char acc, stat, pc;		//Registradores
unsigned char memory[256];			//Memoria
unsigned char binary[256] = {0xF};	// = {1,150, 2,1, 1,3, 2,3, 12,10, 8,0, 2,3, 0,1, 7,0, 2,1, 0,3, 14,10, 0,1, 15,0}; //Binario executável
unsigned char opcode, operando;

void fetch_instruction() {
	opcode = binary[pc++] & 0xF;
	operando = binary[pc++];
	//pc++;
}

void load_mem() {
	stat &= 0b001;
	acc = memory[operando];
	//stat &= ~0b1;
}
void load_const() {
	stat &= 0b001;
	acc = operando;
	//stat &= ~0b1;
}

void store() {
	stat &= 0b001;
	memory[operando] = acc;
}

void add() {
	int ac = acc;
	ac += memory[operando];
	stat &= 0b001;
	if (ac >> 8)  //Carry
		stat |= 0b010;
	acc = ac & 0xFF;
}

void sub() {
	int ac = acc;
	ac -= memory[operando];
	stat &= 0b001;
	if (ac < 0)	 //overflow
		stat |= 0b100;
	acc -= memory[operando];
}

void mul() {
	int ac = acc;
	ac *= memory[operando];
	stat &= 0b001;
	if (ac >> 8)  //Overflow
		stat |= 0b100;
	acc *= memory[operando];
}

void div() {
	stat &= 0b001;
	acc /= memory[operando];
}

void inc() {
	stat &= 0b001;
	if (acc == 0xFF)
		stat |= 0b010;
	acc++;
}

void dec() {
	stat &= 0b001;
	if (acc == 0x0)
		stat |= 0b100;
	acc--;
}

void biwiseAnd () {
	stat &= 0b001;
	acc &= memory[operando];
}

void biwiseOr() {
	stat &= 0b001;
	acc |= memory[operando];
}

void biwiseNot() {
	stat &= 0b001;
	acc = ~memory[operando];
}

void jmp() {
	stat &= 0b001;
	pc = operando;
}

void jz() {
	stat &= 0b001;
	if (!(stat & 0b1))
		pc = operando;
}

void jnz() {
	stat &= 0b001;
	if (stat & 0b1)
		pc = operando;
}

int halt = 0;
void hlt() {
	pc -= 2;  //volta pro halt
	halt = 1;
}
void (*run_instruction[])() = {&load_mem, &load_const, &store, &add, &sub, &mul, &div, &inc, &dec, &biwiseAnd, &biwiseOr, &biwiseNot, &jmp, &jz, &jnz, &hlt};
std::string instuction_asm[] = {"load0", "load1", "store", "add", "sub", "mul", "div", "inc", "dec", "and", "or", "not", "jmp", "jz", "jnz", "hlt"};
bool load_program(const std::string & FilePath){
	std::ifstream program(FilePath, std::ifstream::binary);
	if(!program.good()) return 0;
	program.seekg(0, program.end);
	int length = program.tellg();
	program.seekg(0, program.beg);
	if(length > 1 << 8) throw std::length_error("Binary exceed maximum size of 256 bytes");
	program.read((char*)binary ,length);
	return 1;
}
bool load_program(){
	unsigned char binary_def[] = {1, 150, 2, 1, 1, 3, 2, 3, 12, 10, 8, 0, 2, 3, 0, 1, 7, 0, 2, 1, 0, 3, 14, 10, 0, 1, 15, 0};
	memcpy(binary, binary_def, sizeof(binary_def));
	return 1;
}
void reset(){
	acc = stat = pc = 0;
	memset(binary, 0xF, sizeof(binary));
	memset(memory, 0, sizeof(memory));
}
void step(){
	fetch_instruction();
	(run_instruction[opcode])();
	if (acc) {//if acc is not zero
		stat |= 0b1;
	} else {
		stat &= ~0b1;
	}
}
std::set<unsigned char> instructions_not_use_operando = {7, 8, 15};			 //inc, dec, hlt
std::set<unsigned char> instructions_use_memory = {0, 2, 3, 4, 5, 6, 9, 10};	 //load0, store, add, sub, mul, div, and, or
std::set<unsigned char> instructions_use_address = {12, 13, 14};				 //jmp, jz, jnz
disass_instruction disass(unsigned char address) {
	disass_instruction out;
	out.opcode = binary[address] & 0xF;
	out.operando = binary[address+1];
	out.use_operando = !(instructions_not_use_operando.find(out.opcode) != instructions_not_use_operando.end());
	out.use_memory = instructions_use_memory.find(out.opcode) != instructions_use_memory.end();
	out.use_address = instructions_use_address.find(out.opcode) != instructions_use_address.end();
	return out;
}
std::string disassString(unsigned char address) {
	std::string out;
	disass_instruction dissasOut = disass(address);
	out = instuction_asm[dissasOut.opcode % 0x10].c_str();
	if (dissasOut.use_operando) {
		if (dissasOut.use_address) {
			auto diss_ope = cpu::disass(dissasOut.operando);
			out += " $" + std::to_string(dissasOut.operando) + "\t(" + cpu::instuction_asm[diss_ope.opcode % 0x10].c_str() + ")";  //$add (disass(add))
		} else if (dissasOut.use_memory)
			out += " $" + std::to_string(dissasOut.operando) + "\t(" + std::to_string(cpu::memory[dissasOut.operando]) + ")";
		else
			out += " " + std::to_string(dissasOut.operando);
	}
	return out;
}
}  // namespace cpu