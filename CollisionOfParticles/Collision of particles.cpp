#include <SFML\Graphics.hpp>
#define M_PI       3.14159265358979323846
#include <math.h>
#include "Vector2.h"
#include <vector>
#include<iostream>

const int heght = 1024;
const int width = 728;

class MaterialPoint
{
public:
	MaterialPoint()
	{
		position = Vector2(0, 0);
		velosity = Vector2(0, 0);
		acceleration = Vector2(0, 0);

		mass = rand() % 1 + 100;
	}

	
	Vector2 GetImpulse(MaterialPoint a) const
	{
		return velosity*mass;
	}
	void UpdatePosition(float dt)
	{
		position += velosity * dt;
	}

	Vector2 position;
	Vector2 velosity;
	Vector2 acceleration;
	float mass;
};

class Ball : public MaterialPoint
{
public:
	float radius;
	int type;
};

Vector2 GetCentre(sf::Texture a, float b)
{
	sf::Vector2u size = a.getSize();
	return Vector2(size.x*b / 2, size.y / 2 * b);
}

struct Map
{
	Vector2 size;
	std::vector<Ball> balls;
	Vector2 dp;
	Vector2 d;

	void UpdatePosition(float dt, Vector2 BallSize)
	{
		for (int i = 0; i != balls.size(); ++i)
		{
			balls[i].UpdatePosition(dt);

			if ((balls[i].position.x > size.x - BallSize.x / 2)&&(balls[i].velosity.x > 0))
				balls[i].velosity = Vector2(balls[i].velosity.x*(-1), balls[i].velosity.y);
			if ((balls[i].position.y > size.y - BallSize.y / 2) && (balls[i].velosity.y > 0))
				balls[i].velosity = Vector2(balls[i].velosity.x, balls[i].velosity.y*(-1));
			if ((balls[i].position.x < 0 + BallSize.x / 2) && (balls[i].velosity.x < 0))
				balls[i].velosity = Vector2(balls[i].velosity.x*(-1), balls[i].velosity.y);
			if ((balls[i].position.y < 0 + BallSize.y / 2) && (balls[i].velosity.y < 0))
				balls[i].velosity = Vector2(balls[i].velosity.x, balls[i].velosity.y*(-1));

			for (int j = 0; j != balls.size(); ++j)
			{
				if (i != j)
				{
					d = balls[j].position - balls[i].position;
					if (((d) * (d)/4 < (balls[i].radius * balls[i].radius)) && ((balls[j].velosity - balls[i].velosity) * (d) <= 0))
					{
						dp = 2 * (((balls[i].velosity - balls[j].velosity) / (1 / balls[i].mass + 1 / balls[j].mass) * d.norm()) * d.norm());
						balls[j].velosity = balls[j].velosity + dp / balls[j].mass;
						balls[i].velosity = balls[i].velosity - dp / balls[i].mass;
					}
				}
			}
		}

	}
};

int main()
{
	sf::Clock clock;
	Map map;
	Ball ball;
	map.size = Vector2(heght, width);
	sf::RenderWindow window(sf::VideoMode(map.size.x, map.size.y), "My window");
	double Pretime = 0;
	double Pulltime = 0;

	sf::Texture Textureball;
	Textureball.loadFromFile("Ball-green-128.png");
	sf::Sprite circle(Textureball);
	circle.setScale(0.2f, 0.2f);
	sf::Vector2u circleSize = circle.getTexture()->getSize();
	Vector2 BallSize = Vector2(circleSize.x*0.2, circleSize.y*0.2);
	circle.setOrigin(circleSize.x / 2, circleSize.y / 2);

	while (window.isOpen())
	{
		sf::Time time = clock.getElapsedTime();
		double dt = time.asSeconds() - Pretime;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::Color(148, 0, 211, 240));

		if ((sf::Mouse::isButtonPressed(sf::Mouse::Left)) && (time.asSeconds() - Pulltime > 0.2))
		{
			Pulltime = time.asSeconds();
			sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

			Vector2 centre = GetCentre(Textureball, 0.2f);
			ball.position = Vector2(mousePosition.x, mousePosition.y);
			ball.velosity = Vector2(rand() % 201 + (-100), rand() % 201 + (-100));
			ball.radius = (BallSize.x / 2);
			map.balls.push_back(ball);

			circle.setPosition(mousePosition.x, mousePosition.y);

		}

		for (auto& i : map.balls)
		{
			circle.setPosition(i.position.x, i.position.y);
			window.draw(circle);
		}

		
		map.UpdatePosition(dt, BallSize); 
		window.display();
		Pretime = time.asSeconds();
	}
	return 0;
}
