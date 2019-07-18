 #pragma once

#include <memory>		//pour l'utilisation des unique_ptr
#include <assert.h>



///////////// listes des diff�rents ID n�cessaires///////////// 
namespace Identifier
{
	enum ID { Texture, Sound, Font };
}

namespace Textures    //Textures avec un s, � diff�rencier du Texture du dessus
{
	enum ID {	Landscape, Desert,
				Eagle, Raptor, 
				Missile
			};			
}
/////////////////////////////////////////////////////////////// 



template <typename Ressource, typename Identifier>		//Ressource Holder est une classe contenant des "map" dont chaque �l�ment comprend
class RessourceHolder									// une ressource et un ID
{
	public:
		void				load(Identifier id, const std::string& filename);

		//surcharge du load
		template <typename Parameter>
		void				load(Identifier id, const std::string& filename, const Parameter& secondParam); //le fait de mettre Parameter& nous permet
																//de pouvoir placer n'importe quel type ici. Par exemple, les shaders peuvent avoir comme
																//second param�tre soit un sf::Shader, soit un std::string, ainsi on g�re les deux
													//De plus, sf::Texture dispose d'une seconde m�thode qui permet de ne prendre qu'une partie d'une 
													//image, on fait d'une pierre deux coups (voir p. 46 du PDF)

		Ressource&			get(Identifier id);				//get permettant de r�cup�rer une ressource � partir de son ID (voir plus bas)

		const Ressource&	get(Identifier id) const;	//m�me fonction membre qu'au dessus, mais avec const avant
														//const en C++ permet d'indiquer au compilateur ou a l'utilisateur que la ressource
														//n'est pas modifiable ou ne modifie pas
														//Ici Ressource est une adresse (&) (const � la fin) qui renvoie une Ressource const (const au d�but)

	private:
		std::map<Ressource,				//std::map est une variable contenant 2 objets (ressource et unique_ptr) et s'appelle mRessourceMap 
			std::unique_ptr<Ressource>> mRessourceMap;				//les unique_ptr et map sont une mani�re de g�rer la m�moire de fa�on "optimale"
																	//(faire une recherche google pour comprendre)
};


/////////LOAD////////	
template <typename Ressource, typename Identifier>
void RessourceHolder<Ressource, Identifier>::load(Identifier id, const std::string& filename)
{
	std::unique_ptr<Ressource> ressource(new Ressource());		//on cr�e un objet Ressource nomm� 'ressource' que l'on store dans le unique_ptr
	if (!ressource->loadFromFile(filename))								//ici ressource est un pointeur d'o� le ->, on charge le fichier dans ressource
	{
		throw std::runtime_error("RessourceHolder::load - Failed to load" + filename);		//gestion de l'erreur via un runtime_error	
		auto inserted = mRessourceMap.insert(std::make_pair(id, std::move(ressource)));  //ici on save la variable qui a cr�� l'erreur (??)
		assert(inserted.second);								//fonction utile en mode debug, cr�� un breakpoint si False, voir internet
	}
	mRessourceMap.insert(std::make_pair(id, std::move(ressource)));		//make_pair permet de faire en sorte que id et std::move(ressource) soit convertis en
																	//Ressource et std::unique_ptr<Ressource> (voir sur internet)
};

////////LOAD surcharg� pour shaders///////
template <typename Ressource, typename Identifier>
template <typename Parameter>
void RessourceHolder<Ressource, Identifier>::load(Identifier id, const std::string& filename, const Parameter& secondParam)
{
	std::unique_ptr<Ressource> ressource(new Ressource());		
	if (!ressource->loadFromFile(filename, secondParam))		
	{
		throw std::runtime_error("RessourceHolder::load - Failed to load" + filename);	
		auto inserted = mRessourceMap.insert(std::make_pair(id, std::move(ressource))); 
		assert(inserted.second);								
	}
	mRessourceMap.insert(std::make_pair(id, std::move(ressource)));
																	
}

/////////GET////////
template <typename Ressource, typename Identifier>
Ressource& RessourceHolder<Ressource, Identifier>::get(Identifier id)
{
	auto found = mRessourceMap.find(id);  //la fonction find est un it�rateur qui recherche dans la map l'id et renvoie l'�l�ment (ou end() si non trouv�)
										//le type auto est un type sp�cial du C++11 qui d�duit tout seul le type de la variable
	assert(found != mRessourceMap.end());			//si le find arrive au bout de la liste sans rien trouver, c'est que la ressource n'est pas stock�e
	return *found->second;			//une fois l'id trouv�, on retourne son SECOND �l�ment, d'o� le ->second 
									//(il ne faut pas oublier qu'en C++ 'classe.methode' pour les pointeurs s'�crit 'classe->methode')
}

//////Classe Texture sur laquelle est bas�e la g�n�ralisation Ressource
/*
class TextureHolder
{
private:
	std::map<Textures::ID,				//std::map est une variable contenant 2 objets (textures::id et unique_ptr) et s'appelle mTextureMap 
		std::unique_ptr<sf::Texture>> mTextureMap;				//les unique_ptr et map sont une mani�re de g�rer la m�moire de fa�on "optimale"
																//(faire une recherche google pour comprendre)

public:
	void				load(Textures::ID id, const std::string& filename);

	sf::Texture&		get(Textures::ID id);				//get permettant de r�cup�rer une texture � partir de son ID (voir plus bas)

	const sf::Texture&	get(Textures::ID id) const;	//m�me fonction membre qu'au dessus, mais avec const avant
													//const en C++ est comme un define, ce constructeur permet de rendre cette texture
													//charg�e non modifiable une fois qu'elle est cr��e
};
/////////LOAD////////
void TextureHolder::load(Textures::ID id, const std::string& filename)
{
	std::unique_ptr<sf::Texture> texture(new sf::Texture());		//on cr�e un objet sf::Texture nomm� 'texture' que l'on store dans le unique_ptr
	if (!texture->loadFromFile(filename))								//ici texture est un pointeur d'o� le ->, on charge le fichier dans texture
	{
		throw std::runtime_error("TextureHolder::load - Failed to load" + filename);		//gestion de l'erreur via un runtime_error	
		auto inserted = mTextureMap.insert(std::make_pair(id, std::move(texture)));  //ici on save la variable qui a cr�� l'erreur (??)
		assert(inserted.second);								//fonction utile en mode debug, cr�� un breakpoint si False, voir internet
	}
	mTextureMap.insert(std::make_pair(id, std::move(texture)));		//make_pair permet de faire en sorte que id et std::move(texture) soit convertis en
																	//Texture::ID et std::unique_ptr<sf::Texture (voir sur internet)
}
;
/////////GET////////
sf::Texture& TextureHolder::get(Textures::ID id)
{
	auto found = mTextureMap.find(id);  //la fonction find est un it�rateur qui recherche dans la map l'id et renvoie l'�l�ment (ou end() si non trouv�)
										//le type auto est un type sp�cial du C++11 qui d�duit tout seul le type de la variable
	assert(found != mTextureMap.end());			//si le find arrive au bout de la liste sans rien trouver, c'est que la texture n'est pas stock�e
	return *found->second;			//une fois l'id trouv�, on retourne son SECOND �l�ment, d'o� le ->second 
									//(il ne faut pas oublier qu'en C++ 'classe.methode' pour les pointeurs s'�crit 'classe->methode')
}
*/