from polybori.PyPolyBoRi import BooleSet,Variable,Polynomial,mod_mon_set, if_then_else, Monomial, top_index

#def all_monomials_of_degree_d(d,variables):
#    res=all_monomials_of_degree_d_new(d, variables)
#    ref=all_monomials_of_degree_d_old(d, variables)
#    assert res==ref, (d, variables)
#    return res
def all_monomials_of_degree_d_old(d,variables):
    if d==0:
        return Polynomial(1).set()
    if len(variables)==0:
        return BooleSet()
    variables=sorted(set(variables),reverse=True,key=top_index)

    m=variables[-1]
    for v in variables[:-1]:
        m=v+m
    m=m.set()
    i=0
    res=Polynomial(1).set()
    while(i<d):
        i=i+1
        res=res.cartesianProduct(m).diff(res)
    return res

def all_monomials_of_degree_d(d, variables):
    
    variables=Monomial(variables)
    variables=list(variables.variables())
    if d>len(variables):
        return Polynomial(0)
    if d<0:
        return Polynomial(1)
    if len(variables)==0:
        assert d==0
        return 1
    deg_variables=variables[-d:]
    #this ensures sorting by indices
    res=Monomial(deg_variables)
    ring=Polynomial(variables[0]).ring()
    for i in xrange(1, len(variables)-d+1):
        deg_variables=variables[-d-i:-i]
        res=Polynomial(res)
        nav=res.navigation()
        navs=[]
        while not nav.constant():
            navs.append(BooleSet(nav,ring))
            nav=nav.then_branch()
        #navs=reversed(nav)
        acc=Polynomial(1)
        for (nav, v) in reversed(zip(navs, deg_variables)):
            acc=if_then_else(v, acc, nav)
        res=acc
    return res.set()
    

def power_set(variables):
    variables=sorted(set(variables),reverse=True,key=top_index)
    res=Polynomial(1).set()
    for v in variables:
        res=if_then_else(v,res,res)
    return res 
if __name__=='__main__':
    from blocks import declare_ring,Block
    r=declare_ring([Block("x",10000)],globals())
    print list(all_monomials_of_degree_d(0,[Variable(i) for i in range(100)]))
    print list(all_monomials_of_degree_d(1,[Variable(i) for i in range(10)]))
    print list(all_monomials_of_degree_d(2,[Variable(i) for i in range(4)]))
    print list(all_monomials_of_degree_d(3,[Variable(i) for i in range(4)]))
    print list(all_monomials_of_degree_d(4,[Variable(i) for i in range(4)]))
    print list(all_monomials_of_degree_d(0,[]))
    print list(all_monomials_of_degree_d(1,[]))
    print list(power_set([Variable(i) for i in range(2)]))
    print list(power_set([Variable(i) for i in range(4)]))
    print list(power_set([]))
    #every monomial in the first 8 var, which is at most linear in the first 5
    print list(mod_mon_set(power_set([Variable(i) for i in range(8)]),all_monomials_of_degree_d(2,[Variable(i) for i in range(5)])))
    
    #specialized normal form computation
    print Polynomial(
        mod_mon_set(
            (x(1)*x(2)+x(1)+1).set(),
            all_monomials_of_degree_d(2,[Variable(i) for i in range(1000)])))
    print list(mod_mon_set(power_set([Variable(i) for i in range(50)]),all_monomials_of_degree_d(2,[Variable(i) for i in range(1000)])))
def monomial_from_indices(indices):
    l=sorted(indices,reverse=True)
    res=Monomial()
    for i in l:
        res=res*Variable(i)
        
    return res
