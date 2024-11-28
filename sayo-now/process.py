import argparse
import time
import json
import math
import itertools

def atoi(s):
    s=str(s)
    return int(s[2:],16) if s.startswith("0x") else int(s)

defs = {}
args = {}
def get_arg_len(s:str):
    if s == "RGB888":
        return 3
    x=0
    for c in s:
        if not c.isdigit(): continue
        x=x*10+int(c)
    if x==0: x=8
    return x//8

with open("translate_config/def.txt", 'r', encoding='UTF-8') as def_file:
    for line in def_file.readlines():
        line = line.rstrip()
        pos = line.find('#')
        if pos != -1:
            line = line[:pos].rstrip()
        
        pos=line.find(' ')
        defs[line[:pos]]=line[pos:]
        
        
with open("translate_config/args.txt", 'r', encoding='UTF-8') as arg_file:
    for line in arg_file.readlines():
        line = line.rstrip()
        pos = line.find('#')
        if pos != -1:
            line = line[:pos].rstrip()
        
        line=line.split()
        if len(line) == 0:
            continue
        args[atoi(line[0])]=line[1:]
        
        # print(line[0],args[line[0]])

parser = argparse.ArgumentParser(prog='process')

parser.add_argument("input_file")
parser.add_argument("output_file")
parser.add_argument('--debug',action='store_true')

sys_args=parser.parse_args()

labels = {}
p = 0

with open(sys_args.input_file, 'r' , encoding='UTF-8') as fin:
    code = fin.readlines()
    


def process_line(line:str):
    line = line.rstrip()
    pos = line.find('#')
    if pos != -1:
        line = line[:pos].rstrip()
    
    def pas(s:str):
        return ' '.join(list(map(lambda word: defs.get(word,word).strip(), s.split(' '))))
    
    nw = pas(line)
    while nw != line:
        line = nw
        nw = pas(line)
    return line

word_cnt={}

for line in code:
    line=process_line(line)
    words=line.split()
    for word in words:
        if word not in word_cnt:
            word_cnt[word]=1
        else:
            word_cnt[word]+=1
    
# opts that must jump and never back
jump_opts=[0x55,0xfe,0xff,0xf5,0x2] # RET,RES,EXIT,JMP_TO_SCRIPT,JMP

# print(jump_opts)

cur_fn = "__program_entrance"
fn_code = {}
fn_to = {}
vis_fn = set()

for line in code:
    line=process_line(line)
    if line.startswith("label __fn_"):
        cur_fn=line[6:]
        fn_code[cur_fn]=[]
        fn_to[cur_fn]=[]
        continue
    if cur_fn == "__program_entrance":
        continue
    if (pos:=line.find("__fn_")) != -1:
        fn_to[cur_fn].append(line[pos:].split()[0])
    fn_code[cur_fn].append(line)

code = []
def dfs(fn:str):
    vis_fn.add(fn)
    code.append("label "+fn)
    for line in fn_code[fn]:
        code.append(line)
    for v in fn_to[fn]:
        if v not in vis_fn:
            dfs(v)
dfs("__fn_main")
jumped:bool=False

word_cnt={}

for line in code:
    line=process_line(line)
    words=line.split()
    for word in words:
        if word not in word_cnt:
            word_cnt[word]=1
        else:
            word_cnt[word]+=1

new_code = []
for line in code:
    words=line.split()
    
    if len(words) == 0:
        continue
    
    if words[0] == 'label':
        if word_cnt[words[1]] == 1:
            continue
        if sys_args.debug:
            print(p,words)
        jumped=False
        labels[words[1]]=p
        continue
    
    if jumped:
        continue
    
    if sys_args.debug:
        print(p,words)
        
    new_code.append((p,words))
    
    if atoi(words[0]) in jump_opts:
        jumped=True
        
    
    p+=1
    for arg in args[atoi(words[0])]:
        p+=get_arg_len(arg)
    

res = []


for id,words in new_code:
    opt:int=atoi(words[0])
    values=[]
    words=list(map(lambda word:labels.get(word,word),words[1:]))
    for i in range(len(words)):
        arg:str=args[opt][i]
        num=atoi(words[i])
        l=get_arg_len(arg)
        if arg.upper().find("BE")==-1: # LE
            for _ in range(l):
                values.append(num%256)
                num //= 256
        else:
            tmp=[]
            for _ in range(l):
                tmp.append(num%256)
                num //= 256
            tmp.reverse()
            for _ in tmp:
                values.append(_)
    res.append({"mode":opt,"values":values})
    
        
with open(sys_args.output_file,'w',encoding="UTF-8") as fout:
    fout.write(json.dumps({"lastModify":int(time.time()),"name":"script","steps":res}))
    