#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "j1Fonts.h"
#include "j1App.h"

#define CURSOR_WIDTH 2


enum class UiType {

	LABEL, 
	IMAGE, 
	BUTTON, 
	CHECKBOX, 
	INTEXT,
	WINDOW,
	NONE
};

class UiElement
{
public:

	//Constructor
	UiElement(iPoint position);

	//Print the element on the screen
	virtual void Draw() {}

	//CleanUp
	void CleanUp() {}

public:

	//Position of the element
	iPoint position;

	//Type of ui element
	UiType type;

	//Indicates if an element is visible and we can interact with it
	bool active;
};

class UiImage : public UiElement
{
public:

	//Constructor
	UiImage(iPoint position,SDL_Rect section);

	void Draw();

	void CleanUp();
	
	

private:

	//Atlas texture
	SDL_Texture* atlas;

	//Section we want to draw from the atlas
	SDL_Rect section;

};

class UiLabel : public UiElement
{
public:

	//Constructors
	UiLabel(iPoint position, char* label, _TTF_Font* font = App->font->default);

	void Draw();

private:

	//Text we want to print
	p2SString text;

	//Font
	_TTF_Font* font;

};

class UiActiveElement : public UiElement
{
public:

	UiActiveElement(iPoint position);

	void Start() {}

	void Update();

	void Draw();

	void Act() {}

	void CleanUp() {}

	bool MouseOnTop() const;

public:

	bool active;

protected:

	//Atlas
	SDL_Texture* atlas;

	//Sections to draw
	SDL_Rect section_idle;
	SDL_Rect section_selected;

	//Pointer to the section we want to print
	SDL_Rect* current_section =nullptr;

	//Area in which the mouse can activate the element
	SDL_Rect action_area;

	
	
};

class UiCheckBox : public UiActiveElement
{
public:

	void Draw();

	void Act() ;

private:

	//Value we want to switch
	bool* bool_ptr;

};

class UiButton: public UiElement
{

public:

	

	//Ui label or Ui image, depending on what
	//   !!!CANT ADD AN ACTIVE ELEMENT OR A WINDOW!!!!
	UiElement* son_element;
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
	UiImage*	AddImage(iPoint position, SDL_Rect section);
	UiLabel*	AddLabel(iPoint position, char* label, _TTF_Font* font = App->font->default);

	//Call Draw() function of all the UiElements
	void DrawUi() const;

	SDL_Texture* GetAtlas() const;

private:

	//List with all the UI elements
	p2List<UiElement*> ui_elements;
	
	SDL_Texture* atlas;
	p2SString atlas_file_name;
};

#endif // __j1GUI_H__