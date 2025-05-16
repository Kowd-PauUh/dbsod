# install g++
install_g++:
	@ sudo apt-get update
	@ sudo apt-get install g++ -y

# clean artifacts
clean:
	@ rm -rf bin/*
