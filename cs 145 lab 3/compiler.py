import sys

freq_key = [
  'C5', 'CSHARP5', 'D5', 'DSHARP5', 'E5', 'F5', 'FSHARP5', 'G5', 'GSHARP5', 'A5', 'ASHARP5', 'B5',
  'C6', 'CSHARP6', 'D6', 'DSHARP6', 'E6', 'F6', 'FSHARP6', 'G6', 'GSHARP6', 'A6', 'ASHARP6', 'B6',
  'C7', 'CSHARP7', 'D7', 'DSHARP7', 'E7', 'F7', 'FSHARP7', 'G7', 'GSHARP7', 'A7', 'ASHARP7', 'B7',
  'REST'
];
freq_val = '0123456789abcdefghijklmnopqrstuvwxyz!';
duration_key = [ 'wholedot', 'whole', 'halfdot', 'half', 'quarterdot', 'quarter', 'eighthdot', 'eighth', 'sixteenth', 'thirtysecond'];
duration_val = '0123456789';

buffer = "";

def findFreq(str):
  for i in range(0, len(freq_key)):
    if str == freq_key[i]:
      return freq_val[i]
  return None
  
def findDuration(str):
  for i in range(0, len(duration_key)):
    if str == duration_key[i]:
      return duration_val[i]
  return None

with open(sys.argv[1]) as f:
    content = f.readlines()
    for line in content:
      parts = line.split()
      if len(parts) == 3:
        if parts[0] != 'tie':
          print 'Error'
          exit()
          # error
        else:
          buffer += findDuration(parts[1]) + findFreq(parts[2]) + '1'
      elif len(parts) is 2:
        buffer += findDuration(parts[0]) + findFreq(parts[1]) + '0'

print buffer    