`timescale 1ns / 1ps
module filter_tb;

	// Inputs
	reg clk;
	reg [15:0] rj;
	reg sign;
	reg [7:0] shift;
	reg [39:0] data;
	// Outputs
	wire [39:0] out_data;
	wire flag;
	//Memories and variables	
	reg [15:0] mem [0:2027];
	reg [15:0] in_data [0:1499];
	reg [39:0] in_new_data [0:1499];
	reg [15:0] temp_in;
	reg [39:0] temp_data;
	reg [15:0] in_rj [0:15];
	reg [15:0] in_coeff [0:511];
	reg [7:0] shift_coeff [0:511];
	reg sign_coeff [0:511];
	
	integer m;
	integer index = 0;
	integer final_var = 0;
	integer file;
    integer rj_size = 16;
    integer coeff_size = 512;
    integer data_size = 1500; 
	integer state_mon;
	reg [39:0] temp_mon;
	reg [39:0] u_mon;

	// Instantiate the Unit Under Test (UUT)
	filter uut (
		.clk(clk),
		.rj(in_rj[index]),
		.sign(sign_coeff[index]),
		.shift(shift_coeff[index]),
		.data(in_new_data[index]),
		.out(out_data),
		.flag(flag)
	);
	
    initial 
    begin
        clk = 0;
        forever #20 clk = ~clk;
    end
  
	initial begin
		$system("pwd");
		clk = 0;
		file = $fopen("C:\\Users\\uxk220003\\Desktop\\Verliog Codes\\Projects\\Filter\\dataout.txt");
		$readmemh("C:\\Users\\uxk220003\\Desktop\\Verliog Codes\\Projects\\Filter\\data.txt", mem);
		
		//Seperating inputs, coefficients and rjs from the input file
		for(m = 0; m < rj_size; m = m + 1)
		begin
			in_rj[m] = mem[m];
		end
		/*$display("Array values:");
		for (integer i = 0; i < 16; i = i + 1) 
        begin
        $display("in_new_data[%0d] = %h", i, in_rj[i]);
        end*/
		
		for(m = 0; m < coeff_size; m = m + 1)
		begin
			in_coeff[m] = mem[m + rj_size];
		end
	
		for(m = 0; m < data_size; m = m + 1)
		begin
			in_data[m] = mem[m + rj_size + coeff_size];
		end

		//Extending the sign bits to make the input of 40 bits
		
        for (m = 0; m < data_size; m = m + 1)
		begin
			temp_in = in_data[m];
			temp_data[39:0] = { {8{temp_in[15] }}, temp_in[15:0] } ; //if temp_in = 6EF4, then temp_data = 0000 0000 0000 0000 0000 0000 0110 1110 1111 0100 
			in_new_data[m] = temp_data << 16; //left shift the above value by 16 times to get. in_new_data = 0000 0000 0110 1110 1111 0100 0000 0000 0000 0000
		end

		//Extracting the signs and shift values from the coefficients
		
        for (m = 0; m < coeff_size; m = m + 1)
		begin
			sign_coeff[m] = in_coeff[m] >> 8;
			shift_coeff[m] = in_coeff[m] & 16'h00FF;
		end
	
	end
		
		//Always block for initialization
		always @ (posedge clk) begin : init
		
			if(index < rj_size) begin
				rj = in_rj[index];
				sign = sign_coeff[index];
				shift = shift_coeff[index];
				data = in_new_data[index];
				index = index + 1;
			end
			
			else if(index < coeff_size) begin
				sign = sign_coeff[index];
				shift = shift_coeff[index];
				data = in_new_data[index];
				index = index + 1;
			end
			
			else if(index < data_size) begin
				data = in_new_data[index];
				index = index + 1;
			end
				
			else begin
				disable init;
			end
		end	
      
		//Always block for generating final output.txt
		always @ (posedge clk) 
		begin	: final_txt
			state_mon = uut.state;
			temp_mon = uut.temp1;
			u_mon = uut.u;
			if(flag) 
			begin
			$fwrite(file, "%H\n", out_data);
			$display("The value of output is: %h", out_data );
			final_var = final_var + 1;
			if(final_var == data_size) 
			begin
				$fclose(file);
				$stop;
				disable final_txt;
			end
			end
		    end
		
endmodule



