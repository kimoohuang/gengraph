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
