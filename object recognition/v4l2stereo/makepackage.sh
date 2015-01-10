make clean
make gstreamer
rm -rf ~/temp/v4l2stereo
mkdir ~/temp
mkdir ~/temp/v4l2stereo
mkdir ~/temp/v4l2stereo/deb
mkdir ~/temp/v4l2stereo/deb/DEBIAN
mkdir ~/temp/v4l2stereo/deb/usr
mkdir ~/temp/v4l2stereo/deb/usr/bin
cp v4l2stereo ~/temp/v4l2stereo/deb/usr/bin
cp deb/DEBIAN/control ~/temp/v4l2stereo/deb/DEBIAN
cp deb/DEBIAN/copyright ~/temp/v4l2stereo/deb/DEBIAN
cp deb/DEBIAN/docs ~/temp/v4l2stereo/deb/DEBIAN
cp deb/DEBIAN/install ~/temp/v4l2stereo/deb/DEBIAN
sudo chmod -R 0755 ~/temp/v4l2stereo/deb/usr
dpkg -b ~/temp/v4l2stereo/deb v4l2stereo.deb
rm -rf ~/temp/v4l2stereo
echo You may also wish to upload a screenshot for this version to http://screenshots.debian.net/upload
