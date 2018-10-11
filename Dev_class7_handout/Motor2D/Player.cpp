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
	idle_left.speed = 0.2f;

	idle_right.PushBack({ 163, 214, 22, 25 });
	idle_right.PushBack({ 130, 214, 24, 25 });
	idle_right.PushBack({ 98, 214, 24, 25 });
	idle_right.PushBack({ 66, 214, 24, 25 });
	
	idle_right.loop = true;
	idle_right.speed = 0.07f;

	// Runnig animations
	run_right.PushBack({ 1, 96, 23, 25 });
	run_right.PushBack({ 32, 99, 25, 25 });
	run_right.PushBack({ 65, 99, 23, 25 });
	run_right.PushBack({ 100, 98, 20, 25 });
	run_right.PushBack({ 133, 97, 20, 25 });
	run_right.PushBack({ 165, 99, 17, 25 });
	run_right.PushBack({ 199, 99, 18, 25 });
	run_right.PushBack({ 228, 98, 20, 25 });
	run_right.loop = true;
	run_right.speed = 0.07f;

	run_left.PushBack({ 271, 15 , 27, 23 });
	run_left.PushBack({ 23, 57, 27, 23 });
	run_left.PushBack({ 85, 57, 29, 24 });  //DONE
	run_left.PushBack({ 146, 57, 31, 23 });
	
	run_left.loop = true;
	run_left.speed = 0.2f;

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


	player.w = 20;//rect used for the player
	player.h = 20;

	
	StaminaRect.w = 10;
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
	Texturesflipped = App->tex->Load("textures/Fire_Wisp/fireSheet2.png");
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
			automatic_right = true;
		}
	}
	if (automatic_right) {
		data.xpos += (data.xvel + 1.5);
	}
	//__________________
	
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
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

	if (data.ypos == yposaux) {
		automatic_right = false;
		automatic_left = false;
	}
	//_________________

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		
		if (data.yvel > -3 && jumping) {
			if (StaminaRect.w >= 121) {
				StaminaRect.w -= 120;
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

		if (jumping == false) {
			//data.yvel = 1.0;
			bot_reached = false;
			top_reached = false;
			jumping = true;
			data.yvel = 10;
			yposaux = data.ypos;
		}
	}
	if (jumping) {
		data.ypos -= data.yvel;
		data.yvel -= 0.3;

		if (data.ypos >= yposaux) {
			data.ypos = yposaux;
			jumping = false;
		}
	}
	if (!jumping) {
		data.yvel = 0.0;
		if (StaminaRect.w <= 300) {
			StaminaRect.w += 1;
		}
	}
	if (fall_atack && !jumping) {
		data.yvel = 25.0;
		data.ypos += data.yvel;
		data.yvel += 2;
		if (data.ypos >= yposaux) {
			fall_atack = false;
			data.ypos = yposaux;
		}
	}
	//_____________

	player.x = data.xpos;
	player.y = data.ypos;
	

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
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		SCANCODE_A = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		SCANCODE_W = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		SCANCODE_S = true;
	}
	/*SCANCODE_A = false;
	SCANCODE_W = false;
	SCANCODE_D = false;
	SCANCODE_S = false;
*/

	//LAST DIRECTION RECORD
	
	if (SCANCODE_D == true /*&& jumping*/) {  //RIGHT 
		LastDirectionLeft = false;
		LastDirectionRight = true;	
	}
	if (SCANCODE_A == true) {
		LastDirectionLeft = true;
		LastDirectionRight = false;
	}

	//ANIMS BASED ON LAST DIRECTION RECORD


	//JUMP LEFT
	if (jumping && (data.yvel > 0) && LastDirectionLeft ) {  //JUMPING UP + LEFT
		current_animation = &run_left;
		player = current_animation->GetCurrentFrame();

		App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &player, NULL, data.yvel*4.0, 1, 1, 1.0);
	}
	if (jumping && (data.yvel < 0) && LastDirectionLeft) {  //JUMPING DOWN + LEFT
		current_animation = &run_left;
		player = current_animation->GetCurrentFrame();

		App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &player, NULL, data.yvel*4.0, 1, 1, 1.0);
	}

	//MOVE LEFT 
	if (!jumping && movingleft) {  //NOT  JUMPING + MOVING LEFT
		current_animation = &run_left;
		player = current_animation->GetCurrentFrame();

		App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &player, NULL, data.yvel*4.0, 1, 1, 1.0);
	}


	//JUMP RIGHT
	if (jumping && (data.yvel > 0) && LastDirectionRight) {  //JUMPING UP + LEFT
		current_animation = &run_left;
		player = current_animation->GetCurrentFrame();

		App->render->Blit(Texturesflipped, (int)data.xpos, (int)data.ypos, &player, NULL, data.yvel*4.0, 1, 1, 1.0);
	}
	if (jumping && (data.yvel < 0) && LastDirectionRight) {  //JUMPING DOWN + LEFT
		current_animation = &run_left;
		player = current_animation->GetCurrentFrame();

		App->render->Blit(Texturesflipped, (int)data.xpos, (int)data.ypos, &player, NULL, data.yvel*4.0, 1, 1, 1.0);
	}

	//MOVE LEFT 
	if (!jumping && movingleft) {  //NOT  JUMPING + MOVING LEFT
		current_animation = &run_left;
		player = current_animation->GetCurrentFrame();

		App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &player, NULL, data.yvel*4.0, 1, 1, 1.0);
	}
	


	if (LastDirectionLeft && !jumping && !movingleft) {
		current_animation = &idle_left;
		player = current_animation->GetCurrentFrame();

		App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &player, NULL, 0.0, 1, 1, 1.0);
	}






	App->render->DrawQuad(player, 0, 255, 0, 100);

	App->render->DrawQuad(StaminaRect, 0, 0, 255, 100);
}
