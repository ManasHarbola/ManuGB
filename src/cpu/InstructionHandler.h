#pragma once

#include <unordered_map>
#include <functional>
#include <vector>
#include <iterator>
#include "CPU.h"

class Instruction {
public:
    Instruction(uint8_t t_cycles, std::vector<std::function<void(CPU&)> > ops);
    bool finished();
    uint8_t num_t_cycles();
    void execute(CPU &cpu);
private:
    uint8_t t_cycles_;
    //Used for conditional operations that have variable cycle durations
    bool conditional_stop_{false};

    std::vector<std::function<void(CPU&)> >ops_;
    std::vector<std::function<void(CPU&)> >::iterator it_;

    //8-bit load instructions
    //LD r, r'
    static void ld_r1_r2_8(uint8_t &r1, uint8_t r2);
    //LD r, n
    static void ld_r_n(CPU& cpu, uint8_t &r);
    //LD r, (HL)
    static void ld_r_HL(CPU& cpu, uint8_t &r);
    //LD (HL), r
    static void ld_HL_r(CPU& cpu, uint8_t r);
    //LD (HL), n
    static void ld_HL_n_1(CPU& cpu);
    static void ld_HL_n_2(CPU& cpu);
    //LD A, (BC)
    static void ld_A_BC(CPU& cpu);
    //LD A, (DE)
    static void ld_A_DE(CPU& cpu);
    //LD A, (nn)
    static void ld_A_nn_1(CPU &cpu);
    static void ld_A_nn_2(CPU &cpu);
    static void ld_A_nn_3(CPU &cpu);
    //LD (BC), A
    static void ld_BC_A(CPU& cpu);
    //LD (DE), A
    static void ld_DE_A(CPU& cpu);
    //LD (nn), A
    static void ld_nn_A_1(CPU &cpu);
    static void ld_nn_A_2(CPU &cpu);
    static void ld_nn_A_3(CPU &cpu);
    //LD A, (FF00+n)
    static void ld_A_FF00_n_1(CPU& cpu);
    static void ld_A_FF00_n_2(CPU& cpu);
    //LD (FF00+n), A
    static void ld_FF00_n_A_1(CPU &cpu);
    static void ld_FF00_n_A_2(CPU &cpu);
    //LD A, (FF00+C)
    static void ld_A_FF00_C(CPU& cpu);
    //LD (FF00 + C), A
    static void ld_FF00_C_A(CPU& cpu);
    //LDI (HL), A
    static void ldi_HL_A(CPU& cpu);
    //LDI A, (HL)
    static void ldi_A_HL(CPU& cpu);
    //LDD (HL), A
    static void ldd_HL_A(CPU& cpu);
    //LDD A, (HL)
    static void ldd_A_HL(CPU& cpu);

    //16-bit load instructions
    //LD rr, nn
    static void ld_rr_nn_1(CPU &cpu, uint16_t &rr);
    static void ld_rr_nn_2(CPU &cpu, uint16_t &rr);
    //LD (nn), SP
    static void ld_nn_SP_1(CPU& cpu);
    static void ld_nn_SP_2(CPU& cpu);
    static void ld_nn_SP_3(CPU& cpu);
    static void ld_nn_SP_4(CPU& cpu);
    //LD SP, HL
    static void ld_SP_HL(CPU& cpu);
    //LD HL, SP + r8
    static void ld_HL_SP_r8_1(CPU &cpu);
    static void ld_HL_SP_r8_2(CPU &cpu);
    //PUSH rr
    static void push_rr_1(CPU &cpu, uint16_t rr);
    static void push_rr_2(CPU &cpu, uint16_t rr);
    static void push_rr_3(CPU &cpu, uint16_t rr);
    //pop rr
    static void pop_rr_1(CPU &cpu, uint16_t &rr);
    static void pop_rr_2(CPU &cpu, uint16_t &rr);
    

    //8-bit/16-bit arithmetic instruction
    //ADD A, r
    static void add_A_r(CPU &cpu, uint8_t r);
    //ADD A, n
    static void add_A_n(CPU &cpu);
    //ADD A, (HL)
    static void add_A_HL(CPU &cpu);
    //ADD SP, n
    static void add_SP_n_1(CPU &cpu);
    static void add_SP_n_2(CPU &cpu);
    static void add_SP_n_3(CPU &cpu);
    //ADD HL, rr
    static void add_HL_rr(CPU &cpu, uint16_t rr);
    //ADC A, r
    static void adc_A_r(CPU &cpu, uint8_t r);
    //ADC A, n
    static void adc_A_n(CPU &cpu);
    //ADC A, (HL)
    static void adc_A_HL(CPU &cpu);
    //SUB r
    static void sub_r(CPU &cpu, uint8_t r);
    //SUB n
    static void sub_n(CPU &cpu);
    //SUB (HL)
    static void sub_HL(CPU &cpu);
    //SBC A, r
    static void sbc_r(CPU &cpu, uint8_t r);
    //SBC A, n
    static void sbc_n(CPU &cpu);
    //SBC A, (HL)
    static void sbc_A_HL(CPU &cpu);
    //AND r
    static void and_r(CPU &cpu, uint8_t r);
    //AND n
    static void and_n(CPU &cpu);
    //AND (HL)
    static void and_HL(CPU &cpu);
    //XOR r
    static void xor_r(CPU &cpu, uint8_t r);
    //XOR n
    static void xor_n(CPU &cpu);
    //XOR (HL)
    static void xor_HL(CPU &cpu);
    //OR R
    static void or_r(CPU &cpu, uint8_t r);
    //OR n
    static void or_n(CPU &cpu);
    //OR (HL)
    static void or_HL(CPU &cpu);
    //CP r
    static void cp_r(CPU &cpu, uint8_t r);
    //CP n
    static void cp_n(CPU &cpu);
    //CP (HL)
    static void cp_HL(CPU &cpu);
    //INC r
    static void inc_r(CPU &cpu, uint8_t &r);
    //INC rr
    static void inc_rr(CPU &cpu, uint16_t &rr);
    //INC (HL)
    static void inc_HL_1(CPU &cpu);
    static void inc_HL_2(CPU &cpu);
    //DEC r
    static void dec_r(CPU &cpu, uint8_t &r);
    //DEC rr
    static void dec_rr(CPU &cpu, uint16_t &rr);
    //DEC (HL)
    static void dec_HL_1(CPU &cpu);
    static void dec_HL_2(CPU &cpu);
    //DAA
    static void daa(CPU &cpu);
    //CPL
    static void cpl(CPU &cpu);

    //CPU Control Instructions
    //SCF
    static void scf(CPU &cpu);
    //CCF
    static void ccf(CPU &cpu);
    //NOP
    static void nop(CPU &cpu);
    /*
    //HALT
    static void halt(CPU &cpu);
    //STOP
    static void stop(CPU &cpu);
    */
    //EI
    static void ei(CPU &cpu);
    //DI
    static void di(CPU &cpu);

    //Jump Instructions
    //JP nn
    static void jp_nn_1(CPU &cpu);
    static void jp_nn_2(CPU &cpu);
    static void jp_nn_3(CPU &cpu);
    //JP cc nn
    static void jp_cc_nn_1(CPU &cpu);
    void jp_cc_nn_2(CPU &cpu, uint8_t flag, bool not_cc);
    static void jp_cc_nn_3(CPU &cpu);
    //JP HL
    static void jp_hl(CPU &cpu);
    //JR n
    static void jr_n_1(CPU &cpu);
    static void jr_n_2(CPU &cpu);
    //JR cc, n
    void jr_cc_n_1(CPU &cpu, uint8_t flag, bool not_cc);
    static void jr_cc_n_2(CPU &cpu);
    //CALL nn
    static void call_nn_1(CPU &cpu);
    static void call_nn_2(CPU &cpu);
    static void call_nn_3(CPU &cpu);
    static void call_nn_4(CPU &cpu);
    static void call_nn_5(CPU &cpu);
    //CALL cc nn
    static void call_cc_nn_1(CPU &cpu);
    void call_cc_nn_2(CPU &cpu, uint8_t flag, bool not_cc);
    static void call_cc_nn_3(CPU &cpu);
    static void call_cc_nn_4(CPU &cpu);
    static void call_cc_nn_5(CPU &cpu);
    //RET
    static void ret_1(CPU &cpu);
    static void ret_2(CPU &cpu);
    static void ret_3(CPU &cpu);
    //RET cc
    //ret_cc_1 is NOT static on purpose!
    void ret_cc_1(CPU &cpu, uint8_t flag, bool not_cc);   
    static void ret_cc_2(CPU &cpu);
    static void ret_cc_3(CPU &cpu);
    static void ret_cc_4(CPU &cpu);
    //RETI
    static void reti_1(CPU &cpu);
    static void reti_2(CPU &cpu);
    static void reti_3(CPU &cpu);   
    //RST n - where n in {0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38}
    static void rst_1(CPU &cpu, uint8_t n);
    static void rst_2(CPU &cpu, uint8_t n);
    static void rst_3(CPU &cpu, uint8_t n);


    //8-bit shift, rotate and bit instructions
    //BIT n, r
    static void bit_n_r(CPU &cpu, uint8_t bit_pos, uint8_t &r);
    //BIT n, (HL)
    static void bit_n_HL_1(CPU &cpu, uint8_t bit_pos);
    static void bit_n_HL_2(CPU &cpu);
    //SET n, r
    static void set_n_r(uint8_t bit_pos, uint8_t &r);
    //SET n, (HL)
    static void set_n_HL_1(CPU &cpu, uint8_t bit_pos);
    static void set_n_HL_2(CPU &cpu);
    //RES n, r
    static void res_n_r(uint8_t bit_pos, uint8_t &r);
    //RES n, (HL)
    static void res_n_HL_1(CPU &cpu, uint8_t bit_pos);
    static void res_n_HL_2(CPU &cpu);
    //SWAP r
    static void swap_r(CPU &cpu, uint8_t &r);
    //SWAP (HL)
    static void swap_HL_1(CPU &cpu);
    static void swap_HL_2(CPU &cpu);
    //SRL r
    static void srl_r(CPU &cpu, uint8_t &r);
    //SRL (HL)
    static void srl_HL_1(CPU &cpu);
    static void srl_HL_2(CPU &cpu);
    //SLA r
    static void sla_r(CPU &cpu, uint8_t &r);
    //SLA (HL)
    static void sla_HL_1(CPU &cpu);
    static void sla_HL_2(CPU &cpu);
    //SRA r
    static void sra_r(CPU &cpu, uint8_t &r);
    //SRA (HL)
    static void sra_HL_1(CPU &cpu);
    static void sra_HL_2(CPU &cpu);
    //RL r
    static void rl_r(CPU &cpu, uint8_t &r);
    //RL (HL)
    static void rl_HL_1(CPU &cpu);
    static void rl_HL_2(CPU &cpu);
    //RR r
    static void rr_r(CPU &cpu, uint8_t &r);
    //RR (HL)
    static void rr_HL_1(CPU &cpu);
    static void rr_HL_2(CPU &cpu);
    //RLC r
    static void rlc_r(CPU &cpu, uint8_t &r);
    //RLC (HL)
    static void rlc_HL_1(CPU &cpu);
    static void rlc_HL_2(CPU &cpu);
    //RRC r
    static void rrc_r(CPU &cpu, uint8_t &r);
    //RRC (HL)
    static void rrc_HL_1(CPU &cpu);
    static void rrc_HL_2(CPU &cpu);
    //RRC r
    static void rrc_r(CPU &cpu, uint8_t &r);
    //RRC (HL)
    static void rrc_HL_1(CPU &cpu);
    static void rrc_HL_2(CPU &cpu);
    //RLCA
    static void rlca(CPU &cpu);
    //RRCA
    static void rrca(CPU &cpu);
    //RLA
    static void rla(CPU &cpu);
    //RRA
    static void rra(CPU &cpu);
};

class InstructionHandler
{
public:
    static Instruction get_instruction(bool cb_prefixed, uint8_t opcode);
private:
    static const std::unordered_map<uint8_t, Instruction> non_prefixed_ops_;
    static const std::unordered_map<uint8_t, Instruction> cb_prefixed_ops_;
};
