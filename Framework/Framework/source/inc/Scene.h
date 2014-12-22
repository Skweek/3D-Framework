#ifndef _SCENE_H_
#define _SCENE_H_

#include "Utility.h"
#include <vector>

class RenderableObject;
class Lines;
class Text;

class Scene
{
public:
	static void Create(unsigned int a_uiShader);
	static void Destroy();
	static Scene* Get();

	void AddChild(RenderableObject* a_pObject);
	void AddLine(glm::vec3 a_pStart, glm::vec3 a_pEnd);
	void AddText(Text* a_pText);
	void ClearLines();
	void RemoveChildren();

	void UpdateChildren(float a_fDelta);
	void RenderChildren();

	// Get the depth texture for depth testing in render calls
	unsigned int GetDepth() { return m_uiDepthTexture; }

	// Provide get access to the scene renderer FBO
	unsigned int GetBuffer() { return m_uiDeferredRenderFBO; }

protected:	
	static Scene* m_pInstance;

	Scene(unsigned int a_uiCompShader);
	~Scene();

	void							InitFBO();
	void							InitQuad();
	void							RenderQuad(unsigned int a_uiLightTexture);

	Utility::RenderData*			m_pRenderData;

	std::vector<RenderableObject*>	m_pChildren;
	std::vector<Text*>				m_pText;
	Lines*							m_pLines;

	/* Deferred Rendering */
	/* All scene children will be draw through a geometry pass */
	/* All lights will then be drawn to a light pre pass */
	/* Composite pass will then combine the two */
	unsigned int					m_uiDeferredRenderFBO;

	// Composite shader to draw the final scene
	unsigned int					m_uiShaderProgram;

	// textures for fbo output
	unsigned int					m_uiDiffuseTexture;
	unsigned int					m_uiNormalTexture;
	unsigned int					m_uiPositionTexture;
	unsigned int					m_uiDepthTexture;

	unsigned int					m_uiSpecularTexture;
	unsigned int					m_uiMaterialTexture;

	

};

#endif // _SCENE_H_