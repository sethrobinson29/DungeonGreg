all: greg

greg: game.cpp dungeon.h characters.h graphics.h
	g++ -o greg game.cpp dungeon.h characters.h graphics.h

clean: 
	$(RM) greg