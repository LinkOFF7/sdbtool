CC=g++

all:
	$(CC) sdbtool.cpp -o sdbtool
	
clean:
	rm -rf *.o sdbtool