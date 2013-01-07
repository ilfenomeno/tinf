#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <queue>
using namespace std;

class EDecoder {
    private:
        // Privatne varijable
        string information; // originalna informacija
        string decodedInformation;
        map<string, char> symbolMap;  // mapa (rjecnik) simbola za svaki kod

    public:
        // Javne funkcije
        EDecoder() {
        }

        void readInformation(string &file_path) {
            FILE *f = fopen(file_path.c_str(), "r");

            // Procitaj cijelu datoteku
            this->information = "";

            char c;

            c = fgetc(f);

            while (!feof(f)) {
                this->information += c;

                c = fgetc(f);
            }

            fclose(f);
        }

        void decode() {
            this->decodedInformation = "";

            int n = this->information.size();

            string s = "";

            for (int i = 0; i < n; i++) {
                s += this->information[i];

                if (this->symbolMap.count(s)) {
                    this->decodedInformation += this->symbolMap[s];
                    s = "";
                }
            }

            if (s != "") {
                this->decodedInformation += '#';
            }
        }

        void readSymbolCodes(string &file_path) {
            FILE *f = fopen(file_path.c_str(), "r");

            char symbol;
            char buffer[4096];

            while (!feof(f)) {
                fscanf(f, "%c=%s\n", &symbol, buffer);

                string code = buffer;

                symbolMap[code] = symbol;
            }

            fclose(f);
        }

        void writeDecodedInformation(string &file_path) {
            FILE *f = fopen(file_path.c_str(), "w");

            fprintf(f, "%s", decodedInformation.c_str());

            fclose(f);
        }
};

int main(int argc, char **argv) {
    string edecoderIn_path = argv[1];
    string ecoderCode_path = argv[2];
    string destination_path = argv[3];

    EDecoder edecoder = EDecoder();

    edecoder.readInformation(edecoderIn_path);
    edecoder.readSymbolCodes(ecoderCode_path);
    edecoder.decode();
    edecoder.writeDecodedInformation(destination_path);
    
    return 0;
}
