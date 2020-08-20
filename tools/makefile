CXX?=c++
CC?=cc
SDL2FLAGS=$(shell pkg-config sdl2  --cflags --libs)
SDL2IMAGEFLAGS=$(shell pkg-config sdl2_image  --cflags --libs)
SDL2TTFFLAGS=$(shell pkg-config sdl2_ttf  --cflags --libs)
CXXFLAGS?=-std=c99 -Wall -pedantic -Werror -Wshadow -Wstrict-aliasing -Wstrict-overflow

map_editor_mac : map_editor.c ../source/core.c ../source/tile.c
	${CC} -O2 -o ../out/$@ $^ ${SDL2FLAGS} ${SDL2IMAGEFLAGS} $(SDL2TTFFLAGS) ${CXXFLAGS}

map_editor_win : map_editor.c ../source/core.c ../source/tile.c
	gcc -O2 -g -o ../out/$@ $^ -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf 
