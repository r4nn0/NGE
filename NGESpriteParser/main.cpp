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
    int count=0;
    std::vector<std::string> dataTotal;
    std::vector<unsigned long long> sizes;
    while(argc){
        i++;
        if(!strcmp(argv[i], "-o")){
            outName=argv[i+1];
            i+=2;
            argc-=2;
            continue;
        }
        std::FILE* in = std::fopen(argv[i], "rb");
        std::fseek(in, 0, SEEK_END);
        unsigned long long size=std::ftell(in);
        std::fseek(in, 0, SEEK_SET);
        std::vector<char> data;
        data.resize(size);
        std::fread(&data[0], sizeof(char), size, in);
        std::fclose(in);
        
        dataTotal.push_back(std::string(data.begin(), data.end()));
        std::cout << size << std::endl;
        sizes.push_back(size);
        argc--;
        count++;
        data.clear();
    }
    if(outName=="") {
        outName=argv[1];
        outName=outName.substr(0,outName.find_last_of("."));
    }
    outData+=(char)outName.length();
    outData+=outName;
    outData+=(char)count;
    outName+=".ngesprite";
    for(int frames=0;frames<dataTotal.size();frames++){
        for(int i=sizeof(unsigned long long)-1;i>=0;i--){
            outData+=(char)((sizes[frames]>>i*8)&0xff);
        }
        outData+="BEG";
        outData+=dataTotal[frames];
        outData+="END";
    }   
    std::ofstream out(outName, std::ios::out | std::ios::binary);
    out.write(outData.c_str(), outData.length());
    out.close();
    return 0;
}