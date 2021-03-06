// HashChecker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <limits>
#include <map>
#include <iostream>
#include <fstream>
#include <ctime>

#ifdef _WIN32
#include <cstdlib>
#endif // _WIN32


#define RANGE 100

struct Point2D
{
	int x;
	int y;

	Point2D() = delete;

	Point2D(int _x, int _y) {
		x = _x;
		y = _y;
	}

	bool operator==(const Point2D& other) const
	{
		return (x == other.x && y == other.y);
	}
};

size_t hash(const Point2D& k, int x, int y) 
{
	unsigned long int a = k.x * x;
	unsigned long int b = k.y * y + std::numeric_limits<int>::max();
	if (k.x <= 0) {
		a = a << b;
	}
	if (k.y <= 0)
	{
		b = b << a <<1;
	}

	size_t res = a ^ b;
	
	return res;
}
int main()
{
	int bestx = 0, besty = 0, bestcols = std::numeric_limits<int>::max();

	int space = 0;

	std::ofstream jsonFile;
	std::ofstream logFile;
	jsonFile.open("out.json");
	logFile.open("log.txt");

	jsonFile << "[";
	time_t timenow = time(0);
	logFile << "Log start "<<ctime(&timenow)<<"\n";

	for (int kx = 2; kx < 1000; kx++) {
		for (int ky = 2; ky < 1000; ky++) {
			std::map<size_t, int> cols;

			int colisions = 0;
			space = 0;

			for (int i = -RANGE; i < RANGE; i++)
			{
				for (int j = -RANGE; j < RANGE; j++)
				{
					auto& temp = ++cols[hash(Point2D(i, j), kx, ky)];
					if (temp != 1)
					{
						colisions++;
					}
					space++;
				}
				/*if (space % RANGE * 10 == 0) {
					std::cout << space / RANGE << "(*" << RANGE << "): " << (((double)colisions * 100) / space) << "%\n";
				}*/
			}

			jsonFile << "{\"x\":" << kx << ", \"y\":" << ky << ", \"colisions\":" << colisions << ", \"space\":" << space << "},\n";

			if (colisions < bestcols)
			{
				std::cout << "[New best]\nNumbers: " << kx << ", " << ky << "\n";
				std::cout << "Collisions: " << colisions << " (" << (((double)colisions * 100) / space) << "%)\n";
				std::cout << "Space: " << space << "\n";
				std::cout << "Cols size: " << cols.size() << "\n";

				timenow = time(0);
				logFile<< ctime(&timenow)<<" New best: "<<kx<<", "<<ky<<" (collisions: "<<colisions<<"("<< (((double)colisions * 100) / space) << "%)\n";
				bestx = kx;
				besty = ky;
				bestcols = colisions;
			}
		}
	}

	std::cout << "Best x,y: " << bestx << ", " << besty << "\n";
	std::cout << "Collisions: " << bestcols << " (" << (((double)bestcols * 100) / space) << "%)\n";
	jsonFile << "]";
	jsonFile.close();
	timenow = time(0);
	logFile << "[" << ctime(&timenow) << "] End of log\n";
	logFile.close();
#ifdef _WIN32
	system("PAUSE");
#endif // _WIN32



    return 0;
}

