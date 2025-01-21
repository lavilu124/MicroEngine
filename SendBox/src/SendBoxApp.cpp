#include <SFML/Graphics.hpp>
#include "LightInclude.h"

int main() {

	sf::RenderWindow w(sf::VideoMode(1920, 1080), "app");

	Micro::RadialLight light;
	light.setRange(150);
	light.setColor(sf::Color::Blue);


	Micro::EdgeVector edges;
	edges.emplace_back(sf::Vector2f(200.f, 100.f),
		sf::Vector2f(200.f, 300.f));


	while (w.isOpen()) {
		sf::Event e;
		while (w.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				w.close();
			}
			else if (e.type == sf::Event::MouseMoved) {
				sf::Vector2f mp(sf::Mouse::getPosition(w));
				light.setPosition(mp);
				light.castLight(edges.begin(), edges.end());
			}
		}

		w.clear();
		w.draw(light);
		w.display();
	}
	return 0;
}