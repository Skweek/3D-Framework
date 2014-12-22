#ifndef _CHAT_BOX_H
#define _CHAT_BOX_H

#include "Utility.h"
#include <vector>
#include <string>

class Quad;
class Text;
class BitmapFont;

class ChatBox
{
public:
	ChatBox();
	~ChatBox();

	void Initialise(glm::vec2 a_pPos, glm::vec2 a_pSize);

	void AddText(const char* a_ccText);

	void RenderChatBox();
	
protected:
	int m_iLineSize;

	BitmapFont* m_pChatFont;

	std::vector<std::string> m_pText;
	Quad* m_pChatBoxBG;

	glm::vec2 m_pPos;
	glm::vec2 m_pSize;

};

#endif //_CHAT_BOX_H