all: clean Final03

Final03:
	clear
	g++ main.cpp library.cpp book.cpp page.cpp user.cpp -I . -o Final03
	./Final03

run: 
	./Final03

clean:
	rm -f Final03
