#pragma once
#include <SFML/Graphics.hpp>
#include "SceneNode.hpp"

class Entity : public SceneNode  //cet héritage a été ajouté après l'écriture de SceneNode, afin de rendre les entités affichable via l'affichabilité de SceneNode
{
	public:
		void			setVelocity(sf::Vector2f velocity);		//2 méthodes pour régler la vitesse du vaisseau
		void			setVelocity(float vx, float vy);
		sf::Vector2f	getVelocity() const;				//un get pour connaitre la vitesse du vaisseau, const veut dire qu'on ne peut pas modifier la valeur

	private:
		sf::Vector2f	mVelocity;							//la vitesse sera stockée ici
};
															//Ainsi, lorsque l'on créera un vaisseau, il sera de classe Entity et on pourra alors
void Entity::setVelocity(sf::Vector2f velocity)				//régler ses différents paramètres (vitesse, position, etc selon les méthodes créées)
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









