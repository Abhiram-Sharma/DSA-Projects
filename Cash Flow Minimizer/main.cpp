#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <algorithm>
#include <climits>
using namespace std;
struct Bank{
    string name;
    int netAmount;
    unordered_set<string> types;};
template <typename K, typename V>
class Pair{
    private:
        K key;
        V value;
    public:
        Pair(K key, V value) : key(key), value(value) {}
        K getKey() { return key; }
        void setKey(K key) { this->key = key; }
        V getValue() { return value; }
        void setValue(V value) { this->value = value; }};

int getMinIndex(vector<Bank>& listOfNetAmounts, int numBanks){
    int min = INT_MAX;
    int minIndex = -1;
    for(int i=0;i<numBanks;i++){
        if (listOfNetAmounts[i].netAmount == 0){
            continue;}
        if(listOfNetAmounts[i].netAmount<min){
            minIndex = i;
            min = listOfNetAmounts[i].netAmount;}}
    return minIndex;}

int getSimpleMaxIndex(vector<Bank>& listOfNetAmounts, int numBanks){
    int max = INT_MIN;
    int maxIndex = -1;
    for(int i=0;i<numBanks;i++){
        if(listOfNetAmounts[i].netAmount==0){
            continue;}
        if(listOfNetAmounts[i].netAmount>max){
            maxIndex = i;
            max = listOfNetAmounts[i].netAmount;}}
    return maxIndex;}

Pair<int, string> getMaxIndex(vector<Bank>& listOfNetAmounts, int numBanks, int minIndex, vector<Bank>& input, int maxNumTypes){
    int max = INT_MIN;
    int maxIndex = -1;
    string matchingType = "";
    for(int i=0;i<numBanks;i++){
        if(listOfNetAmounts[i].netAmount==0){
            continue;}
        if(listOfNetAmounts[i].netAmount<0){
            continue;}
        vector<string> v;
        auto ls = listOfNetAmounts[minIndex].types.begin();
        auto rs = listOfNetAmounts[i].types.begin();
        while(ls != listOfNetAmounts[minIndex].types.end() && rs != listOfNetAmounts[i].types.end()){
            string type1 = *ls;
            string type2 = *rs;
            if(type1 == type2){
                v.push_back(type1);}
            ls++;
            rs++;}
        if(!v.empty() && max < listOfNetAmounts[i].netAmount){
            max = listOfNetAmounts[i].netAmount;
            maxIndex = i;
            matchingType = v[0];}}
    return Pair<int, string>(maxIndex, matchingType);}

void printAns(vector<vector<Pair<int, string>>>& ansGraph, int numBanks, vector<Bank>& input){
    cout<<"\nThe Transactions for minimum cash-flow are : \n";
    for(int i=0;i<numBanks;i++){
        for(int j=0;j<numBanks;j++){
            if(i == j){
                continue;}
            if(ansGraph[i][j].getKey() != 0 && ansGraph[j][i].getKey() != 0){
                if(ansGraph[i][j].getKey() == ansGraph[j][i].getKey()){
                    ansGraph[i][j].setKey(0);
                    ansGraph[j][i].setKey(0);}
                else if(ansGraph[i][j].getKey() > ansGraph[j][i].getKey()){
                    ansGraph[i][j].setKey(ansGraph[i][j].getKey() - ansGraph[j][i].getKey());
                    ansGraph[j][i].setKey(0);
                    cout<<input[i].name<<" pays Rs "<<ansGraph[i][j].getKey()<<" to "<<input[j].name<<" via "<<ansGraph[i][j].getValue()<<endl;}
                else{
                    ansGraph[j][i].setKey(ansGraph[j][i].getKey() - ansGraph[i][j].getKey());
                    ansGraph[i][j].setKey(0);
                    cout<<input[j].name<<" pays Rs "<<ansGraph[j][i].getKey()<<" to "<<input[i].name<<" via "<<ansGraph[j][i].getValue()<<endl;}}
            else if(ansGraph[i][j].getKey() != 0){
                cout << input[i].name << " pays Rs " << ansGraph[i][j].getKey() << " to " << input[j].name << " via " << ansGraph[i][j].getValue() << endl;}
            else if(ansGraph[j][i].getKey() != 0){
                cout << input[j].name << " pays Rs " << ansGraph[j][i].getKey() << " to " << input[i].name << " via " << ansGraph[j][i].getValue() << endl;}
            ansGraph[i][j].setKey(0);
            ansGraph[j][i].setKey(0);}}
    cout<<endl;}

void minimizeCashFlow(int numBanks, vector<Bank>& input, unordered_map<string, int>& indexOf, int numTransactions, vector<vector<int>>& graph, int maxNumTypes){
    vector<Bank> listOfNetAmounts(numBanks);
    for(int b=0;b<numBanks;b++){
        listOfNetAmounts[b].name = input[b].name;
        listOfNetAmounts[b].types = input[b].types;
        int amount = 0;
        for(int i=0;i<numBanks;i++){
            amount += graph[i][b];}
        for(int j=0;j<numBanks;j++){
            amount += (-1) * graph[b][j];}
        listOfNetAmounts[b].netAmount = amount;}
    vector<vector<Pair<int, string>>> ansGraph(numBanks, vector<Pair<int, string>>(numBanks, Pair<int, string>(0, "")));
    int numZeroNetAmounts = 0;
    for(int i = 0; i < numBanks; i++){
        if(listOfNetAmounts[i].netAmount == 0){
            numZeroNetAmounts++;}}
    while(numZeroNetAmounts != numBanks){
        int minIndex = getMinIndex(listOfNetAmounts, numBanks);
        Pair<int, string> maxAns = getMaxIndex(listOfNetAmounts, numBanks, minIndex, input, maxNumTypes);
        int maxIndex = maxAns.getKey();
        if(maxIndex == -1){
            ansGraph[minIndex][0].setKey(abs(listOfNetAmounts[minIndex].netAmount));
            ansGraph[minIndex][0].setValue(*input[minIndex].types.begin());
            int simpleMaxIndex = getSimpleMaxIndex(listOfNetAmounts, numBanks);
            ansGraph[0][simpleMaxIndex].setKey(abs(listOfNetAmounts[minIndex].netAmount));
            ansGraph[0][simpleMaxIndex].setValue(*input[simpleMaxIndex].types.begin());
            listOfNetAmounts[simpleMaxIndex].netAmount += listOfNetAmounts[minIndex].netAmount;
            listOfNetAmounts[minIndex].netAmount = 0;
            if(listOfNetAmounts[minIndex].netAmount == 0){
                numZeroNetAmounts++;}
            if(listOfNetAmounts[simpleMaxIndex].netAmount == 0){
                numZeroNetAmounts++;}}
        else{
            int transactionAmount = min(abs(listOfNetAmounts[minIndex].netAmount), listOfNetAmounts[maxIndex].netAmount);
            ansGraph[minIndex][maxIndex].setKey(transactionAmount);
            ansGraph[minIndex][maxIndex].setValue(maxAns.getValue());
            listOfNetAmounts[minIndex].netAmount += transactionAmount;
            listOfNetAmounts[maxIndex].netAmount -= transactionAmount;
            if(listOfNetAmounts[minIndex].netAmount == 0){
                numZeroNetAmounts++;}
            if(listOfNetAmounts[maxIndex].netAmount == 0){
                numZeroNetAmounts++;}}}
    printAns(ansGraph, numBanks, input);}

int main(){
    cout << "\nWelcome to CASH FLOW MINIMIZER\n- by Abhiram Sharma\n";
    cout << "\nThis program minimizes the number of transactions processed among multiple banks that make use of different modes of payment. There is one world bank with all payment modes to act as the intermediary between banks that have no common modes of payment.\n";
    cout << "\nEnter the number of banks in transactions.\n";
    int numBanks;
    cin >> numBanks;
    vector<Bank> input(numBanks);
    unordered_map<string, int> indexOf;
    cout << "\nEnter the details of the banks and transactions as stated below : \n";
    cout << "Bank name, number of payment modes it has, and the payment modes.\n";
    cout << "Bank name and payment modes should not contain spaces.\n";
    int maxNumTypes = 0;
    for(int i=0;i<numBanks;i++){
        if(i == 0){
            cout << "World Bank: ";}
        else{
            cout << "Bank " << i << ": ";}
        string name;
        cin >> name;
        input[i].name = name;
        indexOf[name] = i;
        int numTypes;
        cin >> numTypes;
        if(i == 0){
            maxNumTypes = numTypes;}
        unordered_set<string> types(numTypes);
        for(int j=0;j<numTypes;j++){
            string type;
            cin >> type;
            types.insert(type);}
        input[i].types = types;}
    cout << "Enter number of transactions : ";
    int numTransactions;
    cin >> numTransactions;
    vector<vector<int>> graph(numBanks, vector<int>(numBanks, 0));
    cout << "Enter the details of the transactions as stated below:\n";
    cout << "Bank name from which the transaction originates, bank name to which the transaction is made, and the amount.\n";
    for(int i = 0; i < numTransactions; i++){
        string from, to;
        int amount;
        cin >> from >> to >> amount;
        if(indexOf.find(from) != indexOf.end() && indexOf.find(to) != indexOf.end()){
            int fromIdx = indexOf[from];
            int toIdx = indexOf[to];
            graph[fromIdx][toIdx] = amount;}
        else{
            cout << "Invalid bank names: " << from << ", " << to << endl;}}
    minimizeCashFlow(numBanks, input, indexOf, numTransactions, graph, maxNumTypes);
    return 0;}
