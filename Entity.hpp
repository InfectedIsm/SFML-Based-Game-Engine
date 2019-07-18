#pragma once
#include <SFML/Graphics.hpp>
#include "SceneNode.hpp"

class Entity : public SceneNode  //cet h�ritage a �t� ajout� apr�s l'�criture de SceneNode, afin de rendre les entit�s affichable via l'affichabilit� de SceneNode
{
	public:
		void			setVelocity(sf::Vector2f velocity);		//2 m�thodes pour r�gler la vitesse du vaisseau
		void			setVelocity(float vx, float vy);
		sf::Vector2f	getVelocity() const;				//un get pour connaitre la vitesse du vaisseau, const veut dire qu'on ne peut pas modifier la valeur

	private:
		sf::Vector2f	mVelocity;							//la vitesse sera stock�e ici
};
															//Ainsi, lorsque l'on cr�era un vaisseau, il sera de classe Entity et on pourra alors
void Entity::setVelocity(sf::Vector2f velocity)				//r�gler ses diff�rents param�tres (vitesse, position, etc selon les m�thodes cr��es)
{
	mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
	mVelocity.x = vx;
	mVelocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const
{
	return mVelocity;
}









