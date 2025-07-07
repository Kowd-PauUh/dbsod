# install g++
install_g++:
	@ sudo apt-get update
	@ sudo apt-get install g++ -y

# install eigen
install_eigen:
	@ sudo apt-get update
	@ sudo apt-get install libeigen3-dev -y

# create .venv, install dependencies and dbsod in dev mode
venv: 
	@ python3 -m venv .venv
	@ bash -c "source .venv/bin/activate && pip install -r requirements.txt && pip install -e ."

# build dbsod.so
build:
	@ mkdir -p dbsod/build
	@ dpkg -s libeigen3-dev >/dev/null 2>&1 || ( $(MAKE) --no-print-directory install_eigen )
	@ g++ -O3 -Wall -shared -std=c++11 \
	      -fPIC $(wildcard dbsod_cpp/*.cpp) \
		  -Iinclude -I/usr/include/eigen3 \
		  -o dbsod/build/dbsod.so

# launch jupyterlab for examples
notebook:
	@ bash -c "source .venv/bin/activate && jupyter lab --ip=0.0.0.0 --port 8502 --NotebookApp.token=''"
