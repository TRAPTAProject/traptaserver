# Run this script by to docker container as follow:
# docker run --rm -v ${PWD}:/host trapta_ubuntu:1.1 bash /host/build.sh

#!/bin/bash
cd /home
echo "Creating TRAPTA folder and copying Qt dynamic libraries"
mkdir TRAPTA && cd TRAPTA
mkdir lib && cd lib
cp /usr/local/Qt-5.12.9/lib/libQt5Core.so.5.12.9 libQt5Core.so.5
cp /usr/local/Qt-5.12.9/lib/libQt5Gui.so.5.12.9 libQt5Gui.so.5
cp /usr/local/Qt-5.12.9/lib/libQt5Network.so.5.12.9 libQt5Network.so.5
cp /usr/local/Qt-5.12.9/lib/libQt5Sql.so.5.12.9 libQt5Sql.so.5
cp /usr/local/Qt-5.12.9/lib/libQt5Widgets.so.5.12.9 libQt5Widgets.so.5
cp /usr/local/Qt-5.12.9/lib/libQt5XcbQpa.so.5.12.9 libQt5XcbQpa.so.5
chmod -x *
cd ..
mkdir plugins && cd plugins
mkdir imageformats && cd imageformats
cp /usr/local/Qt-5.12.9/plugins/imageformats/libqjpeg.so .
cp /usr/local/Qt-5.12.9/plugins/imageformats/libqsvg.so .
cp /usr/local/Qt-5.12.9/plugins/imageformats/libqwebp.so .
chmod -x *
cd ..
mkdir platforms && cd platforms
cp /usr/local/Qt-5.12.9/plugins/platforms/libqxcb.so .
chmod -x *
cd ..
mkdir sqldrivers && cd sqldrivers
cp /usr/local/Qt-5.12.9/plugins/sqldrivers/libqsqlite.so .
chmod -x *
cd ../../
touch qt.conf
cd ..

echo "Cloning https://github.com/TRAPTAProject/traptaserver.git into /home"
git clone https://github.com/TRAPTAProject/traptaserver.git
echo "Compiling..."
cd traptaserver
/usr/local/Qt-5.12.9/bin/qmake
make
cd ../TRAPTA
echo "Copying to TRAPTA folder"
cp ../traptaserver/TRAPTA .
echo "Adjusting lib path in binary"
chrpath -r ./lib/ ./TRAPTA
cd ..
echo "Archiving and compressing"
tar zcvf trapta-linux_x86-64.tar.gz TRAPTA
echo "Copying archive to host folder"
cp trapta-linux_x86-64.tar.gz /host


