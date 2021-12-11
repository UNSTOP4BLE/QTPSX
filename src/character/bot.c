/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "bot.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//bot character structure
enum
{
	bot_ArcMain_Idle0,
	bot_ArcMain_Idle1,
	bot_ArcMain_Idle2,
	bot_ArcMain_Idle3,
	bot_ArcMain_Left0,
	bot_ArcMain_Left1,
	bot_ArcMain_Down0,
	bot_ArcMain_Down1,
	bot_ArcMain_Up0,
	bot_ArcMain_Up1,
	bot_ArcMain_Right0,
	bot_ArcMain_Right1,
	
	bot_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[bot_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_bot;

//bot character definitions
static const CharFrame char_bot_frame[] = {
	{bot_ArcMain_Idle0, {  18,   89, 202, 163}, { 114, 163}}, 
	{bot_ArcMain_Idle1, {  10,   88, 223, 161}, { 124, 161}}, 
	{bot_ArcMain_Idle2, {  8,    93, 214, 162}, { 120, 162}},  
	{bot_ArcMain_Idle3, {  38,   86, 193, 161}, { 103, 161}}, 
	
	{bot_ArcMain_Left0, {  17,   62, 186, 190}, { 144, 190}},  
	{bot_ArcMain_Left1, {  39,   61, 186, 191}, { 141, 191}},  
	
	{bot_ArcMain_Down0, {  32,   106, 178, 147}, { 121, 147}},  
	{bot_ArcMain_Down1, {  14,   106, 182, 146}, { 120, 146}},  

	
	{bot_ArcMain_Up0, {  18,   58, 206, 193}, { 123, 193}},  
	{bot_ArcMain_Up1, {  13,   60, 207, 193}, { 124, 193}},   

	{bot_ArcMain_Right0, {  55,   103, 150, 151}, { 52, 151}},  
	{bot_ArcMain_Right1, {  61,   100, 151, 151}, { 53, 151}},   
};

static const Animation char_bot_anim[CharAnim_Max] = {
    {1, (const u8[]){ 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, ASCR_CHGANI, CharAnim_Idle}}, //CharAnim_Idle
	{2, (const u8[]){ 4, 5, ASCR_BACK, 1}},         //CharAnim_Left
	{1, (const u8[]){ 4, ASCR_CHGANI, CharAnim_LeftAlt}}, //CharAnim_LeftAlt
	{2, (const u8[]){ 6, 7, ASCR_BACK, 1}},         //CharAnim_Down
	{1, (const u8[]){ 4, ASCR_CHGANI, CharAnim_DownAlt}},   //CharAnim_DownAlt
	{2, (const u8[]){ 8, 9, ASCR_BACK, 1}},         //CharAnim_Up
	{1, (const u8[]){ 4, ASCR_CHGANI, CharAnim_UpAlt}},   //CharAnim_UpAlt
	{2, (const u8[]){ 10, 11, ASCR_BACK, 1}},         //CharAnim_Right
	{1, (const u8[]){ 4, ASCR_CHGANI, CharAnim_RightAlt}},   //CharAnim_RightAlt
};

//bot character functions
void Char_bot_SetFrame(void *user, u8 frame)
{
	Char_bot *this = (Char_bot*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_bot_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_bot_Tick(Character *character)
{
	Char_bot *this = (Char_bot*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_bot_SetFrame);
	Character_Draw(character, &this->tex, &char_bot_frame[this->frame]);
}

void Char_bot_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_bot_Free(Character *character)
{
	Char_bot *this = (Char_bot*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_bot_New(fixed_t x, fixed_t y)
{
	//Allocate bot object
	Char_bot *this = Mem_Alloc(sizeof(Char_bot));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_bot_New] Failed to allocate bot object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_bot_Tick;
	this->character.set_anim = Char_bot_SetAnim;
	this->character.free = Char_bot_Free;
	
	Animatable_Init(&this->character.animatable, char_bot_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 2;
	
	this->character.focus_x = FIXED_DEC(0,1);
	this->character.focus_y = FIXED_DEC(-80,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\BOT.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", 
		"idle1.tim", 
		"idle2.tim", 
		"idle3.tim", 
		"left0.tim", 
		"left1.tim", 
		"down0.tim",  
		"down1.tim",   
		"up0.tim", 
		"up1.tim", 
		"right0.tim", 
		"right1.tim", 
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
