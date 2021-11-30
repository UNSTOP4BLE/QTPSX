/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "qtm.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"
#include "../random.h"
//qtm character structure
enum
{
	qtm_ArcMain_Idle0,
	qtm_ArcMain_Idle1,
	qtm_ArcMain_Left,
	qtm_ArcMain_Down,
	qtm_ArcMain_Up,
	qtm_ArcMain_Right,
	qtm_ArcMain_Leftb,
	qtm_ArcMain_Downb,
	qtm_ArcMain_Upb,
	qtm_ArcMain_Rightb,
	
	qtm_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[qtm_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_qtm;

//qtm character definitions
static const CharFrame char_qtm_frame[] = {
	{qtm_ArcMain_Idle0, {  1,   12, 99, 107}, { 52, 107}},  
	{qtm_ArcMain_Idle0, {  107,   7, 101, 110}, { 53, 110}},  
	{qtm_ArcMain_Idle0, {  3,   137, 102, 112}, { 55, 112}},  
	{qtm_ArcMain_Idle0, {  114,   137, 101, 112}, { 55, 112}},  
	{qtm_ArcMain_Idle1, {  1,   18, 98, 114}, { 52, 114}},  
	{qtm_ArcMain_Idle1, {  118,   17, 97, 114}, { 51, 114}},  
	{qtm_ArcMain_Idle1, {  3,   137, 96, 114}, { 51, 114}},  
	{qtm_ArcMain_Idle1, {  110,   135, 95, 114}, { 49, 114}},  
	
	{qtm_ArcMain_Left, {  8,   3, 67, 119}, { 35, 119}},  
	{qtm_ArcMain_Left, {  105,   3, 66, 119}, { 35, 119}},  
	{qtm_ArcMain_Left, {  1,   134, 69, 119}, { 37, 119}},  
	{qtm_ArcMain_Left, {  120,   138, 69, 118}, { 36, 118}},  
	
	{qtm_ArcMain_Down, {  0,   11, 117, 106}, { 54, 106}},  
	{qtm_ArcMain_Down, {  123,   9, 117, 108}, { 55, 108}},  
	{qtm_ArcMain_Down, {  1,   143, 115, 112}, { 54, 112}},  
	{qtm_ArcMain_Down, {  122,   140, 113, 113}, { 53, 113}},  
	
	{qtm_ArcMain_Up, {  4,   7, 110, 117}, { 60, 117}},  
	{qtm_ArcMain_Up, {  118,   4, 107, 117}, { 58, 117}},  
	{qtm_ArcMain_Up, {  9,   136, 102, 115}, { 54, 115}},  
	{qtm_ArcMain_Up, {  124,   137, 100, 115}, { 53, 115}},  

	{qtm_ArcMain_Right, {  0,   1, 112, 111}, { 51, 111}},  
	{qtm_ArcMain_Right, {  119,   3, 112, 111}, { 52, 111}},  
	{qtm_ArcMain_Right, {  2,   130, 102, 109}, { 47, 109}},  
	{qtm_ArcMain_Right, {  130,   129, 99, 110}, { 45, 110}},  

	{qtm_ArcMain_Leftb, {  7,   3, 71, 120}, { 40, 120}},
	{qtm_ArcMain_Leftb, {  100,   1, 73, 120}, { 42, 120}},
	{qtm_ArcMain_Leftb, {  3,   133, 68, 120}, { 36, 120}},
	{qtm_ArcMain_Leftb, {  119,   136, 70, 120}, { 37, 120}},
	
	{qtm_ArcMain_Downb, {  1,   6, 116, 112}, { 54, 112}},  
	{qtm_ArcMain_Downb, {  123,   4, 126, 113}, { 55, 113}},  
	{qtm_ArcMain_Downb, {  1,   138, 115, 117}, { 54, 117}},  
	{qtm_ArcMain_Downb, {  122,   135, 113, 118}, { 53, 118}},  
	
	{qtm_ArcMain_Upb, {  5,   5, 109, 119}, { 59, 119}},  
	{qtm_ArcMain_Upb, {  119,   3, 109, 120}, { 59, 120}},  
	{qtm_ArcMain_Upb, {  13,   134, 102, 118}, { 54, 118}},  
	{qtm_ArcMain_Upb, {  125,   133, 100, 119}, { 53, 119}},  

	{qtm_ArcMain_Rightb, {  2,   2, 112, 113}, { 51, 113}},  
	{qtm_ArcMain_Rightb, {  121,   5, 111, 113}, { 51, 113}},  
	{qtm_ArcMain_Rightb, {  6,   142, 102, 112}, { 48, 112}},  
	{qtm_ArcMain_Rightb, {  127,   140, 99, 112}, { 46, 112}},  
};

static const Animation char_qtm_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0, 1, 2, 3, 4, 5, 6, 7, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 8, 9, 10, 11, ASCR_BACK, 1}},         //CharAnim_Left
	{0, (const u8[]){ 24, 25, 26, 27, ASCR_BACK, 1}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 12, 13, 14, 15, ASCR_BACK, 1}},         //CharAnim_Down
	{0, (const u8[]){ 28, 29, 30, 31, ASCR_BACK, 1}},   //CharAnim_DownAlt
	{2, (const u8[]){ 16, 17, 18, 19, ASCR_BACK, 1}},         //CharAnim_Up
	{0, (const u8[]){ 32, 33, 34, 35, ASCR_BACK, 1}},   //CharAnim_UpAlt
	{2, (const u8[]){ 20, 21, 22, 23, ASCR_BACK, 1}},         //CharAnim_Right
	{0, (const u8[]){ 36, 37, 38, 39, ASCR_BACK, 1}}   //CharAnim_RightAlt
};

//qtm character functions
void Char_qtm_SetFrame(void *user, u8 frame)
{
	Char_qtm *this = (Char_qtm*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_qtm_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_qtm_Tick(Character *character)
{
	Char_qtm *this = (Char_qtm*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_qtm_SetFrame);
	Character_Draw(character, &this->tex, &char_qtm_frame[this->frame]);
}

void Char_qtm_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_qtm_Free(Character *character)
{
	Char_qtm *this = (Char_qtm*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_qtm_New(fixed_t x, fixed_t y)
{
	//Allocate qtm object
	Char_qtm *this = Mem_Alloc(sizeof(Char_qtm));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_qtm_New] Failed to allocate qtm object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_qtm_Tick;
	this->character.set_anim = Char_qtm_SetAnim;
	this->character.free = Char_qtm_Free;
	
	Animatable_Init(&this->character.animatable, char_qtm_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 1;
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-115,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\QTM.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //qtm_ArcMain_Idle0
		"idle1.tim", //qtm_ArcMain_Idle1
		"left.tim",  //qtm_ArcMain_Left
		"down.tim",  //qtm_ArcMain_Down0
		"up.tim",    //qtm_ArcMain_Up
		"right.tim", //qtm_ArcMain_Right
		"leftb.tim",  //qtm_ArcMain_Leftb
		"downb.tim",  //qtm_ArcMain_Down1
		"upb.tim",    //qtm_ArcMain_Upb
		"rightb.tim", //qtm_ArcMain_Rightb
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
