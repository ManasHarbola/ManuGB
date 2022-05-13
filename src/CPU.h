#pragma once

/*
SM83 CPU Emulation Implementation
*/
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
        CPU();

        void tick();
        //useful for debugging
        registers get_registers_cpy() {return _registers;}
    private:
        registers registers_;
        uint8_t flag_register_;

};
