module filter(clk, rj, sign, shift, data, out, flag);

input clk;
input [15:0] rj;
input sign;
input [7:0] shift;
input [39:0] data;
output reg [39:0] out;
output reg flag;

reg [39:0] x_arr [0:1499];
reg [15:0] r_arr [0:15];
reg [7:0] shift_coeff [0:511];
reg sign_coeff [0:511];
reg [39:0] u;
reg [39:0] temp;
reg [39:0] temp1;
reg [39:0] temp2;

integer i;
integer j;
integer n;
integer pos;
integer state = 0;
integer index = 0; 
integer rsize = 16; 
integer coeff_size = 512;
integer data_size = 1500;


always@(posedge clk) begin
	case(state)
		0: begin // load respective arrays with input values
				
			if(index < rsize) begin
				r_arr[index] = rj; //array of r values
				sign_coeff[index] = sign; //array of sign values of the coefficent 
				shift_coeff[index] = shift; // array of magnitude of coefficient values
				x_arr[index] = data; // sign extended input data array
				index = index + 1; 
			end
			else if(index < coeff_size) begin
				sign_coeff[index] = sign;
				shift_coeff[index] = shift;
				x_arr[index] = data;
				index = index + 1;
			end
			else if(index < data_size) begin
					//out_data_1[index] = 0;
					x_arr[index] = data;
					index = index + 1;
			end	
			else begin
				index = 0;
				n = 0;
				state = 1;
			end	
		end
		1: begin// start computation for an input value
			flag = 0;
			if(n < data_size) begin
				i = 0;
				j = 0;
				temp1 = 40'b0;
				temp2 = 40'b0;
				pos = 0;
				state = 2;
			end
			else begin 
				state = 0;
			end
		end
		2: begin
			if(i < rsize) begin
				u = 40'b0;
				temp = 40'b0;
				j = pos;
				state = 3;
			end
			else begin
				flag = 1;
				out = temp1;
				n = n + 1;
				state = 1;
			end
		end
		3: begin //Computation of each u term, i.e. u1, u2, u3...
			if(j < (pos +  r_arr[i])) begin
				temp = 0;
				if(n >= shift_coeff[j])begin
					if(sign_coeff[j]) begin
						temp = x_arr[n - shift_coeff[j]];
						temp = (~temp) + 1;
					end
					else begin
						temp = x_arr[n - shift_coeff[j]];
					end
				end
				u = u + temp;
				j = j + 1;
			end
			else begin//Accumulating u terms
				pos = pos +  r_arr[i];
				temp1 = temp1 + u;
				state =4;
			end
		   end
		4: begin//Performing shift in the next cycle
				temp2[39:0] = {temp1[39], temp1[39:1]};//right shift
				temp1 = temp2;
				i = i + 1;
				state = 2;
			end
	endcase
end
endmodule