project('byzantine', 'cpp',
        license : ['Boost'],
        version : '0.1',
        default_options : ['cpp_std=c++17', 'warning_level=3', 'werror=true'])

inc_dir = include_directories(['include', 
                                get_option('asio_include_dir'), 
                                get_option('cli11_include_dir'), 
                                get_option('spdlog_include_dir')])

threadd = dependency('threads')

commander_src = ['src/commander/main.cpp']

executable('commander',
           sources : commander_src,
           dependencies: threadd,
           include_directories : inc_dir
          )

leutnant_src = ['src/leutnant/main.cpp']

executable('leutnant',
           sources : leutnant_src,
            dependencies: threadd,
           include_directories : inc_dir
          )
