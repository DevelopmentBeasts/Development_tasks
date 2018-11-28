#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes

class UiElement
{
public:

	//Constructor
	UiElement(iPoint position);

	~UiElement();

	//Position of the element
	iPoint position;

	//Print the element on the screen
	virtual void Draw() const = 0;

	//CleanUp
	void CleanUp();

};

class UiImage : public UiElement
{
public:

	//Constructor
	UiImage(iPoint position,SDL_Rect section);

	void Draw() const override;

private:

	//Atlas texture
	const SDL_Texture* atlas;

	//Section we want to draw from the atlas
	SDL_Rect section;

	
};

class UiLabel : public UiElement
{
public:

	UiLabel(iPoint position, char* label);

	void Draw() const override;

private:

	//Text we want to print
	p2SString label;

	//Font
	_TTF_Font* font;

};

class UiActiveElement : public UiElement
{
public:

	void Draw() const override;

	virtual void Act() =0;

	void CleanUp();

public:

	bool active;

protected:
	
	//Atlas
	SDL_Texture* atlas;

	//Sections to draw
	SDL_Rect bt_selected;
	SDL_Rect bt_unselected;

	//Pointer to the section we want to print
	SDL_Rect* current_bt=nullptr;

	//Text / title we want to print
	p2SString label;

	//Area in which the mouse can activate the element
	SDL_Rect action_area;

	//Font
	_TTF_Font* font=nullptr;

	
};

class UiCheckBox : public UiActiveElement
{
public:

	void Draw() const override;

	void Act() override;

private:

	//Value we want to switch
	bool* bool_ptr;

};

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions

	const SDL_Texture* GetAtlas() const;

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;
};

#endif // __j1GUI_H__