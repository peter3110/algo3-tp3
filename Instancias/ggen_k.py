import random as rnd
n=1




while(n<110):
    
    n=n+1
    m=((n)*(n-1))/2
    k=n-1

    title='time_'+str(n)
    
    prob=open(title,'w')
    
    prob.write(str(n)+' '+str(m)+' '+str(k)+'\n')
        
    for i in xrange(1,n+1):
        for j in xrange(i+1,n+1):
            prob.write(str(i)+' '+str(j)+' '+str(rnd.randint(1,100))+'\n')

        

    prob.close()    

