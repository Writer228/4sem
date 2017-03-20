#include <SFML\Graphics.hpp>
#include <math.h>
#include <vector>
#include "Vector2.h"

const int heght = 1024;
const int width = 728;

struct Hero
{
	Vector2 velocity;
	Vector2 position;

	void UpdatePosition(float dt)
	{
		position = position + velocity*dt;
	}
};

struct Bullet
{
	Vector2 velocity;
	Vector2 position;

	void UpdatePosition(float dt)
	{
		position = position + velocity*dt;
	}
};

struct Laser
{
	Vector2 position;
	Vector2 velocity;
	Vector2 len;
};

struct Map
{
	Vector2 size;
	Hero hero;
	std::vector<Bullet> bullets;
	Laser laser;

	void UpdatePosition(float dt)
	{
		hero.UpdatePosition(dt);
		for (auto it = bullets.begin(); it != bullets.end(); ++it)
		{
			it->UpdatePosition(dt);
		}

		if (hero.position.x > size.x)
			hero.position = Vector2(0, hero.position.y);
		if (hero.position.y > size.y)
			hero.position = Vector2(hero.position.x, 0);
		if (hero.position.x < 0)
			hero.position = Vector2(size.x, hero.position.y);
		if (hero.position.y < 0)
			hero.position = Vector2(hero.position.x, size.y);

		bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet bul) {return ((bul.position.x > heght) || (bul.position.y > width) || (bul.position.x < 0) || (bul.position.y < 0)); }), bullets.end());
	}

	
};


int main()
{
  sf::Clock clock;
  Map map;
  Bullet bull;
  Laser laser;
  int NumBull = 0;
  map.size = Vector2(heght, width);
  sf::RenderWindow window(sf::VideoMode(map.size.x, map.size.y), "My window");
  double Pretime = 0;
  double ShotTime = 0;

  map.hero.position = Vector2(map.size.x / 2, map.size.y / 2);

  sf::Texture TextureHero;
  TextureHero.loadFromFile("hero.png");
  sf::Sprite SpriteHero(TextureHero);
  sf::Texture TextureBullet;
  TextureBullet.loadFromFile("bullet.png");
  sf::Sprite SpriteBullet(TextureBullet);

  sf::RectangleShape rectangle(sf::Vector2f(0, 0));
  rectangle.setFillColor(sf::Color(0, 184, 217));

  while (window.isOpen())
  {
	  sf::Event event;
	  sf::Time time = clock.getElapsedTime();
	  double dt = time.asSeconds() - Pretime;

	  sf::Vector2u SpriteHeroSize = SpriteHero.getTexture()->getSize();
	  SpriteHero.setOrigin(SpriteHeroSize.x / 2, SpriteHeroSize.y / 2);
	  sf::Vector2u SpriteBulletSize = SpriteBullet.getTexture()->getSize();
	  SpriteBullet.setOrigin(SpriteBulletSize.x / 2, SpriteBulletSize.y / 2);

	  

	  while (window.pollEvent(event))
	  {
		  if (event.type == sf::Event::Closed)
			  window.close();
	  }

	  window.clear(sf::Color::Color(148, 0, 211, 240)); 

	  map.hero.velocity = Vector2(0, 0);
	  SpriteHero.setPosition(map.hero.position.x, map.hero.position.y);

	  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		  map.hero.velocity += Vector2(0, -300);
	  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		  map.hero.velocity += Vector2(300, 0);
	  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		  map.hero.velocity += Vector2(-300, 0);
	  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		  map.hero.velocity += Vector2(0, 300);

	  sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
	  Vector2 center = Vector2(SpriteHero.getPosition().x, SpriteHero.getPosition().y);
	  Vector2 direction = Vector2(mousePosition.x, mousePosition.y) - center;
	  SpriteHero.setRotation(90 + atan2f(direction.y, direction.x) * 180 / PI);

	  if ((sf::Mouse::isButtonPressed(sf::Mouse::Left))&&(time.asSeconds() - ShotTime > 0.5))
	  {
		  if (NumBull == 5)//rearm
		  {
			  NumBull = 0;
			  ShotTime = time.asSeconds() + 2.0;
		  }
		  else
		  {
			  ShotTime = time.asSeconds();
			  NumBull++;
		  }
		  
		  bull.position = map.hero.position + direction.norm() * 100;
		  bull.velocity = direction.norm() * 500;
		  map.bullets.push_back(bull);
	  }

	  map.laser.len = Vector2(10, 0);

	  if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	  {
		  map.laser.position = map.hero.position + direction.norm() * 100;
		  map.laser.velocity.y = 100;
		  map.laser.len.y += map.laser.velocity.y;
		  rectangle.setSize(sf::Vector2f(map.laser.len.x, map.laser.len.y));
		  rectangle.setPosition(map.laser.position.x, map.laser.position.y);
		  rectangle.setRotation(270 + atan2f(direction.y, direction.x) * 180 / PI);
		  window.draw(rectangle);
	  }

	  for (auto& bullet : map.bullets)
	  {
		  SpriteBullet.setPosition(bullet.position.x, bullet.position.y);
		  window.draw(SpriteBullet);
	  }

	  map.UpdatePosition(dt);
	  window.draw(SpriteHero);
	 
	  window.display();
	  Pretime = time.asSeconds();
  }
  return 0;
}
