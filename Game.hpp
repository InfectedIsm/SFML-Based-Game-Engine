#pragma once
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>

#include <iostream>	

class Game
{
public:
	Game();
	void	run();					//Ce sont des m�thodes qui peuvent �tre appel�es depuis l'ext�rieur de la classe
	void	init();

private:							//Ce sont les m�thodes qui ne peuvent �tre appel�es que dans la classe Game et pas en dehors
	void	processEvents();
	void	update(sf::Time deltaTime);
	void	render();
	void	handlePlayerInput(sf::Keyboard::Key key, bool IsPressed);	//cette m�thode r�cup�re la touche appuy�e et attribue un bool�en � la variable
																		//affect� a la touche (voir les variable private plus bas)
public:

private:							//Ce sont des variables qui ne peuvent �tre accessibles et visibles que par la classe Game
	sf::RenderWindow	mWindow;
	sf::Texture			mTexture;
	sf::Sprite			mPlayer;

	//update
	bool mIsMovingUp, mIsMovingDown, mIsMovingLeft, mIsMovingRight;		//bool�ens affect�s aux touches de clavier
	float playerSpeed;

	//run
	float fps;
};

/////////INIT////////
void Game::init()
{
	fps = (1.f / 60.f);
	mIsMovingUp, mIsMovingDown, mIsMovingLeft, mIsMovingRight = false;
	playerSpeed = 1.f;

}
/////////GAME SET////////
Game::Game()
	: mWindow(sf::VideoMode(640, 480), "SFML Application"), mTexture(), mPlayer() //c'est de cette fa�on que l'on initialise les variables priv�es d'une classe
																				  //en mettant classe::classe():priv_var1,priv_var2, ..., priv_varN { }
{
	if (!mTexture.loadFromFile("Media/Textures/Starship1.png"))  //la m�thode loadFromFile renvoie True si la texture a bien �t� charg�e
	{														 // /!\ : Les dossiers � charger doivent �tre plac�s dams Projects/nom_projet/nom_projet
															 //gestion de l'erreur
	}
	mPlayer.setTexture(mTexture);
	mPlayer.setPosition(100.f, 100.f);

}

/////////RUN////////
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;		//initialise la variable timeSinceLastUpdate de classe Time � la valeur 0
	sf::Time TimePerFrame = sf::seconds(fps);	//FPS d�sir�s = 60, il faut donc 60 frames par secondes, soit une frame = 1/60s

	while (mWindow.isOpen())
	{
		timeSinceLastUpdate += clock.restart();			//a chaque entr�e dans la m�thode Game::run() on incr�mente la variable du temps �coul�
		while (timeSinceLastUpdate > TimePerFrame)		//quand enfin timeSinceLastUpdate est �gale ) 1/60s, on peut actualiser le jeu
		{
			processEvents();
			update(timeSinceLastUpdate);
			render();
		}

	}
}
/////////HANDLE INPUT////////
void Game::handlePlayerInput(sf::Keyboard::Key key, bool IsPressed)
{
	if (key == sf::Keyboard::Up)
		mIsMovingUp = IsPressed;
	else if (key == sf::Keyboard::Down)
		mIsMovingDown = IsPressed;
	else if (key == sf::Keyboard::Left)
		mIsMovingLeft = IsPressed;
	else if (key == sf::Keyboard::Right)
		mIsMovingRight = IsPressed;
}

/////////PROCESS////////
void Game::processEvents()						//la fonction processEvents a pour but de sonder les �v�nements provenant de Windows (clavier, souris, etc)
{												//et d'agir en fonction
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);	//si une touche est appuy�e, alors on affecte 'true' au bool�en affect� � la touche
			break;										//(voir la fonction handlePlayerInput juste au dessus)

		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);	//Dans le cas inverse, on lui affecte la valeur 'false'
			break;										//Ainsi, via ces bool�ens accessible par les autres fonctions de la classe, on pourra savoir
														//a tout moment l'�tat des diff�rentes touches
		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}

/////////UPDATE////////
void Game::update(sf::Time deltaTime)	//la fonction update M�J la logique du jeu, elle intervient � chaque �v�nemment dans le jeu
{										//Elle est remplie au fur et � mesure que les fonctionnalit�s du jeu apparaissent
	sf::Vector2f movement(0.f, 0.f);
	float corrected_playerSpeed = 0;

	if ((mIsMovingRight || mIsMovingLeft) && (mIsMovingDown || mIsMovingUp))	//v�rifie si un d�placement en diagonale est effectu�
		corrected_playerSpeed = playerSpeed / 1.4142;							//si tel est le cas, la vitesse est divis�e par racine de 2
	else 																		//ainsi la vitesse de d�placement diagonale est la m�me
		corrected_playerSpeed = playerSpeed;

	if (mIsMovingUp)					//en mettant des if et pas des else if, on rend possible la prise en compte simultan�e de plusieurs touche	
		movement.y -= corrected_playerSpeed;		//lors d'une frame
	if (mIsMovingDown)
		movement.y += corrected_playerSpeed;
	if (mIsMovingLeft)
		movement.x -= corrected_playerSpeed;
	if (mIsMovingRight)
		movement.x += corrected_playerSpeed;


	mPlayer.move(movement * 30.f * deltaTime.asSeconds());
	std::cout << "(" << movement.x << " ; " << movement.y << " ; " << deltaTime.asSeconds() << ")" << "\r";


}

/////////RENDER////////
void Game::render()				//la fonction permet de rendre � l'�cran le contenu du jeu. Elle agit en 3 temps :
{
	mWindow.clear();			//1) elle colore le fond (ici noir) et efface tous les anciens ojets
	mWindow.draw(mPlayer);		//2) elle desinne tous les nouveaux objets

	mWindow.display();			//3) elle les affiche ) l'�cran
}