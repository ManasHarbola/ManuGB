#include "InstructionHandler.h"

Instruction::Instruction(uint8_t t_cycles, std::vector<std::function<void (CPU&)> > ops) :
t_cycles_(t_cycles), ops_(ops), it_(ops_.begin()) {

}

bool Instruction::finished() {return (it_ == ops_.end()) || conditional_stop_;}
uint8_t Instruction::num_t_cycles() {return t_cycles_;}
void Instruction::execute(CPU &cpu) {
    if (!finished()) {
        (*it_)(cpu);
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
void Instruction::ld_HL_n_1(CPU& cpu) {
    //cpu.mmu_.write(cpu.registers_.hl, cpu.mmu_.read(cpu.registers_.pc++));
    cpu.n_ = cpu.mmu_.read(cpu.registers_.pc++);
}
void Instruction::ld_HL_n_2(CPU& cpu) {
    cpu.mmu_.write(cpu.registers_.hl, cpu.n_);
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
void Instruction::ld_A_nn_1(CPU &cpu) {
    cpu.lsb_ = cpu.mmu_.read(cpu.registers_.pc++);
}
void Instruction::ld_A_nn_2(CPU &cpu) {
    cpu.msb_ = cpu.mmu_.read(cpu.registers_.pc++);
}
void Instruction::ld_A_nn_3(CPU &cpu) {
    cpu.nn_ = (((uint16_t) cpu.msb_) << 8) | ((uint16_t) cpu.lsb_);
    cpu.registers_.a = cpu.nn_;
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
void Instruction::ld_nn_A_1(CPU &cpu) {
    cpu.lsb_ = cpu.mmu_.read(cpu.registers_.pc++);
}
void Instruction::ld_nn_A_2(CPU &cpu) {
    cpu.msb_ = cpu.mmu_.read(cpu.registers_.pc++);
}
void Instruction::ld_nn_A_3(CPU &cpu) {
    cpu.nn_ = (((uint16_t) cpu.msb_) << 8) | ((uint16_t) cpu.lsb_);
    cpu.mmu_.write(cpu.nn_, cpu.registers_.a);
}
//LD A, (FF00+n)
void Instruction::ld_A_FF00_n_1(CPU &cpu) {
    cpu.n_ = cpu.mmu_.read(cpu.registers_.pc++);
}
void Instruction::ld_A_FF00_n_2(CPU &cpu) {
    cpu.registers_.a = cpu.mmu_.read(0xFF00 | ((uint16_t) cpu.n_));
}
//LD (FF00+n), A
void Instruction::ld_A_FF00_n_1(CPU &cpu) {
    cpu.n_ = cpu.mmu_.read(cpu.registers_.pc++);
}
void Instruction::ld_A_FF00_n_2(CPU &cpu) {
    cpu.mmu_.write(0xFF00 | ((uint16_t) cpu.n_), cpu.registers_.a);
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
void Instruction::ld_rr_nn_1(CPU &cpu, uint16_t &rr) {
    cpu.lsb_ = cpu.mmu_.read(cpu.registers_.pc++);
}
void Instruction::ld_rr_nn_2(CPU &cpu, uint16_t &rr) {
    cpu.msb_ = cpu.mmu_.read(cpu.registers_.pc++);
    rr = (((uint16_t) cpu.msb_) << 8) | ((uint16_t) cpu.lsb_);
}

//LD (nn), SP
void Instruction::ld_nn_SP_1(CPU &cpu) {
    cpu.lsb_ = cpu.mmu_.read(cpu.registers_.pc++);
}
void Instruction::ld_nn_SP_2(CPU &cpu) {
    cpu.msb_ = cpu.mmu_.read(cpu.registers_.pc++);
}
void Instruction::ld_nn_SP_3(CPU &cpu) {
    cpu.nn_ = (((uint16_t) cpu.msb_) << 8) | ((uint16_t) cpu.lsb_);
    cpu.mmu_.write(cpu.nn_, (uint8_t)(cpu.registers_.sp & 0xFF));
}
void Instruction::ld_nn_SP_4(CPU &cpu) {
    cpu.mmu_.write(cpu.nn_ + 1, (uint8_t)(cpu.registers_.sp >> 8));
}

//LD SP, HL
void Instruction::ld_SP_HL(CPU& cpu) {
    cpu.registers_.sp = cpu.registers_.hl;
}

//LD HL, SP + r8
/*
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
*/

void Instruction::ld_HL_SP_r8_1(CPU &cpu) {
    cpu.n_ = cpu.mmu_.read(cpu.registers_.pc++);
}
void Instruction::ld_HL_SP_r8_2(CPU &cpu) {
    int8_t r8 = (int8_t) cpu.n_;
    uint16_t sum = cpu.registers_.sp + r8;

    cpu.registers_.f &= ~ZERO;
    cpu.registers_.f &= ~SUB;

    if (((cpu.registers_.sp ^ r8 ^ sum) & 0x100) == 0x100)
        cpu.registers_.f |= CARRY;
    if (((cpu.registers_.sp ^ r8 ^ sum) & 0x10) == 0x10)
        cpu.registers_.f |= HALF_CARRY;
    cpu.registers_.hl = sum;
}

//PUSH rr
void Instruction::push_rr_1(CPU &cpu, uint16_t rr) {
    //do nothing
    return;
}
void Instruction::push_rr_2(CPU &cpu, uint16_t rr) {
    cpu.mmu_.write(--cpu.registers_.sp, (uint8_t) (rr >> 8));
   
}
void Instruction::push_rr_3(CPU &cpu, uint16_t rr) {
    cpu.mmu_.write(--cpu.registers_.sp, (uint8_t) (rr & 0xFF));
}

//pop rr
void Instruction::pop_rr_1(CPU &cpu, uint16_t &rr) {
    cpu.lsb_ = cpu.mmu_.read(cpu.registers_.sp++);
}
void Instruction::pop_rr_2(CPU &cpu, uint16_t &rr) {
    cpu.msb_ = cpu.mmu_.read(cpu.registers_.sp++);
    rr = (((uint16_t) cpu.msb_) << 8) | ((uint16_t) cpu.lsb_);
    //last four bits of af must be 0 if rr is af
    if (&rr == &cpu.registers_.af)
        cpu.registers_.af &= 0xFFF0;
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
    add_A_r(cpu, cpu.mmu_.read(cpu.registers_.hl));
}
//ADD SP, n
void Instruction::add_SP_n_1(CPU &cpu) {
    cpu.n_ = cpu.mmu_.read(cpu.registers_.pc++);
}
void Instruction::add_SP_n_2(CPU &cpu) {
    cpu.res_ = cpu.registers_.sp + ((int8_t) cpu.n_);
    cpu.registers_.f &= ~(ZERO | SUB);
    cpu.registers_.f = ((cpu.res_ & 0x0F) < (cpu.registers_.sp & 0x0F)) ? (cpu.registers_.f | HALF_CARRY) : (cpu.registers_.f & ~HALF_CARRY);
    cpu.registers_.f = ((cpu.res_ & 0xFF) < (cpu.registers_.sp & 0xFF)) ? (cpu.registers_.f | CARRY)  : (cpu.registers_.f & ~CARRY);
}
void Instruction::add_SP_n_3(CPU &cpu) {
    cpu.registers_.sp = cpu.res_;
}

//ADD HL, rr
void Instruction::add_HL_rr(CPU &cpu, uint16_t rr) {
    int res = cpu.registers_.hl + rr;
    cpu.registers_.f &= ~(SUB);
    if ((cpu.registers_.hl & 0x0FFF) + (rr & 0x0FFF) > 0x0FFF) {
        cpu.registers_.f |= HALF_CARRY;
    } else {
        cpu.registers_.f &= ~HALF_CARRY;
    }
    cpu.registers_.f = ((res >> 16) != 0) ? (cpu.registers_.f | CARRY) : (cpu.registers_.f & ~CARRY);
    cpu.registers_.hl = (uint16_t)(res & 0xFFFF);
}
//ADC A, r
void Instruction::adc_A_r(CPU &cpu, uint8_t r){
    uint8_t carry = (cpu.registers_.f & CARRY) ? 1 : 0;
    uint16_t result = cpu.registers_.a + r + carry;

    cpu.registers_.f &= ~SUB;
    cpu.registers_.f = (result & 0xFF00) ? (cpu.registers_.f | CARRY) : (cpu.registers_.f & ~CARRY);
    cpu.registers_.f = (((cpu.registers_.a & 0x0F) + (r & 0x0F) + carry) > 0x0F) ? (cpu.registers_.f | HALF_CARRY) : (cpu.registers_.f & ~HALF_CARRY);
    cpu.registers_.a = result & 0xFF;
    cpu.registers_.f = (cpu.registers_.a == 0) ? (cpu.registers_.f | ZERO) : (cpu.registers_.f & ~ZERO);
}

//ADC A, n
void Instruction::adc_A_n(CPU &cpu){
    adc_A_r(cpu, cpu.mmu_.read(cpu.registers_.pc++));
}
//ADC A, (HL)
void Instruction::adc_A_HL(CPU &cpu){
    adc_A_r(cpu, cpu.mmu_.read(cpu.registers_.hl));
}
//SUB r
void Instruction::sub_r(CPU &cpu, uint8_t r){
    uint16_t res = cpu.registers_.a - r;
    cpu.registers_.f = (res == 0) ? (cpu.registers_.f | ZERO) : (cpu.registers_.f & ~ZERO);
    cpu.registers_.f |= SUB;
    cpu.registers_.f = ((r & 0x0F) > (cpu.registers_.a & 0x0F)) ? (cpu.registers_.f | HALF_CARRY) : (cpu.registers_.f & ~HALF_CARRY);
    cpu.registers_.f = (res >> 8 != 0) ? (cpu.registers_.f | CARRY) : (cpu.registers_.f & ~CARRY);
    cpu.registers_.a = (uint8_t)(res & 0x00FF);
}
//SUB n
void Instruction::sub_n(CPU &cpu){
    sub_r(cpu, cpu.mmu_.read(cpu.registers_.pc++));
}
//SUB (HL)
void Instruction::sub_HL(CPU &cpu){
    sub_r(cpu, cpu.mmu_.read(cpu.registers_.hl));
}
//SBC A, r
void Instruction::sbc_r(CPU &cpu, uint8_t r) {
    uint8_t carry = (cpu.registers_.f & CARRY) ? 1 : 0;
    uint16_t res = cpu.registers_.a - r - carry;
    cpu.registers_.f = (res == 0) ? (cpu.registers_.f | ZERO) : (cpu.registers_.f | ~ZERO);
    cpu.registers_.f |= SUB;
    if (((cpu.registers_.a ^ r ^ (res & 0xFF)) & (1 << 4)) != 0) {
        cpu.registers_.h |= HALF_CARRY;
    } else {
        cpu.registers_.h &= ~HALF_CARRY;
    }
    cpu.registers_.f = (res < 0) ? (cpu.registers_.f | CARRY) : (cpu.registers_.f & ~CARRY);
    cpu.registers_.a = (uint8_t) (res & 0xFF);
}
//SBC A, n
void Instruction::sbc_n(CPU &cpu){
    sbc_r(cpu, cpu.mmu_.read(cpu.registers_.pc++));
}
//SBC A, (HL)
void Instruction::sbc_A_HL(CPU &cpu){
    sbc_r(cpu, cpu.mmu_.read(cpu.registers_.hl));
}
//AND r
void Instruction::and_r(CPU &cpu, uint8_t r){
    cpu.registers_.a &= r;
    cpu.registers_.f = (cpu.registers_.a == 0) ? (cpu.registers_.f | ZERO) : (cpu.registers_.f & ~ZERO);
    cpu.registers_.f &= (~SUB);
    cpu.registers_.f |= HALF_CARRY;
    cpu.registers_.f &= (~CARRY);
}
//AND n
void Instruction::and_n(CPU &cpu){
    and_r(cpu, cpu.mmu_.read(cpu.registers_.pc++));
}
//AND (HL)
void Instruction::and_HL(CPU &cpu){
    and_r(cpu, cpu.mmu_.read(cpu.registers_.hl));
}
//XOR r
void Instruction::xor_r(CPU &cpu, uint8_t r){
    cpu.registers_.a ^= r;
    cpu.registers_.f = (cpu.registers_.a == 0) ? (cpu.registers_.f | ZERO) : (cpu.registers_.f & ~ZERO);
    cpu.registers_.f &= ~(SUB | HALF_CARRY | CARRY);
}
//XOR n
void Instruction::xor_n(CPU &cpu){
    xor_r(cpu, cpu.mmu_.read(cpu.registers_.pc++));
}
//XOR (HL)
void Instruction::xor_HL(CPU &cpu){
    xor_r(cpu, cpu.mmu_.read(cpu.registers_.hl));
}
//OR R
void Instruction::or_r(CPU &cpu, uint8_t r){
    cpu.registers_.a |= r;
    cpu.registers_.f = (cpu.registers_.a == 0) ? (cpu.registers_.f | ZERO) : (cpu.registers_.f & ~ZERO);
    cpu.registers_.f &= ~(SUB | HALF_CARRY | CARRY);
}
//OR n
void Instruction::or_n(CPU &cpu){
    or_r(cpu, cpu.mmu_.read(cpu.registers_.pc++));
}
//OR (HL)
void Instruction::or_HL(CPU &cpu){
    or_r(cpu, cpu.mmu_.read(cpu.registers_.hl));
}
//CP r
void Instruction::cp_r(CPU &cpu, uint8_t r){
    uint16_t res = cpu.registers_.a - r;
    cpu.registers_.f = (res == 0) ? (cpu.registers_.f | ZERO) : (cpu.registers_.f & ~ZERO);
    cpu.registers_.f |= SUB;
    cpu.registers_.f = ((r & 0x0F) > (cpu.registers_.a & 0x0F)) ? (cpu.registers_.f | HALF_CARRY) : (cpu.registers_.f & ~HALF_CARRY);
    cpu.registers_.f = (res >> 8 != 0) ? (cpu.registers_.f | CARRY) : (cpu.registers_.f & ~CARRY);
}
//CP n
void Instruction::cp_n(CPU &cpu){
    cp_r(cpu, cpu.mmu_.read(cpu.registers_.pc++));
}
//CP (HL)
void Instruction::cp_HL(CPU &cpu){
    cp_r(cpu, cpu.mmu_.read(cpu.registers_.hl));
}
//INC r
void Instruction::inc_r(CPU &cpu, uint8_t &r){
    cpu.registers_.f &= (~SUB);
    cpu.registers_.f = ((r & 0x0F) == 0x0F) ? (cpu.registers_.f | HALF_CARRY) : (cpu.registers_.f & ~HALF_CARRY);
    r++;
    cpu.registers_.f = (r == 0) ? (cpu.registers_.f | ZERO) : (cpu.registers_.f & ~ZERO);
}
//INC rr
void Instruction::inc_rr(CPU &cpu, uint16_t &rr) {
    rr++;
}
//INC (HL)
void Instruction::inc_HL_1(CPU &cpu) {
    cpu.n_ = cpu.mmu_.read(cpu.registers_.hl); 
}
void Instruction::inc_HL_2(CPU &cpu) {
    cpu.registers_.f = ((cpu.n_ & 0x0F) == 0x0F) ? (cpu.registers_.f | HALF_CARRY) : (cpu.registers_.f & ~HALF_CARRY);
    cpu.n_++;
    cpu.registers_.f = (cpu.n_ == 0) ? (cpu.registers_.f | ZERO) : (cpu.registers_.f & ~ZERO);
    cpu.mmu_.write(cpu.registers_.hl, cpu.n_);
}
//DEC r
void Instruction::dec_r(CPU &cpu, uint8_t &r) {
    cpu.registers_.f |= SUB;
    cpu.registers_.f = ((r & 0x0F) == 0x00) ? (cpu.registers_.f | HALF_CARRY) : (cpu.registers_.f & ~HALF_CARRY);
    r--;
    cpu.registers_.f = (r == 0) ? (cpu.registers_.f | ZERO) : (cpu.registers_.f & ~ZERO);
}
//DEC rr
void Instruction::dec_rr(CPU &cpu, uint16_t &rr) {
    rr--;
}
//DEC (HL)
void Instruction::dec_HL_1(CPU &cpu){
    cpu.n_ = cpu.mmu_.read(cpu.registers_.hl); 
}
void Instruction::dec_HL_2(CPU &cpu) {
    cpu.registers_.f |= SUB;
    cpu.registers_.f = ((cpu.n_ & 0x0F) == 0x00) ? (cpu.registers_.f | HALF_CARRY) : (cpu.registers_.f & ~HALF_CARRY);
    cpu.n_--;
    cpu.registers_.f = (cpu.n_ == 0) ? (cpu.registers_.f | ZERO) : (cpu.registers_.f & ~ZERO);
    cpu.mmu_.write(cpu.registers_.hl, cpu.n_);
}
//DAA
void Instruction::daa(CPU &cpu){
    int a = cpu.registers_.a;
    if (cpu.registers_.f & SUB) {
        if (cpu.registers_.f & HALF_CARRY)
            a = (a - 6) & 0xFF;
        if (cpu.registers_.f & CARRY)
            a -= 0x60;
    } else {
        if ((cpu.registers_.f & HALF_CARRY) || ((a & 0xF) > 9))
            a += 0x06;
        if ((cpu.registers_.f & CARRY) || (a > 0x9F))
            a += 0x60;
    }
    cpu.registers_.f &= ~(HALF_CARRY | ZERO);
    if ((a & 0x100) == 0x100)
        cpu.registers_.f |= CARRY;
    a &= 0xFF;
    if (a == 0)
        cpu.registers_.f |= ZERO;
    cpu.registers_.a = (uint8_t) a;
}
//CPL
void Instruction::cpl(CPU &cpu){
    cpu.registers_.a = ~cpu.registers_.a;
    cpu.registers_.f |= (SUB | HALF_CARRY);
}
//SCF
void Instruction::scf(CPU &cpu) {
    cpu.registers_.f &= ~(SUB | HALF_CARRY);
    cpu.registers_.f |= CARRY;
}
//CCF
void Instruction::ccf(CPU &cpu) {
    cpu.registers_.f &= ~(SUB | HALF_CARRY);
    cpu.registers_.f ^= CARRY;
}
//NOP
void Instruction::nop(CPU &cpu) {
    //do nothing
}
/*
//HALT
void Instruction::halt(CPU &cpu) {
    //do nothing because we handle this in CPU.cpp
}
//STOP
void Instruction::stop(CPU &cpu) {
    cpu.state_ = STOP;
    //when gameboy resumes, the opcode following the 0x10 opcode for STOP is ignored
    cpu.registers_.pc++;
}
*/
//EI
void Instruction::ei(CPU &cpu) {
    cpu.int_manager_.enable_IME(true);
}
//DI
void Instruction::di(CPU &cpu) {
    cpu.int_manager_.disable_IME();
}
//JP nn
void Instruction::jp_nn_1(CPU &cpu) {
    cpu.lsb_ = cpu.mmu_.read(cpu.registers_.pc++);
}
void Instruction::jp_nn_2(CPU &cpu) {
    cpu.msb_ = cpu.mmu_.read(cpu.registers_.pc++);
}
void Instruction::jp_nn_3(CPU &cpu) {
    cpu.registers_.pc = (((uint16_t) cpu.msb_) << 8) | ((uint16_t) cpu.lsb_);
}

//JP cc nn
void Instruction::jp_cc_nn_1(CPU &cpu) {
    jp_nn_1(cpu);
}
void Instruction::jp_cc_nn_2(CPU &cpu, uint8_t flag, bool not_cc) {
    jp_nn_2(cpu);
    if (not_cc) {
        if ((cpu.registers_.f & flag) != 0) {
            //instruction 2 won't run
            conditional_stop_ = true;
        }
    } else {
        if ((cpu.registers_.f & flag) == 0) {
            //instruction 2 won't run
            conditional_stop_ = true;
        }
    }
}
void Instruction::jp_cc_nn_3(CPU &cpu) {
    jp_nn_3(cpu);
}

//JP HL
void Instruction::jp_hl(CPU &cpu) {
    cpu.registers_.pc = cpu.registers_.hl;
}

//JR n
void Instruction::jr_n_1(CPU &cpu) {
    cpu.n_ = cpu.mmu_.read(cpu.registers_.pc++);
}
void Instruction::jr_n_2(CPU &cpu) {
    cpu.registers_.pc += ((int8_t) cpu.n_);
}

//JR cc nn
void Instruction::jr_cc_n_1(CPU &cpu, uint8_t flag, bool not_cc) {
    cpu.n_ = cpu.mmu_.read(cpu.registers_.pc++);
    if (not_cc) {
        if ((cpu.registers_.f & flag) != 0) {
            //instruction 2 won't run
            conditional_stop_ = true;
        }
    } else {
        if ((cpu.registers_.f & flag) == 0) {
            //instruction 2 won't run
            conditional_stop_ = true;
        }
    }
}
void Instruction::jr_cc_n_2(CPU &cpu) {
    cpu.registers_.pc += ((int8_t) cpu.n_);
}
//CALL nn
void Instruction::call_nn_1(CPU &cpu) {
    cpu.lsb_ = cpu.mmu_.read(cpu.registers_.pc++);
}
void Instruction::call_nn_2(CPU &cpu) {
    cpu.msb_ = cpu.mmu_.read(cpu.registers_.pc++);
}
void Instruction::call_nn_3(CPU &cpu) {
    cpu.registers_.sp--;
}
void Instruction::call_nn_4(CPU &cpu) {
    cpu.mmu_.write(cpu.registers_.sp--, (uint8_t)(cpu.registers_.pc >> 8));
}
void Instruction::call_nn_5(CPU &cpu) {
    cpu.mmu_.write(cpu.registers_.sp, (uint8_t)(cpu.registers_.pc & 0xFF));
    cpu.registers_.pc = (((uint16_t) cpu.msb_) << 8) | ((uint16_t) cpu.lsb_);
}

//CALL cc nn
void Instruction::call_cc_nn_1(CPU &cpu) {
    call_nn_1(cpu);
}
void Instruction::call_cc_nn_2(CPU &cpu, uint8_t flag, bool not_cc) {
    call_nn_2(cpu);
    if (not_cc) {
        if ((cpu.registers_.f & flag) != 0) {
            //following instructions won't run
            conditional_stop_ = true;
        }
    } else {
        if ((cpu.registers_.f & flag) == 0) {
            //following instructions won't run
            conditional_stop_ = true;
        }
    }
}
void Instruction::call_cc_nn_3(CPU &cpu) {
    call_nn_3(cpu);
}
void Instruction::call_cc_nn_4(CPU &cpu) {
    call_nn_4(cpu);
}
void Instruction::call_cc_nn_5(CPU &cpu) {
    call_nn_5(cpu);
}


//RET
void Instruction::ret_1(CPU &cpu) {
    cpu.lsb_ = cpu.mmu_.read(cpu.registers_.sp++);
}
void Instruction::ret_2(CPU &cpu) {
    cpu.msb_ = cpu.mmu_.read(cpu.registers_.sp++);
}
void Instruction::ret_3(CPU &cpu) {
    cpu.registers_.pc = (((uint16_t) cpu.msb_) << 8) | ((uint16_t) cpu.lsb_);
}

//RET cc
void Instruction::ret_cc_1(CPU &cpu, uint8_t flag, bool not_cc) {
    if (not_cc) {
        if ((cpu.registers_.f & flag) != 0) {
            //instructions 2,3,4 won't run
            conditional_stop_ = true;
        }
    } else {
        if ((cpu.registers_.f & flag) == 0) {
            conditional_stop_ = true;
        }
    }
}
void Instruction::ret_cc_2(CPU &cpu) {
    cpu.lsb_ = cpu.mmu_.read(cpu.registers_.sp++);
}
void Instruction::ret_cc_3(CPU &cpu) {
    cpu.msb_ = cpu.mmu_.read(cpu.registers_.sp++);
}
void Instruction::ret_cc_4(CPU &cpu) {
    cpu.registers_.pc = (((uint16_t) cpu.msb_) << 8) | ((uint16_t) cpu.lsb_);
}

//RETI
void Instruction::reti_1(CPU &cpu) {
    cpu.lsb_ = cpu.mmu_.read(cpu.registers_.sp++);
}
void Instruction::reti_2(CPU &cpu) {
    cpu.msb_ = cpu.mmu_.read(cpu.registers_.sp++);
}
void Instruction::reti_3(CPU &cpu) {
    cpu.registers_.pc = (((uint16_t) cpu.msb_) << 8) | ((uint16_t) cpu.lsb_);
    cpu.int_manager_.enable_IME();
}

//RST n
void Instruction::rst_1(CPU &cpu, uint8_t n) {
    cpu.registers_.sp--;
}
void Instruction::rst_2(CPU &cpu, uint8_t n) {
    cpu.mmu_.write(cpu.registers_.sp--, (uint8_t)(cpu.registers_.pc >> 8));
    cpu.mmu_.write(cpu.registers_.sp, (uint8_t)(cpu.registers_.pc & 0xFF));
}
void Instruction::rst_3(CPU &cpu, uint8_t n) {
    cpu.registers_.pc = (uint16_t) n;
}
//BIT n, r
void Instruction::bit_n_r(CPU &cpu, uint8_t bit_pos, uint8_t &r) {
    cpu.registers_.f = ((r & (1 << bit_pos)) == 0) ? (cpu.registers_.f | ZERO) : (cpu.registers_.f & ~ZERO);
    cpu.registers_.f &= ~(SUB);
    cpu.registers_.f |= HALF_CARRY;
}
//BIT n, (HL)
void Instruction::bit_n_HL_1(CPU &cpu, uint8_t bit_pos) {
    cpu.n_ = cpu.mmu_.read(cpu.registers_.hl) & (1 << bit_pos);
}
void Instruction::bit_n_HL_2(CPU &cpu) {
    cpu.registers_.f = (cpu.n_ == 0) ? (cpu.registers_.f | ZERO) : (cpu.registers_.f & ~ZERO);
    cpu.registers_.f &= ~(SUB);
    cpu.registers_.f |= HALF_CARRY;
}
//SET n, r
void Instruction::set_n_r(uint8_t bit_pos, uint8_t &r) {
    r |= (1 << bit_pos);
}
//SET n, (HL)
void Instruction::set_n_HL_1(CPU &cpu, uint8_t bit_pos) {
    cpu.n_ = cpu.mmu_.read(cpu.registers_.hl) | (1 << bit_pos);
}
void Instruction::set_n_HL_2(CPU &cpu) {
    cpu.mmu_.write(cpu.registers_.hl, cpu.mmu_.read(cpu.n_));
}
//RES n, r
void Instruction::res_n_r(uint8_t bit_pos, uint8_t &r) {
    r &= ~(1 << bit_pos);
}
//RES n, (HL)
void Instruction::res_n_HL_1(CPU &cpu, uint8_t bit_pos) {
    cpu.n_ = cpu.mmu_.read(cpu.registers_.hl) & ~(1 << bit_pos);
}
void Instruction::res_n_HL_2(CPU &cpu) {
    cpu.mmu_.write(cpu.registers_.hl, cpu.mmu_.read(cpu.n_));
}
//SWAP r
void Instruction::swap_r(CPU &cpu, uint8_t &r) {
    r = (r << 8) | (r >> 8);
    cpu.registers_.f = 0;
    if (r == 0)
        cpu.registers_.f |= ZERO;
}
//SWAP (HL)
void Instruction::swap_HL_1(CPU &cpu) {
    cpu.n_ = cpu.mmu_.read(cpu.registers_.hl);
}
void Instruction::swap_HL_2(CPU &cpu) {
    swap_r(cpu, cpu.n_);
    cpu.mmu_.write(cpu.registers_.hl, cpu.n_);
}
//SRL r
void Instruction::srl_r(CPU &cpu, uint8_t &r) {
    cpu.registers_.f = 0;
    cpu.registers_.f |= (r & 0x1) ? CARRY : 0;
    r >>= 1;
    cpu.registers_.f |= (r == 0) ? ZERO : 0;   
}
//SRL (HL)
void Instruction::srl_HL_1(CPU &cpu) {
    cpu.n_ = cpu.mmu_.read(cpu.registers_.hl);
}
void Instruction::srl_HL_2(CPU &cpu) {
    srl_r(cpu, cpu.n_);
    cpu.mmu_.write(cpu.registers_.hl, cpu.n_);
}
//SLA r
void Instruction::sla_r(CPU &cpu, uint8_t &r) {
    cpu.registers_.f = 0;
    cpu.registers_.f |= (r & 0x80) ? CARRY : 0;
    r <<= 1;
    cpu.registers_.f |= (r == 0) ? ZERO : 0;
}
//SLA (HL)
void Instruction::sla_HL_1(CPU &cpu) {
    cpu.n_ = cpu.mmu_.read(cpu.registers_.hl);
}
void Instruction::sla_HL_2(CPU &cpu) {
    sla_r(cpu, cpu.n_);
    cpu.mmu_.write(cpu.registers_.hl, cpu.n_);
}
//SRA r
void Instruction::sra_r(CPU &cpu, uint8_t &r) {
    cpu.registers_.f = 0;
    cpu.registers_.f |= (r & 0x1) ? CARRY : 0;
    r = (r >> 1) | (r & 0x80);
    cpu.registers_.f |= (r == 0) ? ZERO : 0;
}
//SRA (HL)
void Instruction::sra_HL_1(CPU &cpu) {
    cpu.n_ = cpu.mmu_.read(cpu.registers_.hl);
}
void Instruction::sra_HL_2(CPU &cpu) {
    sra_r(cpu, cpu.n_);
    cpu.mmu_.write(cpu.registers_.hl, cpu.n_);
}
//RL r
void Instruction::rl_r(CPU &cpu, uint8_t &r) {
    bool c = (r & 0x80);
    r = (r << 1) | ((cpu.registers_.f & CARRY) ? 1 : 0);
    cpu.registers_.f = 0;
    cpu.registers_.f |= (r == 0) ? ZERO : 0;
    cpu.registers_.f |= (c) ? CARRY : 0;
}
//RL (HL)
void Instruction::rl_HL_1(CPU &cpu) {
    cpu.n_ = cpu.mmu_.read(cpu.registers_.hl);
}
void Instruction::rl_HL_2(CPU &cpu) {
    rl_r(cpu, cpu.n_);
    cpu.mmu_.write(cpu.registers_.hl, cpu.n_);
}
//RR r
void Instruction::rr_r(CPU &cpu, uint8_t &r) {
    bool c = (r & 1);
    r = (r >> 1) | ((cpu.registers_.f & CARRY) ? (1 << 7) : 0);
    cpu.registers_.f = 0;
    cpu.registers_.f |= (r == 0) ? ZERO : 0;
    cpu.registers_.f |= (c) ? CARRY : 0;
}
//RR (HL)
void Instruction::rr_HL_1(CPU &cpu) {
    cpu.n_ = cpu.mmu_.read(cpu.registers_.hl);
}
void Instruction::rr_HL_2(CPU &cpu) {
    rr_r(cpu, cpu.n_);
    cpu.mmu_.write(cpu.registers_.hl, cpu.n_);
}
//RLC r
void Instruction::rlc_r(CPU &cpu, uint8_t &r) {
    bool c = (r & (1 << 7));
    r = (r << 1) | ((r & 0x80) >> 7);
    cpu.registers_.f = 0;
    cpu.registers_.f |= (r == 0) ? ZERO : 0;
    cpu.registers_.f |= (c) ? CARRY : 0;
}
//RLC (HL)
void Instruction::rlc_HL_1(CPU &cpu) {
    cpu.n_ = cpu.mmu_.read(cpu.registers_.hl);
}
void Instruction::rlc_HL_2(CPU &cpu) {
    rlc_r(cpu, cpu.n_);
    cpu.mmu_.write(cpu.registers_.hl, cpu.n_);
}
//RRC r
void Instruction::rrc_r(CPU &cpu, uint8_t &r) {
    bool c = (r & 1);
    r = (r >> 1) | ((r & 1) << 7);
    cpu.registers_.f = 0;
    cpu.registers_.f |= (r == 0) ? ZERO : 0;
    cpu.registers_.f |= (c) ? CARRY : 0;
}
//RRC (HL)
void Instruction::rrc_HL_1(CPU &cpu) {
    cpu.n_ = cpu.mmu_.read(cpu.registers_.hl);
}
void Instruction::rrc_HL_2(CPU &cpu) {
    rrc_r(cpu, cpu.n_);
    cpu.mmu_.write(cpu.registers_.hl, cpu.n_);
}
//RLCA
void Instruction::rlca(CPU &cpu) {
    rlc_r(cpu, cpu.registers_.a);
    cpu.registers_.f &= ~(ZERO);
}
//RRCA
void Instruction::rrca(CPU &cpu) {
    rrc_r(cpu, cpu.registers_.a);
    cpu.registers_.f &= ~(ZERO);
}
//RLA
void Instruction::rla(CPU &cpu) {
    rl_r(cpu, cpu.registers_.a);
    cpu.registers_.f &= ~(ZERO);
}
//RRA
void Instruction::rra(CPU &cpu) {
    rr_r(cpu, cpu.registers_.a);
    cpu.registers_.f &= ~(ZERO);
}