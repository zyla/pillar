import sys
sys.path.append('./deps/scons-compiledb')

import scons_compiledb

env = Environment(tools = ['default', 'clang++'])
scons_compiledb.enable(env)

sources = Glob('src/**/*.cc') + Glob('src/*.cc')
env.Program('target/tests', sources + ['deps/gtest/googletest/src/gtest-all.cc', 'deps/gtest/googletest/src/gtest_main.cc'],
    CPPPATH=['src', 'deps/gtest/googletest', 'deps/gtest/googletest/include'],
    CPPFLAGS='-std=c++17 -g -fcolor-diagnostics',
    LIBS=['pthread'])

env.CompileDb()

sources_notest = filter(lambda f: '_test.cc' not in f.name, sources)
# TODO: build some actual program besides tests

# vi: ft=python
