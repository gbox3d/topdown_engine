
ex01 : ex01.c ../source/core.c ../source/ui/text_lable.c
	gcc -o ../out/$@ $^  -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

ex02 : ex02.c ../source/core.c ../source/ui/text_lable.c ../source/ui/button.c
	gcc -o ../out/$@ $^  -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -g

graph_ex01 : graph_ex01.c ../source/core.c ../source/graph.c
	gcc -o ../out/$@ $^  -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -g

graph_ex02 : graph_ex02.c ../source/core.c ../source/graph.c ../source/ui/text_lable.c ../source/ui/button.c
	gcc -o ../out/$@ $^  -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -g

spr_ex01 : spr_ex01.c ../source/core.c ../source/entity/sprite.c ../source/graph.c
	gcc -o ../out/$@ $^  -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -g

exodos : exodos.c ../source/core.c ../source/tile.c ../source/core.c ../tools/cmd_parser.c
	gcc -o ../out/$@ $^  -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -g

box2d_ex01 : box2d_ex01.cpp ../source/box2d/src/common/b2_math.cpp
	g++ -o ../out/$@ $^  -I ../box2d/include

	