{
    "variables": {
        "prefers_libcpp":"<!(python -c \"import os;import platform;u=platform.uname();print((u[0] == 'Darwin' and int(u[2][0:2]) >= 13) and '-stdlib=libstdc++' not in os.environ.get('CXXFLAGS','') and '-mmacosx-version-min' not in os.environ.get('CXXFLAGS',''))\")"
    },
    "targets": [
        {
            "target_name": "mp4duration",
            "sources": [
                "src/mp4duration.cc",
                "src/parse.cc"
            ],
            "cflags!": [
                "-fno-exceptions"
            ],
            "cflags_cc!": [
                "-fno-exceptions"
            ],
            "conditions": [
                [
                    "OS==\"mac\"",
                    {
                        "xcode_settings": {
                            "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
                        }
                    }
                ],
                [ '"<(prefers_libcpp)"=="True"', {
                    'xcode_settings': {
                        'MACOSX_DEPLOYMENT_TARGET':'10.9'
                    }
                }]
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")"
            ]
        }
    ]
}
