#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "Player.h"
#include <math.h>
#include "j1Input.h"

//Destructor

PlayerClass::~PlayerClass() {

}
PlayerClass::PlayerClass() {   //DO PUSHBACKS WITH XML
	this->name = 'a';
	current_animation = NULL;

	// Idle animations
	idle_left.PushBack({ 19, 7 ,45-19, 37-7 });  
	idle_left.PushBack({ 81, 9, 26, 27 });
	idle_left.PushBack({ 143, 7 , 26, 29 });    //DONE
	idle_left.PushBack({ 206, 7, 23, 28 });
	
	idle_left.loop = true;
	idle_left.speed = 0.12f;

	

	// Runnig animations
	

	run_left.PushBack({ 271, 15 , 27, 23 });
	run_left.PushBack({ 23, 57, 27, 23 });
	run_left.PushBack({ 85, 57, 29, 24 });  //DONE
	run_left.PushBack({ 146, 57, 31, 23 });
	
	run_left.loop = true;
	run_left.speed = 0.12f;

	// Jump animations
	jump_right.PushBack({ 2, 158, 20, 25 });
	jump_right.PushBack({ 34, 158, 20, 25 });
	jump_right.loop = true;
	jump_right.speed = 0.25f;

	fall_right.PushBack({ 87, 157, 22, 26 });
	fall_right.PushBack({ 119, 157, 22, 26 });
	fall_right.loop = true;
	fall_right.speed = 0.25f;

	jump_left.PushBack({ 272, 158, 20, 25 });
	jump_left.PushBack({ 240, 158, 20, 25 });
	jump_left.loop = true;
	jump_left.speed = 0.25f;

	fall_left.PushBack({ 185, 157, 22, 26 });
	fall_left.PushBack({ 153, 157, 22, 26 });
	fall_left.loop = true;
	fall_left.speed = 0.25f;

	// Attack animations
	attack_right.PushBack({ 1, 272, 29, 27 });
	attack_right.PushBack({ 64, 272, 29, 27 });
	attack_right.PushBack({ 135, 272, 35, 27 });
	attack_right.PushBack({ 198, 272, 40, 27 });
	attack_right.PushBack({ 253, 272, 42, 27 });
	attack_right.loop = false;
	attack_right.speed = 0.25f;

	attack_right.PushBack({ 269, 244, 29, 27 });
	attack_right.PushBack({ 206, 244, 29, 27 });
	attack_right.PushBack({ 129, 244, 35, 27 });
	attack_right.PushBack({ 61, 244, 40, 27 });
	attack_right.PushBack({ 5, 244, 42, 27 });
	attack_left.loop = false;
	attack_left.speed = 0.25f;
}
bool PlayerClass::Start() {
	
	bool ret = true;
	pugi::xml_parse_result result = PlayerStartFile.load_file("StartPlayerConfig.xml");
	if (result == NULL) {
		LOG("Could not load StartPlayerConfig.xml. pugi error: %s", result.description());
		//ret = false;
	}
	if (ret == true) {
		//Load all Player starter info
		LOG("Loading player info at the start of the game");
	}
	PlayerXmlNode = PlayerStartFile.child("config").child("player");
	if (PlayerXmlNode == NULL) {
		LOG("PlayerXmlNode is not reading correctly");
	}
	//Asigment of the values
	uint type1 = PlayerXmlNode.attribute("type").as_uint();
	switch (type1) {
	case 0:
		data.type = FIRE_WISP;
	case 1: 
		data.type = WATER_WISP;
	case 2: 
		data.type = ROCK_WISP;
	case 3:
		data.type = WIND_WISP;
	}
	/*data.xpos = PlayerXmlNode.attribute("xpos").as_uint();
	data.ypos = PlayerXmlNode.attribute("ypos").as_uint();*/
	/*data.yvel = PlayerXmlNode.attribute("yvel").as_uint();*/
	/*data.xvel = PlayerXmlNode.attribute("xvel").as_uint(1);*/
	data.xpos = 700;
	data.ypos = 500;
	data.yvel = 0.0;
	data.xvel = 5.0;


	playerrect.w = 60;//rect used for the player
	playerrect.h = 60;

	
	StaminaRect.w = 13;
	StaminaRect.h = 40;

	StaminaRect.x = 30;
	StaminaRect.y = 700;
	//___________________________________________________________________________
	LOG("Resseting anims");
	idle_right.Reset();
	idle_left.Reset();
	run_right.Reset();
	run_left.Reset();
	jump_right.Reset();
	jump_left.Reset();
	fall_right.Reset();
	fall_left.Reset();
	attack_right.Reset();
	attack_left.Reset();

	LOG("LOADING PLAYER TEXTURES");

	Textures = App->tex->Load("textures/Fire_Wisp/fireSheet.png");
	
	current_animation = &idle_right;
	return ret;
}


bool PlayerClass::Update(float dt) {

	MovePlayer();
	PlayerAnims();
	//SDL_RenderCopyEx();
	



	

	return true;

}





void PlayerClass::MovePlayer() {
	
	//by default you are not moving (usefull bools for anim system)
	movingleft = false;
	movingright = false;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		movingleft = false;
		movingright = true;
		automatic_left = false;
		if (!automatic_right) {
			data.xpos += data.xvel;
		}
		if (jumping) {
			automatic_right = true; //automatic left or right is a bool that allows you to stop pressing "D" in the air without stoping the x movement of the character.
		}
	}
	if (automatic_right) {
		data.xpos += (data.xvel + 1.5); // a little boost of the speed in the air to make the jump more interesting in a plataformer game
	}
	//__________________
	
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) { // the same as "D"
		movingleft = true;
		movingright = false;
		automatic_right = false;
		if (!automatic_left) {
			data.xpos -= data.xvel;
		}
		if (jumping) {
			automatic_left = true;
		}
	}
	if (automatic_left) {
		data.xpos -= (data.xvel + 1.5);
	}

	if (data.ypos == yposaux) {  //if the y position touches the ground stops the automatic left and right of the jump
		automatic_right = false;
		automatic_left = false;
	}
	//_________________

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		
		if (data.yvel > -3 && jumping) { // if you are in the top of your jump you can press "S" to fall down doing a smash in the ground
			if (StaminaRect.w >= 121) { 
				StaminaRect.w -= 120; // the stamina bar have 300p and every time you press "S" in the air you use 120p
				if (jumping) {
					fall_atack = true;
					jumping = false;
					automatic_left = false;  
					automatic_right = false;

				}
			}
		}
	}
	//____________________
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {

		if (jumping == false) { //this is here because we don't want the player to press w in the air and jump again if it is not allowed
			//data.yvel = 1.0;
			bot_reached = false;
			top_reached = false;
			jumping = true;
			data.yvel = 10;  //initial y speed
			yposaux = data.ypos;
		}
	}
	if (jumping) {
		data.ypos -= data.yvel; //this makes the y speed decrease and become negative to turn back in the top ending up falling down to the ground
		data.yvel -= 0.3;

		if (data.ypos >= yposaux) {
			data.ypos = yposaux;
			jumping = false;
		}
	}
	if (!jumping) {
		data.yvel = 0.0;
		if (StaminaRect.w <= 300) {  // here the stamina rect grows its points if you don't jump bc you are "in rest"
			StaminaRect.w += 1;
		}
	}
	if (fall_atack && !jumping) { // a big boost of the speed when you want to fall down with a smash to make it more impressive
		data.yvel = 25.0;
		data.ypos += data.yvel;
		data.yvel += 2;
		if (data.ypos >= yposaux) {
			fall_atack = false;
			data.ypos = yposaux;
		}
	}
	//_____________

	playerrect.x = data.xpos;  //here we put the SDL_Rect where we print the sprites in the player position.
	playerrect.y = data.ypos;
	

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		StaminaRect.y -= 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		StaminaRect.y += 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		StaminaRect.x -= 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		StaminaRect.x += 2;
	}
}
void PlayerClass::PlayerAnims() {

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		SCANCODE_D = true;
		SCANCODE_A = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		SCANCODE_A = true;
		SCANCODE_D = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		SCANCODE_W = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		SCANCODE_S = true;
	}
	

	//LAST DIRECTION RECORD (usefull to continue looking the same side when we are idl and we run or jump
	
	if (SCANCODE_D == true ) {  //RIGHT 
		LastDirectionLeft = false;
		LastDirectionRight = true;	
	}
	if (SCANCODE_A == true) {
		LastDirectionLeft = true;
		LastDirectionRight = false;
	}

	//ANIMS BASED ON LAST DIRECTION RECORD


	// JUMP UP STRAIGHT (going up)

	if (jumping  && (data.yvel > 0) && !movingleft && !movingright && !automatic_left && !automatic_right) {  //looking left
		current_animation = &run_left;

		CurrentAnimationRect = current_animation->GetCurrentFrame();

		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect, &playerrect, 90.0, NULL, SDL_FLIP_NONE);

		App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &playerrect, NULL, 0.0, 1, 1, 1.0);
	}

	// JUMP UP STRAIGHT (going down)

	if (jumping && (data.yvel < 0) && !movingleft && !movingright && !automatic_left && !automatic_right) {  //looking left
		current_animation = &run_left;

		CurrentAnimationRect = current_animation->GetCurrentFrame();

		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect, &playerrect, 90, NULL, SDL_FLIP_HORIZONTAL);

		App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &playerrect, NULL, 0.0, 1, 1, 1.0);
	}



	//GOING DOWN AS ATACK

	if (!jumping && (data.yvel < 0) && !movingleft && !movingright && !automatic_left && !automatic_right) { //looking the left
		current_animation = &run_left;
		CurrentAnimationRect = current_animation->GetCurrentFrame();

		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect, &playerrect, 90, NULL, SDL_FLIP_HORIZONTAL);

		App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &playerrect, NULL, 0.0, 1, 1, 1.0);
	}


	//JUMP LEFT
	if (jumping && (data.yvel > 0) && LastDirectionLeft  && automatic_left) {  //JUMPING UP + LEFT
		current_animation = &run_left;

		CurrentAnimationRect = current_animation->GetCurrentFrame();

		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect, &playerrect, data.yvel*4, NULL, SDL_FLIP_NONE);

		App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &playerrect, NULL, 0.0, 1, 1, 1.0);
	}
	if (jumping && (data.yvel < 0) && LastDirectionLeft && automatic_left) {  //JUMPING DOWN + LEFT
		current_animation = &run_left;

		CurrentAnimationRect = current_animation->GetCurrentFrame();

		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect, &playerrect, data.yvel * 4, NULL, SDL_FLIP_NONE);

		App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &playerrect, NULL, 0.0, 1, 1, 1.0);
	}

	//MOVE LEFT 
	if (!jumping && movingleft) {  //NOT  JUMPING + MOVING LEFT
		current_animation = &run_left;
		
		CurrentAnimationRect = current_animation->GetCurrentFrame();

		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect, &playerrect, NULL, NULL, SDL_FLIP_NONE);

		App->render->Blit(Textures, (int)data.xpos, (int)data.ypos,&playerrect, NULL, 0.0, 1, 1, 1.0);
	}


	//MOVE RIGHT 
	if (!jumping && movingright) {  //NOT  JUMPING + MOVING LEFT
		current_animation = &run_left;
		
		CurrentAnimationRect = current_animation->GetCurrentFrame();
		
		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect,&playerrect , NULL, NULL, SDL_FLIP_HORIZONTAL);

		App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &playerrect, NULL, data.yvel*4.0, 1, 1, 1.0);
	}
	// JUMP RIGHT

	if (jumping && (data.yvel > 0) && LastDirectionRight && automatic_right) {  //JUMPING UP + LEFT
		current_animation = &run_left;

		CurrentAnimationRect = current_animation->GetCurrentFrame();

		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect, &playerrect, data.yvel * (-4), NULL, SDL_FLIP_HORIZONTAL);

		App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &playerrect, NULL, 0.0, 1, 1, 1.0);
	}
	if (jumping && (data.yvel < 0) && LastDirectionRight && automatic_right) {  //JUMPING DOWN + LEFT
		current_animation = &run_left;

		CurrentAnimationRect = current_animation->GetCurrentFrame();

		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect, &playerrect, data.yvel * (-4), NULL, SDL_FLIP_HORIZONTAL);

		App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &playerrect, NULL, 0.0, 1, 1, 1.0);
	}

	// IDEL LEFT
	if (LastDirectionLeft && !jumping && !movingleft) { 
		current_animation = &idle_left;

		CurrentAnimationRect = current_animation->GetCurrentFrame();

		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect, &playerrect, data.yvel * (-4), NULL, SDL_FLIP_NONE);

		App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &playerrect, NULL, 0.0, 1, 1, 1.0);
	}

	//IDEL RIGHT
	if (LastDirectionRight && !jumping && !movingright) {   
		current_animation = &idle_left;

		CurrentAnimationRect = current_animation->GetCurrentFrame();

		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect, &playerrect, data.yvel * (-4), NULL, SDL_FLIP_HORIZONTAL);

		App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &playerrect, NULL, 0.0, 1, 1, 1.0);
	}






	//App->render->DrawQuad(playerrect, 0, 255, 0, 100);

	App->render->DrawQuad(StaminaRect, 0, 0, 255, 100);
}
