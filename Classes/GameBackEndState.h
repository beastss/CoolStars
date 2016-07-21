#ifndef __BACKENDSTATE_H__
#define __BACKENDSTATE_H__
class GameBackEndState
{
public:
	static GameBackEndState *theModel();
	void setBusinessMode(bool isBusinessMode){ m_isBusinessMode = isBusinessMode; }
	bool isBusinessMode(){ return m_isBusinessMode; }
private:
	GameBackEndState();
private:
	bool m_isBusinessMode;
};
#endif