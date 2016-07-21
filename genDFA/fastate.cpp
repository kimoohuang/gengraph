#include <sstream>
#include "fastate.h"

FAState::~FAState() {
	m_NFAStates.clear();
	m_Transition.clear();
}

FAState::FAState(std::set<FAState*> NFAState, int nID) {
	m_NFAStates = NFAState;
	m_nStateID = nID;
	m_bAcceptingState = false;
	m_GroupID = 0;
	StateIterator iter;
	for(iter = NFAState.begin(); iter != NFAState.end(); ++iter) {
		if((*iter)->m_bAcceptingState) {
			m_bAcceptingState = true;
		}
	}
}
FAState::FAState(const FAState &other){ 
	*this = other;
}

void FAState::AddTransition(Letter inputCh, FAState *pState){
	m_Transition.insert(std::make_pair(inputCh, pState));
}

void FAState::RemoveTransition(FAState* pState) {
	std::multimap<Letter, FAState*>::iterator iter;
	for(iter = m_Transition.begin(); iter != m_Transition.end();) {
		FAState *toState = iter->second;
		if(toState == pState)
			m_Transition.erase(iter++);
		else 
			++iter;
	}
}

void FAState::GetTransition(Letter inputCh, Table &States) {
	States.clear();
	std::multimap<Letter, FAState*>::iterator iter;
	for(iter = m_Transition.lower_bound(inputCh); iter != m_Transition.upper_bound(inputCh); ++iter) {
		FAState *pState = iter->second;
		States.push_back(pState);
	}
    if(inputCh != 1160755946 && inputCh != 1893067709){
    iter = m_Transition.find(1893067709);
    if(iter  != m_Transition.end()){
        FAState *pState = iter ->second;
        States.push_back(pState);
    }
    }
}

std::set<FAState*>& FAState::GetNFAState() { 
	return m_NFAStates; 
}

bool FAState::IsDeadEnd() {
	if(m_bAcceptingState)
		return false;
	if(m_Transition.empty())
		return true;
	std::multimap<Letter, FAState*>::iterator iter;
	for(iter=m_Transition.begin(); iter!=m_Transition.end(); ++iter){
		FAState *toState = iter->second;
		if(toState != this)
			return false;
	}
	return true;
}	

FAState& FAState::operator=(const FAState& other) { 
	this->m_Transition	= other.m_Transition; 
	this->m_nStateID	= other.m_nStateID;
	this->m_NFAStates	= other.m_NFAStates;
	return *this;
}

bool FAState::operator==(const FAState& other) {
	if(m_NFAStates.empty())
		return(m_nStateID == other.m_nStateID);
	else 
		return(m_NFAStates == other.m_NFAStates);
}

std::string FAState::getStringID(){
	std::string result;
	std::stringstream out;
	//if(m_bAcceptingState){
	//	out << "{" << m_nStateID << "}";
	//} else {
		out << m_nStateID;
	//}
	return out.str();
}



