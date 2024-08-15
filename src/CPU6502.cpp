#include "CPU6502.h"

#include <cstdint>

#include "Bus.h"

using namespace std;

CPU6502::CPU6502() {
    using a = CPU6502;
    lookup = {
        {"BRK", &a::BRK, &a::IMM, 7}, {"ORA", &a::ORA, &a::IZX, 6},
        {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
        {"???", &a::NOP, &a::IMP, 3}, {"ORA", &a::ORA, &a::ZP0, 3},
        {"ASL", &a::ASL, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5},
        {"PHP", &a::PHP, &a::IMP, 3}, {"ORA", &a::ORA, &a::IMM, 2},
        {"ASL", &a::ASL, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
        {"???", &a::NOP, &a::IMP, 4}, {"ORA", &a::ORA, &a::ABS, 4},
        {"ASL", &a::ASL, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
        {"BPL", &a::BPL, &a::REL, 2}, {"ORA", &a::ORA, &a::IZY, 5},
        {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
        {"???", &a::NOP, &a::IMP, 4}, {"ORA", &a::ORA, &a::ZPX, 4},
        {"ASL", &a::ASL, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
        {"CLC", &a::CLC, &a::IMP, 2}, {"ORA", &a::ORA, &a::ABY, 4},
        {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
        {"???", &a::NOP, &a::IMP, 4}, {"ORA", &a::ORA, &a::ABX, 4},
        {"ASL", &a::ASL, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7},
        {"JSR", &a::JSR, &a::ABS, 6}, {"AND", &a::AND, &a::IZX, 6},
        {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
        {"BIT", &a::BIT, &a::ZP0, 3}, {"AND", &a::AND, &a::ZP0, 3},
        {"ROL", &a::ROL, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5},
        {"PLP", &a::PLP, &a::IMP, 4}, {"AND", &a::AND, &a::IMM, 2},
        {"ROL", &a::ROL, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
        {"BIT", &a::BIT, &a::ABS, 4}, {"AND", &a::AND, &a::ABS, 4},
        {"ROL", &a::ROL, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
        {"BMI", &a::BMI, &a::REL, 2}, {"AND", &a::AND, &a::IZY, 5},
        {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
        {"???", &a::NOP, &a::IMP, 4}, {"AND", &a::AND, &a::ZPX, 4},
        {"ROL", &a::ROL, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
        {"SEC", &a::SEC, &a::IMP, 2}, {"AND", &a::AND, &a::ABY, 4},
        {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
        {"???", &a::NOP, &a::IMP, 4}, {"AND", &a::AND, &a::ABX, 4},
        {"ROL", &a::ROL, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7},
        {"RTI", &a::RTI, &a::IMP, 6}, {"EOR", &a::EOR, &a::IZX, 6},
        {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
        {"???", &a::NOP, &a::IMP, 3}, {"EOR", &a::EOR, &a::ZP0, 3},
        {"LSR", &a::LSR, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5},
        {"PHA", &a::PHA, &a::IMP, 3}, {"EOR", &a::EOR, &a::IMM, 2},
        {"LSR", &a::LSR, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
        {"JMP", &a::JMP, &a::ABS, 3}, {"EOR", &a::EOR, &a::ABS, 4},
        {"LSR", &a::LSR, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
        {"BVC", &a::BVC, &a::REL, 2}, {"EOR", &a::EOR, &a::IZY, 5},
        {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
        {"???", &a::NOP, &a::IMP, 4}, {"EOR", &a::EOR, &a::ZPX, 4},
        {"LSR", &a::LSR, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
        {"CLI", &a::CLI, &a::IMP, 2}, {"EOR", &a::EOR, &a::ABY, 4},
        {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
        {"???", &a::NOP, &a::IMP, 4}, {"EOR", &a::EOR, &a::ABX, 4},
        {"LSR", &a::LSR, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7},
        {"RTS", &a::RTS, &a::IMP, 6}, {"ADC", &a::ADC, &a::IZX, 6},
        {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
        {"???", &a::NOP, &a::IMP, 3}, {"ADC", &a::ADC, &a::ZP0, 3},
        {"ROR", &a::ROR, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5},
        {"PLA", &a::PLA, &a::IMP, 4}, {"ADC", &a::ADC, &a::IMM, 2},
        {"ROR", &a::ROR, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
        {"JMP", &a::JMP, &a::IND, 5}, {"ADC", &a::ADC, &a::ABS, 4},
        {"ROR", &a::ROR, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
        {"BVS", &a::BVS, &a::REL, 2}, {"ADC", &a::ADC, &a::IZY, 5},
        {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
        {"???", &a::NOP, &a::IMP, 4}, {"ADC", &a::ADC, &a::ZPX, 4},
        {"ROR", &a::ROR, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
        {"SEI", &a::SEI, &a::IMP, 2}, {"ADC", &a::ADC, &a::ABY, 4},
        {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
        {"???", &a::NOP, &a::IMP, 4}, {"ADC", &a::ADC, &a::ABX, 4},
        {"ROR", &a::ROR, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7},
        {"???", &a::NOP, &a::IMP, 2}, {"STA", &a::STA, &a::IZX, 6},
        {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 6},
        {"STY", &a::STY, &a::ZP0, 3}, {"STA", &a::STA, &a::ZP0, 3},
        {"STX", &a::STX, &a::ZP0, 3}, {"???", &a::XXX, &a::IMP, 3},
        {"DEY", &a::DEY, &a::IMP, 2}, {"???", &a::NOP, &a::IMP, 2},
        {"TXA", &a::TXA, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
        {"STY", &a::STY, &a::ABS, 4}, {"STA", &a::STA, &a::ABS, 4},
        {"STX", &a::STX, &a::ABS, 4}, {"???", &a::XXX, &a::IMP, 4},
        {"BCC", &a::BCC, &a::REL, 2}, {"STA", &a::STA, &a::IZY, 6},
        {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 6},
        {"STY", &a::STY, &a::ZPX, 4}, {"STA", &a::STA, &a::ZPX, 4},
        {"STX", &a::STX, &a::ZPY, 4}, {"???", &a::XXX, &a::IMP, 4},
        {"TYA", &a::TYA, &a::IMP, 2}, {"STA", &a::STA, &a::ABY, 5},
        {"TXS", &a::TXS, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 5},
        {"???", &a::NOP, &a::IMP, 5}, {"STA", &a::STA, &a::ABX, 5},
        {"???", &a::XXX, &a::IMP, 5}, {"???", &a::XXX, &a::IMP, 5},
        {"LDY", &a::LDY, &a::IMM, 2}, {"LDA", &a::LDA, &a::IZX, 6},
        {"LDX", &a::LDX, &a::IMM, 2}, {"???", &a::XXX, &a::IMP, 6},
        {"LDY", &a::LDY, &a::ZP0, 3}, {"LDA", &a::LDA, &a::ZP0, 3},
        {"LDX", &a::LDX, &a::ZP0, 3}, {"???", &a::XXX, &a::IMP, 3},
        {"TAY", &a::TAY, &a::IMP, 2}, {"LDA", &a::LDA, &a::IMM, 2},
        {"TAX", &a::TAX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
        {"LDY", &a::LDY, &a::ABS, 4}, {"LDA", &a::LDA, &a::ABS, 4},
        {"LDX", &a::LDX, &a::ABS, 4}, {"???", &a::XXX, &a::IMP, 4},
        {"BCS", &a::BCS, &a::REL, 2}, {"LDA", &a::LDA, &a::IZY, 5},
        {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 5},
        {"LDY", &a::LDY, &a::ZPX, 4}, {"LDA", &a::LDA, &a::ZPX, 4},
        {"LDX", &a::LDX, &a::ZPY, 4}, {"???", &a::XXX, &a::IMP, 4},
        {"CLV", &a::CLV, &a::IMP, 2}, {"LDA", &a::LDA, &a::ABY, 4},
        {"TSX", &a::TSX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 4},
        {"LDY", &a::LDY, &a::ABX, 4}, {"LDA", &a::LDA, &a::ABX, 4},
        {"LDX", &a::LDX, &a::ABY, 4}, {"???", &a::XXX, &a::IMP, 4},
        {"CPY", &a::CPY, &a::IMM, 2}, {"CMP", &a::CMP, &a::IZX, 6},
        {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
        {"CPY", &a::CPY, &a::ZP0, 3}, {"CMP", &a::CMP, &a::ZP0, 3},
        {"DEC", &a::DEC, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5},
        {"INY", &a::INY, &a::IMP, 2}, {"CMP", &a::CMP, &a::IMM, 2},
        {"DEX", &a::DEX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
        {"CPY", &a::CPY, &a::ABS, 4}, {"CMP", &a::CMP, &a::ABS, 4},
        {"DEC", &a::DEC, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
        {"BNE", &a::BNE, &a::REL, 2}, {"CMP", &a::CMP, &a::IZY, 5},
        {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
        {"???", &a::NOP, &a::IMP, 4}, {"CMP", &a::CMP, &a::ZPX, 4},
        {"DEC", &a::DEC, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
        {"CLD", &a::CLD, &a::IMP, 2}, {"CMP", &a::CMP, &a::ABY, 4},
        {"NOP", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
        {"???", &a::NOP, &a::IMP, 4}, {"CMP", &a::CMP, &a::ABX, 4},
        {"DEC", &a::DEC, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7},
        {"CPX", &a::CPX, &a::IMM, 2}, {"SBC", &a::SBC, &a::IZX, 6},
        {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
        {"CPX", &a::CPX, &a::ZP0, 3}, {"SBC", &a::SBC, &a::ZP0, 3},
        {"INC", &a::INC, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5},
        {"INX", &a::INX, &a::IMP, 2}, {"SBC", &a::SBC, &a::IMM, 2},
        {"NOP", &a::NOP, &a::IMP, 2}, {"???", &a::SBC, &a::IMP, 2},
        {"CPX", &a::CPX, &a::ABS, 4}, {"SBC", &a::SBC, &a::ABS, 4},
        {"INC", &a::INC, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
        {"BEQ", &a::BEQ, &a::REL, 2}, {"SBC", &a::SBC, &a::IZY, 5},
        {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
        {"???", &a::NOP, &a::IMP, 4}, {"SBC", &a::SBC, &a::ZPX, 4},
        {"INC", &a::INC, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
        {"SED", &a::SED, &a::IMP, 2}, {"SBC", &a::SBC, &a::ABY, 4},
        {"NOP", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
        {"???", &a::NOP, &a::IMP, 4}, {"SBC", &a::SBC, &a::ABX, 4},
        {"INC", &a::INC, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7},
    };
}

CPU6502::~CPU6502() {
}

void CPU6502::write(uint16_t a, uint8_t d) {
    bus->write(a, d);
}

uint8_t CPU6502::read(uint16_t a) {
    return bus->read(a, false);
}

void CPU6502::clock() {
    if (cycles == 0) {
        opcode = read(pc);
        pc++;

        cycles = lookup[opcode].cycles;

        uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();
        uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();

        cycles += (additional_cycle1 & additional_cycle2);
    }
    cycles--;
}

void CPU6502::reset() {
    a = 0;
    x = 0;
    y = 0;

    // When CPU is turned on Stack pointer is actually
    // zero. In the third cycle the Stack pointer is set
    // to 0x0100. For next three cyles the Stack pointer
    // is decremented. Therefore it starts from 0x0FD.
    // Use the link below for reference.
    // https://www.pagetable.com/?p=410
    stkp = 0xFD;

    status = 0x00 | U;

    // This is a hardcoded address which store where the
    // Program counter starts.
    addr_abs = 0xFFFC;
    uint16_t lo = read(addr_abs);
    uint16_t hi = read(addr_abs + 1);

    pc = (hi << 8) | lo;

    addr_rel = 0x0000;
    addr_rel = 0x0000;
    fetched = 0x00;

    cycles = 8;
}

void CPU6502::irq() {
    if (!GetFlag(I)) {
        write(0x0100 + stkp, (pc >> 8) & 0x00FF);
        stkp--;
        write(0x0100 + stkp, pc & 0x00FF);
        stkp--;

        SetFlag(B, 0);
        SetFlag(U, 1);
        SetFlag(I, 1);
        write(0x0100 + stkp, status);
        stkp--;

        addr_abs = 0xFFFE;
        uint16_t lo = read(addr_abs);
        uint16_t hi = read(addr_abs + 1);
        pc = (hi << 8) | lo;

        cycles = 7;
    }
}

void CPU6502::nmi() {
    write(0x0100 + stkp, (pc >> 8) & 0x00FF);
    stkp--;
    write(0x0100 + stkp, pc & 0x00FF);
    stkp--;

    SetFlag(B, 0);
    SetFlag(U, 1);
    SetFlag(I, 1);
    write(0x0100 + stkp, status);
    stkp--;

    addr_abs = 0xFFFA;
    uint16_t lo = read(addr_abs);
    uint16_t hi = read(addr_abs + 1);
    pc = (hi << 8) | lo;

    cycles = 8;
}

// Addressing Modes

// Implied
uint8_t CPU6502::IMP() {
    fetched = a;
    return 0;
}

// Immediate
// Stores the address of the current instruction and
// updates the Program Counter for the operand
uint8_t CPU6502::IMM() {
    addr_abs = pc;
    pc++;
    return 0;
}

// Zero Page
uint8_t CPU6502::ZP0() {
    addr_abs = read(pc);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t CPU6502::ZPX() {
    addr_abs = (read(pc) + x);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t CPU6502::ZPY() {
    addr_abs = (read(pc) + y);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

uint8_t CPU6502::REL() {
    addr_rel = read(pc);
    pc++;

    // Check if the last bit i.e Negative bit is
    // set or not.
    if (addr_rel & 0x80) {
        addr_rel |= 0xFF00;
    }
    return 0;
}

uint8_t CPU6502::ABS() {
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;
    return 0;
}

uint8_t CPU6502::ABX() {
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;
    addr_abs += x;

    if ((addr_abs & 0xFF00) != (hi << 8)) {
        return 1;
    }
    return 0;
}

uint8_t CPU6502::ABY() {
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;
    addr_abs += y;

    if ((addr_abs & 0xFF00) != (hi << 8)) {
        return 1;
    }
    return 0;
}

uint8_t CPU6502::IND() {
    uint16_t ptr_lo = read(pc);
    pc++;
    uint16_t ptr_hi = read(pc);
    pc++;

    uint16_t ptr = (ptr_hi << 8) | ptr_lo;

    // If it is the end of the page we
    // go back to the start of the same page.
    // This is a bug in the original hardware
    // and we are emulating it.
    if (ptr_lo == 0x00FF) {
        addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
    } else {
        addr_abs = read(ptr + 1) << 8 | read(ptr + 0);
    }
    return 0;
}

uint8_t CPU6502::IZX() {
    uint16_t t = read(pc);
    pc++;

    uint16_t lo = read((t + (uint16_t)x) & 0x00FF);
    uint16_t hi = read((t + (uint16_t)x + 1) & 0x00FF);

    addr_abs = hi << 8 | lo;
    return 0;
}

uint8_t CPU6502::IZY() {
    uint16_t t = read(pc);
    pc++;

    uint16_t lo = read(t & 0x00FF);
    uint16_t hi = read((t + 1) & 0x00FF);

    addr_abs = (hi << 8) | lo;
    addr_abs += y;

    if ((addr_abs & 0xFF00) != (hi << 8)) {
        return 1;
    }
    return 0;
}

uint8_t CPU6502::fetch() {
    if (!(lookup[opcode].addrmode == &CPU6502::IMM)) {
        fetched = read(addr_abs);
    }

    return fetched;
}

// Instructions

uint8_t CPU6502::ADC() {
    fetch();
    uint16_t temp = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(C);
    SetFlag(C, temp > 0xFF);
    SetFlag(Z, (temp & 0x00FF) == 0x00);
    SetFlag(N, temp & 0x80);
    SetFlag(V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ temp)) &
                   0x0080);
    a = temp & 0x00FF;
    return 1;
}

uint8_t CPU6502::AND() {
    fetch();
    a = a & fetched;

    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);

    return 1;
}

uint8_t CPU6502::ASL() {
    fetch();
    uint16_t temp = a << 1;

    SetFlag(C, (temp & 0xFF00));
    SetFlag(Z, (temp & 0x00FF) == 0x00);
    SetFlag(N, (temp & 0x80));

    if (lookup[opcode].addrmode == &CPU6502::IMP) {
        a = temp & 0x00FF;
    } else {
        write(addr_abs, temp & 0x00FF);
    }
    return 0;
}

uint8_t CPU6502::BCC() {
    if (!GetFlag(C)) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }

        pc = addr_abs;
    }
    return 0;
}

uint8_t CPU6502::BCS() {
    if (GetFlag(C)) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }
    }
    return 0;
}

uint8_t CPU6502::BEQ() {
    if (GetFlag(Z)) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }
    }
    return 0;
}

uint8_t CPU6502::BIT() {
    fetch();
    uint16_t temp = a & fetched;
    SetFlag(Z, (a & fetched) == 0x00);
    SetFlag(V, fetched & 0x40);
    SetFlag(N, fetched & 0x80);
    return 0;
}

uint8_t CPU6502::BMI() {
    if (GetFlag(N)) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }
    }
    return 0;
}

uint8_t CPU6502::BNE() {
    if (GetFlag(Z)) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }
    }
    return 0;
}

uint8_t CPU6502::BPL() {
    if (!GetFlag(N)) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }
    }
    return 0;
}

uint8_t CPU6502::BRK() {
    pc++;

    SetFlag(I, 1);
    write((0x0100 + stkp), (pc >> 8) & 0x00FF);
    stkp--;
    write((0x0100 + stkp), pc & 0x00FF);
    stkp--;

    SetFlag(B, 1);
    write((0x0100 + stkp), status);
    SetFlag(B, 0);

    pc = ((uint16_t)read(0xFFFF) << 8) | (uint16_t)read(0xFFFE);
    return 0;
}

uint8_t CPU6502::BVC() {
    if (!GetFlag(V)) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }
    }
    return 0;
}

uint8_t CPU6502::BVS() {
    if (GetFlag(V)) {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
            cycles++;
        }
    }
    return 0;
}

uint8_t CPU6502::CLC() {
    SetFlag(C, 0);
    return 0;
}

uint8_t CPU6502::CLD() {
    SetFlag(D, 0);
    return 0;
}

uint8_t CPU6502::CLI() {
    SetFlag(I, 0);
    return 0;
}

uint8_t CPU6502::CLV() {
    SetFlag(V, 0);
    return 0;
}

uint8_t CPU6502::CMP() {
    fetch();

    uint16_t temp = (uint16_t)a - (uint16_t)fetched;
    SetFlag(C, a >= fetched);
    SetFlag(Z, a == fetched);
    SetFlag(N, temp & 0x0080);

    return 1;
}

uint8_t CPU6502::CPX() {
    fetch();

    uint16_t temp = (uint16_t)x - (uint16_t)fetched;
    SetFlag(C, x >= fetched);
    SetFlag(Z, x == fetched);
    SetFlag(N, temp & 0x0080);

    return 0;
}

uint8_t CPU6502::CPY() {
    fetch();

    uint16_t temp = (uint16_t)x - (uint16_t)fetched;
    SetFlag(C, y >= fetched);
    SetFlag(Z, y == fetched);
    SetFlag(N, temp & 0x0080);

    return 0;
}

uint8_t CPU6502::DEC() {
    fetch();

    uint16_t temp = (uint16_t)fetched - 1;
    write(addr_abs, temp & 0x00FF);
    SetFlag(Z, (temp & 0x00FF) == 0x00);
    SetFlag(N, (temp & 0x00FF) & 0x80);

    return 0;
}

uint8_t CPU6502::DEX() {
    uint16_t temp = (uint16_t)x - 1;
    SetFlag(Z, (temp & 0x00FF) == 0x00);
    SetFlag(N, temp & 0x0080);
    x = temp;

    return 0;
}

uint8_t CPU6502::DEY() {
    uint16_t temp = (uint16_t)y - 1;
    SetFlag(Z, (temp & 0x00FF) == 0x00);
    SetFlag(N, temp & 0x0080);
    y = temp;

    return 0;
}

uint8_t CPU6502::EOR() {
    fetch();
    a = a ^ fetched;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);

    return 1;
}

uint8_t CPU6502::INC() {
    fetch();
    uint16_t temp = (uint16_t)fetched + 1;
    SetFlag(Z, (temp & 0x00FF) == 0x00);
    SetFlag(N, temp & 0x0080);
    write(addr_abs, (temp & 0x00FF));

    return 0;
}

uint8_t CPU6502::INX() {
    x++;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);

    return 0;
}

uint8_t CPU6502::INY() {
    y++;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);

    return 0;
}

uint8_t CPU6502::JMP() {
    pc = addr_abs;

    return 0;
}

uint8_t CPU6502::JSR() {
    pc--;

    write((0x0100 + stkp), (pc >> 8) & 0x00FF);
    stkp--;
    write((0x0100 + stkp), pc & 0x00FF);
    stkp--;

    pc = addr_abs;

    return 0;
}

uint8_t CPU6502::LDA() {
    fetch();
    a = fetched;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);

    return 1;
}

uint8_t CPU6502::LDX() {
    fetch();
    x = fetched;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);

    return 1;
}

uint8_t CPU6502::LDY() {
    fetch();
    y = fetched;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);

    return 1;
}

uint8_t CPU6502::LSR() {
    fetch();
    SetFlag(C, fetched & 0x01);
    uint16_t temp = fetched >> 1;
    SetFlag(N, temp & 0x80);
    if (lookup[opcode].addrmode == &CPU6502::IMP) {
        a = temp & 0x00FF;
    } else {
        write(addr_abs, temp & 0x00FF);
    }
    return 0;
}

// Note: Still need to understand why.
uint8_t CPU6502::NOP() {
    switch (opcode) {
        case 0x1C:
        case 0x3C:
        case 0x5C:
        case 0x7C:
        case 0xDC:
        case 0xFC:
            return 1;
            break;
    }
    return 0;
}

uint8_t CPU6502::ORA() {
    fetch();
    a |= fetched;
    SetFlag(N, a == 0x00);
    SetFlag(N, a & 0x80);
    return 0;
}

uint8_t CPU6502::PHA() {
    write((0x0100 + stkp), a);
    stkp--;
    return 0;
}

uint8_t CPU6502::PHP() {
    // Note: find out why B and U are set.
    write((0x0100 + stkp), status | B | U);
    SetFlag(B, 0);
    SetFlag(U, 0);
    stkp--;
    return 0;
}

uint8_t CPU6502::PLA() {
    stkp++;
    a = read((0x0100 + stkp));
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 0;
}

uint8_t CPU6502::PLP() {
    stkp++;
    status = read((0x0100 + stkp));
    SetFlag(U, 1);
    return 0;
}

uint8_t CPU6502::ROL() {
    fetch();
    SetFlag(C, fetched & 0x01);
    return 0;
}
uint8_t CPU6502::ROR() {
    return 0;
}
uint8_t CPU6502::RTI() {
    return 0;
}
uint8_t CPU6502::RTS() {
    return 0;
}
uint8_t CPU6502::SBC() {
    fetch();
    uint16_t invertedVal = ((uint16_t)fetched ^ 0x00FF);
    uint16_t temp = (int16_t)a + (int16_t)invertedVal + (uint16_t)GetFlag(C);

    SetFlag(C, temp & 0xFF00);
    SetFlag(Z, ((temp & 0x00FF) == 0x00));
    // Note: this is not clear to me yet.
    SetFlag(V, (temp ^ (uint16_t)a) & (temp ^ invertedVal) & 0x0080);
    SetFlag(N, temp & 0x80);

    a = temp & 0x00FF;
    return 1;
}
uint8_t CPU6502::SEC() {
    return 0;
}
uint8_t CPU6502::SED() {
    return 0;
}
uint8_t CPU6502::SEI() {
    return 0;
}
uint8_t CPU6502::STA() {
    return 0;
}
uint8_t CPU6502::STX() {
    return 0;
}
uint8_t CPU6502::STY() {
    return 0;
}
uint8_t CPU6502::TAX() {
    return 0;
}
uint8_t CPU6502::TAY() {
    return 0;
}
uint8_t CPU6502::TSX() {
    return 0;
}
uint8_t CPU6502::TXA() {
    return 0;
}
uint8_t CPU6502::TXS() {
    return 0;
}
uint8_t CPU6502::TYA() {
    return 0;
}
