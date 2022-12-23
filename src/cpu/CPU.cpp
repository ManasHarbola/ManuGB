#include "CPU.h"
#include <cstdio>

CPU::CPU(MMU &mmu, PPU &ppu, Timer &timer, InterruptManager &int_manager) :
mmu_(mmu), ppu_(ppu), timer_(timer),
state_(CPUState::FETCH), int_manager_(int_manager) {
    //initialize registers to start values
    init_registers();
}

CPU::~CPU() {

}

void CPU::write_state_to_log(std::ofstream& os) {
    if (os.is_open()) {
        //"A:01 F:B0 B:00 C:13 D:00 E:D8 H:01 L:4D SP:FFFE PC:0100 PCMEM:00,C3,13,02";
        char buf[74];
        snprintf(buf, 74,
                 "A:%02X F:%02X B:%02X C:%02X D:%02X E:%02X H:%02X L:%02X SP:%04X PC:%04X PCMEM:%02X,%02X,%02X,%02X",
                 registers_.a, registers_.f, registers_.b,
                 registers_.c, registers_.d, registers_.e,
                 registers_.h, registers_.l, registers_.sp,
                 registers_.pc,
                 mmu_.read(registers_.pc), mmu_.read(registers_.pc+1),
                 mmu_.read(registers_.pc+2), mmu_.read(registers_.pc+3));
        os << std::string(buf) << std::endl;   
    }
}

void CPU::init_registers() {
    //initialize register, flag register, sp, and pc
    registers_.af = 0x01B0;
    registers_.bc = 0x0013;
    registers_.de = 0x00D8;
    registers_.hl = 0x014D;
    registers_.sp = 0xFFFE;
    registers_.pc = 0x0100;
}

void CPU::check_for_interrupt() {
    //reset inst_finished_ flag to false
    inst_finished_ = false;
    //check if IME is enabled and interrupt was requested
    if (int_manager_.get_IME() && int_manager_.interrupt_requested()) {
        //disable IME so no other interrupts are serviced at this time
        int_manager_.disable_IME();
        state_ = INT_NOP1;
    } else {
        //get ready to fetch next instruction
        state_ = FETCH;
    }
}

void CPU::tick() {
    t_cycle_lock_++;
    total_t_cycles_++;

    //return if we haven't completed an m-cycle
    if (t_cycle_lock_ < 4) {
        return;
    }
    t_cycle_lock_ = 0;

    switch (state_)
    {
        case FETCH:
        {
            if (halt_bug_)
                halt_bug_ = false;

            //check for pending IME enable
            if (int_manager_.pending_enable_IME()) {
                int_manager_.enable_IME();
            }
            
            //for debugging purposes - remove later
            if (registers_.pc == desired_pc) {
                desired_pc_count++;
                std::cout << "\nreached non-prefixed pc " << (int) desired_pc << " "
                          << (unsigned int) desired_pc_count << std::endl;
            }

            //get instruction at pc and increment pc
            curr_inst_ = mmu_.read(registers_.pc++);
            prev_pc = registers_.pc - 1;

            //check if this is 0xCB-prefixed instruction
            if (curr_inst_ == 0xCB) {
                state_ = FETCH_CB;
                inst_cb_prefixed_ = true;
                return;
            }
            //check for HALT instruction
            if (curr_inst_ == 0x76) {
                if (int_manager_.halt_bug_encountered()) {
                    halt_bug_ = true;
                } else {
                    state_ = HALT;
                }
                return;
            }
            //check for STOP instruction
            if (curr_inst_ == 0x10) {
                state_ = STOP;
                //ignore next instruction
                registers_.pc++;
                return;
            }

            inst_cb_prefixed_ = false;
            //inst_ = &Instruction::get_instruction(false, curr_inst_);
            inst_ = Instruction::get_instruction(false, curr_inst_);

            //check if instruction only need one machine cycle to complete
            if (inst_->num_t_cycles() == 4) {
                inst_->execute(*this);
                check_for_interrupt();
                return;
            }
            state_ = DECODE_EXECUTE;
            break;
        }
        case FETCH_CB:
        {
            //check for pending IME enable
            if (int_manager_.pending_enable_IME()) {
                int_manager_.enable_IME();
            }

            curr_inst_ = mmu_.read(registers_.pc++);
            inst_ = Instruction::get_instruction(true, curr_inst_);
            //check if instruction only needs the two fetch cycles to complete
            if (inst_->num_t_cycles() == 8) {
                inst_->execute(*this);
                check_for_interrupt();
                return;
            }
            state_ = DECODE_EXECUTE;
            break;
        }
        case DECODE_EXECUTE:
        {
            inst_->execute(*this);
            if (inst_finished_)
                check_for_interrupt();
            break;
        }
        case INT_NOP1:
        {
            state_ = INT_NOP2;
            break;
        }
        case INT_NOP2:
        {
            state_ = INT_PUSHSP1;
            break;
        }
        case INT_PUSHSP1:
        {
            //push MSB of PC onto stack
            mmu_.write(--registers_.sp, (uint8_t)((registers_.pc & 0xFF00) >> 8));
            state_ = INT_PUSHSP2;
            break;
        }
        case INT_PUSHSP2:
        {
            //push LSB of PC onto stack
            mmu_.write(--registers_.sp, (uint8_t)(registers_.pc & 0x00FF));
            state_ = INT_JUMP;
            break;
        }
        case INT_JUMP:
        {
            //set PC to ISR address execute interrupt
            InterruptType requested_int = int_manager_.get_requested_interrupt();
            registers_.pc = InterruptManager::isr_addrs.at(requested_int);
            //clear interrupt because it will be handled
            int_manager_.clear_interrupt(requested_int);
            state_ = FETCH;
            break;
        }
        case HALT:
        {
            if (int_manager_.get_IME()) {
                if (int_manager_.interrupt_requested()) {
                    state_ = INT_NOP1;
                    return;
                }
                state_ = FETCH;
            } else if (int_manager_.interrupt_requested()) {
                state_ = FETCH;
                return;
            }
            break;
        }
        case STOP:
        {
            if (int_manager_.get_IME() && int_manager_.interrupt_requested()) {
                state_ = INT_NOP1;
            }
            break;
        }
        default:
        {
            std::cout << "This wasn't supposed to happen" << std::endl;
            break;
        }
    }
}