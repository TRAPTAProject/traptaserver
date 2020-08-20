Building trapta server for linux:

- Build the trapta_ubuntu docker image (https://github.com/TRAPTAProject/trapta-build)
- Make sure this image is loaded in docker and refered as trapta_ubuntu:1.1

$ docker images
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
trapta_ubuntu       1.1                 df7edac81a15        20 hours ago        658MB


In a terminal:

cd to the directory of this readme.txt file, then:

$ docker run --rm -v ${PWD}:/host trapta_ubuntu:1.1 bash /host/build.sh

Once done, you can find the archive (.tar.gz) in the current dir
