#include "InstructionHandler.h"

bool Instruction::finished() {return it_ == ops_.end();}
uint8_t Instruction::num_t_cycles() {return t_cycles_;}
void Instruction::execute(CPU &cpu) {
    if (!finished()) {
        (**it_)(&cpu);
        it_++;
    }
}

Instruction InstructionHandler::get_instruction(bool cb_prefixed, uint8_t opcode) {
    return cb_prefixed ? cb_prefixed_ops_.at(opcode) : non_prefixed_ops_.at(opcode);
}

std::unordered_map<uint8_t, Instruction> InstructionHandler::non_prefixed_ops_ = {
    //define opcode to Instruction key-value pairs here
    //ex. {0x12, Instruction}
};

std::unordered_map<uint8_t, Instruction> InstructionHandler::cb_prefixed_ops_ = {
    //define cb-prefixed opcode to function key-value pairs here
    //ex. {0x12, Instruction}
};

//LD r, r'
void Instruction::ld_r1_r2_8(uint8_t &r1, uint8_t r2) {r1 = r2;}
//LD r, n
void Instruction::ld_r_n(CPU& cpu, uint8_t &r) {
    r = cpu.mmu_.read(cpu.registers_.pc++);
}
//LD r, (HL)
void Instruction::ld_r_HL(CPU& cpu, uint8_t &r) {
    r = cpu.mmu_.read(cpu.registers_.hl);
}
//LD (HL), r
void Instruction::ld_HL_r(CPU& cpu, uint8_t r) {
    cpu.mmu_.write(cpu.registers_.hl, r);
}
//LD (HL), n
void Instruction::ld_HL_n(CPU& cpu) {
    cpu.mmu_.write(cpu.registers_.hl, cpu.mmu_.read(cpu.registers_.pc++));
}
//LD A, (BC)
void Instruction::ld_A_BC(CPU& cpu) {
    cpu.registers_.a = cpu.mmu_.read(cpu.registers_.bc);
}
//LD A, (DE)
void Instruction::ld_A_DE(CPU& cpu) {
    cpu.registers_.a = cpu.mmu_.read(cpu.registers_.de);
}
//LD A, (nn)
void Instruction::ld_A_nn(CPU& cpu) {
    uint8_t lsb = cpu.mmu_.read(cpu.registers_.pc++);
    uint8_t msb = cpu.mmu_.read(cpu.registers_.pc++);
    uint16_t nn = (((uint16_t) msb) << 8) | ((uint16_t) lsb);
    cpu.mmu_.write(nn, (uint8_t) (cpu.registers_.sp & 0x00FF));
    cpu.mmu_.write(nn + 1, (uint8_t)(cpu.registers_.sp & (0xFF00)));
}
//LD (BC), A
void Instruction::ld_BC_A(CPU& cpu) {
    cpu.mmu_.write(cpu.registers_.bc, cpu.registers_.a);
}
//LD (DE), A
void Instruction::ld_DE_A(CPU& cpu) {
    cpu.mmu_.write(cpu.registers_.de, cpu.registers_.a);
}
//LD (nn), A
void Instruction::ld_nn_A(CPU& cpu) {
    uint8_t lsb = cpu.mmu_.read(cpu.registers_.pc++);
    uint8_t msb = cpu.mmu_.read(cpu.registers_.pc++);
    uint16_t nn = (((uint16_t) msb) << 8) | ((uint16_t) lsb);
    cpu.mmu_.write(nn, cpu.registers_.a);
}
//LD A, (FF00+n)
void Instruction::ld_A_FF00_n(CPU& cpu) {
    uint16_t n = (uint16_t) cpu.mmu_.read(cpu.registers_.pc++);
    cpu.registers_.a = cpu.mmu_.read(0xFF00 | n);
}
//LD (FF00+n), A
void Instruction::ld_FF00_n_A(CPU& cpu) {
    uint16_t n = (uint16_t) cpu.mmu_.read(cpu.registers_.pc++);
    cpu.mmu_.write(0xFF00 | n, cpu.registers_.a);
}
//LD A, (FF00+C)
void Instruction::ld_A_FF00_C(CPU& cpu) {
    cpu.registers_.a = cpu.mmu_.read(0xFF00 | ((uint16_t) cpu.registers_.c));
}
//LD (FF00 + C), A
void Instruction::ld_FF00_C_A(CPU& cpu) {
    cpu.mmu_.write(0xFF00 | ((uint16_t) cpu.registers_.c), cpu.registers_.a);
}
//LDI (HL), A
void Instruction::ldi_HL_A(CPU& cpu) {
    cpu.mmu_.write(cpu.registers_.hl++, cpu.registers_.a);
}
//LDI A, (HL)
void Instruction::ldi_A_HL(CPU& cpu) {
    cpu.registers_.a = cpu.mmu_.read(cpu.registers_.hl++);
}
//LDD (HL), A
void Instruction::ldd_HL_A(CPU& cpu) {
    cpu.mmu_.write(cpu.registers_.hl--, cpu.registers_.a);
}
//LDD A, (HL)
void Instruction::ldd_A_HL(CPU& cpu) {
    cpu.registers_.a = cpu.mmu_.read(cpu.registers_.hl--);
}

//16-bit load instructions
//LD rr, nn
void Instruction::ld_rr_nn(CPU& cpu, uint16_t &rr) {
    uint8_t lsb = cpu.mmu_.read(cpu.registers_.pc++);
    uint8_t msb = cpu.mmu_.read(cpu.registers_.pc++);
    uint16_t nn = (((uint16_t) msb) << 8) | ((uint16_t) lsb); 
    rr = nn;
}
//LD (nn), SP
void Instruction::ld_nn_SP(CPU& cpu) {
    uint8_t lsb = cpu.mmu_.read(cpu.registers_.pc++);
    uint8_t msb = cpu.mmu_.read(cpu.registers_.pc++);
    uint16_t nn = (((uint16_t) msb) << 8) | ((uint16_t) lsb);
    cpu.mmu_.write(nn, (uint8_t)(cpu.registers_.sp & 0xFF));
    cpu.mmu_.write(nn + 1, (uint8_t)(cpu.registers_.sp >> 8));
}
//LD SP, HL
void Instruction::ld_SP_HL(CPU& cpu) {
    cpu.registers_.sp = cpu.registers_.hl;
}

//LD HL, SP + r8
void Instruction::ld_HL_SP_r8(CPU& cpu) {
    int8_t r8 = cpu.mmu_.read(cpu.registers_.pc);
    uint16_t sum = cpu.registers_.sp + r8;

    cpu.registers_.f &= ~ZERO;
    cpu.registers_.f &= ~SUB;

    if (((cpu.registers_.sp ^ r8 ^ sum) & 0x100) == 0x100)
        cpu.registers_.f |= CARRY;
    if (((cpu.registers_.sp ^ r8 ^ sum) & 0x10) == 0x10)
        cpu.registers_.f |= HALF_CARRY;
    cpu.registers_.hl = sum;
    cpu.registers_.pc++;
}

//PUSH rr
void Instruction::push_rr(CPU& cpu, uint16_t rr) {
    cpu.mmu_.write(--cpu.registers_.sp, (uint8_t) (rr >> 8));
    cpu.mmu_.write(--cpu.registers_.sp, (uint8_t) (rr & 0xFF));
}
//pop rr
void Instruction::pop_rr(CPU& cpu, uint16_t &rr) {
    uint8_t lsb = cpu.mmu_.read(cpu.registers_.sp++);
    uint8_t msb = cpu.mmu_.read(cpu.registers_.sp++);
    rr = (((uint16_t) msb) << 8) | ((uint16_t) lsb);

    //last four bits of af must be 0 if rr is af
    if (&rr == &cpu.registers_.af) {
        cpu.registers_.af &= 0xFFF0;
    }
}

//ADD A, r
void Instruction::add_A_r(CPU &cpu, uint8_t r){
    cpu.registers_.f &= ~SUB;
    uint8_t res = cpu.registers_.a + r;
    //update zero flag
    if (res == 0)
        cpu.registers_.f |= ZERO;
    //update half carry
    if (((cpu.registers_.a & 0x0F) + (r & 0x0F)) > 0x0F)
        cpu.registers_.f |= HALF_CARRY;
    //update carry
    if (res & 0xFF00)
        cpu.registers_.f |= CARRY;
    cpu.registers_.a = res;
}
//ADD A, n
void Instruction::add_A_n(CPU &cpu){
    add_A_r(cpu, cpu.mmu_.read(cpu.registers_.pc++));
}
//ADD A, (HL)
void Instruction::add_A_HL(CPU &cpu){

}
//ADC A, r
void Instruction::adc_A_r(CPU &cpu, uint8_t r){

}
//ADC A, n
void Instruction::adc_A_n(CPU &cpu){

}
//ADC A, (HL)
void Instruction::adc_A_HL(CPU &cpu){

}
//SUB r
void Instruction::sub_r(CPU &cpu, uint8_t r){

}
//SUB n
void Instruction::sub_n(CPU &cpu){

}
//SUB (HL)
void Instruction::sub_HL(CPU &cpu){

}
//SBC A, r
void Instruction::sbc_r(CPU &cpu, uint8_t r){

}
//SBC A, n
void Instruction::sbc_n(CPU &cpu){

}
//SBC A, (HL)
void Instruction::sbc_A_HL(CPU &cpu){

}
//AND r
void Instruction::and_r(CPU &cpu, uint8_t r){

}
//AND n
void Instruction::and_n(CPU &cpu){

}
//AND (HL)
void Instruction::and_HL(CPU &cpu){

}
//XOR r
void Instruction::xor_r(CPU &cpu, uint8_t r){

}
//XOR n
void Instruction::xor_n(CPU &cpu){

}
//XOR (HL)
void Instruction::xor_HL(CPU &cpu){

}
//OR R
void Instruction::or_r(CPU &cpu, uint8_t r){

}
//OR n
void Instruction::or_n(CPU &cpu){

}
//OR (HL)
void Instruction::or_HL(CPU &cpu){

}
//CP r
void Instruction::cp_r(CPU &cpu, uint8_t r){

}
//CP n
void Instruction::cp_n(CPU &cpu){

}
//CP (HL)
void Instruction::cp_HL(CPU &cpu){

}
//INC r
void Instruction::inc_r(CPU &cpu, uint8_t r){

}
//INC (HL)
void Instruction::inc_HL(CPU &cpu){

}
//DEC r
void Instruction::dec_r(CPU &cpu, uint8_t r){

}
//DEC (HL)
void Instruction::dec_HL(CPU &cpu){

}
//DAA
void Instruction::daa(CPU &cpu){

}
//CPL
void Instruction::cpl(CPU &cpu){

}