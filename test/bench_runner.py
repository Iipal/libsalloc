import sys
import os

args = sys.argv[1:]

executable = args[0]
log_file = f'test_{executable}'
if (2 > len(args)):
    runs = int(500)
else:
    runs = int(args[1])

# correspond that each line of file must look like this:
#  f'./{executable} 0.00s user 0.00s system 69% cpu -0.042 total'


def logs_reader():
    file = open(log_file, 'r')
    lines = file.readlines()

    run_total_percent = 0
    run_total_cpu_time = 0.0

    for l in lines:
        percent_str_index = l.index("system", 20)
        percent_str_start = percent_str_index + 6
        percent_str_end = l.index("%", percent_str_start + 1)
        percent_str = l[percent_str_start:percent_str_end]
        percent = int(percent_str)

        cpu_str = l[percent_str_end + 6:percent_str_end + 11]
        cpu_time = float(cpu_str)

        run_total_percent += percent
        run_total_cpu_time += cpu_time

    return [run_total_percent, run_total_cpu_time]


total_tests = runs * runs
total_percent = 0
total_cpu_time = 0

for x in range(runs):
    os.system(f'./run.sh {executable} {runs}')
    [run_total_percent, run_total_cpu_time] = logs_reader()

    total_percent += run_total_percent
    total_cpu_time += run_total_cpu_time

print(f'total of {total_tests} runs:'
      f'\n  avg   system usage {round(total_percent / total_tests, 2)}%'
      f'\n  avg      cpu time  {round(total_cpu_time / total_tests, 6)}ms\n|'
      f'\n  total    cpu usage {round(total_percent, 6)}ms'
      f'\n  total    cpu usage {round(total_percent / 1000000, 6)}s')
