/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "termination.h"

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
	IO_Data arc_smoke, arc_smoke_ptr[1];
	IO_Data arc_warning, arc_warning_ptr[1];
	IO_Data arc_saw, arc_saw_ptr[1];

	Gfx_Tex tex_back0; //Background
	Gfx_Tex tex_error; //Background
	Gfx_Tex tex_bsod; //Background

	//tv0 state
	Gfx_Tex tex_tv0 ;
	u8 tv0_frame, tv0_tex_id;
	//tv0 state
	Gfx_Tex tex_tv0l ;
	u8 tv0l_frame, tv0l_tex_id;

	//warning
	Gfx_Tex tex_warning ;
	u8 warning_frame, warning_tex_id;

	//saw
	Gfx_Tex tex_saw ;
	u8 saw_frame, saw_tex_id;
	
	Animatable tv0right_animatable;
	Animatable tv0eyeright_animatable;
	Animatable tv0wright_animatable;
	Animatable tv0errright_animatable;
	Animatable tv0bsodright_animatable;
	Animatable tv0dicright_animatable;
	Animatable tv0left_animatable;
	Animatable tv0eyeleft_animatable;
	Animatable tv0wleft_animatable;
	Animatable tv0errleft_animatable;
	Animatable tv0bsodleft_animatable;
	Animatable tv0dicleft_animatable;

	Animatable warning_animatable;
	Animatable saw_animatable;
} Back_termination;

static const CharFrame saw_frame[7] = {
	//saw0.png
	{0, {  0,   146,  144,  110}, { 0,  0}},
	{0, {  0,  46,  191,  96}, { 0,  0}},
	//saw1.png
	{0, {  0, 0,  256,  58}, { 0,  0}},
	{0, {  0, 58,  256,  40}, { 0,  0}},
	{0, {  0, 98,  256,  42}, { 0,  0}},
	{0, {  0, 140,  256,  41}, { 0,  0}},
	{0, {  0, 181,  256,  37}, { 0,  0}},
};
//saw
static const Animation saw_anim[1] = {
	{1, (const u8[]){0, 1, 2, 3, 4, 5, 6, ASCR_BACK, 0}},
};

//saw functions
void termination_saw_SetFrame(void *user, u8 frame)
{
	Back_termination *this = (Back_termination*)user;
	
	//Check if this is a new frame
	if (frame != this->saw_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &saw_frame[this->saw_frame = frame];
		if (cframe->tex != this->saw_tex_id)
			Gfx_LoadTex(&this->tex_saw, this->arc_saw_ptr[this->saw_tex_id = cframe->tex], 0);
	}
}

void termination_saw_Draw(Back_termination *this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame *cframe = &saw_frame[this->saw_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_saw, &src, &dst, stage.camera.bzoom);
}


static const CharFrame warning_frame[4] = {
	{0, {  0,   0,  57,  50}, { 0,  0}},
	{0, {  57,  0,  57,  50}, { 0,  0}},
	{0, {  114, 0,  57,  50}, { 0,  0}},
	{0, {  171, 0,  57,  50}, { 0,  0}},
};
//warning
static const Animation warning_anim[1] = {
	{1, (const u8[]){0, 0, 1, 2, 3,  ASCR_BACK, 0}},
};

//warning functions
void termination_warning_SetFrame(void *user, u8 frame)
{
	Back_termination *this = (Back_termination*)user;
	
	//Check if this is a new frame
	if (frame != this->warning_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &warning_frame[this->warning_frame = frame];
		if (cframe->tex != this->warning_tex_id)
			Gfx_LoadTex(&this->tex_warning, this->arc_warning_ptr[this->warning_tex_id = cframe->tex], 0);
	}
}

void termination_warning_Draw(Back_termination *this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame *cframe = &warning_frame[this->warning_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_warning, &src, &dst, stage.camera.bzoom);
}


//tv0 animation and rects
static const CharFrame tv0_frame[8] = {
	//tv0.png
	//idle
	{0, { 62,   0,  128,  67}, { 128,  67}}, //right
	//eye
	{0, { 62,  67,  127,  67}, { 128,  67}}, //right
	//warning
	{0, {62, 134,  128, 67}, { 128,  67}}, //right 1
	//tv1.png
	{0, {0, 0,  128, 67}, { 128,  67}}, //right 2
	{0, {0, 67, 128, 67}, { 128,  67}}, //right 3 (turned off)

	//error
	{0, { 0,   134,  127,  67}, { 128,  67}}, //right

	//bsod
	{0, { 128,   67,  128, 67}, { 128,  67}}, //right

	//incomin drop
	{0, { 129,   0,  127, 67}, { 128,  67}}, //right
};



static const Animation tv0right_anim[1] = {

	{2, (const u8[]){0, 0, ASCR_BACK, 1}}, 
};


static const Animation tv0eyeright_anim[1] = {

	{2, (const u8[]){1, 1, ASCR_BACK, 1}},
};   


static const Animation tv0wright_anim[1] = {

	{2, (const u8[]){2, 3, 4, ASCR_BACK, 1}}, 
};

static const Animation tv0errright_anim[1] = {

	{2, (const u8[]){5, 5, ASCR_BACK, 1}}, 
};

static const Animation tv0bsodright_anim[1] = {

	{2, (const u8[]){6, 6, ASCR_BACK, 1}}, 
};

static const Animation tv0dicright_anim[1] = {

	{2, (const u8[]){7, 7, ASCR_BACK, 1}}, 
};

//tv0 functions
void termination_tv0_SetFrame(void *user, u8 frame)
{
	Back_termination *this = (Back_termination*)user;
	
	//Check if this is a new frame
	if (frame != this->tv0_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &tv0_frame[this->tv0_frame = frame];
		if (cframe->tex != this->tv0_tex_id)
			Gfx_LoadTex(&this->tex_tv0, this->arc_tv0_ptr[this->tv0_tex_id = cframe->tex], 0);
	}
}



void termination_tv0_Draw(Back_termination *this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame *cframe = &tv0_frame[this->tv0_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_tv0, &src, &dst, stage.camera.bzoom);
}


static const CharFrame tv0l_frame[8] = {
	//tv0.png
	//idle tv
	{0, {  0,   0,  62,  52}, { 62,  52}}, //left 
	//red eye tv thing
	{0, {  0,  67,  62,  51}, { 62,  52}}, //left 
	//warning left
	{0, {0,   134,  62, 52}, { 62,  52}}, //left 1 
	{0, {0,   204,  62, 52}, { 62,  52}}, //left 2
	{0, {62,   205,  62, 51}, { 62,  52}}, //left 3 (turned off)

	//error left
	{0, {194,   102,  62, 51}, { 62,  52}}, //left

	//bsod left
	{0, {194,   0,  62, 51}, { 62,  52}}, //left

	//drop incoming left
	{0, {194,   51, 62, 51}, { 62,  52}}, //left
};
//idle
static const Animation tv0left_anim[1] = {

	{2, (const u8[]){0, 0, ASCR_BACK, 1}},
};
//eye
static const Animation tv0eyeleft_anim[1] = {

	{2, (const u8[]){1, 1, ASCR_BACK, 1}},
};
//warning
static const Animation tv0wleft_anim[1] = {

	{2, (const u8[]){2, 3, 4, ASCR_BACK, 1}},
};
//error
static const Animation tv0errleft_anim[1] = {

	{2, (const u8[]){5, 5, ASCR_BACK, 1}},
};
//bsod
static const Animation tv0bsodleft_anim[1] = {

	{2, (const u8[]){6, 6, ASCR_BACK, 1}},
};
//drop incoming
static const Animation tv0dicleft_anim[1] = {

	{2, (const u8[]){7, 7, ASCR_BACK, 1}},
};


//tv0 functions
void termination_tv0l_SetFrame(void *user, u8 frame)
{
	Back_termination *this = (Back_termination*)user;
	
	//Check if this is a new frame
	if (frame != this->tv0l_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &tv0l_frame[this->tv0l_frame = frame];
		if (cframe->tex != this->tv0l_tex_id)
			Gfx_LoadTex(&this->tex_tv0l, this->arc_tv0l_ptr[this->tv0l_tex_id = cframe->tex], 0);
	}
}

void termination_tv0l_Draw(Back_termination *this, fixed_t x, fixed_t y)
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
void Back_termination_DrawFG(StageBack *back)
{
	Back_termination *this = (Back_termination*)back;
	
	fixed_t fx, fy;
	fx = stage.camera.x;
	fy = stage.camera.y;

	//Animate and draw warning
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{
		switch (stage.song_step & 7)
		{
			case 0:
				Animatable_SetAnim(&this->warning_animatable, 0);
				break;
		}
	}
	
	Animatable_Animate(&this->warning_animatable, (void*)this, termination_warning_SetFrame);
	termination_warning_Draw(this, FIXED_DEC(-28,1), FIXED_DEC(-40,1));
}

void Back_termination_DrawBG(StageBack *back)
{
	Back_termination *this = (Back_termination*)back;
	
	fixed_t fx, fy;
	fx = stage.camera.x;
	fy = stage.camera.y;

	//Animate and draw saw
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{
		switch (stage.song_step & 7)
		{
			case 0:
				Animatable_SetAnim(&this->saw_animatable, 0);
				break;
		}
	}
	
	Animatable_Animate(&this->saw_animatable, (void*)this, termination_saw_SetFrame);
	termination_saw_Draw(this, FIXED_DEC(-28,1), FIXED_DEC(-40,1));

	//Animate and draw tv0left
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{
		switch (stage.song_step & 7)
		{
			case 0:
				Animatable_SetAnim(&this->tv0left_animatable, 0);
				Animatable_SetAnim(&this->tv0eyeleft_animatable, 0);
				Animatable_SetAnim(&this->tv0wleft_animatable, 0);
				Animatable_SetAnim(&this->tv0errleft_animatable, 0);
	            Animatable_SetAnim(&this->tv0bsodleft_animatable, 0);
	            Animatable_SetAnim(&this->tv0dicleft_animatable, 0);
				break;
		}
	}

	//idle tv
	
	termination_tv0l_Draw(this, FIXED_DEC(40,1) - fx, FIXED_DEC(37,1) - fy);
    
	termination_tv0_Draw(this, FIXED_DEC(360,1) - fx, FIXED_DEC(43,1) - fy);

/*
████████╗███████╗██████╗░███╗░░░███╗██╗███╗░░██╗░█████╗░████████╗██╗░█████╗░███╗░░██╗
╚══██╔══╝██╔════╝██╔══██╗████╗░████║██║████╗░██║██╔══██╗╚══██╔══╝██║██╔══██╗████╗░██║
░░░██║░░░█████╗░░██████╔╝██╔████╔██║██║██╔██╗██║███████║░░░██║░░░██║██║░░██║██╔██╗██║
░░░██║░░░██╔══╝░░██╔══██╗██║╚██╔╝██║██║██║╚████║██╔══██║░░░██║░░░██║██║░░██║██║╚████║
░░░██║░░░███████╗██║░░██║██║░╚═╝░██║██║██║░╚███║██║░░██║░░░██║░░░██║╚█████╔╝██║░╚███║
░░░╚═╝░░░╚══════╝╚═╝░░╚═╝╚═╝░░░░░╚═╝╚═╝╚═╝░░╚══╝╚═╝░░╚═╝░░░╚═╝░░░╚═╝░╚════╝░╚═╝░░╚══╝*/

switch(stage.stage_id)
{
	case StageId_2_2:
    
	//eye
    if (stage.timercount >= 1311 && stage.timercount <= 1613)
	{
	Animatable_Animate(&this->tv0eyeleft_animatable, (void*)this, termination_tv0l_SetFrame);

	Animatable_Animate(&this->tv0eyeright_animatable, (void*)this, termination_tv0_SetFrame);	
	}
	
    //drop
	else if (stage.timercount >= 4629 && stage.timercount <= 5836)
	{
	Animatable_Animate(&this->tv0dicleft_animatable, (void*)this, termination_tv0l_SetFrame);
    
	Animatable_Animate(&this->tv0dicright_animatable, (void*)this, termination_tv0_SetFrame);
	}

    //warning
    else if (stage.timercount >= 5837 && stage.timercount <= 8256)
	{
	Animatable_Animate(&this->tv0wleft_animatable, (void*)this, termination_tv0l_SetFrame);

	Animatable_Animate(&this->tv0wright_animatable, (void*)this, termination_tv0_SetFrame);
	}

    //eye
	else if (stage.timercount >= 10634 && stage.timercount <= 10671)
	{
	Animatable_Animate(&this->tv0eyeleft_animatable, (void*)this, termination_tv0l_SetFrame);
    
	Animatable_Animate(&this->tv0eyeright_animatable, (void*)this, termination_tv0_SetFrame);
	}

	//error
	else if (stage.timercount >= 13354 && stage.timercount <= 13392)
	{
	Animatable_Animate(&this->tv0errleft_animatable, (void*)this, termination_tv0l_SetFrame);
    
	Animatable_Animate(&this->tv0errright_animatable, (void*)this, termination_tv0_SetFrame);
	}

	//bsod
	else if (stage.timercount >= 13392 && stage.timercount <= 15811)
	{
	Animatable_Animate(&this->tv0bsodleft_animatable, (void*)this, termination_tv0l_SetFrame);
    
	Animatable_Animate(&this->tv0bsodright_animatable, (void*)this, termination_tv0_SetFrame);
	}

    //warning
	else if (stage.timercount >= 15812 && stage.timercount <= 18237)
	{
	Animatable_Animate(&this->tv0wleft_animatable, (void*)this, termination_tv0l_SetFrame);

	Animatable_Animate(&this->tv0wright_animatable, (void*)this, termination_tv0_SetFrame);
	}

	else
	{
	 Animatable_Animate(&this->tv0left_animatable, (void*)this, termination_tv0l_SetFrame);

	 Animatable_Animate(&this->tv0right_animatable, (void*)this, termination_tv0_SetFrame);
	}

	break;
  default:
break;


}
	
	//Draw bsod background
	if (stage.stage_id == StageId_2_2 && stage.timercount >= 13392 && stage.timercount <=15811) 
	{
	RECT bsod_src = {0, 0, 256, 256};
	RECT_FIXED bsod_dst = {
		FIXED_DEC(-165,1) - fx,
		FIXED_DEC(-140,1) - fy,
		FIXED_DEC(550,1),
		FIXED_DEC(260,1)
	};
	
	Stage_DrawTex(&this->tex_bsod, &bsod_src, &bsod_dst, stage.camera.bzoom);
	}

	//Draw error background
	if (stage.stage_id == StageId_2_2 && stage.timercount >=13354 && stage.timercount <= 13392) 
	{
	RECT error_src = {0, 0, 256, 256};
	RECT_FIXED error_dst = {
		FIXED_DEC(-165,1) - fx,
		FIXED_DEC(-140,1) - fy,
		FIXED_DEC(550,1),
		FIXED_DEC(260,1)
	};
	
	Stage_DrawTex(&this->tex_error, &error_src, &error_dst, stage.camera.bzoom);
	}

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

void Back_termination_Free(StageBack *back)
{
	Back_termination *this = (Back_termination*)back;
	
	//Free tv0 archive
	Mem_Free(this->arc_tv0);

	//Free smoke archive
	Mem_Free(this->arc_smoke);

	//Free warning archive
	Mem_Free(this->arc_warning);

	//Free warning archive
	Mem_Free(this->arc_saw);

	//Free structure
	Mem_Free(this);
}

StageBack *Back_termination_New(void)
{
	//Allocate background structure
	Back_termination *this = (Back_termination*)Mem_Alloc(sizeof(Back_termination));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = Back_termination_DrawFG;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_termination_DrawBG;
	this->back.free = Back_termination_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\WEEK2\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Gfx_LoadTex(&this->tex_error, Archive_Find(arc_back, "error.tim"), 0);
	Gfx_LoadTex(&this->tex_bsod, Archive_Find(arc_back, "bsod.tim"), 0);
	Mem_Free(arc_back);

	//Load tv0 textures
	this->arc_tv0 = IO_Read("\\WEEK2\\TV.ARC;1");
	this->arc_tv0_ptr[0] = Archive_Find(this->arc_tv0, "tv0.tim");
	this->arc_tv0_ptr[1] = Archive_Find(this->arc_tv0, "tv1.tim");
	//Load tv0 textures
	this->arc_tv0l = IO_Read("\\WEEK2\\TV.ARC;1");
	this->arc_tv0l_ptr[0] = Archive_Find(this->arc_tv0l, "tv0.tim");

	//Load warning textures
	this->arc_warning = IO_Read("\\STAGE\\WARNING.ARC;1");
	this->arc_warning_ptr[0] = Archive_Find(this->arc_warning, "warning.tim");

	//Load warning textures
	this->arc_saw = IO_Read("\\STAGE\\SAW.ARC;1");
	this->arc_saw_ptr[0] = Archive_Find(this->arc_saw, "saw0.tim");

	
	//Initialize tv0 state
	Animatable_Init(&this->tv0right_animatable, tv0right_anim);
	Animatable_Init(&this->tv0eyeright_animatable, tv0eyeright_anim);
	Animatable_Init(&this->tv0wright_animatable, tv0wright_anim);
	Animatable_Init(&this->tv0errright_animatable, tv0errright_anim);
	Animatable_Init(&this->tv0bsodright_animatable, tv0bsodright_anim);
	Animatable_Init(&this->tv0dicright_animatable, tv0dicright_anim);
	Animatable_Init(&this->tv0left_animatable, tv0left_anim);
	Animatable_Init(&this->tv0eyeleft_animatable, tv0eyeleft_anim);
	Animatable_Init(&this->tv0wleft_animatable, tv0wleft_anim);
	Animatable_Init(&this->tv0errleft_animatable, tv0errleft_anim);
	Animatable_Init(&this->tv0bsodleft_animatable, tv0bsodleft_anim);
	Animatable_Init(&this->tv0dicleft_animatable, tv0dicleft_anim);

	Animatable_Init(&this->warning_animatable, warning_anim);
	Animatable_Init(&this->saw_animatable, saw_anim);
	
	Animatable_SetAnim(&this->tv0right_animatable, 0);
	Animatable_SetAnim(&this->tv0eyeright_animatable, 0);
	Animatable_SetAnim(&this->tv0wright_animatable, 0);
	Animatable_SetAnim(&this->tv0errright_animatable, 0);
	Animatable_SetAnim(&this->tv0bsodright_animatable, 0);
	Animatable_SetAnim(&this->tv0left_animatable, 0);
	Animatable_SetAnim(&this->tv0eyeleft_animatable, 0);
	Animatable_SetAnim(&this->tv0wleft_animatable, 0);
	Animatable_SetAnim(&this->tv0errleft_animatable, 0);
	Animatable_SetAnim(&this->tv0bsodleft_animatable, 0);
	Animatable_SetAnim(&this->tv0dicleft_animatable, 0);
	this->tv0_frame = this->tv0_tex_id = 0xFF; //Force art load
	this->tv0l_frame = this->tv0l_tex_id = 0xFF; //Force art load

	this->warning_frame = this->warning_tex_id = 0xFF; //Force art load

	this->saw_frame = this->saw_tex_id = 0xFF; //Force art load

	return (StageBack*)this;
}
