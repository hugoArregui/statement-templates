test: clean

	mkdir -p bin
	g++ -Isrc/ -IsampleUses/ test/test.cpp sampleUses/parallelFor.cpp -o bin/test -Wall -Wextra -pedantic -std=c++11 -lpthread -ggdb3
	bin/test

pack: clean

	tar zcvf statement-templates.tgz --exclude=statement-templates.tgz *

clean: 

	rm -fr bin

