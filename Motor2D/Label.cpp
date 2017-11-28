#include "Label.h"
#include "j1App.h"
#include "j1Fonts.h"


Label::Label(uint x, uint y, j1Module* callback) : Widgets(Widgets::UiElemType::LABEL)
{
	this->callback = callback;
}


Label::~Label()
{
}

bool Label::Start()
{
	return true;
}

bool Label::PreUpdate()
{
	return true;
}

bool Label::Update()
{
	return true;
}

bool Label::PostUpdate()
{
	return true;
}

bool Label::CleanUp()
{
	return true;
}

void Label::Draw()
{
}
