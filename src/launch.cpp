#ifndef PASSI
#define PASSI 1000000
#endif
#include "gol.h"
#include <math.h>
/*
	------------------------------------------------------------------------------------------------------
	To run the program, Allegro 5 must be installed, compile with::

	c++ -fopenmp -lallegro_image -lallegro_memfile -lallegro_ttf -lallegro_font -lallegro_physfs 
	-lallegro_dialog -lallegro_video -lallegro_acodec -lallegro_main -lallegro_primitives -lallegro_audio 
	-lallegro launch.cpp -o1

	Created by Perfidio Matteo.
	------------------------------------------------------------------------------------------------------
*/
int main(int argc, char **argv)
{

	int N = sqrt(PASSI);
    int M = sqrt(PASSI);

	int showWidth = 10; //set to 10 for the parallel speed test
	int showHeight = 10; //set to 10 for the parallel speed test
	bool done = false;
	bool redraw = true;
	int FPS = 25;

	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	if(!al_init())
		return -1;

	ALLEGRO_DISPLAY *display = al_create_display(showWidth,showHeight);
    if(!display){
        cout << "Fatal error, unable to create a display";
        return -1;
    }
    ALLEGRO_BITMAP *buffer = al_create_bitmap(showWidth,showHeight);
    if(!buffer){
        cout << "Fatal error, unable to create a buffer";
        return -1;
    }

	int windowHeight = al_get_display_height(display);
    int windowWidth = al_get_display_width(display);
    float sx = windowWidth / float(showWidth);
    float sy = windowHeight / float(showHeight);
    int scale = std::min(sx, sy);
    int scaleW = showWidth * scale;
    int scaleH = showHeight * scale;
    int scaleX = (windowWidth - scaleW) / 2;
    int scaleY = (windowHeight - scaleH) / 2;
	al_set_window_title(display, "Game of life");
	al_set_target_bitmap(buffer);
    al_clear_to_color(al_map_rgb(0, 0, 0));
	al_install_keyboard();
	event_queue = al_create_event_queue();
	timer = al_create_timer(3.0/FPS);


	GamingOfLife manager(scaleW, scaleH, scaleX, scaleY, buffer, display, M,N);

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_start_timer(timer);

	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP) 
        	if(ev.keyboard.keycode==ALLEGRO_KEY_ESCAPE) 
				done=true;
            	
		if(done==false && al_is_event_queue_empty(event_queue))
		{
			//manager.showMap();
			manager.nextChild();
            al_flip_display();
			done=true; 
		}

	}

	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_display(display);

	return 0;
}
