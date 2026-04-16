# Radar Defense
**A Godot GDExtension Project**

## on first download

execute in root:   
``` bash
git submodule update --init --recursive
```

to compile the code with headerInformation run
```bash
python.exe -m SCons platform=windows target=template_debug compiledb=yes -j8 
```

To compile the code run
``` bash
python.exe -m SCons platform=windows target=template_debug -j8
```

to clean the code run
```bash
python.exe -m SCons -c
```
