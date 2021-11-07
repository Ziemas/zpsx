project('zpsx', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++17'])

fmt_dep = dependency('fmt')

sources = files('zpsx.cpp', 'cpu.cpp', 'bus.cpp')

exe = executable('zpsx', sources,
  install : true,
  dependencies : fmt_dep)

test('basic', exe)
