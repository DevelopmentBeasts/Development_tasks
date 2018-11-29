#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	AddImage({ 350,50 }, { 485, 829, 328, 103 });
	AddLabel({350, 20},"Hey kiddos, don't forget to pray to Ric before you go to sleep");

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{

	DrawUi();

	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------
UiImage* j1Gui::AddImage(iPoint position, SDL_Rect section)
{
	UiImage* item;
	item = new UiImage(position, section);

	ui_elements.add(item);

	return item;
}

UiLabel* j1Gui::AddLabel(iPoint position, char* label, _TTF_Font* font)
{
	UiLabel* item;
	item = new UiLabel(position, label, font);

	ui_elements.add(item);

	return item;
}

void j1Gui::DrawUi() const
{
	
	p2List_item<UiElement*>* item;

	if (ui_elements.count() > 0)
	{
		for (item = ui_elements.start; item != nullptr; item = item->next)
		{
			item->data->Draw();
		}
	}
}

//UI Element
UiElement::UiElement(iPoint position):position(position)
{}


//=====================================================================================================================
//UI Image
UiImage::UiImage(iPoint position, SDL_Rect section) : UiElement(position), section(section)
{
	atlas = App->gui->GetAtlas();
}

void UiImage::Draw()
{
	App->render->Blit(atlas, position.x, position.y, &section, 0.0f);
}

void UiImage::CleanUp()
{
	atlas = nullptr;
}


//=====================================================================================================================
//UI label
UiLabel::UiLabel(iPoint position, char* label, _TTF_Font* font):UiElement(position), font(font)
{
	text = label;
}


void UiLabel::Draw()
{
	SDL_Texture* texture = App->font->Print(text.GetString(), { 255,255,255,255 }, font);
	
	App->render->Blit(texture, position.x, position.y, 0, 0.0f);

	App->tex->UnLoad(texture);
	texture = nullptr;
}

//=====================================================================================================================
//Ui active element
UiActiveElement::UiActiveElement(iPoint position, char* title):UiElement(position)
{
	if (title != nullptr)
		label = title;

	bt_idle		= { 0,0,0,0 };
	bt_selected = { 0,0,0,0 };

}

void UiActiveElement::Update()
{



}

bool UiActiveElement::MouseOnTop() const
{
	bool ret = false;

	iPoint mouse_position;
	App->input->GetMousePosition(mouse_position.x, mouse_position.y);

	if (mouse_position.x > action_area.x &&
		mouse_position.x < action_area.x + action_area.w &&
		mouse_position.y > action_area.y &&
		mouse_position.y < action_area.y + action_area.h)
		ret = true;
	

	return ret;
}