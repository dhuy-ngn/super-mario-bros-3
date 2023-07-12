#pragma once

#include "GameObject.h"
#include "Mario.h"
#include <map>

#define FONT_BBOX_WIDTH						8
#define DEFAULT_TIME						300

#define SPRITE_HUD_ID						99999
#define SPRITE_ICONMARIO_ID					99998
#define SPRITE_FILLARROW_ID					50005
#define ANI_P_ID							5006

#define SPRITE_FONT_0_ID				50013
#define SPRITE_FONT_1_ID				50014
#define SPRITE_FONT_2_ID				50015
#define SPRITE_FONT_3_ID				50016
#define SPRITE_FONT_4_ID				50017
#define SPRITE_FONT_5_ID				50018
#define SPRITE_FONT_6_ID				50019
#define SPRITE_FONT_7_ID				50020
#define SPRITE_FONT_8_ID				50021
#define SPRITE_FONT_9_ID				50022
#define SPRITE_FONT_A_ID				50023
#define SPRITE_FONT_B_ID				50024
#define SPRITE_FONT_C_ID				50025
#define SPRITE_FONT_D_ID				50026
#define SPRITE_FONT_E_ID				50027
#define SPRITE_FONT_F_ID				50028
#define SPRITE_FONT_G_ID				50029
#define SPRITE_FONT_H_ID				50030
#define SPRITE_FONT_I_ID				50031
#define SPRITE_FONT_J_ID				50032
#define SPRITE_FONT_K_ID				50033
#define SPRITE_FONT_L_ID				50034
#define SPRITE_FONT_M_ID				50035
#define SPRITE_FONT_N_ID				50036
#define SPRITE_FONT_O_ID				50037
#define SPRITE_FONT_P_ID				50038
#define SPRITE_FONT_Q_ID				50039
#define SPRITE_FONT_R_ID				50040
#define SPRITE_FONT_S_ID				50041
#define SPRITE_FONT_T_ID				50042
#define SPRITE_FONT_U_ID				50043
#define SPRITE_FONT_V_ID				50044
#define SPRITE_FONT_W_ID				50045
#define SPRITE_FONT_X_ID				50046
#define SPRITE_FONT_Y_ID				50047
#define SPRITE_FONT_Z_ID				50048

#define HUD_DIFF_P				15
#define HUD_DIFF_ROW			4	
#define HUD_DIFF_METTER			68
#define HUD_DIFF_FIRST_ROW		8
#define HUD_DIFF_SECOND_ROW		16
#define HUD_DIFF_PLAYER			110
#define HUD_DIFF_TIME			5
#define HUD_DIFF_MONEY			12
#define HUD_DIFF_LIFE			93
#define HUD_DIFF_SCORE			60

#define HUD_TIME_MAX	3
#define HUD_SCORE_MAX	7

#define HUD_HEIGHT 32

#pragma once
class CHud : public CGameObject
{
public:
	std::map<char, LPSPRITE> fonts;
	int speed_stack = 0;
	int money = 0;
	int time = 0;
	int remainTime = DEFAULT_TIME;
	int marioLife = 4;
	int score = 0;
	LPANIMATION PAni = NULL;

	LPSPRITE playerSprite = NULL;
	vector<LPSPRITE> moneySprites;
	vector<LPSPRITE> remainTimeSprites;
	vector<LPSPRITE> mariolifeSprites;
	vector<LPSPRITE> scoreSprites;
	vector<LPSPRITE> powerMeterSprite;

	virtual void Update(DWORD dt);
	CHud();
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	void GetMarioSpeedStack();
	void GetMarioCoin();
	void GetMarioLife();
	void GetMarioScore();
	void GetMarioRemainingTime();
	void initFonts();
	LPSPRITE mappingFont(char c);
	vector<LPSPRITE> StringToSprite(string str);
};

