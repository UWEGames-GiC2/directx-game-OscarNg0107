#pragma once
#include"GameObject2D.h"
#include"ImageGO2D.h"
#include"TextGO2D.h"
#include"DrawData2D.h"

#include<vector>

class mainMenu
{
public:
	mainMenu(std::shared_ptr <DrawData2D> _DD2D);
	~mainMenu();
	void init(int _outputWidth, int _outputHeight);

	std::vector<std::shared_ptr<GameObject2D>> GetGameObject2DList() { return m_GameObjects2D; };

protected:
	std::shared_ptr <DrawData2D>m_DD2D;
	std::vector<std::shared_ptr<GameObject2D>>m_GameObjects2D;

};

