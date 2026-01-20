Here record all needed script to maintain requirement.

1. Update cmake to v3.20.6
```
wget https://cmake.org/files/v3.20/cmake-3.20.6-linux-x86_64.sh
sudo bash cmake-3.20.6-linux-x86_64.sh --skip-licence --prefix=/usr
cmake --version
```

2. Update git to latest version
```
sudo add-apt-repository ppa:git-core/ppa -y
sudo apt-key adv --recv-keys --keyserver keyserver.ubuntu.com A1715D88E1DF1F24 40976EAF437D05B5 3B4FE6ACC0B21F32 A6616109451BBBF2
sudo apt-get update
sudo apt-get install git -y
git --version
```

3. ar usage
refer to:
[sample usage](https://www.geeksforgeeks.org/c/how-to-create-a-static-library-in-c/)
[-l namespace](https://stackoverflow.com/questions/6578484/telling-gcc-directly-to-link-a-library-statically)