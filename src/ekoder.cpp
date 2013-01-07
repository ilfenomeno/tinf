#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <queue>
using namespace std;

class ECoder {
    private:
        // Struktura <simbol, vjerojatost>
        struct SymProb {
            char symbol;
            double p;

            SymProb(const char &_symbol, double _p) : symbol(_symbol), p(_p) {}
        };

        // Klasa za kreiranje Huffmanovog stabla
        class HuffmanTree {
            // Cvor stabla
            private:
                // !!!!! Dogovorno ce desno dijete imat manju vjerojatnost !!!!!
                // Prema uputama onom ko ima manju vjerojatnost dajemo 0, a drugom
                // dajemo 1; dakle desnom djetetu dajemo 0, a lijevom 1
                struct Node {
                    double p;
                    char bit;
                    Node *leftChild;
                    Node *rightChild;
                };

                // Struktura za usporedjivanje cvorova u "redu s prioritetom" (aka PQ)
                struct PQCompare {
                    bool operator()(const Node* nl, const Node* nr) const {
                        return nl->p > nr->p;
                    }
                };

                map<char, string> codeMap;  // Mapa kodova za svaki simbol
                Node *root;

                // Privatne funkcije
                void traverse(Node *node, string code) {
                    // printf("%lf %c %s\n", node->p, node->bit, code.c_str());

                    // Ako nije korijen, jedino korijen nema postavljen bit
                    if (node->bit) {
                        code += node->bit; 
                    }

                    // Ako je trenutni cvor simbol, onda spremi njegov kod i izadji
                    // Ovo je dio trika (opisanog nize u kodu)
                    // Samo simbol ima lijevo, a nema desno dijete
                    if (node->leftChild && !node->rightChild) {
                        this->codeMap[node->leftChild->bit] = code;

                        return;
                    }
                    
                    this->traverse(node->leftChild, code);
                    this->traverse(node->rightChild, code);
                }

                void createCodeMap() {
                    this->traverse(this->root, "");
                }

                void deleteRecursive(Node *node) {
                    if (node->leftChild) {
                        this->deleteRecursive(node->leftChild);
                    }

                    if (node->rightChild) {
                        this->deleteRecursive(node->rightChild);
                    }

                    delete node;
                }

            public:
                HuffmanTree(vector<SymProb> &sp) {
                    priority_queue<Node *, vector<Node *>, PQCompare> pq;

                    int n = sp.size();

                    for (int i = 0; i < n; i++) {
                        // Kreiraj novi list i postavi njegovu vjerojatnost
                        // na vjerojatnost simbola
                        Node *node = new Node();
                        node->p = sp[i].p;

                        // Kreiraj novi cvor i postavi njegov bit na simbol
                        // Ovo je trik kad budemo putovali stablom da znamo
                        // kojem simbolu odgovara koja vjerojatnost
                        Node *symbol = new Node();
                        symbol->bit = sp[i].symbol;
                        
                        // Ovo je dio tog trika, ako cvor ima samo lijevo dijete
                        // onda znaci da smo dosli do kraja stabla i mozemo naci
                        // kod za simbol koji je u lijevom djetetu
                        node->leftChild = symbol;

                        pq.push(node);
                    }

                    while (pq.size() > 1) {
                        // Dogovorno manju vjerojatnost smjestano u desno dijete
                        Node *rightChild = pq.top();
                        pq.pop();

                        Node *leftChild = pq.top();
                        pq.pop();

                        rightChild->bit = '0'; // Dogovorno desno dijete dobiva 0
                        leftChild->bit = '1';

                        Node *parent = new Node();
                        
                        parent->rightChild = rightChild;
                        parent->leftChild = leftChild;
                        parent->p = leftChild->p + rightChild->p;  // Roditelj ima zbrojenu vjerojatnost

                        pq.push(parent);
                    }

                    this->root = pq.top();

                    this->createCodeMap();
                }

                ~HuffmanTree() {
                    this->deleteRecursive(this->root);
                }

                map<char, string> getCodeMap() {
                    return this->codeMap;
                }
        };

        // Privatne varijable
        string information; // originalna informacija
        string encodedInformation;
        map<char, string> codeMap;  // mapa (rjecnik) kodova za svaki simbol

        // Privatne funkcije
        vector<SymProb> calculateProbabilities() {
            vector<SymProb> sp;
            map<char, int> spMap;

            int n = information.size();

            // Prebroji koliko kojih slova ima u izvornoj informaciji
            // i spremi broj pojavljivanja u mapu (rjecnik), npr. ['a']=666
            for (int i = 0; i < n; i++) {
               char symbol = this->information[i];

                if (spMap.count(symbol) > 0) {
                    spMap[symbol]++;
                }
                else {
                    spMap[symbol] = 1;
                }
            }

            map<char, int>::iterator it;

            // Spremi vjerojatnosti simbola iz mape u vektor i vrati taj vektor
            for (it = spMap.begin(); it != spMap.end(); it++) {
                char symbol = (*it).first;
                int occurences = (*it).second;

                sp.push_back(SymProb(symbol, 1.0*occurences / n));
            }

            return sp;
        }

    public:
        // Javne funkcije
        ECoder() {
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
            vector<SymProb> sp = this->calculateProbabilities();

            HuffmanTree ht = HuffmanTree(sp);

            this->codeMap = ht.getCodeMap();

            int n = this->information.size(); 

            this->encodedInformation = "";

            // Prodji po svakom simbolu informacije i zamijeni ga pripadnim kodom
            for (int i = 0; i < n; i++) {
                char symbol = this->information[i];

                this->encodedInformation += this->codeMap[symbol];
            }
        }

        void writeSymbolCodes(string &file_path) {
            FILE *f = fopen(file_path.c_str(), "w");

            map<char, string>::iterator it;

            // Prodji po simbolima i ispisi ih zajedno sa kodovima u obliku
            // $simbol=$kod, npr. d=001
            for (it = this->codeMap.begin(); it != this->codeMap.end(); it++) {
                char symbol = (*it).first;

                fprintf(f, "%c=%s\n", symbol, this->codeMap[symbol].c_str());
            }

            fclose(f);
        }

        void writeEncodedInformation(string &file_path) {
            FILE *f = fopen(file_path.c_str(), "w");

            fprintf(f, "%s", this->encodedInformation.c_str());

            fclose(f);
        }
};

int main(int argc, char **argv) {
    string source_path = argv[1];
    string ecoderOut_path = argv[2];
    string ecoderCode_path = argv[3];

    ECoder ecoder = ECoder();

    ecoder.readInformation(source_path);
    ecoder.encode();
    ecoder.writeSymbolCodes(ecoderCode_path);
    ecoder.writeEncodedInformation(ecoderOut_path);
    
    return 0;
}
