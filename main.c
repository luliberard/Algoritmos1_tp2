#include "nave.h"
#include "caracteres.h"
#include "movimientos.h"
#include "vectores.h"
#include "diccionario.h"
#include "renderizar.h"
#include "config.h"
#include "graficador.h"
#include "objetos.h"
#include "lista.h"
#include "disparo.h"
#include "asteroide.h"
//#include "juego.h"

int main() 
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;

	SDL_CreateWindowAndRenderer(VENTANA_ANCHO, VENTANA_ALTO, 0, &window, &renderer);
	SDL_SetWindowTitle(window, "Asteroids");

	int dormir = 0;
	
	if(graficador_inicializar(ARCHIVO_BIN,VENTANA_ANCHO, VENTANA_ALTO))//Aca levanto todos los sprites del archivo binario
		return EXIT_FAILURE;
	
	//zona de variables-----------------
	nave_t nave;
	float dt = 1/(float)JUEGO_FPS;
	float tiempo = 0;
	float puntos = 0;
	//Tendria que hacer un vector dinamico donde vaya guardando los puntajes y una funcion que me diga cual de todos es el mejor
	float mejor_puntaje = 0; //pero mientras uso esto jeje lol

	inicializar_valores(&nave);//Inicializo los valores de la nave
	lista_t *lista_disparo = lista_crear(); //creo la lista de disparos,debo eliminarla al final		
	lista_t *lista_asteroides = lista_crear();
	int vidas_disponibles = CANT_VIDAS_INICIAL;
	int cant_asteroides = CANT_ASTEROIDES_INICIAL;

	for(size_t i=0; i<= cant_asteroides; i++)//Aca estoy creando los asteroides que aparecen apenas empieza el juego
	{	
		crear_asteroide(lista_asteroides);
	}
	//aca se podria usar la funcion de generar_nuevos_asteroides()

	//end zona de variables-------------------------------
	unsigned int ticks = SDL_GetTicks();
	while(1) {
		if(SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				break;
	    		if (event.type == SDL_KEYDOWN) {
				// BEGIN código del alumno------------------------------------
				switch(event.key.keysym.sym) {
					case SDLK_UP:
						nave.potencia+= INCREMENTO_POTENCIA;
						break;
					case SDLK_SPACE:
						crear_disparo(lista_disparo,nave);
						break;
					case SDLK_RIGHT:				
						nave.angulo -= NAVE_ROTACION_PASO;
						break;
					case SDLK_LEFT:					
						nave.angulo += NAVE_ROTACION_PASO;
						break;
				}
				// END código del alumno--------------------------------------------------
			}
			continue;
		}
        		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        		SDL_RenderClear(renderer);
        		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);

			// BEGIN código del alumno-------------------------------------
			nave_mover(&nave, dt);//Muevo la nave en un fx que modifica todos sus parametros
			
			disparos_modificar(lista_disparo,dt,renderer);
			//Esta fx modifica los parametros de todos los disparos y en simulatneo los grafica, esto lo hice asi para recorrer la lista una sola vez en vez de varias veces
			asteroide_modificar(lista_asteroides,dt,renderer);

			////////////////ZONA DE DIBUJO/////////////////
			if(nave_dibujar(&nave, renderer)==false)
				break;
			/*
			if(colisiono_nave_asteroide(&nave, lista_asteroides)
			{
				vidas_disponibles --;
				if(hay_vidas_disponibles(vidas_disponibles))
					comenzar_nueva_vida(&nave, lista_asteroides);
				else
					comenzar_nueva_partida();
			}
			*/
			//aca de nuevo el problema del mejor_puntaje:
			dibujar_parametros(puntos, mejor_puntaje, caracteres, tam_caracteres, renderer);
			
			dibujar_vidas(&nave, renderer,vidas_disponibles);
			////////////////////////////////////////////////
			
			
		// END código del alumno----------------------------------------------
        	SDL_RenderPresent(renderer);
		ticks = SDL_GetTicks() - ticks;
		if(dormir) {
			SDL_Delay(dormir);

			dormir = 0;
		}
		else if(ticks < 1000 / JUEGO_FPS)
			SDL_Delay(1000 / JUEGO_FPS - ticks);
		ticks = SDL_GetTicks();
	}
	// BEGIN código del alumno------------------------------------------------
	//graficador_finalizar();
	lista_destruir(lista_disparo,free);
	lista_destruir(lista_asteroides,free);

	// END código del alumno--------------------------------------------------

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}

