#include <string.h>
#include <stdint.h>
#include "bios.h"
#include "debugscreen.h"
#include "gpu.h"
#include "crc.h"
#include "util.h"

#include "datadumper.h"

#define BLOCKSIZE 8

#define STARTX 10
#define STARTY 52
#define MAXENDX   630
#define MAXENDY   460


// DO NOT CHANGE DIRECTLY
#define ENDX ((((MAXENDX-STARTX)/BLOCKSIZE)*BLOCKSIZE)+STARTX)
#define ENDY ((((MAXENDY-STARTY) / BLOCKSIZE) * BLOCKSIZE)+STARTY)
#define USTARTX (STARTX+BLOCKSIZE)
#define USTARTY (STARTY+BLOCKSIZE)
#define UENDX (ENDX-BLOCKSIZE)
#define UENDY (ENDY-BLOCKSIZE)
// END DO NOT CHANGE DIRECTLY

static inline void write_byte(const uint8_t data, struct gpu_solid_rect *filled) {
    for(uint8_t bit = 0; bit < 8; bit++) {
        filled->color = (!((data >> bit) & 1)) * 0xFFFFFF;
        gpu_draw_solid_rect(filled);
        filled->pos.x += BLOCKSIZE;
        if(filled->pos.x == UENDX) {
            filled->pos.x = USTARTX;
            filled->pos.y += BLOCKSIZE;
        }
    }
}

void dump_data(void) {
    // Clear display
	struct gpu_solid_rect background = {
		.pos = {
			.x = 0,
			.y = 10,
		},
		.size = {
			.width = 640,
			.height = 470,
		},
		.color = 0xFFFFFF,
		.semi_transp = 0,
	};
	gpu_draw_solid_rect(&background);        
        
        struct gpu_solid_rect filled = {
                .pos = {
			.x = 10,
			.y = 52,
		},
		.size = {
			.width = BLOCKSIZE,
			.height = BLOCKSIZE,
		},
		.color = 0x000000,
		.semi_transp = 0,
        };

        // draw frame
          

        // top and bottom
        filled.size.width = (ENDX-STARTX);
        gpu_draw_solid_rect(&filled);        
        filled.pos.y = UENDY;
        gpu_draw_solid_rect(&filled);

        // sides
        filled.pos.y = STARTY;
        filled.size.width  = BLOCKSIZE;
        filled.size.height = (ENDY-STARTY);
        gpu_draw_solid_rect(&filled);
        filled.pos.x = UENDX;       
        gpu_draw_solid_rect(&filled);       
       
       
        // draw data
        
        filled.size.height = BLOCKSIZE;

        // number of bytes storable
        const uint16_t FRAMESIZE  = (((UENDX-USTARTX)/BLOCKSIZE) * ((UENDY-USTARTY)/BLOCKSIZE))/ 8;
        uint8_t buf[FRAMESIZE*2];
        
        //memset(buf, 'A', sizeof(buf));

        InitCard(0);
        //StartCard();
        while(1);
        //int32_t fd = FileOpen("bu00:BESLEM-99999TONYHAX", 0x1);
        int32_t fd = FileOpen("bu00:BASLUS-01384DRACULA", 0x1);
        if(fd < 0) {
            volatile uint32_t err = GetLastError();
            while(1);
        }
        delay_ds(1);
        int32_t res = FileRead(fd, buf, sizeof(buf));
        if(res != (int32_t)sizeof(buf)) {
            while(1);
        }
        FileClose(fd); 

        int datacount = sizeof(buf);
        
        // number of data bytes storeable
        const uint16_t framedata = FRAMESIZE - 6;
        for(int i = 0; i < datacount; i += framedata) {
            const int dataleft = datacount - i;
            const uint16_t datanow = dataleft > framedata ? framedata : dataleft;
            // this can be an unaligned load, is this a problem on real hw?
            const uint32_t checksum = crc32(&buf[i], datanow);
            
            // write the data size in LE
            filled.pos.x = USTARTX;
            filled.pos.y = USTARTY;
            write_byte(datanow, &filled);
            write_byte(datanow >> 8, &filled);

            // write the data
            for(int j = i; j < (i+datanow); j++) {
                write_byte(buf[j], &filled);
            }
            
            // write checksum in LE
            filled.pos.x = UENDX - (4*8*BLOCKSIZE);
            filled.pos.y = UENDY - BLOCKSIZE;
            write_byte(checksum, &filled);
            write_byte(checksum >> 8, &filled);
            write_byte(checksum >> 16, &filled);
            write_byte(checksum >> 24, &filled);      
            while(1);
        }

        while(1);
}