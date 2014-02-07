#include <SOIL/SOIL.h>
#include "resource.h"

unsigned char *load_image(char *name, int *width, int *height)
{
	return SOIL_load_image(name, width, height, 0, SOIL_LOAD_RGB);
}

void free_image(unsigned char *image)
{
	SOIL_free_image_data(image);
}
