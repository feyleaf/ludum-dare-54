//ic_math.h
//Iclea Project Advanced Math Functions:
//Oct 2012: Linear Algebra and Trigonometry
//Dec 2020: Perlin Noise Algorithm
#ifndef IC_MATH_H
#define IC_MATH_H
#include "global.h"

#define PI 3.14159265359f
constexpr uint32_t BIT_NOISE1 = 0xB5297A4D;
constexpr uint32_t BIT_NOISE2 = 0x68E31DA4;
constexpr uint32_t BIT_NOISE3 = 0x1B56C4E9;
constexpr uint32_t CAP = 0xFFFFFFFF;

namespace ic {
	struct coord
	{
		int x;
		int y;
		coord() { x = 0; y = 0; }
		coord(int _x, int _y) { x = _x; y = _y; }
		coord(const coord& src) { x = src.x; y = src.y; }
	};


	uint32_t random(uint32_t position, uint32_t seed);
	template <typename I> std::string n2hexstr(I w, size_t hex_len = sizeof(I) << 1);
	std::string UUID(uint32_t ticker);

	sf::Vector2f toVector(coord src);
	float calcMagnitude(sf::Vector2f v);
	float calcDist(sf::Vector2f pta, sf::Vector2f ptb);
	bool isCollision(sf::Vector2f pt, sf::IntRect box);
	float dot(sf::Vector2f a, sf::Vector2f b);
	sf::Vector2f normalVector(sf::Vector2f src);
	float angle(sf::Vector2f b);
	sf::Vector2f unitVector(float theta);
	sf::Vector2f scalar(float c, sf::Vector2f v);
	sf::Vector2i scalar(int c, sf::Vector2i v);
	sf::Vector2u scalar(unsigned int c, sf::Vector2u v);
	void initRandom();
	void initRandom(unsigned long _seed);
	float distortedCosine(float _theta, long seed);
	unsigned int anyRandom(long seed);
	unsigned char newRandom(unsigned char low, unsigned char hi, float _theta, long seed);
	unsigned char noiseyPixel(coord pos, unsigned char low, unsigned char range, int con, long seed);
	unsigned char bennyPixel(coord pos, unsigned char low, unsigned char range, float offset);

	//perlin noise algorithm
	float interpolate(float a0, float a1, float w);
	sf::Vector2f randomGradient(int ix, int iy);
	float dotGridGradient(int ix, int iy, float x, float y);
	float perlin(float x, float y);
	float perlin(sf::Vector2f vec);
	float perlin(coord c);



	int max3(int a, int b, int c);

	bool operator==(const coord& a, const coord& b);
	bool operator!=(const coord& a, const coord& b);
	bool operator<(const coord& a, const coord& b);
	bool operator>(const coord& a, const coord& b);
	bool operator<=(const coord& a, const coord& b);
	bool operator>=(const coord& a, const coord& b);

	coord operator+(const coord& a, const coord& b);
	coord operator-(const coord& a, const coord& b);
}// close iclea ("ic") namespace

#endif //IC_MATH_H ends
