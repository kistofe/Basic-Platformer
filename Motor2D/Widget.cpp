#include "j1App.h"
#include "Widget.h"

Widget::Widget(UiElemType type, iPoint pos, j1Module* callback) : type(type)
{
	this->callback = callback;
	position.create(pos.x, pos.y);
	relative_position.create(0, 0);
}

Widget::~Widget()
{}

void Widget::UpdateAttachedPositions()
{
	p2List_item<Widget*>* iterator = attached_widgets.start;

	while (iterator)
	{
		iterator->data->position = position + iterator->data->relative_position;
		iterator = iterator->next;
	}
}

void Widget::Attach(Widget * widg_to_attach, const iPoint relative_pos)
{
	widg_to_attach->relative_position = relative_pos;
	attached_widgets.add(widg_to_attach);
}

