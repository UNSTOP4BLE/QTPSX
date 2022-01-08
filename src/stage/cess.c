/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "cess.h"

#include "../mem.h"
#include "../archive.h"
#include "../animation.h"

//Week 2 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	IO_Data arc_tv0, arc_tv0_ptr[2];
	IO_Data arc_tv0l, arc_tv0l_ptr[1];

	Gfx_Tex tex_back0; //Background

	//tv0 state
	Gfx_Tex tex_tv0 ;
	u8 tv0_frame, tv0_tex_id;
	//tv0 state
	Gfx_Tex tex_tv0l ;
	u8 tv0l_frame, tv0l_tex_id;
	
	Animatable tv0right_animatable;
	Animatable tv0left_animatable;
} Back_cess;

//tv0 animation and rects
static const CharFrame tv0_frame[4] = {
	//idle
	{0, {  0,  76,  128,  67}, { 128,  68}}, //right
	{0, {129,  81,  127,  67}, { 127,  67}}, //right
	{0, {  0, 147,  129,  67}, { 128,  67}}, //right 1
	{0, {129, 155,  127,  67}, { 127,  67}}, //right 2
};

static const Animation tv0right_anim[1] = {

	{2, (const u8[]){0, 1, 2, 3, ASCR_BACK, 0}}, 
};

//tv0 functions
void cess_tv0_SetFrame(void *user, u8 frame)
{
	Back_cess *this = (Back_cess*)user;
	
	//Check if this is a new frame
	if (frame != this->tv0_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &tv0_frame[this->tv0_frame = frame];
		if (cframe->tex != this->tv0_tex_id)
			Gfx_LoadTex(&this->tex_tv0, this->arc_tv0_ptr[this->tv0_tex_id = cframe->tex], 0);
	}
}



void cess_tv0_Draw(Back_cess *this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame *cframe = &tv0_frame[this->tv0_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_tv0, &src, &dst, stage.camera.bzoom);
}


static const CharFrame tv0l_frame[4] = {
	//idle tv
	{0, {  0, 204,  62,  52}, { 62,  52}}, //left 
	{0, { 62, 205,  62,  51}, { 62,  51}}, //left 
	{0, {124, 205,  62,  51}, { 62,  51}}, //left 1 
	{0, {188, 204,  62,  52}, { 62,  52}}, //left 2
};
//idle
static const Animation tv0left_anim[1] = {

	{2, (const u8[]){0, 1, 2, 3, ASCR_BACK, 0}},
};

//tv0 functions
void cess_tv0l_SetFrame(void *user, u8 frame)
{
	Back_cess *this = (Back_cess*)user;
	
	//Check if this is a new frame
	if (frame != this->tv0l_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &tv0l_frame[this->tv0l_frame = frame];
		if (cframe->tex != this->tv0l_tex_id)
			Gfx_LoadTex(&this->tex_tv0l, this->arc_tv0l_ptr[this->tv0l_tex_id = cframe->tex], 0);
	}
}

void cess_tv0l_Draw(Back_cess *this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame *cframe = &tv0l_frame[this->tv0l_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_tv0l, &src, &dst, stage.camera.bzoom);
}

//Week 2 background functions
void Back_cess_DrawFG(StageBack *back)
{
	Back_cess *this = (Back_cess*)back;
	
	fixed_t fx, fy;
	fx = stage.camera.x;
	fy = stage.camera.y;

}
void Back_cess_DrawBG(StageBack *back)
{
	Back_cess *this = (Back_cess*)back;
	
	fixed_t fx, fy;
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	//Animate and draw tv0right
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{
		switch (stage.song_step & 7)
		{
			case 0:
				Animatable_SetAnim(&this->tv0right_animatable, 0);
				break;
		}
	}
	//Animate and draw tv0left
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{
		switch (stage.song_step & 7)
		{
			case 0:
				Animatable_SetAnim(&this->tv0left_animatable, 0);
				break;
		}
	}

	//idle tv
	cess_tv0l_Draw(this, FIXED_DEC(40,1) - fx, FIXED_DEC(37,1) - fy);
    Animatable_Animate(&this->tv0left_animatable, (void*)this, cess_tv0l_SetFrame);
	cess_tv0_Draw(this, FIXED_DEC(360,1) - fx, FIXED_DEC(43,1) - fy);
	Animatable_Animate(&this->tv0right_animatable, (void*)this, cess_tv0_SetFrame);

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

void Back_cess_Free(StageBack *back)
{
	Back_cess *this = (Back_cess*)back;
	
	//Free tv0 archive
	Mem_Free(this->arc_tv0);

	//Free tv0 archive
	Mem_Free(this->arc_tv0l);

	//Free structure
	Mem_Free(this);
}

StageBack *Back_cess_New(void)
{
	//Allocate background structure
	Back_cess *this = (Back_cess*)Mem_Alloc(sizeof(Back_cess));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = Back_cess_DrawFG;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_cess_DrawBG;
	this->back.free = Back_cess_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\WEEK4\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back2.tim"), 0);
	Mem_Free(arc_back);

	//Load tv0 textures
	this->arc_tv0 = IO_Read("\\WEEK4\\BACK.ARC;1");
	this->arc_tv0_ptr[0] = Archive_Find(this->arc_tv0, "tv2.tim");
	//Load tv0 textures
	this->arc_tv0l = IO_Read("\\WEEK2\\TV.ARC;1");
	this->arc_tv0l_ptr[0] = Archive_Find(this->arc_tv0l, "tv1.tim");
	
	//Initialize tv0 state
	Animatable_Init(&this->tv0right_animatable, tv0right_anim);
	Animatable_Init(&this->tv0left_animatable, tv0left_anim);
	
	Animatable_SetAnim(&this->tv0right_animatable, 0);
	Animatable_SetAnim(&this->tv0left_animatable, 0);
	this->tv0_frame = this->tv0_tex_id = 0xFF; //Force art load
	this->tv0l_frame = this->tv0l_tex_id = 0xFF; //Force art load

	return (StageBack*)this;
}
