#include <iostream>
#include<fstream>
#include<cstdio>
#include <map>
using namespace std;

struct Record
{
    char nombre[30];
    char carrera[20];
    int ciclo;

    void setData() {
        cout << "Alumno:";
        cin >> nombre;
        cout << "Carrera: ";
        cin >> carrera;
        cout << "Ciclo: ";
        cin >> ciclo;
    }

    void showData() {
        cout << "\nNombre: " << nombre;
        cout << "\nCarrera: " << carrera;
        cout << "\nCiclo : " << ciclo;
    }

    string getKey() {
        return nombre;
    } 
};

struct Index{
    char key[30];
    long pos;
};

class RandomFile {
private:
    string fileName;
    string indexName;
    //map: mantiene ordenado las entradas
    map<string, long> index;

public:
    RandomFile(string _fileName) {
        this->fileName = _fileName;
        this->indexName = _fileName + "_ind";
        readIndex();
    }
    
    ~RandomFile(){
        writeIndex();
    }

    /*
    * leer el indice desde disco
    */
    void readIndex()
    {
        ifstream indexFile;
        indexFile.open(this->indexName, ios::binary);
        Index obj;
        while (indexFile.read((char *) &obj, sizeof(Index))) {
            string temp = obj.key;
            this->index.insert({temp,obj.pos});
        }
        indexFile.close();
    }

    /*
    * Regresa el indice al disco
    */
    void writeIndex(){
        ofstream indexFile;
        indexFile.open(this->indexName, ios::binary);
        Index obj;
        for(auto const &i : this->index) {
            for(int j = 0;j<30;j++){
                if(j >= i.first.size()){
                    obj.key[j] = ' ';
                }
                else{
                    obj.key[j] = i.first[j];
                }
            }
            obj.pos = i.second;
            indexFile.write((char *) &obj, sizeof(Index));
        }
        indexFile.close();
    }

    /*
    * Escribe el registro al final del archivo de datos. Se actualiza el indice. 
    */
    void write_record(Record record) {
        ofstream dataFile;
        dataFile.open(this->fileName, ios::app | ios::binary);
        long posFisica = dataFile.tellp();
        dataFile.write((char*)&record, sizeof(Record));
        string temp = record.getKey();
        index.insert({temp,posFisica});
        dataFile.close();
    }


    /*
    * Busca un registro que coincida con la key
    */
    Record* search(string key) {
        Record* result = nullptr;
        Record temp;

        ifstream dataFile;
        dataFile.open(this->fileName, ios::binary);
        dataFile.seekg(this->index.find(key)->second, ios::beg);
        dataFile.read((char *) &(temp), sizeof(Record));

        result = &temp;

        dataFile.close();
        return result;
    }

    /*
   * Muestra todos los registros de acuerdo como fueron insertados en el archivo de datos
   */
    void scanAll() {
        ifstream dataFile;
        dataFile.open(this->fileName, ios::binary);
        Record obj;
        while (dataFile.read((char *) &obj, sizeof(Record))) {
            obj.showData();
        }
        dataFile.close();
    }

    /*
   * Muestra todos los registros de acuerdo a como estan ordenados en el indice
   */
    void scanAllByIndex() {
       ifstream dataFile;
       dataFile.open(this->fileName, ios::binary);
       Record obj;
       for(auto const &i : this->index) {
            dataFile.seekg(i.second, ios::beg);
            dataFile.read((char *) &obj, sizeof(Record));
            obj.showData();
        }
        dataFile.close();
    }

};


