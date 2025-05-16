# install g++
install_g++:
	@ sudo apt-get update
	@ sudo apt-get install g++ -y

# create .venv, install dependencies and dbsod in dev mode
venv: 
	@ python3 -m venv .venv
	@ bash -c "source .venv/bin/activate && pip install -r requirements.txt && pip install -e ."

# clean artifacts
clean:
	@ rm -rf bin/*
