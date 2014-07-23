#!/bin/bash
#Ânderson Ignácio da Silva -2014/1 - www.esplatforms.blogpsot.com.br
#to run just type in terminal sudo chmod  +x compilator.sh, add your password and execute (./compilator.sh)
sudo clear
echo -e "\e[4m\e[1m\e[34m\tCompiler for ARM Cortex M4F - STELLARIS LM4F120 Ver. 1.0"
echo -e "\e[31m\e[1m\e[24m\nAuthor:\e[21m\e[97m Ânderson Ignácio da Silva - http://esplatforms.blogspot.com.br/"
echo -e "\e[31m\e[1m\e[24mOS Tested:\e[21m\e[97m Ubuntu 13.04 or higher"
echo -e "\e[31m\e[1m\e[24mFree source code for everyone. :)\e[21m\e[97m"

echo -n "Your current directory:"
pwd 
read -e -p "Please type the directory folder of the project:" srcDir 
if [[ $srcDir == *~* ]]
then
  srcDir=$(sed -e 's/~//' <<<$srcDir)
  srcDir=$HOME$srcDir
fi
echo $srcDir
read -e -p "Please type the directory folder of the stellaris library:" srcLib 
if [[ $srcLib == *~* ]]
then
  srcLib=$(sed -e 's/~//' <<<$srcLib)
  srcLib=$HOME$srcLib
fi
cd "$srcDir"

echo $srcLib
read -e -p "Please type the name of the main file project(*.c):" srcMainFile

echo "LOG - USER:$USER" &>  log_compiler.txt
echo "      DATE:$(date)" &>> log_compiler.txt

mainC=`echo $srcMainFile | cut -f1 -d'.'`
cp "$srcLib"boards/ek-lm4f120xl/blinky/blinky.ld loader.ld
cp "$srcLib"boards/ek-lm4f120xl/blinky/startup_gcc.c .
files=$(ls *.c | tr '\n' ' ' | tr '\r' ' ' )

echo -ne "Compiling"
for i in {1..10}
do
	sleep .1s
	echo -ne "."
done

arm-none-eabi-gcc $files -g -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 \
-mfloat-abi=softfp -Os -ffunction-sections -fdata-sections -MD -std=c99 \
-Wall -pedantic -DPART_LM4F120H5QR -c -I$srcLib -DTARGET_IS_BLIZZARD_RA1 &>> log_compiler.txt

sleep .1s

if  grep -qs "Failed" log_compiler.txt | grep -qs "undefined" log_compiler.txt; then 
	echo -e "\e[31mFailed!\e[97m"
	exit 0
else
	echo -e "\e[34mComplete!\e[97m"
fi

echo -ne "Generating output"
for i in {1..10}
do
	sleep .1s
	echo -ne "."
done

object=$(ls *.o | tr '\n' ' ' | tr '\r' ' ')
arm-none-eabi-ld -T loader.ld --entry ResetISR -o a.out $object --gc-sections &>> log_compiler.txt

sleep .1s

if  grep -qs "Failed" log_compiler.txt | grep -qs "undefined" log_compiler.txt; then 
	echo -e "\e[31mFailed!\e[97m"
	exit 0
else
	echo -e "\e[34mComplete!\e[97m"
fi

echo -ne "Generating bin"
for i in {1..4}
do
	sleep .1s
	echo -ne "."
done

arm-none-eabi-objcopy -O binary a.out "$mainC".bin &>> log_compiler.txt

sleep .1s

if  grep -qs "Failed" log_compiler.txt | grep -qs "undefined" log_compiler.txt; then 
	echo -e "\e[31mFailed!\e[97m"
	exit 0
else
	echo -e "\e[34mComplete!\e[97m"
fi

echo -ne "Cleaning"
for i in {1..4}
do
	sleep .1s
	echo -ne "."
done

rm -rf "$mainC".o "$mainC".d a.out startup_gcc* loader.ld

sleep .1s

if  grep -qs "Failed" log_compiler.txt | grep -qs "undefined" log_compiler.txt; then 
	echo -e "\e[31mFailed!\e[97m"
	exit 0
else
	echo -e "\e[34mComplete!\e[97m"
fi

echo -e "\n\e[34m Compilation Finished!, for details see log_compiler.txt\e[97m"

exit 0
