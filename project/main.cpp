/**
  * Assignment: memory
  * Operating Systems
  */

// function/class definitions you are going to use
#include <sys/resource.h>
#include <sys/time.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

// although it is good habit, you don't have to type 'std::' before many objects by including this line
using namespace std;

// size of the image
const int64_t SIZE = 16384ULL;
const int64_t REPEAT = 2ULL;

int main(int argc, char *argv[])
{
	float *img = new float[SIZE * SIZE];

	// fill with dummy data
	for (int64_t i = 0; i < SIZE; i++)
	{
		for (int64_t j = 0; j < SIZE; j++)
		{
			img[j * SIZE + i] = (2 * j + i) % 32768;
		}
	}

	// this dummy value is needed to avoid compilers eliminating the loop as part of a optimisation
	uint64_t dummy = 0;

	// ADJUST BELOW, BUT keep writing to the dummy variable

	// ? Dum_elem is a single element of the dummy value
	// * It is declared here because it is faster to only declare once
	float dum_elem = 0;

	// Apply an averaging imaging filter to some input image, and write in to an output image.
	// A pixel in the output image is calculated by averaging 9 pixels: the pixel at the same
	// coordinates in the input image, and the adjecent pixels.
	for (int r = 0; r < REPEAT; r++)
	{
		for (int i = 1; i < SIZE - 1; i++)
		{
			for (int j = 1; j < SIZE - 1; j++)
			{
				//for each dum_elem, we start by resetting it
				dum_elem = 0;

				//we add certain pixels to the dummy element
				for (short k = -1; k < 2; k++)
				{
					for (short l = -1; l < 2; l++)
					{
						dum_elem += img[(i + k) * SIZE + j + l];
					}
				}

				//because of averaging 9 pixels we divide by 9
				dum_elem = dum_elem / 9;

				// we add to the actual dummy (the output)
				dummy += (dum_elem);
			}
		}
	}

	// ADJUST ABOVE, BUT keep writing to the dummy variable

	delete[] img;

	//some diagnostics of our code
	struct rusage usage;
	getrusage(RUSAGE_SELF, &usage);
	std::cout << "user time:                    " << usage.ru_utime.tv_sec << "." << std::fixed << std::setw(6) << std::setprecision(6) << std::setfill('0') << usage.ru_utime.tv_usec << " s" << std::endl;
	// ! ADDED SYSTEM CPU TIME
	std::cout << "system time:                  " << usage.ru_stime.tv_sec << "." << std::fixed << std::setw(6) << std::setprecision(6) << std::setfill('0') << usage.ru_stime.tv_usec << " s" << std::endl;
	std::cout << "soft page faults:             " << usage.ru_minflt << std::endl;
	std::cout << "hard page faults:             " << usage.ru_majflt << std::endl;
#ifdef __APPLE__
	std::cout << "max memory:                   " << usage.ru_maxrss / 1024 << " KiB" << std::endl;
#else
	std::cout << "max memory:                   " << usage.ru_maxrss << " KiB" << std::endl;
#endif
	std::cout << "voluntary context switches:   " << usage.ru_nvcsw << std::endl;
	std::cout << "involuntary context switches: " << usage.ru_nivcsw << std::endl;
	std::cout << "dummy value (ignore):         " << dummy << std::endl; // this value is printed to avoid optimisations
	return 0;
}
