#include <cstdio>
#include <map>
#include <string>
using namespace std;

class ZDecoder {
    private:
        string information;
        string decodedInformation;
        int G[5];
        bool HT[10][5];
        int syndromeIndex[10];
        map<string, int> encodedDataMap;

        // Postavi generirajucu matricu
        void setG() {
            // Generirajuci polinom je x^3 + x + 1
            // 1011 = 11
            this->G[3] = 11;
            this->G[2] = this->G[3] << 1;
            this->G[1] = this->G[3] << 2;
            this->G[0] = this->G[3] << 3;
        }

        string getEncodedData(int d) {
            int res = 0;

            for (int i = 0; i < 4; i++) {
                bool bit = (d & (1 << (3 - i))) != 0;

                res ^= bit * this->G[i];
            }
 
            string s = "";

            for (int i = 6; i >= 0; i--) {
                bool bit = (res & (1 << i)) != 0;

                s += bit + '0';
            }

            return s;
        }

        // Mapiraj sve kodirane podatke da znamo od kojih su podataka nastali
        void mapEncodedData() {
            for (int i = 0; i < 16; i++) {
                string encodedData = this->getEncodedData(i);

                this->encodedDataMap[encodedData] = i;
            }
        }

        // Postavi matricu provjere pariteta
        void setHT() {
            // Matrica provjere pariteta
            // 1 1 1 0 1 0 0
            // 0 1 1 1 0 1 0
            // 0 0 1 1 1 0 1
            // Nama treba transponirana matrica
            
            int init[7][3] = {  {1, 0, 0},
                                {1, 1, 0},
                                {1, 1, 1},
                                {0, 1, 1},
                                {1, 0, 1},
                                {0, 1, 0},
                                {0, 0, 1}   };

            for (int i = 0; i < 7; i++) {
                for (int j = 0; j < 3; j++) {
                    this->HT[i][j] = init[i][j];
                }
            }
        }

        // Mapiraj poziciju pogreske za sve sindrome
        // Sindromi su u intervalu <000, 111]
        // 000 znaci da nema pogreske
        void mapSyndromes() {
            // Nema pogreske
            syndromeIndex[0] = 0;

            // Sindrom 001 znaci da se pogreska desila na prvom bitu
            syndromeIndex[1] = 1;

            // Sindrom 010 -> pogreska na drugom bitu
            syndromeIndex[2] = 2;

            syndromeIndex[5] = 3; // 101
            syndromeIndex[3] = 4; // 011
            syndromeIndex[7] = 5; // 111
            syndromeIndex[6] = 6; // 110
            syndromeIndex[4] = 7; // 100
        }

        int computeSyndrome(string &s) {
            int syndrome = 0;

            for (int j = 0; j < 3; j++) {
                bool syndromeBit = 0;

                for (int i = 0; i < 7; i++) {
                    bool bit = s[i] == '1';

                    syndromeBit ^= bit * HT[i][j];
                }

                syndrome |= syndromeBit << (2 - j);
            }

            return syndrome;
        }

        string getDecodedData(string &encodedData) {
            int data = this->encodedDataMap[encodedData];

            string s = "";

            for (int i = 3; i >= 0; i--) {
                bool bit = (data & (1 << i)) != 0;

                s += bit + '0';
            }

            return s;
        }

        // Izracunaj sindrom, detektiraj na kojem se bitu desila pogreska
        // te vrati tu poziciju
        int getErrorIndex(string &s) {
            int syndrome = this->computeSyndrome(s);

            return this->syndromeIndex[syndrome];
        }

    public:
        ZDecoder() {
            this->setG();
            this->mapEncodedData();

            this->setHT();
            this->mapSyndromes();
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
            int n = this->information.size();

            this->decodedInformation = "";

            for (int i = 0; i < n; i += 7) {
                string encodedData = this->information.substr(i, 7);

                int errorIndex = this->getErrorIndex(encodedData);

                // Ako ima pogreske, ispravi je
                if (errorIndex) {
                   encodedData[7 - errorIndex] = !(encodedData[7 - errorIndex] == '1') + '0';
                }

                // Nadji odgovarajuci izvorni podatak za kodirani podatak
                this->decodedInformation += this->getDecodedData(encodedData);
            }
        }

        void writeDecodedInformation(string &file_path) {
            FILE *f = fopen(file_path.c_str(), "w");

            fprintf(f, "%s", this->decodedInformation.c_str());

            fclose(f);
        }
};

int main(int argc, char **argv) {
    string zdecoderIn_path = argv[1];
    string zdecoderOut_path = argv[2];

    ZDecoder zdecoder = ZDecoder();

    zdecoder.readInformation(zdecoderIn_path);
    zdecoder.decode();
    zdecoder.writeDecodedInformation(zdecoderOut_path);

    return 0;
}
