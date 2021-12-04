/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "dad.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Dad character structure
enum
{
	Dad_ArcMain_Idle0,
	Dad_ArcMain_Idle1,
	Dad_ArcMain_Left,
	Dad_ArcMain_Down,
	Dad_ArcMain_Up,
	Dad_ArcMain_Right,
	
	Dad_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Dad_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Dad;

//Dad character definitions
static const CharFrame char_dad_frame[] = {
	{Dad_ArcMain_Idle0, {  1,   12, 99, 107}, { 52, 107}},  
	{Dad_ArcMain_Idle0, {  107,   7, 101, 110}, { 54, 110}}, 
	{Dad_ArcMain_Idle0, {  2,   137, 102, 112}, { 55, 112}}, 
	{Dad_ArcMain_Idle0, {  112,   137, 102, 112}, { 55, 112}},  
	{Dad_ArcMain_Idle1, {  1,   18, 98, 114}, { 52, 114}}, 
	{Dad_ArcMain_Idle1, {  118,   17, 96, 113}, { 50, 113}}, 
	{Dad_ArcMain_Idle1, {  2,   137, 96, 113}, { 50, 113}},  
	{Dad_ArcMain_Idle1, {  110,   135, 94, 113}, { 49, 113}},  
	
	{Dad_ArcMain_Left, {  8,   3, 66, 119}, { 35, 119}},  
	{Dad_ArcMain_Left, {  105,   3, 66, 119}, { 35, 119}}, 
	{Dad_ArcMain_Left, {  1,   134, 69, 119}, { 37, 119}}, 
	{Dad_ArcMain_Left, {  120,   137, 69, 119}, { 36, 119}}, 
	
	{Dad_ArcMain_Down, {  0,   10, 117, 107}, { 54, 107}}, 
	{Dad_ArcMain_Down, {  123,   8, 117, 109}, { 55, 109}}, 
	{Dad_ArcMain_Down, {  1,   142, 115, 113}, { 54, 113}}, 
	{Dad_ArcMain_Down, {  122,   139, 113, 114}, { 53, 114}}, 
	
	{Dad_ArcMain_Up, {  2,   6, 110, 117}, { 60, 117}}, 
	{Dad_ArcMain_Up, {  118,   4, 107, 117}, { 58, 117}}, 
	{Dad_ArcMain_Up, {  8,   136, 102, 115}, { 54, 115}}, 
	{Dad_ArcMain_Up, {  125,   135, 100, 116}, { 53, 116}}, 

	{Dad_ArcMain_Right, {  1,   1, 112, 111}, { 51, 111}}, 
	{Dad_ArcMain_Right, {  119,   3, 112, 111}, { 52, 111}}, 
	{Dad_ArcMain_Right, {  1,   129, 102, 110}, { 48, 110}}, 
	{Dad_ArcMain_Right, {  129,   129, 100, 110}, { 47, 110}}, 
};

static const Animation char_dad_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0, 1, 2, 3, 4, 5, 6, 7, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 8, 9, 10, 11, ASCR_BACK, 1}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 12, 13, 14, 15, ASCR_BACK, 1}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 16, 17, 18, 19, ASCR_BACK, 1}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){ 20, 21, 22, 23, ASCR_BACK, 1}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//Dad character functions
void Char_Dad_SetFrame(void *user, u8 frame)
{
	Char_Dad *this = (Char_Dad*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_dad_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Dad_Tick(Character *character)
{
	Char_Dad *this = (Char_Dad*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Dad_SetFrame);
	Character_Draw(character, &this->tex, &char_dad_frame[this->frame]);
}

void Char_Dad_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Dad_Free(Character *character)
{
	Char_Dad *this = (Char_Dad*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Dad_New(fixed_t x, fixed_t y)
{
	//Allocate dad object
	Char_Dad *this = Mem_Alloc(sizeof(Char_Dad));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Dad_New] Failed to allocate dad object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Dad_Tick;
	this->character.set_anim = Char_Dad_SetAnim;
	this->character.free = Char_Dad_Free;
	
	Animatable_Init(&this->character.animatable, char_dad_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 1;
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-115,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\DAD.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //Dad_ArcMain_Idle0
		"idle1.tim", //Dad_ArcMain_Idle1
		"left.tim",  //Dad_ArcMain_Left
		"down.tim",  //Dad_ArcMain_Down
		"up.tim",    //Dad_ArcMain_Up
		"right.tim", //Dad_ArcMain_Right
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
