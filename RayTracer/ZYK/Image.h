#ifndef IMAGE_H
#define IMAGE_H
namespace zyk
{
	struct Image
	{
		Image()
		{
			nbytes=width=height=0;
			data=0;
		}
		~Image();
		void free_data();
		bool loadImg(const char*filename);
		int width,height;
		int nbytes;
		unsigned char*data;
	};
}
#endif