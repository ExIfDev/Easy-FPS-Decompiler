#include <iostream>
#include <string>
#include "PakFile.h"
#include "windows.h"
int main(int argc, char* argv[])
{

    std::string option;
    std::string inPath;
    std::string outPath;

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "============================================================" << std::endl;
    std::cout << "=           Easy FPS Pak tool V1.00 by Aexadev             =" << std::endl;
    std::cout << "============================================================" << std::endl;
    std::cout << "GitHub: https://github.com/ExIfDev" << std::endl;
    std::cout << "YouTube: www.youtube.com/@Aexadev" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    if (argc == 1)
    {
        std::cout<<"USAGE: EFPSE_Tools.exe <option> <Input> <output>"<< "\n";
        std::cout <<"options: -u (unpack .pak), -p (protect .pak), -l (list files in .pak)" << "\n";
        return 0;
   
    }
    else if (argc < 3)
    {
        std::cout << "[ERROR] Expected at least 2 arguments but got "<< argc -1 << "\n";
        return 0;

    }


    if (argc >= 3)
    {
        option = argv[1];
        inPath = argv[2];
        if (option == "-l") //LIST
        {
            Pakfile::OutDict(inPath);
            return 0;

        }
    }
    if (argc == 4)
    {
        outPath = argv[3];

        if (option == "-u") //UNPACK
        {

            Pakfile::Unpack(inPath, outPath);
            return 0;
        }
        else if (option == "-p")//PROTECT
        {
            Pakfile::Protect(inPath, outPath);
            return 0;
        }
        else
        {
            std::cout << "unrecognized option " << option << " ,aborting." << std::endl;
            std::exit(1);
        }

    }
    return 0;
}
