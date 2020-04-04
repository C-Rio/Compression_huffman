all : compression decompression

compression : compression.c Huffmann.c Huffmann.h
		gcc compression.c Huffmann.c Huffmann.h -o compression -lm -Wall

decompression : decompression.c Huffmann.c Huffmann.h
		gcc decompression.c Huffmann.c Huffmann.h -o decompression -lm -Wall

clean :
	rm -rf *.o compression decompression
