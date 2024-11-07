clean:
	-rm -r "build"

build: dust_sim.cpp
	-mkdir "build"
	g++ .\dust_sim.cpp -o .\build\dust_sim 
run:
	./build/dust_sim