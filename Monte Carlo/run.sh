cc=c++
	${cc} main.cpp -o main -lpthread
	${cc} 'test.cpp' -o ts -lpthread
	./ts $1 $2 $3 data.txt
	rm data.txt
