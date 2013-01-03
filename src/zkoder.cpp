#include <cstdio>
#include <string>
using namespace std;

class ZCoder {
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
        ZCoder() {
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

        void encode() {
            // Samo je 16 mogucih podataka, zato je bolje to odmah
            // izracunat za polinom x^3 + x + 1, nego svaki put racunat
            this->precomputeCRC();
        }

        void writeEncodedInformation(string &file_path) {
            FILE *f = fopen(file_path.c_str(), "w");

            string encodedInformation = "";

            int n = this->information.size(); 

            // Dodaj 0 na kraju ako je potrebno da bude djeljivo s 4
            for (int i = 0; i < (4 - n % 4) % 4; i++)
                this->information += '0';

            // Mozda se promijenio n pa ga treba opet nac
            n = this->information.size();

            for (int i = 0; i < n; i += 4) {
                string data = this->information.substr(i, 4); 

                // Mora se pretvorit iz stringa u broj
                bool d3 = data[0] == '1';
                bool d2 = data[1] == '1';
                bool d1 = data[2] == '1';
                bool d0 = data[3] == '1';

                int d = 8*d3 + 4*d2 + 2*d1 + d0;

                encodedInformation += data + CRC[d];
            }

            fprintf(f, "%s", encodedInformation.c_str());

            fclose(f);
        }
};

int main(int argc, char **argv) {
    string zcoderIn_path = argv[1];
    string zcoderOut_path = argv[2];

    ZCoder zcoder = ZCoder();

    zcoder.readInformation(zcoderIn_path);
    zcoder.encode();
    zcoder.writeEncodedInformation(zcoderOut_path);

    return 0;
}
