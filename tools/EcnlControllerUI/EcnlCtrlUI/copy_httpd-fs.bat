copy /Y favicon.ico ..\..\..\uip\apps\webserver\httpd-fs\favicon.ico
copy /Y information.json ..\..\..\uip\apps\webserver\httpd-fs\information.json

mkdir ..\..\..\uip\apps\webserver\httpd-fs\css
copy /Y css\ecn_gen.css ..\..\..\uip\apps\webserver\httpd-fs\css\ecn_gen.css

mkdir ..\..\..\uip\apps\webserver\httpd-fs\devices
..\tools\jsmin < devices\ClassGroupList.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\ClassGroupList.json
..\tools\jsmin < devices\ClassList00.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\ClassList00.json
..\tools\jsmin < devices\ClassList0E.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\ClassList0E.json
..\tools\jsmin < devices\ClassList0F.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\ClassList0F.json
..\tools\jsmin < devices\ClassList01.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\ClassList01.json
..\tools\jsmin < devices\ClassList02.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\ClassList02.json
..\tools\jsmin < devices\ClassList03.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\ClassList03.json
..\tools\jsmin < devices\ClassList04.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\ClassList04.json
..\tools\jsmin < devices\ClassList05.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\ClassList05.json
..\tools\jsmin < devices\ClassList06.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\ClassList06.json
..\tools\jsmin < devices\PropertyList000A.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList000A.json
..\tools\jsmin < devices\PropertyList000B.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList000B.json
..\tools\jsmin < devices\PropertyList000C.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList000C.json
..\tools\jsmin < devices\PropertyList000D.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList000D.json
..\tools\jsmin < devices\PropertyList000E.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList000E.json
..\tools\jsmin < devices\PropertyList000F.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList000F.json
..\tools\jsmin < devices\PropertyList0EF0.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0EF0.json
..\tools\jsmin < devices\PropertyList0001.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0001.json
..\tools\jsmin < devices\PropertyList001A.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList001A.json
..\tools\jsmin < devices\PropertyList001B.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList001B.json
..\tools\jsmin < devices\PropertyList001C.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList001C.json
..\tools\jsmin < devices\PropertyList001D.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList001D.json
..\tools\jsmin < devices\PropertyList001E.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList001E.json
..\tools\jsmin < devices\PropertyList001F.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList001F.json
..\tools\jsmin < devices\PropertyList0002.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0002.json
..\tools\jsmin < devices\PropertyList002A.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList002A.json
..\tools\jsmin < devices\PropertyList002B.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList002B.json
..\tools\jsmin < devices\PropertyList002C.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList002C.json
..\tools\jsmin < devices\PropertyList02A0.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList02A0.json
..\tools\jsmin < devices\PropertyList0003.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0003.json
..\tools\jsmin < devices\PropertyList03B2.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList03B2.json
..\tools\jsmin < devices\PropertyList03B7.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList03B7.json
..\tools\jsmin < devices\PropertyList03B8.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList03B8.json
..\tools\jsmin < devices\PropertyList03B9.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList03B9.json
..\tools\jsmin < devices\PropertyList03BB.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList03BB.json
..\tools\jsmin < devices\PropertyList03C5.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList03C5.json
..\tools\jsmin < devices\PropertyList03C6.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList03C6.json
..\tools\jsmin < devices\PropertyList03D3.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList03D3.json
..\tools\jsmin < devices\PropertyList0004.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0004.json
..\tools\jsmin < devices\PropertyList0005.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0005.json
..\tools\jsmin < devices\PropertyList05FD.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList05FD.json
..\tools\jsmin < devices\PropertyList0006.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0006.json
..\tools\jsmin < devices\PropertyList0007.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0007.json
..\tools\jsmin < devices\PropertyList0008.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0008.json
..\tools\jsmin < devices\PropertyList0009.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0009.json
..\tools\jsmin < devices\PropertyList0010.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0010.json
..\tools\jsmin < devices\PropertyList0011.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0011.json
..\tools\jsmin < devices\PropertyList0012.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0012.json
..\tools\jsmin < devices\PropertyList0013.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0013.json
..\tools\jsmin < devices\PropertyList0014.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0014.json
..\tools\jsmin < devices\PropertyList0015.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0015.json
..\tools\jsmin < devices\PropertyList0016.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0016.json
..\tools\jsmin < devices\PropertyList0017.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0017.json
..\tools\jsmin < devices\PropertyList0018.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0018.json
..\tools\jsmin < devices\PropertyList0019.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0019.json
..\tools\jsmin < devices\PropertyList0020.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0020.json
..\tools\jsmin < devices\PropertyList0021.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0021.json
..\tools\jsmin < devices\PropertyList0022.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0022.json
..\tools\jsmin < devices\PropertyList0023.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0023.json
..\tools\jsmin < devices\PropertyList0025.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0025.json
..\tools\jsmin < devices\PropertyList0026.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0026.json
..\tools\jsmin < devices\PropertyList026B.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList026B.json
..\tools\jsmin < devices\PropertyList026E.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList026E.json
..\tools\jsmin < devices\PropertyList026F.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList026F.json
..\tools\jsmin < devices\PropertyList0027.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0027.json
..\tools\jsmin < devices\PropertyList027A.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList027A.json
..\tools\jsmin < devices\PropertyList027B.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList027B.json
..\tools\jsmin < devices\PropertyList027C.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList027C.json
..\tools\jsmin < devices\PropertyList027D.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList027D.json
..\tools\jsmin < devices\PropertyList027E.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList027E.json
..\tools\jsmin < devices\PropertyList027F.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList027F.json
..\tools\jsmin < devices\PropertyList0028.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0028.json
..\tools\jsmin < devices\PropertyList0029.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0029.json
..\tools\jsmin < devices\PropertyList0130.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0130.json
..\tools\jsmin < devices\PropertyList0133.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0133.json
..\tools\jsmin < devices\PropertyList0134.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0134.json
..\tools\jsmin < devices\PropertyList0135.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0135.json
..\tools\jsmin < devices\PropertyList0139.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0139.json
..\tools\jsmin < devices\PropertyList0142.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0142.json
..\tools\jsmin < devices\PropertyList0143.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0143.json
..\tools\jsmin < devices\PropertyList0145.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0145.json
..\tools\jsmin < devices\PropertyList0146.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0146.json
..\tools\jsmin < devices\PropertyList0260.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0260.json
..\tools\jsmin < devices\PropertyList0261.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0261.json
..\tools\jsmin < devices\PropertyList0263.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0263.json
..\tools\jsmin < devices\PropertyList0267.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0267.json
..\tools\jsmin < devices\PropertyList0272.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0272.json
..\tools\jsmin < devices\PropertyList0273.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0273.json
..\tools\jsmin < devices\PropertyList0279.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0279.json
..\tools\jsmin < devices\PropertyList0280.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0280.json
..\tools\jsmin < devices\PropertyList0281.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0281.json
..\tools\jsmin < devices\PropertyList0282.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0282.json
..\tools\jsmin < devices\PropertyList0283.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0283.json
..\tools\jsmin < devices\PropertyList0287.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0287.json
..\tools\jsmin < devices\PropertyList0288.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0288.json
..\tools\jsmin < devices\PropertyList0289.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0289.json
..\tools\jsmin < devices\PropertyList0290.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0290.json
..\tools\jsmin < devices\PropertyList0405.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0405.json
..\tools\jsmin < devices\PropertyList0601.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0601.json
..\tools\jsmin < devices\PropertyList0602.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyList0602.json
..\tools\jsmin < devices\PropertyListBase.json > ..\..\..\uip\apps\webserver\httpd-fs\devices\PropertyListBase.json

mkdir ..\..\..\uip\apps\webserver\httpd-fs\imgs
copy /Y imgs\core-s.svg ..\..\..\uip\apps\webserver\httpd-fs\imgs\core-s.svg
copy /Y imgs\logo.gif ..\..\..\uip\apps\webserver\httpd-fs\imgs\logo.gif

mkdir ..\..\..\uip\apps\webserver\httpd-fs\js
copy /Y js\ctrlui.min.js ..\..\..\uip\apps\webserver\httpd-fs\js\ctrlui.min.js
copy /Y js\FileSaver.min.js ..\..\..\uip\apps\webserver\httpd-fs\js\FileSaver.min.js
copy /Y js\jszip.min.js ..\..\..\uip\apps\webserver\httpd-fs\js\jszip.min.js
copy /Y js\linq.min.js ..\..\..\uip\apps\webserver\httpd-fs\js\linq.min.js
copy /Y js\mscorlib.min.js ..\..\..\uip\apps\webserver\httpd-fs\js\mscorlib.min.js

mkdir ..\..\..\uip\apps\webserver\httpd-fs\js\images
copy /Y js\images\ajax-loader.gif ..\..\..\uip\apps\webserver\httpd-fs\js\images\ajax-loader.gif

mkdir ..\..\..\uip\apps\webserver\httpd-fs\templates
copy /Y templates\main.c ..\..\..\uip\apps\webserver\httpd-fs\templates\main.c
copy /Y templates\main.cfg ..\..\..\uip\apps\webserver\httpd-fs\templates\main.cfg
copy /Y templates\main.h ..\..\..\uip\apps\webserver\httpd-fs\templates\main.h

mkdir ..\..\..\uip\apps\webserver\httpd-fs\themes
copy /Y themes\ecn-theme.min.css ..\..\..\uip\apps\webserver\httpd-fs\themes\ecn-theme.min.css
