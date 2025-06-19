#include "Template.h"

REGISTER_GAMEOBJECT(Template);

Template::Template (Micro::SystemManager* systemManager, const sf::Sprite& sprite, const std::string& name, Micro::Collision::collisionLayer layer, int level)
    : GameObject(systemManager, sprite, name, layer, level)
{	

}



void Template::Start(){

} 

void Template::Update(float DeltaTime){
    
}
    
	
 
