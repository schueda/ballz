PROGS = ballz
INCDIR = .
ALLEGRO = -lallegro_image -lallegro_primitives -lallegro_dialog -lallegro_ttf -lallegro_font -lallegro -lallegro_main
CFLAGS = -I $(INCDIR) -std=c99 -Wall
LIBS = $(ALLEGRO)

CC = gcc

%.o:  %.c
	$(CC) $(CFLAGS) -c $<

debug: CFLAGS += -g -D__DEBUG__

all debug: $(PROGS)

$(PROGS) : % : %.o grafico.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	@echo "Limpando sujeira ..."
	@rm -f *% *.bak *~

purge: clean
	@echo "Limpando tudo ..."
	@rm -rf core *.o
	@rm -f $(PROGS)

