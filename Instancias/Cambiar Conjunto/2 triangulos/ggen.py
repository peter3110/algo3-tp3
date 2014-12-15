m=0
n=1
k=3
base=[(1,2),(1,3),(2,3),(2,4),(3,4)]

while(n<110):
    n=n+3
    m=5+m
    title='desfav_vec1_'+str(n)
    tit2='sol_desfav_vec1_'+str(n)
    prob=open(title,'w')
    
    prob.write(str(n)+' '+str(m)+' '+str(k)+'\n')
    
    for i in xrange(m/5):
        for b in base:
            prob.write(str(b[0]+i*3)+' '+str(b[1]+i*3)+' '+str(n)+'\n')
    
    sol=open(tit2,'w')
    sol0='1 2 2 3 '
    for w in range(n/3-1):
        sol0=sol0+'2 2 3 '
    sol.write(sol0)
    sol.close()
    prob.close()    

