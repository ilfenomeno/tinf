#include <cstdio>
#include <string>
using namespace std;

class ZDecoder {
    private:
        string information;
        string decodedInformation;
        int syndromeIndex[10];

        // Mapiraj poziciju pogreske za sve sindrome
        // Sindromi su u intervalu <000, 111]
        // 000 znaci da nema pogreske
        void mapSyndromes() {
            // Nema pogreske
            syndromeIndex[0] = 0;

            // Sindrom 011 znaci da se pogreska desila na prvom bitu
            syndromeIndex[3] = 1;

            // Sindrom 110 -> pogreska na drugom bitu
            syndromeIndex[6] = 2;

            syndromeIndex[7] = 3; // 111
            syndromeIndex[5] = 4; // 101
            syndromeIndex[1] = 5; // 001
            syndromeIndex[2] = 6; // 010
            syndromeIndex[4] = 7; // 100
        }

        int computeSyndrome(string &s) {
            // String s moramo prosirit sa 000 (extendat)
            string sex = s + "000";

            int n = sex.size();

            bool r3, r2, r1;
            
            r3 = r2 = r1 = 0;

            for (int i = 0; i < n; i++) {
                bool bit = sex[i] == '1';

                bool r3_old = r3;
                bool r2_old = r2;
                bool r1_old = r1;

                r3 = r2_old;
                r2 = r1_old ^ r3_old;
                r1 = bit ^ r3_old;
            }

            int syndrome = 4*r3 + 2*r2 + r1;

            return syndrome;
        }

        // Izracunaj sindrom, detektiraj na kojem se bitu desila pogreska
        // te vrati tu poziciju
        int getErrorIndex(string &s) {
            int syndrome = this->computeSyndrome(s);

            return this->syndromeIndex[syndrome];
        }

    public:
        ZDecoder() {
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
                string data = this->information.substr(i, 7);

                int errorIndex = this->getErrorIndex(data);

                // Ako ima pogreske, ispravi je
                if (errorIndex) {
                   data[7 - errorIndex] = !(data[7 - errorIndex] == '1') + '0';
                }

                // Makni zadnja 3 zalihosna bita
                this->decodedInformation += data.substr(0, 4);
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
