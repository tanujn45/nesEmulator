#include <iostream>
#include <sstream>

#include "Bus.h"
#include "CPU6502.h"

const string GREEN = "\033[32m";
const string RED = "\033[31m";
const string ORIG_COLOR = "\033[0m";
const string CYAN = "\033[36m";

using namespace std;
class Emulation {
   public:
    Bus nes;
    map<uint16_t, string> mapAsm;
    Emulation() {
        // Load Program (assembled at
        // https://www.masswerk.at/6502/assembler.html)
        /*
                *=$8000
                LDX #10
                STX $0000
                LDX #3
                STX $0001
                LDY $0000
                LDA #0
                CLC
                loop
                ADC $0001
                DEY
                BNE loop
                STA $0002
                NOP
                NOP
                NOP
        */

        // Convert hex string into bytes for RAM
        stringstream ss;
        ss << "A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 "
              "FA 8D 02 00 EA EA EA";
        uint16_t nOffset = 0x8000;
        while (!ss.eof()) {
            string b;
            ss >> b;
            nes.ram[nOffset++] = (uint8_t)stoul(b, nullptr, 16);
        }

        // Set Reset Vector
        nes.ram[0xFFFC] = 0x00;
        nes.ram[0xFFFD] = 0x80;

        // Dont forget to set IRQ and NMI vectors if you want to play with those

        // Extract dissassembly
        mapAsm = nes.cpu.disassemble(0x0000, 0xFFFF);
        nes.cpu.reset();
    }

    string hex(uint32_t n, uint8_t d) {
        string s(d, '0');
        for (int i = d - 1; i >= 0; i--, n >>= 4)
            s[i] = "0123456789ABCDEF"[n & 0xF];
        return s;
    };

    void printRam(uint16_t nAddr, int nRows, int nColumns) {
        cout << "\n";
        for (int row = 0; row < nRows; row++) {
            string sOffset = "$" + hex(nAddr, 4) + ":";
            for (int col = 0; col < nColumns; col++) {
                sOffset += " " + hex(nes.read(nAddr, true), 2);
                nAddr += 1;
            }
            cout << sOffset << "\n";
        }
    }

    string color(bool condition) {
        return condition ? GREEN : RED;
    }

    void printCpu() {
        cout << "\nSTATUS: ";
        cout << color(nes.cpu.status & CPU6502::C) << "C" << ORIG_COLOR << " ";
        cout << color(nes.cpu.status & CPU6502::Z) << "V" << ORIG_COLOR << " ";
        cout << color(nes.cpu.status & CPU6502::I) << "I" << ORIG_COLOR << " ";
        cout << color(nes.cpu.status & CPU6502::D) << "D" << ORIG_COLOR << " ";
        cout << color(nes.cpu.status & CPU6502::B) << "B" << ORIG_COLOR << " ";
        cout << color(nes.cpu.status & CPU6502::U) << "-" << ORIG_COLOR << " ";
        cout << color(nes.cpu.status & CPU6502::V) << "V" << ORIG_COLOR << " ";
        cout << color(nes.cpu.status & CPU6502::N) << "N" << ORIG_COLOR << " ";
        cout << "\n";
        cout << "PC: $" << hex(nes.cpu.pc, 4) << "  ";
        cout << "A: $" << hex(nes.cpu.a, 2) << " [" << to_string(nes.cpu.a)
             << "]" << "  ";
        cout << "X: $" << hex(nes.cpu.x, 2) << " [" << to_string(nes.cpu.x)
             << "]" << "  ";
        cout << "Y: $" << hex(nes.cpu.y, 2) << " [" << to_string(nes.cpu.y)
             << "]" << "  ";
        cout << "Stack P: $" << hex(nes.cpu.stkp, 4);
    }

    void printCode(int nLines) {
        cout << "\n\n";
        auto it = mapAsm.find(nes.cpu.pc);

        // Center the PC instruction line
        int halfLines = nLines / 2;
        int currentLine = 0;

        cout << "Code Disassembly:\n";

        // Print lines before the current PC
        for (int i = 0; i < halfLines && it != mapAsm.begin(); ++i) {
            if (it != mapAsm.begin()) {
                --it;
                ++currentLine;
            }
        }

        // Print the PC line
        if (it != mapAsm.end()) {
            cout << CYAN << "$" << hex(it->first, 4) << ": " << it->second
                 << ORIG_COLOR << "\n";
            ++currentLine;
            ++it;
        }

        // Print lines after the current PC
        while (currentLine < nLines && it != mapAsm.end()) {
            cout << "$" << hex(it->first, 4) << ": " << it->second << "\n";
            ++it;
            ++currentLine;
        }
    }

    void ClearScreen() {
        cout << "\033[2J\033[1;1H";
    }

    void runEmulation() {
        ClearScreen();
        char command = 'r';
        do {
            if (command == 's') {
                do {
                    nes.cpu.clock();
                } while (!nes.cpu.complete());
            } else if (command == 'r') {
                nes.cpu.reset();
            } else if (command == 'i') {
                nes.cpu.irq();
            } else if (command == 'n') {
                nes.cpu.nmi();
            }

            printRam(0x0000, 16, 16);
            printRam(0x8000, 16, 16);
            printCpu();
            printCode(20);

            cout << "[s]tep [r]eset [i]rq [n]mi [q]uit\n";
            cout << "Enter Command: ";
            cin >> command;
            ClearScreen();
        } while (command != 'q');
    }
};

int main() {
    Emulation em;
    em.runEmulation();
}
