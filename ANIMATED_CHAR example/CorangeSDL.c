#include "corange.h"



#define MAX_BOSSES 100

animated_char* bosses[MAX_BOSSES];


renderer* dr = NULL;
static int anim_id = 0;
static int displayNo = 1;

/*
animated_object* a_imrod;
animated_object* a_imrod1;
animated_object* a_imrod2;
animated_object* a_imrod3;
animated_object* a_imrod4;
 */



void onclick_anim0() { anim_id = 0; }
void onclick_anim1() { anim_id = 1; }

void onclick_anim2()  { displayNo -=1; }
void onclick_anim3()  { displayNo = (displayNo+1)%100; }

//void onclick_anim2()  { bosses->position.x-=1; }
//void onclick_anim3()  { bosses->position.x+=1; }


char* itoa(int val, int base){
	static char buf[32] = {0};
	int i = 30;
	for(; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];
	return &buf[i+1];

}



void rendering_init() {

	int i, xcol=0, startx=-50;

	graphics_viewport_set_title("Renderers");
	graphics_viewport_set_size(1550,850);

	folder_load(P("$CORANGE/rendering/imrod/"));
	folder_load(P("$CORANGE/rendering/imrod/test"));

	for(i=0; i<MAX_BOSSES; i++)
	{
		char buffer[3] = {0};

		if(i>10)
		{
			buffer[0]  = 48+(i/10);
			buffer[1]  = 48+(i%10);
		}
		else
		{
			buffer[0] =48;
			buffer[1] =48+i;
		}

		printf("\nname %s ", buffer);

		bosses[i] = entity_new(buffer, animated_char);

		animated_char_load_skeleton(bosses[i], asset_hndl_new(P("$CORANGE/rendering/imrod/imrod.skl")));
		bosses[i]->renderable = asset_hndl_new(P("$CORANGE/rendering/imrod/imrod.bmf"));
		//   a_imrod->animation = asset_hndl_new(P("$CORANGE/rendering/imrod/imrod.ani"));
		bosses[i]->anim_bot = asset_hndl_new(P("$CORANGE/rendering/imrod/imrod.ani"));
		bosses[i]->anim_top = asset_hndl_new(P("$CORANGE/rendering/imrod/test/imrod-test.ani"));
		bosses[i]->rotation = quat_rotation_y(1.57);
		bosses[i]->scale = vec3_new(1.25, 1.25, 1.25);
		bosses[i]->position = vec3_new( ( (i%10)*10)+startx, 0, xcol*10 );


		if(i%10==0)
			xcol++;
	}



	/* Put some text on the screen */

	ui_button* framerate = ui_elem_new("framerate", ui_button);
	ui_button_move(framerate, vec2_new(10,10));
	ui_button_resize(framerate, vec2_new(30,25));
	ui_button_set_label(framerate, "FRAMERATE");
	ui_button_disable(framerate);

	ui_button* object = ui_elem_new("object", ui_button);
	ui_button_move(object, vec2_new(10, graphics_viewport_height() - 70));
	ui_button_resize(object, vec2_new(60,25));
	ui_button_set_label(object, "Object");
	ui_button_disable(object);

	ui_button* anim0 = ui_elem_new("anim0", ui_button);
	ui_button_move(anim0, vec2_new(80, graphics_viewport_height() - 70));
	ui_button_resize(anim0, vec2_new(50,25));
	ui_button_set_label(anim0, "anim 0");

	ui_button* anim1 = ui_elem_new("anim1", ui_button);
	ui_button_move(anim1, vec2_new(140, graphics_viewport_height() - 70));
	ui_button_resize(anim1, vec2_new(50,25));
	ui_button_set_label(anim1, "anim 1");

	ui_button* anim2 = ui_elem_new("anim2", ui_button);
	ui_button_move(anim2, vec2_new(200, graphics_viewport_height() - 70));
	ui_button_resize(anim2, vec2_new(50,25));
	ui_button_set_label(anim2, "Disp -");

	ui_button* anim3 = ui_elem_new("dino", ui_button);
	ui_button_move(anim3, vec2_new(260, graphics_viewport_height() - 70));
	ui_button_resize(anim3, vec2_new(40,25));
	ui_button_set_label(anim3, "Disp +");



	ui_button_set_onclick(anim0, onclick_anim0);
	ui_button_set_onclick(anim1, onclick_anim1);
	ui_button_set_onclick(anim2, onclick_anim2);
	ui_button_set_onclick(anim3, onclick_anim3);


	/* New Camera and light */

	camera* cam = entity_new("camera", camera);
	cam->position = vec3_new(5.0, 88.0, 5.0);
	cam->target = vec3_new(0, 7, 0);

	/* Renderer Setup */

	dr = renderer_new(asset_hndl_new_load(P("$CORANGE/rendering/graphics.cfg")));
	renderer_set_camera(dr, cam);
	renderer_set_tod(dr, 0.15, 0);
	renderer_set_skydome_enabled(dr, true);

	SDL_GL_CheckError();
}

void rendering_event(SDL_Event event) {

	camera* cam = entity_get("camera");
	camera_control_orbit(cam, event);

}



void rendering_update() {
	int i;
	camera* cam = entity_get("camera");

	camera_control_joyorbit(cam, frame_time());

	ui_button* framerate = ui_elem_get("framerate");
	ui_button_set_label(framerate, frame_rate_string());


	for(i=0; i<displayNo; i++)
		animated_char_update(bosses[i], frame_time() * 0.25);
		// animated_object_update(bosses[i], anim_id, frame_time() * 0.25);



}

void rendering_render() {

	int i;

	for(i=0; i<displayNo; i++)
	{

			renderer_add(dr, render_char_animated( bosses[i] ));
	}

	renderer_render(dr);

}

void rendering_finish() {
	renderer_delete(dr);
}

int main(int argc, char **argv) {

#ifdef _WIN32
	FILE* ctt = fopen("CON", "w" );
	FILE* fout = freopen( "CON", "w", stdout );
	FILE* ferr = freopen( "CON", "w", stderr );
#endif

	corange_init("assets_core");
	rendering_init();

	int running = 1;
	SDL_Event event;

	while(running) {

		frame_begin();

		while(SDL_PollEvent(&event)) {
			switch(event.type){
			case SDL_KEYDOWN: break;
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_ESCAPE) { running = 0; }
				if (event.key.keysym.sym == SDLK_PRINTSCREEN) { graphics_viewport_screenshot(); }
				if ((event.key.keysym.sym == SDLK_r) &&
						(event.key.keysym.mod == KMOD_LCTRL)) {
					asset_reload_all();
				}
				break;
			case SDL_QUIT:
				running = 0;
				break;

			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				ui_update();
				ui_event(event);
				break;


			}
			rendering_event(event);

		}

		rendering_update();


		rendering_render();
		ui_render();
		graphics_swap();

		frame_end();
	}

	rendering_finish();

	corange_finish();

	return 0;
}
