#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>

#define AANTAL_KOEIEN 11

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 576

#define LIJF	16
#define KOP		8
#define STAART	3
#define LINKER_VOOR_POOT    3
#define RECHTER_VOOR_POOT   3
#define LINKER_ACHTER_POOT  3
#define RECHTER_ACHTER_POOT 3

#define KOE_GROOTTE_MULTIPLIER	1 // Decimale getallen zijn af te raden, tenzij je de compiler wilt laten schreeuwen lol.

// Define the mighty cow as something drawable @ some x,y-coordinate:
typedef struct _koe_
{
	SDL_Color kleur;	// Kleur van koe
	int x;	// X pos van koe
	int y;	// Y pos van koe
} koe;

void koe_renderaar(koe *koeien, SDL_Renderer *renderer);
int process_input(koe *koeien);
void slapen(SDL_Renderer *renderer, SDL_Window *window);
void spawn_greta(koe *koeien);
void npc_koe_config(koe *koeien);
void error_handle_sdl(SDL_Renderer *renderer, SDL_Window *window);

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	/* Variabelen */
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	unsigned int window_flags = 0;
	unsigned int renderer_flags = SDL_RENDERER_ACCELERATED;
	koe koeien[AANTAL_KOEIEN] = {0};	// array van koeien

	srand((unsigned int)time(NULL));	// seed random getal generator

	/* Koe dingen */
	npc_koe_config(koeien);
	spawn_greta(koeien);

	/* SDL init */
	window = SDL_CreateWindow("All your grass are belong to Greta", SDL_WINDOWPOS_UNDEFINED,
							  SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
							  window_flags);
	renderer = SDL_CreateRenderer(window, -1, renderer_flags);
	error_handle_sdl(renderer, window); // rest van de init (error handle)
	
	while (1)
	{
		// Refresh the backbuffer to its original state:
		// RGB (39, 174, 96) should be a green grass color
		SDL_SetRenderDrawColor(renderer, 39, 174, 96, 255);
		SDL_RenderClear(renderer);
		// Process selected inputs and pay close attention to moving
		// our freshly spawned NPC cows:
		for (int spawn_die_koe = 0; spawn_die_koe < AANTAL_KOEIEN; spawn_die_koe++)
		{
			koe_renderaar(&koeien[spawn_die_koe], renderer);
		}
		if (process_input(koeien) == 1) // check of de gebruiker het programma wil stoppen
		{
			break; // ga uit die while loep
		}
		// Render redrawn scene to front buffer, showing it in the
		// actual window:
		SDL_RenderPresent(renderer);
		// Remember ~ 60 FPS of smooth Greta movements - PC Master Race!
		SDL_Delay(16);
	}
	slapen(renderer, window);	// Als esc of kruisje == 1 met return ga dan maar slapen :)
	return 0;
}

void error_handle_sdl(SDL_Renderer *renderer, SDL_Window *window)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	if (window == NULL)
	{
		printf("Failed to create window -- Error: %s\n",
			   SDL_GetError());
		exit(1);
	}

	if (renderer == NULL) // error handling:
	{
		printf("Failed to create renderer -- Error: %s\n",
			   SDL_GetError());
		exit(1);
	}
}

void spawn_greta(koe *koeien)
{
	koeien[10].x = SCREEN_WIDTH / 2; 	// Greta x cords
	koeien[10].y = SCREEN_HEIGHT / 2;	// Greta y cords
	koeien[10].kleur.r = 135;	// Greta R kleur waarde
	koeien[10].kleur.g = 54;	// Greta G kleur waarde
	koeien[10].kleur.b = 0;		// Greta B kleur waarde
	koeien[10].kleur.a = 255;	// Greta ALPHA waarde
}	

void npc_koe_config(koe *koeien)
{
	for (int iteratie = 0; iteratie < (AANTAL_KOEIEN); iteratie++)	// Koe generator
	{
		koeien[iteratie].x = rand() % SCREEN_WIDTH;		// Koe x cords
		koeien[iteratie].y = rand() % SCREEN_HEIGHT;	// Koe y cords
		koeien[iteratie].kleur.r = (Uint8)rand() % 255;	// Koe R kleur waarde
		koeien[iteratie].kleur.g = (Uint8)rand() % 255; // Koe G kleur waarde
		koeien[iteratie].kleur.b = (Uint8)rand() % 255; // Koe B kleur waarde
		koeien[iteratie].kleur.a = 255;					// Koe ALPHA/TRANSPARENCY waarde
	}
}

void koe_renderaar(koe *koeien, SDL_Renderer *renderer)
{
	// Update Greta's location based on the input data:
	SDL_Rect lijf = {koeien->x, koeien->y, LIJF*KOE_GROOTTE_MULTIPLIER, LIJF*KOE_GROOTTE_MULTIPLIER};
	SDL_Rect kop = {koeien->x + 4*KOE_GROOTTE_MULTIPLIER, koeien->y - 8*KOE_GROOTTE_MULTIPLIER, KOP*KOE_GROOTTE_MULTIPLIER, KOP*KOE_GROOTTE_MULTIPLIER};
	SDL_Rect staart = {koeien->x + 7*KOE_GROOTTE_MULTIPLIER, koeien->y + 16*KOE_GROOTTE_MULTIPLIER, STAART*KOE_GROOTTE_MULTIPLIER, STAART*KOE_GROOTTE_MULTIPLIER};
	SDL_Rect linkervoorpoot = {koeien->x - 3*KOE_GROOTTE_MULTIPLIER, koeien->y + 2*KOE_GROOTTE_MULTIPLIER, LINKER_VOOR_POOT*KOE_GROOTTE_MULTIPLIER, LINKER_VOOR_POOT*KOE_GROOTTE_MULTIPLIER};
	SDL_Rect rechtervoorpoot = {koeien->x + 16*KOE_GROOTTE_MULTIPLIER, koeien->y + 2*KOE_GROOTTE_MULTIPLIER, RECHTER_VOOR_POOT*KOE_GROOTTE_MULTIPLIER, RECHTER_VOOR_POOT*KOE_GROOTTE_MULTIPLIER};
	SDL_Rect linkerachterpoot = {koeien->x - 3*KOE_GROOTTE_MULTIPLIER, koeien->y + 12*KOE_GROOTTE_MULTIPLIER, LINKER_ACHTER_POOT*KOE_GROOTTE_MULTIPLIER, LINKER_ACHTER_POOT*KOE_GROOTTE_MULTIPLIER};
	SDL_Rect rechterachterpoot = {koeien->x + 16*KOE_GROOTTE_MULTIPLIER, koeien->y + 12*KOE_GROOTTE_MULTIPLIER, RECHTER_ACHTER_POOT*KOE_GROOTTE_MULTIPLIER, RECHTER_ACHTER_POOT*KOE_GROOTTE_MULTIPLIER};

	// RGB (135, 54, 0) should be a brown fur color for Greta
	SDL_SetRenderDrawColor(renderer, koeien->kleur.r, koeien->kleur.g, koeien->kleur.b, 255);
	// Redraw Grata in the backbuffer to her updated location:
	SDL_RenderFillRect(renderer, &lijf);
	SDL_RenderFillRect(renderer, &kop);
	SDL_RenderFillRect(renderer, &staart);
	SDL_RenderFillRect(renderer, &linkervoorpoot);
	SDL_RenderFillRect(renderer, &rechtervoorpoot);
	SDL_RenderFillRect(renderer, &linkerachterpoot);
	SDL_RenderFillRect(renderer, &rechterachterpoot);
}

int process_input(koe *koeien)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.scancode)
			{
			case SDL_SCANCODE_ESCAPE:
				return 1;
			case SDL_SCANCODE_RIGHT:
				koeien[10].x += 4;
				break;
			case SDL_SCANCODE_LEFT:
				koeien[10].x -= 4;
				break;
			case SDL_SCANCODE_UP:
				koeien[10].y -= 4;
				break;
			case SDL_SCANCODE_DOWN:
				koeien[10].y += 4;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
	return 0;
}

void slapen(SDL_Renderer *renderer, SDL_Window *window)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}