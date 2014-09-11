#!/bin/sh

#~/software/hadoop/bin/hadoop jar ~/software/hadoop/contrib/streaming/hadoop-0.20.1.12-streaming.jar -mapper "php map.php" -reducer "php reduce.php" -input /hadoop_game/data/  -output /hadoop_game/output/weizili  -file map.php -file reduce.php

_datenow=`date +%Y%m%d`
_timenow=`date +%H%M%S`
_time=${_datenow}_${_timenow}

exec 2>> ./logs/trace.log
set -x
_weizili_dir=/hadoop_game/weizili
_phase1_output=${_weizili_dir}/phase1_output_${_time}
_phase2_output=${_weizili_dir}/phase2_output_${_time}

_phase1_reduce_tasks=2
_enable_thread_worker=false

~/software/hadoop/bin/hadoop jar ~/software/hadoop/contrib/streaming/hadoop-0.20.1_v2-streaming.jar \
        -mapper "./phase1_map --enable_thread_worker=${_enable_thread_worker}" \
        -reducer "./phase1_reduce --enable_thread_worker=${_enable_thread_worker}" \
        -numReduceTasks ${_phase1_reduce_tasks} \
        -input /hadoop_game/data/  -output ${_phase1_output} -file phase1_map -file phase1_reduce

_result="./result.sort.txt"
_result_temp="./result.temp.txt"
rm -rf ${_result_temp} ${_result}
~/software/hadoop/bin/hadoop fs -getmerge ${_phase1_output} ${_result_temp}

./phase2_reduce --fin_path=${_result_temp} --fout_path=${_result}


