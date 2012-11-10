#include <darnit/darnit.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define	SCALE	4

#define cosine(x) sine((x)+90)

int sinetable[360];

int sine(int angle) {
	angle %= 360;
	if (angle < 0)
		angle = 360 + angle;
	return sinetable[angle];
}

void sinetable_init() {
	int i;

	for (i = 0; i < 360; i++) {
		sinetable[i] = sin(M_PI * i / 180)*3600;// * 0x100;
	}
	return;
}

int main(int argc, char **argv) {
	DARNIT_PLATFORM platform;
	DARNIT_TILE *tile;
	DARNIT_TILESHEET *ts;
	unsigned int *pixbuf;
	int w, h, x, y;//, t;
	//int sinc_1, sinc_2, sinc_3, sinc_4;

	sinetable_init();
	if (!darnitInit("Arne", "arne"))
		return -1;
	platform = darnitPlatformGet();

	w = platform.screen_w / SCALE;
	h = platform.screen_h / SCALE;

	ts = darnitRenderTilesheetNew(1, 1, w, h, DARNIT_PFORMAT_RGB5A1);
	tile = darnitRenderTileAlloc(1);

	darnitRenderTileMove(tile, 0, ts, 0, 0);
	darnitRenderTileSetTilesheetCoord(tile, 0, ts, 0, 0, w, h);
	darnitRenderTileSizeSet(tile, 0, platform.screen_w, platform.screen_h);
	pixbuf = malloc(sizeof(unsigned int) * w * h);
	if (!ts || !pixbuf)
		darnitQuit();
	
	memset(pixbuf, 0, w * h * sizeof(unsigned int));
	darnitRenderTilesheetUpdate(ts, 0, 0, w, h, pixbuf);
	
	int mov1, mov2, c1, c2, c3;

	int t = 0;

	for (;;) {
		if(t%2) {
			for (y = 0; y < h; y++)
				for (x = 0; x < w; x++) {
					//int mov0=x+y+cosine((2*sine(t/2))/10)+sine(360*x/100);
					mov1=360*y/h+(t>>1);
					mov2=360*x/w;
					c1=sine(mov1+(t>>1))/2+((mov2>>1)-mov1-mov2+(t>>1));
					//int c2=sine((c1+sine(mov0+t/10)+sine(y/40+t/2)+sine((x+y)/100)));
					c2=sine((c1+sine((y>>2)+(t>>1))+sine((x+y)))/10);
					c3=sine((c2+(cosine(mov1+mov2+c2/10)>>2)+cosine(mov2)+sine(x))/10);
					pixbuf[y * w + x] = (c1+c2+c3)/150+64;///0x10+128;
				}
			darnitRenderTilesheetUpdate(ts, 0, 0, w, h, pixbuf);
		}
		t++;
		
		darnitRenderBegin();
		darnitRenderTileDraw(tile, ts, 1);
		darnitRenderEnd();
		darnitLoop();
	}

	return 0;
}