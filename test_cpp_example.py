#!/usr/bin/python3
import cpp_example
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