# UltraPse
**An extensible software platform for biological sequence representations**

## Installation

Source code in this repositiory is for Ubuntu Linux platform. We have tested these codes on **Ubuntu 14.04.5 LTS**. There is no guarantee that these codes can be compiled and executed on other platforms without modifications.

These codes depend on the lua-devel version 5.3. It has been proven that lower version will **NOT** work. Please go to http://www.lua.org/download.html. Download version 5.3 lua environment and configure that on your machine.

After your lua environment is ready, find out the location of your lua.hpp and liblua.a. They usually reside in positions like 
`xxxx/lua-5.3.4/install/include` and `xxxx/lua-5.3.4/install/lib`, where xxxx is usually the directory you unpack your lua source codes.

Suppose the xxxx equals to ~/bin. You should then **EDIT** the following two lines of the Makefile of UltraPse.

EXT_HEADER=`~/bin/lua-5.3.4/install/include`

EXT_LIB=`~/bin/lua-5.3.4/install/lib`

After saving the Makefile, just type make. You will get UltraPse compiled. The upse is the executable of UltraPse software.
To test your compilation, just type **./upse --help** or **./upse -q prop -n stdprot**

To do more test, please first `make modules`. This will make the one-hot mode BSO. 

You can type `make test` to test UltraPse with TDFs.

You can type `make test_mod` to test UltraPse with BSOs.

For ***Microsoft Windows*** platform, just download the Win-x86_64.zip package from Release/Win-x86_64. Unpack it to your favoured location and use command line environment to use the executable. The documents, plugins, and test datasets are included.

For ***Microsoft Windows*** platform, you can use the upse-test.cmd to have a quick view.

If you got any difficulties, just send emails to PufengDu@gmail.com to complain. We will try out best to fix it asap.

## Citation
Please cite the following paper if you find UltraPse useful in your work:

[1] **Du, Pu-Feng**; Zhao, Wei; Miao, Yang-Yang; Wei, Le-Yi; Wang, Likun.	2017. "UltraPse: A Universal and Extensible Software Platform for Representing Biological Sequences." *Int. J. Mol. Sci.* 18, no. 11: 2400. http://dx.doi.org/10.3390/ijms18112400
