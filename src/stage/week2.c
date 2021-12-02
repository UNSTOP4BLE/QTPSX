/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "week2.h"

#include "../mem.h"
#include "../archive.h"
#include "../animation.h"

//Week 2 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	IO_Data arc_tv0, arc_tv0_ptr[1];
	IO_Data arc_smoke, arc_smoke_ptr[1];

	Gfx_Tex tex_back0; //Background
	Gfx_Tex tex_error; //Background
	Gfx_Tex tex_bsod; //Background

	//tv0 state
	Gfx_Tex tex_tv0;
	u8 tv0_frame, tv0_tex_id;

	//smoke state
	Gfx_Tex tex_smoke;
	u8 smoke_frame, smoke_tex_id;
	
	Animatable tv0_animatable;
	Animatable tv0a2_animatable;
	Animatable smoke_animatable;
} Back_Week2;

//tv0 animation and rects
static const CharFrame tv0_frame[10] = {
	//idle tv
	{0, {  0,   0,  47,  39}, { 48,  40}}, //left 
	{0, { 47,   0,  98,  52}, { 91,  52}}, //right
	//red eye tv thing
	{0, {  0,  52,  47,  39}, { 47,  39}}, //left 
	{0, { 47,  52,  98,  52}, { 91,  52}}, //right
	//warning left
	{0, {0,   104,  47, 40}, { 47,  39}}, //left 1 
	{0, {0,   155,  47, 39}, { 47,  39}}, //left 2
	{0, {0,   207,  48, 39}, { 47,  39}}, //left 3 (turned off)
	//warning
	{0, {47, 104,  98, 51}, { 91,  52}}, //right 1
	{0, {47, 155,  98, 51}, { 91,  52}}, //right 2
	{0, {47, 206,  98, 50}, { 91,  52}}, //right 3 (turned off)
};
static const Animation tv0_anim[1] = {

	//idle tv
	{2, (const u8[]){0, 2, ASCR_BACK, 0}}, //Left
};

static const Animation tv0a2_anim[1] = {

	//idle tv
	{2, (const u8[]){1, 3, ASCR_BACK, 0}}, //Left
};


//tv0 functions
void Week2_tv0_SetFrame(void *user, u8 frame)
{
	Back_Week2 *this = (Back_Week2*)user;
	
	//Check if this is a new frame
	if (frame != this->tv0_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &tv0_frame[this->tv0_frame = frame];
		if (cframe->tex != this->tv0_tex_id)
			Gfx_LoadTex(&this->tex_tv0, this->arc_tv0_ptr[this->tv0_tex_id = cframe->tex], 0);
	}
}


void Week2_tv0_Draw(Back_Week2 *this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame *cframe = &tv0_frame[this->tv0_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_tv0, &src, &dst, stage.camera.bzoom);
}

//smoke animation and rects
static const CharFrame smoke_frame[10] = {
    {0, {  0,    0,  13,  13}, { 0,  13}},
	{0, {  13,   0,  31,  39}, { 0,  39}},
	{0, {  44,   0,  33,  47}, { 0,  47}},
	{0, {  77,   0,  41,  61}, { 0,  61}},
	{0, { 118,   0,  43,  67}, { 0,  67}},
	{0, { 161,   0,  49,  79}, { 0,  79}},
	{0, { 210,   0,  46,  86}, { 0,  86}},
	{0, { 0,     37, 45,  89}, { 0,  89}},
	{0, { 45,    63, 45,  91}, { 0,  91}},
	{0, { 90,    67, 49,  86}, { 0,  86}},
	{0, { 139,   79, 51,  95}, { 0,  95}},
	{0, { 190,   87, 53,  79}, { 0,  79}},
	{0, { 0,     142, 45,  81}, { 0,  81}},
	{0, {  45,   154, 46,  63}, { 0,  63}},
	{0, {  90,   152, 47,  65}, { 0,  65}},
	{0, {  137,  173, 43,  50}, { 0,  50}},
	{0, {  180,  175, 32,  43}, { 0,  43}},
	{0, {  180,  173, 76,  45}, { 0,  45}},
};
static const Animation smoke_anim[1] = {

	//idle smoke
	{2, (const u8[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, ASCR_BACK, 0}}, //Left
};


//smoke functions
void Week2_smoke_SetFrame(void *user, u8 frame)
{
	Back_Week2 *this = (Back_Week2*)user;
	
	//Check if this is a new frame
	if (frame != this->smoke_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &smoke_frame[this->smoke_frame = frame];
		if (cframe->tex != this->smoke_tex_id)
			Gfx_LoadTex(&this->tex_smoke, this->arc_smoke_ptr[this->smoke_tex_id = cframe->tex], 0);
	}
}


void Week2_smoke_Draw(Back_Week2 *this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame *cframe = &smoke_frame[this->smoke_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_smoke, &src, &dst, stage.camera.bzoom);
}

//Week 2 background functions
void Back_Week2_DrawBG(StageBack *back)
{
	Back_Week2 *this = (Back_Week2*)back;
	
	fixed_t fx, fy;
	fx = stage.camera.x;
	fy = stage.camera.y;

	if (stage.stage_id == StageId_1_3 && stage.song_step >= 98) {
	//Draw error background
	RECT error_src = {0, 0, 256, 256};
	RECT_FIXED error_dst = {
		FIXED_DEC(-165,1) - fx,
		FIXED_DEC(-140,1) - fy,
		FIXED_DEC(550,1),
		FIXED_DEC(260,1)
	};
	
	Stage_DrawTex(&this->tex_error, &error_src, &error_dst, stage.camera.bzoom);
	}

	//Animate and draw tv0
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{
		switch (stage.song_step & 7)
		{
			case 0:
				Animatable_SetAnim(&this->tv0_animatable, 0);
				Animatable_SetAnim(&this->tv0a2_animatable, 0);
				break;
		}
	}
	
	Animatable_Animate(&this->tv0_animatable, (void*)this, Week2_tv0_SetFrame);
	{
	Week2_tv0_Draw(this, FIXED_DEC(-50,1) - fx, FIXED_DEC(30,1) - fy);
	Week2_tv0_Draw(this,  FIXED_DEC(50,1) - fx, FIXED_DEC(30,1) - fy);
	}
    
	Animatable_Animate(&this->tv0a2_animatable, (void*)this, Week2_tv0_SetFrame);
	{
	Week2_tv0_Draw(this, FIXED_DEC(150,1) - fx, FIXED_DEC(30,1) - fy);
	Week2_tv0_Draw(this, FIXED_DEC(250,1) - fx, FIXED_DEC(30,1) - fy);
	}

	//Animate and draw smoke
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{
		switch (stage.song_step & 7)
		{
			case 0:
				Animatable_SetAnim(&this->smoke_animatable, 0);
				break;
		}
	}
	
	Animatable_Animate(&this->smoke_animatable, (void*)this, Week2_smoke_SetFrame);
	Week2_smoke_Draw(this, FIXED_DEC(-50,1) - fx, FIXED_DEC(30,1) - fy);
	Week2_smoke_Draw(this,  FIXED_DEC(50,1) - fx, FIXED_DEC(30,1) - fy);

	//Draw background
	RECT back_src = {0, 0, 256, 256};
	RECT_FIXED back_dst = {
		FIXED_DEC(-165,1) - fx,
		FIXED_DEC(-140,1) - fy,
		FIXED_DEC(550,1),
		FIXED_DEC(260,1)
	};
	
	Stage_DrawTex(&this->tex_back0, &back_src, &back_dst, stage.camera.bzoom);
	
}

void Back_Week2_Free(StageBack *back)
{
	Back_Week2 *this = (Back_Week2*)back;
	
	//Free tv0 archive
	Mem_Free(this->arc_tv0);

	//Free tv0 archive
	Mem_Free(this->arc_smoke);

	//Free structure
	Mem_Free(this);
}

StageBack *Back_Week2_New(void)
{
	//Allocate background structure
	Back_Week2 *this = (Back_Week2*)Mem_Alloc(sizeof(Back_Week2));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Week2_DrawBG;
	this->back.free = Back_Week2_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\WEEK2\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Gfx_LoadTex(&this->tex_error, Archive_Find(arc_back, "error.tim"), 0);
	Mem_Free(arc_back);

	//Load tv0 textures
	this->arc_tv0 = IO_Read("\\WEEK2\\TV0.ARC;1");
	this->arc_tv0_ptr[0] = Archive_Find(this->arc_tv0, "tv0.tim");
	
	//Load smoke textures
	this->arc_smoke = IO_Read("\\WEEK2\\SMOKE.ARC;1");
	this->arc_smoke_ptr[0] = Archive_Find(this->arc_smoke, "smoke.tim");
	
	//Initialize tv0 state
	Animatable_Init(&this->tv0_animatable, tv0_anim);
	Animatable_Init(&this->tv0a2_animatable, tv0a2_anim);
	Animatable_SetAnim(&this->tv0a2_animatable, 0);
	Animatable_SetAnim(&this->tv0_animatable, 0);
	this->tv0_frame = this->tv0_tex_id = 0xFF; //Force art load

	//Initialize smoke state
	Animatable_Init(&this->smoke_animatable, smoke_anim);
	Animatable_SetAnim(&this->smoke_animatable, 0);
	this->smoke_frame = this->smoke_tex_id = 0xFF; //Force art load

	return (StageBack*)this;
}
