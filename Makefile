# install g++
install_g++:
	@ sudo apt-get update
	@ sudo apt-get install g++ -y

# install gtest
install_gtest:
	@ sudo apt-get update
	@ sudo apt-get install libgtest-dev -y

# create .venv, install dependencies and dbsod in dev mode
venv:
	@ uv venv
	@ uv pip install -r requirements.txt --python .venv
	@ uv pip install -e . --python .venv

# reinstall package, effectively rebuilding .so file
rebuild:
	@ uv pip install -e . --python .venv

# launch jupyterlab for examples
notebook:
	@ bash -c "source .venv/bin/activate && jupyter lab --ip=0.0.0.0 --port 8502 --NotebookApp.token=''"

# run cpp tests
test_cpp:
	@ g++ -std=c++23 \
		$(filter-out dbsod_cpp/bindings.cpp, $(wildcard dbsod_cpp/*.cpp)) \
		$(wildcard tests/*.cpp) \
		-Iinclude -O3 -g -Wall -o test -lgtest -lgtest_main
	@ ./test
	@ rm test

# run python tests
test_py:
	@ bash -c "source .venv/bin/activate && pytest tests/ -v --capture=sys"

# run tests
test: test_cpp rebuild test_py
