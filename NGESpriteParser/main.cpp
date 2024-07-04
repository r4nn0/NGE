#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
void usage(){
    std::cout << "Usage: ngesp.exe [OPTION] file1 file2 ..." << std::endl
              <<    "\tOptions:"
              <<        "\t\t-o [FILENAME]\t\t-- Specify output file name" << std::endl;
}

int main(int argc, char* argv[]){
    if(argc<2){
        usage();
        return 1;
    }
    argc--;
    std::string outName="";
    std::string outData="";
    int i=0;
    std::vector<char*> dataTotal;
    char* data = (char*)malloc(1);
    int sizeTot=0;
    while(argc){
        i++;
        if(!strcmp(argv[i], "-o")){
            outName=argv[i+1];
            i+=2;
        }
        std::ifstream in(argv[i], std::ios::binary | std::ios::out);
        in.seekg(0, std::ios::end);
        unsigned int size=in.tellg();
        sizeTot+=size;
        data = (char*)realloc(data, sizeTot);
        
        in.read(&data[sizeTot-size], size);
        in.close();
        argc--;
    }
    if(outName=="") outName=argv[1];
    outData+=(char)outName.length();
    outData+=outName;
    outData+=(char)i;
    outName+=".ngesprite";
    
        outData+="BEG";
        outData+=data;
        outData+="END";
        
    std::ofstream out(outName, std::ios::out | std::ios::binary);
    out.write(outData.c_str(), outData.length());
    out.close();
    return 0;
}