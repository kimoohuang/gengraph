#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string>

#include "automaton.h"

using namespace std;


int main(int argc, char * argv[])
{
    if(argc != 3){
        cout <<"Usage:"<<endl;
        cout <<"    nfatodfa NFA_FILE dfa_FILE" << endl;
        cout <<"    NFA_FILE : input, the NFA_FILE" <<endl;
        cout <<"    DFA_FILE : output, the DFA_FILE" <<endl;
    }

    fstream f_nfa(argv[1], ios_base::in);
    fstream f_dfa(argv[2], ios_base::out);

    // get the num of nodes in nfa
    string line; 
    string numofnodes; 
    while(getline(f_nfa, line))
        numofnodes = line;
    size_t pos = numofnodes.find("#");
    int numNfaNodes = atoi(numofnodes.substr(pos+1).c_str());
    //cout << numNfaNodes;
    //
    f_nfa.close();
    f_nfa.open(argv[1], ios_base::in);
/*
    while(getline(f_nfa, line)){
        if(line.find("#")!= line.npos)
            continue;
        else if(line.find("doublecircle")!= line.npos){
            size_t p1 = line.find("N");
            size_t p2 = line.find(" [shape");
    //        m_NFATable[atoi(line.substr(p1+1,p2-p1-1).c_str())]->m_bAcceptingState = true; 
        }else if(line.find("->") != line.npos){
            size_t p1 = line.find("N");
            size_t p2 = line.find("-> N");
            size_t p3 = line.find("[");
            int left = atoi(line.substr(p1+1,p2-p1-2).c_str());
            cout << left << "--";
            int right= atoi(line.substr(p2+4,p3-p2-5).c_str());
            cout << right <<endl;
            size_t p4 = line.find("\"");
            size_t p5 = line.find("]");
            unsigned int label = (unsigned int)atol(line.substr(p4+1, p5-p4-3).c_str());
            cout << label <<endl;

        }
    }
*/
    AutoMaton* am = new AutoMaton(f_nfa, numNfaNodes);
    am->PrintNTable();
    am->ConvertNFAtoDFA();
    am->PrintDTable();
    am->MinimizeDFA();
    am->PrintDTable();

}


