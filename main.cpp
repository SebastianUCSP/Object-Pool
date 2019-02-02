#include <iostream>
#include <cstdlib>
#include <allegro.h>
#include <fstream>
#include <algorithm>


using namespace std;

BITMAP *buffer;
BITMAP *muertebmp;
BITMAP *tanquebmp;
BITMAP *tanque;
BITMAP *ladrillo;
BITMAP *trofeo;
BITMAP *bala;

//pos inicial tanque
int direccion = 0; // tanque y disparo
int direccion_bala;
int pos_x = 30*14; // enemigo tanke bala
int pos_y = 30*15; // enemigo tanque bala

int anterior_pos_x; // enemigo
int anterior_pos_y; // enemigo

//puntaje
int score_1 = 0; // enemigo
int score_2 = 0;
int score_3 = 0;

//estado del jugador
bool vida = true; // enemigo

//mapa
int const ancho = 21; // juego
int const alto = 31; // juego

//bala

int pos_b_x; // enemigo  y bala
int pos_b_y; // enemigo  y bala
int origen_b_x;  // bala
int origen_b_y;  // bala

class Juego{

public:
    static Juego *getInstancia(){
            if(instancia == 0)
                instancia = new Juego();
            return instancia;
        }
    void crearMapa();
    bool finJuego();
    void imprimirSprite();
    void Inicializar();

protected:

    char mapa[ancho][alto]={
       "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
       "X            | |            X",
       "X                           X",
       "X      X             X      X",
       "X      X             X      X",
       "X      X             X      X",
       "X|     X             X     |X",
       "X    XXXXX         XXXXX    X",
       "X             X             X",
       "X             X             X",
       "X      |      X      |      X",
       "X         XXXXXXXXX         X",
       "X             X             X",
       "X        |    X    |        X",
       "XXXXXX        X        XXXXXX",
       "X                           X",
       "X                           X",
       "X           XXXXX           X",
       "X                           X",
       "X             o             X",
       "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    };

    private:
        static Juego *instancia;
};

void Juego::crearMapa()
{
   for(int fila = 0; fila < ancho; fila++)
    {
        for(int columna = 0; columna < alto; columna++)
        {
            if(mapa[fila][columna] == 'X')
            {
                draw_sprite(buffer, ladrillo, columna*30, fila*30);
            }
            else if(mapa[fila][columna] == 'o')
            {
                draw_sprite(buffer, trofeo, columna*30, fila*30);
            }
        }
   }
}


bool Juego::finJuego()
{
   for(int fila = 0; fila < ancho; fila++)
    {
        for(int columna = 0; columna < alto; columna++)
        {
            if(mapa[fila][columna] == 'o')
            {
                return true;
            }
        }
   }
   return false;
}


void Juego::imprimirSprite()
{
   blit(buffer, screen, 0,0,0,0,880,630);
}

void Juego::Inicializar()
{
    buffer = create_bitmap(880, 630);
    ladrillo = load_bitmap("bloque.bmp",NULL);
    tanquebmp = load_bitmap("tanque.bmp",NULL);
    tanque = create_bitmap(33, 33);
    trofeo = load_bitmap("trofeo.bmp",NULL);
    bala = load_bitmap("bala.bmp",NULL);
    muertebmp = load_bitmap("muerte.bmp", NULL);

}

class Sprite
{
    public:
        virtual void CrearSprite() = 0;
};

class Enemigo: public Sprite, public Juego
{
    public:
        Enemigo();
        Enemigo(int, int);
        ~Enemigo();
        void init(int x, int y){
            pos_x_e = x;
            pos_y_e = y;
        }
        void CrearSprite();
        void Colision();
        void movimientoEnemigo();
        void crear(){
            if(!enUso()) return;
            cantidad--;
            Enemigo();
        }
        bool enUso() const {return cantidad > 0; }



    private:
        BITMAP *enemigobmp;
        BITMAP *enemigo;
        //pos enemigo
        int direccion_enemigo = 0;
        int pos_x_e;
        int pos_y_e;
        int anterior_pos_x_e;
        int anterior_pos_y_e;
        int cantidad;
};


Enemigo::Enemigo()
{
    pos_x_e = 30*14;
    pos_y_e = 30*8;
    direccion_enemigo = rand()%4;
    enemigo = create_bitmap(30, 30);
    enemigobmp = load_bitmap("enemigo1.bmp",NULL);
    cantidad = 0;
}


Enemigo::Enemigo(int x, int y)
{
    pos_x_e = x;
    pos_y_e = y;
    direccion_enemigo = rand()%4;
    enemigo = create_bitmap(30, 30);
    enemigobmp = load_bitmap("enemigo1.bmp",NULL);
}
Enemigo::~Enemigo(){
    delete enemigo;
}


void Enemigo::CrearSprite()
{
    blit(enemigobmp, enemigo, 0,0,0,0,30,30);
    draw_sprite(buffer, enemigo, pos_x_e, pos_y_e);
}


void Enemigo::Colision()
{
    if((pos_y == pos_y_e && pos_x == pos_x_e) || (pos_y_e == anterior_pos_y && pos_x_e == anterior_pos_x)){
        for(int i = 0; i <= 5; i++){
            clear(tanque);
            clear(buffer);
            crearMapa();

            blit(muertebmp, tanque, i*33,0,0,0,33,33);
            draw_sprite(buffer, tanque,pos_x, pos_y);

            imprimirSprite();
            rest(80);
            vida = false;
        }
    }

    if((pos_b_y == pos_y_e && pos_b_x == pos_x_e)){
        pos_x_e = 30*50;
        pos_y_e = 30*50;
        score_1 += 10;
    }

    if(pos_x_e == 30*14 && pos_y_e == 30*19){
        vida = false;
    }
}

void Enemigo::movimientoEnemigo()
{
   CrearSprite();
   Colision();

   if(mapa[pos_y_e/30][pos_x_e/30] == '|')
    {
           direccion_enemigo = rand()%4;
    }
   if(direccion_enemigo == 0)
    {
       if(mapa[pos_y_e/30][(pos_x_e-30)/30] != 'X')
       {
           pos_x_e -= 30;
       }
       else direccion_enemigo = rand()%4;
   }
   if(direccion_enemigo == 1)
   {
       if(mapa[pos_y_e/30][(pos_x_e+30)/30] != 'X')
       {
           pos_x_e += 30;
       }
       else direccion_enemigo = rand()%4;
   }
   if(direccion_enemigo == 2)
   {
       if(mapa[(pos_y_e-30)/30][pos_x_e/30] != 'X')
       {
           pos_y_e -= 30;
       }
       else direccion_enemigo = rand()%4;
   }
   if(direccion_enemigo == 3)
   {
       if(mapa[(pos_y_e+30)/30][pos_x_e/30] != 'X')
       {
           pos_y_e += 30;
       }
       else direccion_enemigo = rand()%4;
   }
}

class Enemigo_Pool
{
public:
    void crear_Enemigo(int x, int y);
    void creacion()
    {
        for(int i = 0; i < POOL_SIZE; i++)
        {
            enemigos[i].crear();
        }
    }

    private:
        static const int POOL_SIZE = 20;
        Enemigo enemigos[POOL_SIZE];
};

void Enemigo_Pool::crear_Enemigo(int x, int y)
{
    for(int i = 0; i < POOL_SIZE; i++)
    {
        if(!enemigos[i].enUso())
        {
            enemigos[i].init(x, y);
            return;
        }
    }
}

class Tanque: public Sprite, public Juego
{
    public:
        bool activo;
        void CrearSprite();
        void direccionamiento(int &, int &, int &);
};

void Tanque::direccionamiento(int &direccion, int &pos_x, int &pos_y)
{

       if(key[KEY_LEFT]){
           direccion = direccion_bala = 0;
            activo = true;
        }
       else if(key[KEY_RIGHT]){
           direccion = direccion_bala = 1;
            activo = true;
        }
       else if(key[KEY_UP]){
           direccion = direccion_bala = 2;
            activo = true;
        }
       else if(key[KEY_DOWN]){
           direccion =direccion_bala =  3;
            activo = true;
        }
           if(direccion == 0 && activo == true){
                if(mapa[pos_y/30][(pos_x-30)/30] != 'X'){
                   pos_x -= 30;
                   activo = false;
                }
                else
                    direccion = 0;
           }

           if(direccion == 1 && activo == true){
                if(mapa[pos_y/30][(pos_x+30)/30] != 'X'){
                   pos_x += 30;
                    activo = false;
                }
                else
                    direccion = 1;
           }

           if(direccion == 2 && activo == true){
                if(mapa[(pos_y-30)/30][pos_x/30] != 'X'){
                  pos_y -= 30;
                  activo = false;
                }
                else
                    direccion = 2;
           }

           if(direccion == 3 && activo == true){
                if(mapa[(pos_y+30)/30][pos_x/30] != 'X'){
                   pos_y += 30;
                   activo = false;
                }
                else
                    direccion = 3;
           }
}

void Tanque::CrearSprite()
{
    blit(tanquebmp, tanque, direccion*33,0,0,0,33,33);
    draw_sprite(buffer, tanque, pos_x, pos_y);
}

class Bala: public  Sprite, public Juego{
    public:
        void disparo(int &, int&,int );
        void CrearSprite();

};


void Bala::disparo(int &pos_b_x, int &pos_b_y,int direccion)
{
    origen_b_x = pos_x;
    origen_b_y = pos_y;

    if(direccion == 0){
        if(key[KEY_SPACE]){
            pos_b_x = origen_b_x;
            pos_b_y = origen_b_y;
        }
        pos_b_x -= 30;
        if(mapa[pos_b_y/30][pos_b_x/30] == 'X')
            pos_b_x = 30*100;
    }

    if(direccion == 1){
        if(key[KEY_SPACE]){
            pos_b_x = origen_b_x;
            pos_b_y = origen_b_y;
        }
        pos_b_x += 30;
        if(mapa[pos_b_y/30][pos_b_x/30] == 'X')
            pos_b_x = 30*100;
    }

    if(direccion == 2){
        if(key[KEY_SPACE]){
            pos_b_x = origen_b_x;
            pos_b_y = origen_b_y;
        }
        pos_b_y -= 30;
        if(mapa[pos_b_y/30][pos_b_x/30] == 'X')
            pos_b_x = 30*100;
    }

    if(direccion == 3){
        if(key[KEY_SPACE]){
            pos_b_x = origen_b_x;
            pos_b_y = origen_b_y;
        }
        pos_b_y += 30;
        if(mapa[pos_b_y/30][pos_b_x/30] == 'X')
            pos_b_x = 30*100;
    }
}

void Bala::CrearSprite()
{
    draw_sprite(buffer, bala, pos_b_x, pos_b_y);
}

Juego *Juego::instancia = 0;

int main()
{
    allegro_init();
	install_keyboard();

	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 880, 630, 0, 0);

	//int puntajes[] = {score_1, score_2, score_3};
	//sort(puntajes, puntajes+3);

	Juego *configuracion = Juego::getInstancia();
    configuracion->Inicializar();

    Enemigo_Pool pool;
    pool.creacion();




/*
    Enemigo *A = new Enemigo;
    Enemigo *B = new Enemigo;
    Enemigo *C = new Enemigo;
    Enemigo *D = new Enemigo;
    Enemigo *E = new Enemigo;
    Enemigo *F = new Enemigo;
*/

    Tanque *tan = new Tanque;
    Bala *balin = new Bala;

    bool nivel1 = true;
    bool nivel2 = false;


    while(!key[KEY_ESC] && configuracion->finJuego() && vida && nivel1){

       anterior_pos_x = pos_x;
       anterior_pos_y = pos_y;

       tan->direccionamiento(direccion, pos_x, pos_y);
       balin->disparo(pos_b_x, pos_b_y,direccion_bala);


       clear(buffer);
	   configuracion->crearMapa();
	   tan->CrearSprite();
	   balin->CrearSprite();




/*
	   A->movimientoEnemigo();
	   B->movimientoEnemigo();
	   C->movimientoEnemigo();
	   D->movimientoEnemigo();
*/
	   configuracion->imprimirSprite();
	   rest(20);

	   clear(tanque);
       configuracion->imprimirSprite();
       rest(70);

       if(score_1 == 40){
        nivel1 = false;
       }

       ofstream guardar_puntaje;
       guardar_puntaje.open("Puntuacion_obtenida_1.txt");

       guardar_puntaje << "Tu puntuacion fue:" << score_1;
       guardar_puntaje.close();
    }

    if(nivel1 == false)
        nivel2 = true;
/*
    while(!key[KEY_ESC] && configuracion->finJuego() && vida && nivel2){

       anterior_pos_x = pos_x;
       anterior_pos_y = pos_y;

       tan->direccionamiento(direccion, pos_x, pos_y);
       balin->disparo(pos_b_x, pos_b_y,direccion_bala);


       clear(buffer);
	   configuracion->crearMapa();
	   tan->CrearSprite();
	   balin->CrearSprite();


	   A->movimientoEnemigo();
	   B->movimientoEnemigo();
	   C->movimientoEnemigo();
	   D->movimientoEnemigo();
	   E->movimientoEnemigo();
	   F->movimientoEnemigo();

	   configuracion->imprimirSprite();
	   rest(20);

	   clear(tanque);
       configuracion->imprimirSprite();
       rest(70);

       score_2 = score_1-40;

       if(score_1 == 100){
        nivel1 = false;
       }


       ofstream guardar_puntaje;
       guardar_puntaje.open("Puntuacion_obtenida_2.txt");

       guardar_puntaje << "Tu puntuacion fue:" << score_2;
       guardar_puntaje.close();
    }
*/

	return 0;
}
END_OF_MAIN();
