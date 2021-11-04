Main menu
```
******************************************************
**               Welcome to MGComm                  **
** ------------------------------------------------ **
**  0  Exit                                         **
**  1  Reset device                                 **
** ------------------------------------------------ **
**  UL transmission configuration                   **
** ------------------------------------------------ **
**  A  Show valid AT$MGPHYCFG parameters range      **
**  B  Read current parameters values               **
**  C  Send AT$MGPHYCFG with modified values        **
******************************************************
QuadM4 | select operation : C
```
AT$MGPHYCFG menu
```
******************************************************************************************
** AT$MGPHYCFG command parameters                                                       **
******************************************************************************************
** id  name            description                        values                        **
** --  --------------- ---------------------------------- ----------------------------- **
**  1  start           start/stop configuration           0:stop 1:start                **
**  2  ncell_id        physical layer cell identify       0..503                        **
**  3  n_rnti          radio network temporary identifier 0..65536                      **
**  4  delta_ss        delta sequence-shift pattern       0..29                         **
**  5  cyclic_shift    intended for DMRS field            0..7                          **
**  6  nS              subframe id                        1..10                         **
**  7  modu            modulation mapper                  0:BPSK 1:QPSK 2:QAM16 3:QAM64 **
**  8  n_cp_l          cyclic prefix length               0:normal 1:extended           **
**  9  group_hop       group hopping state                0:disable 1:enable            **
** 10  group_assing    group assignment                   0..29                         **
** 11  seq_hop_enabled sequence hopping state             0:disable 1:enable            **
******************************************************************************************
```
