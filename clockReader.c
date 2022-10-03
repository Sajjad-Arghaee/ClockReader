#include <stdio.h>
#include <bmpio.h>
#include <math.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

unsigned char input_pic[2000][2000][3];
unsigned char around_croped[2000][2000][3];
unsigned char croped_part[2000][2000][3];
unsigned char dataset[2000][2000][3];
unsigned char horizontally_resized[2000][2000][3];
unsigned char resized_pic[2000][2000][3];
unsigned char average[2000][2000][3];
int count_line;
int count_width;
int newheight = 750, newwidth = 750, hour = 0, minute = 0;
int height, width;
double second = 0;
int TheLenght, NumberOfCropedParts = 0, counter = 0, status=0;
char path[50];
void crop();
void resize();
void compare();
void prime_croper(unsigned char input_pic[][2000][3], int*, int*, int*, int*);
void find_line(int*, int*);
int find_column(int, int, int*, int*);
void str_copy(int, int, int, int);
void get_average();
void croper(unsigned char around_croped[][2000][3], int i1, int i2, int j1, int j2);
int trueyab(int);
int trueyab2(int);
void horizontal(int);
void vertical(int);
void cut3();
void cut9();
double min_finder(double, double);
void must_init(bool test, const char* description)
{
    if (test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int main()
{
	int o, v, z, q, f;
	char address[50];
    char path[50];
	// first of all, we have to set our datasets...
	for(q=0;q<10;q++){
		system("CLS");
		printf("Hello and welcome to read_clock program :)\tthis code can understand any clock you drew!!\n");
		printf("please wait a moment while bulding datasets...\n");
		printf("%d%% completed", 10*(q+1));
		for (o = 0; o < 750; o++)
            	for (v = 0; v < 750; v++)
            	    for (z = 0; z < 3; z++)
            	        average[o][v][z] = 0;
		for(f=1;f<=15;f++){
		 	// reset all pixels of picture to zero :
        	for (o = 0; o < 2000; o++)
        	    for (v = 0; v < 2000; v++)
        	        for (z = 0; z < 3; z++)
           		        horizontally_resized[o][v][z] = 0;
        	for (o = 0; o < 2000; o++)
            	for (v = 0; v < 2000; v++)
            	    for (z = 0; z < 3; z++)
            	        resized_pic[o][v][z] = 0;
			sprintf(path, "%d\\%d.bmp", q, f);
			readBMP(path, &width, &height, input_pic);
			if(width == 0) return 1;
			crop();
			resize();
			get_average();
		}
		if(q == 0){
			for (o = 0; o < 750; o++)
           		for (v = 0; v < 750; v++){
                	if(average[o][v][0] + average[o][v][1] + average[o][v][2] >= 300){
               			average[o][v][0] = 255;
               			average[o][v][1] = 255;
               			average[o][v][2] = 255;
					}
					else{
						average[o][v][0] = 0;
               			average[o][v][1] = 0;
               			average[o][v][2] = 0;
					}
				}
			}
		else{
			for (o = 0; o < 750; o++)
           		for (v = 0; v < 750; v++){
                	if(average[o][v][0] + average[o][v][1] + average[o][v][2] >= 550){
               			average[o][v][0] = 255;
               			average[o][v][1] = 255;
               			average[o][v][2] = 255;
					}
					else{
						average[o][v][0] = 0;
               			average[o][v][1] = 0;
               			average[o][v][2] = 0;
					}
				}			
		}
		if(q == 3) cut3();
		if(q == 9) cut9();
		sprintf(address, "dataset%d.bmp", q);
        saveBMP(average, 750, 750, address);      	
	}
	status = 1;
	printf("\nlet's start... please tell the name of picture you drew!");
	printf("\ndon't forget that the picure's format should be .bmp !!!");
	printf("\ntype here : ");
    //receiving the name of input_picture: 
    gets(path);
    readBMP(path, &width, &height, input_pic);
    if(width == 0) return 1;
    printf("\ntime is : ");
    crop();
    while (counter != NumberOfCropedParts) {
    // reset all pixels of picture to zero :
        for (o = 0; o < 2000; o++)
            for (v = 0; v < 2000; v++)
                for (z = 0; z < 3; z++)
                    horizontally_resized[o][v][z] = 0;
        for (o = 0; o < 2000; o++)
            for (v = 0; v < 2000; v++)
                for (z = 0; z < 3; z++)
                    resized_pic[o][v][z] = 0;
        if (counter == 2) {
            printf(":");
            counter++;
            continue;
        }
        if (counter == 5) {
            printf(":");
            counter++;
            continue;
        }
		// after croping the numbers in clock test picture,
		// we need to resize them and compare them with datasets :
        sprintf(path, "out%d.bmp", counter);
        readBMP(path, &width, &height, croped_part);
        resize();
        compare();
        counter++;
    }
    
    must_init(al_init(), "allegro");
    must_init(al_init_primitives_addon(), "primitives");

    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio codecs");
    must_init(al_reserve_samples(16), "reserve samples");

    int condition = 0, first = 1, x=0;
    double l = 217, n = 400, f1=0;
    int count = 0;
    ALLEGRO_TIMER* timer1 = al_create_timer(1.0 / 60.0);
    must_init(timer1, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    ALLEGRO_DISPLAY* disp = al_create_display(600, 600);
    must_init(disp, "display");

    must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* clock = al_load_bitmap("watch.png");
    must_init(clock, "clock");

    ALLEGRO_SAMPLE* tiktok = al_load_sample("tiktok.wav");
    must_init(tiktok, "tiktok");

    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer1));

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;
    float x1 = 303 + 50 * cos(((hour % 12 - 3) * 30) * 3.141592 / 180), y1 = 305 + 50 * sin(((hour % 12 - 3) * 30) * 3.141592 / 180);
    float x2 = 303 + 100 * cos(((minute - 15) * 6) * 3.141592 / 180), y2 = 305 + 100 * sin(((minute - 15) * 6) * 3.141592 / 180);
    float x3 = 303 + 150 * cos(((second - 15) * 6) * 3.141592 / 180), y3 = 305 + 150 * sin(((second - 15) * 6) * 3.141592 / 180);
    float x4 = 217 + 30 * cos(((condition - 15) * 6) * 3.141592 / 180), y4 = 304 + 30 * sin(((condition - 15) * 6) * 3.141592 / 180);
    al_start_timer(timer1);
    al_play_sample(tiktok, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    al_draw_bitmap(clock, 100, 100, 0);
    while (1)
    {

        al_wait_for_event(queue, &event);
		// we have to redraw the image , after 1 second :
        if ((al_get_timer_count(timer1) % 60) == 0) {
            al_draw_bitmap(clock, 100, 100, 0);
            l = 217;
            n = 400;
        }

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            redraw = true;
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        if (done)
            break;

        if (redraw && al_is_event_queue_empty(queue))
        {
            if (count == 1) {
                al_draw_bitmap(clock, 100, 100, 0);
                count = 0;
            }

            al_draw_circle(303, 305, 10, al_map_rgb(0, 255, 0), 2);
            al_draw_filled_circle(303, 305, 2, al_map_rgb(0, 255, 0));
            al_draw_filled_circle(303, 217, 2, al_map_rgb(0, 255, 0));
            al_draw_line(303, 305, x1, y1, al_map_rgb(147, 112, 219), 3);
            al_draw_line(303, 305, x2, y2, al_map_rgb(0, 0, 200), 3);
            // 1/60 < 0.02 :
            if(second >= floor(second) && second<= floor(second) + 0.02)
                al_draw_line(303, 305, x3, y3, al_map_rgb(255, 0, 0), 1);
            // the up statement, move the second hand
            al_draw_line(303, 217, 288, 203, al_map_rgb(255, 255, 0), 2);


            second += 1.0/60.0;
            x3 = 303 + 150 * cos(((second - 15) * 6) * 3.141592 / 180);
            y3 = 305 + 150 * sin(((second - 15) * 6) * 3.141592 / 180);
            if (second >= 60)
                second = 0;
            if (second == 0) {
                minute++;
                x2 = 303 + 100 * cos(((minute % 60 - 15) * 6) * 3.141592 / 180);
                y2 = 305 + 100 * sin(((minute % 60 - 15) * 6) * 3.141592 / 180);
                al_draw_bitmap(clock, 100, 100, 0);
            }
            if (minute > 0 && minute % 60 == 0 && first == 1) {
                hour++;
                first = 0;
                x1 = 303 + 50 * cos(((hour % 12 - 3) * 30) * 3.141592 / 180);
                y1 = 305 + 50 * sin(((hour % 12 - 3) * 30) * 3.141592 / 180);
            }

            redraw = false;
        }

        if(f1 != 0)
            al_draw_line(217, 400, x4, y4, al_map_rgb(147, 112, 219), 1);
        f1 += 0.0001;
        condition++;
        x4 = l + 30 * cos(((condition - 15) * 6) * 3.141592 / 180);
        y4 = n + 10 * sin(((condition - 15) * 6) * 3.141592 / 180);
        l -= 0.2;
        n -= 1.4;

        al_flip_display();

    }
    al_destroy_bitmap(clock);
    al_destroy_display(disp);
    al_destroy_timer(timer1);
    al_destroy_event_queue(queue);
    
    return 0;
}
// this is the main function for crop :
void crop() {
    int i1 = 0, i2 = 0, j1 = 0, j2 = 0, q, i, j;
    char name[50];
    // cutting around the original photo : 
    prime_croper(input_pic, &width, &height, &j1, &i1);
    if(status == 0) return;
    count_line = 1;
    while (count_line > 0) {
        find_line(&i1, &i2);
        count_width = 1;
        j1 = 0;
        j2 = 0;
        while (count_width > 0) {
            q = find_column(i1, i2, &j1, &j2);
            if (q == -1) break;
            if (j1 == width - 1) break;
            // we have to name the outputs different :
            sprintf(name, "out%d.bmp", NumberOfCropedParts);
            // fill in the desired part of the image :
            for (i = 0; i < TheLenght+1; i++)
                for (j = 0; j < j2-j1; j++) {
                    if (croped_part[i][j][0] + croped_part[i][j][1] + croped_part[i][j][2] >= 550) {
                        croped_part[i][j][0] = 255;
                        croped_part[i][j][1] = 255;
                        croped_part[i][j][2] = 255;
                    }
                    else {
                        croped_part[i][j][0] = 0;
                        croped_part[i][j][1] = 0;
                        croped_part[i][j][2] = 0;
                    }
                }
           	saveBMP(croped_part, j2 - j1, TheLenght + 1, name);
            NumberOfCropedParts++;
            j1 = j2;
        }
        i1 = i2;
    }
}
// this function, cut around the prime picture :
void prime_croper(unsigned char input_pic[][2000][3], int* w, int* h, int* w0, int* h0) {
	// lmin is line min(first line that is not completely white) , cmin is column min
	// lmax is line max , lmin is column max
    int i, j, lmin = -1, lmax = -1, cmin = -1, cmax = -1;
    for (i = *h0; i < *h; i++) {
        for (j = *w0; j < *w; j++) {
            if (input_pic[i][j][0] != 255 || input_pic[i][j][1] != 255 || input_pic[i][j][2] != 255) {
                lmin = i;
                break;
            }
        }
        if (lmin != -1) break;
    }
    for (i = *h - 1; i > lmin; i--) {
        for (j = 0; j < *w; j++) {
            if (input_pic[i][j][0] != 255 || input_pic[i][j][1] != 255 || input_pic[i][j][2] != 255) {
                lmax = i;
                break;
            }
        }
        if (lmax != -1)	break;
    }
    for (j = *w0; j < *w; j++) {
        for (i = *h0; i < *h; i++) {
            if (input_pic[i][j][0] != 255 || input_pic[i][j][1] != 255 || input_pic[i][j][2] != 255) {
                cmin = j;
                break;
            }
        }
        if (cmin != -1)
            break;
    }
    for (j = *w - 1; j > cmin; j--) {
        for (i = *h0; i < *h; i++) {
            if (input_pic[i][j][0] != 255 || input_pic[i][j][1] != 255 || input_pic[i][j][2] != 255) {
                cmax = j;
                break;
            }
        }
        if (cmax != -1) 	break;
    }
    *w = cmax - cmin;
    *h = lmax - lmin;
    for (i = 0; i < *h; i++) {
        for (j = 0; j < *w; j++) {
            around_croped[i][j][0] = input_pic[i + lmin][j + cmin][0];
            around_croped[i][j][1] = input_pic[i + lmin][j + cmin][1];
            around_croped[i][j][2] = input_pic[i + lmin][j + cmin][2];
        }
    }
}
// scanning from up to down of picture :
void find_line(int* i1, int* i2) {
    int a = *i1, b, t, p = 0;
    while (p == 0 && a < height) {
        t = 0;
        for (b = 0; b < width; b++) {
            if (around_croped[a][b][0] != 255 || around_croped[a][b][1] != 255 || around_croped[a][b][2] != 255)
                t++;
        }
        if (t != 0 || a == height - 1) {
            *i1 = a;
            p = 1;
        }
        a++;
    }
    if (*i1 == height - 1)
        return;
    // it indicates that we have reached the end of the image
    // and we couldn't find anything else :(
    for (a = *i1 + 1; a < height; a++) {
        t = 0;
        for (b = 0; b < width; b++) {
            if (around_croped[a][b][0] != 255 || around_croped[a][b][1] != 255 || around_croped[a][b][2] != 255)
                t++;
        }
        if (t == 0 || a == height - 1) {
            *i2 = a;
            break;
        }
    }
    if (a == height - 1)
        count_line = 0;
}
// scanning from left to right of picture :
int find_column(int i1, int i2, int* j1, int* j2) {
    int a, b, t;
    t = 0;
    for (b = trueyab(*j1 + 1); b < width; b++) {
        for (a = i1; a <= i2; a++) {
            if (around_croped[a][b][0] != 255 || around_croped[a][b][1] != 255 || around_croped[a][b][2] != 255) {
                *j1 = b;
                t = 1;
                break;
            }
        }
        if (b == width - 1 && t == 0)	return -1;
        if (t == 1)	break;
    }
    for (b = *j1 + 1; b <= width; b++) {
        t = 0;
        for (a = i1; a <= i2; a++) {
            if (around_croped[a][b][0] != 255 || around_croped[a][b][1] != 255 || around_croped[a][b][2] != 255) {
                t++;
            }
        }
        if (t == 0 || b == width) {
            *j2 = b;
            break;
        }
    }
    if (*j2 >= width - 1)
        count_width = 0;
    croper(around_croped, i1, i2, *j1, *j2);
}
// copying the finally croped_parts to an array, in order to save it :
void str_copy(int i1, int i2, int j1, int j2) {
    int a, b;
    TheLenght = i2 - i1;
    for (a = i1; a <= i2 + 1; a++) {
        for (b = j1; b <= j2; b++) {
            croped_part[a - i1][b - j1][0] = around_croped[a][b][0];
            croped_part[a - i1][b - j1][1] = around_croped[a][b][1];
            croped_part[a - i1][b - j1][2] = around_croped[a][b][2];
        }
    }
}
// croper function , make the final croped_pictures after we find the position of a shape :
void croper(unsigned char around_croped[][2000][3], int i1, int i2, int j1, int j2) {
    int i, j, lmin = -1, lmax = -1, cmin = j1, cmax = j2;
    for (i = i1; i <= i2; i++) {
        for (j = cmin; j <= trueyab2(cmax); j++) {
            if (around_croped[i][j][0] != 255 || around_croped[i][j][1] != 255 || around_croped[i][j][2] != 255) {
                lmin = i;
                break;
            }
        }
        if (lmin != -1) break;
    }
    for (i = i2; i >= i1; i--) {
        for (j = cmin; j <= trueyab2(cmax); j++) {
            if (around_croped[i][j][0] != 255 || around_croped[i][j][1] != 255 || around_croped[i][j][2] != 255) {
                lmax = i;
                break;
            }
        }
        if (lmax != -1)	break;
    }
    TheLenght = lmax - lmin;
    str_copy(lmin, lmax, cmin, cmax);
}
// trueyab is a function that prevents from overflowing :
int trueyab(int a) {
    if (a == 1) return 0;
    else return a;
}
// trueyab2 is a function that prevents from overflowing :
int trueyab2(int a) {
    if (a == width) return a - 1;
    else return a;
}
// main function of resize operation :
void resize() {
    int i, j;
    horizontal(newwidth);
    width = newwidth;
    vertical(newheight);
}
//incresing or reducing width of input_picture :
void horizontal(int newwidth) {
    int i = 0, k, a, b;
    double  t = (newwidth * 1.0) / (1.0 * width), tremain, j1 = 0, j2 = 0, fill;
    while (i < height) {
        j2 = 0;
        j1 = 0;
        while (j2 != newwidth) {
            // because of posibility computational error in rounding , we do this :
            // (it will work at the end of loop)
            if (newwidth - j2 < 0.0001) {
                fill = newwidth - j2;
                if(status == 1){
                	for (k = 0; k < 3; k++)
                    	horizontally_resized[i][newwidth][k] += fill * croped_part[i][width][k];
                }
                else{
                	for (k = 0; k < 3; k++)
                    	horizontally_resized[i][newwidth][k] += fill * around_croped[i][width][k];
				}
				//j1  relatet to input_pic's width and j2 related to horizontally_resized's width
                j2 += fill;
                j1 += fill / t;
                continue;
            }
            // 'tremain' holds the value of newwidth to width ratio
            tremain = t;
            while (tremain > 0) {
                fill = min_finder(tremain, floor(j2) + 1 - j2);
                a = floor(j2);
                b = floor(j1);
                if(status == 1){
				for (k = 0; k < 3; k++)
                    horizontally_resized[i][a][k] += fill * croped_part[i][b][k];
                }
				else{
					for (k = 0; k < 3; k++)
                   		horizontally_resized[i][a][k] += fill * around_croped[i][b][k];
				}
				//j1  relatet to input_pic's width and j2 related to horizontally_resized's width
                j2 += fill;
                j1 += fill / t;
                tremain -= fill;

            }
        }
        i++;
    }
}
//incresing or reducing height of input_picture :
// (the vertical function works like horizontal function
// and we just need to change minor things)
void vertical(int newheight) {
    int j = 0, k, a, b;
    double  t = (1.0 * newheight) / (1.0 * height), tremain, i1 = 0, i2 = 0, fill;
    while (j < width) {
        i1 = 0;
        i2 = 0;
        while (i2 != newheight) {
            if (newheight - i2 < 0.001) {
                fill = newheight - i2;
                for (k = 0; k < 3; k++)
                    resized_pic[newheight][j][k] += fill * horizontally_resized[height][j][k];
                i2 += fill;
                i1 += fill / t;
                continue;
            }
            tremain = t;
            while (tremain > 0) {
                fill = min_finder(tremain, floor(i2) + 1 - i2);
                a = floor(i2);
                b = floor(i1);
                for (k = 0; k < 3; k++)
                    resized_pic[a][j][k] += fill * horizontally_resized[b][j][k];
                i2 += fill;
                i1 += fill / t;
                tremain -= fill;
            }
        }
        j++;
    }
}
double min_finder(double a, double b) {
    if (a > b) return b;
    else return a;
}
// comparing the numbers(croped numbers) with datasets :
void compare() {
    int i, j, k, result = 10, c, w, h;
    long int min = 999999999, t;
    char path[100];
    for (i = 0; i < 750; i++) {
        for (j = 0; j < 750; j++) {
            if (resized_pic[i][j][0] + resized_pic[i][j][1] + resized_pic[i][j][2] >= 500) {
                resized_pic[i][j][0] = 255;
                resized_pic[i][j][1] = 255;
                resized_pic[i][j][2] = 255;
            }
            else {
                resized_pic[i][j][0] = 0;
                resized_pic[i][j][1] = 0;
                resized_pic[i][j][2] = 0;
            }
        }
    }
    for (c = 0; c <= 9; c++) {
        t = 0;
        sprintf(path, "dataset%d.bmp", c);
        readBMP(path, &w, &h, dataset);
        for (i = 0; i < 750; i++)
            for (j = 0; j < 750; j++)
                for (k = 0; k < 3; k++)
                    t += abs(dataset[i][j][k] - resized_pic[i][j][k]);
        if (t < min) {
            result = c;
            min = t;
        }
    }
    printf("%d", result);
    if(counter == 0)	hour   +=    10*result;
    if(counter == 1)	hour   +=       result;
    if(counter == 3)	minute    += 10*result;
    if(counter == 4)	minute    +=    result;
    if(counter == 6)    second +=  10.0*result;
    if(counter == 7)	second +=   1.0*result;
}
void get_average(){
	int i, j, k;
	for(i=0;i<750;i++)
		for(j=0;j<750;j++)
			for(k=0;k<3;k++)
				average[i][j][k] += resized_pic[i][j][k] / 15;
}
void cut3(){
	int i, j, k;
	for(i=0;i<750;i++)
		for(j=0;j<100;j++)
			for(k=0;k<3;k++)
				average[i][j][k] = 255;
	for(i=300;i<500;i++)
		for(j=150;j<300;j++)
			for(k=0;k<3;k++)
				average[i][j][k] = 255;
}
void cut9(){
	int i, j, k;
	for(i=600;i<750;i++)
		for(j=0;j<200;j++)
			for(k=0;k<3;k++)
				average[i][j][k] = 255;	
}

