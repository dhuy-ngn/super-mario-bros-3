#pragma once

#include "Scene.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Mario.h"
#include "Game.h"
#include <dinput.h>

#define SCENE_SECTION_UNKNOWN		-1
#define SCENE_SECTION_SPRITES		3
#define SCENE_SECTION_ANIMATIONS	4
#define SCENE_SECTION_OBJECTS		6

#define MAX_SCENE_LINE 1024
#define ID_SPRITE_BACKGROUND	50003
#define	ID_SPRITE_GROUND		50001
#define ID_SPRITE_ARROW			50010
#define ID_ANI_THREE			5000

#define THREE_X	135
#define THREE_Y	115
#define ARROW_X	74
#define ARROW_Y 147

#define GROUND_WIDTH	256

#define SCROLLING_TIME	2000
#define SWITCH_TIME		300

#define WORLD_SCENE_ID	1
#define PLAY_SCENE_ID	5

#define INTRO_SCENE_STATE_ARROW_UP		0
#define INTRO_SCENE_STATE_ARROW_DOWN	1

class CIntroScene : public CScene {
protected:
	vector<LPGAMEOBJECT> objects;
	LPSPRITE background;
	LPANIMATION three;
	LPSPRITE arrow;
	ULONGLONG switch_scene_start;
	BOOLEAN isSwitch = false;
	BOOLEAN isArrowUp = true;

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
public:
	CIntroScene(int id, LPCWSTR filePath);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_OBJECTS(string line);
	void SetState(int state);
};

class CIntroSceneKeyHandler : public CSceneKeyHandler
{
public:
	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CIntroSceneKeyHandler(LPSCENE s) : CSceneKeyHandler(s) {};
};