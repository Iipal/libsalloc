#!/usr/bin/zsh

EXENAME=$1
RUNS=$2

if [ -z "$EXENAME" ]
then
    EXENAME=clang_build
fi
if [ -z "$RUNS" ]
then
    RUNS=500
fi

RUNNER=`echo time ./$EXENAME`

for x in `seq 1 $RUNS`; do eval $RUNNER;
done 2> test_$EXENAME
