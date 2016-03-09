#! /usr/bin/python

class A1(object):
    def method(*argv): return argv

a = A1()
print 'Call a.method()      ->', a.method()
print 'Call a.method(1,2,3) ->', a.method(1,2,3)
print '''
      !! Note that the \'a\' instance is passed (implicitely) as the first
      !! function parameter (something like <__main__.A1 object at 0x7f...>
      '''

''' The following will throw as A1.method is a bound method (i.e. non-static)
    of the A1 class:
    TypeError: unbound method method() must be called with A1 instance as 
    first argument (got int instance instead)
'''
# print 'Call A.method(1,2,3) ->', A1.method(1,2,3)


''' A static method does not receive an implicit first argument.
    It (i.e. the static method) can be called either on the class 
    (such as A.method()) or on an instance (such as A().method()). The 
    instance is ignored except for its class.
'''
class A2(object):
    @staticmethod
    def method(*argv): return argv

a = A2()
print 'Call a.method()      ->', a.method()
print 'Call a.method(1,2,3) ->', a.method(1,2,3)
print 'Call A.method(1,2,3) ->', A2.method(1,2,3) ## static call
print '''
      !! Note that no class instance is is passed to the call
      '''

''' So in a normal (class) bound method call, the instance is passed implicitely
    as the first argument, whereas for a static method no implicit arguments are
    passed; the method is invoked via scoping the class name.
    There is a third option, where we do pass an implicit first argument, but
    NOT an instance; instead the argument is the class type itself. That is,
    A class method receives the class as implicit first argument, just like an
    instance method receives the instance.
    It can be called either on the class (such as A3.method()) or on an 
    instance (such as A3().method()). The instance is ignored except for its 
    class. If a class method is called for a derived class, the derived class 
    object is passed as the implied first argument.
'''
class A3(object):
    @classmethod
    def method(*argv): return argv

a = A3()
print 'Call a.method()      ->', a.method()
print 'Call a.method(1,2,3) ->', a.method(1,2,3)
print 'Call A.method(1,2,3) ->', A3.method(1,2,3) ## static call
print '''
      !! Note that The class object (i.e. something like <class '__main__.A3'>)
      !! is (implicitely) passed as the first argument.
      '''
