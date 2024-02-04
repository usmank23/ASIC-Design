#include <iostream>
#include <iomanip>  
#include <sstream>
#include <bitset>
#include <string> 
#if 0
bool isValidHex(const std::string& input) {
    // Check if the string is a valid hexadecimal number
    for (char c : input) {
        if (!isxdigit(c)) {
            return false;
        }
    }
    return true;
}
#endif
bool isValidHex(const std::string& input) {
    // Check if the string is a valid hexadecimal number
    for (std::string::const_iterator it = input.begin(); it != input.end(); ++it) {
        if (!isxdigit(*it)) {
            return false;
        }
    }
    return true;
}

std::string formatHex(unsigned int value) {
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << value;
    return oss.str();
}

int main(int argc, char* argv[]) {
    // Check if two arguments are provided
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <hex_value_a> <hex_value_b>" << std::endl;
        return 1; // Exit with an error code
    }

    // Input variables
    std::string input_a(argv[1]);
    std::string input_b(argv[2]);
    unsigned int a, b;

    // Validate 'a'
    if (!isValidHex(input_a)) {
        std::cerr << "Invalid input for 'a'. Please enter a valid hex value." << std::endl;
        return 1; // Exit with an error code
    }

    // Convert 'a' to unsigned int
    a = std::stoul(input_a, NULL, 16);

    // Input for variable 'b'
    //std::cout << "Enter the value of b (hex format): ";
    //std::cin >> input_b;

    // Validate 'b'
    if (!isValidHex(input_b)) {
        std::cerr << "Invalid input for 'b'. Please enter a valid hex value." << std::endl;
        return 1; // Exit with an error code
    }

    // Convert 'b' to unsigned int
    b = std::stoul(input_b, NULL, 16);

    // Convert 'a' and 'b' to binary and store in 'x' and 'y'
    std::bitset<32> x(a);
    std::bitset<32> y(b);

    // Output the values
    //std::cout << "a (hex) = 0x" << formatHex(a) << ", a (decimal) = " << a << ", a (binary) = " << x << std::endl;
    //std::cout << "b (hex) = 0x" << formatHex(b) << ", b (decimal) = " << b << ", b (binary) = " << y << std::endl;
    
    int res = 0; 
    for (int i = 31; i >=0; i--) {
        //std::cout << "x of i" << x[i] << std::endl;
        if (x[i]) {
            std::bitset<32> shiftedNumber = y >> 32-i;
                // Output the result in hexadecimal
            //std::cout << "shiftedNumber= " << shiftedNumber << std::endl;
            res = res + static_cast<int>(shiftedNumber.to_ulong());
        }
    }

    // Output the result in hexadecimal
    std::cout << "res (hex) = 0x" << formatHex(res) << std::endl;

    return 0; // Exit successfully
}

