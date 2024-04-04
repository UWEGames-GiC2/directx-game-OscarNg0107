#include "pch.h"
#include "mainMenu.h"

mainMenu::mainMenu(std::shared_ptr <DrawData2D> _DD2D)
{
	m_DD2D = _DD2D;
}

mainMenu::~mainMenu()
{
}

void mainMenu::init(int _outputWidth, int _outputHeight)
{
	//Menu Init

	std::shared_ptr<TextGO2D> textTitle = std::make_shared<TextGO2D>("Title");
	textTitle->SetScale(5.0f);
	textTitle->SetPos(Vector2(_outputWidth / 2 - XMVectorGetX(m_DD2D->m_Font.get()->MeasureString(textTitle->GetString().data(), true)) * 5 / 2, _outputHeight / 5));
	textTitle->SetColour(Color((float*)&Colors::Black));
	m_GameObjects2D.push_back(textTitle);

	std::shared_ptr<TextGO2D> textStart = std::make_shared<TextGO2D>("Start");
	textStart->SetScale(2.0f);
	textStart->SetPos(Vector2(_outputWidth / 2 - XMVectorGetX(m_DD2D->m_Font.get()->MeasureString(textStart->GetString().data(), true)) * 2 / 2, _outputHeight / 2));
	textStart->SetColour(Color((float*)&Colors::Black));
	m_GameObjects2D.push_back(textStart);

	std::shared_ptr<TextGO2D> textExit = std::make_shared<TextGO2D>("Exit");
	textExit->SetScale(2.0f);
	textExit->SetPos(Vector2(_outputWidth / 2 - XMVectorGetX(m_DD2D->m_Font.get()->MeasureString(textExit->GetString().data(), true)) * 2 / 2, _outputHeight / 2 - -XMVectorGetX(m_DD2D->m_Font.get()->MeasureString(textStart->GetString().data(), true))));
	textExit->SetColour(Color((float*)&Colors::Black));
	m_GameObjects2D.push_back(textExit);
}

