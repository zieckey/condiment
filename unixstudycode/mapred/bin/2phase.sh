#!/bin/bash

#~/software/hadoop/bin/hadoop jar ~/software/hadoop/contrib/streaming/hadoop-0.20.1.12-streaming.jar -mapper "php map.php" -reducer "php reduce.php" -input /hadoop_game/data/  -output /hadoop_game/output/weizili  -file map.php -file reduce.php

if [ $# -lt 1 ];then
    echo "Usage $0 [phase1_output_dir_name]"
    echo "For example: $0 phase1_output_20111225_124122"
    exit
fi


_datenow=`date +%Y%m%d`
_timenow=`date +%H%M%S`
_time=${_datenow}_${_timenow}

_phase1_output_dir=$1
_reduce_task=1

if [ $# -eq 2 ];then
_reduce_task=$2
fi

exec 2>> ./run.log
set -x

_phase2_output=/hadoop_game/weizili/phase2_output_${_time}

#_reader=LineReader
_reader=BufferReader

#_writer=BufferWriter
_writer=ThreadWriter

~/software/hadoop/bin/hadoop jar ~/software/hadoop/contrib/streaming/hadoop-0.20.1_v2-streaming.jar \
        -mapper "./phase2_map  --file_reader_type=${_reader} --file_writer_type=${_writer} " \
        -reducer "./phase2_reduce  --file_reader_type=${_reader} --file_writer_type=${_writer} " \
        -numReduceTasks ${_reduce_task} \
        -input /hadoop_game/weizili/${_phase1_output_dir}  \
        -output ${_phase2_output} \
        -file phase2_map -file phase2_reduce

_result="./result_phase2.sort.txt"
_result_temp="./result_phase2.temp.txt"
rm -rf ${_result_temp}
~/software/hadoop/bin/hadoop fs -getmerge ${_phase2_output} ${_result_temp}
sort ${_result_temp} > ${_result}

