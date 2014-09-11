#!/bin/bash
#~/software/hadoop/bin/hadoop jar ~/software/hadoop/contrib/streaming/hadoop-0.20.1.12-streaming.jar -mapper "php map.php" -reducer "php reduce.php" -input /hadoop_game/data/  -output /hadoop_game/output/weizili  -file map.php -file reduce.php

_datenow=`date +%Y%m%d`
_timenow=`date +%H%M%S`
_time=${_datenow}_${_timenow}

exec 2>> ./run.log
set -x
_phase1_output=/hadoop_game/weizili/phase1_output_${_time}
_reduce_task=2

#_reader=LineReader
_reader=BufferReader

_writer=BufferWriter
#_writer=ThreadWriter

~/software/hadoop/bin/hadoop jar ~/software/hadoop/contrib/streaming/hadoop-0.20.1_v2-streaming.jar \
    -mapper "./phase1_map --file_reader_type=${_reader} --file_writer_type=${_writer}" \
    -reducer "./phase1_reduce --file_reader_type=${_reader}  --file_writer_type=${_writer}" \
    -numReduceTasks ${_reduce_task} \
    -Dmapred.compress.output=true -Dmapred.output.compression.codec=org.apache.hadoop.io.compress.GzipCode \
    -input /hadoop_game/data/  -output ${_phase1_output} -file phase1_map -file phase1_reduce 

#-jobconf mapred.compress.map.output=true -jobconf mapred.map.output.compression.codec=org.apache.hadoop.io.compress.GzipCode \

_result="./result_phase1.sort.txt"
_result_temp="./result_phase1.temp.txt"
rm -rf ${_result_temp}
~/software/hadoop/bin/hadoop fs -getmerge ${_phase1_output} ${_result_temp}
sort ${_result_temp} > ${_result}

