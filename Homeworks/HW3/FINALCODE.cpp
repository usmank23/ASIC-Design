#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdlib>
using namespace std;

std::vector<unsigned int> readHexadecimalValuesFromFile(const std::string& filename) {
    std::vector<unsigned int> hexValues;
    std::ifstream inputFile(filename.c_str());

    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file: " << filename << std::endl;
        return hexValues; // Return empty vector
    }

    std::string hexValue;
    while (inputFile >> hexValue) {
        // Convert hexadecimal string to integer and store it in the vector
        unsigned int decimalValue;
        std::stringstream ss;
        ss << std::hex << hexValue;
        ss >> decimalValue;
        hexValues.push_back(decimalValue);
    }

    inputFile.close();
    return hexValues;
}

vector<long int> readAndFormatHexValues(const string& filename) {
    vector<long int> formattedHexValuesAsLong;
    ifstream file(filename.c_str());
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return formattedHexValuesAsLong;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string hexValue;
        while (iss >> hexValue) {
            long int x;
            stringstream ss;
            ss << hex << hexValue;
            ss >> x;

            ostringstream formattedStream;
            if ((x & 0x8000) == 0x8000) {
                formattedStream << "FFFFFF" << uppercase << setfill('0') << setw(4) << hex << x << "0000";
            } else {
                formattedStream << "000000" << uppercase << setfill('0') << setw(4) << hex << x << "0000";
            }

            formattedHexValuesAsLong.push_back(strtol(formattedStream.str().c_str(), NULL, 16));
        }
    }

    file.close();
    return formattedHexValuesAsLong;
}

long int twos_comp(long int num){
    num = ~num;
    return num +1;
}

long int multiplication(long int x, unsigned int h){
    long int input = 0, temp,temp1;
    long int result = 0;
    int i;
    for(i =0;i<16;i++){
        if((h>>i)&1!=0)
            break;
    }
    while(i<16){
        input = x;
        temp =0;
        temp1 = h>>i;
        if((temp1&1)!=0)
        {
            temp1 = temp1>>16;
            if((temp1&1) == 1)
                temp = twos_comp(input);
            else
                temp = input;    
        }
        result+= temp;
        result = result>>1;
        i++;
    }
    return result;
}

long int* convolution(long int x_arr[], unsigned int h_arr[], int size){
    int n,k;
    long int* y = new long int[size];
    long int temp;
    for(n=0;n<size;n++)
    {
        temp =0;
        for(k=0;k<256;k++){
            if((n-k)<0)
                break;
            temp += multiplication(x_arr[n-k],h_arr[k]);
        }
        y[n] = temp;
    }
    return y;
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << "filename filenamecoeff filenameout" <<endl;
        return 1;
    }

    string filename = argv[1];
    string filenamecoeff = argv[2];
    string filenameout = argv[3];

    long int* result_int;
    string result[1000];

    vector<long int> formattedHexValuesAsLong = readAndFormatHexValues(filename);
    long int xyz[1000];

    for (size_t i = 0; i < formattedHexValuesAsLong.size(); ++i)
    {
        xyz[i] = formattedHexValuesAsLong[i];
    }

    vector<unsigned int> hexValues = readHexadecimalValuesFromFile(filenamecoeff);
    unsigned int coeff[1000];

    for (size_t j = 0; j < hexValues.size(); ++j) 
    {
        coeff[j] = hexValues[j];
    }

    int size = sizeof(xyz)/sizeof(xyz[0]);
    result_int = convolution(xyz,coeff,size);
    int len;
    

    for(int i=0;i<size;i++)
    {
        string zeros ("");
        stringstream ss;
        ss<<uppercase<<hex<<result_int[i];

        result[i] = (ss.str());
        len = 16 - result[i].size();
        while(len > 0 )
        {
            zeros = zeros + "0";
            len--;
        }
        result[i]  = (zeros + result[i]).substr(6);
        
        // std::cout << result[i] << std::endl;
    }

    ofstream outputFile(filenameout.c_str());
    if (!outputFile.is_open()) {
        std::cerr << "Error opening the file: " << filenameout << std::endl;
        return 1;
    }

    for (int i = 0; i < size; i++) {
        outputFile << result[i] << std::endl;
    }
    outputFile.close();

    return 0;
}

