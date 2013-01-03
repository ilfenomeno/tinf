#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

class Channel {
    private:
        string information;
        int errorProb; 

    public:
        Channel() {
            this->information = "";
            this->errorProb = 2000; // 1/2000 je vjerojatnost pogreske, al ovako je lakse racunat
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

        void applyNoise() {
            srand(time(NULL)); 

            int n = this->information.size();

            for (int i = 0; i < n; i++) {
                // Baca brojeve u intervalu [0, errorProb>, tj. [0, 2000>
                int ranNum = rand() % this->errorProb;

                // Ako je pogodjen neki broj (bilo koji) od 2000 brojeva, onda promijeni bit
                if (ranNum == 666) {
                    char c = this->information[i];

                    if (c == '0') {
                        c = '1';
                    }
                    else if (c == '1') {
                        c = '0';
                    }

                    this->information[i] = c;
                }
            }
        }

        void writeNoisyInformation(string &file_path) {
            FILE *f = fopen(file_path.c_str(), "w");

            fprintf(f, "%s", this->information.c_str());

            fclose(f);
        }
};

int main(int argc, char **argv) {
    string channelIn_path = argv[1];
    string channelOut_path = argv[2];

    Channel channel = Channel();

    channel.readInformation(channelIn_path);
    channel.applyNoise();
    channel.writeNoisyInformation(channelOut_path);

    return 0;
}
