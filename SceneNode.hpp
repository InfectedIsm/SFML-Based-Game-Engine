#pragma once
#include <memory>		//pour l'utilisation des unique_ptr
#include <vector>		//pour l'utilisation des vector
#include <algorithm>	//pour la fonction find_if de la m�thode detachChild
#include <assert.h>
#include <SFML/Graphics.hpp>

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable //ces h�ritages ont �t� ajout� � la fin afin de rendre le Scene Node
																						//affichable comme n'importe quel objet, car chacun des noeuds seront
																					   //des objets. On pourra donc utiliser les m�thodes .draw(), etc
{
public:
	typedef std::unique_ptr<SceneNode>	Ptr;			

private:												//quand le constructeur est appel�, les deux variables du dessous sont initialis�es
	std::vector<Ptr>					mChildren;		//ce std::vector contiendra tous les enfants d'un parent
	SceneNode*							mParent;		//Ici le parent est initialis� sur un nullpointer

public:
										SceneNode();	
	void								attachChild(Ptr child);					
	Ptr									detachChild(const SceneNode& node);	//cette m�thode recherche le noeud dans l'arbre, le retire de son contenant et le 
																			//le renvoie en argument, toujours envelopp� dans le unique_ptr. Si l'argument
																			//est ignor�, le noeud est d�truit

private:
	virtual void					draw(sf::RenderTarget& target, sf::RenderStates states) const;			//ce sont des m�thodes virtuelles, cela veut dire que
	virtual void					drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;	//ces fonctions existent dans la classe m�re Drawable
																										//mais que l'on demande au compilateur d'utiliser plut�t
																										//notre m�thode lorsque l'on l'appelle pour un objet de
																									//notre classe comme pour window.draw(*node)
																									//Sinon il utiliserait la m�thode draw de sf::Drawable
};	

void SceneNode::attachChild(Ptr child)
{
	child->mParent = this;						//le mot-cl� "this" 
	mChildren.push_back(std::move(child));		//� revoir sur le pdf + internet...
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
	auto found = std::find_if(mChildren.begin(), mChildren.end(), [&](Ptr& p) -> bool {return p.get() == &node; });
						//find_if(borne_inf, borne_sup, condition)
						//ci-dessous, explication de la condition du find_if du dessus, c'est une syntaxe specifique du C++11 qui permet d'�crire une fonction 
						//en "dur" directement dans le code
						//						  [&]								(Ptr& p)		     -> bool	   {return p.get() == &node; });			
						//[ type de donn�es auquelles la fonction a acc�s ] (ParameterType Parameter) -> return type	 {corps de la fonction}

	assert(found !=  mChildren.end());		//si le find_if renvoie la fin du std::vector � found, alors c'est qu'il y a une erreur
											//voir dans la librairie RessourceHolder l'explication de assert

	Ptr result = std::move(*found);		//ici on d�place le noeud trouv� en dehors du container dans la variable result
	result->mParent = nullptr;			//on affecte � ce noeud un parent "null", il devient "orphelin" et peut �tre rattach� � un autre parent ou alors �tre parent
	mChildren.erase(found);				//on efface l'�l�ment vide du container
	return result;						//et on renvoie le pointeur contenant le noeud d�sir�
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();		//via l'op�rateur *=, on combine l'op�ration d�ja en cours sur la target (via getTransform())
										//avec une op�ration suppl�mentaire (l'argument states de la fonction). Ainsi si la target choisie �tait d�j� transform�e
										//par exemple par une rotation, on peut aussi lui ajouter un agrandissement, ou autre
	
	drawCurrent(target, states);			//on dessine ensuite la target avec l'�tat qui lui est appliqu�
	for (const Ptr& child : mChildren)		//et on fait de m�me pour tous les noeuds enfants
	{
		child->draw(target, states);
	}
	
/* 1�re fa�on d'�crire la boucle du dessus en C++ ancien, on peut voir que c'est bien moins lisible. D'o� l'int�ret d'utiliser le C++11 d�s que possible
	for (auto itr = mChildren.begin(); itr != mChildren.end(); ++itr)
	{																
		(*itr)->draw(target, states);
	}
	2eme fa�on (plus lisible d�j�)
	
	*/
}



