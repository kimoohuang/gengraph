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
