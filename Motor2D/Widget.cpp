#include "j1App.h"
#include "Widget.h"
#include "j1Input.h"
#include "j1Render.h"
#include "p2Log.h"

Widget::Widget(UiElemType type, iPoint pos, j1Module* callback) : type(type)
{
	this->callback = callback;
	position.create(pos.x, pos.y);
	relative_position.create(0, 0);
	being_clicked = false;
	attached = false;
	last_mousepos = { 0, 0 };
}

Widget::~Widget()
{}

void Widget::Drag()
{
	world_area.x = position.x;
	world_area.y = position.y;

	p2List_item<Widget*>* iterator = attached_widgets.end;

	while (iterator)
	{
		if (iterator->data->being_clicked)
			return;
		iterator = iterator->prev;
	}

	// Check if the element is being clicked or not
	SDL_Point temp_mousepos_sdl;
	App->input->GetMousePosition(temp_mousepos_sdl.x, temp_mousepos_sdl.y);
	temp_mousepos_sdl.x = App->render->ScreenToWorld(temp_mousepos_sdl.x, temp_mousepos_sdl.y).x;
	temp_mousepos_sdl.y = App->render->ScreenToWorld(temp_mousepos_sdl.x, temp_mousepos_sdl.y).y;
	if (!being_clicked && SDL_PointInRect(&temp_mousepos_sdl, &world_area) && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		being_clicked = true, LOG("Clicked = TRUE");
	if (being_clicked && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		being_clicked = false, LOG("Clicked = FALSE");
	iPoint temp_mousepos;
	temp_mousepos.create(temp_mousepos_sdl.x, temp_mousepos_sdl.y);

	// Move element if it's clicked and if last mouse position is different from the new one
	if (!attached && being_clicked && last_mousepos != temp_mousepos && last_mousepos.x != 0 && last_mousepos.y != 0 && draggable)
	{
		position += temp_mousepos - last_mousepos;
	}
	if (attached && being_clicked && last_mousepos != temp_mousepos && last_mousepos.x != 0 && last_mousepos.y != 0 && draggable)
	{
		relative_position += temp_mousepos - last_mousepos;
	}
	if (being_clicked)
	last_mousepos = temp_mousepos;
}

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
	widg_to_attach->attached = true;
	attached_widgets.add(widg_to_attach);
}

