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


send_at_mgphycfg()
{
  echo '******************************************************************************************'
  echo '** AT$MGPHYCFG command parameters                                                       **'
  echo '******************************************************************************************'
  echo '** id  name            description                        values                        **'
  echo '** --  --------------- ---------------------------------- ----------------------------- **'
  echo '**  1  start           start/stop configuration           0:stop 1:start                **'
  echo '**  2  ncell_id        physical layer cell identify       0..503                        **'
  echo '**  3  n_rnti          radio network temporary identifier 0..65536                      **'
  echo '**  4  delta_ss        delta sequence-shift pattern       0..29                         **'
  echo '**  5  cyclic_shift    intended for DMRS field            0..7                          **'
  echo '**  6  nS              subframe id                        1..10                         **'
  echo '**  7  n_cp_l          cyclic prefix length               0:normal 1:extended           **'
  echo '**  8  group_assing    group assignment                   0..29                         **'
  echo '******************************************************************************************'
  echo

  start=0
  ncell_id=0
  n_rnti=0
  delta_ss=0
  cyclic_shift=0
  nS=1
  n_cp_l=0
  group_assing=0

  echo "Set required parameters only. Leave default values for others."
  echo
  read -e -i "$start"           -p "start            " input
  start="${input:-$start}"
  read -e -i "$ncell_id"        -p "ncell_id         " input
  ncell_id="${input:-$ncell_id}"
  read -e -i "$n_rnti"          -p "n_rnti           " input
  n_rnti="${input:-$n_rnti}"
  read -e -i "$delta_ss"        -p "delta_ss         " input
  delta_ss="${input:-$delta_ss}"
  read -e -i "$cyclic_shift"    -p "cyclic_shift     " input
  cyclic_shift="${input:-$cyclic_shift}"
  read -e -i "$nS"              -p "nS               " input
  nS="${input:-$nS}"
  read -e -i "$n_cp_l"          -p "n_cp_l           " input
  n_cp_l="${input:-$n_cp_l}"
  read -e -i "$group_assing"    -p "group_assing     " input
  group_assing="${input:-$group_assing}"

  echo
  echo "\$MGPHYCFG=$start,$ncell_id,$n_rnti,$delta_ss,$cyclic_shift,$nS,$n_cp_l,$group_assing"
  echo
  adb shell ./data/local/tmp/ut-ModemAt "-c '\$MGPHYCFG=$start,$ncell_id,$n_rnti,$delta_ss,$cyclic_shift,$nS,$n_cp_l,$group_assing"
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

read_diag_ok=`adb shell ls -l /system/bin/read_diag | grep "No such file or directory" | wc -l`
dciscan_ok=`adb shell ls -l /system/bin/dciscan | grep "No such file or directory" | wc -l`
webs_ok=`adb shell ls -l /data/local/tmp/webs | grep "No such file or directory" | wc -l`
at_cmd_ok=`adb shell ls -l /data/local/tmp/ut-ModemAt | grep "No such file or directory" | wc -l`

if [[ $read_diag_ok != 0 || $dciscan_ok != 0 || $webs_ok != 0 || $at_cmd_ok != 0 ]]
then
  echo One of required components is missing:
  echo "- read_diag  @ /system/bin/"
  echo "- dciscan    @ /system/bin/"
  echo "- webs       @ /data/local/tmp/"
  echo "- ut-ModemAt @ /data/local/tmp/"
  exit 1
fi

dci_count_1=`adb shell ps dciscan | wc -l`
dci_count_2=`adb shell ps /system/bin/./dciscan | wc -l`

if [[ $dci_count_1 == 2 || $dci_count_2 == 2 ]]
then
  echo DCI Server is running
else
  echo DCI Server is not running
  adb shell /system/bin/./dciscan server > /dev/null &
  echo Starting DCI Server and waiting 10 sec
  sleep 10
fi

webs_count=`adb shell ps ./webs | wc -l`

if [[ $webs_count == 2 ]]
then
  echo Web Server is running
else
  echo Starting Web Server 
  adb shell "cd /data/local/tmp; ./webs > /dev/null" &
  adb forward tcp:3000 tcp:3000
fi

sleep 2
echo Start Browser
sensible-browser http://127.0.0.1:3000/ &

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
  echo '**  9  Camp on LTE cell (switch to LTE before)      **'
  echo '**  N  Show neighbour cells (WCDMA/LTE)             **'
  echo '**  P  Paging signals                               **'
  echo '** ------------------------------------------------ **'
  echo '**  UL transmission configuration                   **'
  echo '** ------------------------------------------------ **'
  echo '**  A  Show valid AT$MGPHYCFG parameters range      **'
  echo '**  B  Read current parameters values               **'
  echo '**  C  Send AT$MGPHYCFG with modified values        **'
  echo '******************************************************'

  read -p "$device_name | select operation : " option

  case $option in
    0) stay_in_loop=false;;
    1) adb reboot;;
    2) adb shell "read_diag --req GET_VERSION; sleep 2";;
    3) adb shell "read_diag --req GET_IMSI:1; sleep 2";;
    4) adb shell "read_diag --req GET_IMSI:2; sleep 2";;
    5) adb shell "read_diag --req RAT_SEL:1; sleep 2";;
    6) adb shell "read_diag --req RAT_SEL:2; sleep 2";;
    7) adb shell "read_diag --req RAT_SEL:3; sleep 2";;
    8) adb shell "read_diag --req SCAN; sleep 2";;
    9) camp_lte_cell;;
    A)
       adb shell ./data/local/tmp/ut-ModemAt "-c '\$MGPHYCFG=?'"
       read -p "Press ENTER to continue ..."
       ;;
    B)
       adb shell ./data/local/tmp/ut-ModemAt "-c '\$MGPHYCFG?'"
       read -p "Press ENTER to continue ..."
       ;;
    C) send_at_mgphycfg;;
    N) x-terminal-emulator -e "adb shell 'read_diag --events 0 --msgs 0:0x0 --logs 0x4111,0x41AC,0xB031,0xB119,0xB197'";;
    P) x-terminal-emulator -e "adb shell 'read_diag --events 0 --msgs 0:0x0 --logs 0xB0CB'";;
    *) echo Select 0..9, A, B, C, N, P;;
  esac
done

exit 0
