#ifndef _COIN_
#define _COIN_

#include "Animation.h"
#include "Entity.h"

class Coin : public Entity
{

public:
	Coin(uint x, uint y);
	~Coin();

	bool Start();
	bool Update();
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	void CreateAnimationPushBacks();

public:
	
	Animation			spin;
	SDL_Texture*		coin_tex = nullptr;

};
#endif