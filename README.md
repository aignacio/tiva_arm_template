Tiva_ARM_template
=================

Tools for a complete environment to develop on TIVA launchpad arm {TM4C123GH6PM}.
This repo contains some libs for tiva, template for compile, scripts, troubleshooting... 

__Requirements:__
 
* [gcc arm toolchain](https://launchpad.net/gcc-arm-embedded)
* [lm4flash](https://github.com/utzig/lm4tools)
* [OpenOCD](http://openocd.sourceforge.net/)
* [eclipse-cdt](http://www.eclipse.org/downloads/packages/eclipse-ide-cc-linux-developers-includes-incubating-components/indigosr1)

__Instructions:__

The tutorial to develop your ambiance can be searched in Embedded System Platforms blog's. To test if yours basic tools are correctly installed, edit the definitions of the __MAKEFILE__ in template/ folder, and then: 
    
    cd template/
    make all
    make load
