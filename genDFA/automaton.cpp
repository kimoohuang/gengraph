/** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
 *  This file is part of regex library
 *  Copyright (C) 2011 Avinash Dongre ( dongre.avinash@gmail.com )
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 * 
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **/
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <sstream>
#include "automaton.h"


AutoMaton::AutoMaton(){
}

AutoMaton::AutoMaton(std::fstream &f_nfa, int num){
    string line;
    for(int i = 0; i < num; i++)
        m_NFATable.push_back(new FAState(i));
    
    while(getline(f_nfa, line)){
        if(line.find("#")!= line.npos)
            ;
        else if(line.find("doublecircle")!= line.npos){
            size_t p1 = line.find("N");
            size_t p2 = line.find(" [shape");
            m_NFATable[atoi(line.substr(p1+1,p2-p1-1).c_str())]->m_bAcceptingState = true; 
        }else if(line.find("->") != line.npos){
            size_t p1 = line.find("N");
            size_t p2 = line.find("-> N");
            size_t p3 = line.find("[");
            int left = atoi(line.substr(p1+1,p2-p1-2).c_str());
            int right= atoi(line.substr(p2+4,p3-p2-5).c_str());
            size_t p4 = line.find("\"");
            size_t p5 = line.find("]");
            unsigned int label = (unsigned int)atol(line.substr(p4+1, p5-p4-3).c_str());
            if(label != 1160755946 && label != 2146952987)  // epsilon and sigma
                m_InputSet.insert(label);
            m_NFATable[left]->m_Transition.insert(std::multimap<Letter, FAState*>::value_type(label, m_NFATable[right]));
        }
    }
}

AutoMaton::~AutoMaton(){
    CleanUp();
}

void AutoMaton::CleanUp(){
    for(int i = 0; i < (int)m_NFATable.size(); ++i) {
        delete m_NFATable[i];
    }
    m_NFATable.clear();

    for(int i = 0; i < (int)m_DFATable.size(); ++i) {
        delete m_DFATable[i];
    }
    m_DFATable.clear();

    m_nNextStateID = 0;

    m_InputSet.clear();
}

void AutoMaton::PrintTable(Table &table) {
	std::string TableString;
	for(int i = 0; i < (int)table.size(); ++i) {
		FAState *pState = table[i];
		if(pState->m_bAcceptingState) {
			TableString += "\t" + pState->getStringID() + "\t\n";
		}
	}
	TableString += "\n";
	for( int i = 0; i < (int)table.size(); ++i) {
		FAState *pState = table[i];
		std::vector<FAState*> State;
		pState->GetTransition(1160755946, State); //get epsilon transition
		for(int j = 0; j < (int)State.size(); ++j) {
			TableString += "\t" + pState->getStringID() + " -> " + State[j]->getStringID();
			TableString += "\t[label = \"epsilon \"]\n";
		}
		pState->GetTransition(2146952987, State); //get sigma transition
		for(int j = 0; j < (int)State.size(); ++j) {
			TableString += "\t" + pState->getStringID() + " -> " + State[j]->getStringID();
			TableString += "\t[label = \"sigma \"]\n";
		}
		std::set<Letter>::iterator iter;
		for(iter = m_InputSet.begin(); iter != m_InputSet.end(); ++iter) {
			pState->GetTransition(*iter, State);
			for(int j = 0; j < (int)State.size(); ++j) {
				TableString += "\t" + pState->getStringID() + " -> " + State[j]->getStringID();
				std::stringstream out;
				out << *iter;
				TableString += "\t[label = \"" + out.str() + " \"]\n";
			}
		}
	}
	std::cout << TableString;
}
      
void AutoMaton::PrintNTable() {
    PrintTable(m_NFATable);
}

void AutoMaton::PrintDTable() {
    PrintTable(m_DFATable);
}

void AutoMaton::EpsilonClosure(std::set<FAState*> startSet, std::set<FAState*> &result) {
	std::stack<FAState*> UnVisitedStates;	
	result.clear();	
	result = startSet; 
	StateIterator iter;
	for(iter = startSet.begin(); iter != startSet.end(); ++iter) {
		UnVisitedStates.push(*iter);
	}

	while(!UnVisitedStates.empty()) {
		FAState* curState = UnVisitedStates.top();
		UnVisitedStates.pop();

		Table epsilonStates;
		curState->GetTransition(1160755946, epsilonStates);

		TableIterator epsilonItr;

		for ( epsilonItr = epsilonStates.begin(); epsilonItr != epsilonStates.end(); ++epsilonItr ){
			if(result.find((*epsilonItr)) == result.end()) {
				result.insert((*epsilonItr));
				UnVisitedStates.push((*epsilonItr));
			}
		}
	}
}

void AutoMaton::Move(Letter chInput, std::set<FAState*> NFAState, std::set<FAState*> &Result) {

	Result.clear();
	StateIterator iter;
	for(iter = NFAState.begin(); iter != NFAState.end(); ++iter) {
		Table States;
		(*iter)->GetTransition(chInput, States);
		for(int index = 0; index < (int)States.size(); ++index){
			Result.insert(States[index]);
		}
	}
}

void AutoMaton::ConvertNFAtoDFA() {
	for(int i = 0; i < (int)m_DFATable.size(); ++i)
		delete m_DFATable[i];
	m_DFATable.clear();

	if(m_NFATable.size() == 0)
		return;
	
	std::set<FAState*> NFAStartStateSet;
	NFAStartStateSet.insert(m_NFATable[0]);

	std::set<FAState*> DFAStartStateSet;
	EpsilonClosure(NFAStartStateSet, DFAStartStateSet);

	m_nNextStateID = 0;
	FAState *DFAStartState = new FAState(DFAStartStateSet, m_nNextStateID++);

	m_DFATable.push_back(DFAStartState);

	Table UnVisitedStates;
	UnVisitedStates.push_back(DFAStartState);
	while(!UnVisitedStates.empty()) {

		FAState* CurDFAState = UnVisitedStates[UnVisitedStates.size()-1];
		UnVisitedStates.pop_back();

		std::set<Letter>::iterator iter;
		for(iter = m_InputSet.begin(); iter != m_InputSet.end(); ++iter) {
			std::set<FAState*> MoveRes, EpsilonClosureRes;

			Move(*iter, CurDFAState->GetNFAState(), MoveRes);
			EpsilonClosure(MoveRes, EpsilonClosureRes);

			//StateIterator MoveResItr;
			//StateIterator EpsilonClosureResItr;

			bool bFound = false;
			FAState *s   = NULL;
			for(int i = 0; i < (int)m_DFATable.size(); ++i) {
				s = m_DFATable[i];
				if(s->GetNFAState() == EpsilonClosureRes) {
					bFound = true;
					break;
				}
			}
			if(!bFound) {
				FAState* U = new FAState(EpsilonClosureRes, m_nNextStateID++);
				UnVisitedStates.push_back(U);
				m_DFATable.push_back(U);                                
				CurDFAState->AddTransition(*iter, U);
			} else {
				CurDFAState->AddTransition(*iter, s);
			}
		} // for inputSet
	}	
	ReduceDFA();
}

void AutoMaton::ReduceDFA() {
	std::set<FAState*> DeadEndSet;
	for(int i = 0; i < (int)m_DFATable.size(); ++i) {
		if(m_DFATable[i]->IsDeadEnd()) {
			DeadEndSet.insert(m_DFATable[i]);
		}
	}
	if(DeadEndSet.empty())
		return;
	StateIterator iter;
	for(iter = DeadEndSet.begin(); iter != DeadEndSet.end(); ++iter) {
		for(int i = 0; i < (int)m_DFATable.size(); ++i)
			m_DFATable[i]->RemoveTransition(*iter);

		TableIterator pos;
		for(pos = m_DFATable.begin(); pos != m_DFATable.end(); ++pos) {
			if(*pos == *iter) {
				break;
			}
		}
		m_DFATable.erase(pos);
		delete *iter;
	}
}

void AutoMaton::MinimizeDFA () {
	TableIterator pos;
    FAState* First = m_DFATable[0];
    std::multimap<Letter, FAState*>::iterator iter;
	for(pos = m_DFATable.begin(); pos != m_DFATable.end(); ++pos) {
        for(iter = (*pos)->m_Transition.begin(); iter != (*pos)->m_Transition.end(); ){
            if(iter->second == First)
                (*pos)->m_Transition.erase(iter++);
            else
                ++iter;
        }
    }
}

