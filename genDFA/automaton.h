#ifndef _REG_EX_H_
#define _REG_EX_H_

#include <stack>
#include <set>
#include <list>
#include <string>
#include <vector>
#include <map>
#include <fstream>


#include "fastate.h"
using namespace std;


class AutoMaton{
public:
	AutoMaton();
	AutoMaton(std::fstream &e_nfa_dot, int num_node);
	~AutoMaton();

	typedef std::vector<FAState*> Table;   // store the automaton
	typedef Table::reverse_iterator TableReverseIterator;
	typedef Table::iterator TableIterator; 
	typedef std::set<FAState*>::iterator StateIterator;   //store the transltation

	void PrintNTable();
	void PrintDTable();
	void ConvertNFAtoDFA();

	void MinimizeDFA ();

private:
	Table m_NFATable;  // a dfa
	Table m_DFATable; // a dfa

	//std::stack<Table >       m_CharacterClassStack;
	//std::stack<char>         m_ExpressionStack;
	std::set<Letter>         m_InputSet;
	int                      m_nNextStateID;

	void EpsilonClosure(std::set<FAState*> T, std::set<FAState*> &Res);
	void Move(Letter chInput, std::set<FAState*> T, std::set<FAState*> &Res);
	void ReduceDFA();
	void CleanUp();


	void PrintTable(Table &table);

protected:
};
#endif
