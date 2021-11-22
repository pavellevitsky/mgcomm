#!/bin/bash

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

at_cmd_enabled=`adb shell ls -l /data/local/tmp/ut-ModemAt | grep "No such file or directory" | wc -l`

if [[ $at_cmd_enabled != 0 ]]
then
  echo ut-ModemAt is missing @ /data/local/tmp 
  exit 1
fi

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
  echo '** ------------------------------------------------ **'
  echo '**  UL transmission configuration                   **'
  echo '** ------------------------------------------------ **'
  echo '**  A  Show valid AT$MGPHYCFG parameters range      **'
  echo '**  B  Read current parameters values               **'
  echo '**  C  Send AT$MGPHYCFG with modified values        **'
  echo '******************************************************'

  read -p "$device_name | select operation : " option

  case $option in
    0) stay_in_loop=false
       ;;
    1) adb reboot
       ;;
    A) adb shell ./data/local/tmp/ut-ModemAt "-c '\$MGPHYCFG=?'"
       read -p "Press ENTER to continue ..."
       ;;
    B) adb shell ./data/local/tmp/ut-ModemAt "-c '\$MGPHYCFG?'"
       read -p "Press ENTER to continue ..."
       ;;
    C) send_at_mgphycfg
       ;;
    *) echo Valid options are : 0, 1, A, B or C
       read -p "Press ENTER to continue ..."
       ;;
  esac

done

exit 0
