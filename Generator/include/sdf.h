#ifndef SDF_H
#define SDF_H

namespace sdf {

//Supplied functions to convert from rgba to source
bool nonzeroAlpha(int clr);
bool nonBlack(int clr);

class Source {
	unsigned width, height;
	bool* inside;
public:
	Source(const char* filename, bool (*isInside)(int));
	~Source();

	float calcDistance(float u, float v, float range) const;	//all in 0-1 as fraction of source image space
};

class SDF {
	float* data;
public:
	const unsigned width, height;

	SDF(const Source& src, int width, int height, float range);
	~SDF();

	void writeGrayscale(const char* filename);	//Write as grayscale png
	void writeAlpha(const char* filename);		//Write simple signed distance field file
};

}

#endif //_H