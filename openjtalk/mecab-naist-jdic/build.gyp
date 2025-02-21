{
    'includes': ["../../common.gypi"],
    'targets': [
        {
            'target_name': 'mecab-naist-jdic',
            'type': "none",
            'dependencies': ["../mecab/build.gyp:mecab-dict-index"],
            'actions': [
                {
                    'action_name': 'compile-dictionary',
                    'inputs': [
                        'char.def',
                        'left-id.def',
                        'matrix.def',
                        '1naist-jdic.csv',
                        'pos-id.def',
                        'rewrite.def',
                        'right-id.def',
                        '2unidic-csj.csv',
                        'unk.def',
                        '3mecab-ipadic-neologd.csv',
                        '4bep-eng.csv',
                    ],
                    'outputs': [
                        'char.bin',
                        'matrix.bin',
                        'sys.dic',
                        'unk.dic'
                    ],
                    "conditions": [
                        ['OS == "win"', {
                            'action': ['<(PRODUCT_DIR)/mecab-dict-index<(EXECUTABLE_SUFFIX) -d <(RULE_INPUT_DIRNAME) -o <(RULE_INPUT_DIRNAME)'],
                        }],
                        ['OS != "win"', {
                            'action': ['<(PRODUCT_DIR)/mecab-dict-index<(EXECUTABLE_SUFFIX)'],
                        }],
                    ]

                },
            ]
        }
    ]
}
