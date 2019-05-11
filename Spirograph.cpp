#include <vector>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <random>
#include <chrono>
#include <time.h>

int h = 20;
int w = 20;



int main(){
	
	// RNDN generator
	std::mt19937 mt;
	std::uniform_int_distribution<int> dist(10, 230);
	mt.seed(std::time(0));
	
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window (sf::VideoMode(700, 700), "Spirograph!", sf::Style::Default, settings);
	
	int frame_time_limit = 30;
	window.setFramerateLimit(frame_time_limit);
	
	float R = 120;
	float r = 30;
	float d = 50;
	
	// This dot "draws" the line
	sf::CircleShape center;
	center.setRadius(2);
	center.setFillColor(sf::Color::Yellow);
	center.setOrigin(1, 1);
	
	sf::CircleShape outer_circle;
	outer_circle.setRadius(R);
	outer_circle.setFillColor(sf::Color(0, 0, 0, 0));
	outer_circle.setOutlineColor(sf::Color::Blue);
	outer_circle.setOutlineThickness(1);
	outer_circle.setOrigin(R, R);
	outer_circle.setPosition(350, 350);
	
	sf::CircleShape inner_circle;
	inner_circle.setRadius(r);
	inner_circle.setFillColor(sf::Color::Black);
	inner_circle.setOutlineThickness(1);
	inner_circle.setFillColor(sf::Color(0, 0, 0, 0));
	inner_circle.setOutlineColor(sf::Color::Green);
	inner_circle.setOrigin(r, r);
	inner_circle.setPosition(350+(R-r), 350);
	
	center.setPosition(inner_circle.getOrigin());
	float angle = 0.01;
	
	// ROTATION
	float x = inner_circle.getPosition().x - 350;
	float y = inner_circle.getPosition().y - 350;
	float xTemp = inner_circle.getPosition().x;
	float yTemp = inner_circle.getPosition().y;

	sf::VertexArray lines(sf::LineStrip);
	
	float theta = 0;
	bool btn_was_released = true;
	bool pause = true;
	int red = 10, green = 10, blue = 10;
	
	center.setPosition(inner_circle.getPosition().x  + d, 350);
	
	sf::Clock clock;
	sf::Time elapsed;
	
	clock.restart();
	
	while(window.isOpen()){
		sf::Event event;
		window.clear(sf::Color::Black);
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
			}
		}
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){btn_was_released = true;}
		
		// RESET
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
			inner_circle.setPosition(350+(R-r), 350);
			center.setPosition(inner_circle.getPosition().x  + d, 350);
			x = inner_circle.getPosition().x - 350;
			y = inner_circle.getPosition().y - 350;
			xTemp = inner_circle.getPosition().x;
			yTemp = inner_circle.getPosition().y;
			angle = 0.01;
			theta = 0;
			frame_time_limit = 30;
			window.setFramerateLimit(frame_time_limit);
			lines.clear();
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && btn_was_released){
			if(pause){
				pause = false;
				btn_was_released = false;
			} else {
				pause = true;
				btn_was_released = false;
			}
		}
		// INPUT
		// Change inner circle radius
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
				r += 2;
				inner_circle.setRadius(r);
				inner_circle.setOrigin(r, r);
				inner_circle.setPosition(350+(R-r), 350);
				center.setPosition(inner_circle.getPosition().x + d, 350);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
				r -= 2;
				inner_circle.setRadius(r);
				inner_circle.setOrigin(r, r);
				inner_circle.setPosition(350+(R-r), 350);
				center.setPosition(inner_circle.getPosition().x + d, 350);
			}
		}

		// Change outer circle radius
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
				R += 2;
				
				outer_circle.setRadius(R);
				outer_circle.setOrigin(R, R);
				outer_circle.setPosition(350, 350);
				inner_circle.setPosition(350+(R-r), 350);
				center.setPosition(inner_circle.getPosition().x + d, 350);
				
			}
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
				R -= 2;
				
				outer_circle.setRadius(R);
				outer_circle.setOrigin(R, R);
				outer_circle.setPosition(350, 350);
				inner_circle.setPosition(350+(R-r), 350);
				center.setPosition(inner_circle.getPosition().x + d, 350);
			}
		
		}

		
		
		// Change length of d
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
			d += 0.5;
			center.setPosition(inner_circle.getPosition().x + d, 350);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
			d -= 0.5;
			center.setPosition(inner_circle.getPosition().x  + d, 350);
		}
		// Speed Up/Down
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && frame_time_limit < 500){
			frame_time_limit += 1;
			window.setFramerateLimit(frame_time_limit);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && frame_time_limit > 24){
			frame_time_limit -= 1;
			window.setFramerateLimit(frame_time_limit);
		}
		
		if(!pause){
			// Rotate inner circle
			xTemp = (x*std::cos(angle)) - (y*std::sin(angle));

			yTemp = (x*std::sin(angle)) + (y*std::cos(angle));
		
			inner_circle.setPosition(xTemp+350, yTemp+350);
			
			// Rotate draw point
			sf::Vector2f point;
			
			point.x = (R-r) * std::cos(theta) + d * std::cos(((R-r)/r) * theta);
			point.y = (R-r) * std::sin(theta) - d * std::sin(((R-r)/r) * theta);
			point.x += 350;
			point.y += 350;
			elapsed = clock.getElapsedTime(); 
			if(elapsed.asSeconds() >= 0.2){
				red = dist(mt);
				green = dist(mt);
				blue = dist(mt);
				elapsed = sf::milliseconds(0);
				clock.restart();
			}
			sf::Color color(red, green, blue);
			sf::Vertex v(point, color);

			lines.append(v);
			center.setPosition(point);
						
			theta += angle;
		
			if(theta == 360){
				theta = 0.0;
			}
		}

		window.draw(outer_circle);
		window.draw(inner_circle);
		window.draw(center);

		window.draw(lines);
		window.display();
		// Update the coordinates 
		x = inner_circle.getPosition().x - 350;
		y = inner_circle.getPosition().y - 350;
	}
}

