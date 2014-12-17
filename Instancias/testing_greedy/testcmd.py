import subprocess
import glob

for nomArch in glob.glob('*') :
    if "time" in nomArch:
        rep=0
        result=[]
        ress=[]
        while(rep<2):		
            batcmd="..\greedy_tim.exe " + nomArch
            res = subprocess.check_output(batcmd, shell=True)
            print res
           # result.append(res)
           # ress.append(float(res[2:]))
            rep=rep+1
        #print (result[0])[:2], min(ress)
