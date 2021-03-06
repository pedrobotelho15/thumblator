#include "thumblator.hpp"

//PARAMETER BY TERMINAL
//argv[0] = ./thumblator
//argv[1] = <input>
//argv[2] = <output>
int main(int argc, char* argv[]){ 
    
    //PARAMETER CHECK
    if(argc > 3){
        cout << "\033[1;37mThumblator:\033[0m \033[1;31mFatal error\033[0m: too much parameters.\nAborting.\n";
        return 0;
    }
    else if(argc < 2){
        cout << "\033[1;37mThumblator:\033[0m \033[1;31mFatal error\033[0m: no input file.\nAborting.\n";
        return 0;
    }

    //OPENING AND CHECKING INPUT FILE
    ifstream input_file(argv[1]);
    if(!(input_file.is_open())){
        cout << "\033[1;37mThumblator:\033[0m \033[1;31mFatal error\033[0m: file doesn't exist.\nAborting.\n";
        return 0;
    }
    
    //EXTENSION CHECK
    string input_file_name;
    input_file_name = argv[1];
    if (input_file_name.find(".in") == input_file_name.npos){
        cout << "\033[1;37mThumblator:\033[0m \033[1;31mFatal error\033[0m: invalid extension.\nAborting.\n";
        return 0;
    }
    
    //FILE TO STRING
    stringstream auxiliary_buffer;
    auxiliary_buffer << input_file.rdbuf();
    string source_code;
    source_code = auxiliary_buffer.str();

    long unsigned int step = 0;
    short line_number = 0;
    long int numeric_opcode = 0;
    string opcode;

    //REMOVE SPACES
    while (source_code.find(' ') != source_code.npos)
        source_code.erase(source_code.find(' '), 1);
    
    //INSTRUCTION DECODE
    while (step < source_code.size()){
        while (source_code[step] != '\n' && step != source_code.size())
            opcode += source_code[step++];
        numeric_opcode = strtol(opcode.c_str(), NULL, 16);
        opcode = decodeInstructions(numeric_opcode, ++line_number);
        if (opcode.empty()) return 0;
        source_code.replace(step, 1, ":  " + opcode + "\n");
        step += opcode.size() + 4;
        opcode.clear();
    }
    
    //REMOVE LAST LINE FEED
    source_code.erase(source_code.size() - 1, 1);
    
    //OUTPUT FILE CREATION
    ofstream output_file;
    if (argc == 2)
        output_file.open(input_file_name.replace(input_file_name.find(".in"), 3, ".out"));
    else
        output_file.open(argv[2]);
    
    //STRING TO FILE
    output_file << source_code;

    //CLOSING FILES
    output_file.close();
    input_file.close();
    return 0;
} 
