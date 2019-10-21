cc=c++
	${cc} main.cpp -o main -lpthread -O3
	${cc} 'test.cpp' -o ts -lpthread -O3
	./ts $1 $2 $3 data.txt
	rm data.txt
