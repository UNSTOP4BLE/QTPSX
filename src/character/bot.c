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
	bot_ArcMain_Idle4,
	bot_ArcMain_Idle5,
	bot_ArcMain_Idle6,
	bot_ArcMain_Idle7,
	bot_ArcMain_Idle8,
	bot_ArcMain_Idle9,
	bot_ArcMain_Left0,
	bot_ArcMain_Left1,
	bot_ArcMain_Left2,
	bot_ArcMain_Left3,
	bot_ArcMain_Down0,
	bot_ArcMain_Down1,
	bot_ArcMain_Down2,
	bot_ArcMain_Down3,
	bot_ArcMain_Up0,
	bot_ArcMain_Up1,
	bot_ArcMain_Up2,
	bot_ArcMain_Up3,
	bot_ArcMain_Right0,
	bot_ArcMain_Right1,
	bot_ArcMain_Right2,
	bot_ArcMain_Right3,
	
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
	{bot_ArcMain_Idle0, {  0,   0, 202, 163}, { 114, 163}}, 
	{bot_ArcMain_Idle1, {  0,   0, 217, 161}, { 123, 161}}, 
	{bot_ArcMain_Idle2, {  0,   0, 223, 161}, { 124, 161}}, 
	{bot_ArcMain_Idle3, {  0,   0, 223, 162}, { 126, 162}}, 
	{bot_ArcMain_Idle4, {  0,   0, 214, 162}, { 120, 162}}, 
	{bot_ArcMain_Idle5, {  0,   0, 200, 163}, { 106, 163}}, 
	{bot_ArcMain_Idle6, {  0,   0, 193, 161}, { 103, 161}}, 
	{bot_ArcMain_Idle7, {  0,   0, 188, 163}, { 103, 163}}, 
	{bot_ArcMain_Idle8, {  0,   0, 191, 165}, { 103, 165}}, 
	{bot_ArcMain_Idle9, {  0,   0, 200, 166}, { 112, 166}}, 
	
	{bot_ArcMain_Left0, {  0,   0, 186, 190}, { 141, 190}}, 
	{bot_ArcMain_Left1, {  0,   0, 186, 191}, { 141, 191}}, 
	{bot_ArcMain_Left2, {  0,   0, 183, 197}, { 141, 197}}, 
	{bot_ArcMain_Left3, {  0,   0, 182, 199}, { 141, 199}},  
	
	{bot_ArcMain_Down0, {  0,   0, 178, 147}, { 121, 147}}, 
	{bot_ArcMain_Down1, {  0,   0, 182, 146}, { 120, 146}}, 
	{bot_ArcMain_Down2, {  0,   0, 192, 144}, { 121, 144}}, 
	{bot_ArcMain_Down3, {  0,   0, 203, 140}, { 127, 140}}, 
	
	{bot_ArcMain_Up0, {  0,   0, 206, 193}, { 125, 193}}, 
	{bot_ArcMain_Up1, {  0,   0, 207, 193}, { 125, 193}}, 
	{bot_ArcMain_Up2, {  0,   0, 201, 194}, { 124, 194}}, 
	{bot_ArcMain_Up3, {  0,   0, 199, 194}, { 121, 194}}, 

	{bot_ArcMain_Right0, {  0,   0, 150, 151}, { 52, 151}}, 
	{bot_ArcMain_Right1, {  0,   0, 151, 151}, { 53, 151}}, 
	{bot_ArcMain_Right2, {  0,   0, 145, 152}, { 54, 152}},   
	{bot_ArcMain_Right3, {  0,   0, 141, 152}, { 54, 152}}, 
};

static const Animation char_bot_anim[CharAnim_Max] = {
    {2, (const u8[]){ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, ASCR_CHGANI, CharAnim_Idle}}, //CharAnim_Idle
	{2, (const u8[]){ 10, 11, 12, 13, ASCR_BACK, 1}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 14, 15, 16, 17, ASCR_BACK, 1}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 18, 19, 20, 21, ASCR_BACK, 1}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){ 22, 23, 24, 25, ASCR_BACK, 1}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
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
		"idle4.tim", 
		"idle5.tim", 
		"idle6.tim", 
		"idle7.tim", 
		"idle8.tim", 
		"idle9.tim", 
		"left0.tim", 
		"left1.tim", 
		"left2.tim", 
		"left3.tim", 
		"down0.tim",  
		"down1.tim",  
		"down2.tim",  
		"down3.tim",  
		"up0.tim", 
		"up1.tim", 
		"up2.tim", 
		"up3.tim",    
		"right0.tim", 
		"right1.tim", 
		"right2.tim", 
		"right3.tim", 
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
