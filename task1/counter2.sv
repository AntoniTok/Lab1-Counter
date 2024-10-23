module counter #(
    parameter WIDTH = 8
)(
    //interface signals
    input logic clk,
    input logic rst,
    input logic en,
    output logic [WIDTH-1:0] count
);

always_ff @ (posedge clk or posedge rst)  // Detecting posedge rst for asynchronous reset
    if (rst) 
        count <= {WIDTH{1'b0}};  // Asynchronous reset: clear counter immediately
    else 
        count <= count + {{WIDTH-1{1'b0}}, en};

endmodule
