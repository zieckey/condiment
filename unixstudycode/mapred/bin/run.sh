#!/bin/bash

#~/software/hadoop/bin/hadoop jar ~/software/hadoop/contrib/streaming/hadoop-0.20.1.12-streaming.jar -mapper "php map.php" -reducer "php reduce.php" -input /hadoop_game/data/  -output /hadoop_game/output/weizili  -file map.php -file reduce.php

_datenow=`date +%Y%m%d`
_timenow=`date +%H%M%S`
_time=${_datenow}_${_timenow}

mkdir -p logs

exec 2>> ./logs/trace.log
set -x
_weizili_dir=/hadoop_game/weizili
_phase1_output=${_weizili_dir}/phase1_output_${_time}
_phase2_output=${_weizili_dir}/phase2_output_${_time}

~/software/hadoop/bin/hadoop fs -test ${_weizili_dir}
if [ $0 -ne 0 ];then
    ~/software/hadoop/bin/hadoop fs -mkdir ${_weizili_dir}
fi    

_phase1_reduce_tasks=70

~/software/hadoop/bin/hadoop jar ~/software/hadoop/contrib/streaming/hadoop-0.20.1_v2-streaming.jar \
        -mapper "./phase1_map" \
        -reducer "./phase1_reduce" \
        -numReduceTasks ${_phase1_reduce_tasks} \
        -jobconf mapred.compress.map.output=true \
        -input /hadoop_game/data/  \
        -output ${_phase1_output} \
        -file phase1_map -file phase1_reduce

#-jobconf mapred.output.compress=true\

_result="./result.sort.txt"
_result_temp="./temp.txt"
rm -rf ${_result} ${_result_temp}
~/software/hadoop/bin/hadoop fs -getmerge ${_phase1_output} ${_result_temp}
./phase2_reduce --fin_path=${_result_temp} --fout_path=${_result}


