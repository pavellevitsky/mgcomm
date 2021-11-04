#!/bin/bash

camp_lte_cell()
{
  earfcn=6300
  pci=1

  echo "Default values (Network in the Box) EARFCN:6300 PCI:1"

  read -e -i "$earfcn" -p "Enter EARFCN : " input
  earfcn="${input:-$earfcn}"

  read -e -i "$pci" -p "Enter PCI : " input
  pci="${input:-$pci}"

  echo "read_diag --req CAMP_LTE:$earfcn,$pci"
  adb shell "read_diag --req CAMP_LTE:$earfcn,$pci"

  echo
  read -p "Press ENTER to continue ..."
}


devices_count=`adb devices | wc -l`

# Exit the script if no device is attached
if [[ $devices_count == 2 ]]
then
  echo There is no device attached
  exit 1
fi

# Exit the script if more than one device is attached
if [[ $devices_count > 3 ]]
then
  echo Only one attached device expected
  exit 1
fi

dci_count=`adb shell ps /system/bin/dciscan | wc -l`

if [[ $dci_count != 2 ]]
then
  echo DCI Server is not running
  sleep 2
  echo Start DCI Server
  adb shell dciscan server > /dev/null &
  sleep 10
fi

webs_count=`adb shell ps ./webs | wc -l`

if [[ $webs_count != 2 ]]
then
  echo Start Web Server 
  adb shell "cd /data/local/tmp; ./webs > /dev/null" &
  adb forward tcp:3000 tcp:3000
fi

sleep 2
echo Start Browser
#sensible-browser http://127.0.0.1:3000/ &

stay_in_loop=true

device_name=`adb devices | grep "device" | grep -v "devices" | xargs | cut -d ' ' -f 1`

while [ $stay_in_loop = true ]
do
  clear

  echo '******************************************************'
  echo '**               Welcome to MGComm                  **'
  echo '** ------------------------------------------------ **'
  echo '**  0  Exit                                         **'
  echo '**  1  Reset device                                 **'
  echo '**  2  Show modem FW version                        **'
  echo '**  3  Show IMSI slot 1                             **'
  echo '**  4  Show IMSI slot 2                             **'
  echo '**  5  Set GSM preffered network type               **'
  echo '**  6  Set WCDMA preffered network type             **'
  echo '**  7  Set LTE preffered network type               **'
  echo '**  8  Trigger power scan                           **'
  echo '**  9  Camp on LTE cell (while LTE network is set)  **'
  echo '******************************************************'

  read -p "$device_name | select operation : " option

  case $option in
    0) stay_in_loop=false;;
    1) adb reboot;;
    2) adb shell "read_diag --req GET_VERSION";;
    3) adb shell "read_diag --req GET_IMSI:1";;
    4) adb shell "read_diag --req GET_IMSI:2";;
    5) adb shell "read_diag --req RAT_SEL:1; sleep 2";;
    6) adb shell "read_diag --req RAT_SEL:2; sleep 2";;
    7) adb shell "read_diag --req RAT_SEL:3; sleep 2";;
    8) adb shell "read_diag --req SCAN";;
    9) camp_lte_cell;;
    *) echo Select 0..9;;
  esac
done

exit 0
