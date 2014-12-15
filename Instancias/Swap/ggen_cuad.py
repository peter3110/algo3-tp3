m=1
n=2
k=2




while(n<110):
    ini=1
    tmp=2
    br1=3
    br2=4
    
    n=n+2
    m=3+m
    title='desfav_vec2_'+str(n)
    tit2='sol_desfav_vec2_'+str(n)
    prob=open(title,'w')
    
    prob.write(str(n)+' '+str(m)+' '+str(k)+'\n')
    prob.write(str(ini)+' '+str(tmp)+' ' +str(n)+'\n')

    
    for i in xrange((m-1)/3):
        prob.write(str(ini)+' '+str(br1)+' '+str(n)+'\n')
        prob.write(str(tmp)+' '+str(br2)+' '+str(n)+'\n')
        prob.write(str(br1)+' '+str(br2)+' '+str(n)+'\n')
        ini=br1
        tmp=br2
        br1=br1+2
        br2=br2+2
        
    sol=open(tit2,'w')
    sol0='2 '
    for w in range(n-1):
        sol0=sol0+'1 '
    sol.write(sol0)
    sol.close()
    prob.close()    

