#include "RandomFile.h"

using namespace std;

void writeFile(string filename){
    RandomFile file(filename);
    Record record;
    for (int i = 0; i < 3; i++)
    {
        record.setData();
        file.write_record(record);
    } 
}

void readFile(string filename){
    RandomFile file(filename);
    cout<<"\n--------- show all data -----------\n";
    file.scanAll();
    cout<<"\n--------- show all sorted data -----------\n";
    file.scanAllByIndex();
    string searching;
    cout<<"\n--------- Insert a key to search ............\n";
    cin>>searching;
    Record* rec = nullptr;
    rec = file.search(searching); 
    rec->showData(); 
}

int main(){
    writeFile("data12.dat");
    readFile("data12.dat");
    return 0;
}