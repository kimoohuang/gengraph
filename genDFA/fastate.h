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
#ifndef _REGEXSTATE_H_
#define _REGEXSTATE_H_

#include <map>
#include <set>
#include <vector>
#include <string>

typedef unsigned int Letter;

class FAState{
private:	
	std::set<FAState*> m_NFAStates;	
public:
	typedef std::vector<FAState*> Table;
	typedef std::set<FAState*>::iterator StateIterator;
	std::multimap<Letter, FAState*> m_Transition;	 // translate fuctiuon
	int m_nStateID;     //state id
	bool m_bAcceptingState; //accept mark 
	bool m_Marked;
	int  m_GroupID;

	FAState() : m_nStateID(-1), m_bAcceptingState(false) {};
	FAState(int nID) : m_nStateID(nID), m_bAcceptingState(false), m_GroupID(0) {};
	FAState(std::set<FAState*> NFAState, int nID);
	FAState(const FAState &other);

	virtual ~FAState();

	void AddTransition(Letter inputCh, FAState *pState);
	void RemoveTransition(FAState* pState);
	void GetTransition(Letter inputCh, Table &States);
	std::set<FAState*>& GetNFAState();

	bool IsDeadEnd();
	FAState& operator=(const FAState& other);
	bool operator==(const FAState& other);

	std::string getStringID();
protected:
};
#endif
