set time2=%time: =0%
set fn=%date:~-10,4%%date:~-5,2%%date:~-2,2%%time2:~0,2%%time2:~3,2%%time2:~6,2%

..\..\srecord\srec_cat --address-length=4 --line-length=70 ..\ukadecot\DefaultBuild\ukadecot.mot ..\..\uip\apps\webserver\httpd-fs.bin -binary -offset 0xFFF00000 ..\..\kadecot\kadecot_names.bin -binary -offset 0xFFFE0000  -o ..\flash\ukadecot_%fn%.mot
