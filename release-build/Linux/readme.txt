apt-get -y install git wget xz-utils build-essential libfontconfig1-dev libfreetype6-dev libx11-dev libxext-dev libxfixes-dev libxi-dev libxrender-dev libxcb1-dev libx11-xcb-dev libxcb-glx0-dev libxkbcommon-x11-dev mesa-common-dev freeglut3 freeglut3-dev libglew1.5 libglew1.5-dev libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx libgl1-mesa-dev chrpath python
wget https://download.qt.io/archive/qt/5.14/5.14.2/single/qt-everywhere-src-5.14.2.tar.xz
tar xf qt-everywhere-src-5.14.2.tar.xz
cd qt-every...
rm -rf web* qt3d qtmultimedia qtvirtualkeyboard qtscript qtremoteobjects qtlocation qtandroidextras qtcanvas3d qtcharts qtdatavis3d qtgamepad qtmacextras qtpurchasing qtscxml qtsensors qtserialbus qtserialport qtspeech qtvirtualkeyboard qtwinextras qtwebchannel qtwebengine qtwebglplugin qtwebsockets qtwebview qtwayland qtxmlpatterns qtgraphicaleffects qtconnectivity qtactiveqt qtnetworkauth qttools qtlottie qtquick3d qtquicktimeline
./configure -opensource -nomake tests -nomake examples -qt-xcb -no-icu -confirm-license -no-dbus -sql-sqlite
make
make install
cd ..
git clone https://github.com/TRAPTAProject/traptaserver.git
cd traptaserver
/usr/local/Qt-5.14.2/bin/qmake
make
Copy all plugins and libraries

cp /usr/local/Qt-5.12.9/lib/libQt5Quick.so.5.12.9 ./libQt5Quick.so.5
pi@raspberrypi:~/TRAPTAViewer/lib $ cp /usr/local/Qt-5.12.9/lib/libQt5Core.so.5.12.9 ./libQt5Core.so.5
pi@raspberrypi:~/TRAPTAViewer/lib $ cp /usr/local/Qt-5.12.9/lib/libQt5Gui.so.5.12.9 ./libQt5Gui.so.5
pi@raspberrypi:~/TRAPTAViewer/lib $ cp /usr/local/Qt-5.12.9/lib/libQt5Network.so.5.12.9 ./libQt5Network.so.5
pi@raspberrypi:~/TRAPTAViewer/lib $ cp /usr/local/Qt-5.12.9/lib/libQt5OpenGL.so.5.12.9 ./libQt5OpenGL.so.5
pi@raspberrypi:~/TRAPTAViewer/lib $ cp /usr/local/Qt-5.12.9/lib/libQt5Qml.so.5.12.9 ./libQt5Qml.so.5
pi@raspberrypi:~/TRAPTAViewer/lib $ cp /usr/local/Qt-5.12.9/lib/libQt5Widgets.so.5.12.9 ./libQt5Widgets.so.5

chrpath -r ./lib/ ./TRAPTA

docker built -t trapta_ubuntu:1.1 .
docker save trapta_ubuntu:1.1 | gzip > trapta_ubuntu_1.1.tar.gz

# Add file in current host dir
docker run --rm -v ${PWD}:/host trapta_ubuntu:1.1 touch /host/newfile

# execute host test.sh with container
Building trapta server for linux:

- Build the trapta_ubuntu docker image (https://github.com/TRAPTAProject/trapta-build)
- Make sure this image is loaded in docker and refered as trapta_ubuntu:1.1

$ docker images
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
trapta_ubuntu       1.1                 df7edac81a15        20 hours ago        658MB


In a terminal:

cd to the directory of this readme.txt file, then:

$ docker run --rm -v ${PWD}:/host trapta_ubuntu:1.1 bash /host/build.sh

Once done, you will find in the archive (.tar.gz) in the current dir
