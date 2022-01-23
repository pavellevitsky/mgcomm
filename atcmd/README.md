```
USAGE
-----
./ut-ModemAt [-d | -f] [-c $MGPHYCFG=param1,param2,...]

optional argument '-f' enables reading commands from file at-commands-list.txt
optional argument '-c command' writes the command to Unix device
optional argument '-d' enables dumps

EXAMPLES
--------
./ut-ModemAt -f
./ut-ModemAt -c +CGMI
./ut-ModemAt -c +CFUN?
./ut-ModemAt -c +CFUN=0
./ut-ModemAt -c +CFUN?
./ut-ModemAt -c +CFUN=1
./ut-ModemAt -d -c I
./ut-ModemAt -c $MGPHYCFG=1
```
