#pragma once
#include "Entity.hpp"



class Aircraft : public Entity //hérite ainsi indirectement de SceneNode puisque Entity hérite de SceneNode
{
public:
	enum Type
	{
		Eagle, Raptor,
	};

public:
	explicit		Aircraft(Type type);
	virtual void	drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;	//ajouté après l'écriture de SceneNode

private:
	Type			mType;
	sf::Sprite		mSprite; //ajouté après l'écriture de SceneNode
};

Aircraft::Aircraft(Type type) : mType(type)		// le :mType(type) veut dire que dès que la méthode est utilisée, la valeur de mType est fixée
{

}

void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}