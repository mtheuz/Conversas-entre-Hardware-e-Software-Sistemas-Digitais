
#include <stdio.h>
#include <unistd.h>
#include "gpu_lib.h"

int main()
{   
    if (open_gpu_device() == 0)
        return 0;

    	clear_screen();
	clear_poligonos();
	clear_sprites();
}
