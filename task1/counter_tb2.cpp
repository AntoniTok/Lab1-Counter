#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env){
    int i;
    int clk;
    int pause_cycles = 0;  // To track the number of pause cycles
    bool paused = false;   // Flag to track if we are in a pause state

    Verilated::commandArgs(argc, argv);
    //init top verilog instance
    Vcounter* top = new Vcounter;
    //init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    //run simulation for many clock cycles
    for(i=0; i<300; i++){

        //dump variables into VCD file and toggle clock
        for(clk=0; clk<2; clk++){
            tfp->dump(2*i+clk);  // Back to picoseconds scale
            top->clk = !top->clk;
            top->eval(); 
        }

        // Reset signal control (active for the first 2 cycles)
        top->rst = (i < 2);

        // If the counter reaches 0x9 and we're not already in a pause state
        if (top->count == 0x9 && !paused) {
            pause_cycles = 3;  // Initiate 3-cycle pause
            paused = true;     // Set the pause flag
        }

        // Manage enable signal
        if (pause_cycles > 0) {
            top->en = 0;       // Disable counting during the pause
            pause_cycles--;    // Decrease the pause counter
        } else {
            top->en = 1;       // Resume counting once pause is done
            paused = false;    // Clear the pause flag
        }

        // Stop simulation if Verilator finish condition is met
        if (Verilated::gotFinish()) exit(0);
    }

    // Close the VCD trace file
    tfp->close();
    exit(0);
}
