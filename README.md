# Vishnu
Copyright (c) 2017-2019 GMRV/URJC.

## Introduction

[DC Explorer](http://cajalbbp.es/dcexplorer_web/ "DC Explorer"), [Pyramidal Explorer](http://gmrv.es/gmrvvis/pyramidalexplorer/ "Pyramidal Explorer") and [ClInt Explorer](http://gmrv.es/gmrvvis/clintexplorer/ "ClInt Explorer") are the core of an application suite 
designed to help scientists to explore their data. Vishnu is a communication 
framework that allows them to interchange information and cooperate in 
real-time.

## Dependencies

* VishnuCommon
* ManCo
* EspinaCore (optional)
* EspinaExtensions (optional)

## Building

Vishnu has been successfully built and tested on Ubuntu 18.04/Windows 10.
The following steps are for build:

```bash
$ git clone https://github.com/gmrvvis/Vishnu.git
$ mkdir Vishnu/build && cd Vishnu/build
$ cmake .. [-DCLONE_SUBPROJECTS=ON]
```

## License

GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
