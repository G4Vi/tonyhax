#include <string.h>
#include "bios.h"
#include "debugscreen.h"
#include "gpu.h"

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

/* doesn't check if at end of screen*/
static inline void write_byte(const uint8_t data, struct gpu_solid_rect *filled) {
    for(uint8_t bit = 0; bit < 8; bit++) {
        filled->color = (!((data >> bit) & 1)) * 0xFFFFFF;
        gpu_draw_solid_rect(filled);
        filled->pos.x += BLOCKSIZE;
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
        memset(buf, 'A', sizeof(buf));
        int datacount = sizeof(buf);
        
        // number of data bytes storeable
        const uint16_t framedata = FRAMESIZE - 6;
        for(int i = 0; i < datacount; i += framedata) {
            const int dataleft = datacount - i;
            uint16_t datanow = dataleft > framedata ? framedata : dataleft;
            // write the data size in LE
            filled.pos.x = USTARTX;
            filled.pos.y = USTARTY;
            write_byte(datanow, &filled);
            write_byte(datanow >> 8, &filled);

            // write the data
            

            
            // write checksum in LE
            filled.pos.x = UENDX - (4*8*BLOCKSIZE);
            filled.pos.y = UENDY - BLOCKSIZE;
            uint32_t checksum = 0xDEADBEEF;
            write_byte(checksum, &filled);
            write_byte(checksum >> 8, &filled);
            write_byte(checksum >> 16, &filled);
            write_byte(checksum >> 24, &filled);      
            while(1);
        }
       
 
        for(uint16_t y = 52; y < (470-8); y+=8) {

            for(uint16_t x = 10; x < (630-64); x+=64) {
                uint8_t data = 'A';
                                
            }
            filled.pos.x = 10;
            filled.pos.y += 8;

        }
        /*for (uint8_t y = 0; y < qrcode.size; y++) {
            // Each horizontal module
            
            for (uint8_t x = 0; x < qrcode.size; x++) {
                if(qrcode_getModule(&qrcode, x, y)) {
                   gpu_draw_solid_rect(&filled);
                }
                filled.pos.x += BLOCKSIZE;
                
            }
            filled.pos.x = 10;
            filled.pos.y += BLOCKSIZE;            
        }*/
        
      

 
        while(1);


}