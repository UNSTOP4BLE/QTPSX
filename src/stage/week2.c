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
	IO_Data arc_tv0, arc_tv0_ptr[2];
	IO_Data arc_tv0l, arc_tv0l_ptr[1];
	IO_Data arc_smoke, arc_smoke_ptr[1];

	Gfx_Tex tex_back0; //Background
	Gfx_Tex tex_error; //Background
	Gfx_Tex tex_bsod; //Background

	//tv0 state
	Gfx_Tex tex_tv0 ;
	u8 tv0_frame, tv0_tex_id;
	//tv0 state
	Gfx_Tex tex_tv0l ;
	u8 tv0l_frame, tv0l_tex_id;

	//smoke state
	Gfx_Tex tex_smoke;
	u8 smoke_frame, smoke_tex_id;
	
	
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
	Animatable smoke_animatable;
} Back_Week2;

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
	{1, {0, 0,  128, 67}, { 128,  67}}, //right 2
	{1, {0, 67, 128, 67}, { 128,  67}}, //right 3 (turned off)

	//error
	{1, { 0,   134,  127,  67}, { 128,  67}}, //right

	//bsod
	{1, { 128,   67,  128, 67}, { 128,  67}}, //right

	//incomin drop
	{1, { 129,   0,  127, 67}, { 128,  67}}, //right
};



static const Animation tv0right_anim[1] = {

	{2, (const u8[]){0, ASCR_BACK, 0}}, 
};


static const Animation tv0eyeright_anim[1] = {

	{2, (const u8[]){1, ASCR_BACK, 0}},
};   


static const Animation tv0wright_anim[1] = {

	{2, (const u8[]){2, 3, 4, ASCR_BACK, 0}}, 
};

static const Animation tv0errright_anim[1] = {

	{2, (const u8[]){5, ASCR_BACK, 0}}, 
};

static const Animation tv0bsodright_anim[1] = {

	{2, (const u8[]){6, ASCR_BACK, 0}}, 
};

static const Animation tv0dicright_anim[1] = {

	{2, (const u8[]){7, ASCR_BACK, 0}}, 
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

	{2, (const u8[]){0, ASCR_BACK, 0}},
};
//eye
static const Animation tv0eyeleft_anim[1] = {

	{2, (const u8[]){1, ASCR_BACK, 0}},
};
//warning
static const Animation tv0wleft_anim[1] = {

	{2, (const u8[]){2, 3, 4, ASCR_BACK, 0}},
};
//error
static const Animation tv0errleft_anim[1] = {

	{2, (const u8[]){5, ASCR_BACK, 0}},
};
//bsod
static const Animation tv0bsodleft_anim[1] = {

	{2, (const u8[]){6, ASCR_BACK, 0}},
};
//drop incoming
static const Animation tv0dicleft_anim[1] = {

	{2, (const u8[]){7, ASCR_BACK, 0}},
};

//smoke animation and rects
static const CharFrame smoke_frame[18] = {
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
	{2, (const u8[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, ASCR_BACK, 0}}, 
};

//tv0 functions
void Week2_tv0l_SetFrame(void *user, u8 frame)
{
	Back_Week2 *this = (Back_Week2*)user;
	
	//Check if this is a new frame
	if (frame != this->tv0l_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &tv0l_frame[this->tv0l_frame = frame];
		if (cframe->tex != this->tv0l_tex_id)
			Gfx_LoadTex(&this->tex_tv0, this->arc_tv0l_ptr[this->tv0l_tex_id = cframe->tex], 0);
	}
}

void Week2_tv0l_Draw(Back_Week2 *this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame *cframe = &tv0l_frame[this->tv0l_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_tv0, &src, &dst, stage.camera.bzoom);
}


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
void Back_Week2_DrawFG(StageBack *back)
{
	Back_Week2 *this = (Back_Week2*)back;
	
	fixed_t fx, fy;
	fx = stage.camera.x;
	fy = stage.camera.y;

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
	
	int smoke = 0;
	int smokecooldown = 0;

	if (smokecooldown > 0) 
        smokecooldown -= 1;
	

	if (smoke == 1)
	{

       	if (smokecooldown == 0)
            smoke = 0;

		Animatable_Animate(&this->smoke_animatable, (void*)this, Week2_smoke_SetFrame);
		Week2_smoke_Draw(this, FIXED_DEC(34,1), FIXED_DEC(37,1));

		Animatable_Animate(&this->smoke_animatable, (void*)this, Week2_smoke_SetFrame);
		Week2_smoke_Draw(this, FIXED_DEC(290,1), FIXED_DEC(43,1));
	}

	//holy fuck this is garbage 
	//smoke 1
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 316 && stage.song_step <= 336) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
	//smoke 2
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 384 && stage.song_step <= 400) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
	//smoke 3
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 448 && stage.song_step <= 464) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
	//smoke 4
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 512 && stage.song_step <= 528) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
	//smoke 5
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 576 && stage.song_step <= 592) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
	//smoke 6
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 640 && stage.song_step <= 656) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
	//smoke 6
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 704 && stage.song_step <= 720) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
	//smoke 7
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 768 && stage.song_step <= 784) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
	//smoke 8
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 832 && stage.song_step <= 848) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
	//smoke 9
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 640 && stage.song_step <= 656) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
	//smoke 10
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 1216 && stage.song_step <= 1222) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
	//smoke 11
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 1248 && stage.song_step <= 1261) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
	//smoke 12
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 1280 && stage.song_step <= 1294) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
	//smoke 13
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 1312 && stage.song_step <= 1325) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
	//smoke 14
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 1344 && stage.song_step <= 1357) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
	//smoke 15
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 1376 && stage.song_step <= 1388) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
	//smoke 16
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 1408 && stage.song_step <= 1422) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
	//smoke 17
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 1440 && stage.song_step <= 1453) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
    //smoke 18
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 1472 && stage.song_step <= 1486) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
    //smoke 19
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 1504 && stage.song_step <= 1517) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
    //smoke 20
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 1536 && stage.song_step <= 1549) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
    //smoke 21
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 1600 && stage.song_step <= 1614) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
    //smoke 22
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 1632 && stage.song_step <= 1645) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
    //smoke 23
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 1664 && stage.song_step <= 1677) 
	{
		smoke = 1;
		smokecooldown = 50;
	}
    //smoke 24
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 1696 && stage.song_step <= 1709) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
    //smoke 25
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 1408 && stage.song_step <= 1422) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
    //smoke 26
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 1729 && stage.song_step <= 1742) 
	{
		smoke = 1; 
		smokecooldown = 50;
	}
}

void Back_Week2_DrawBG(StageBack *back)
{
	Back_Week2 *this = (Back_Week2*)back;
	
	fixed_t fx, fy;
	fx = stage.camera.x;
	fy = stage.camera.y;

/*
	░█████╗░░█████╗░██████╗░███████╗██╗░░░░░███████╗░██████╗░██████╗
	██╔══██╗██╔══██╗██╔══██╗██╔════╝██║░░░░░██╔════╝██╔════╝██╔════╝
	██║░░╚═╝███████║██████╔╝█████╗░░██║░░░░░█████╗░░╚█████╗░╚█████╗░
	██║░░██╗██╔══██║██╔══██╗██╔══╝░░██║░░░░░██╔══╝░░░╚═══██╗░╚═══██╗
	╚█████╔╝██║░░██║██║░░██║███████╗███████╗███████╗██████╔╝██████╔╝
	░╚════╝░╚═╝░░╚═╝╚═╝░░╚═╝╚══════╝╚══════╝╚══════╝╚═════╝░╚═════╝░ */
	//Animate and draw tv0right
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{
		switch (stage.song_step & 7)
		{
			case 0:
				Animatable_SetAnim(&this->tv0right_animatable, 0);
				Animatable_SetAnim(&this->tv0eyeright_animatable, 0);
				Animatable_SetAnim(&this->tv0wright_animatable, 0);
				Animatable_SetAnim(&this->tv0errright_animatable, 0);
	            Animatable_SetAnim(&this->tv0bsodright_animatable, 0);
	            Animatable_SetAnim(&this->tv0dicright_animatable, 0);
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
				Animatable_SetAnim(&this->tv0eyeleft_animatable, 0);
				Animatable_SetAnim(&this->tv0wleft_animatable, 0);
				Animatable_SetAnim(&this->tv0errleft_animatable, 0);
	            Animatable_SetAnim(&this->tv0bsodleft_animatable, 0);
	            Animatable_SetAnim(&this->tv0dicleft_animatable, 0);
				break;
		}
	}


	//idle
	Animatable_Animate(&this->tv0left_animatable, (void*)this, Week2_tv0l_SetFrame);
	Week2_tv0l_Draw(this, FIXED_DEC(34,1) - fx, FIXED_DEC(37,1) - fy);
    
	Animatable_Animate(&this->tv0right_animatable, (void*)this, Week2_tv0_SetFrame);
	Week2_tv0_Draw(this, FIXED_DEC(290,1) - fx, FIXED_DEC(43,1) - fy);
	
     
	switch(stage.stage_id)
	{
	case StageId_1_2:
      
	//eye
    if (stage.song_step >= 404 && stage.song_step <= 420)
	{
	Animatable_Animate(&this->tv0eyeleft_animatable, (void*)this, Week2_tv0l_SetFrame);

	Animatable_Animate(&this->tv0eyeright_animatable, (void*)this, Week2_tv0_SetFrame);	
	}
	
    //eye
	if (stage.song_step == 760)
	{
	Animatable_Animate(&this->tv0eyeleft_animatable, (void*)this, Week2_tv0l_SetFrame);
    
	Animatable_Animate(&this->tv0eyeright_animatable, (void*)this, Week2_tv0_SetFrame);
	}

    //warning
	if (stage.song_step >= 768 && stage.song_step <= 896)
	{
	Animatable_Animate(&this->tv0wleft_animatable, (void*)this, Week2_tv0l_SetFrame);

	Animatable_Animate(&this->tv0wright_animatable, (void*)this, Week2_tv0_SetFrame);
	}

    //eye
	if (stage.song_step >= 896 && stage.song_step <= 914)
	{
	Animatable_Animate(&this->tv0eyeleft_animatable, (void*)this, Week2_tv0l_SetFrame);
    
	Animatable_Animate(&this->tv0eyeright_animatable, (void*)this, Week2_tv0_SetFrame);
	}
	break;
/*
	░█████╗░███████╗███╗░░██╗░██████╗░█████╗░██████╗░██╗░░░██╗
	██╔══██╗██╔════╝████╗░██║██╔════╝██╔══██╗██╔══██╗╚██╗░██╔╝
	██║░░╚═╝█████╗░░██╔██╗██║╚█████╗░██║░░██║██████╔╝░╚████╔╝░
	██║░░██╗██╔══╝░░██║╚████║░╚═══██╗██║░░██║██╔══██╗░░╚██╔╝░░
	╚█████╔╝███████╗██║░╚███║██████╔╝╚█████╔╝██║░░██║░░░██║░░░
	░╚════╝░╚══════╝╚═╝░░╚══╝╚═════╝░░╚════╝░╚═╝░░╚═╝░░░╚═╝░░░

	░█████╗░██╗░░░██╗███████╗██████╗░██╗░░░░░░█████╗░░█████╗░██████╗░
	██╔══██╗██║░░░██║██╔════╝██╔══██╗██║░░░░░██╔══██╗██╔══██╗██╔══██╗
	██║░░██║╚██╗░██╔╝█████╗░░██████╔╝██║░░░░░██║░░██║███████║██║░░██║
	██║░░██║░╚████╔╝░██╔══╝░░██╔══██╗██║░░░░░██║░░██║██╔══██║██║░░██║
	╚█████╔╝░░╚██╔╝░░███████╗██║░░██║███████╗╚█████╔╝██║░░██║██████╔╝
	░╚════╝░░░░╚═╝░░░╚══════╝╚═╝░░╚═╝╚══════╝░╚════╝░╚═╝░░╚═╝╚═════╝░*/

	case StageId_1_3:
	//idle
	Animatable_Animate(&this->tv0left_animatable, (void*)this, Week2_tv0l_SetFrame);
	Week2_tv0l_Draw(this, FIXED_DEC(34,1) - fx, FIXED_DEC(37,1) - fy);
    
	Animatable_Animate(&this->tv0right_animatable, (void*)this, Week2_tv0_SetFrame);
	Week2_tv0_Draw(this, FIXED_DEC(290,1) - fx, FIXED_DEC(43,1) - fy);
      
	//eye
    if (stage.song_step >= 256 && stage.song_step <= 316)
	{
	Animatable_Animate(&this->tv0eyeleft_animatable, (void*)this, Week2_tv0l_SetFrame);

	Animatable_Animate(&this->tv0eyeright_animatable, (void*)this, Week2_tv0_SetFrame);	
	}
	
    //drop
	if (stage.song_step >= 960 && stage.song_step <= 1215)
	{
	Animatable_Animate(&this->tv0dicleft_animatable, (void*)this, Week2_tv0l_SetFrame);
    
	Animatable_Animate(&this->tv0dicright_animatable, (void*)this, Week2_tv0_SetFrame);
	}

    //warning
	if (stage.song_step >= 1216 && stage.song_step <= 1728)
	{
	Animatable_Animate(&this->tv0wleft_animatable, (void*)this, Week2_tv0l_SetFrame);

	Animatable_Animate(&this->tv0wright_animatable, (void*)this, Week2_tv0_SetFrame);
	}

    //eye
	if (stage.song_step >= 2232 && stage.song_step <= 2240)
	{
	Animatable_Animate(&this->tv0eyeleft_animatable, (void*)this, Week2_tv0l_SetFrame);
    
	Animatable_Animate(&this->tv0eyeright_animatable, (void*)this, Week2_tv0_SetFrame);
	}

	//error
	if (stage.song_step >= 2808 && stage.song_step <= 2815)
	{
	Animatable_Animate(&this->tv0errleft_animatable, (void*)this, Week2_tv0l_SetFrame);
    
	Animatable_Animate(&this->tv0errright_animatable, (void*)this, Week2_tv0_SetFrame);
	}

	//bsod
	if (stage.song_step >= 2816 && stage.song_step <= 3326)
	{
	Animatable_Animate(&this->tv0bsodleft_animatable, (void*)this, Week2_tv0l_SetFrame);
    
	Animatable_Animate(&this->tv0bsodright_animatable, (void*)this, Week2_tv0_SetFrame);
	}

    //warning
	if (stage.song_step >= 3327 && stage.song_step <= 3840)
	{
	Animatable_Animate(&this->tv0wleft_animatable, (void*)this, Week2_tv0l_SetFrame);

	Animatable_Animate(&this->tv0wright_animatable, (void*)this, Week2_tv0_SetFrame);
	}

	break;
  default:
break;
}	
	//Draw bsod background
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 2790 && stage.song_step <= 2926) 
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
	if (stage.stage_id == StageId_1_3 && stage.song_step >= 2808 && stage.song_step <= 2815) 
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

void Back_Week2_Free(StageBack *back)
{
	Back_Week2 *this = (Back_Week2*)back;
	
	//Free tv0 archive
	Mem_Free(this->arc_tv0);

	//Free smoke archive
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
	this->back.draw_fg = Back_Week2_DrawFG;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Week2_DrawBG;
	this->back.free = Back_Week2_Free;
	
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
	this->arc_tv0l_ptr[0] = Archive_Find(this->arc_tv0l, "tv1.tim");
	
	//Load smoke textures
	this->arc_smoke = IO_Read("\\WEEK2\\SMOKE.ARC;1");
	this->arc_smoke_ptr[0] = Archive_Find(this->arc_smoke, "smoke.tim");
	
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

	//Initialize smoke state
	Animatable_Init(&this->smoke_animatable, smoke_anim);
	Animatable_SetAnim(&this->smoke_animatable, 0);
	this->smoke_frame = this->smoke_tex_id = 0xFF; //Force art load

	return (StageBack*)this;
}
