#include <iostream>
#include "huffman.hpp"
using namespace std;

int main(int argc, char* argv[]){
    if(argc != 3){
        cout << "Unable to detect Files";
		exit(1);}
    huffman f(argv[1], argv[2]);
    f.decompress();
    cout << "Decompression successful" << endl;
    return 0;}