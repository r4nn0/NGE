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
    std::vector<std::vector<char>> dataTotal;
    unsigned sizeTot=0;
    while(argc){
        i++;
        std::cout << argv[i] << std::endl;
        if(!strcmp(argv[i], "-o")){
            outName=argv[i+1];
            i+=2;
            argc-=2;
            continue;
        }
        std::ifstream in(argv[i], std::ios::binary);
        std::vector<char> data;
        in.seekg(0, std::ios::end);
        unsigned long long size=in.tellg();
        in.seekg(0, std::ios::beg);

        data.resize(size);
        in.read(&data[0], size);
        in.close();
        dataTotal.push_back(data);
        argc--;
        count++;
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
        outData+="BEG";
        for(int temp=0;temp<dataTotal[frames].size();temp++){
            outData+=dataTotal[frames][temp];
        }
        outData+="END";
    }   
    std::ofstream out(outName, std::ios::out | std::ios::binary);
    out.write(outData.c_str(), outData.length());
    out.close();
    return 0;
}