#!/bin/bash
#Ânderson Ignácio da Silva -2014/1 - www.esplatforms.blogpsot.com.br
#to run just type in terminal sudo chmod  +x arm_installer_stellaris.sh, add your password and execute (./arm_ins....)
var=$USER 
sudo clear

AdjustSystem()
{
  #n-continue same line // e- special chars
  echo -ne "\nAdding repository source of gcc arm..." 
  # argument -y no asking // &-redirect without echo // > - copy return to this file replacing all // >> - > - copy return to this file without replace
  sudo add-apt-repository ppa:terry.guo/gcc-arm-embedded -y &> ~/log_report_installer_arm.txt 
  if  grep -qs "Failed" ~/log_report_installer_arm.txt; then 
    echo -e "\e[31mFailed!\e[97m"
  else
    echo -e "\e[34mComplete!\e[97m"
  fi

  echo -n "Updating source repositories..."
  sudo apt-get update -y &> /dev/null
  echo -e "\e[34mComplete!\e[97m"
 
  echo -n "Installing gcc for arm..."
  sudo apt-get install gcc-arm-none-eabi -y &> ~/log_report_installer_arm.txt
  if  grep -qs "E: Unable to locate package gcc-arm-none-eabi" ~/log_report_installer_arm.txt; then 
    #Fix the reference in terry.guo repo changing saucy by raring
    sudo sed -i 's/saucy/raring/g' /etc/apt/sources.list.d/terry_guo-gcc-arm-embedded-saucy.list
    sudo apt-get update -y &> /dev/null
    sudo apt-get install gcc-arm-none-eabi -y &> ~/log_report_installer_arm.txt
    echo -e "\e[34mRepo bug fixed!\e[97m"
  else
    echo -e "\e[34mComplete!\e[97m"
  fi
}

StellarisInstaller()
{
  echo -en "\nCreating the source folder..."
  mkdir -p $srcFolder/stellarisware &> ~/log_report_installer_arm.txt
  if  grep -qs "Failed" ~/log_report_installer_arm.txt; then 
    echo -e "\e[31mFailed!\e[97m"
  else
    echo -e "\e[34mComplete!\e[97m"
  fi

  echo -en "Download \e[31m\e[1mStellaris Library(http://www.ti.com/tool/sw-ek-lm4f120xl)\e[21m\e[97m..."
  while true
  do
    if [ -f ~/Downloads/SW-EK-LM4F120XL-9453.exe ]; then
      echo -e "\e[34mComplete!\e[97m"
      break
    fi
  done

  sudo apt-get install -y unzip &>> /dev/null
  unzip -u ~/Downloads/SW-EK-LM4F120XL-9453.exe -d ~/$srcFolder/stellarisware &>> /dev/null

  echo -n "Making the library..."
  cd ~/$srcFolder/stellarisware
  make &> ~/log_report_installer_arm.txt
  if  grep -qs "Failed" ~/log_report_installer_arm.txt; then 
    echo -e "\e[31mFailed!\e[97m"
  else
    echo -e "\e[34mComplete!\e[97m"
  fi
}

lm4Install()
{
  echo -ne "\nInstalling some required tools..."
  sudo apt-get install libusb-1.0-0-dev -y &> ~/log_report_installer_arm.txt
  if  grep -qs "Failed" ~/log_report_installer_arm.txt; then 
    echo -e "\e[31mFailed!\e[97m"
  else
    echo -e "\e[34mComplete!\e[97m"
  fi

  echo -ne "Installing git tools..."
  sudo apt-get install git -y &> ~/log_report_installer_arm.txt
  if  grep -qs "Failed" ~/log_report_installer_arm.txt; then 
    echo -e "\e[31mFailed!\e[97m"
  else
    echo -e "\e[34mComplete!\e[97m"
  fi
  
  echo -ne "Clonning gitrepo of lm4tools..."
  cd ~/$srcFolder
  git clone https://github.com/utzig/lm4tools.git &> ~/log_report_installer_arm.txt
  if  grep -qs "Failed" ~/log_report_installer_arm.txt; then 
    echo -e "\e[31mFailed!\e[97m"
  else
    echo -e "\e[34mComplete!\e[97m"
  fi

  cd ~/$srcFolder/lm4tools/lm4flash
  echo -ne "Making tools..."
  make &> ~/log_report_installer_arm.txt
  if  grep -qs "Failed" ~/log_report_installer_arm.txt; then 
    echo -e "\e[31mFailed!\e[97m"
  else
    echo -e "\e[34mComplete!\e[97m"
  fi
}

autoInstall()
{
  echo -ne "Adding lm4flash to usr profile..."
  sudo echo "export PATH=$PATH:$HOME/$srcFolder/lm4tools/lm4flash" &>> ~/.profile 
  echo -e "\e[34mComplete!\e[97m"
 
  echo 'ATTRS{idVendor}=="1cbe", ATTRS{idProduct}=="00fd", GROUP="users", MODE="0660"' | \
  sudo tee /etc/udev/rules.d/99-stellaris-launchpad.rules &>> /dev/null
  sudo usermod -aG users users $var &>> /dev/null
}

openInstall()
{
  mkdir -p ~/$srcFolder/
  cd ~/$srcFolder/
  echo -ne "Cloning openOCD repositories..."
  git clone http://git.code.sf.net/p/openocd/code &> ~/log_report_installer_arm.txt
  if  grep -qs "Failed" ~/log_report_installer_arm.txt; then 
    echo -e "\e[31mFailed!\e[97m"
  else
    echo -e "\e[34mComplete!\e[97m"
  fi

  mv -f code openocd &>> /dev/null
  cd ~/$srcFolder/openocd
  echo -ne "Updating git repo and changing the branch..."
  git fetch http://openocd.zylin.com/openocd refs/changes/22/922/15 &>> /dev/null
  git checkout FETCH_HEAD &>> /dev/null
  echo -e "\e[34mComplete!\e[97m"

  echo -ne "Setting the module..."
  ./bootstrap &>> /dev/null
  ./configure --enable-maintainer-mode --enable-ti-icdi &>> /dev/null
  make &>> /dev/null
  echo -e "\e[34mComplete!\e[97m"

  echo -ne "Installing the openOCD..."
  sudo make install &> ~/log_report_installer_arm.txt
  if  grep -qs "Failed" ~/log_report_installer_arm.txt; then 
    echo -e "\e[31mFailed!\e[97m"
  else
    echo -e "\e[34mComplete!\e[97m"
  fi
}

echo -e "\e[4m\e[1m\e[34m\tInstaller for ARM Cortex M4F - STELLARIS LM4F120 Ver. 1.0"
echo -e "\e[31m\e[1m\e[24m\nAuthor:\e[21m\e[97m Ânderson Ignácio da Silva - http://esplatforms.blogspot.com.br/"
echo -e "\e[31m\e[1m\e[24mOS Tested:\e[21m\e[97m Ubuntu 13.04 or higher"
echo -e "\e[31m\e[1m\e[24mWARNING 1:\e[21m\e[97mRemember to put the stellaris library file (SW-EK-LM4F120XL-9453.exe)\ninto ~/Downloads!"
echo -e "\e[31m\e[1m\e[24mWARNING 2:\e[21m\e[97mVisit http://openocd.zylin.com/#/c/922/ to check the last updates and\nthen update the script of openOCD"
echo -e "\e[31m\e[1m\e[24mFree source code for everyone. :)\e[21m\e[97m"
read -e -p "Type folder name to install:" srcFolder

if [[ $srcFolder == *~* ]]
then
  srcFolder=$(sed -e 's/~//' <<<$srcFolder)
  srcFolder=$HOME$srcFolder
fi

echo -e "\e[31m\e[1m\nStep 1 - Installing GCC-ARM: \e[21m\e[97m"
AdjustSystem;
echo -e "\e[31m\e[1m\nStep 2 - Installing the Stellaris Library: \e[21m\e[97m"
StellarisInstaller $srcFolder
echo -e "\e[31m\e[1m\nStep 3 - Installing lm4tools: \e[21m\e[97m"
lm4Install $srcFolder
echo -e "\e[31m\e[1m\nStep 4 - Configuring auto-tools: \e[21m\e[97m\n"
autoInstall $srcFolder
echo -e "\e[31m\e[1m\nStep 5 - Instaling OpenOCD: \e[21m\e[97m\n"
openInstall $srcFolder

sudo rm -rf ~/log_report_installer_arm.txt &>> /dev/null

echo -e "\e[34m------------------Success!------------------\e[97m"

echo -e "\e[0m"
exit 0
