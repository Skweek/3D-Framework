#include "ChatBox.h"
#include "Renderable\Quad.h"
#include "Renderable\Text.h"

#include "ShaderManager.h"
#include "ResourceManager.h"
#include "Scene.h"

// Some consts
const float cfFontScale = 0.4f;

ChatBox::ChatBox() {}

ChatBox::~ChatBox() 
{
	delete m_pChatBoxBG;

	m_pText.clear();
}

void ChatBox::Initialise(glm::vec2 a_pPos, glm::vec2 a_pSize)
{
	m_pChatBoxBG = new Quad(a_pPos, a_pSize, ShaderManager::Get()->GetShader("Simple"));

	m_iLineSize = (int)((a_pSize.y - 20.f) / 2.5f);

	m_pChatFont = ResourceManager::Get()->LoadFont("../fonts/tnr/tnr.fnt");

	m_pSize = a_pSize;
	m_pPos = a_pPos;
}

void ChatBox::AddText(const char* a_ccText)
{
	// convert our text to a string to make it easier to manipulate
	std::string sText = a_ccText;

	// search for new line character '\n'
	unsigned int loc = sText.find("\n");
	while(loc != std::string::npos)
	{
		// get the line we need to process
		std::string sCurLine = sText.substr(0, loc);

		// Cut it from the original text so we don't process that
		sText = sText.substr(loc+2);

		// is the string too long to fit in the chat box?
		int len = sCurLine.length();
		int iNumLines = (len / m_iLineSize) + 1;

		for(int i = 0; i < iNumLines; ++i)
		{
			std::string ps = sCurLine.substr(i*m_iLineSize, m_iLineSize);
			m_pText.push_back(ps);
		}

		// find the next newline
		loc = sText.find("\n");
	}

	// No new lines? Just sort through the rest of the text and make sure it will fit on each line
	int len = sText.length();
	int iNumLines = (len / m_iLineSize) + 1;

	for(int i = 0; i < iNumLines; ++i)
	{
		std::string ps = sText.substr(i*m_iLineSize, m_iLineSize);
		m_pText.push_back(ps);
	}
}

void ChatBox::RenderChatBox()
{
	Scene::Get()->AddChild(m_pChatBoxBG);

	int start = 0;
	if(m_pText.size() > 10) start = m_pText.size() - 10;

	for(int i = start; i < (int)m_pText.size(); ++i)
	{
		Scene::Get()->AddChild(new Text(m_pText[i].c_str(), glm::vec4(m_pPos.x + 5.f, (m_pPos.y + 5) + (i * 12), 0, 1), glm::vec4(1), ShaderManager::Get()->GetShader("Font"), m_pChatFont, 0.5f));
	}
}