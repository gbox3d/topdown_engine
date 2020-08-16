
ex01 : ex01.c ../source/core.c ../source/ui/text_lable.c
	gcc -o ../out/$@ $^  -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

ex02 : ex02.c ../source/core.c ../source/ui/text_lable.c ../source/ui/button.c
	gcc -o ../out/$@ $^  -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -g

graph_ex01 : graph_ex01.c ../source/core.c ../source/graph.c
	gcc -o ../out/$@ $^  -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -g

graph_ex02 : graph_ex02.c ../source/core.c ../source/graph.c ../source/ui/text_lable.c ../source/ui/button.c
	gcc -o ../out/$@ $^  -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -g

spr_ex01 : spr_ex01.c ../source/core.c ../source/core.c ../source/entity/sprite.c
	gcc -o ../out/$@ $^  -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -g