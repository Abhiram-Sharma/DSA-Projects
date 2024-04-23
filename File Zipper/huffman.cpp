#include "huffman.hpp"

void huffman::createArr() {
    for(int i=0;i<128;i++){
        arr.push_back(new Node());
        arr[i]->data = i;
        arr[i]->freq = 0;}}

void huffman::traverse(Node* r, string str){
    if(r->left == NULL && r->right == NULL){
        r->code = str;
        return;}
    traverse(r->left, str + '0');
    traverse(r->right, str + '1');}

int huffman::binToDec(string inStr){
    int res = 0;
    for (auto c : inStr){
        res = res * 2 + c - '0';}
    return res;}

string huffman::decToBin(int inNum){
    string temp = "", res = "";
    while(inNum > 0){
        temp += (inNum % 2 + '0');
        inNum /= 2;}
    res.append(8 - temp.length(), '0');
    for(int i = temp.length() - 1; i >= 0; i--){
        res += temp[i];}
    return res;}

void huffman::buildTree(char a_code, string& path){
    Node* curr = root;
    for(int i = 0; i < path.length(); i++){
        if(path[i] == '0'){
            if(curr->left == NULL){
                curr->left = new Node();}
            curr = curr->left;}
        else if (path[i] == '1') {
            if (curr->right == NULL) {
                curr->right = new Node();}
            curr = curr->right;}}
    curr->data = a_code;}

void huffman::createMinHeap(){
    char id;
    inFile.open(inFileName, ios::in);
    inFile.get(id);
    while(!inFile.eof()){
        arr[id]->freq++;
        inFile.get(id);}
    inFile.close();
    for(int i=0;i<128;i++){
        if(arr[i]->freq>0){
            minHeap.push(arr[i]);}}}

void huffman::createTree(){
    Node *left, *right;
    priority_queue <Node*, vector<Node*>, Compare> tempPQ(minHeap);
    while(tempPQ.size() != 1){
        left = tempPQ.top();
        tempPQ.pop(); 
        right = tempPQ.top();
        tempPQ.pop();
        root = new Node();
        root->freq = left->freq + right->freq;
        root->left = left;
        root->right = right;
        tempPQ.push(root);}}

void huffman::createCodes(){
    traverse(root, "");}

void huffman::saveEncodedFile(){
    inFile.open(inFileName, ios::in);
    outFile.open(outFileName, ios::out | ios::binary);
    string in = "";
    string s = "";
    char id;
    in += (char)minHeap.size();
    priority_queue <Node*, vector<Node*>, Compare> tempPQ(minHeap);
    while(!tempPQ.empty()){
        Node* curr = tempPQ.top();
        in += curr->data;
        s.assign(127 - curr->code.length(), '0');
        s += '1';
        s += curr->code;
        in += (char)binToDec(s.substr(0, 8));
        for(int i = 0; i < 15; i++){
            s = s.substr(8);
            in += (char)binToDec(s.substr(0, 8));}
        tempPQ.pop();}
    s.clear();
    inFile.get(id);
    while(!inFile.eof()){
        s += arr[id]->code;
        while(s.length() > 8){
            in += (char)binToDec(s.substr(0, 8));
            s = s.substr(8);}
        inFile.get(id);}
    int count = 8 - s.length();
	if(s.length() < 8){
		s.append(count, '0');}
	in += (char)binToDec(s);
    in += (char)count;
	outFile.write(in.c_str(), in.size());
	inFile.close();
	outFile.close();}

void huffman::saveDecodedFile() {
    inFile.open(inFileName, ios::in | ios::binary);
    outFile.open(outFileName, ios::out);
    unsigned char size;
    inFile.read(reinterpret_cast<char*>(&size), 1);
    inFile.seekg(-1, ios::end);
    char count0;
    inFile.read(&count0, 1);
    inFile.seekg(1 + 17 * size, ios::beg);
    vector<unsigned char> text;
    unsigned char textseg;
    inFile.read(reinterpret_cast<char*>(&textseg), 1);
    while(!inFile.eof()){
        text.push_back(textseg);
        inFile.read(reinterpret_cast<char*>(&textseg), 1);}
    Node *curr = root;
    string path;
    for(int i=0;i<text.size()-1;i++){
        path = decToBin(text[i]);
        if(i==text.size()-2){
            path = path.substr(0, 8 - count0);}
        for(int j = 0; j < path.size(); j++){
            if(path[j] == '0'){
                curr = curr->left;}
            else{
                curr = curr->right;}
            if(curr->left == NULL && curr->right == NULL){
                outFile.put(curr->data);
                curr = root;}}}
    inFile.close();
    outFile.close();}

void huffman::getTree(){
    inFile.open(inFileName, ios::in | ios::binary);
    unsigned char size;
    inFile.read(reinterpret_cast<char*>(&size), 1);
    root = new Node();
    for(int i = 0; i < size; i++){
        char aCode;
        unsigned char hCodeC[16];
        inFile.read(&aCode, 1);
        inFile.read(reinterpret_cast<char*>(hCodeC), 16);
        string hCodeStr = "";
        for(int i=0;i<16;i++){
            hCodeStr += decToBin(hCodeC[i]);}
        int j = 0;
        while(hCodeStr[j] == '0'){
            j++;}
        hCodeStr = hCodeStr.substr(j+1);
        buildTree(aCode, hCodeStr);}
    inFile.close();}

void huffman::compress(){
    createMinHeap();
    createTree();
    createCodes();
    saveEncodedFile();}

void huffman::decompress(){
    getTree();
    saveDecodedFile();}