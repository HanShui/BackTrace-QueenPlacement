objects = QueenPlacement.o
QueenPlacement : (objects)
	cc -o QueenPlacement $(objects)
	
.PHONY : clean
clean : 
	rm QueenPlacement $(objects)