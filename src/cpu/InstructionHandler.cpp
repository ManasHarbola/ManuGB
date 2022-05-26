#include "InstructionHandler.h"

Instruction::Instruction(uint8_t t_cycles, std::vector<std::function<void (CPU&)> > ops) :
t_cycles_(t_cycles), ops_(ops), it_(ops_.begin()) {

}

Instruction::Instruction(const Instruction &inst) :
t_cycles_(inst.t_cycles_), ops_(inst.ops_), it_(ops_.begin()) {

}

/*
Instruction::~Instruction() {

}
*/

//bool Instruction::finished() {return (it_ == ops_.end()) || conditional_stop_;}
uint8_t Instruction::num_t_cycles() {return t_cycles_;}
void Instruction::execute(CPU &cpu) {
    //if (!finished()) {
    if (!cpu.inst_finished_) {
        (*it_)(cpu);
        it_++;
    }
    //Executed all instructions, let CPU know that instruction is finished
    if (it_ == ops_.end()) {
        cpu.inst_finished_ = true;
    }
}

/*
Instruction Instruction::get_instruction(bool cb_prefixed, uint8_t opcode) {
    return cb_prefixed ? cb_prefixed_ops_.at(opcode) : non_prefixed_ops_.at(opcode);
}
*/

std::unique_ptr<Instruction> Instruction::get_instruction(bool cb_prefixed, uint8_t opcode) {
    if (cb_prefixed) {
        /*
        std::unique_ptr<Instruction> ptr(new Instruction(cb_prefixed_ops_.at(opcode)));
        return ptr;
        */
        return std::make_unique<Instruction>(Instruction(cb_prefixed_ops_.at(opcode)));
    }
    
    //return std::unique_ptr<Instruction>(new Instruction(non_prefixed_ops_.at(opcode)));
    return std::make_unique<Instruction>(Instruction(non_prefixed_ops_.at(opcode)));
    
    //std::unique_ptr<Instruction> ptr(new Instruction(non_prefixed_ops_.at(opcode)));
    //return std::make_unique<Instruction>(Instruction(non_prefixed_ops_.at(opcode)));
}

//use std::bind to avoid having to use std::function calls
const std::unordered_map<uint8_t, Instruction> Instruction::non_prefixed_ops_ = {
    //define opcode to Instruction key-value pairs here
    //ex. {0x12, Instruction}
    {0x00, Instruction(4, {nop})},
    {0x01, Instruction(12, {ld_bc_nn_1, ld_bc_nn_2})},
    {0x02, Instruction(8, {ld_BC_A})},
    {0x03, Instruction(8, {inc_bc})},
    {0x04, Instruction(4, {inc_b})},
    {0x05, Instruction(4, {dec_b})}, 
    {0x06, Instruction(8, {ld_b_n})},
    {0x07, Instruction(4, {rlca})},
    {0x08, Instruction(20, {ld_nn_SP_1, ld_nn_SP_2, ld_nn_SP_3, ld_nn_SP_4})},
    {0x09, Instruction(8, {add_HL_BC})},
    {0x0A, Instruction(8, {ld_A_BC})},
    {0x0B, Instruction(8, {dec_bc})},
    {0x0C, Instruction(4, {inc_c})},
    {0x0D, Instruction(4, {dec_c})},
    {0x0E, Instruction(8, {ld_c_n})},
    {0x0F, Instruction(4, {rrca})},

    //{0x10, Instruction(4, {stop})},
    {0x11, Instruction(12, {ld_de_nn_1, ld_de_nn_2})},
    {0x12, Instruction(8, {ld_DE_A})},
    {0x13, Instruction(8, {inc_de})},
    {0x14, Instruction(4, {inc_d})},
    {0x15, Instruction(4, {dec_d})}, 
    {0x16, Instruction(8, {ld_d_n})},
    {0x17, Instruction(4, {rla})},
    {0x18, Instruction(12, {jr_n_1, jr_n_2})},
    {0x19, Instruction(8, {add_HL_DE})},
    {0x1A, Instruction(8, {ld_A_DE})},
    {0x1B, Instruction(8, {dec_de})},
    {0x1C, Instruction(4, {inc_e})},
    {0x1D, Instruction(4, {dec_e})},
    {0x1E, Instruction(8, {ld_e_n})},
    {0x1F, Instruction(4, {rra})},

    {0x20, Instruction(12, {jr_nz_n_1, jr_nz_n_2})},
    {0x21, Instruction(12, {ld_hl_nn_1, ld_hl_nn_2})},
    {0x22, Instruction(8, {ldi_HL_A})},
    {0x23, Instruction(8, {inc_hl})},
    {0x24, Instruction(4, {inc_h})},
    {0x25, Instruction(4, {dec_h})}, 
    {0x26, Instruction(8, {ld_h_n})},
    {0x27, Instruction(4, {daa})},
    {0x28, Instruction(12, {jr_z_n_1, jr_z_n_2})},
    {0x29, Instruction(8, {add_HL_HL})},
    {0x2A, Instruction(8, {ldi_A_HL})},
    {0x2B, Instruction(8, {dec_hl})},
    {0x2C, Instruction(4, {inc_l})},
    {0x2D, Instruction(4, {dec_l})},
    {0x2E, Instruction(8, {ld_l_n})},
    {0x2F, Instruction(4, {cpl})},

    {0x30, Instruction(12, {jr_nc_n_1, jr_nc_n_2})},
    {0x31, Instruction(12, {ld_sp_nn_1, ld_sp_nn_2})},
    {0x32, Instruction(8, {ldd_HL_A})},
    {0x33, Instruction(8, {inc_sp})},
    {0x34, Instruction(12, {inc_HL_1, inc_HL_2})},
    {0x35, Instruction(12, {dec_HL_1, dec_HL_2})}, 
    {0x36, Instruction(12, {ld_HL_n_1, ld_HL_n_2})},
    {0x37, Instruction(4, {scf})},
    {0x38, Instruction(12, {jr_c_n_1, jr_c_n_2})},
    {0x39, Instruction(8, {add_HL_SP})},
    {0x3A, Instruction(8, {ldd_A_HL})},
    {0x3B, Instruction(8, {dec_sp})},
    {0x3C, Instruction(4, {inc_a})},
    {0x3D, Instruction(4, {dec_a})},
    {0x3E, Instruction(8, {ld_a_n})},
    {0x3F, Instruction(4, {ccf})},

    {0x40, Instruction(4, {ld_b_b})},
    {0x41, Instruction(4, {ld_b_c})},
    {0x42, Instruction(4, {ld_b_d})},
    {0x43, Instruction(4, {ld_b_e})},
    {0x44, Instruction(4, {ld_b_h})},
    {0x45, Instruction(4, {ld_b_l})},
    {0x46, Instruction(8, {ld_b_HL})},
    {0x47, Instruction(4, {ld_b_a})},
    {0x48, Instruction(4, {ld_c_b})},
    {0x49, Instruction(4, {ld_c_c})},
    {0x4A, Instruction(4, {ld_c_d})},
    {0x4B, Instruction(4, {ld_c_e})},
    {0x4C, Instruction(4, {ld_c_h})},
    {0x4D, Instruction(4, {ld_c_l})},
    {0x4E, Instruction(8, {ld_c_HL})},
    {0x4F, Instruction(4, {ld_c_a})},

    {0x50, Instruction(4, {ld_d_b})},
    {0x51, Instruction(4, {ld_d_c})},
    {0x52, Instruction(4, {ld_d_d})},
    {0x53, Instruction(4, {ld_d_e})},
    {0x54, Instruction(4, {ld_d_h})},
    {0x55, Instruction(4, {ld_d_l})},
    {0x56, Instruction(8, {ld_d_HL})},
    {0x57, Instruction(4, {ld_d_a})},
    {0x58, Instruction(4, {ld_e_b})},
    {0x59, Instruction(4, {ld_e_c})},
    {0x5A, Instruction(4, {ld_e_d})},
    {0x5B, Instruction(4, {ld_e_e})},
    {0x5C, Instruction(4, {ld_e_h})},
    {0x5D, Instruction(4, {ld_e_l})},
    {0x5E, Instruction(8, {ld_e_HL})},
    {0x5F, Instruction(4, {ld_e_a})},
    
    {0x60, Instruction(4, {ld_h_b})},
    {0x61, Instruction(4, {ld_h_c})},
    {0x62, Instruction(4, {ld_h_d})},
    {0x63, Instruction(4, {ld_h_e})},
    {0x64, Instruction(4, {ld_h_h})},
    {0x65, Instruction(4, {ld_h_l})},
    {0x66, Instruction(8, {ld_h_HL})},
    {0x67, Instruction(4, {ld_h_a})},
    {0x68, Instruction(4, {ld_l_b})},
    {0x69, Instruction(4, {ld_l_c})},
    {0x6A, Instruction(4, {ld_l_d})},
    {0x6B, Instruction(4, {ld_l_e})},
    {0x6C, Instruction(4, {ld_l_h})},
    {0x6D, Instruction(4, {ld_l_l})},
    {0x6E, Instruction(8, {ld_l_HL})},
    {0x6F, Instruction(4, {ld_l_a})},

    {0x70, Instruction(4, {ld_HL_b})},
    {0x71, Instruction(4, {ld_HL_c})},
    {0x72, Instruction(4, {ld_HL_d})},
    {0x73, Instruction(4, {ld_HL_e})},
    {0x74, Instruction(4, {ld_HL_h})},
    {0x75, Instruction(4, {ld_HL_l})},
    //{0x76, Instruction(8, {ld_h_HL})},
    {0x77, Instruction(4, {ld_HL_a})},
    {0x78, Instruction(4, {ld_a_b})},
    {0x79, Instruction(4, {ld_a_c})},
    {0x7A, Instruction(4, {ld_a_d})},
    {0x7B, Instruction(4, {ld_a_e})},
    {0x7C, Instruction(4, {ld_a_h})},
    {0x7D, Instruction(4, {ld_a_l})},
    {0x7E, Instruction(8, {ld_a_HL})},
    {0x7F, Instruction(4, {ld_a_a})},

    {0x80, Instruction(4, {add_A_b})},
    {0x81, Instruction(4, {add_A_c})},
    {0x82, Instruction(4, {add_A_d})},
    {0x83, Instruction(4, {add_A_e})},
    {0x84, Instruction(4, {add_A_h})},
    {0x85, Instruction(4, {add_A_l})},
    {0x86, Instruction(8, {add_A_HL})},
    {0x87, Instruction(4, {add_A_a})},
    {0x88, Instruction(4, {adc_A_b})},
    {0x89, Instruction(4, {adc_A_c})},
    {0x8A, Instruction(4, {adc_A_d})},
    {0x8B, Instruction(4, {adc_A_e})},
    {0x8C, Instruction(4, {adc_A_h})},
    {0x8D, Instruction(4, {adc_A_l})},
    {0x8E, Instruction(8, {adc_A_HL})},
    {0x8F, Instruction(4, {adc_A_a})},

    {0x90, Instruction(4, {sub_A_b})},
    {0x91, Instruction(4, {sub_A_c})},
    {0x92, Instruction(4, {sub_A_d})},
    {0x93, Instruction(4, {sub_A_e})},
    {0x94, Instruction(4, {sub_A_h})},
    {0x95, Instruction(4, {sub_A_l})},
    {0x96, Instruction(8, {sub_HL})},
    {0x97, Instruction(4, {sub_A_a})},
    {0x98, Instruction(4, {sbc_A_b})},
    {0x99, Instruction(4, {sbc_A_c})},
    {0x9A, Instruction(4, {sbc_A_d})},
    {0x9B, Instruction(4, {sbc_A_e})},
    {0x9C, Instruction(4, {sbc_A_h})},
    {0x9D, Instruction(4, {sbc_A_l})},
    {0x9E, Instruction(8, {sbc_A_HL})},
    {0x9F, Instruction(4, {sbc_A_a})},

    {0xA0, Instruction(4, {and_b})},
    {0xA1, Instruction(4, {and_c})},
    {0xA2, Instruction(4, {and_d})},
    {0xA3, Instruction(4, {and_e})},
    {0xA4, Instruction(4, {and_h})},
    {0xA5, Instruction(4, {and_l})},
    {0xA6, Instruction(8, {and_HL})},
    {0xA7, Instruction(4, {and_a})},
    {0xA8, Instruction(4, {xor_b})},
    {0xA9, Instruction(4, {xor_c})},
    {0xAA, Instruction(4, {xor_d})},
    {0xAB, Instruction(4, {xor_e})},
    {0xAC, Instruction(4, {xor_h})},
    {0xAD, Instruction(4, {xor_l})},
    {0xAE, Instruction(8, {xor_HL})},
    {0xAF, Instruction(4, {xor_a})},    

    {0xB0, Instruction(4, {or_b})},
    {0xB1, Instruction(4, {or_c})},
    {0xB2, Instruction(4, {or_d})},
    {0xB3, Instruction(4, {or_e})},
    {0xB4, Instruction(4, {or_h})},
    {0xB5, Instruction(4, {or_l})},
    {0xB6, Instruction(8, {or_HL})},
    {0xB7, Instruction(4, {or_a})},
    {0xB8, Instruction(4, {cp_b})},
    {0xB9, Instruction(4, {cp_c})},
    {0xBA, Instruction(4, {cp_d})},
    {0xBB, Instruction(4, {cp_e})},
    {0xBC, Instruction(4, {cp_h})},
    {0xBD, Instruction(4, {cp_l})},
    {0xBE, Instruction(8, {cp_HL})},
    {0xBF, Instruction(4, {cp_a})},

    {0xC0, Instruction(20, {ret_nz_1, ret_nz_2, ret_nz_3, ret_nz_4})},
    {0xC1, Instruction(12, {pop_bc_1, pop_bc_2})},
    {0xC2, Instruction(16, {jp_nz_nn_1, jp_nz_nn_2, jp_nz_nn_3})},
    {0xC3, Instruction(16, {jp_nn_1, jp_nn_2, jp_nn_3})},
    {0xC4, Instruction(24, {call_nz_nn_1, call_nz_nn_2, call_nz_nn_3, call_nz_nn_4, call_nz_nn_5})},
    {0xC5, Instruction(16, {push_bc_1, push_bc_2, push_bc_3})},
    {0xC6, Instruction(8, {add_A_n})},
    {0xC7, Instruction(16, {rst_00h_1, rst_00h_2, rst_00h_3})},
    {0xC8, Instruction(20, {ret_z_1, ret_z_2, ret_z_3, ret_z_4})},
    {0xC9, Instruction(16, {ret_1, ret_2, ret_3})},
    {0xCA, Instruction(16, {jp_z_nn_1, jp_z_nn_2, jp_z_nn_3})},
    //{0xCB, Instruction(4, {})},
    {0xCC, Instruction(24, {call_z_nn_1, call_z_nn_2, call_z_nn_3, call_z_nn_4, call_z_nn_5})},
    {0xCD, Instruction(24, {call_nn_1, call_nn_2, call_nn_3, call_nn_4, call_nn_5})},
    {0xCE, Instruction(8, {adc_A_n})},
    {0xCF, Instruction(16, {rst_08h_1, rst_08h_2, rst_08h_3})},

    {0xD0, Instruction(20, {ret_nc_1, ret_nc_2, ret_nc_3, ret_nc_4})},
    {0xD1, Instruction(12, {pop_de_1, pop_de_2})},
    {0xD2, Instruction(16, {jp_nc_nn_1, jp_nc_nn_2, jp_nc_nn_3})},

    {0xD4, Instruction(24, {call_nc_nn_1, call_nc_nn_2, call_nc_nn_3, call_nc_nn_4, call_nc_nn_5})},
    {0xD5, Instruction(16, {push_de_1, push_de_2, push_de_3})},
    {0xD6, Instruction(8, {sub_n})},
    {0xD7, Instruction(16, {rst_10h_1, rst_10h_2, rst_10h_3})},
    {0xD8, Instruction(20, {ret_c_1, ret_c_2, ret_c_3, ret_c_4})},
    {0xD9, Instruction(16, {reti_1, reti_2, reti_3})},
    {0xDA, Instruction(16, {jp_c_nn_1, jp_c_nn_2, jp_c_nn_3})},

    {0xDC, Instruction(24, {call_c_nn_1, call_c_nn_2, call_c_nn_3, call_c_nn_4, call_c_nn_5})},
    
    {0xDE, Instruction(8, {sbc_n})},
    {0xDF, Instruction(16, {rst_18h_1, rst_18h_2, rst_18h_3})},

    {0xE0, Instruction(12, {ld_FF00_n_A_1, ld_FF00_n_A_2})},
    {0xE1, Instruction(12, {pop_hl_1, pop_hl_2})},
    {0xE2, Instruction(8, {ld_FF00_C_A})},

    {0xE5, Instruction(16, {push_hl_1, push_hl_2, push_hl_3})},
    {0xE6, Instruction(8, {and_n})},
    {0xE7, Instruction(16, {rst_20h_1, rst_20h_2, rst_20h_3})},
    {0xE8, Instruction(16, {add_SP_n_1, add_SP_n_2, add_SP_n_3})},
    {0xE9, Instruction(4, {jp_hl})},
    {0xEA, Instruction(16, {ld_nn_A_1, ld_nn_A_2, ld_nn_A_3})},

    {0xEE, Instruction(8, {xor_n})},
    {0xEF, Instruction(16, {rst_28h_1, rst_28h_2, rst_28h_3})},

    {0xF0, Instruction(12, {ld_A_FF00_n_1, ld_A_FF00_n_2})},
    {0xF1, Instruction(12, {pop_af_1, pop_af_2})},
    {0xF2, Instruction(8, {ld_A_FF00_C})},
    {0xF3, Instruction(4, {di})},

    {0xF5, Instruction(16, {push_af_1, push_af_2, push_af_3})},
    {0xF6, Instruction(8, {or_n})},
    {0xF7, Instruction(16, {rst_30h_1, rst_30h_2, rst_30h_3})},
    {0xF8, Instruction(12, {ld_HL_SP_r8_1, ld_HL_SP_r8_2})},
    {0xF9, Instruction(8, {ld_SP_HL})},
    {0xFA, Instruction(16, {ld_A_nn_1, ld_A_nn_2, ld_A_nn_3})},
    {0xFB, Instruction(4, {ei})},

    {0xFE, Instruction(8, {cp_n})},
    {0xFF, Instruction(16, {rst_38h_1, rst_38h_2, rst_38h_3})}
};

const std::unordered_map<uint8_t, Instruction> Instruction::cb_prefixed_ops_ = {
    //define cb-prefixed opcode to function key-value pairs here
    //ex. {0x12, Instruction}
    {0x00, Instruction(8, {rlc_b})},
    {0x01, Instruction(8, {rlc_c})},
    {0x02, Instruction(8, {rlc_d})},
    {0x03, Instruction(8, {rlc_e})},
    {0x04, Instruction(8, {rlc_h})},
    {0x05, Instruction(8, {rlc_l})},
    {0x06, Instruction(16, {rlc_HL_1, rlc_HL_2})},
    {0x07, Instruction(8, {rlc_a})},
    {0x08, Instruction(8, {rrc_b})},
    {0x09, Instruction(8, {rrc_c})},
    {0x0A, Instruction(8, {rrc_d})},
    {0x0B, Instruction(8, {rrc_e})},
    {0x0C, Instruction(8, {rrc_h})},
    {0x0D, Instruction(8, {rrc_l})},
    {0x0E, Instruction(16, {rrc_HL_1, rrc_HL_2})},
    {0x0F, Instruction(8, {rrc_a})},
    
    {0x10, Instruction(8, {rl_b})},
    {0x11, Instruction(8, {rl_c})},
    {0x12, Instruction(8, {rl_d})},
    {0x13, Instruction(8, {rl_e})},
    {0x14, Instruction(8, {rl_h})},
    {0x15, Instruction(8, {rl_l})},
    {0x16, Instruction(16, {rl_HL_1, rl_HL_2})},
    {0x17, Instruction(8, {rl_a})},
    {0x18, Instruction(8, {rr_b})},
    {0x19, Instruction(8, {rr_c})},
    {0x1A, Instruction(8, {rr_d})},
    {0x1B, Instruction(8, {rr_e})},
    {0x1C, Instruction(8, {rr_h})},
    {0x1D, Instruction(8, {rr_l})},
    {0x1E, Instruction(16, {rr_HL_1, rr_HL_2})},
    {0x1F, Instruction(8, {rr_a})},

    {0x20, Instruction(8, {sla_b})},
    {0x21, Instruction(8, {sla_c})},
    {0x22, Instruction(8, {sla_d})},
    {0x23, Instruction(8, {sla_e})},
    {0x24, Instruction(8, {sla_h})},
    {0x25, Instruction(8, {sla_l})},
    {0x26, Instruction(16, {sla_HL_1, sla_HL_2})},
    {0x27, Instruction(8, {sla_a})},
    {0x28, Instruction(8, {sra_b})},
    {0x29, Instruction(8, {sra_c})},
    {0x2A, Instruction(8, {sra_d})},
    {0x2B, Instruction(8, {sra_e})},
    {0x2C, Instruction(8, {sra_h})},
    {0x2D, Instruction(8, {sra_l})},
    {0x2E, Instruction(16, {sra_HL_1, sra_HL_2})},
    {0x2F, Instruction(8, {sra_a})},

    {0x30, Instruction(8, {swap_b})},
    {0x31, Instruction(8, {swap_c})},
    {0x32, Instruction(8, {swap_d})},
    {0x33, Instruction(8, {swap_e})},
    {0x34, Instruction(8, {swap_h})},
    {0x35, Instruction(8, {swap_l})},
    {0x36, Instruction(16, {swap_HL_1, swap_HL_2})},
    {0x37, Instruction(8, {swap_a})},
    {0x38, Instruction(8, {srl_b})},
    {0x39, Instruction(8, {srl_c})},
    {0x3A, Instruction(8, {srl_d})},
    {0x3B, Instruction(8, {srl_e})},
    {0x3C, Instruction(8, {srl_h})},
    {0x3D, Instruction(8, {srl_l})},
    {0x3E, Instruction(16, {srl_HL_1, srl_HL_2})},
    {0x3F, Instruction(8, {srl_a})},

    {0x40, Instruction(8, {bit_0_b})},
    {0x41, Instruction(8, {bit_0_c})},
    {0x42, Instruction(8, {bit_0_d})},
    {0x43, Instruction(8, {bit_0_e})},
    {0x44, Instruction(8, {bit_0_h})},
    {0x45, Instruction(8, {bit_0_l})},
    {0x46, Instruction(16, {bit_0_HL_1, bit_0_HL_2})},
    {0x47, Instruction(8, {bit_0_a})},
    {0x48, Instruction(8, {bit_1_b})},
    {0x49, Instruction(8, {bit_1_c})},
    {0x4A, Instruction(8, {bit_1_d})},
    {0x4B, Instruction(8, {bit_1_e})},
    {0x4C, Instruction(8, {bit_1_h})},
    {0x4D, Instruction(8, {bit_1_l})},
    {0x4E, Instruction(16, {bit_1_HL_1, bit_1_HL_2})},
    {0x4F, Instruction(8, {bit_1_a})},

    {0x50, Instruction(8, {bit_2_b})},
    {0x51, Instruction(8, {bit_2_c})},
    {0x52, Instruction(8, {bit_2_d})},
    {0x53, Instruction(8, {bit_2_e})},
    {0x54, Instruction(8, {bit_2_h})},
    {0x55, Instruction(8, {bit_2_l})},
    {0x56, Instruction(16, {bit_2_HL_1, bit_2_HL_2})},
    {0x57, Instruction(8, {bit_2_a})},
    {0x58, Instruction(8, {bit_3_b})},
    {0x59, Instruction(8, {bit_3_c})},
    {0x5A, Instruction(8, {bit_3_d})},
    {0x5B, Instruction(8, {bit_3_e})},
    {0x5C, Instruction(8, {bit_3_h})},
    {0x5D, Instruction(8, {bit_3_l})},
    {0x5E, Instruction(16, {bit_3_HL_1, bit_3_HL_2})},
    {0x5F, Instruction(8, {bit_3_a})},

    {0x60, Instruction(8, {bit_4_b})},
    {0x61, Instruction(8, {bit_4_c})},
    {0x62, Instruction(8, {bit_4_d})},
    {0x63, Instruction(8, {bit_4_e})},
    {0x64, Instruction(8, {bit_4_h})},
    {0x65, Instruction(8, {bit_4_l})},
    {0x66, Instruction(16, {bit_4_HL_1, bit_4_HL_2})},
    {0x67, Instruction(8, {bit_4_a})},
    {0x68, Instruction(8, {bit_5_b})},
    {0x69, Instruction(8, {bit_5_c})},
    {0x6A, Instruction(8, {bit_5_d})},
    {0x6B, Instruction(8, {bit_5_e})},
    {0x6C, Instruction(8, {bit_5_h})},
    {0x6D, Instruction(8, {bit_5_l})},
    {0x6E, Instruction(16, {bit_5_HL_1, bit_5_HL_2})},
    {0x6F, Instruction(8, {bit_5_a})},

    {0x70, Instruction(8, {bit_6_b})},
    {0x71, Instruction(8, {bit_6_c})},
    {0x72, Instruction(8, {bit_6_d})},
    {0x73, Instruction(8, {bit_6_e})},
    {0x74, Instruction(8, {bit_6_h})},
    {0x75, Instruction(8, {bit_6_l})},
    {0x76, Instruction(16, {bit_6_HL_1, bit_6_HL_2})},
    {0x77, Instruction(8, {bit_6_a})},
    {0x78, Instruction(8, {bit_7_b})},
    {0x79, Instruction(8, {bit_7_c})},
    {0x7A, Instruction(8, {bit_7_d})},
    {0x7B, Instruction(8, {bit_7_e})},
    {0x7C, Instruction(8, {bit_7_h})},
    {0x7D, Instruction(8, {bit_7_l})},
    {0x7E, Instruction(16, {bit_7_HL_1, bit_7_HL_2})},
    {0x7F, Instruction(8, {bit_7_a})},

    {0x80, Instruction(8, {res_0_b})},
    {0x81, Instruction(8, {res_0_c})},
    {0x82, Instruction(8, {res_0_d})},
    {0x83, Instruction(8, {res_0_e})},
    {0x84, Instruction(8, {res_0_h})},
    {0x85, Instruction(8, {res_0_l})},
    {0x86, Instruction(16, {res_0_HL_1, res_0_HL_2})},
    {0x87, Instruction(8, {res_0_a})},
    {0x88, Instruction(8, {res_1_b})},
    {0x89, Instruction(8, {res_1_c})},
    {0x8A, Instruction(8, {res_1_d})},
    {0x8B, Instruction(8, {res_1_e})},
    {0x8C, Instruction(8, {res_1_h})},
    {0x8D, Instruction(8, {res_1_l})},
    {0x8E, Instruction(16, {res_1_HL_1, res_1_HL_2})},
    {0x8F, Instruction(8, {res_1_a})},

    {0x90, Instruction(8, {res_2_b})},
    {0x91, Instruction(8, {res_2_c})},
    {0x92, Instruction(8, {res_2_d})},
    {0x93, Instruction(8, {res_2_e})},
    {0x94, Instruction(8, {res_2_h})},
    {0x95, Instruction(8, {res_2_l})},
    {0x96, Instruction(16, {res_2_HL_1, res_2_HL_2})},
    {0x97, Instruction(8, {res_2_a})},
    {0x98, Instruction(8, {res_3_b})},
    {0x99, Instruction(8, {res_3_c})},
    {0x9A, Instruction(8, {res_3_d})},
    {0x9B, Instruction(8, {res_3_e})},
    {0x9C, Instruction(8, {res_3_h})},
    {0x9D, Instruction(8, {res_3_l})},
    {0x9E, Instruction(16, {res_3_HL_1, res_3_HL_2})},
    {0x9F, Instruction(8, {res_3_a})},

    {0xA0, Instruction(8, {res_4_b})},
    {0xA1, Instruction(8, {res_4_c})},
    {0xA2, Instruction(8, {res_4_d})},
    {0xA3, Instruction(8, {res_4_e})},
    {0xA4, Instruction(8, {res_4_h})},
    {0xA5, Instruction(8, {res_4_l})},
    {0xA6, Instruction(16, {res_4_HL_1, res_4_HL_2})},
    {0xA7, Instruction(8, {res_4_a})},
    {0xA8, Instruction(8, {res_5_b})},
    {0xA9, Instruction(8, {res_5_c})},
    {0xAA, Instruction(8, {res_5_d})},
    {0xAB, Instruction(8, {res_5_e})},
    {0xAC, Instruction(8, {res_5_h})},
    {0xAD, Instruction(8, {res_5_l})},
    {0xAE, Instruction(16, {res_5_HL_1, res_5_HL_2})},
    {0xAF, Instruction(8, {res_5_a})},

    {0xB0, Instruction(8, {res_6_b})},
    {0xB1, Instruction(8, {res_6_c})},
    {0xB2, Instruction(8, {res_6_d})},
    {0xB3, Instruction(8, {res_6_e})},
    {0xB4, Instruction(8, {res_6_h})},
    {0xB5, Instruction(8, {res_6_l})},
    {0xB6, Instruction(16, {res_6_HL_1, res_6_HL_2})},
    {0xB7, Instruction(8, {res_6_a})},
    {0xB8, Instruction(8, {res_7_b})},
    {0xB9, Instruction(8, {res_7_c})},
    {0xBA, Instruction(8, {res_7_d})},
    {0xBB, Instruction(8, {res_7_e})},
    {0xBC, Instruction(8, {res_7_h})},
    {0xBD, Instruction(8, {res_7_l})},
    {0xBE, Instruction(16, {res_7_HL_1, res_7_HL_2})},
    {0xBF, Instruction(8, {res_7_a})},

    {0xC0, Instruction(8, {set_0_b})},
    {0xC1, Instruction(8, {set_0_c})},
    {0xC2, Instruction(8, {set_0_d})},
    {0xC3, Instruction(8, {set_0_e})},
    {0xC4, Instruction(8, {set_0_h})},
    {0xC5, Instruction(8, {set_0_l})},
    {0xC6, Instruction(16, {set_0_HL_1, set_0_HL_2})},
    {0xC7, Instruction(8, {set_0_a})},
    {0xC8, Instruction(8, {set_1_b})},
    {0xC9, Instruction(8, {set_1_c})},
    {0xCA, Instruction(8, {set_1_d})},
    {0xCB, Instruction(8, {set_1_e})},
    {0xCC, Instruction(8, {set_1_h})},
    {0xCD, Instruction(8, {set_1_l})},
    {0xCE, Instruction(16, {set_1_HL_1, set_1_HL_2})},
    {0xCF, Instruction(8, {set_1_a})},

    {0xD0, Instruction(8, {set_2_b})},
    {0xD1, Instruction(8, {set_2_c})},
    {0xD2, Instruction(8, {set_2_d})},
    {0xD3, Instruction(8, {set_2_e})},
    {0xD4, Instruction(8, {set_2_h})},
    {0xD5, Instruction(8, {set_2_l})},
    {0xD6, Instruction(16, {set_2_HL_1, set_2_HL_2})},
    {0xD7, Instruction(8, {set_2_a})},
    {0xD8, Instruction(8, {set_3_b})},
    {0xD9, Instruction(8, {set_3_c})},
    {0xDA, Instruction(8, {set_3_d})},
    {0xDB, Instruction(8, {set_3_e})},
    {0xDC, Instruction(8, {set_3_h})},
    {0xDD, Instruction(8, {set_3_l})},
    {0xDE, Instruction(16, {set_3_HL_1, set_3_HL_2})},
    {0xDF, Instruction(8, {set_3_a})},

    {0xE0, Instruction(8, {set_4_b})},
    {0xE1, Instruction(8, {set_4_c})},
    {0xE2, Instruction(8, {set_4_d})},
    {0xE3, Instruction(8, {set_4_e})},
    {0xE4, Instruction(8, {set_4_h})},
    {0xE5, Instruction(8, {set_4_l})},
    {0xE6, Instruction(16, {set_4_HL_1, set_4_HL_2})},
    {0xE7, Instruction(8, {set_4_a})},
    {0xE8, Instruction(8, {set_5_b})},
    {0xE9, Instruction(8, {set_5_c})},
    {0xEA, Instruction(8, {set_5_d})},
    {0xEB, Instruction(8, {set_5_e})},
    {0xEC, Instruction(8, {set_5_h})},
    {0xED, Instruction(8, {set_5_l})},
    {0xEE, Instruction(16, {set_5_HL_1, set_5_HL_2})},
    {0xEF, Instruction(8, {set_5_a})},

    {0xF0, Instruction(8, {set_6_b})},
    {0xF1, Instruction(8, {set_6_c})},
    {0xF2, Instruction(8, {set_6_d})},
    {0xF3, Instruction(8, {set_6_e})},
    {0xF4, Instruction(8, {set_6_h})},
    {0xF5, Instruction(8, {set_6_l})},
    {0xF6, Instruction(16, {set_6_HL_1, set_6_HL_2})},
    {0xF7, Instruction(8, {set_6_a})},
    {0xF8, Instruction(8, {set_7_b})},
    {0xF9, Instruction(8, {set_7_c})},
    {0xFA, Instruction(8, {set_7_d})},
    {0xFB, Instruction(8, {set_7_e})},
    {0xFC, Instruction(8, {set_7_h})},
    {0xFD, Instruction(8, {set_7_l})},
    {0xFE, Instruction(16, {set_7_HL_1, set_7_HL_2})},
    {0xFF, Instruction(8, {set_7_a})},
};

//LD r, r'
void Instruction::ld_r1_r2(uint8_t &r1, uint8_t r2) {r1 = r2;}
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
    cpu.registers_.a = cpu.mmu_.read(cpu.nn_);
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
void Instruction::ld_FF00_n_A_1(CPU &cpu) {
    cpu.n_ = cpu.mmu_.read(cpu.registers_.pc++);
}
void Instruction::ld_FF00_n_A_2(CPU &cpu) {
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
        rr &= 0xFFF0;
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
            //conditional_stop_ = true;
            cpu.inst_finished_ = true;
        }
    } else {
        if ((cpu.registers_.f & flag) == 0) {
            //instruction 2 won't run
            //conditional_stop_ = true;
            cpu.inst_finished_ = true;
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
            //conditional_stop_ = true;
            cpu.inst_finished_ = true;
        }
    } else {
        if ((cpu.registers_.f & flag) == 0) {
            //instruction 2 won't run
            //conditional_stop_ = true;
            cpu.inst_finished_ = true;
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
            //conditional_stop_ = true;
            cpu.inst_finished_ = true;
        }
    } else {
        if ((cpu.registers_.f & flag) == 0) {
            //following instructions won't run
            //conditional_stop_ = true;
            cpu.inst_finished_ = true;
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
            //conditional_stop_ = true;
            cpu.inst_finished_ = true;
        }
    } else {
        if ((cpu.registers_.f & flag) == 0) {
            //conditional_stop_ = true;
            cpu.inst_finished_ = true;
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



//0x
void Instruction::ld_bc_nn_1(CPU &cpu) {ld_rr_nn_1(cpu, cpu.registers_.bc);}
void Instruction::ld_bc_nn_2(CPU &cpu) {ld_rr_nn_2(cpu, cpu.registers_.bc);}
void Instruction::inc_bc(CPU &cpu) {inc_rr(cpu, cpu.registers_.bc);}
void Instruction::inc_b(CPU &cpu) {inc_r(cpu, cpu.registers_.b);}
void Instruction::dec_b(CPU &cpu) {dec_r(cpu, cpu.registers_.b);}
void Instruction::ld_b_n(CPU &cpu) {ld_r_n(cpu, cpu.registers_.b);}
void Instruction::add_HL_BC(CPU &cpu) {add_HL_rr(cpu, cpu.registers_.bc);}
void Instruction::dec_bc(CPU &cpu) {dec_rr(cpu, cpu.registers_.bc);}
void Instruction::inc_c(CPU &cpu) {inc_r(cpu, cpu.registers_.c);}
void Instruction::dec_c(CPU &cpu) {dec_r(cpu, cpu.registers_.c);}
void Instruction::ld_c_n(CPU &cpu) {ld_r_n(cpu, cpu.registers_.c);}
//1x
void Instruction::ld_de_nn_1(CPU &cpu) {ld_rr_nn_1(cpu, cpu.registers_.de);}
void Instruction::ld_de_nn_2(CPU &cpu) {ld_rr_nn_2(cpu, cpu.registers_.de);}
void Instruction::inc_de(CPU &cpu) {inc_rr(cpu, cpu.registers_.de);}
void Instruction::inc_d(CPU &cpu) {inc_r(cpu, cpu.registers_.d);}
void Instruction::dec_d(CPU &cpu) {dec_r(cpu, cpu.registers_.d);}
void Instruction::ld_d_n(CPU &cpu) {ld_r_n(cpu, cpu.registers_.d);}
void Instruction::add_HL_DE(CPU &cpu) {add_HL_rr(cpu, cpu.registers_.de);}
void Instruction::dec_de(CPU &cpu) {dec_rr(cpu, cpu.registers_.de);}
void Instruction::inc_e(CPU &cpu) {inc_r(cpu, cpu.registers_.e);}
void Instruction::dec_e(CPU &cpu) {dec_r(cpu, cpu.registers_.e);}
void Instruction::ld_e_n(CPU &cpu) {ld_r_n(cpu, cpu.registers_.e);}
//2x
void Instruction::jr_nz_n_1(CPU &cpu) {jr_cc_n_1(cpu, ZERO, true);}
void Instruction::jr_nz_n_2(CPU &cpu) {jr_cc_n_2(cpu);}
void Instruction::ld_hl_nn_1(CPU &cpu) {ld_rr_nn_1(cpu, cpu.registers_.hl);}
void Instruction::ld_hl_nn_2(CPU &cpu) {ld_rr_nn_2(cpu, cpu.registers_.hl);}
void Instruction::inc_hl(CPU &cpu) {inc_rr(cpu, cpu.registers_.hl);}
void Instruction::inc_h(CPU &cpu) {inc_r(cpu, cpu.registers_.h);}
void Instruction::dec_h(CPU &cpu) {dec_r(cpu, cpu.registers_.h);}
void Instruction::ld_h_n(CPU &cpu) {ld_r_n(cpu, cpu.registers_.h);}
void Instruction::jr_z_n_1(CPU &cpu) {jr_cc_n_1(cpu, ZERO, false);}
void Instruction::jr_z_n_2(CPU &cpu) {jr_cc_n_2(cpu);}
void Instruction::add_HL_HL(CPU &cpu) {add_HL_rr(cpu, cpu.registers_.hl);}
void Instruction::dec_hl(CPU &cpu) {dec_rr(cpu, cpu.registers_.hl);}
void Instruction::inc_l(CPU &cpu) {inc_r(cpu, cpu.registers_.l);}
void Instruction::dec_l(CPU &cpu) {dec_r(cpu, cpu.registers_.l);}
void Instruction::ld_l_n(CPU &cpu) {ld_r_n(cpu, cpu.registers_.l);}
//3x
void Instruction::jr_nc_n_1(CPU &cpu) {jr_cc_n_1(cpu, CARRY, true);}
void Instruction::jr_nc_n_2(CPU &cpu) {jr_cc_n_2(cpu);}
void Instruction::ld_sp_nn_1(CPU &cpu) {ld_rr_nn_1(cpu, cpu.registers_.sp);}
void Instruction::ld_sp_nn_2(CPU &cpu) {ld_rr_nn_2(cpu, cpu.registers_.sp);}
void Instruction::inc_sp(CPU &cpu) {inc_rr(cpu, cpu.registers_.sp);}
void Instruction::jr_c_n_1(CPU &cpu) {jr_cc_n_1(cpu, CARRY, false);}
void Instruction::jr_c_n_2(CPU &cpu) {jr_cc_n_2(cpu);}
void Instruction::add_HL_SP(CPU &cpu) {add_HL_rr(cpu, cpu.registers_.sp);}
void Instruction::dec_sp(CPU &cpu) {dec_rr(cpu, cpu.registers_.sp);}
void Instruction::inc_a(CPU &cpu) {inc_r(cpu, cpu.registers_.a);}
void Instruction::dec_a(CPU &cpu) {dec_r(cpu, cpu.registers_.a);}
void Instruction::ld_a_n(CPU &cpu) {ld_r_n(cpu, cpu.registers_.a);}
//4x
void Instruction::ld_b_b(CPU &cpu) {ld_r1_r2(cpu.registers_.b, cpu.registers_.b);}
void Instruction::ld_b_c(CPU &cpu) {ld_r1_r2(cpu.registers_.b, cpu.registers_.c);}
void Instruction::ld_b_d(CPU &cpu) {ld_r1_r2(cpu.registers_.b, cpu.registers_.d);}
void Instruction::ld_b_e(CPU &cpu) {ld_r1_r2(cpu.registers_.b, cpu.registers_.e);}
void Instruction::ld_b_h(CPU &cpu) {ld_r1_r2(cpu.registers_.b, cpu.registers_.h);}
void Instruction::ld_b_l(CPU &cpu) {ld_r1_r2(cpu.registers_.b, cpu.registers_.l);}
void Instruction::ld_b_HL(CPU &cpu) {ld_r_HL(cpu, cpu.registers_.b);}
void Instruction::ld_b_a(CPU &cpu) {ld_r1_r2(cpu.registers_.b, cpu.registers_.a);}
void Instruction::ld_c_b(CPU &cpu) {ld_r1_r2(cpu.registers_.c, cpu.registers_.b);}
void Instruction::ld_c_c(CPU &cpu) {ld_r1_r2(cpu.registers_.c, cpu.registers_.c);}
void Instruction::ld_c_d(CPU &cpu) {ld_r1_r2(cpu.registers_.c, cpu.registers_.d);}
void Instruction::ld_c_e(CPU &cpu) {ld_r1_r2(cpu.registers_.c, cpu.registers_.e);}
void Instruction::ld_c_h(CPU &cpu) {ld_r1_r2(cpu.registers_.c, cpu.registers_.h);}
void Instruction::ld_c_l(CPU &cpu) {ld_r1_r2(cpu.registers_.c, cpu.registers_.l);}
void Instruction::ld_c_HL(CPU &cpu) {ld_r_HL(cpu, cpu.registers_.c);}
void Instruction::ld_c_a(CPU &cpu) {ld_r1_r2(cpu.registers_.c, cpu.registers_.a);}
//5x
void Instruction::ld_d_b(CPU &cpu) {ld_r1_r2(cpu.registers_.d, cpu.registers_.b);}
void Instruction::ld_d_c(CPU &cpu) {ld_r1_r2(cpu.registers_.d, cpu.registers_.c);}
void Instruction::ld_d_d(CPU &cpu) {ld_r1_r2(cpu.registers_.d, cpu.registers_.d);}
void Instruction::ld_d_e(CPU &cpu) {ld_r1_r2(cpu.registers_.d, cpu.registers_.e);}
void Instruction::ld_d_h(CPU &cpu) {ld_r1_r2(cpu.registers_.d, cpu.registers_.h);}
void Instruction::ld_d_l(CPU &cpu) {ld_r1_r2(cpu.registers_.d, cpu.registers_.l);}
void Instruction::ld_d_HL(CPU &cpu) {ld_r_HL(cpu, cpu.registers_.d);}
void Instruction::ld_d_a(CPU &cpu) {ld_r1_r2(cpu.registers_.d, cpu.registers_.a);}
void Instruction::ld_e_b(CPU &cpu) {ld_r1_r2(cpu.registers_.e, cpu.registers_.b);}
void Instruction::ld_e_c(CPU &cpu) {ld_r1_r2(cpu.registers_.e, cpu.registers_.c);}
void Instruction::ld_e_d(CPU &cpu) {ld_r1_r2(cpu.registers_.e, cpu.registers_.d);}
void Instruction::ld_e_e(CPU &cpu) {ld_r1_r2(cpu.registers_.e, cpu.registers_.e);}
void Instruction::ld_e_h(CPU &cpu) {ld_r1_r2(cpu.registers_.e, cpu.registers_.h);}
void Instruction::ld_e_l(CPU &cpu) {ld_r1_r2(cpu.registers_.e, cpu.registers_.l);}
void Instruction::ld_e_HL(CPU &cpu) {ld_r_HL(cpu, cpu.registers_.e);}
void Instruction::ld_e_a(CPU &cpu) {ld_r1_r2(cpu.registers_.e, cpu.registers_.a);}
//6x
void Instruction::ld_h_b(CPU &cpu) {ld_r1_r2(cpu.registers_.h, cpu.registers_.b);}
void Instruction::ld_h_c(CPU &cpu) {ld_r1_r2(cpu.registers_.h, cpu.registers_.c);}
void Instruction::ld_h_d(CPU &cpu) {ld_r1_r2(cpu.registers_.h, cpu.registers_.d);}
void Instruction::ld_h_e(CPU &cpu) {ld_r1_r2(cpu.registers_.h, cpu.registers_.e);}
void Instruction::ld_h_h(CPU &cpu) {ld_r1_r2(cpu.registers_.h, cpu.registers_.h);}
void Instruction::ld_h_l(CPU &cpu) {ld_r1_r2(cpu.registers_.h, cpu.registers_.l);}
void Instruction::ld_h_HL(CPU &cpu) {ld_r_HL(cpu, cpu.registers_.h);}
void Instruction::ld_h_a(CPU &cpu) {ld_r1_r2(cpu.registers_.h, cpu.registers_.a);}
void Instruction::ld_l_b(CPU &cpu) {ld_r1_r2(cpu.registers_.l, cpu.registers_.b);}
void Instruction::ld_l_c(CPU &cpu) {ld_r1_r2(cpu.registers_.l, cpu.registers_.c);}
void Instruction::ld_l_d(CPU &cpu) {ld_r1_r2(cpu.registers_.l, cpu.registers_.d);}
void Instruction::ld_l_e(CPU &cpu) {ld_r1_r2(cpu.registers_.l, cpu.registers_.e);}
void Instruction::ld_l_h(CPU &cpu) {ld_r1_r2(cpu.registers_.l, cpu.registers_.h);}
void Instruction::ld_l_l(CPU &cpu) {ld_r1_r2(cpu.registers_.l, cpu.registers_.l);}
void Instruction::ld_l_HL(CPU &cpu) {ld_r_HL(cpu, cpu.registers_.l);}
void Instruction::ld_l_a(CPU &cpu) {ld_r1_r2(cpu.registers_.l, cpu.registers_.a);}
//7x
void Instruction::ld_HL_b(CPU &cpu) {ld_HL_r(cpu, cpu.registers_.b);}
void Instruction::ld_HL_c(CPU &cpu) {ld_HL_r(cpu, cpu.registers_.c);}
void Instruction::ld_HL_d(CPU &cpu) {ld_HL_r(cpu, cpu.registers_.d);}
void Instruction::ld_HL_e(CPU &cpu) {ld_HL_r(cpu, cpu.registers_.e);}
void Instruction::ld_HL_h(CPU &cpu) {ld_HL_r(cpu, cpu.registers_.h);}
void Instruction::ld_HL_l(CPU &cpu) {ld_HL_r(cpu, cpu.registers_.l);}
void Instruction::ld_HL_a(CPU &cpu) {ld_HL_r(cpu, cpu.registers_.a);}
void Instruction::ld_a_b(CPU &cpu) {ld_r1_r2(cpu.registers_.a, cpu.registers_.b);}
void Instruction::ld_a_c(CPU &cpu) {ld_r1_r2(cpu.registers_.a, cpu.registers_.c);}
void Instruction::ld_a_d(CPU &cpu) {ld_r1_r2(cpu.registers_.a, cpu.registers_.d);}
void Instruction::ld_a_e(CPU &cpu) {ld_r1_r2(cpu.registers_.a, cpu.registers_.e);}
void Instruction::ld_a_h(CPU &cpu) {ld_r1_r2(cpu.registers_.a, cpu.registers_.h);}
void Instruction::ld_a_l(CPU &cpu) {ld_r1_r2(cpu.registers_.a, cpu.registers_.l);}
void Instruction::ld_a_HL(CPU &cpu) {ld_r_HL(cpu, cpu.registers_.a);}
void Instruction::ld_a_a(CPU &cpu) {ld_r1_r2(cpu.registers_.a, cpu.registers_.a);}
//8x
void Instruction::add_A_b(CPU &cpu) {add_A_r(cpu, cpu.registers_.b);}
void Instruction::add_A_c(CPU &cpu) {add_A_r(cpu, cpu.registers_.c);}
void Instruction::add_A_d(CPU &cpu) {add_A_r(cpu, cpu.registers_.d);}
void Instruction::add_A_e(CPU &cpu) {add_A_r(cpu, cpu.registers_.e);}
void Instruction::add_A_h(CPU &cpu) {add_A_r(cpu, cpu.registers_.h);}
void Instruction::add_A_l(CPU &cpu) {add_A_r(cpu, cpu.registers_.l);}
void Instruction::add_A_a(CPU &cpu) {add_A_r(cpu, cpu.registers_.a);}
void Instruction::adc_A_b(CPU &cpu) {adc_A_r(cpu, cpu.registers_.b);}
void Instruction::adc_A_c(CPU &cpu) {adc_A_r(cpu, cpu.registers_.c);}
void Instruction::adc_A_d(CPU &cpu) {adc_A_r(cpu, cpu.registers_.d);}
void Instruction::adc_A_e(CPU &cpu) {adc_A_r(cpu, cpu.registers_.e);}
void Instruction::adc_A_h(CPU &cpu) {adc_A_r(cpu, cpu.registers_.h);}
void Instruction::adc_A_l(CPU &cpu) {adc_A_r(cpu, cpu.registers_.l);}
void Instruction::adc_A_a(CPU &cpu) {adc_A_r(cpu, cpu.registers_.a);}
//9x
void Instruction::sub_A_b(CPU &cpu) {sub_r(cpu, cpu.registers_.b);}
void Instruction::sub_A_c(CPU &cpu) {sub_r(cpu, cpu.registers_.c);}
void Instruction::sub_A_d(CPU &cpu) {sub_r(cpu, cpu.registers_.d);}
void Instruction::sub_A_e(CPU &cpu) {sub_r(cpu, cpu.registers_.e);}
void Instruction::sub_A_h(CPU &cpu) {sub_r(cpu, cpu.registers_.h);}
void Instruction::sub_A_l(CPU &cpu) {sub_r(cpu, cpu.registers_.l);}
void Instruction::sub_A_a(CPU &cpu) {sub_r(cpu, cpu.registers_.a);}
void Instruction::sbc_A_b(CPU &cpu) {sbc_r(cpu, cpu.registers_.b);}
void Instruction::sbc_A_c(CPU &cpu) {sbc_r(cpu, cpu.registers_.c);}
void Instruction::sbc_A_d(CPU &cpu) {sbc_r(cpu, cpu.registers_.d);}
void Instruction::sbc_A_e(CPU &cpu) {sbc_r(cpu, cpu.registers_.e);}
void Instruction::sbc_A_h(CPU &cpu) {sbc_r(cpu, cpu.registers_.h);}
void Instruction::sbc_A_l(CPU &cpu) {sbc_r(cpu, cpu.registers_.l);}
void Instruction::sbc_A_a(CPU &cpu) {sbc_r(cpu, cpu.registers_.a);}
//Ax 
void Instruction::and_b(CPU &cpu) {and_r(cpu, cpu.registers_.b);}
void Instruction::and_c(CPU &cpu) {and_r(cpu, cpu.registers_.c);}
void Instruction::and_d(CPU &cpu) {and_r(cpu, cpu.registers_.d);}
void Instruction::and_e(CPU &cpu) {and_r(cpu, cpu.registers_.e);}
void Instruction::and_h(CPU &cpu) {and_r(cpu, cpu.registers_.h);}
void Instruction::and_l(CPU &cpu) {and_r(cpu, cpu.registers_.l);}
void Instruction::and_a(CPU &cpu) {and_r(cpu, cpu.registers_.a);}
void Instruction::xor_b(CPU &cpu) {xor_r(cpu, cpu.registers_.b);}
void Instruction::xor_c(CPU &cpu) {xor_r(cpu, cpu.registers_.c);}
void Instruction::xor_d(CPU &cpu) {xor_r(cpu, cpu.registers_.d);}
void Instruction::xor_e(CPU &cpu) {xor_r(cpu, cpu.registers_.e);}
void Instruction::xor_h(CPU &cpu) {xor_r(cpu, cpu.registers_.h);}
void Instruction::xor_l(CPU &cpu) {xor_r(cpu, cpu.registers_.l);}
void Instruction::xor_a(CPU &cpu) {xor_r(cpu, cpu.registers_.a);}
//Bx 
void Instruction::or_b(CPU &cpu) {or_r(cpu, cpu.registers_.b);}
void Instruction::or_c(CPU &cpu) {or_r(cpu, cpu.registers_.c);}
void Instruction::or_d(CPU &cpu) {or_r(cpu, cpu.registers_.d);}
void Instruction::or_e(CPU &cpu) {or_r(cpu, cpu.registers_.e);}
void Instruction::or_h(CPU &cpu) {or_r(cpu, cpu.registers_.h);}
void Instruction::or_l(CPU &cpu) {or_r(cpu, cpu.registers_.l);}
void Instruction::or_a(CPU &cpu) {or_r(cpu, cpu.registers_.a);}
void Instruction::cp_b(CPU &cpu) {cp_r(cpu, cpu.registers_.b);}
void Instruction::cp_c(CPU &cpu) {cp_r(cpu, cpu.registers_.c);}
void Instruction::cp_d(CPU &cpu) {cp_r(cpu, cpu.registers_.d);}
void Instruction::cp_e(CPU &cpu) {cp_r(cpu, cpu.registers_.e);}
void Instruction::cp_h(CPU &cpu) {cp_r(cpu, cpu.registers_.h);}
void Instruction::cp_l(CPU &cpu) {cp_r(cpu, cpu.registers_.l);}
void Instruction::cp_a(CPU &cpu) {cp_r(cpu, cpu.registers_.a);}
//Cx 
void Instruction::ret_nz_1(CPU &cpu) {ret_cc_1(cpu, ZERO, true);}
void Instruction::ret_nz_2(CPU &cpu) {ret_cc_2(cpu);}
void Instruction::ret_nz_3(CPU &cpu) {ret_cc_3(cpu);}
void Instruction::ret_nz_4(CPU &cpu) {ret_cc_4(cpu);}
void Instruction::pop_bc_1(CPU &cpu) {pop_rr_1(cpu, cpu.registers_.bc);}
void Instruction::pop_bc_2(CPU &cpu) {pop_rr_2(cpu, cpu.registers_.bc);}
void Instruction::jp_nz_nn_1(CPU &cpu) {jp_cc_nn_1(cpu);}
void Instruction::jp_nz_nn_2(CPU &cpu) {jp_cc_nn_2(cpu, ZERO, true);}
void Instruction::jp_nz_nn_3(CPU &cpu) {jp_cc_nn_3(cpu);}
void Instruction::call_nz_nn_1(CPU &cpu) {call_cc_nn_1(cpu);}
void Instruction::call_nz_nn_2(CPU &cpu) {call_cc_nn_2(cpu, ZERO, true);}
void Instruction::call_nz_nn_3(CPU &cpu) {call_cc_nn_3(cpu);}
void Instruction::call_nz_nn_4(CPU &cpu) {call_cc_nn_4(cpu);}
void Instruction::call_nz_nn_5(CPU &cpu) {call_cc_nn_5(cpu);}
void Instruction::push_bc_1(CPU &cpu) {push_rr_1(cpu, cpu.registers_.bc);}
void Instruction::push_bc_2(CPU &cpu) {push_rr_2(cpu, cpu.registers_.bc);}
void Instruction::push_bc_3(CPU &cpu) {push_rr_3(cpu, cpu.registers_.bc);}
void Instruction::rst_00h_1(CPU &cpu) {rst_1(cpu, 0x00);}
void Instruction::rst_00h_2(CPU &cpu) {rst_2(cpu, 0x00);}
void Instruction::rst_00h_3(CPU &cpu) {rst_3(cpu, 0x00);}
void Instruction::ret_z_1(CPU &cpu) {ret_cc_1(cpu, ZERO, false);}
void Instruction::ret_z_2(CPU &cpu) {ret_cc_2(cpu);}
void Instruction::ret_z_3(CPU &cpu) {ret_cc_3(cpu);}
void Instruction::ret_z_4(CPU &cpu) {ret_cc_4(cpu);}
void Instruction::jp_z_nn_1(CPU &cpu) {jp_cc_nn_1(cpu);}
void Instruction::jp_z_nn_2(CPU &cpu) {jp_cc_nn_2(cpu, ZERO, false);}
void Instruction::jp_z_nn_3(CPU &cpu) {jp_cc_nn_3(cpu);}
void Instruction::call_z_nn_1(CPU &cpu) {call_cc_nn_1(cpu);}
void Instruction::call_z_nn_2(CPU &cpu) {call_cc_nn_2(cpu, ZERO, false);}
void Instruction::call_z_nn_3(CPU &cpu) {call_cc_nn_3(cpu);}
void Instruction::call_z_nn_4(CPU &cpu) {call_cc_nn_4(cpu);}
void Instruction::call_z_nn_5(CPU &cpu) {call_cc_nn_5(cpu);}
void Instruction::rst_08h_1(CPU &cpu) {rst_1(cpu, 0x08);}
void Instruction::rst_08h_2(CPU &cpu) {rst_2(cpu, 0x08);}
void Instruction::rst_08h_3(CPU &cpu) {rst_3(cpu, 0x08);}
//Dx 
void Instruction::ret_nc_1(CPU &cpu) {ret_cc_1(cpu, CARRY, true);}
void Instruction::ret_nc_2(CPU &cpu) {ret_cc_2(cpu);}
void Instruction::ret_nc_3(CPU &cpu) {ret_cc_3(cpu);}
void Instruction::ret_nc_4(CPU &cpu) {ret_cc_4(cpu);}
void Instruction::pop_de_1(CPU &cpu) {pop_rr_1(cpu, cpu.registers_.de);}
void Instruction::pop_de_2(CPU &cpu) {pop_rr_2(cpu, cpu.registers_.de);}
void Instruction::jp_nc_nn_1(CPU &cpu) {jp_cc_nn_1(cpu);}
void Instruction::jp_nc_nn_2(CPU &cpu) {jp_cc_nn_2(cpu, CARRY, true);}
void Instruction::jp_nc_nn_3(CPU &cpu) {jp_cc_nn_3(cpu);}
void Instruction::call_nc_nn_1(CPU &cpu) {call_cc_nn_1(cpu);}
void Instruction::call_nc_nn_2(CPU &cpu) {call_cc_nn_2(cpu, CARRY, true);}
void Instruction::call_nc_nn_3(CPU &cpu) {call_cc_nn_3(cpu);}
void Instruction::call_nc_nn_4(CPU &cpu) {call_cc_nn_4(cpu);}
void Instruction::call_nc_nn_5(CPU &cpu) {call_cc_nn_5(cpu);}
void Instruction::push_de_1(CPU &cpu) {push_rr_1(cpu, cpu.registers_.de);}
void Instruction::push_de_2(CPU &cpu) {push_rr_2(cpu, cpu.registers_.de);}
void Instruction::push_de_3(CPU &cpu) {push_rr_3(cpu, cpu.registers_.de);}
void Instruction::rst_10h_1(CPU &cpu) {rst_1(cpu, 0x10);}
void Instruction::rst_10h_2(CPU &cpu) {rst_2(cpu, 0x10);}
void Instruction::rst_10h_3(CPU &cpu) {rst_3(cpu, 0x10);}
void Instruction::ret_c_1(CPU &cpu) {ret_cc_1(cpu, CARRY, false);}
void Instruction::ret_c_2(CPU &cpu) {ret_cc_2(cpu);}
void Instruction::ret_c_3(CPU &cpu) {ret_cc_3(cpu);}
void Instruction::ret_c_4(CPU &cpu) {ret_cc_4(cpu);}
void Instruction::jp_c_nn_1(CPU &cpu) {jp_cc_nn_1(cpu);}
void Instruction::jp_c_nn_2(CPU &cpu) {jp_cc_nn_2(cpu, CARRY, false);}
void Instruction::jp_c_nn_3(CPU &cpu) {jp_cc_nn_3(cpu);}
void Instruction::call_c_nn_1(CPU &cpu) {call_cc_nn_1(cpu);}
void Instruction::call_c_nn_2(CPU &cpu) {call_cc_nn_2(cpu, CARRY, false);}
void Instruction::call_c_nn_3(CPU &cpu) {call_cc_nn_3(cpu);}
void Instruction::call_c_nn_4(CPU &cpu) {call_cc_nn_4(cpu);}
void Instruction::call_c_nn_5(CPU &cpu) {call_cc_nn_5(cpu);}
void Instruction::rst_18h_1(CPU &cpu) {rst_1(cpu, 0x08);}
void Instruction::rst_18h_2(CPU &cpu) {rst_2(cpu, 0x08);}
void Instruction::rst_18h_3(CPU &cpu) {rst_3(cpu, 0x08);}
//Ex 
void Instruction::pop_hl_1(CPU &cpu) {pop_rr_1(cpu, cpu.registers_.hl);}
void Instruction::pop_hl_2(CPU &cpu) {pop_rr_2(cpu, cpu.registers_.hl);}
void Instruction::push_hl_1(CPU &cpu) {push_rr_1(cpu, cpu.registers_.hl);}
void Instruction::push_hl_2(CPU &cpu) {push_rr_2(cpu, cpu.registers_.hl);}
void Instruction::push_hl_3(CPU &cpu) {push_rr_3(cpu, cpu.registers_.hl);}
void Instruction::rst_20h_1(CPU &cpu) {rst_1(cpu, 0x20);}
void Instruction::rst_20h_2(CPU &cpu) {rst_2(cpu, 0x20);}
void Instruction::rst_20h_3(CPU &cpu) {rst_3(cpu, 0x20);}
void Instruction::rst_28h_1(CPU &cpu) {rst_1(cpu, 0x28);}
void Instruction::rst_28h_2(CPU &cpu) {rst_2(cpu, 0x28);}
void Instruction::rst_28h_3(CPU &cpu) {rst_3(cpu, 0x28);}
//Fx 
void Instruction::pop_af_1(CPU &cpu) {pop_rr_1(cpu, cpu.registers_.af);}
void Instruction::pop_af_2(CPU &cpu) {pop_rr_2(cpu, cpu.registers_.af);}
void Instruction::push_af_1(CPU &cpu) {push_rr_1(cpu, cpu.registers_.af);}
void Instruction::push_af_2(CPU &cpu) {push_rr_2(cpu, cpu.registers_.af);}
void Instruction::push_af_3(CPU &cpu) {push_rr_3(cpu, cpu.registers_.af);}
void Instruction::rst_30h_1(CPU &cpu) {rst_1(cpu, 0x30);}
void Instruction::rst_30h_2(CPU &cpu) {rst_2(cpu, 0x30);}
void Instruction::rst_30h_3(CPU &cpu) {rst_3(cpu, 0x30);}
void Instruction::rst_38h_1(CPU &cpu) {rst_1(cpu, 0x38);}
void Instruction::rst_38h_2(CPU &cpu) {rst_2(cpu, 0x38);}
void Instruction::rst_38h_3(CPU &cpu) {rst_3(cpu, 0x38);}

//0xCB prefixed instructions
//0x
void Instruction::rlc_b(CPU &cpu) {rlc_r(cpu, cpu.registers_.b);}
void Instruction::rlc_c(CPU &cpu) {rlc_r(cpu, cpu.registers_.c);}
void Instruction::rlc_d(CPU &cpu) {rlc_r(cpu, cpu.registers_.d);}
void Instruction::rlc_e(CPU &cpu) {rlc_r(cpu, cpu.registers_.e);}
void Instruction::rlc_h(CPU &cpu) {rlc_r(cpu, cpu.registers_.h);}
void Instruction::rlc_l(CPU &cpu) {rlc_r(cpu, cpu.registers_.l);}
void Instruction::rlc_a(CPU &cpu) {rlc_r(cpu, cpu.registers_.a);}
void Instruction::rrc_b(CPU &cpu) {rrc_r(cpu, cpu.registers_.b);}
void Instruction::rrc_c(CPU &cpu) {rrc_r(cpu, cpu.registers_.c);}
void Instruction::rrc_d(CPU &cpu) {rrc_r(cpu, cpu.registers_.d);}
void Instruction::rrc_e(CPU &cpu) {rrc_r(cpu, cpu.registers_.e);}
void Instruction::rrc_h(CPU &cpu) {rrc_r(cpu, cpu.registers_.h);}
void Instruction::rrc_l(CPU &cpu) {rrc_r(cpu, cpu.registers_.l);}
void Instruction::rrc_a(CPU &cpu) {rrc_r(cpu, cpu.registers_.a);}
//1x 
void Instruction::rl_b(CPU &cpu) {rl_r(cpu, cpu.registers_.b);}
void Instruction::rl_c(CPU &cpu) {rl_r(cpu, cpu.registers_.c);}
void Instruction::rl_d(CPU &cpu) {rl_r(cpu, cpu.registers_.d);}
void Instruction::rl_e(CPU &cpu) {rl_r(cpu, cpu.registers_.e);}
void Instruction::rl_h(CPU &cpu) {rl_r(cpu, cpu.registers_.h);}
void Instruction::rl_l(CPU &cpu) {rl_r(cpu, cpu.registers_.l);}
void Instruction::rl_a(CPU &cpu) {rl_r(cpu, cpu.registers_.a);}
void Instruction::rr_b(CPU &cpu) {rr_r(cpu, cpu.registers_.b);}
void Instruction::rr_c(CPU &cpu) {rr_r(cpu, cpu.registers_.c);}
void Instruction::rr_d(CPU &cpu) {rr_r(cpu, cpu.registers_.d);}
void Instruction::rr_e(CPU &cpu) {rr_r(cpu, cpu.registers_.e);}
void Instruction::rr_h(CPU &cpu) {rr_r(cpu, cpu.registers_.h);}
void Instruction::rr_l(CPU &cpu) {rr_r(cpu, cpu.registers_.l);}
void Instruction::rr_a(CPU &cpu) {rr_r(cpu, cpu.registers_.a);}
//2x 
void Instruction::sla_b(CPU &cpu) {sla_r(cpu, cpu.registers_.b);}
void Instruction::sla_c(CPU &cpu) {sla_r(cpu, cpu.registers_.c);}
void Instruction::sla_d(CPU &cpu) {sla_r(cpu, cpu.registers_.d);}
void Instruction::sla_e(CPU &cpu) {sla_r(cpu, cpu.registers_.e);}
void Instruction::sla_h(CPU &cpu) {sla_r(cpu, cpu.registers_.h);}
void Instruction::sla_l(CPU &cpu) {sla_r(cpu, cpu.registers_.l);}
void Instruction::sla_a(CPU &cpu) {sla_r(cpu, cpu.registers_.a);}
void Instruction::sra_b(CPU &cpu) {sra_r(cpu, cpu.registers_.b);}
void Instruction::sra_c(CPU &cpu) {sra_r(cpu, cpu.registers_.c);}
void Instruction::sra_d(CPU &cpu) {sra_r(cpu, cpu.registers_.d);}
void Instruction::sra_e(CPU &cpu) {sra_r(cpu, cpu.registers_.e);}
void Instruction::sra_h(CPU &cpu) {sra_r(cpu, cpu.registers_.h);}
void Instruction::sra_l(CPU &cpu) {sra_r(cpu, cpu.registers_.l);}
void Instruction::sra_a(CPU &cpu) {sra_r(cpu, cpu.registers_.a);}
//3x 
void Instruction::swap_b(CPU &cpu) {swap_r(cpu, cpu.registers_.b);}
void Instruction::swap_c(CPU &cpu) {swap_r(cpu, cpu.registers_.c);}
void Instruction::swap_d(CPU &cpu) {swap_r(cpu, cpu.registers_.d);}
void Instruction::swap_e(CPU &cpu) {swap_r(cpu, cpu.registers_.e);}
void Instruction::swap_h(CPU &cpu) {swap_r(cpu, cpu.registers_.h);}
void Instruction::swap_l(CPU &cpu) {swap_r(cpu, cpu.registers_.l);}
void Instruction::swap_a(CPU &cpu) {swap_r(cpu, cpu.registers_.a);}
void Instruction::srl_b(CPU &cpu) {srl_r(cpu, cpu.registers_.b);}
void Instruction::srl_c(CPU &cpu) {srl_r(cpu, cpu.registers_.c);}
void Instruction::srl_d(CPU &cpu) {srl_r(cpu, cpu.registers_.d);}
void Instruction::srl_e(CPU &cpu) {srl_r(cpu, cpu.registers_.e);}
void Instruction::srl_h(CPU &cpu) {srl_r(cpu, cpu.registers_.h);}
void Instruction::srl_l(CPU &cpu) {srl_r(cpu, cpu.registers_.l);}
void Instruction::srl_a(CPU &cpu) {srl_r(cpu, cpu.registers_.a);}
//4x 
void Instruction::bit_0_b(CPU &cpu) {bit_n_r(cpu, 0, cpu.registers_.b);}
void Instruction::bit_0_c(CPU &cpu) {bit_n_r(cpu, 0, cpu.registers_.c);}
void Instruction::bit_0_d(CPU &cpu) {bit_n_r(cpu, 0, cpu.registers_.d);}
void Instruction::bit_0_e(CPU &cpu) {bit_n_r(cpu, 0, cpu.registers_.e);}
void Instruction::bit_0_h(CPU &cpu) {bit_n_r(cpu, 0, cpu.registers_.h);}
void Instruction::bit_0_l(CPU &cpu) {bit_n_r(cpu, 0, cpu.registers_.l);}
void Instruction::bit_0_HL_1(CPU &cpu) {bit_n_HL_1(cpu, 0);}
void Instruction::bit_0_HL_2(CPU &cpu) {bit_n_HL_2(cpu);}
void Instruction::bit_0_a(CPU &cpu) {bit_n_r(cpu, 0, cpu.registers_.a);}
void Instruction::bit_1_b(CPU &cpu) {bit_n_r(cpu, 1, cpu.registers_.b);}
void Instruction::bit_1_c(CPU &cpu) {bit_n_r(cpu, 1, cpu.registers_.c);}
void Instruction::bit_1_d(CPU &cpu) {bit_n_r(cpu, 1, cpu.registers_.d);}
void Instruction::bit_1_e(CPU &cpu) {bit_n_r(cpu, 1, cpu.registers_.e);}
void Instruction::bit_1_h(CPU &cpu) {bit_n_r(cpu, 1, cpu.registers_.h);}
void Instruction::bit_1_l(CPU &cpu) {bit_n_r(cpu, 1, cpu.registers_.l);}
void Instruction::bit_1_HL_1(CPU &cpu) {bit_n_HL_1(cpu, 1);}
void Instruction::bit_1_HL_2(CPU &cpu) {bit_n_HL_2(cpu);}
void Instruction::bit_1_a(CPU &cpu) {bit_n_r(cpu, 1, cpu.registers_.a);}
//5x 
void Instruction::bit_2_b(CPU &cpu) {bit_n_r(cpu, 2, cpu.registers_.b);}
void Instruction::bit_2_c(CPU &cpu) {bit_n_r(cpu, 2, cpu.registers_.c);}
void Instruction::bit_2_d(CPU &cpu) {bit_n_r(cpu, 2, cpu.registers_.d);}
void Instruction::bit_2_e(CPU &cpu) {bit_n_r(cpu, 2, cpu.registers_.e);}
void Instruction::bit_2_h(CPU &cpu) {bit_n_r(cpu, 2, cpu.registers_.h);}
void Instruction::bit_2_l(CPU &cpu) {bit_n_r(cpu, 2, cpu.registers_.l);}
void Instruction::bit_2_HL_1(CPU &cpu) {bit_n_HL_1(cpu, 2);}
void Instruction::bit_2_HL_2(CPU &cpu) {bit_n_HL_2(cpu);}
void Instruction::bit_2_a(CPU &cpu) {bit_n_r(cpu, 2, cpu.registers_.a);}
void Instruction::bit_3_b(CPU &cpu) {bit_n_r(cpu, 3, cpu.registers_.b);}
void Instruction::bit_3_c(CPU &cpu) {bit_n_r(cpu, 3, cpu.registers_.c);}
void Instruction::bit_3_d(CPU &cpu) {bit_n_r(cpu, 3, cpu.registers_.d);}
void Instruction::bit_3_e(CPU &cpu) {bit_n_r(cpu, 3, cpu.registers_.e);}
void Instruction::bit_3_h(CPU &cpu) {bit_n_r(cpu, 3, cpu.registers_.h);}
void Instruction::bit_3_l(CPU &cpu) {bit_n_r(cpu, 3, cpu.registers_.l);}
void Instruction::bit_3_HL_1(CPU &cpu) {bit_n_HL_1(cpu, 3);}
void Instruction::bit_3_HL_2(CPU &cpu) {bit_n_HL_2(cpu);}
void Instruction::bit_3_a(CPU &cpu) {bit_n_r(cpu, 3, cpu.registers_.a);}
//6x 
void Instruction::bit_4_b(CPU &cpu) {bit_n_r(cpu, 4, cpu.registers_.b);}
void Instruction::bit_4_c(CPU &cpu) {bit_n_r(cpu, 4, cpu.registers_.c);}
void Instruction::bit_4_d(CPU &cpu) {bit_n_r(cpu, 4, cpu.registers_.d);}
void Instruction::bit_4_e(CPU &cpu) {bit_n_r(cpu, 4, cpu.registers_.e);}
void Instruction::bit_4_h(CPU &cpu) {bit_n_r(cpu, 4, cpu.registers_.h);}
void Instruction::bit_4_l(CPU &cpu) {bit_n_r(cpu, 4, cpu.registers_.l);}
void Instruction::bit_4_HL_1(CPU &cpu) {bit_n_HL_1(cpu, 4);}
void Instruction::bit_4_HL_2(CPU &cpu) {bit_n_HL_2(cpu);}
void Instruction::bit_4_a(CPU &cpu) {bit_n_r(cpu, 4, cpu.registers_.a);}
void Instruction::bit_5_b(CPU &cpu) {bit_n_r(cpu, 5, cpu.registers_.b);}
void Instruction::bit_5_c(CPU &cpu) {bit_n_r(cpu, 5, cpu.registers_.c);}
void Instruction::bit_5_d(CPU &cpu) {bit_n_r(cpu, 5, cpu.registers_.d);}
void Instruction::bit_5_e(CPU &cpu) {bit_n_r(cpu, 5, cpu.registers_.e);}
void Instruction::bit_5_h(CPU &cpu) {bit_n_r(cpu, 5, cpu.registers_.h);}
void Instruction::bit_5_l(CPU &cpu) {bit_n_r(cpu, 5, cpu.registers_.l);}
void Instruction::bit_5_HL_1(CPU &cpu) {bit_n_HL_1(cpu, 5);}
void Instruction::bit_5_HL_2(CPU &cpu) {bit_n_HL_2(cpu);}
void Instruction::bit_5_a(CPU &cpu) {bit_n_r(cpu, 5, cpu.registers_.a);}
//7x 
void Instruction::bit_6_b(CPU &cpu) {bit_n_r(cpu, 6, cpu.registers_.b);}
void Instruction::bit_6_c(CPU &cpu) {bit_n_r(cpu, 6, cpu.registers_.c);}
void Instruction::bit_6_d(CPU &cpu) {bit_n_r(cpu, 6, cpu.registers_.d);}
void Instruction::bit_6_e(CPU &cpu) {bit_n_r(cpu, 6, cpu.registers_.e);}
void Instruction::bit_6_h(CPU &cpu) {bit_n_r(cpu, 6, cpu.registers_.h);}
void Instruction::bit_6_l(CPU &cpu) {bit_n_r(cpu, 6, cpu.registers_.l);}
void Instruction::bit_6_HL_1(CPU &cpu) {bit_n_HL_1(cpu, 6);}
void Instruction::bit_6_HL_2(CPU &cpu) {bit_n_HL_2(cpu);}
void Instruction::bit_6_a(CPU &cpu) {bit_n_r(cpu, 6, cpu.registers_.a);}
void Instruction::bit_7_b(CPU &cpu) {bit_n_r(cpu, 7, cpu.registers_.b);}
void Instruction::bit_7_c(CPU &cpu) {bit_n_r(cpu, 7, cpu.registers_.c);}
void Instruction::bit_7_d(CPU &cpu) {bit_n_r(cpu, 7, cpu.registers_.d);}
void Instruction::bit_7_e(CPU &cpu) {bit_n_r(cpu, 7, cpu.registers_.e);}
void Instruction::bit_7_h(CPU &cpu) {bit_n_r(cpu, 7, cpu.registers_.h);}
void Instruction::bit_7_l(CPU &cpu) {bit_n_r(cpu, 7, cpu.registers_.l);}
void Instruction::bit_7_HL_1(CPU &cpu) {bit_n_HL_1(cpu, 7);}
void Instruction::bit_7_HL_2(CPU &cpu) {bit_n_HL_2(cpu);}
void Instruction::bit_7_a(CPU &cpu) {bit_n_r(cpu, 7, cpu.registers_.a);}
//8x 
void Instruction::res_0_b(CPU &cpu) {res_n_r(0, cpu.registers_.b);}
void Instruction::res_0_c(CPU &cpu) {res_n_r(0, cpu.registers_.c);}
void Instruction::res_0_d(CPU &cpu) {res_n_r(0, cpu.registers_.d);}
void Instruction::res_0_e(CPU &cpu) {res_n_r(0, cpu.registers_.e);}
void Instruction::res_0_h(CPU &cpu) {res_n_r(0, cpu.registers_.h);}
void Instruction::res_0_l(CPU &cpu) {res_n_r(0, cpu.registers_.l);}
void Instruction::res_0_HL_1(CPU &cpu) {res_n_HL_1(cpu, 0);}
void Instruction::res_0_HL_2(CPU &cpu) {res_n_HL_2(cpu);}
void Instruction::res_0_a(CPU &cpu) {res_n_r(0, cpu.registers_.a);}
void Instruction::res_1_b(CPU &cpu) {res_n_r(1, cpu.registers_.b);}
void Instruction::res_1_c(CPU &cpu) {res_n_r(1, cpu.registers_.c);}
void Instruction::res_1_d(CPU &cpu) {res_n_r(1, cpu.registers_.d);}
void Instruction::res_1_e(CPU &cpu) {res_n_r(1, cpu.registers_.e);}
void Instruction::res_1_h(CPU &cpu) {res_n_r(1, cpu.registers_.h);}
void Instruction::res_1_l(CPU &cpu) {res_n_r(1, cpu.registers_.l);}
void Instruction::res_1_HL_1(CPU &cpu) {res_n_HL_1(cpu, 1);}
void Instruction::res_1_HL_2(CPU &cpu) {res_n_HL_2(cpu);}
void Instruction::res_1_a(CPU &cpu) {res_n_r(1, cpu.registers_.a);}
//9x 
void Instruction::res_2_b(CPU &cpu) {res_n_r(2, cpu.registers_.b);}
void Instruction::res_2_c(CPU &cpu) {res_n_r(2, cpu.registers_.c);}
void Instruction::res_2_d(CPU &cpu) {res_n_r(2, cpu.registers_.d);}
void Instruction::res_2_e(CPU &cpu) {res_n_r(2, cpu.registers_.e);}
void Instruction::res_2_h(CPU &cpu) {res_n_r(2, cpu.registers_.h);}
void Instruction::res_2_l(CPU &cpu) {res_n_r(2, cpu.registers_.l);}
void Instruction::res_2_HL_1(CPU &cpu) {res_n_HL_1(cpu, 2);}
void Instruction::res_2_HL_2(CPU &cpu) {res_n_HL_2(cpu);}
void Instruction::res_2_a(CPU &cpu) {res_n_r(2, cpu.registers_.a);}
void Instruction::res_3_b(CPU &cpu) {res_n_r(3, cpu.registers_.b);}
void Instruction::res_3_c(CPU &cpu) {res_n_r(3, cpu.registers_.c);}
void Instruction::res_3_d(CPU &cpu) {res_n_r(3, cpu.registers_.d);}
void Instruction::res_3_e(CPU &cpu) {res_n_r(3, cpu.registers_.e);}
void Instruction::res_3_h(CPU &cpu) {res_n_r(3, cpu.registers_.h);}
void Instruction::res_3_l(CPU &cpu) {res_n_r(3, cpu.registers_.l);}
void Instruction::res_3_HL_1(CPU &cpu) {res_n_HL_1(cpu, 3);}
void Instruction::res_3_HL_2(CPU &cpu) {res_n_HL_2(cpu);}
void Instruction::res_3_a(CPU &cpu) {res_n_r(3, cpu.registers_.a);}
//Ax 
void Instruction::res_4_b(CPU &cpu) {res_n_r(4, cpu.registers_.b);}
void Instruction::res_4_c(CPU &cpu) {res_n_r(4, cpu.registers_.c);}
void Instruction::res_4_d(CPU &cpu) {res_n_r(4, cpu.registers_.d);}
void Instruction::res_4_e(CPU &cpu) {res_n_r(4, cpu.registers_.e);}
void Instruction::res_4_h(CPU &cpu) {res_n_r(4, cpu.registers_.h);}
void Instruction::res_4_l(CPU &cpu) {res_n_r(4, cpu.registers_.l);}
void Instruction::res_4_HL_1(CPU &cpu) {res_n_HL_1(cpu, 4);}
void Instruction::res_4_HL_2(CPU &cpu) {res_n_HL_2(cpu);}
void Instruction::res_4_a(CPU &cpu) {res_n_r(4, cpu.registers_.a);}
void Instruction::res_5_b(CPU &cpu) {res_n_r(5, cpu.registers_.b);}
void Instruction::res_5_c(CPU &cpu) {res_n_r(5, cpu.registers_.c);}
void Instruction::res_5_d(CPU &cpu) {res_n_r(5, cpu.registers_.d);}
void Instruction::res_5_e(CPU &cpu) {res_n_r(5, cpu.registers_.e);}
void Instruction::res_5_h(CPU &cpu) {res_n_r(5, cpu.registers_.h);}
void Instruction::res_5_l(CPU &cpu) {res_n_r(5, cpu.registers_.l);}
void Instruction::res_5_HL_1(CPU &cpu) {res_n_HL_1(cpu, 5);}
void Instruction::res_5_HL_2(CPU &cpu) {res_n_HL_2(cpu);}
void Instruction::res_5_a(CPU &cpu) {res_n_r(5, cpu.registers_.a);}
//Bx 
void Instruction::res_6_b(CPU &cpu) {res_n_r(6, cpu.registers_.b);}
void Instruction::res_6_c(CPU &cpu) {res_n_r(6, cpu.registers_.c);}
void Instruction::res_6_d(CPU &cpu) {res_n_r(6, cpu.registers_.d);}
void Instruction::res_6_e(CPU &cpu) {res_n_r(6, cpu.registers_.e);}
void Instruction::res_6_h(CPU &cpu) {res_n_r(6, cpu.registers_.h);}
void Instruction::res_6_l(CPU &cpu) {res_n_r(6, cpu.registers_.l);}
void Instruction::res_6_HL_1(CPU &cpu) {res_n_HL_1(cpu, 6);}
void Instruction::res_6_HL_2(CPU &cpu) {res_n_HL_2(cpu);}
void Instruction::res_6_a(CPU &cpu) {res_n_r(6, cpu.registers_.a);}
void Instruction::res_7_b(CPU &cpu) {res_n_r(7, cpu.registers_.b);}
void Instruction::res_7_c(CPU &cpu) {res_n_r(7, cpu.registers_.c);}
void Instruction::res_7_d(CPU &cpu) {res_n_r(7, cpu.registers_.d);}
void Instruction::res_7_e(CPU &cpu) {res_n_r(7, cpu.registers_.e);}
void Instruction::res_7_h(CPU &cpu) {res_n_r(7, cpu.registers_.h);}
void Instruction::res_7_l(CPU &cpu) {res_n_r(7, cpu.registers_.l);}
void Instruction::res_7_HL_1(CPU &cpu) {res_n_HL_1(cpu, 7);}
void Instruction::res_7_HL_2(CPU &cpu) {res_n_HL_2(cpu);}
void Instruction::res_7_a(CPU &cpu) {res_n_r(7, cpu.registers_.a);}
//Cx 
void Instruction::set_0_b(CPU &cpu) {set_n_r(0, cpu.registers_.b);}
void Instruction::set_0_c(CPU &cpu) {set_n_r(0, cpu.registers_.c);}
void Instruction::set_0_d(CPU &cpu) {set_n_r(0, cpu.registers_.d);}
void Instruction::set_0_e(CPU &cpu) {set_n_r(0, cpu.registers_.e);}
void Instruction::set_0_h(CPU &cpu) {set_n_r(0, cpu.registers_.h);}
void Instruction::set_0_l(CPU &cpu) {set_n_r(0, cpu.registers_.l);}
void Instruction::set_0_HL_1(CPU &cpu) {set_n_HL_1(cpu, 0);}
void Instruction::set_0_HL_2(CPU &cpu) {set_n_HL_2(cpu);}
void Instruction::set_0_a(CPU &cpu) {set_n_r(0, cpu.registers_.a);}
void Instruction::set_1_b(CPU &cpu) {set_n_r(1, cpu.registers_.b);}
void Instruction::set_1_c(CPU &cpu) {set_n_r(1, cpu.registers_.c);}
void Instruction::set_1_d(CPU &cpu) {set_n_r(1, cpu.registers_.d);}
void Instruction::set_1_e(CPU &cpu) {set_n_r(1, cpu.registers_.e);}
void Instruction::set_1_h(CPU &cpu) {set_n_r(1, cpu.registers_.h);}
void Instruction::set_1_l(CPU &cpu) {set_n_r(1, cpu.registers_.l);}
void Instruction::set_1_HL_1(CPU &cpu) {set_n_HL_1(cpu, 1);}
void Instruction::set_1_HL_2(CPU &cpu) {set_n_HL_2(cpu);}
void Instruction::set_1_a(CPU &cpu) {set_n_r(1, cpu.registers_.a);}
//Dx 
void Instruction::set_2_b(CPU &cpu) {set_n_r(2, cpu.registers_.b);}
void Instruction::set_2_c(CPU &cpu) {set_n_r(2, cpu.registers_.c);}
void Instruction::set_2_d(CPU &cpu) {set_n_r(2, cpu.registers_.d);}
void Instruction::set_2_e(CPU &cpu) {set_n_r(2, cpu.registers_.e);}
void Instruction::set_2_h(CPU &cpu) {set_n_r(2, cpu.registers_.h);}
void Instruction::set_2_l(CPU &cpu) {set_n_r(2, cpu.registers_.l);}
void Instruction::set_2_HL_1(CPU &cpu) {set_n_HL_1(cpu, 2);}
void Instruction::set_2_HL_2(CPU &cpu) {set_n_HL_2(cpu);}
void Instruction::set_2_a(CPU &cpu) {set_n_r(2, cpu.registers_.a);}
void Instruction::set_3_b(CPU &cpu) {set_n_r(3, cpu.registers_.b);}
void Instruction::set_3_c(CPU &cpu) {set_n_r(3, cpu.registers_.c);}
void Instruction::set_3_d(CPU &cpu) {set_n_r(3, cpu.registers_.d);}
void Instruction::set_3_e(CPU &cpu) {set_n_r(3, cpu.registers_.e);}
void Instruction::set_3_h(CPU &cpu) {set_n_r(3, cpu.registers_.h);}
void Instruction::set_3_l(CPU &cpu) {set_n_r(3, cpu.registers_.l);}
void Instruction::set_3_HL_1(CPU &cpu) {set_n_HL_1(cpu, 3);}
void Instruction::set_3_HL_2(CPU &cpu) {set_n_HL_2(cpu);}
void Instruction::set_3_a(CPU &cpu) {set_n_r(3, cpu.registers_.a);}
//Ex 
void Instruction::set_4_b(CPU &cpu) {set_n_r(4, cpu.registers_.b);}
void Instruction::set_4_c(CPU &cpu) {set_n_r(4, cpu.registers_.c);}
void Instruction::set_4_d(CPU &cpu) {set_n_r(4, cpu.registers_.d);}
void Instruction::set_4_e(CPU &cpu) {set_n_r(4, cpu.registers_.e);}
void Instruction::set_4_h(CPU &cpu) {set_n_r(4, cpu.registers_.h);}
void Instruction::set_4_l(CPU &cpu) {set_n_r(4, cpu.registers_.l);}
void Instruction::set_4_HL_1(CPU &cpu) {set_n_HL_1(cpu, 4);}
void Instruction::set_4_HL_2(CPU &cpu) {set_n_HL_2(cpu);}
void Instruction::set_4_a(CPU &cpu) {set_n_r(4, cpu.registers_.a);}
void Instruction::set_5_b(CPU &cpu) {set_n_r(5, cpu.registers_.b);}
void Instruction::set_5_c(CPU &cpu) {set_n_r(5, cpu.registers_.c);}
void Instruction::set_5_d(CPU &cpu) {set_n_r(5, cpu.registers_.d);}
void Instruction::set_5_e(CPU &cpu) {set_n_r(5, cpu.registers_.e);}
void Instruction::set_5_h(CPU &cpu) {set_n_r(5, cpu.registers_.h);}
void Instruction::set_5_l(CPU &cpu) {set_n_r(5, cpu.registers_.l);}
void Instruction::set_5_HL_1(CPU &cpu) {set_n_HL_1(cpu, 5);}
void Instruction::set_5_HL_2(CPU &cpu) {set_n_HL_2(cpu);}
void Instruction::set_5_a(CPU &cpu) {set_n_r(5, cpu.registers_.a);}
//Fx 
void Instruction::set_6_b(CPU &cpu) {set_n_r(6, cpu.registers_.b);}
void Instruction::set_6_c(CPU &cpu) {set_n_r(6, cpu.registers_.c);}
void Instruction::set_6_d(CPU &cpu) {set_n_r(6, cpu.registers_.d);}
void Instruction::set_6_e(CPU &cpu) {set_n_r(6, cpu.registers_.e);}
void Instruction::set_6_h(CPU &cpu) {set_n_r(6, cpu.registers_.h);}
void Instruction::set_6_l(CPU &cpu) {set_n_r(6, cpu.registers_.l);}
void Instruction::set_6_HL_1(CPU &cpu) {set_n_HL_1(cpu, 6);}
void Instruction::set_6_HL_2(CPU &cpu) {set_n_HL_2(cpu);}
void Instruction::set_6_a(CPU &cpu) {set_n_r(6, cpu.registers_.a);}
void Instruction::set_7_b(CPU &cpu) {set_n_r(7, cpu.registers_.b);}
void Instruction::set_7_c(CPU &cpu) {set_n_r(7, cpu.registers_.c);}
void Instruction::set_7_d(CPU &cpu) {set_n_r(7, cpu.registers_.d);}
void Instruction::set_7_e(CPU &cpu) {set_n_r(7, cpu.registers_.e);}
void Instruction::set_7_h(CPU &cpu) {set_n_r(7, cpu.registers_.h);}
void Instruction::set_7_l(CPU &cpu) {set_n_r(7, cpu.registers_.l);}
void Instruction::set_7_HL_1(CPU &cpu) {set_n_HL_1(cpu, 7);}
void Instruction::set_7_HL_2(CPU &cpu) {set_n_HL_2(cpu);}
void Instruction::set_7_a(CPU &cpu) {set_n_r(7, cpu.registers_.a);}