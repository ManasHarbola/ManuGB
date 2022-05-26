#pragma once

#include <unordered_map>
#include <memory>
#include <functional>
#include <vector>
#include <iterator>
#include "../Constants.h"
//#include "CPU.h"
class Instruction;
#include "CPU.h"

class Instruction {
public:
    Instruction(uint8_t t_cycles, std::vector<std::function<void(CPU&)> > ops);
    Instruction(const Instruction &inst);
    //virtual ~Instruction();
    //static Instruction get_instruction(bool cb_prefixed, uint8_t opcode);
    static std::unique_ptr<Instruction> get_instruction(bool cb_prefixed, uint8_t opcode);
    //bool finished();
    uint8_t num_t_cycles();
    void execute(CPU &cpu);
private:
    uint8_t t_cycles_;
    //Used for conditional operations that have variable cycle durations
    //bool conditional_stop_{false};

    std::vector<std::function<void(CPU&)> >ops_;
    std::vector<std::function<void(CPU&)> >::iterator it_;
    
    //for retrieving Instruction objects
    static const std::unordered_map<uint8_t, Instruction> non_prefixed_ops_;
    static const std::unordered_map<uint8_t, Instruction> cb_prefixed_ops_;

    //8-bit load instructions
    //LD r, r'
    static void ld_r1_r2(uint8_t &r1, uint8_t r2);
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
    static void jp_cc_nn_2(CPU &cpu, uint8_t flag, bool not_cc);
    static void jp_cc_nn_3(CPU &cpu);
    //JP HL
    static void jp_hl(CPU &cpu);
    //JR n
    static void jr_n_1(CPU &cpu);
    static void jr_n_2(CPU &cpu);
    //JR cc, n
    static void jr_cc_n_1(CPU &cpu, uint8_t flag, bool not_cc);
    static void jr_cc_n_2(CPU &cpu);
    //CALL nn
    static void call_nn_1(CPU &cpu);
    static void call_nn_2(CPU &cpu);
    static void call_nn_3(CPU &cpu);
    static void call_nn_4(CPU &cpu);
    static void call_nn_5(CPU &cpu);
    //CALL cc nn
    static void call_cc_nn_1(CPU &cpu);
    static void call_cc_nn_2(CPU &cpu, uint8_t flag, bool not_cc);
    static void call_cc_nn_3(CPU &cpu);
    static void call_cc_nn_4(CPU &cpu);
    static void call_cc_nn_5(CPU &cpu);
    //RET
    static void ret_1(CPU &cpu);
    static void ret_2(CPU &cpu);
    static void ret_3(CPU &cpu);
    //RET cc
    static void ret_cc_1(CPU &cpu, uint8_t flag, bool not_cc);   
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
    //RLCA
    static void rlca(CPU &cpu);
    //RRCA
    static void rrca(CPU &cpu);
    //RLA
    static void rla(CPU &cpu);
    //RRA
    static void rra(CPU &cpu);




    //0x
    static void ld_bc_nn_1(CPU &cpu);
    static void ld_bc_nn_2(CPU &cpu);
    static void inc_bc(CPU &cpu);
    static void inc_b(CPU &cpu);
    static void dec_b(CPU &cpu);
    static void ld_b_n(CPU &cpu);
    static void add_HL_BC(CPU &cpu);
    static void dec_bc(CPU &cpu);
    static void inc_c(CPU &cpu);
    static void dec_c(CPU &cpu);
    static void ld_c_n(CPU &cpu);
    //1x
    static void ld_de_nn_1(CPU &cpu);
    static void ld_de_nn_2(CPU &cpu);
    static void inc_de(CPU &cpu);
    static void inc_d(CPU &cpu);
    static void dec_d(CPU &cpu);
    static void ld_d_n(CPU &cpu);
    static void add_HL_DE(CPU &cpu);
    static void dec_de(CPU &cpu);
    static void inc_e(CPU &cpu);
    static void dec_e(CPU &cpu);
    static void ld_e_n(CPU &cpu);
    //2x
    static void jr_nz_n_1(CPU &cpu);
    static void jr_nz_n_2(CPU &cpu);
    static void ld_hl_nn_1(CPU &cpu);
    static void ld_hl_nn_2(CPU &cpu);
    static void inc_hl(CPU &cpu);
    static void inc_h(CPU &cpu);
    static void dec_h(CPU &cpu);
    static void ld_h_n(CPU &cpu);
    static void jr_z_n_1(CPU &cpu);
    static void jr_z_n_2(CPU &cpu);
    static void add_HL_HL(CPU &cpu);
    static void dec_hl(CPU &cpu);
    static void inc_l(CPU &cpu);
    static void dec_l(CPU &cpu);
    static void ld_l_n(CPU &cpu);
    //3x
    static void jr_nc_n_1(CPU &cpu);
    static void jr_nc_n_2(CPU &cpu);
    static void ld_sp_nn_1(CPU &cpu);
    static void ld_sp_nn_2(CPU &cpu);
    static void inc_sp(CPU &cpu);
    static void jr_c_n_1(CPU &cpu);
    static void jr_c_n_2(CPU &cpu);
    static void add_HL_SP(CPU &cpu);
    static void dec_sp(CPU &cpu);
    static void inc_a(CPU &cpu);
    static void dec_a(CPU &cpu);
    static void ld_a_n(CPU &cpu);
    //4x
    static void ld_b_b(CPU &cpu);
    static void ld_b_c(CPU &cpu);
    static void ld_b_d(CPU &cpu);
    static void ld_b_e(CPU &cpu);
    static void ld_b_h(CPU &cpu);
    static void ld_b_l(CPU &cpu);
    static void ld_b_HL(CPU &cpu);
    static void ld_b_a(CPU &cpu);
    static void ld_c_b(CPU &cpu);
    static void ld_c_c(CPU &cpu);
    static void ld_c_d(CPU &cpu);
    static void ld_c_e(CPU &cpu);
    static void ld_c_h(CPU &cpu);
    static void ld_c_l(CPU &cpu);
    static void ld_c_HL(CPU &cpu);
    static void ld_c_a(CPU &cpu);
    //5x
    static void ld_d_b(CPU &cpu);
    static void ld_d_c(CPU &cpu);
    static void ld_d_d(CPU &cpu);
    static void ld_d_e(CPU &cpu);
    static void ld_d_h(CPU &cpu);
    static void ld_d_l(CPU &cpu);
    static void ld_d_HL(CPU &cpu);
    static void ld_d_a(CPU &cpu);
    static void ld_e_b(CPU &cpu);
    static void ld_e_c(CPU &cpu);
    static void ld_e_d(CPU &cpu);
    static void ld_e_e(CPU &cpu);
    static void ld_e_h(CPU &cpu);
    static void ld_e_l(CPU &cpu);
    static void ld_e_HL(CPU &cpu);
    static void ld_e_a(CPU &cpu);
    //6x
    static void ld_h_b(CPU &cpu);
    static void ld_h_c(CPU &cpu);
    static void ld_h_d(CPU &cpu);
    static void ld_h_e(CPU &cpu);
    static void ld_h_h(CPU &cpu);
    static void ld_h_l(CPU &cpu);
    static void ld_h_HL(CPU &cpu);
    static void ld_h_a(CPU &cpu);
    static void ld_l_b(CPU &cpu);
    static void ld_l_c(CPU &cpu);
    static void ld_l_d(CPU &cpu);
    static void ld_l_e(CPU &cpu);
    static void ld_l_h(CPU &cpu);
    static void ld_l_l(CPU &cpu);
    static void ld_l_HL(CPU &cpu);
    static void ld_l_a(CPU &cpu);
    //7x
    static void ld_HL_b(CPU &cpu);
    static void ld_HL_c(CPU &cpu);
    static void ld_HL_d(CPU &cpu);
    static void ld_HL_e(CPU &cpu);
    static void ld_HL_h(CPU &cpu);
    static void ld_HL_l(CPU &cpu);
    static void ld_HL_a(CPU &cpu);
    static void ld_a_b(CPU &cpu);
    static void ld_a_c(CPU &cpu);
    static void ld_a_d(CPU &cpu);
    static void ld_a_e(CPU &cpu);
    static void ld_a_h(CPU &cpu);
    static void ld_a_l(CPU &cpu);
    static void ld_a_HL(CPU &cpu);
    static void ld_a_a(CPU &cpu);
    //8x
    static void add_A_b(CPU &cpu);
    static void add_A_c(CPU &cpu);
    static void add_A_d(CPU &cpu);
    static void add_A_e(CPU &cpu);
    static void add_A_h(CPU &cpu);
    static void add_A_l(CPU &cpu);
    static void add_A_a(CPU &cpu);
    static void adc_A_b(CPU &cpu);
    static void adc_A_c(CPU &cpu);
    static void adc_A_d(CPU &cpu);
    static void adc_A_e(CPU &cpu);
    static void adc_A_h(CPU &cpu);
    static void adc_A_l(CPU &cpu);
    static void adc_A_a(CPU &cpu);
    //9x
    static void sub_A_b(CPU &cpu);
    static void sub_A_c(CPU &cpu);
    static void sub_A_d(CPU &cpu);
    static void sub_A_e(CPU &cpu);
    static void sub_A_h(CPU &cpu);
    static void sub_A_l(CPU &cpu);
    static void sub_A_a(CPU &cpu);
    static void sbc_A_b(CPU &cpu);
    static void sbc_A_c(CPU &cpu);
    static void sbc_A_d(CPU &cpu);
    static void sbc_A_e(CPU &cpu);
    static void sbc_A_h(CPU &cpu);
    static void sbc_A_l(CPU &cpu);
    static void sbc_A_a(CPU &cpu);
    //Ax
    static void and_b(CPU &cpu);
    static void and_c(CPU &cpu);
    static void and_d(CPU &cpu);
    static void and_e(CPU &cpu);
    static void and_h(CPU &cpu);
    static void and_l(CPU &cpu);
    static void and_a(CPU &cpu);
    static void xor_b(CPU &cpu);
    static void xor_c(CPU &cpu);
    static void xor_d(CPU &cpu);
    static void xor_e(CPU &cpu);
    static void xor_h(CPU &cpu);
    static void xor_l(CPU &cpu);
    static void xor_a(CPU &cpu);
    //Bx
    static void or_b(CPU &cpu);
    static void or_c(CPU &cpu);
    static void or_d(CPU &cpu);
    static void or_e(CPU &cpu);
    static void or_h(CPU &cpu);
    static void or_l(CPU &cpu);
    static void or_a(CPU &cpu);
    static void cp_b(CPU &cpu);
    static void cp_c(CPU &cpu);
    static void cp_d(CPU &cpu);
    static void cp_e(CPU &cpu);
    static void cp_h(CPU &cpu);
    static void cp_l(CPU &cpu);
    static void cp_a(CPU &cpu);
    //Cx
    static void ret_nz_1(CPU &cpu);
    static void ret_nz_2(CPU &cpu);
    static void ret_nz_3(CPU &cpu);
    static void ret_nz_4(CPU &cpu);
    static void pop_bc_1(CPU &cpu);
    static void pop_bc_2(CPU &cpu);
    static void jp_nz_nn_1(CPU &cpu);
    static void jp_nz_nn_2(CPU &cpu);
    static void jp_nz_nn_3(CPU &cpu);
    static void call_nz_nn_1(CPU &cpu);
    static void call_nz_nn_2(CPU &cpu);
    static void call_nz_nn_3(CPU &cpu);
    static void call_nz_nn_4(CPU &cpu);
    static void call_nz_nn_5(CPU &cpu);
    static void push_bc_1(CPU &cpu);
    static void push_bc_2(CPU &cpu);
    static void push_bc_3(CPU &cpu);
    static void rst_00h_1(CPU &cpu);
    static void rst_00h_2(CPU &cpu);
    static void rst_00h_3(CPU &cpu);
    static void ret_z_1(CPU &cpu);
    static void ret_z_2(CPU &cpu);
    static void ret_z_3(CPU &cpu);
    static void ret_z_4(CPU &cpu);
    static void jp_z_nn_1(CPU &cpu);
    static void jp_z_nn_2(CPU &cpu);
    static void jp_z_nn_3(CPU &cpu);
    static void call_z_nn_1(CPU &cpu);
    static void call_z_nn_2(CPU &cpu);
    static void call_z_nn_3(CPU &cpu);
    static void call_z_nn_4(CPU &cpu);
    static void call_z_nn_5(CPU &cpu);
    static void rst_08h_1(CPU &cpu);
    static void rst_08h_2(CPU &cpu);
    static void rst_08h_3(CPU &cpu);
    //Dx
    static void ret_nc_1(CPU &cpu);
    static void ret_nc_2(CPU &cpu);
    static void ret_nc_3(CPU &cpu);
    static void ret_nc_4(CPU &cpu);
    static void pop_de_1(CPU &cpu);
    static void pop_de_2(CPU &cpu);
    static void jp_nc_nn_1(CPU &cpu);
    static void jp_nc_nn_2(CPU &cpu);
    static void jp_nc_nn_3(CPU &cpu);
    static void call_nc_nn_1(CPU &cpu);
    static void call_nc_nn_2(CPU &cpu);
    static void call_nc_nn_3(CPU &cpu);
    static void call_nc_nn_4(CPU &cpu);
    static void call_nc_nn_5(CPU &cpu);
    static void push_de_1(CPU &cpu);
    static void push_de_2(CPU &cpu);
    static void push_de_3(CPU &cpu);
    static void rst_10h_1(CPU &cpu);
    static void rst_10h_2(CPU &cpu);
    static void rst_10h_3(CPU &cpu);
    static void ret_c_1(CPU &cpu);
    static void ret_c_2(CPU &cpu);
    static void ret_c_3(CPU &cpu);
    static void ret_c_4(CPU &cpu);
    static void jp_c_nn_1(CPU &cpu);
    static void jp_c_nn_2(CPU &cpu);
    static void jp_c_nn_3(CPU &cpu);
    static void call_c_nn_1(CPU &cpu);
    static void call_c_nn_2(CPU &cpu);
    static void call_c_nn_3(CPU &cpu);
    static void call_c_nn_4(CPU &cpu);
    static void call_c_nn_5(CPU &cpu);
    static void rst_18h_1(CPU &cpu);
    static void rst_18h_2(CPU &cpu);
    static void rst_18h_3(CPU &cpu);
    //Ex
    static void pop_hl_1(CPU &cpu);
    static void pop_hl_2(CPU &cpu);
    static void push_hl_1(CPU &cpu);
    static void push_hl_2(CPU &cpu);
    static void push_hl_3(CPU &cpu);
    static void rst_20h_1(CPU &cpu);
    static void rst_20h_2(CPU &cpu);
    static void rst_20h_3(CPU &cpu);
    static void rst_28h_1(CPU &cpu);
    static void rst_28h_2(CPU &cpu);
    static void rst_28h_3(CPU &cpu);
    //Fx
    static void pop_af_1(CPU &cpu);
    static void pop_af_2(CPU &cpu);
    static void push_af_1(CPU &cpu);
    static void push_af_2(CPU &cpu);
    static void push_af_3(CPU &cpu);
    static void rst_30h_1(CPU &cpu);
    static void rst_30h_2(CPU &cpu);
    static void rst_30h_3(CPU &cpu);
    static void rst_38h_1(CPU &cpu);
    static void rst_38h_2(CPU &cpu);
    static void rst_38h_3(CPU &cpu);

    //0xCB prefixed instructions
    //0x
    static void rlc_b(CPU &cpu);
    static void rlc_c(CPU &cpu);
    static void rlc_d(CPU &cpu);
    static void rlc_e(CPU &cpu);
    static void rlc_h(CPU &cpu);
    static void rlc_l(CPU &cpu);
    static void rlc_a(CPU &cpu);
    static void rrc_b(CPU &cpu);
    static void rrc_c(CPU &cpu);
    static void rrc_d(CPU &cpu);
    static void rrc_e(CPU &cpu);
    static void rrc_h(CPU &cpu);
    static void rrc_l(CPU &cpu);
    static void rrc_a(CPU &cpu);
    //1x
    static void rl_b(CPU &cpu);
    static void rl_c(CPU &cpu);
    static void rl_d(CPU &cpu);
    static void rl_e(CPU &cpu);
    static void rl_h(CPU &cpu);
    static void rl_l(CPU &cpu);
    static void rl_a(CPU &cpu);
    static void rr_b(CPU &cpu);
    static void rr_c(CPU &cpu);
    static void rr_d(CPU &cpu);
    static void rr_e(CPU &cpu);
    static void rr_h(CPU &cpu);
    static void rr_l(CPU &cpu);
    static void rr_a(CPU &cpu);
    //2x
    static void sla_b(CPU &cpu);
    static void sla_c(CPU &cpu);
    static void sla_d(CPU &cpu);
    static void sla_e(CPU &cpu);
    static void sla_h(CPU &cpu);
    static void sla_l(CPU &cpu);
    static void sla_a(CPU &cpu);
    static void sra_b(CPU &cpu);
    static void sra_c(CPU &cpu);
    static void sra_d(CPU &cpu);
    static void sra_e(CPU &cpu);
    static void sra_h(CPU &cpu);
    static void sra_l(CPU &cpu);
    static void sra_a(CPU &cpu);
    //3x
    static void swap_b(CPU &cpu);
    static void swap_c(CPU &cpu);
    static void swap_d(CPU &cpu);
    static void swap_e(CPU &cpu);
    static void swap_h(CPU &cpu);
    static void swap_l(CPU &cpu);
    static void swap_a(CPU &cpu);
    static void srl_b(CPU &cpu);
    static void srl_c(CPU &cpu);
    static void srl_d(CPU &cpu);
    static void srl_e(CPU &cpu);
    static void srl_h(CPU &cpu);
    static void srl_l(CPU &cpu);
    static void srl_a(CPU &cpu);
    //4x
    static void bit_0_b(CPU &cpu);
    static void bit_0_c(CPU &cpu);
    static void bit_0_d(CPU &cpu);
    static void bit_0_e(CPU &cpu);
    static void bit_0_h(CPU &cpu);
    static void bit_0_l(CPU &cpu);
    static void bit_0_HL_1(CPU &cpu);
    static void bit_0_HL_2(CPU &cpu);
    static void bit_0_a(CPU &cpu);
    static void bit_1_b(CPU &cpu);
    static void bit_1_c(CPU &cpu);
    static void bit_1_d(CPU &cpu);
    static void bit_1_e(CPU &cpu);
    static void bit_1_h(CPU &cpu);
    static void bit_1_l(CPU &cpu);
    static void bit_1_HL_1(CPU &cpu);
    static void bit_1_HL_2(CPU &cpu);
    static void bit_1_a(CPU &cpu);
    //5x
    static void bit_2_b(CPU &cpu);
    static void bit_2_c(CPU &cpu);
    static void bit_2_d(CPU &cpu);
    static void bit_2_e(CPU &cpu);
    static void bit_2_h(CPU &cpu);
    static void bit_2_l(CPU &cpu);
    static void bit_2_HL_1(CPU &cpu);
    static void bit_2_HL_2(CPU &cpu);
    static void bit_2_a(CPU &cpu);
    static void bit_3_b(CPU &cpu);
    static void bit_3_c(CPU &cpu);
    static void bit_3_d(CPU &cpu);
    static void bit_3_e(CPU &cpu);
    static void bit_3_h(CPU &cpu);
    static void bit_3_l(CPU &cpu);
    static void bit_3_HL_1(CPU &cpu);
    static void bit_3_HL_2(CPU &cpu);
    static void bit_3_a(CPU &cpu);
    //6x
    static void bit_4_b(CPU &cpu);
    static void bit_4_c(CPU &cpu);
    static void bit_4_d(CPU &cpu);
    static void bit_4_e(CPU &cpu);
    static void bit_4_h(CPU &cpu);
    static void bit_4_l(CPU &cpu);
    static void bit_4_HL_1(CPU &cpu);
    static void bit_4_HL_2(CPU &cpu);
    static void bit_4_a(CPU &cpu);
    static void bit_5_b(CPU &cpu);
    static void bit_5_c(CPU &cpu);
    static void bit_5_d(CPU &cpu);
    static void bit_5_e(CPU &cpu);
    static void bit_5_h(CPU &cpu);
    static void bit_5_l(CPU &cpu);
    static void bit_5_HL_1(CPU &cpu);
    static void bit_5_HL_2(CPU &cpu);
    static void bit_5_a(CPU &cpu);
    //7x
    static void bit_6_b(CPU &cpu);
    static void bit_6_c(CPU &cpu);
    static void bit_6_d(CPU &cpu);
    static void bit_6_e(CPU &cpu);
    static void bit_6_h(CPU &cpu);
    static void bit_6_l(CPU &cpu);
    static void bit_6_HL_1(CPU &cpu);
    static void bit_6_HL_2(CPU &cpu);
    static void bit_6_a(CPU &cpu);
    static void bit_7_b(CPU &cpu);
    static void bit_7_c(CPU &cpu);
    static void bit_7_d(CPU &cpu);
    static void bit_7_e(CPU &cpu);
    static void bit_7_h(CPU &cpu);
    static void bit_7_l(CPU &cpu);
    static void bit_7_HL_1(CPU &cpu);
    static void bit_7_HL_2(CPU &cpu);
    static void bit_7_a(CPU &cpu);
    //8x
    static void res_0_b(CPU &cpu);
    static void res_0_c(CPU &cpu);
    static void res_0_d(CPU &cpu);
    static void res_0_e(CPU &cpu);
    static void res_0_h(CPU &cpu);
    static void res_0_l(CPU &cpu);
    static void res_0_HL_1(CPU &cpu);
    static void res_0_HL_2(CPU &cpu);
    static void res_0_a(CPU &cpu);
    static void res_1_b(CPU &cpu);
    static void res_1_c(CPU &cpu);
    static void res_1_d(CPU &cpu);
    static void res_1_e(CPU &cpu);
    static void res_1_h(CPU &cpu);
    static void res_1_l(CPU &cpu);
    static void res_1_HL_1(CPU &cpu);
    static void res_1_HL_2(CPU &cpu);
    static void res_1_a(CPU &cpu);
    //9x
    static void res_2_b(CPU &cpu);
    static void res_2_c(CPU &cpu);
    static void res_2_d(CPU &cpu);
    static void res_2_e(CPU &cpu);
    static void res_2_h(CPU &cpu);
    static void res_2_l(CPU &cpu);
    static void res_2_HL_1(CPU &cpu);
    static void res_2_HL_2(CPU &cpu);
    static void res_2_a(CPU &cpu);
    static void res_3_b(CPU &cpu);
    static void res_3_c(CPU &cpu);
    static void res_3_d(CPU &cpu);
    static void res_3_e(CPU &cpu);
    static void res_3_h(CPU &cpu);
    static void res_3_l(CPU &cpu);
    static void res_3_HL_1(CPU &cpu);
    static void res_3_HL_2(CPU &cpu);
    static void res_3_a(CPU &cpu);
    //Ax
    static void res_4_b(CPU &cpu);
    static void res_4_c(CPU &cpu);
    static void res_4_d(CPU &cpu);
    static void res_4_e(CPU &cpu);
    static void res_4_h(CPU &cpu);
    static void res_4_l(CPU &cpu);
    static void res_4_HL_1(CPU &cpu);
    static void res_4_HL_2(CPU &cpu);
    static void res_4_a(CPU &cpu);
    static void res_5_b(CPU &cpu);
    static void res_5_c(CPU &cpu);
    static void res_5_d(CPU &cpu);
    static void res_5_e(CPU &cpu);
    static void res_5_h(CPU &cpu);
    static void res_5_l(CPU &cpu);
    static void res_5_HL_1(CPU &cpu);
    static void res_5_HL_2(CPU &cpu);
    static void res_5_a(CPU &cpu);
    //Bx
    static void res_6_b(CPU &cpu);
    static void res_6_c(CPU &cpu);
    static void res_6_d(CPU &cpu);
    static void res_6_e(CPU &cpu);
    static void res_6_h(CPU &cpu);
    static void res_6_l(CPU &cpu);
    static void res_6_HL_1(CPU &cpu);
    static void res_6_HL_2(CPU &cpu);
    static void res_6_a(CPU &cpu);
    static void res_7_b(CPU &cpu);
    static void res_7_c(CPU &cpu);
    static void res_7_d(CPU &cpu);
    static void res_7_e(CPU &cpu);
    static void res_7_h(CPU &cpu);
    static void res_7_l(CPU &cpu);
    static void res_7_HL_1(CPU &cpu);
    static void res_7_HL_2(CPU &cpu);
    static void res_7_a(CPU &cpu);
    //Cx
    static void set_0_b(CPU &cpu);
    static void set_0_c(CPU &cpu);
    static void set_0_d(CPU &cpu);
    static void set_0_e(CPU &cpu);
    static void set_0_h(CPU &cpu);
    static void set_0_l(CPU &cpu);
    static void set_0_HL_1(CPU &cpu);
    static void set_0_HL_2(CPU &cpu);
    static void set_0_a(CPU &cpu);
    static void set_1_b(CPU &cpu);
    static void set_1_c(CPU &cpu);
    static void set_1_d(CPU &cpu);
    static void set_1_e(CPU &cpu);
    static void set_1_h(CPU &cpu);
    static void set_1_l(CPU &cpu);
    static void set_1_HL_1(CPU &cpu);
    static void set_1_HL_2(CPU &cpu);
    static void set_1_a(CPU &cpu);
    //Dx
    static void set_2_b(CPU &cpu);
    static void set_2_c(CPU &cpu);
    static void set_2_d(CPU &cpu);
    static void set_2_e(CPU &cpu);
    static void set_2_h(CPU &cpu);
    static void set_2_l(CPU &cpu);
    static void set_2_HL_1(CPU &cpu);
    static void set_2_HL_2(CPU &cpu);
    static void set_2_a(CPU &cpu);
    static void set_3_b(CPU &cpu);
    static void set_3_c(CPU &cpu);
    static void set_3_d(CPU &cpu);
    static void set_3_e(CPU &cpu);
    static void set_3_h(CPU &cpu);
    static void set_3_l(CPU &cpu);
    static void set_3_HL_1(CPU &cpu);
    static void set_3_HL_2(CPU &cpu);
    static void set_3_a(CPU &cpu);
    //Ex
    static void set_4_b(CPU &cpu);
    static void set_4_c(CPU &cpu);
    static void set_4_d(CPU &cpu);
    static void set_4_e(CPU &cpu);
    static void set_4_h(CPU &cpu);
    static void set_4_l(CPU &cpu);
    static void set_4_HL_1(CPU &cpu);
    static void set_4_HL_2(CPU &cpu);
    static void set_4_a(CPU &cpu);
    static void set_5_b(CPU &cpu);
    static void set_5_c(CPU &cpu);
    static void set_5_d(CPU &cpu);
    static void set_5_e(CPU &cpu);
    static void set_5_h(CPU &cpu);
    static void set_5_l(CPU &cpu);
    static void set_5_HL_1(CPU &cpu);
    static void set_5_HL_2(CPU &cpu);
    static void set_5_a(CPU &cpu);
    //Fx
    static void set_6_b(CPU &cpu);
    static void set_6_c(CPU &cpu);
    static void set_6_d(CPU &cpu);
    static void set_6_e(CPU &cpu);
    static void set_6_h(CPU &cpu);
    static void set_6_l(CPU &cpu);
    static void set_6_HL_1(CPU &cpu);
    static void set_6_HL_2(CPU &cpu);
    static void set_6_a(CPU &cpu);
    static void set_7_b(CPU &cpu);
    static void set_7_c(CPU &cpu);
    static void set_7_d(CPU &cpu);
    static void set_7_e(CPU &cpu);
    static void set_7_h(CPU &cpu);
    static void set_7_l(CPU &cpu);
    static void set_7_HL_1(CPU &cpu);
    static void set_7_HL_2(CPU &cpu);
    static void set_7_a(CPU &cpu);
};
/*
class InstructionHandler {
public:
    static Instruction get_instruction(bool cb_prefixed, uint8_t opcode);
private:
    static const std::unordered_map<uint8_t, Instruction> non_prefixed_ops_;
    static const std::unordered_map<uint8_t, Instruction> cb_prefixed_ops_;
};
*/