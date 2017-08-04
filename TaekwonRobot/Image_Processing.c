#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <getopt.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include "amazon2_sdk.h"
#include "graphic_api.h"
#include "uart_api.h"
#include "robot_protocol.h"


#define T_VA_Ver "v0.1"
#define WIDTH 180
#define HEIGHT 120





//void Erosion(unsigned short h, unsigned short w, U16* fpga_videodata);

unsigned short max = 50;
unsigned char max_i = 90;
unsigned char gray;
unsigned short count[WIDTH] = { 0 };


static void Teakwon(U16 *fpga_videodata);

void show_interface(void){
    printf("================================================================\n");
	printf("SoC Robotwar T_Va (Ver : %s)\n", T_VA_Ver);
	printf("================================================================\n");
	printf("O : direct camera display on\n");
	printf("F : direct camera display off\n");
    printf("I : Image Processing\n");
	printf("Q : exit \n");
    printf("? : show this interface\n");
	printf("================================================================\n");
}

#include <termios.h>

static struct termios inittio, newtio;

void init_console(void)
{
    tcgetattr(0, &inittio);
    newtio = inittio;
    newtio.c_lflag &= ~ICANON;
    newtio.c_lflag &= ~ECHO;
    newtio.c_lflag &= ~ISIG;
    newtio.c_cc[VMIN] = 1;
    newtio.c_cc[VTIME] = 0;

    cfsetispeed(&newtio, B115200);

    tcsetattr(0, TCSANOW, &newtio);
}


int main(int argc, char **argv){
    show_interface();
    BOOL b_loop = TRUE;
    if (open_graphic() < 0){
        return -1;
    }

    int ret;

    init_console();

    ret = uart_open();
    
    if (ret < 0) printf("Error\n");

    uart_config(UART1, 57600, 8, UART_PARNONE, 1);

    while (b_loop){
        
        int ch = getchar();

        switch (ch){
            
            case 'O':
            case 'o':
                printf("direct camera display on\n");
			    direct_camera_display_on();
			    break;
            case 'F':
            case 'f':
                printf("direct camera display off\n");
			    direct_camera_display_off();
			    break;
            case 'I':
            case 'i':
                printf("Image_Processing Start\n");
                direct_camera_display_off();
                U16* fpga_videodata = (U16*)malloc(WIDTH * HEIGHT * 2);
                int i;
                for (i=0;i <1000; i++){
                        printf("%d", i);
                }
                Stand();
                while(1){
                    Teakwon(fpga_videodata);
                }
                free(fpga_videodata);
                break;
            case 'Q':
            case 'q':
                b_loop = FALSE;
                break;
            case '?':
                show_interface();
                break;
            case '\n':
                break;        
            default :
                printf("Invalid Command\n");
                break;     
        }
    }
    uart_close();
    close_graphic();
    return 0;
}

static void Teakwon(U16* fpga_videodata){
    clear_screen();
    read_fpga_video_data(fpga_videodata);
    int h, w;
    int max;
    int max_h;
    int loop;
    unsigned char tmp[HEIGHT][WIDTH];
    memset(count, 0, WIDTH * 2);
    memset(tmp, 0, HEIGHT * WIDTH);
    max_h = 0;
    max = 10;
    for (h = 1; h < 119; h++) {
        for (w = 3; w < 177; w++) {  
            if(((*(fpga_videodata + (h * WIDTH + w)) & 0xf800) >> 11 ) - ((*(fpga_videodata + (h * WIDTH + w)) & 0x07e0) >> 5)/2 < 2 && 
               ((*(fpga_videodata + (h * WIDTH + w)) & 0xf800) >> 11 ) - ((*(fpga_videodata + (h * WIDTH + w)) & 0x07e0) >> 5)/2 > -2 &&
                (*(fpga_videodata + (h * WIDTH + w)) & 0x001f) - ((*(fpga_videodata + (h * WIDTH + w)) & 0x07e0) >> 5)/2 < 2 &&
                (*(fpga_videodata + (h * WIDTH + w)) & 0x001f) - ((*(fpga_videodata + (h * WIDTH + w)) & 0x07e0) >> 5)/2 > -2 &&
               ((*(fpga_videodata + (h * WIDTH + w)) & 0xf800) >> 11 ) - (*(fpga_videodata + (h * WIDTH + w)) & 0x001f) < 2 &&
               ((*(fpga_videodata + (h * WIDTH + w)) & 0xf800) >> 11 ) - (*(fpga_videodata + (h * WIDTH + w)) & 0x001f) > -2 &&
               ((*(fpga_videodata + (h * WIDTH + w)) & 0xf800) >> 11 ) < 8 &&
               ((*(fpga_videodata + (h * WIDTH + w)) & 0x07e0) >> 5) < 16 &&
                (*(fpga_videodata + (h * WIDTH + w)) & 0x001f) < 8) {
                *(fpga_videodata + (h * WIDTH + w)) = ((0x00 << 11) | (0x3f << 5) | (0x00));
                tmp[h][w] = ((*(fpga_videodata + (h * WIDTH + w)) & 0x07e0) >> 5);
            }
        }
    }

    for (loop = 0; loop < 1; loop++){
        for (h = 2; h < 118; h++) {
            for (w = 4; w < 177; w++) { 
				if (((*(fpga_videodata + (h * WIDTH + w)) & 0x07e0) >> 5) != 0) {
					if (((*(fpga_videodata + ((h - 1)* WIDTH + (w - 1))) & 0x07e0) >> 5) == 0 || 
                        ((*(fpga_videodata + (h * WIDTH + (w - 1))) & 0x07e0) >> 5) == 0 || 
                        ((*(fpga_videodata + ((h + 1)* WIDTH + (w - 1))) & 0x07e0) >> 5) == 0 ||
						((*(fpga_videodata + ((h - 1)* WIDTH + w)) & 0x07e0) >> 5) == 0 || 
                        ((*(fpga_videodata + ((h + 1)* WIDTH + w)) & 0x07e0) >> 5) == 0 ||
                        ((*(fpga_videodata + ((h - 1)* WIDTH + (w + 1))) & 0x07e0) >> 5) == 0 ||
                        ((*(fpga_videodata + (h * WIDTH + (w + 1))) & 0x07e0) >> 5) == 0 ||
                        ((*(fpga_videodata + ((h + 1)* WIDTH + (w + 1))) & 0x07e0) >> 5) == 0){  
						tmp[h][w] = 0;
					}
				}
			}
		}
        for (h = 2; h < 118; h++) {
            for (w = 4; w < 177; w++) { 
        		if (tmp[h][w] == 0) {
					if (tmp[h - 1][w - 1] != 0 || tmp[h][w - 1] != 0 || tmp[h + 1][w - 1] != 0 ||
						tmp[h - 1][w] != 0 || tmp[h + 1][w] != 0 ||
						tmp[h - 1][w + 1] != 0 || tmp[h][w + 1] || tmp[h + 1][w + 1] != 0) {
						*(fpga_videodata + (h * WIDTH + w)) = 0x3f << 5;
					}
				}
			}
        }   
    }

    for (h = 1; h < 119; h++) {
        for (w = 3; w < 177; w++){
            if(((*(fpga_videodata + ((h + 1)* WIDTH + (w + 1))) & 0x07e0) >> 5) == 0x3f){
                count[w]++;
                if (w < 178 && max < (count[w - 2] + count[w - 1] + count[w] + count[w + 1] + count[w + 2])) {
                    max = (count[w - 2] + count[w - 1] + count[w] + count[w + 1] + count[w + 2]);
                    max_h = w;
                }
            }
        }
    }

    printf("%d ", max_h);
    printf("%d\n", count[max_h]);

    for (h = 1; h < 119; h++){
        *(fpga_videodata + (h * WIDTH + max_h)) = ((0x1f << 11) | (0x00 << 5) | (0x00));
        *(fpga_videodata + (h * WIDTH + 60)) = ((0x1f << 11) | (0x1f << 5) | (0x00)); 
        *(fpga_videodata + (h * WIDTH + 120)) = ((0x1f << 11) | (0x1f << 5) | (0x00));
    }

    if ( 60 < max_h && max_h < 120 ){
        if (count[max_h] > 100) { 
            Right_Punch();    
        }
        else {
            Stand();
        } 

    }

    else if (max_h > 120) {
        Turn_left();
    }

    else if (max_h < 60) {
        Turn_Right();
    }
    
    /*if ((max_h > 80) && (max_h <120)){
           
    }    
    else if ((max_h < 80)){
        Right();
    }
    else {
        Left();
    }*/
    draw_fpga_video_data_full(fpga_videodata);
	flip();
}






