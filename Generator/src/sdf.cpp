#include "sdf.h"
#include "lodepng.h"
#include <omp.h>
#include <math.h>
#include <algorithm>

using sdf::Source;
using sdf::SDF;
using std::min;
using std::max;

//Supplied functions to convert from rgba to source
bool sdf::nonzeroAlpha(int clr) {
	return (clr & 0xFF000000) != 0;	//true if a is nonzero
}

bool sdf::nonBlack(int clr) {
	return (clr & 0x00FFFFFF) != 0;	//true if r, g, or b is nonzero
}

Source::Source(const char* filename, bool(*isInside)(int)) {
	std::vector<unsigned char> data;
	printf("Loading source image - ");
	unsigned error = lodepng::decode(data, width, height, filename);
	if (error) printf("ERROR %d: %s\n", error, lodepng_error_text(error));
	else printf("COMPLETE (%ux%u)\n", width, height);

	inside = new bool[width * height];

	for (unsigned y = 0; y < height; y++)
		for (unsigned x = 0; x < width; x++)
			inside[y*width + x] = isInside(*(int*)&data[4*y*width + 4*x]);
}

Source::~Source() {
	delete[] inside;
}

float Source::calcDistance(float u, float v, float range) const {
	int xc = int(u * width);
	int yc = int(v * height);
	int r = int(range * (width + height) / 2);
	int xmin = max(xc - r, 0);
	int ymin = max(yc - r, 0);
	int xmax = min(xc + r, int(width));
	int ymax = min(yc + r, int(height));

	bool in = inside[yc*width + xc];
	int dist = r * r;
	for (int x = xmin; x < xmax; x++) {
		for (int y = ymin; y < ymax; y++) {
			if (x == xc && y == yc) continue;
			if (in != inside[y*width + x]) {
				int dx = x - xc;
				int dy = y - yc;
				int d = dx*dx + dy*dy;
				if (d < dist) dist = d;
			}
		}
	}

	float val = sqrtf(float(dist) / float(r * r));
	if (!in) val = -val;
	return (val + 1.0f) / 2.0f;
}

SDF::SDF(const Source& src, int width, int height, float range) : width(width), height(height) {
	data = new float[width * height];
	
	#pragma omp parallel for
	for (int y = 0; y < height; y++) {
		if (y == 0) {
			printf("Converting to %dx%d SDF using %d threads - ", width, height, omp_get_num_threads());
		}
		for (int x = 0; x < width; x++) {
			float u = float(x) / float(width);
			float v = float(y) / float(height);
			data[y*width + x] = src.calcDistance(u, v, range);
		}
	}

	printf("COMPLETE\n");
}

SDF::~SDF() {
	delete[] data;
}

void SDF::writeGrayscale(const char* filename) {
	std::vector<unsigned char> pixels(4 * width * height);

	for (unsigned y = 0; y < height; y++) {
		for (unsigned x = 0; x < width; x++) {
			pixels[4*y*width + 4*x + 0] = unsigned char(255 * data[y*width + x]);
			pixels[4*y*width + 4*x + 1] = unsigned char(255 * data[y*width + x]);
			pixels[4*y*width + 4*x + 2] = unsigned char(255 * data[y*width + x]);
			pixels[4*y*width + 4*x + 3] = unsigned char(255);
		}
	}

	printf("Writing as grayscale image: %s - ", filename);
	unsigned error = lodepng::encode(filename, pixels, width, height);
	if (error) printf("Encoder error %d: %s\n", error, lodepng_error_text(error));
	else printf("COMPLETE\n");
}

// TODO void SDF::writeAlpha(const char* filename);		//Write simple signed distance field file