EXEC=editor
SOURCES=editor.c
OBJECTS=$(SOURCES:.c=.o)
CC=gcc
CFLAGS=-W -Wall -Werror -std=gnu99 -g
 
.PHONY: default clean
 
default: $(EXEC)


editor.o: editor.c

 
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^ 

clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) Makefile~
