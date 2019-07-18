// SMFL_cours2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <memory>		//pour l'utilisation des unique_ptr
#include <assert.h>

#include "RessourceHolder.hpp" //classe de gestion des ressources (chap 2 du PDF)
#include "Game.hpp"


int main()
{
	Game game;
	game.init();
	game.run();
}

