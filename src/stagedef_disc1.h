	{ //StageId_1_1 (Carefree)
		//Characters
		{Char_BF_New,    FIXED_DEC(60,1),  FIXED_DEC(100,1)},
		{Char_Dad_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{Char_GF_New,     FIXED_DEC(0,1),    FIXED_DEC(0,1)},
		
		//Stage background
		Back_Week4_New,
		
		//Song info
		{FIXED_DEC(11,10),FIXED_DEC(14,10),FIXED_DEC(15,10)},
		1, 1,
		XA_Bopeebo, 0,
		
		StageId_1_2, STAGE_LOAD_FLAG
	},
	{ //StageId_1_2 (Careless)
		//Characters
		{Char_BF_New,    FIXED_DEC(-600,1),  FIXED_DEC(100,1)},
		{Char_qtm_New, FIXED_DEC(120,1),  FIXED_DEC(100,1)},
		{Char_GF_New,     FIXED_DEC(-1000,1),    FIXED_DEC(0,1)},
		
		//Stage background
		Back_Week2_New,
		
		//Song info
		{FIXED_DEC(12,10),FIXED_DEC(14,10),FIXED_DEC(17,10)},
		1, 2,
		XA_Fresh, 2,
		
		StageId_1_3, STAGE_LOAD_FLAG
	},
	{ //StageId_1_3 (Censory-Overload)
		//Characters
		{Char_BF_New,    FIXED_DEC(60,1),  FIXED_DEC(100,1)},
		{Char_bot_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{Char_GF_New,     FIXED_DEC(0,1),    FIXED_DEC(0,1)},
		
		//Stage background
		Back_Week2_New,
		
		//Song info
		{FIXED_DEC(21,10),FIXED_DEC(24,10),FIXED_DEC(29,10)},
		1, 3,
		XA_Dadbattle, 0,
		
		StageId_1_3, 0
	},
	{ //StageId_1_4 (NULL)
		//Characters
		{Char_BF_New,   FIXED_DEC(60,1),  FIXED_DEC(100,1)},
		{Char_Dad_New,   FIXED_DEC(0,1),  FIXED_DEC(-15,1)}, //TODO
		{Char_GF_New,    FIXED_DEC(0,1),    FIXED_DEC(0,1)},
		
		//Stage background
		Back_Week4_New,
		
		//Song info
		{FIXED_DEC(1,1),FIXED_DEC(1,1),FIXED_DEC(1,1)},
		1, 4,
		XA_Tutorial, 2,
		
		StageId_1_4, 0
	},
	
	{ //StageId_2_1 (Terminate)
		//Characters
		{Char_BF_New,      FIXED_DEC(56,1),   FIXED_DEC(85,1)},
		{Char_Dad_New,  FIXED_DEC(-90,1),   FIXED_DEC(85,1)},
		{Char_GF_New,       FIXED_DEC(0,1),  FIXED_DEC(-15,1)},
		
		//Stage background
		Back_Week2_New,
		
		//Song info
		{FIXED_DEC(28,10),FIXED_DEC(29,10),FIXED_DEC(3,1)},
		2, 1,
		XA_Spookeez, 0,
		
		StageId_2_2, STAGE_LOAD_FLAG
	},
	{ //StageId_2_2 (Termination)
		//Characters
		{Char_BF_New,      FIXED_DEC(56,1),   FIXED_DEC(85,1)},
		{Char_Dad_New, FIXED_DEC(-90,1),   FIXED_DEC(85,1)},
		{Char_GF_New,       FIXED_DEC(0,1),  FIXED_DEC(-15,1)},
		
		//Stage background
		Back_Week2_New,
		
		//Song info
		{FIXED_DEC(31,10),FIXED_DEC(31,10),FIXED_DEC(31,10)},
		2, 2,
		XA_South, 2,
		
		StageId_2_3, STAGE_LOAD_FLAG | STAGE_LOAD_OPPONENT
	},
	{ //StageId_2_3 (Cessation)
		//Characters
		{Char_BF_New,      FIXED_DEC(56,1),   FIXED_DEC(85,1)},
		{Char_Dad_New,  FIXED_DEC(-90,1),   FIXED_DEC(85,1)},
		{Char_GF_New,       FIXED_DEC(0,1),  FIXED_DEC(-15,1)},
		
		//Stage background
		Back_Week2_New,
		
		//Song info
		{FIXED_DEC(15,10),FIXED_DEC(15,10),FIXED_DEC(15,10)},
		2, 3,
		XA_Monster, 0,
		
		StageId_2_3, 0
	},