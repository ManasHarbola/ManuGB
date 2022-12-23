#pragma once
/*
DMG GameBoy CPU Emulation Implementation
*/

#include "../Logging.h"
#include <memory>
#include <fstream>
#include "../mmu/MMU.h"
#include "../ppu/PPU.h"
#include "../timer/Timer.h"
#include "../Constants.h"
#include "InterruptManager.h"

class CPU;
#include "InstructionHandler.h"

#define ZERO (1 << 7)
#define SUB (1 << 6)
#define HALF_CARRY (1 << 5)
#define CARRY (1 << 4)

enum CPUState {
    FETCH, FETCH_CB, DECODE_EXECUTE,
    INT_NOP1, INT_NOP2, INT_PUSHSP1, INT_PUSHSP2, INT_JUMP, 
    HALT, STOP
};

struct registers {
    union {
        struct {
            uint8_t f;
            uint8_t a;
        };
        uint16_t af;
    };
    union {
        struct {
            uint8_t c;
            uint8_t b;
        };
        uint16_t bc;
    };
    union {
        struct {
            uint8_t e;
            uint8_t d;
        };
        uint16_t de;
    };
    union {
        struct {
            uint8_t l;
            uint8_t h;
        };
        uint16_t hl;
    };
    
    uint16_t sp;
    uint16_t pc;
};


class CPU {
    public:
        CPU(MMU &mmu, PPU &ppu, Timer &timer, InterruptManager &int_manager);
        virtual ~CPU();
        void init_registers();
        void check_for_interrupt();
        void tick();
        //useful methods for debugging
        registers get_registers() {return registers_;}
        void write_state_to_log(std::ofstream& os);
    private:
        registers registers_;
        CPUState state_;
        MMU &mmu_;
        PPU &ppu_;
        Timer &timer_;
        InterruptManager &int_manager_;

        friend class Instruction;
        
        uint8_t curr_inst_{0x00};
        bool inst_cb_prefixed_{false};
        bool halt_bug_{false};
        
        std::unique_ptr<Instruction> inst_;
        bool inst_finished_{false};

        uint32_t total_t_cycles_{0};
        uint8_t t_cycle_lock_{0};

        //temporary space to store results between operations
        uint8_t lsb_{0};
        uint8_t msb_{0};
        uint8_t n_{0};
        uint16_t nn_{0};
        uint16_t addr_{0};
        uint16_t res_{0};

        uint16_t desired_pc = 0x0100;
        uint16_t desired_pc_count = 0;
        uint16_t prev_pc;
};
