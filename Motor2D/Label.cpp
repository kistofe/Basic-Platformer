#include "Label.h"



Label::Label(uint x, uint y) : Widgets(Widgets::UiElemType::LABEL)
{
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
