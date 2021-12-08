/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "qtmenu.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"
#include "../random.h"
//qtmenu character structure
enum
{
	qtmenu_ArcMain_Idle0,
	qtmenu_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[qtmenu_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_qtmenu;

//qtmenu character definitions
static const CharFrame char_qtmenu_frame[] = {
	{qtmenu_ArcMain_Idle0, {  5,   30, 87, 60}, { 77, 60}},  
	{qtmenu_ArcMain_Idle0, {  102,   30, 87, 60}, { 78, 60}},  
	{qtmenu_ArcMain_Idle0, {  5,   120, 87, 60}, { 77, 60}},  
	{qtmenu_ArcMain_Idle0, {  102,   120, 87, 60}, { 78, 60}},  
};

static const Animation char_qtmenu_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0, 1, 2, 3, ASCR_BACK, 0}}, //CharAnim_Idle
};

//qtmenu character functions
void Char_qtmenu_SetFrame(void *user, u8 frame)
{
	Char_qtmenu *this = (Char_qtmenu*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_qtmenu_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_qtmenu_Tick(Character *character)
{
	Char_qtmenu *this = (Char_qtmenu*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_qtmenu_SetFrame);
	Character_Draw(character, &this->tex, &char_qtmenu_frame[this->frame]);
}

void Char_qtmenu_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_qtmenu_Free(Character *character)
{
	Char_qtmenu *this = (Char_qtmenu*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_qtmenu_New(fixed_t x, fixed_t y)
{
	//Allocate qtmenu object
	Char_qtmenu *this = Mem_Alloc(sizeof(Char_qtmenu));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_qtmenu_New] Failed to allocate qtmenu object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_qtmenu_Tick;
	this->character.set_anim = Char_qtmenu_SetAnim;
	this->character.free = Char_qtmenu_Free;
	
	Animatable_Init(&this->character.animatable, char_qtmenu_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 1;
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-115,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\QTMENU.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //qtmenu_ArcMain_Idle0
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}