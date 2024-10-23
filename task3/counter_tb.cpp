#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env){
    int i;
    int clk;
    
    Verilated::commandArgs(argc, argv);
    //init top verilog instance
    Vcounter* top = new Vcounter;
    //init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    //init Vbuddy
    if (vbdOpen()!=1) return(-1);
    vbdHeader("Lab1: Counter");

    top->clk = 1;
    top->rst = 1;
    top->ld = 0;
    vbdSetMode(1);

    //run simulation for many clock cycles
    for(i=0; i<460; i++){

        //dump variables into VCD file and toggle clock
        for(clk=0; clk<2; clk++){
            tfp->dump(2*i+clk);     //unit is in ps
            top->clk = !top->clk;
            top->eval (); 
        }

        // ++++ Send count value to Vbuddy
        vbdPlot(int(top->count), 0, 255);
        vbdCycle(i+1);
        // ---- end of Vbuddy output section

        // change input stimuli
        top->rst = (i<2) | (i==15);
        //top->ld = (i>4);
        top->ld = vbdFlag();
        if(Verilated::gotFinish()) exit(0);
    }

    vbdClose();     // ++++
    tfp->close();
    exit(0);
}

