# install g++
install_g++:
	@ sudo apt-get update
	@ sudo apt-get install g++ -y

# install eigen
install_eigen:
	@ sudo apt-get update
	@ sudo apt-get install libeigen3-dev

# create .venv, install dependencies and dbsod in dev mode
venv: 
	@ python3 -m venv .venv
	@ bash -c "source .venv/bin/activate && pip install -r requirements.txt && pip install -e ."

# build dbsod.so
build:
	@ mkdir -p dbsod/build
	@ g++ -O3 -Wall -shared -std=c++11 \
	      -fPIC $(wildcard dbsod_cpp/*.cpp) \
		  -Iinclude -I/usr/include/eigen3 \
		  -o dbsod/build/dbsod.so
