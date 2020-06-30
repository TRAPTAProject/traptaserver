- Take a fresh new ubuntu image

$ sudo apt install git build-essential libfontconfig1-dev libfreetype6-dev libx11-dev libxext-dev libxfixes-dev libxi-dev libxrender-dev libxcb1-dev libx11-xcb-dev libxcb-glx0-dev libxkbcommon-x11-dev mesa-common-dev freeglut3 freeglut3-dev libglew1.5 libglew1.5-dev libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx libgl1-mesa-dev chrpath python 

Download Qt and unzip it:
$ wget https://download.qt.io/archive/qt/5.14/5.14.2/single/qt-everywhere-src-5.14.2.tar.xz
$ tar xvf qt-everywhere-src-5.14.2.tar.xz


In order to speed up the building, let's remove the modules we don't need :
$ cd qt-everywhere-src-5.14.2
$ rm -rf web* qt3d qtmultimedia qtvirtualkeyboard qtscript qtremoteobjects qtlocation qtandroidextras qtcanvas3d qtcharts qtdatavis3d qtgamepad qtmacextras qtpurchasing qtscxml qtsensors qtserialbus qtserialport qtspeech qtvirtualkeyboard qtwinextras qtwebchannel qtwebengine qtwebglplugin qtwebsockets qtwebview qtwayland qtxmlpatterns qtgraphicaleffects qtconnectivity qtactiveqt qtnetworkauth qttools qtlottie qtquick3d qtquicktimeline


Run the configure command:
$ ./configure -opensource -nomake tests -nomake examples -qt-xcb -no-icu -confirm-license -no-dbus -sql-sqlite

The final output should look like this:

Configure summary:

Build type: linux-g++ (x86_64, CPU features: mmx sse sse2)
Compiler: gcc 7.5.0
Configuration: sse2 aesni sse3 ssse3 sse4_1 sse4_2 avx avx2 avx512f avx512bw avx512cd avx512dq avx512er avx512ifma avx512pf avx512vbmi avx512vl compile_examples enable_new_dtags f16c largefile precompile_header rdrnd shani x86SimdAlways shared shared rpath release c++11 c++14 c++1z concurrent reduce_exports reduce_relocations stl
Build options:
  Mode ................................... release
  Optimize release build for size ........ no
  Building shared libraries .............. yes
  Using C standard ....................... C11
  Using C++ standard ..................... C++17
  Using ccache ........................... no
  Using new DTAGS ........................ yes
  Relocatable ............................ yes
  Using precompiled headers .............. yes
  Using LTCG ............................. no
  Target compiler supports:
    SSE .................................. SSE2 SSE3 SSSE3 SSE4.1 SSE4.2
    AVX .................................. AVX AVX2
    AVX512 ............................... F ER CD PF DQ BW VL IFMA VBMI
    Other x86 ............................ AES F16C RDRAND SHA
    Intrinsics without -mXXX option ...... yes
  Build parts ............................ libs tools
Qt modules and options:
  Qt Concurrent .......................... yes
  Qt D-Bus ............................... no
  Qt D-Bus directly linked to libdbus .... no
  Qt Gui ................................. yes
  Qt Network ............................. yes
  Qt Sql ................................. yes
  Qt Testlib ............................. yes
  Qt Widgets ............................. yes
  Qt Xml ................................. yes
Support enabled for:
  Using pkg-config ....................... yes
  udev ................................... no
  Using system zlib ...................... yes
  Zstandard support ...................... no
Qt Core:
  DoubleConversion ....................... yes
    Using system DoubleConversion ........ no
  GLib ................................... no
  iconv .................................. yes
  ICU .................................... no
  Built-in copy of the MIME database ..... yes
  Tracing backend ........................ <none>
  Logging backends:
    journald ............................. no
    syslog ............................... no
    slog2 ................................ no
  PCRE2 .................................. yes
    Using system PCRE2 ................... no
Qt Network:
  getifaddrs() ........................... yes
  IPv6 ifname ............................ yes
  libproxy ............................... no
  Linux AF_NETLINK ....................... yes
  OpenSSL ................................ no
    Qt directly linked to OpenSSL ........ no
  OpenSSL 1.1 ............................ no
  DTLS ................................... no
  OCSP-stapling .......................... no
  SCTP ................................... no
  Use system proxies ..................... yes
  GSSAPI ................................. no
Qt Gui:
  Accessibility .......................... yes
  FreeType ............................... yes
    Using system FreeType ................ yes
  HarfBuzz ............................... yes
    Using system HarfBuzz ................ no
  Fontconfig ............................. yes
  Image formats:
    GIF .................................. yes
    ICO .................................. yes
    JPEG ................................. yes
      Using system libjpeg ............... no
    PNG .................................. yes
      Using system libpng ................ yes
  Text formats:
    HtmlParser ........................... yes
    CssParser ............................ yes
    OdfWriter ............................ yes
    MarkdownReader ....................... yes
      Using system libmd4c ............... no
    MarkdownWriter ....................... yes
  EGL .................................... no
  OpenVG ................................. no
  OpenGL:
    Desktop OpenGL ....................... yes
    OpenGL ES 2.0 ........................ no
    OpenGL ES 3.0 ........................ no
    OpenGL ES 3.1 ........................ no
    OpenGL ES 3.2 ........................ no
  Vulkan ................................. no
  Session Management ..................... yes
Features used by QPA backends:
  evdev .................................. yes
  libinput ............................... no
  INTEGRITY HID .......................... no
  mtdev .................................. no
  tslib .................................. no
  xkbcommon .............................. yes
  X11 specific:
    XLib ................................. yes
    XCB Xlib ............................. yes
    EGL on X11 ........................... no
QPA backends:
  DirectFB ............................... no
  EGLFS .................................. no
  LinuxFB ................................ yes
  VNC .................................... yes
  XCB:
    Using system-provided XCB libraries .. no
    XCB XKB .............................. yes
    XCB XInput ........................... yes
    Native painting (experimental) ....... no
    GL integrations:
      GLX Plugin ......................... yes
        XCB GLX .......................... yes
      EGL-X11 Plugin ..................... no
Qt Sql:
  SQL item models ........................ yes
Qt Widgets:
  GTK+ ................................... no
  Styles ................................. Fusion Windows
Qt PrintSupport:
  CUPS ................................... no
Qt Sql Drivers:
  DB2 (IBM) .............................. no
  InterBase .............................. no
  MySql .................................. no
  OCI (Oracle) ........................... no
  ODBC ................................... no
  PostgreSQL ............................. no
  SQLite2 ................................ no
  SQLite ................................. yes
    Using system provided SQLite ......... no
  TDS (Sybase) ........................... no
Qt Testlib:
  Tester for item models ................. yes
Further Image Formats:
  JasPer ................................. no
  MNG .................................... no
  TIFF ................................... yes
    Using system libtiff ................. no
  WEBP ................................... yes
    Using system libwebp ................. no
Qt QML:
  QML network support .................... yes
  QML debugging and profiling support .... yes
  QML just-in-time compiler .............. yes
  QML sequence object .................... yes
  QML XML http request ................... yes
  QML Locale ............................. yes
Qt QML Models:
  QML list model ......................... yes
  QML delegate model ..................... yes
Qt Quick:
  Direct3D 12 ............................ no
  AnimatedImage item ..................... yes
  Canvas item ............................ yes
  Support for Qt Quick Designer .......... yes
  Flipable item .......................... yes
  GridView item .......................... yes
  ListView item .......................... yes
  TableView item ......................... yes
  Path support ........................... yes
  PathView item .......................... yes
  Positioner items ....................... yes
  Repeater item .......................... yes
  ShaderEffect item ...................... yes
  Sprite item ............................ yes

Note: Also available for Linux: linux-clang linux-icc

Note: Disabling X11 Accessibility Bridge: D-Bus or AT-SPI is missing.

Qt is now configured for building. Just run 'make'.
Once everything is built, you must run 'make install'.
Qt will be installed into '/usr/local/Qt-5.14.2'.


Build it:
$ make

Now you can go to bed, it takes a few hours.

$ sudo make install

Add /usr/local/Qt-5.14.2/bin to your path:

$ nano .bashrc
At the end of the file add:
export PATH=$PATH:/usr/local/Qt-5.14.2/bin

Open a new terminal

$ cd traptaserver
$ qmake
$ make

Now we can run the TRAPTA binary.

Here is the tree:

lib
plugins
qt.conf
TRAPTA


Here is the content of the lib folder:

drwxr-xr-x 2 ubuntu-dev2 ubuntu-dev2    4096 juin  30 09:45 ./
drwxr-xr-x 4 ubuntu-dev2 ubuntu-dev2    4096 août  15  2018 ../
lrwxrwxrwx 1 ubuntu-dev2 ubuntu-dev2      20 juin  30 09:38 libQt5Core.so.5 -> libQt5Core.so.5.14.2*
-rwxr-xr-x 1 ubuntu-dev2 ubuntu-dev2 6744056 juin  30 09:36 libQt5Core.so.5.14.2*
lrwxrwxrwx 1 ubuntu-dev2 ubuntu-dev2      19 juin  30 09:39 libQt5Gui.so.5 -> libQt5Gui.so.5.14.2*
-rwxr-xr-x 1 ubuntu-dev2 ubuntu-dev2 7028144 juin  30 09:36 libQt5Gui.so.5.14.2*
lrwxrwxrwx 1 ubuntu-dev2 ubuntu-dev2      23 juin  30 09:39 libQt5Network.so.5 -> libQt5Network.so.5.14.2*
-rwxr-xr-x 1 ubuntu-dev2 ubuntu-dev2 1363472 juin  30 09:37 libQt5Network.so.5.14.2*
lrwxrwxrwx 1 ubuntu-dev2 ubuntu-dev2      19 juin  30 09:39 libQt5Sql.so.5 -> libQt5Sql.so.5.14.2*
-rwxr-xr-x 1 ubuntu-dev2 ubuntu-dev2  306608 juin  30 09:37 libQt5Sql.so.5.14.2*
lrwxrwxrwx 1 ubuntu-dev2 ubuntu-dev2      23 juin  30 09:39 libQt5Widgets.so.5 -> libQt5Widgets.so.5.14.2*
-rwxr-xr-x 1 ubuntu-dev2 ubuntu-dev2 6746008 juin  30 09:37 libQt5Widgets.so.5.14.2*
lrwxrwxrwx 1 ubuntu-dev2 ubuntu-dev2      22 juin  30 09:45 libQt5XcbQpa.so.5 -> libQt5XcbQpa.so.5.14.2*
-rwxr-xr-x 1 ubuntu-dev2 ubuntu-dev2 1104456 juin  30 09:37 libQt5XcbQpa.so.5.14.2*

Here is the content of the plugins folder:

plugins/imageformat:

drwxr-xr-x 2 ubuntu-dev2 ubuntu-dev2   4096 août  15  2018 ./
drwxr-xr-x 5 ubuntu-dev2 ubuntu-dev2   4096 févr. 16  2018 ../
-rwxr-xr-x 1 ubuntu-dev2 ubuntu-dev2 404592 juin  30 09:52 libqjpeg.so*
-rwxr-xr-x 1 ubuntu-dev2 ubuntu-dev2  23200 juin  30 09:52 libqsvg.so*

plugins/platforms:

drwxr-xr-x 2 ubuntu-dev2 ubuntu-dev2  4096 août  15  2018 ./
drwxr-xr-x 5 ubuntu-dev2 ubuntu-dev2  4096 févr. 16  2018 ../
-rwxr-xr-x 1 ubuntu-dev2 ubuntu-dev2 10512 juin  30 09:53 libqxcb.so*

plugins/sqldrivers:

drwxr-xr-x 2 ubuntu-dev2 ubuntu-dev2    4096 août  15  2018 ./
drwxr-xr-x 5 ubuntu-dev2 ubuntu-dev2    4096 févr. 16  2018 ../
-rwxr-xr-x 1 ubuntu-dev2 ubuntu-dev2 1201096 juin  30 09:56 libqsqlite.so*

But the link to the dynamic libraries is the absolute path for this machine. We need to build a folder with the dynamic libraries and change the path to be relative.

$ ldd ./TRAPTA
	linux-vdso.so.1 (0x00007ffe3fcb5000)
	libQt5Widgets.so.5 => /usr/local/Qt-5.14.2/lib/libQt5Widgets.so.5 (0x00007fc7af0a3000)
	libQt5Gui.so.5 => /usr/local/Qt-5.14.2/lib/libQt5Gui.so.5 (0x00007fc7ae7e8000)
	libQt5Network.so.5 => /usr/local/Qt-5.14.2/lib/libQt5Network.so.5 (0x00007fc7ae49b000)
	libQt5Sql.so.5 => /usr/local/Qt-5.14.2/lib/libQt5Sql.so.5 (0x00007fc7ae250000)
	libQt5Core.so.5 => /usr/local/Qt-5.14.2/lib/libQt5Core.so.5 (0x00007fc7ad9de000)
	libstdc++.so.6 => /usr/lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007fc7ad655000)
	libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007fc7ad2b7000)
	libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007fc7ad09f000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fc7accae000)
	libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007fc7aca8f000)
	libGL.so.1 => /usr/lib/x86_64-linux-gnu/libGL.so.1 (0x00007fc7ac803000)
	libpng16.so.16 => /usr/lib/x86_64-linux-gnu/libpng16.so.16 (0x00007fc7ac5d1000)
	libz.so.1 => /lib/x86_64-linux-gnu/libz.so.1 (0x00007fc7ac3b4000)
	libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007fc7ac1b0000)
	/lib64/ld-linux-x86-64.so.2 (0x00007fc7af912000)
	libGLX.so.0 => /usr/lib/x86_64-linux-gnu/libGLX.so.0 (0x00007fc7abf7f000)
	libGLdispatch.so.0 => /usr/lib/x86_64-linux-gnu/libGLdispatch.so.0 (0x00007fc7abcc9000)
	libX11.so.6 => /usr/lib/x86_64-linux-gnu/libX11.so.6 (0x00007fc7ab991000)
	libxcb.so.1 => /usr/lib/x86_64-linux-gnu/libxcb.so.1 (0x00007fc7ab769000)
	libXau.so.6 => /usr/lib/x86_64-linux-gnu/libXau.so.6 (0x00007fc7ab565000)
	libXdmcp.so.6 => /usr/lib/x86_64-linux-gnu/libXdmcp.so.6 (0x00007fc7ab35f000)
	libbsd.so.0 => /lib/x86_64-linux-gnu/libbsd.so.0 (0x00007fc7ab14a000)
	librt.so.1 => /lib/x86_64-linux-gnu/librt.so.1 (0x00007fc7aaf42000)

As you can see, the paths are absolute (/usr/local/Qt-5.14.2/lib/) and hardcoded to a environment.
Instead of /usr/local/Qt-5.14.2/lib/, we want it to be ./lib/

$ chrpath -r ./lib/ ./TRAPTA

Output:
./TRAPTA: RUNPATH=/usr/local/Qt-5.14.2/lib
./TRAPTA: new RUNPATH: ./lib/

$ ldd ./TRAPTA
	linux-vdso.so.1 (0x00007ffd4335c000)
	libQt5Widgets.so.5 => ./lib/libQt5Widgets.so.5 (0x00007fdb99e36000)
	libQt5Gui.so.5 => ./lib/libQt5Gui.so.5 (0x00007fdb9957b000)
	libQt5Network.so.5 => ./lib/libQt5Network.so.5 (0x00007fdb9922e000)
	libQt5Sql.so.5 => ./lib/libQt5Sql.so.5 (0x00007fdb98fe3000)
	libQt5Core.so.5 => ./lib/libQt5Core.so.5 (0x00007fdb98771000)
	libstdc++.so.6 => /usr/lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007fdb983e8000)
	libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007fdb9804a000)
	libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007fdb97e32000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fdb97a41000)
	libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007fdb97822000)
	libGL.so.1 => /usr/lib/x86_64-linux-gnu/libGL.so.1 (0x00007fdb97596000)
	libpng16.so.16 => /usr/lib/x86_64-linux-gnu/libpng16.so.16 (0x00007fdb97364000)
	libz.so.1 => /lib/x86_64-linux-gnu/libz.so.1 (0x00007fdb97147000)
	libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007fdb96f43000)
	/lib64/ld-linux-x86-64.so.2 (0x00007fdb9a6a5000)
	libGLX.so.0 => /usr/lib/x86_64-linux-gnu/libGLX.so.0 (0x00007fdb96d12000)
	libGLdispatch.so.0 => /usr/lib/x86_64-linux-gnu/libGLdispatch.so.0 (0x00007fdb96a5c000)
	libX11.so.6 => /usr/lib/x86_64-linux-gnu/libX11.so.6 (0x00007fdb96724000)
	libxcb.so.1 => /usr/lib/x86_64-linux-gnu/libxcb.so.1 (0x00007fdb964fc000)
	libXau.so.6 => /usr/lib/x86_64-linux-gnu/libXau.so.6 (0x00007fdb962f8000)
	libXdmcp.so.6 => /usr/lib/x86_64-linux-gnu/libXdmcp.so.6 (0x00007fdb960f2000)
	libbsd.so.0 => /lib/x86_64-linux-gnu/libbsd.so.0 (0x00007fdb95edd000)
	librt.so.1 => /lib/x86_64-linux-gnu/librt.so.1 (0x00007fdb95cd5000)

Now the paths are relative. Good.

Now we can tar the folder. The package is ready.














