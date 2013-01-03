#include <cstdio>
#include <string>
using namespace std;

class Channel {
    private:
        string information;

    public:
        Channel() {
            this->information = "";
        }

        void readInformation(string &file_path) {
            FILE *f = fopen(file_path.c_str(), "r");

            // Procitaj cijelu datoteku
            char c;

            while (!feof(f)) {
                fscanf(f, "%c", &c);

                this->information += c;
            }

            fclose(f);
        }

        void applyNoise() {
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
