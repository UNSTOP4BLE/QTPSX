/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "botcess.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//botcess character structure
enum
{
	botcess_ArcMain_Idle0,
	botcess_ArcMain_Idle1,
	botcess_ArcMain_Idle2,
	botcess_ArcMain_Idle3,
	botcess_ArcMain_Idle4,
	botcess_ArcMain_Idle5,
	botcess_ArcMain_Idle6,
	botcess_ArcMain_Idle7,
	botcess_ArcMain_Idle8,
	botcess_ArcMain_Idle9,
	botcess_ArcMain_Left0,
	botcess_ArcMain_Left1,
	botcess_ArcMain_Left2,
	botcess_ArcMain_Down0,
	botcess_ArcMain_Down1,
	botcess_ArcMain_Down2,
	botcess_ArcMain_Up0,
	botcess_ArcMain_Up1,
	botcess_ArcMain_Up2,
	botcess_ArcMain_Right0,
	botcess_ArcMain_Right1,
	botcess_ArcMain_Right2,
	
	botcess_ArcMain_Idleb0,
	botcess_ArcMain_Idleb1,
	botcess_ArcMain_Idleb2,
	botcess_ArcMain_Idleb3,
	botcess_ArcMain_Idleb4,
	botcess_ArcMain_Idleb5,
	botcess_ArcMain_Idleb6,
	botcess_ArcMain_Idleb7,
	botcess_ArcMain_Idleb8,
	botcess_ArcMain_Idleb9,
	botcess_ArcMain_Leftb0,
	botcess_ArcMain_Leftb1,
	botcess_ArcMain_Leftb2,
	botcess_ArcMain_Downb0,
	botcess_ArcMain_Downb1,
	botcess_ArcMain_Downb2,
	botcess_ArcMain_Upb0,
	botcess_ArcMain_Upb1,
	botcess_ArcMain_Upb2,
	botcess_ArcMain_Rightb0,
	botcess_ArcMain_Rightb1,
	botcess_ArcMain_Rightb2,

	botcess_ArcMain_Leftc0,
	botcess_ArcMain_Leftc1,
	botcess_ArcMain_Leftc2,
	botcess_ArcMain_Downc0,
	botcess_ArcMain_Downc1,
	botcess_ArcMain_Downc2,
	botcess_ArcMain_Upc0,
	botcess_ArcMain_Upc1,
	botcess_ArcMain_Upc2,
	botcess_ArcMain_Rightc0,
	botcess_ArcMain_Rightc1,
	botcess_ArcMain_Rightc2,

	botcess_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[botcess_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_botcess;

//botcess character definitions
static const CharFrame char_botcess_frame[] = {
	{botcess_ArcMain_Idle0, {  0,   0, 202, 163}, { 114, 163}}, //0
	{botcess_ArcMain_Idle1, {  0,   0, 217, 161}, { 123, 161}}, //1
	{botcess_ArcMain_Idle2, {  0,   0, 223, 161}, { 124, 161}}, //2
	{botcess_ArcMain_Idle3, {  0,   0, 223, 162}, { 126, 162}}, //3
	{botcess_ArcMain_Idle4, {  0,   0, 214, 162}, { 120, 162}}, //4
	{botcess_ArcMain_Idle5, {  0,   0, 200, 163}, { 106, 163}}, //5
	{botcess_ArcMain_Idle6, {  0,   0, 193, 161}, { 103, 161}}, //6
	{botcess_ArcMain_Idle7, {  0,   0, 188, 163}, { 103, 163}}, //7
	{botcess_ArcMain_Idle8, {  0,   0, 191, 165}, { 103, 165}}, //8
	{botcess_ArcMain_Idle9, {  0,   0, 200, 166}, { 112, 166}}, //9
	
	{botcess_ArcMain_Left0, {  0,   0, 186, 190}, { 141, 190}}, //10
	{botcess_ArcMain_Left1, {  0,   0, 186, 191}, { 141, 191}}, //11
	{botcess_ArcMain_Left2, {  0,   0, 183, 197}, { 141, 197}}, //12
	
	{botcess_ArcMain_Down0, {  0,   0, 178, 147}, { 121, 147}}, //13
	{botcess_ArcMain_Down1, {  0,   0, 182, 146}, { 120, 146}}, //14
	{botcess_ArcMain_Down2, {  0,   0, 192, 144}, { 121, 144}}, //15
	
	{botcess_ArcMain_Up0, {  0,   0, 206, 193}, { 125, 193}}, //16
	{botcess_ArcMain_Up1, {  0,   0, 207, 193}, { 125, 193}}, //17
	{botcess_ArcMain_Up2, {  0,   0, 201, 194}, { 124, 194}}, //18

	{botcess_ArcMain_Right0, {  0,   0, 150, 151}, { 52, 151}}, //19
	{botcess_ArcMain_Right1, {  0,   0, 151, 151}, { 53, 151}}, //20
	{botcess_ArcMain_Right2, {  0,   0, 145, 152}, { 54, 152}}, //21


	{botcess_ArcMain_Idleb0, {  0,   0, 202, 167}, { 127, 167}}, //22
	{botcess_ArcMain_Idleb1, {  0,   0, 217, 165}, { 136, 165}}, //23
	{botcess_ArcMain_Idleb2, {  0,   0, 223, 164}, { 137, 164}}, //24
	{botcess_ArcMain_Idleb3, {  0,   0, 223, 165}, { 139, 165}}, //25
	{botcess_ArcMain_Idleb4, {  0,   0, 214, 165}, { 133, 165}}, //26
	{botcess_ArcMain_Idleb5, {  0,   0, 200, 166}, { 119, 166}}, //27
	{botcess_ArcMain_Idleb6, {  0,   0, 193, 164}, { 116, 164}}, //28
	{botcess_ArcMain_Idleb7, {  0,   0, 188, 166}, { 116, 166}}, //29
	{botcess_ArcMain_Idleb8, {  0,   0, 191, 169}, { 116, 169}}, //30
	{botcess_ArcMain_Idleb9, {  0,   0, 200, 168}, { 125, 168}}, //31
	
	{botcess_ArcMain_Leftb0, {  0,   0, 153, 169}, { 121, 169}}, //32
	{botcess_ArcMain_Leftb1, {  0,   0, 154, 170}, { 122, 170}}, //33
	{botcess_ArcMain_Leftb2, {  0,   0, 155, 172}, { 122, 172}}, //34
	
	{botcess_ArcMain_Downb0, {  0,   0, 192, 148}, { 116, 148}}, //35
	{botcess_ArcMain_Downb1, {  0,   0, 196, 150}, { 119, 150}}, //36
	{botcess_ArcMain_Downb2, {  0,   0, 198, 153}, { 120, 153}}, //37
	
	{botcess_ArcMain_Upb0, {  0,   0, 191, 182}, { 137, 182}}, //38
	{botcess_ArcMain_Upb1, {  0,   0, 191, 179}, { 136, 179}}, //39
	{botcess_ArcMain_Upb2, {  0,   0, 192, 178}, { 136, 178}}, //40

	{botcess_ArcMain_Rightb0, {  0,   0, 187, 159}, { 104, 159}}, //41
	{botcess_ArcMain_Rightb1, {  0,   0, 186, 161}, { 104, 161}}, //42
	{botcess_ArcMain_Rightb2, {  0,   0, 185, 163}, { 103, 163}}, //43

	{botcess_ArcMain_Leftc0, {  0,   0, 186, 190}, { 141, 190}}, //44
	{botcess_ArcMain_Leftc1, {  0,   0, 186, 191}, { 141, 191}}, //45
	{botcess_ArcMain_Leftc2, {  0,   0, 183, 197}, { 141, 197}}, //46
	
	{botcess_ArcMain_Downc0, {  0,   0, 178, 147}, { 121, 147}}, //47
	{botcess_ArcMain_Downc1, {  0,   0, 182, 146}, { 120, 146}}, //48
	{botcess_ArcMain_Downc2, {  0,   0, 192, 144}, { 121, 144}}, //49
	
	{botcess_ArcMain_Upc0, {  0,   0, 206, 196}, { 130, 196}}, //50
	{botcess_ArcMain_Upc1, {  0,   0, 207, 198}, { 131, 198}}, //51
	{botcess_ArcMain_Upc2, {  0,   0, 201, 197}, { 129, 197}}, //52

	{botcess_ArcMain_Rightc0, {  0,   0, 150, 152}, { 51, 152}}, //53
	{botcess_ArcMain_Rightc1, {  0,   0, 151, 152}, { 52, 152}}, //54
	{botcess_ArcMain_Rightc2, {  0,   0, 146, 152}, { 52, 152}}, //55
};

static const Animation char_botcess_anim[CharAnim_Max] = {
    {2, (const u8[]){ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, ASCR_BACK, 0}}, //CharAnim_Idle
	{2, (const u8[]){ 10, 11, 12, ASCR_BACK, 0}},         //CharAnim_Left
	{2, (const u8[]){ 32, 33, 34, 	ASCR_BACK, 0}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 13, 14, 15, ASCR_BACK, 0}},         //CharAnim_Down
	{2, (const u8[]){ 35, 36, 37, 	ASCR_BACK, 0}},   //CharAnim_DownAlt
	{2, (const u8[]){ 16, 17, 18, ASCR_BACK, 0}},         //CharAnim_Up
	{2, (const u8[]){ 38, 39, 40, 	ASCR_BACK, 0}},   //CharAnim_UpAlt
	{2, (const u8[]){ 19, 20, 21, ASCR_BACK, 0}},         //CharAnim_Right
	{2, (const u8[]){ 41, 42, 43, 	ASCR_BACK, 0}},   //CharAnim_RightAlt
	{0, (const u8[]){ ASCR_CHGANI, CharAnim_Idle}},  
	{2, (const u8[]){ 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, ASCR_BACK, 0}}, //CharAnim_Idleb

};

static const Animation char_botcess_anim2[CharAnim_Max] = {
    {2, (const u8[]){ 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, ASCR_BACK, 0}}, //CharAnim_Idle
	{2, (const u8[]){ 44, 45, 46,	ASCR_BACK, 0}},         //CharAnim_Left
	{2, (const u8[]){ 44, 45, 46,	ASCR_BACK, 0}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 13, 14, 15,	ASCR_BACK, 0}},         //CharAnim_Down
	{2, (const u8[]){ 13, 14, 15, 	ASCR_BACK, 0}},   //CharAnim_DownAlt
	{2, (const u8[]){ 50, 51, 52, 	ASCR_BACK, 0}},         //CharAnim_Up
	{2, (const u8[]){ 50, 51, 52, 	ASCR_BACK, 0}},   //CharAnim_UpAlt
	{2, (const u8[]){ 53, 54, 55, 	ASCR_BACK, 0}},         //CharAnim_Right
	{2, (const u8[]){ 53, 54, 55, 	ASCR_BACK, 0}},   //CharAnim_RightAlt
	{0, (const u8[]){ ASCR_CHGANI, CharAnim_Idleb}},  
	{2, (const u8[]){ 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, ASCR_BACK, 0}}, //CharAnim_Idleb

};

//botcess character functions
void Char_botcess_SetFrame(void *user, u8 frame)
{
	Char_botcess *this = (Char_botcess*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_botcess_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_botcess_Tick(Character *character)
{
	Char_botcess *this = (Char_botcess*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	

	//alt anims 2
	if (stage.timercount >= 5736 && stage.timercount <= 7432 || stage.timercount >= 8554 || stage.song_step <= 10)
	Animatable_Animate(&character->animatable2, (void*)this, Char_botcess_SetFrame);
    
	//Animate and draw
	else
	Animatable_Animate(&character->animatable, (void*)this, Char_botcess_SetFrame);

	Character_Draw(character, &this->tex, &char_botcess_frame[this->frame]);
}

void Char_botcess_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Animatable_SetAnim(&character->animatable2, anim);
	Character_CheckStartSing(character);
}

void Char_botcess_Free(Character *character)
{
	Char_botcess *this = (Char_botcess*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_botcess_New(fixed_t x, fixed_t y)
{
	//Allocate botcess object
	Char_botcess *this = Mem_Alloc(sizeof(Char_botcess));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_botcess_New] Failed to allocate botcess object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_botcess_Tick;
	this->character.set_anim = Char_botcess_SetAnim;
	this->character.free = Char_botcess_Free;
	
	Animatable_Init(&this->character.animatable, char_botcess_anim);
	Animatable_Init(&this->character.animatable2, char_botcess_anim2);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 3;
	
	this->character.focus_x = FIXED_DEC(0,1);
	this->character.focus_y = FIXED_DEC(-80,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	//Load art
	this->arc_main = IO_Read("\\CHAR\\BOTCESS.ARC;1");
	
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
		"down0.tim",  
		"down1.tim",  
		"down2.tim",  
		"up0.tim", 
		"up1.tim", 
		"up2.tim", 
		"right0.tim", 
		"right1.tim", 
		"right2.tim", 

		"idleb0.tim", 
		"idleb1.tim", 
		"idleb2.tim", 
		"idleb3.tim", 
		"idleb4.tim", 
		"idleb5.tim", 
		"idleb6.tim", 
		"idleb7.tim", 
		"idleb8.tim", 
		"idleb9.tim", 
		"leftb0.tim", 
		"leftb1.tim", 
		"leftb2.tim", 
		"downb0.tim",  
		"downb1.tim",  
		"downb2.tim",  
		"upb0.tim", 
		"upb1.tim", 
		"upb2.tim",  
		"rightb0.tim", 
		"rightb1.tim", 
		"rightb2.tim", 

		"leftc0.tim", 
		"leftc1.tim", 
		"leftc2.tim", 
		"down0.tim",  
		"down1.tim",  
		"down2.tim",  
		"upc0.tim", 
		"upc1.tim", 
		"upc2.tim", 
		"rightc0.tim", 
		"rightc1.tim", 
		"rightc2.tim", 
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
