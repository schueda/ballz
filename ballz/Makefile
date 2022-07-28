     PROGS = exemplo teste-display teste-mouse teste-teclas
     INCDIR = .
     ALLEGRO = `pkg-config --libs allegro-5 allegro_image-5 allegro_primitives-5 allegro_dialog-5 allegro_font-5 allegro_ttf-5`
         CFLAGS = -I $(INCDIR)
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

