#include <cstdio>
#include <string>
using namespace std;

class ZDecoder {
    private:
        string information;
        string CRC[20];

        // "Preizracunavanje" svih mogucih zastitnih kodova sa generirajucim polinomom x^3 + x + 1
        void precomputeCRC() {
            bool r3, r2, r1;

            for (int i = 0; i < 16; i++) {
                int data = i;

                r3 = r2 = r1 = 0;

                // Dodaj 3 nule na kraj, tj. pomnozi sa 8, tj. left shiftaj za 3
                data <<= 3;

                for (int j = 6; j >= 0; j--) {
                    bool bit = data & (1 << j); // Bit na j-toj poziciji 

                    // Trebaju nam privremeni spremnici za r3, r2 i r1 jer se mijenjaju
                    bool r3_old = r3;
                    bool r2_old = r2;
                    bool r1_old = r1;

                    r3 = r2_old;
                    r2 = r1_old ^ r3_old;
                    r1 = bit ^ r3_old;
                }

                CRC[i] = "";

                CRC[i] += '0' + r3;
                CRC[i] += '0' + r2;
                CRC[i] += '0' + r1;
            }
        }

    public:
        ZDecoder() {
            this->information = "";
        }

        void readInformation(string &file_path) {
            FILE *f = fopen(file_path.c_str(), "r");

            // Procitaj cijelu datoteku
            char c;

            c = fgetc(f);

            while (!feof(f)) {
                this->information += c;

                c = fgetc(f);
            }

            fclose(f);
        }

        void decode() {
            // Samo je 16 mogucih podataka, zato je bolje to odmah
            // izracunat za polinom x^3 + x + 1, nego svaki put racunat
            this->precomputeCRC();
        }

        void writeDecodedInformation(string &file_path) {
            FILE *f = fopen(file_path.c_str(), "w");

            string decodedInformation = "";

            int n = this->information.size(); 

            fprintf(f, "%s", decodedInformation.c_str());

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
