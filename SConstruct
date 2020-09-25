env = Environment(tools = ['default', 'clang++'])
sources = Glob('src/**/*.cc')
env.Program('target/tests', sources + ['deps/gtest/googletest/src/gtest-all.cc', 'deps/gtest/googletest/src/gtest_main.cc'],
    CPPPATH=['src', 'deps/gtest/googletest', 'deps/gtest/googletest/include'],
    CPPFLAGS='-std=c++17',
    LIBS=['pthread'])

sources_notest = filter(lambda f: '_test.cc' not in f.name, sources)
# TODO: build some actual program besides tests

# vi: ft=python
