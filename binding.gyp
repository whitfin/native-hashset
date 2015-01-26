{
  'targets': [
    {
      'target_name': 'addon',
      'sources': [ 'src/addon.cc' ],
      'cflags': [ '-std=c++0x' ],
      'ldflags': [ '-std=c++0x' ],
      'conditions': [
        ['OS=="mac"',
          {
            'cflags': [ '-stdlib=libc++' ],
            'xcode_settings': {
              'GCC_ENABLE_CPP_RTTI': '-frtti',
              'OTHER_CPLUSPLUSFLAGS': [ '-mmacosx-version-min=10.7', '-stdlib=libc++', '-std=c++0x' ],
              'OTHER_LDFLAGS': [ '-std=c++0x' ]
            }
          }
        ],
      ],
      "include_dirs" : [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
