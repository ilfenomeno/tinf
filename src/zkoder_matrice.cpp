#include <cstdio>
#include <string>
using namespace std;

class ZCoder {
    private:
        string information;
        string encodedInformation;
        int G[5];

        void setG() {
            // Generirajuci polinom je x^3 + x + 1
            // 1011 = 11
            this->G[3] = 11;
            this->G[2] = this->G[3] << 1;
            this->G[1] = this->G[3] << 2;
            this->G[0] = this->G[3] << 3;
        }

        string getEncodedData(string &data) {
            int res = 0;

            for (int i = 0; i < 4; i++) {
                bool bit = data[i] == '1';

                res ^= bit * this->G[i];
            }
 
            string s = "";

            for (int i = 6; i >= 0; i--) {
                bool bit = (res & (1 << i)) != 0;

                s += bit + '0';
            }

            return s;
        }

    public:
        ZCoder() {
            this->setG();
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

        void encode() {
            int n = this->information.size(); 

            // Dodaj 0 na kraju ako je potrebno da bude djeljivo s 4
            for (int i = 0; i < (4 - n % 4) % 4; i++)
                this->information += '0';

            // Mozda se promijenio n pa ga treba opet nac
            n = this->information.size();

            this->encodedInformation = "";

            for (int i = 0; i < n; i += 4) {
                string data = this->information.substr(i, 4); 

                this->encodedInformation += this->getEncodedData(data);
            }
        }

        void writeEncodedInformation(string &file_path) {
            FILE *f = fopen(file_path.c_str(), "w");

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
