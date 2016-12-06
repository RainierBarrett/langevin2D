#!/bin/bash
printf "Testing langevin2D.cpp for coverage...\n"
PROJECT_DIR=~/langevin2D
GCOV_PATH=`locate -i langevin2D.cpp.gcno | sed -n 1p | sed -e 's/\/langevin2D.cpp.gcno//g'`
printf "(gcov files can be found at ${GCOV_PATH})\n"
#sudo chmod a+rw $GCOV_PATH/langevin.cpp.gcno
gcov $GCOV_PATH/langevin2D.cpp.gcov -n | grep langevin2D.cpp\' -A1
PERCENT=`gcov $GCOV_PATH/langevin2D.cpp.gcov  | grep langevin2D.cpp -A1| grep "[0-9]\+\.[0-9]\+\?" -o`
if `[ -z "$PERCENT" ]`;
then
    echo
else
    sed -i -e "s/[0-9]\+\.[0-9]\+\%/${PERCENT}%/g" $PROJECT_DIR/README.md
fi

