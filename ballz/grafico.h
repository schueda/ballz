/* Allegro */
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

#include "game_helpers.h"

typedef struct {
  unsigned char R, G, B;
} Pixel;

/* Macros de Pixels para uso no Allegro */
#define PIXEL(r,g,b) (al_map_rgb((unsigned char) r, (unsigned char) g, (unsigned char) b))

typedef struct {
  	ALLEGRO_FONT *title_font;
	ALLEGRO_FONT *big_font;
	ALLEGRO_FONT *medium_font;
	ALLEGRO_FONT *small_font;
} fonts_t;

typedef struct {
	ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_DISPLAY *display;
	ALLEGRO_DISPLAY_MODE disp_data;
	fonts_t *fonts;
} window;

/* Inicia sistema grafico Allegro 5.0 */
window graph_init(int res_width, int res_height);

void draw_menu(window *win);
void draw_setup(window *win, bouncer_t *bouncer, int squares[][COLUNAS_QUADRADO], float offsetY, game_t *game);
void draw_wait(window *win, bouncer_t *bouncer, int squares[][COLUNAS_QUADRADO], game_t *game);
void draw_aim(window *win, bouncer_t *bouncer, float distX, float distY, float dist, int squares[][COLUNAS_QUADRADO], game_t *game);
void draw_shoot(window *win, bouncer_t **bouncer, int balls, int squares[][COLUNAS_QUADRADO], game_t *game);
void draw_gameover(window *win, game_t *game);



/* Finaliza sistema grafico Allegro 5.0 */
void graph_deinit(window win);

/* Lista de cores */
#define ACAFRAO                   PIXEL(244,196,48)
#define AGUA_MARINHA              PIXEL(127,255,212)
#define AGUA_MARINHA_MEDIA        PIXEL(102,205,170)
#define ALIZARINA                 PIXEL(227,38,54)
#define AMARELO                   PIXEL(255,255,0)
#define AMARELO_CLARO             PIXEL(255,255,224)
#define AMARELO_ESVERDEADO        PIXEL(173,255,47)
#define AMARELO_OURO_CLARO        PIXEL(250,250,210)
#define AMARELO_QUEIMADO          PIXEL(238,173,45)
#define AMBAR                     PIXEL(255,191,0)
#define AMEIXA                    PIXEL(221,160,221)
#define AMENDOA                   PIXEL(255,235,205)
#define AMETISTA                  PIXEL(153,102,204)
#define ASPARGO                   PIXEL(123,160,91)
#define AZUL                      PIXEL(0,0,255)
#define AZUL_ACO                  PIXEL(70,130,180)
#define AZUL_ACO_CLARO            PIXEL(176,196,222)
#define AZUL_ALICE                PIXEL(240,248,255)
#define AZUL_ARDOSIA              PIXEL(106,90,205)
#define AZUL_ARDOSIA_CLARO        PIXEL(132,112,255)
#define AZUL_ARDOSIA_ESCURO       PIXEL(72,61,139)
#define AZUL_ARDOSIA_MEDIO        PIXEL(123,104,238)
#define AZUL_AREADO               PIXEL(184,202,212)
#define AZUL_CADETE               PIXEL(95,158,160)
#define AZUL_CAMARADA             PIXEL(5,79,119)
#define AZUL_CELESTE              PIXEL(135,206,235)
#define AZUL_CELESTE_PALIDO       PIXEL(240,255,255)
#define AZUL_CELESTE_BRILHANTE    PIXEL(0,127,255)
#define AZUL_CELESTE_CLARO        PIXEL(135,206,250)
#define AZUL_CELESTE_PROFUNDO     PIXEL(0,191,255)
#define AZUL_CLARO                PIXEL(173,216,230)
#define AZUL_COBALTO              PIXEL(0,71,171)
#define AZUL_ESCURO               PIXEL(0,0,139)
#define AZUL_FLOR_DE_MILHO        PIXEL(100,149,237)
#define AZUL_FORCA_AEREA          PIXEL(93,138,168)
#define AZUL_FURTIVO              PIXEL(30,144,255)
#define AZUL_MARINHO              PIXEL(18,10,143)
#define AZUL_MEDIO                PIXEL(0,0,205)
#define AZUL_MEIA_NOITE           PIXEL(25,25,112)
#define AZUL_NAVAL                PIXEL(0,0,128)
#define AZUL_PETROLEO             PIXEL(8,77,110)
#define AZUL_REAL                 PIXEL(0,0,221)
#define AZUL_TALCO                PIXEL(176,224,230)
#define AZUL_TAPARUERE            PIXEL(36,142,255)
#define AZUL_VIOLETA              PIXEL(138,43,226)
#define BEGE                      PIXEL(245,245,220)
#define BORDO                     PIXEL(128,0,0)
#define BORGONHA                  PIXEL(144,0,32)
#define BRANCO                    PIXEL(255,255,255)
#define BRANCO_ANTIGO             PIXEL(250,235,215)
#define BRANCO_FANTASMA           PIXEL(248,248,255)
#define BRANCO_FLORAL             PIXEL(255,250,240)
#define BRANCO_FUMACA             PIXEL(245,245,245)
#define BRANCO_NAVAJO             PIXEL(255,222,173)
#define BRONZE                    PIXEL(205,127,50)
#define CAQUI                     PIXEL(240,230,140)
#define CAQUI_ESCURO              PIXEL(189,183,107)
#define CARAMELO                  PIXEL(139,87,66)
#define CARDO                     PIXEL(216,191,216)
#define CARMESIM                  PIXEL(220,20,60)
#define CARMIM                    PIXEL(113,47,38)
#define CARMIM_CARNACEO           PIXEL(256,244,240)
#define CASTANHO_AVERMELHADO      PIXEL(138,0,0)
#define CASTANHO_CLARO            PIXEL(210,180,140)
#define CENOURA                   PIXEL(237,145,33)
#define CEREJA                    PIXEL(222,49,99)
#define CEREJA_HOLLYWOOD          PIXEL(244,0,161)
#define CHOCOLATE                 PIXEL(210,105,30)
#define CIANO                     PIXEL(0,255,255)
#define CIANO_CLARO               PIXEL(224,255,255)
#define CIANO_ESCURO              PIXEL(0,139,139)
#define CINZA                     PIXEL(128,128,128)
#define CINZA_ARDOSIA             PIXEL(112,128,144)
#define CINZA_ARDOSIA_CLARO       PIXEL(119,136,153)
#define CINZA_ARDOSIA_ESCURO      PIXEL(47,79,79)
#define CINZA_CLARO               PIXEL(211,211,211)
#define CINZA_ESCURO              PIXEL(30,30,30)
#define CINZA_FOSCO               PIXEL(105,105,105)
#define CINZA_MEDIO               PIXEL(220,220,220)
#define COBRE                     PIXEL(184,115,51)
#define CONCHA                    PIXEL(255,245,238)
#define CORAL                     PIXEL(255,127,80)
#define CORAL_CLARO               PIXEL(240,128,128)
#define COURO                     PIXEL(240,220,130)
#define CREME                     PIXEL(255,253,208)
#define CREME_DE_MARISCO          PIXEL(255,228,196)
#define CREME_DE_MENTA            PIXEL(245,255,250)
#define DOURADO                   PIXEL(218,165,32)
#define DOURADO_ESCURO            PIXEL(184,134,11)
#define DOURADA_LINHACA           PIXEL(166,170,62)
#define DOURADO_PALIDO            PIXEL(238,232,170)
#define ESCARLATE                 PIXEL(255,36,0)
#define ESMERALDA                 PIXEL(80,200,120)
#define FELDSPATO                 PIXEL(209,146,117)
#define FERRUGEM                  PIXEL(183,65,14)
#define FULIGEM                   PIXEL(61,43,31)
#define GRENA                     PIXEL(131,29,28)
#define HERBAL                    PIXEL(46,139,87)
#define INDIGO                    PIXEL(75,0,130)
#define JADE                      PIXEL(0,168,107)
#define JAMBO                     PIXEL(255,69,0)
#define LARANJA                   PIXEL(255,165,0)
#define LARANJA_ESCURO            PIXEL(255,140,0)
#define LAVANDA                   PIXEL(230,230,250)
#define LAVANDA_AVERMELHADA       PIXEL(255,240,245)
#define LILAS                     PIXEL(200,162,200)
#define LIMA                      PIXEL(253,233,16)
#define LINHO                     PIXEL(250,240,230)
#define MADEIRA                   PIXEL(222,184,135)
#define MAGENTA                   PIXEL(255,0,255)
#define MAGENTA_ESCURO            PIXEL(139,0,139)
#define MALVA                     PIXEL(224,176,255)
#define MAMAO_BATIDO              PIXEL(255,239,213)
#define MANA                      PIXEL(240,255,240)
#define MARFIM                    PIXEL(255,255,240)
#define MARROM                    PIXEL(150,75,0)
#define MARROM_AMARELADO          PIXEL(244,164,96)
#define MARROM_CLARO              PIXEL(165,42,42)
#define MARROM_ROSADO             PIXEL(188,143,143)
#define MARROM_SELA               PIXEL(139,69,19)
#define MILHO                     PIXEL(251,236,93)
#define MILHO_CLARO               PIXEL(255,248,220)
#define MOCASSIM                  PIXEL(255,228,181)
#define MOSTARDA                  PIXEL(255,219,88)
#define NEVE                      PIXEL(255,250,250)
#define OCRE                      PIXEL(204,119,34)
#define ORQUIDEA                  PIXEL(218,112,214)
#define ORQUIDEA_ESCURA           PIXEL(153,50,204)
#define ORQUIDEA_MEDIA            PIXEL(186,85,211)
#define OURO                      PIXEL(255,215,0)
#define PESSEGO                   PIXEL(255,218,185)
#define PRATA                     PIXEL(192,192,192)
#define PRETO                     PIXEL(0,0,0)
#define PURPURA                   PIXEL(128,0,128)
#define PURPURA_MEDIA             PIXEL(147,112,219)
#define QUANTUM                   PIXEL(17,17,17)
#define RENDA_ANTIGA              PIXEL(253,245,230)oldlace
#define ROSA                      PIXEL(255,203,219)
#define ROSA_BRILHANTE            PIXEL(255,0,127)
#define ROSA_CHOQUE               PIXEL(252,15,192)
#define ROSA_CLARO                PIXEL(255,182,193)
#define ROSA_EMBACADO             PIXEL(255,228,225)
#define ROSA_FORTE                PIXEL(255,105,180)
#define ROSA_PROFUNDO             PIXEL(255,20,147)
#define ROXO                      PIXEL(153,51,153) 
#define RUTILO                    PIXEL(109,53,26)
#define SALMAO                    PIXEL(250,127,114)
#define SALMAO_CLARO              PIXEL(255,160,122)
#define SALMAO_ESCURO             PIXEL(233,150,122)
#define SEPIA                     PIXEL(112,87,20)
#define SIENA                     PIXEL(255,130,71)
#define TERRACOTA                 PIXEL(226,114,91)
#define TIJOLO_REFRATARIO         PIXEL(178,34,34)
#define TOMATE                    PIXEL(255,99,71)
#define TRIASSICO                 PIXEL(255,36,1)
#define TRIGO                     PIXEL(245,222,179)
#define TURQUESA                  PIXEL(64,224,208)
#define TURQUESA_ESCURA           PIXEL(0,206,209)
#define TURQUESA_MEDIA            PIXEL(72,209,204)
#define TURQUESA_PALIDA           PIXEL(175,238,238)
#define URUCUM                    PIXEL(236,35,0)
#define VERDE                     PIXEL(0,128,0)
#define VERDE_AMARELADO           PIXEL(154,205,50)
#define VERDE_AZULADO             PIXEL(0,128,128)
#define VERDE_CLARO               PIXEL(144,238,144)
#define VERDE_ESCURO              PIXEL(0,100,0)
#define VERDE_FLORESTA            PIXEL(34,139,34)
#define VERDE_FLUORESCENTE        PIXEL(204,255,51)
#define VERDE_GRAMA               PIXEL(124,252,0)
#define VERDE_LIMA                PIXEL(50,205,50)
#define VERDE_LIMAO               PIXEL(0,255,0)
#define VERDE_MAR_CLARO           PIXEL(32,178,170)
#define VERDE_MAR_ESCURO          PIXEL(143,188,143)
#define VERDE_MAR_MEDIO           PIXEL(60,179,113)
#define VERDE_MILITAR             PIXEL(120,134,107)
#define VERDE_OLIVA               PIXEL(128,128,0)
#define VERDE_OLIVA_ESCURA        PIXEL(85,107,47)
#define VERDE_OLIVA_PARDO         PIXEL(107,142,35)
#define VERDE_PALIDO              PIXEL(152,251,152)
#define VERDE_PARIS               PIXEL(127,255,0)
#define VERDE_PRIMAVERA           PIXEL(0,255,127)
#define VERDE_PRIMAVERA_MEDIO     PIXEL(0,250,154)
#define VERMELHO                  PIXEL(255,0,0)
#define VERMELHO_ESCURO           PIXEL(139,0,0)
#define VERMELHO_INDIANO          PIXEL(205,92,92)
#define VERMELHO_VIOLETA          PIXEL(208,32,144)
#define VERMELHO_VIOLETA_MEDIO    PIXEL(199,21,133)
#define VERMELHO_VIOLETA_PALIDO   PIXEL(219,112,147)
#define VIOLETA                   PIXEL(238,130,238)
#define VIOLETA_CLARO             PIXEL(248,203,248)
#define VIOLETA_ESCURO            PIXEL(148,0,211)
