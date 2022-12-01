#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Entry {
    uint32_t ptr;
    uint32_t len;
};
void Usage(void) {
    cout << "SDB Extraction tool (c++ I/O practice)\n\n";
    cout << "Usage:\n";
    cout << "  -e\tExtract all content\n\n";
}

void ExtractFile(const char* file) {
    ifstream input(file, ifstream::binary);
    uint32_t i, k;
    if (input.is_open()) {
        //Читаем заголовок
        uint32_t count; //количество
        input.read(reinterpret_cast<char*>(&count), sizeof(count));
        uint32_t* pointers = new uint32_t[count]; //массив поинтеров
        for (i = 0; i < count; i++) {
            input.read((char*)&pointers[i], sizeof(uint32_t));
        }
        cout << "Block Count: " << count << "\n\n";

        //Читаем остальное
        for (i = 0; i < count; i++) {
            uint32_t curPos = pointers[i];
            input.seekg(curPos, input.beg);
            uint32_t block_count;
            input.read((char*)(&block_count), sizeof(uint32_t));
            cout << "Block " << to_string(i+1) << ": " << block_count << " lines" << "\n";
            Entry* entries = new Entry[block_count]; 
            string* text = new string[block_count];
            for (k = 0; k < block_count; k++) {
                //Таблица
                Entry e;
                input.read((char*)&e, sizeof(Entry));
                entries[k] = e;
            }
            for (k = 0; k < block_count; k++) {
                //Переходим по поинтерам и читаем строки
                input.seekg(entries[k].ptr + curPos, input.beg);
                getline(input, text[k], '\0');
            }
            string fn(file);
            fn.replace(fn.find(".sdb"), 4, "_" + to_string(i) + ".txt");
            ofstream outFile(fn);
            for (k = 0; k < block_count; k++) {
                outFile << text[k] << "\n";;
            }
            outFile.close();
        }
        input.close();
        cout << "Complete" << endl;
    }
}

int main(int argc, char** argv)
{
    if (argc == 3) {
        if (string(argv[1]) == "-e") {
            ExtractFile(argv[2]);
        }
    }  
    else 
        Usage();
}