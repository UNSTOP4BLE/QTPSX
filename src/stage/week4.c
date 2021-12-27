/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "week4.h"

#include "../archive.h"
#include "../mem.h"
#include "../stage.h"
#include "../random.h"
#include "../timer.h"
#include "../animation.h"

//Week 4 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	IO_Data arc_tv0, arc_tv0_ptr[2];
	IO_Data arc_tv0l, arc_tv0l_ptr[1];
	
	//tv0 state
	Gfx_Tex tex_tv0 ;
	u8 tv0_frame, tv0_tex_id;
	//tv0 state
	Gfx_Tex tex_tv0l ;
	u8 tv0l_frame, tv0l_tex_id;

	Animatable tv0wright_animatable;
	Animatable tv0wleft_animatable;

	Gfx_Tex tex_back2; //Sunset

} Back_Week4;

//tv0 animation and rects
static const CharFrame tv0_frame[1] = {
	{1, {0, 67, 128, 67}, { 128,  67}}, //right 3 (turned off)

};

static const Animation tv0wright_anim[1] = {

	{2, (const u8[]){0, ASCR_BACK, 0}}, 
};

//tv0 functions
void Week4_tv0_SetFrame(void *user, u8 frame)
{
	Back_Week4 *this = (Back_Week4*)user;
	
	//Check if this is a new frame
	if (frame != this->tv0_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &tv0_frame[this->tv0_frame = frame];
		if (cframe->tex != this->tv0_tex_id)
			Gfx_LoadTex(&this->tex_tv0, this->arc_tv0_ptr[this->tv0_tex_id = cframe->tex], 0);
	}
}

void Week4_tv0_Draw(Back_Week4 *this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame *cframe = &tv0_frame[this->tv0_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_tv0, &src, &dst, stage.camera.bzoom);
}


static const CharFrame tv0l_frame[1] = {
	//warning left
	{0, {62,   205,  62, 51}, { 62,  52}}, //left 3 (turned off
};
//warning
static const Animation tv0wleft_anim[1] = {

	{2, (const u8[]){0, ASCR_BACK, 0}},
};

//tv0 functions
void Week4_tv0l_SetFrame(void *user, u8 frame)
{
	Back_Week4 *this = (Back_Week4*)user;
	
	//Check if this is a new frame
	if (frame != this->tv0l_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &tv0l_frame[this->tv0l_frame = frame];
		if (cframe->tex != this->tv0l_tex_id)
			Gfx_LoadTex(&this->tex_tv0l, this->arc_tv0l_ptr[this->tv0l_tex_id = cframe->tex], 0);
	}
}

void Week4_tv0l_Draw(Back_Week4 *this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame *cframe = &tv0l_frame[this->tv0l_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_tv0l, &src, &dst, stage.camera.bzoom);
}


//Week 4 background functions
void Back_Week4_DrawFG(StageBack *back)
{
	Back_Week4 *this = (Back_Week4*)back;
	
	fixed_t fx, fy;
	

}

void Back_Week4_DrawMD(StageBack *back)
{
	Back_Week4 *this = (Back_Week4*)back;
	
	fixed_t fx, fy;

}

void Back_Week4_DrawBG(StageBack *back)
{
	Back_Week4 *this = (Back_Week4*)back;
	
	fixed_t fx, fy;


	//idle tv
	fx = stage.camera.x;
	fy = stage.camera.y;

	Animatable_Animate(&this->tv0wleft_animatable, (void*)this, Week4_tv0l_SetFrame);
	Week4_tv0l_Draw(this, FIXED_DEC(40,1) - fx, FIXED_DEC(37,1) - fy);
    
	Animatable_Animate(&this->tv0wright_animatable, (void*)this, Week4_tv0_SetFrame);
	Week4_tv0_Draw(this, FIXED_DEC(360,1) - fx, FIXED_DEC(43,1) - fy);

	//Draw sunset
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	RECT sunset_src = {0, 0, 256, 256};
	RECT_FIXED sunset_dst = {
		FIXED_DEC(-165 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140,1) - fy,
		FIXED_DEC(550 + SCREEN_WIDEOADD,1),
		FIXED_DEC(260,1)
	};
	
	Stage_DrawTex(&this->tex_back2, &sunset_src, &sunset_dst, stage.camera.bzoom);
}

void Back_Week4_Free(StageBack *back)
{
	Back_Week4 *this = (Back_Week4*)back;
	
	//Free tv0 archive
	Mem_Free(this->arc_tv0);

	//Free tv0 archive
	Mem_Free(this->arc_tv0l);

	//Free structure
	Mem_Free(this);
}

StageBack *Back_Week4_New(void)
{
	//Allocate background structure
	Back_Week4 *this = (Back_Week4*)Mem_Alloc(sizeof(Back_Week4));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = Back_Week4_DrawFG;
	this->back.draw_md = Back_Week4_DrawMD;
	this->back.draw_bg = Back_Week4_DrawBG;
	this->back.free = Back_Week4_Free;
	
	//Load tv0 textures
	this->arc_tv0 = IO_Read("\\WEEK2\\TV.ARC;1");
	this->arc_tv0_ptr[0] = Archive_Find(this->arc_tv0, "tv0.tim");
	this->arc_tv0_ptr[1] = Archive_Find(this->arc_tv0, "tv1.tim");
	//Load tv0 textures
	this->arc_tv0l = IO_Read("\\WEEK2\\TV.ARC;1");
	this->arc_tv0l_ptr[0] = Archive_Find(this->arc_tv0l, "tv0.tim");
	

	//Load background textures
	IO_Data arc_back = IO_Read("\\WEEK4\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back2, Archive_Find(arc_back, "back2.tim"), 0);

	//Initialize tv0 state
	Animatable_Init(&this->tv0wright_animatable, tv0wright_anim);
	Animatable_Init(&this->tv0wleft_animatable, tv0wleft_anim);
	
	Animatable_SetAnim(&this->tv0wright_animatable, 0);
	Animatable_SetAnim(&this->tv0wleft_animatable, 0);
	this->tv0_frame = this->tv0_tex_id = 0xFF; //Force art load
	this->tv0l_frame = this->tv0l_tex_id = 0xFF; //Force art load
	Mem_Free(arc_back);
	
	return (StageBack*)this;
}
