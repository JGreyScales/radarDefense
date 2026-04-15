# Radar Defense

## on first download

execute in root:   
``` bash
git submodule update --init --recursive
```



To compile the code run
``` bash
python.exe -m SCons platform=windows target=template_debug -j8
```

to clean the code run
```bash
python.exe -m SCons -c
```