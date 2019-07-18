#pragma once
#include "Entity.hpp"



class Aircraft : public Entity //h�rite ainsi indirectement de SceneNode puisque Entity h�rite de SceneNode
{
public:
	enum Type
	{
		Eagle, Raptor,
	};

public:
	explicit		Aircraft(Type type);
	virtual void	drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;	//ajout� apr�s l'�criture de SceneNode

private:
	Type			mType;
	sf::Sprite		mSprite; //ajout� apr�s l'�criture de SceneNode
};

Aircraft::Aircraft(Type type) : mType(type)		// le :mType(type) veut dire que d�s que la m�thode est utilis�e, la valeur de mType est fix�e
{

}

void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}