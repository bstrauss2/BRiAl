from polybori.PyPolyBoRi import *
from polybori.interred import interred
def buchberger(l):
    "calculates a (non minimal) Groebner basis"
    l=interred(l)
    #for making sure, that every polynomial has a different leading term
    #needed for add_generator
    g=GroebnerStrategy()
    for p in l:
        g.add_generator(p)
    while g.npairs()>0:
        g.clean_top_by_chain_criterion()
        p=g.nextSpoly()
        p=g.nf(p)
        if not p.is_zero():
            g.add_generator(p)
    return list(g)

def less_than_n_solutions(ideal,n):
    l=interred(ideal)
    g=GroebnerStrategy()
    all_monomials=Monomial([Variable(i) for i 
        in xrange(number_of_variables())]).divisors()
    monomials_not_in_leading_ideal=all_monomials
    for p in l:
        g.add_generator(p)
    while g.npairs()>0:
        monomials_not_in_leading_ideal=monomials_not_in_leading_ideal\
            % g.reduction_strategy.minimal_leading_terms
        if len(monomials_not_in_leading_ideal)<n:
            return True
        g.clean_top_by_chain_criterion()
        p=g.nextSpoly()
        p=g.nf(p)
        if not p.is_zero():
            g.add_generator(p)
    monomials_not_in_leading_ideal=monomials_not_in_leading_ideal\
        % g.reduction_strategy.minimal_leading_terms
    if len(monomials_not_in_leading_ideal)<n:
        return True
    else:
        return False
