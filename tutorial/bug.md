## PrIntFuzz
Record every bugs when reproduce PrIntFuzz, use patch/printfuzz.patch to fix it.

1. update cmake version (ref to common.md)
2. update git version (ref to common.md)
3. "fatal error: 'filesystem' file not found" llvm version is 14.0.0, [solution](https://blog.csdn.net/BIT_HXZ/article/details/127297351)

## just
1. sudo apt install just:
    err-msg: "E: Unable to locate package just"
    solution 1: install snap, then use snap install just
    result 1: Work. But I'm not familiar with snap.
    solution 2: set up [Prebuilt-MPR set up](https://docs.makedeb.org/prebuilt-mpr/getting-started/#setting-up-the-repository), then use apt install just
    result 2: makedeb unmaintained
    solution 3: use makedeb repo
    result 3: makedeb repo unmaintained
    
    Conclusion: Don't use just on Ubuntu if Ubuntu Distro not 24.04, or be familiar to snap.
