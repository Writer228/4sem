#include <SFML\Graphics.hpp>
#include <math.h>
#include <vector>
#include <iostream>

using Centroids = std::vector<sf::Color>;
using ClosestCentroidIndices = std::vector<int>;

const int IterationCount = 30;
const int K = 1;

struct ColorBuf
{
	int count = 0;
	long int r, g, b;
	
	ColorBuf& operator +=(const sf::Color& c)
	{
		count++;
		r = r + c.r;
		g = g + c.g;
		b = b + c.b;

		return *this;
	}
};


int Distanse(sf::Color x, sf::Color y)
{
	return ((x.r - y.r) * (x.r - y.r) + (x.g - y.g) * (x.g - y.g) + (x.b - y.b) * (x.b - y.b));
};

Centroids initializeCentroids(sf::Image image, int k)
{
	Centroids result;
	sf::Vector2u photoSize = image.getSize();
	result.resize(k);
	for (int i = 0; i < k; ++i)
	{
		result[i] = image.getPixel(rand() % photoSize.x, rand() % photoSize.y);
	}
	return result;
};

ClosestCentroidIndices findClosestCentroids(sf::Image image, Centroids centroids)
{
	ClosestCentroidIndices result;
	std::vector<int> distance;
	sf::Vector2u photoSize = image.getSize();
	result.resize(photoSize.x * photoSize.y);
	distance.resize(K);

	int min;
	int closest = 0;

	for (int i = 0; i < photoSize.x; i++)
	{
		for (int j = 0; j < photoSize.y; j++)
		{
			for (int n = 0; n < K; n++)
			{
				distance[n] = Distanse(image.getPixel(i, j), centroids[n]);
			}

			min = distance[0];
			for (int n = 0; n < K; n++)
			{
				if (distance[n] < min)
				{
					min = distance[n];
					closest = n;
				}
			}
			result[i * photoSize.y + j] = closest;
		}
	};
	return result;
};

Centroids computeMeans(sf::Image image, ClosestCentroidIndices ids, int x)
{
	Centroids result;
	result.resize(K);
	ColorBuf vec[K];
	sf::Vector2u photoSize = image.getSize();
	for (int i = 0; i < photoSize.x; ++i)
	{
		for (int j = 0; j < photoSize.y; ++j)
		{
			vec[ids[i*photoSize.y + j]] += image.getPixel(i, j);
		}
	};

	for (int i = 0; i < K; i++)
	{
		result[i].r = vec[i].r / vec[i].count;
		result[i].g = vec[i].g / vec[i].count;
		result[i].b = vec[i].b / vec[i].count;
	}
	return result;
};

void changeColors(sf::Image image, Centroids centroids)
{
	sf::Vector2u photoSize = image.getSize();
	ClosestCentroidIndices ids = findClosestCentroids(image, centroids);
	for (int i = 0; i < photoSize.x; ++i)
	{
		for (int j = 0; j < photoSize.y; ++j)
		{
			image.setPixel(i, j, centroids[ids[i * photoSize.y+j]]);
		}
	};

}

int main()
{
	sf::Texture texture;
	texture.loadFromFile("hero.png");
	
	auto image = texture.copyToImage();
	texture.update(image);
	//sf::Sprite photo(texture);
	
	sf::Vector2u photoSize = image.getSize();

	sf::RenderWindow window(sf::VideoMode(photoSize.x, photoSize.y), "My window");

	Centroids centroids = initializeCentroids(image, K);

	for (int it = 0; it < IterationCount; ++it)
	{
		ClosestCentroidIndices ids = findClosestCentroids(image, centroids);
		centroids = computeMeans(image, ids, K);
	};

	changeColors(image, centroids);
	texture.update(image);
	image.saveToFile("result.png");
	texture.update(image);
	sf::Sprite photo;
	photo.setTexture(texture);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.draw(photo);
		window.display();
	}

	return 0;
}
