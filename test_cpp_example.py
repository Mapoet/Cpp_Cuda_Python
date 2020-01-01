#!/usr/bin/python3
#%%
import cpp_example
# import iri2016
# import iri2016.plots as pt
# from matplotlib.pyplot import show
# import datetime as dt
# import pytest
p1=cpp_example.point(2,0)
p2=cpp_example.point(6,3)
l2=cpp_example.line(p1,p2) 
p3=l2.dif
print(l2.dif.x,l2.dif.y)
print(l2.abs,p3.abs)
print(p1.abs,p2.abs)
print(p1.to_json(),p2.to_json())
print(l2.to_json())
l3=cpp_example.from_json(l2.to_json())
print(l3.to_json())
# out=iri2016.IRI(dt.datetime(2008,8,1,0,0,0),[80,800,10],34.,120.)
# pt.altprofile(out)
# show()
# print(out)
# %%
