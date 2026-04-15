# install g++
install_g++:
	@ sudo apt-get update
	@ sudo apt-get install g++ -y

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

# run tests
test:
	@ bash -c "source .venv/bin/activate && pytest tests/ -v --capture=sys"
