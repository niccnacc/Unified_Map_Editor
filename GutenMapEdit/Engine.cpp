//
//  Engine.cpp
//  Protos
//
//  Created by Michael Schwegel on 03.01.13
//
//


#include "Engine.h"

#include "Sprites/Player.h"
#include "Sprites/Button.h"
#include <cmath>


void Engine::updateSprites(sf::RenderWindow &window)
{
	sf::Time timeElapsed = clock.restart();
	frameTime = timeElapsed.asMilliseconds();
	
	framePosition = window.getView().getCenter()
					- sf::Vector2f(window.getSize().x / 2,
								   window.getSize().y / 2);
	
	//DEBUGGING
	//printf("Framepos: %.f, %.f", framePosition.x, framePosition.y);
	//printf("Sprites %li \n",listOfObjects.size()); //Wie viele Objekte gibts?
	
	if(Input::instance().pressed(sf::Mouse::Left, true))
	{
		printf("Clicked without space, first = false\n");
		firstClick = false;
		rotationOffset = 400;
		if(Input::instance().heldDown(sf::Keyboard::Space))
			selectedSprite = nullptr;
	}
	
	
  	std::list<ASprite*>::iterator it;
	for (it = listOfObjects.begin(); it != listOfObjects.end(); ++it)
	{
		//***************
		//Editor Mechanic
		if(Input::instance().pressed(sf::Mouse::Left, true) &&
		   !Input::instance().heldDown(sf::Keyboard::Space))
		{
			sf::Rect<float> border = ((**it)).getGlobalBounds();
			if( border.contains(Input::instance().getMousePosition().x + framePosition.x ,
								Input::instance().getMousePosition().y + framePosition.y) )
			{
				std::cout << "Click on "<< ((**it)).getClass() <<std::endl;
				firstClick = true;
				std::cout << "first = true " << std::endl;
				
				xOffset = border.left - Input::instance().getMousePosition().x;
				yOffset = border.top - Input::instance().getMousePosition().y;
				
				selectedSprite = (*it);
				//selectedSprite->setOrigin(border.width/2, border.height/2);
			}
		}
		

		//*************
		//Game Mechanic
		
    	if((**it).checkIfInSight(framePosition))
    	{
			//listOfObjects.erase(it);
    	}
    	(**it).move(frameTime);
		(**it).update(frameTime);
		window.draw((**it));
		
	}//For ALL Sprites ^
	
	
	//***************
	//Editor Mechanic
	if(Input::instance().released(sf::Mouse::Left, true) &&
	   !firstClick)
	{
		selectedSprite = nullptr;
		printf("set nullptr\n");
	}
	
	if(selectedSprite != 0 &&
	   Input::instance().heldDown(sf::Mouse::Left, true))
	{
		if(firstClick)
		{
			selectedSprite->setPosition(Input::instance().getMousePosition().x + xOffset,
										Input::instance().getMousePosition().y + yOffset);
		}
		else
		{
			
			//Scale
			float distanceX = selectedSprite->getPosition().x - Input::instance().getMousePosition().x;
			float distanceY = selectedSprite->getPosition().y - Input::instance().getMousePosition().y;
				
			//Rotate
			bool clockwise;
			
			double dist = distanceX*distanceX + distanceY*distanceY;
			dist = sqrt(dist);
			double ankathete = Input::instance().getMousePosition().x - selectedSprite->getPosition().x;

			double angle = acos(ankathete/dist);
			angle *= 180 / 3.141592736;
			
			//Rotation Direction Check
			if(Input::instance().getMousePosition().y < selectedSprite->getPosition().y) clockwise = true;	
			if(clockwise) angle *= -1;
			
			std::cout << angle << std::endl;
			std::cout << "offset: "<< rotationOffset << std::endl;
			
			if(rotationOffset != 400)
			{
				selectedSprite->setRotation(angle + rotationOffset);
			}
			else
			{
				rotationOffset = selectedSprite->getRotation() - angle;
			}
		}
	}

	
	
} //UpdateSprites ^





void Engine::updateMenu(sf::RenderWindow &window)
{
	//Deactivate Buttons
	if((Input::instance().pressed(sf::Mouse::Left, true))
	   &&(Input::instance().getMousePosition().x < 900) )
	{
		AddButton.buttonStatus = false;
	}
	
	if(Input::instance().pressed(sf::Mouse::Left, true))
	{
		sf::Rect<float> border = AddButton.getGlobalBounds();
		if( border.contains(
							Input::instance().getMousePosition().x ,
							Input::instance().getMousePosition().y ) )
		{
			std::cout << "Button noticed Click" <<std::endl;
			AddButton.buttonStatus = true;
		}
	}
	
	AddButton.update(frameTime);
	
	
	std::list<ASprite*>::iterator it2;
	for (it2 = listOfMenuItems.begin(); it2 != listOfMenuItems.end(); ++it2)
	{
		
		((**it2)).update(frameTime);
		window.draw((**it2));
		if(Input::instance().pressed(sf::Mouse::Left, true))
		{
			sf::Rect<float> border = ((**it2)).getGlobalBounds();
			if( border.contains(
						Input::instance().getMousePosition().x ,
						Input::instance().getMousePosition().y ) )
			{
				std::cout << "Clicked Class: " << ((**it2)).getClass() <<std::endl;

				if(AddButton.buttonStatus)
				{
					if( ((**it2)).getClass() == "Player")
					{
						std::cout <<"Player"<<std::endl;
						ASprite * copy = new Player();
						copy->setPosition(450, 300);
						this->includeSprite(copy);
					}
				}
			}// if clicked ON! ^
		}
	}
	
	window.draw(AddButton);
	
	return;
	
}//Menu





void Engine::includeSprite(ASprite* figure)
{
	listOfObjects.push_back(figure);
	return;
}

void Engine::loadMenu()
{
	sf::Image img;
	img.create(200, 890, sf::Color(150,150,150));
	img.createMaskFromColor(sf::Color(200,200,200),100);
	sf::Texture * tex = new sf::Texture;
	tex->loadFromImage(img);
	ASprite* MenuBackground = new ASprite(*tex);
	MenuBackground->setPosition(995, 5);
	listOfMenuItems.push_back(MenuBackground);
	

	//std::string path = ;
	Button * but= new Button("addbutton.png");
	but->setPosition(1000, 15);
	AddButton = * but;
	
	Player* play = new Player();
	play->setPosition(995, 85);
	play->setScale(0.5, 0.5);
	listOfMenuItems.push_back(play);
	

	return;
}




std::string Engine::printfps()
{
	timesave += frameTime*10e-3;
	
	counter ++;
    if (counter >= 100)
    {
        fpsSave = counter / timesave;
        timesave = 0;
        counter = 0;
    }
    
	std::stringstream stream;
	stream << "fps: " << int(fpsSave);
	std::string erg = "fps: --";
	
	if (fpsSave > 10e-3)
	{
		erg = stream.str();
	}
	
	return erg;
}


float Engine::getTime()
{
	return frameTime;
}










