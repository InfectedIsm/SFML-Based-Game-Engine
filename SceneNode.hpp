#pragma once
#include <memory>		//pour l'utilisation des unique_ptr
#include <vector>		//pour l'utilisation des vector
#include <algorithm>	//pour la fonction find_if de la méthode detachChild
#include <assert.h>
#include <SFML/Graphics.hpp>

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable //ces héritages ont été ajouté à la fin afin de rendre le Scene Node
																						//affichable comme n'importe quel objet, car chacun des noeuds seront
																					   //des objets. On pourra donc utiliser les méthodes .draw(), etc
{
public:
	typedef std::unique_ptr<SceneNode>	Ptr;			

private:												//quand le constructeur est appelé, les deux variables du dessous sont initialisées
	std::vector<Ptr>					mChildren;		//ce std::vector contiendra tous les enfants d'un parent
	SceneNode*							mParent;		//Ici le parent est initialisé sur un nullpointer

public:
										SceneNode();	
	void								attachChild(Ptr child);					
	Ptr									detachChild(const SceneNode& node);	//cette méthode recherche le noeud dans l'arbre, le retire de son contenant et le 
																			//le renvoie en argument, toujours enveloppé dans le unique_ptr. Si l'argument
																			//est ignoré, le noeud est détruit

private:
	virtual void					draw(sf::RenderTarget& target, sf::RenderStates states) const;			//ce sont des méthodes virtuelles, cela veut dire que
	virtual void					drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;	//ces fonctions existent dans la classe mère Drawable
																										//mais que l'on demande au compilateur d'utiliser plutôt
																										//notre méthode lorsque l'on l'appelle pour un objet de
																									//notre classe comme pour window.draw(*node)
																									//Sinon il utiliserait la méthode draw de sf::Drawable
};	

void SceneNode::attachChild(Ptr child)
{
	child->mParent = this;						//le mot-clé "this" 
	mChildren.push_back(std::move(child));		//à revoir sur le pdf + internet...
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
	auto found = std::find_if(mChildren.begin(), mChildren.end(), [&](Ptr& p) -> bool {return p.get() == &node; });
						//find_if(borne_inf, borne_sup, condition)
						//ci-dessous, explication de la condition du find_if du dessus, c'est une syntaxe specifique du C++11 qui permet d'écrire une fonction 
						//en "dur" directement dans le code
						//						  [&]								(Ptr& p)		     -> bool	   {return p.get() == &node; });			
						//[ type de données auquelles la fonction a accès ] (ParameterType Parameter) -> return type	 {corps de la fonction}

	assert(found !=  mChildren.end());		//si le find_if renvoie la fin du std::vector à found, alors c'est qu'il y a une erreur
											//voir dans la librairie RessourceHolder l'explication de assert

	Ptr result = std::move(*found);		//ici on déplace le noeud trouvé en dehors du container dans la variable result
	result->mParent = nullptr;			//on affecte à ce noeud un parent "null", il devient "orphelin" et peut être rattaché à un autre parent ou alors être parent
	mChildren.erase(found);				//on efface l'élément vide du container
	return result;						//et on renvoie le pointeur contenant le noeud désiré
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();		//via l'opérateur *=, on combine l'opération déja en cours sur la target (via getTransform())
										//avec une opération supplémentaire (l'argument states de la fonction). Ainsi si la target choisie était déjà transformée
										//par exemple par une rotation, on peut aussi lui ajouter un agrandissement, ou autre
	
	drawCurrent(target, states);			//on dessine ensuite la target avec l'état qui lui est appliqué
	for (const Ptr& child : mChildren)		//et on fait de même pour tous les noeuds enfants
	{
		child->draw(target, states);
	}
	
/* 1ère façon d'écrire la boucle du dessus en C++ ancien, on peut voir que c'est bien moins lisible. D'où l'intéret d'utiliser le C++11 dès que possible
	for (auto itr = mChildren.begin(); itr != mChildren.end(); ++itr)
	{																
		(*itr)->draw(target, states);
	}
	2eme façon (plus lisible déjà)
	
	*/
}



