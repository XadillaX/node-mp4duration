{
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
                    "OS==\"mac\"", {
                        "xcode_settings": {
                            "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
                        }
                    }
                ]
            ]
        }
    ]
}
