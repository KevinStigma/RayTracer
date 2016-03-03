#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
namespace zyk
{
	bool Image::loadImg(const char*filename)
	{
		data = stbi_load(filename, &width, &height, &nbytes, 0);
		if(!data)
			return false;
	}

	void Image::free_data()
	{
		if(data)
			STBI_FREE(data);
		data=NULL;
		width=0;
		height=0;
		nbytes=0;
	}

	Image::~Image()
	{
		if(data)
			STBI_FREE(data);
		data=NULL;
	}
}