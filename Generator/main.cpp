#include "sdf.h"

int main(int argc, char* argv[]) {
	sdf::Source source("test.png", sdf::nonzeroAlpha);
	sdf::SDF output(source, 1024, 1024, 64.0f / 4096.0f);
	output.writeGrayscale("output.png");
}