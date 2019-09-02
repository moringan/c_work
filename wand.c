#include "MagickWand/MagickWand.h"

int main(int argc, char *argv[])
{
    MagickWand *newWand;

    MagickWandGenesis();

    newWand = NewMagickWand();
	if(newWand) DestroyMagickWand(newWand);

	void MagickWandTerminus(void);

	return 0;
}
