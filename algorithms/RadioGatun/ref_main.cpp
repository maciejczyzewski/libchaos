/* RadioGatún reference code
 * Public domain
 * For more information on RadioGatún, please refer to 
 * http://radiogatun.noekeon.org/
*/
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <sstream>
#include <vector>
#include "radiogatun.h"

using namespace std;
using namespace radiogatun;

static vector<string> commandList;
static bool waitOnCommand = false;

void usage(string exeName)
{
	exeName = string(exeName,exeName.find_last_of("/\\:")+1);
	cerr << "Usage: " << endl
		 << "  " << exeName << " command-name [command-name ...]]" << endl
		 << endl
		 << "  command-name:" << endl
		 << "     performance" << endl
		 << "     produce-testvectors" << endl
		 << "     verify-testvectors" << endl
		 << endl;
}

void parseParameters(int argc, char ** argv)
{
    for(int i=1; i<argc; i++)
		commandList.push_back(argv[i]);
}

template<class W>
void produceTestVectorResults(istream& in, ostream& out)
{
    while(!in.eof()) {
        string input;
        getline(in, input);
        if ((!in.eof()) && (!(input.substr(0, 1) == "#"))) {
            RadioGatun<W> rg;
            string output, output_hex;
            rg.Hash(input, output);
	        output_hex = stringtohex_BE(output);
	        out << "# RadioGatun[" << sizeof(W)*8 << "](\"" << input << "\") = "
                << output_hex << "\n";
	        out << input << "\n";
	        out << output_hex << endl;
        }
    }
}

void produceTestVectors()
{
    {
        cout << "Producing 'RG32-testvectors' from 'RG-inputstrings'" << endl;
        cout << "===================================================" << endl;
        ifstream fin("RG-inputstrings");
        if (!fin) {
            cerr << "File 'RG-inputstrings' not found." << endl;
        }
        else {
            ofstream fout("RG32-testvectors");
            produceTestVectorResults<UINT32>(fin, fout);
            cout << "OK" << endl;
        }
        cout << endl;
    }
    {
        cout << "Producing 'RG64-testvectors' from 'RG-inputstrings'" << endl;
        cout << "===================================================" << endl;
        ifstream fin("RG-inputstrings");
        if (!fin) {
            cerr << "File 'RG-inputstrings' not found." << endl;
        }
        else {
            ofstream fout("RG64-testvectors");
            produceTestVectorResults<UINT64>(fin, fout);
            cout << "OK" << endl;
        }
        cout << endl;
    }
}

template<class W>
bool verifyTestVectors(istream& in, int& count)
{
    bool OK = true;
    count = 0;
    while(!in.eof()) {
        string input;
        getline(in, input);
        if ((!in.eof()) && (!(input.substr(0, 1) == "#"))) {
            RadioGatun<W> rg;
            string output, output_hex;
            string output_hash;
            getline(in, output_hex);
            output = hextostring_BE(output_hex);
            rg.Hash(input, output_hash);
            if (output_hash != output) {
                OK = false;
    	        cout << "!!! RadioGatun[" << sizeof(W)*8 << "](\"" << input << "\") != "
                    << output_hex << "\n";
            }
            count++;
        }
    }
    return OK;
}

void verifyTestVectors()
{
    {
        cout << "Verifying RadioGatun[32] using 'RG32-testvectors'" << endl;
        cout << "=================================================" << endl;
        int count = 0;
        ifstream fin("RG32-testvectors");
        if (!fin) {
            cerr << "File 'RG32-testvectors' not found." << endl;
        }
        else {
            if (verifyTestVectors<UINT32>(fin, count)) {
                cout << count << " test vectors verified for RadioGatun[32] - all OK" << endl;
            }
            else {
                cout << count << " test vectors verified for RadioGatun[32] BUT SOME FAILED!!!" << endl;
            }
        }
        cout << endl;
    }
    {
        cout << "Verifying RadioGatun[64] using 'RG64-testvectors'" << endl;
        cout << "=================================================" << endl;
        int count = 0;
        ifstream fin("RG64-testvectors");
        if (!fin) {
            cerr << "File 'RG64-testvectors' not found." << endl;
        }
        else {
            if (verifyTestVectors<UINT64>(fin, count)) {
                cout << count << " test vectors verified for RadioGatun[64] - all OK" << endl;
            }
            else {
                cout << count << " test vectors verified for RadioGatun[64] BUT SOME FAILED!!!" << endl;
            }
        }
        cout << endl;
    }
}

template<class W>
void testPerformancesTemplate()
{
    const int trials = 3;
    const unsigned int length13Blocks = 600000;
    const unsigned int lengthBlocks = length13Blocks*13;
    const unsigned int lengthWords = lengthBlocks*3;
    const unsigned int lengthBytes = lengthWords*sizeof(W);
    W *message = new W[lengthWords];
    for(unsigned int i=0; i<lengthWords; i++)
        message[i] = 123+456*i+789*i*i;

    RadioGatun<W> rg;
    clock_t ticks1, ticks2;

    cout << "Benchmarking FastIterate() for RadioGatun[" << sizeof(W)*8 << "]" << endl;
	cout << "=============================================" << endl;
    cout << "Message length is " << lengthBlocks << " blocks (" << lengthBytes << " bytes)" << endl;

    for(int i=1; i<=trials; i++) {
        cout << "  Trial " << i << "/" << trials << endl;
        ticks1 = clock();
        rg.FastIterate(message, length13Blocks);
        ticks2 = clock();

    	double seconds = (double)(ticks2-ticks1) / (double)CLOCKS_PER_SEC;

    	cout << "    Time: " << seconds << " seconds" << endl;
    	cout << "    Performance: " << (lengthBytes/1048576.0/seconds) << " MB/sec" << endl;
    }
    cout << endl;
}

void testPerformances()
{
    if (!isLittleEndian()) {
        cout << "FastIterate assumes that the platform is little endian!" << endl;
    }
    else {
        testPerformancesTemplate<UINT32>();
        testPerformancesTemplate<UINT64>();
    }
}

//----------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
//----------------------------------------------------------------------------------------------------------------------
{
    parseParameters(argc, argv);
    if(commandList.empty())
        usage(argv[0]);
    else {
    	for(vector<string>::iterator it=commandList.begin(); it<commandList.end(); ++it) {
            if(*it=="produce-testvectors") produceTestVectors();
            else if(*it=="verify-testvectors") verifyTestVectors();
            else if(*it=="performance") testPerformances();
		    else cerr << "Unknown command " << *it << endl;
        }
		if(waitOnCommand) pause();
	}
	return EXIT_SUCCESS;
}
