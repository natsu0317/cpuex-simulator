const uint64_t sqrt_table[2048] = {
    0x3effe008
,    0x3f000ffd
,    0x3effa038
,    0x3f002ff5
,    0x3eff6097
,    0x3f004fe5
,    0x3eff2126
,    0x3f006fcd
,    0x3efee1e4
,    0x3f008fad
,    0x3efea2d2
,    0x3f00af86
,    0x3efe63ed
,    0x3f00cf56
,    0x3efe2538
,    0x3f00ef1f
,    0x3efde6b0
,    0x3f010edf
,    0x3efda857
,    0x3f012e98
,    0x3efd6a2b
,    0x3f014e4a
,    0x3efd2c2e
,    0x3f016df3
,    0x3efcee5d
,    0x3f018d95
,    0x3efcb0ba
,    0x3f01ad2f
,    0x3efc7344
,    0x3f01ccc1
,    0x3efc35fa
,    0x3f01ec4b
,    0x3efbf8dd
,    0x3f020bce
,    0x3efbbbed
,    0x3f022b4a
,    0x3efb7f28
,    0x3f024abd
,    0x3efb4290
,    0x3f026a29
,    0x3efb0623
,    0x3f02898e
,    0x3efac9e1
,    0x3f02a8eb
,    0x3efa8dcb
,    0x3f02c840
,    0x3efa51e0
,    0x3f02e78e
,    0x3efa1620
,    0x3f0306d5
,    0x3ef9da8b
,    0x3f032614
,    0x3ef99f20
,    0x3f03454c
,    0x3ef963e0
,    0x3f03647c
,    0x3ef928c9
,    0x3f0383a5
,    0x3ef8eddd
,    0x3f03a2c6
,    0x3ef8b31a
,    0x3f03c1e0
,    0x3ef87881
,    0x3f03e0f3
,    0x3ef83e11
,    0x3f03fffe
,    0x3ef803cb
,    0x3f041f02
,    0x3ef7c9ad
,    0x3f043dff
,    0x3ef78fb8
,    0x3f045cf5
,    0x3ef755ec
,    0x3f047be3
,    0x3ef71c48
,    0x3f049aca
,    0x3ef6e2cc
,    0x3f04b9aa
,    0x3ef6a978
,    0x3f04d883
,    0x3ef6704c
,    0x3f04f755
,    0x3ef63748
,    0x3f05161f
,    0x3ef5fe6c
,    0x3f0534e2
,    0x3ef5c5b6
,    0x3f05539f
,    0x3ef58d28
,    0x3f057254
,    0x3ef554c1
,    0x3f059102
,    0x3ef51c81
,    0x3f05afa9
,    0x3ef4e467
,    0x3f05ce49
,    0x3ef4ac74
,    0x3f05ece2
,    0x3ef474a7
,    0x3f060b74
,    0x3ef43d00
,    0x3f0629ff
,    0x3ef4057f
,    0x3f064883
,    0x3ef3ce23
,    0x3f066701
,    0x3ef396ee
,    0x3f068577
,    0x3ef35fde
,    0x3f06a3e6
,    0x3ef328f3
,    0x3f06c24f
,    0x3ef2f22d
,    0x3f06e0b1
,    0x3ef2bb8c
,    0x3f06ff0c
,    0x3ef28510
,    0x3f071d60
,    0x3ef24eb9
,    0x3f073bad
,    0x3ef21886
,    0x3f0759f3
,    0x3ef1e277
,    0x3f077833
,    0x3ef1ac8d
,    0x3f07966c
,    0x3ef176c6
,    0x3f07b49e
,    0x3ef14124
,    0x3f07d2ca
,    0x3ef10ba5
,    0x3f07f0ee
,    0x3ef0d649
,    0x3f080f0d
,    0x3ef0a111
,    0x3f082d24
,    0x3ef06bfc
,    0x3f084b35
,    0x3ef0370a
,    0x3f08693f
,    0x3ef0023b
,    0x3f088743
,    0x3eefcd8f
,    0x3f08a540
,    0x3eef9906
,    0x3f08c336
,    0x3eef649f
,    0x3f08e126
,    0x3eef305a
,    0x3f08ff0f
,    0x3eeefc38
,    0x3f091cf2
,    0x3eeec837
,    0x3f093ace
,    0x3eee9459
,    0x3f0958a4
,    0x3eee609c
,    0x3f097673
,    0x3eee2d00
,    0x3f09943c
,    0x3eedf987
,    0x3f09b1ff
,    0x3eedc62e
,    0x3f09cfbb
,    0x3eed92f7
,    0x3f09ed70
,    0x3eed5fe1
,    0x3f0a0b1f
,    0x3eed2ceb
,    0x3f0a28c8
,    0x3eecfa17
,    0x3f0a466b
,    0x3eecc763
,    0x3f0a6407
,    0x3eec94cf
,    0x3f0a819c
,    0x3eec625c
,    0x3f0a9f2c
,    0x3eec3009
,    0x3f0abcb5
,    0x3eebfdd6
,    0x3f0ada38
,    0x3eebcbc4
,    0x3f0af7b5
,    0x3eeb99d1
,    0x3f0b152b
,    0x3eeb67fd
,    0x3f0b329b
,    0x3eeb3649
,    0x3f0b5005
,    0x3eeb04b5
,    0x3f0b6d69
,    0x3eead340
,    0x3f0b8ac6
,    0x3eeaa1ea
,    0x3f0ba81d
,    0x3eea70b3
,    0x3f0bc56f
,    0x3eea3f9b
,    0x3f0be2ba
,    0x3eea0ea2
,    0x3f0bfffe
,    0x3ee9ddc8
,    0x3f0c1d3d
,    0x3ee9ad0c
,    0x3f0c3a76
,    0x3ee97c6e
,    0x3f0c57a9
,    0x3ee94bef
,    0x3f0c74d5
,    0x3ee91b8e
,    0x3f0c91fc
,    0x3ee8eb4b
,    0x3f0caf1c
,    0x3ee8bb26
,    0x3f0ccc36
,    0x3ee88b1f
,    0x3f0ce94b
,    0x3ee85b35
,    0x3f0d0659
,    0x3ee82b69
,    0x3f0d2362
,    0x3ee7fbbb
,    0x3f0d4064
,    0x3ee7cc29
,    0x3f0d5d60
,    0x3ee79cb5
,    0x3f0d7a57
,    0x3ee76d5e
,    0x3f0d9748
,    0x3ee73e25
,    0x3f0db432
,    0x3ee70f07
,    0x3f0dd117
,    0x3ee6e007
,    0x3f0dedf6
,    0x3ee6b123
,    0x3f0e0acf
,    0x3ee6825c
,    0x3f0e27a2
,    0x3ee653b2
,    0x3f0e4470
,    0x3ee62523
,    0x3f0e6137
,    0x3ee5f6b1
,    0x3f0e7df9
,    0x3ee5c85b
,    0x3f0e9ab5
,    0x3ee59a20
,    0x3f0eb76b
,    0x3ee56c02
,    0x3f0ed41c
,    0x3ee53dff
,    0x3f0ef0c6
,    0x3ee51018
,    0x3f0f0d6b
,    0x3ee4e24d
,    0x3f0f2a0a
,    0x3ee4b49d
,    0x3f0f46a4
,    0x3ee48708
,    0x3f0f6337
,    0x3ee4598f
,    0x3f0f7fc5
,    0x3ee42c30
,    0x3f0f9c4e
,    0x3ee3feed
,    0x3f0fb8d1
,    0x3ee3d1c5
,    0x3f0fd54e
,    0x3ee3a4b7
,    0x3f0ff1c5
,    0x3ee377c4
,    0x3f100e37
,    0x3ee34aeb
,    0x3f102aa3
,    0x3ee31e2d
,    0x3f10470a
,    0x3ee2f18a
,    0x3f10636b
,    0x3ee2c501
,    0x3f107fc6
,    0x3ee29891
,    0x3f109c1c
,    0x3ee26c3c
,    0x3f10b86c
,    0x3ee24001
,    0x3f10d4b7
,    0x3ee213e0
,    0x3f10f0fc
,    0x3ee1e7d9
,    0x3f110d3c
,    0x3ee1bbeb
,    0x3f112976
,    0x3ee19017
,    0x3f1145ab
,    0x3ee1645c
,    0x3f1161da
,    0x3ee138bb
,    0x3f117e04
,    0x3ee10d33
,    0x3f119a28
,    0x3ee0e1c5
,    0x3f11b647
,    0x3ee0b66f
,    0x3f11d261
,    0x3ee08b32
,    0x3f11ee75
,    0x3ee0600f
,    0x3f120a84
,    0x3ee03504
,    0x3f12268d
,    0x3ee00a12
,    0x3f124291
,    0x3edfdf38
,    0x3f125e8f
,    0x3edfb477
,    0x3f127a89
,    0x3edf89cf
,    0x3f12967d
,    0x3edf5f3f
,    0x3f12b26b
,    0x3edf34c7
,    0x3f12ce54
,    0x3edf0a67
,    0x3f12ea38
,    0x3edee020
,    0x3f130617
,    0x3edeb5f0
,    0x3f1321f0
,    0x3ede8bd9
,    0x3f133dc4
,    0x3ede61d9
,    0x3f135993
,    0x3ede37f1
,    0x3f13755d
,    0x3ede0e20
,    0x3f139121
,    0x3edde468
,    0x3f13ace0
,    0x3eddbac6
,    0x3f13c89a
,    0x3edd913c
,    0x3f13e44f
,    0x3edd67ca
,    0x3f13ffff
,    0x3edd3e6e
,    0x3f141ba9
,    0x3edd152a
,    0x3f14374e
,    0x3edcebfd
,    0x3f1452ee
,    0x3edcc2e7
,    0x3f146e89
,    0x3edc99e7
,    0x3f148a1f
,    0x3edc70ff
,    0x3f14a5b0
,    0x3edc482d
,    0x3f14c13b
,    0x3edc1f72
,    0x3f14dcc2
,    0x3edbf6cd
,    0x3f14f843
,    0x3edbce3f
,    0x3f1513c0
,    0x3edba5c7
,    0x3f152f37
,    0x3edb7d66
,    0x3f154aa9
,    0x3edb551b
,    0x3f156616
,    0x3edb2ce6
,    0x3f15817e
,    0x3edb04c7
,    0x3f159ce1
,    0x3edadcbe
,    0x3f15b840
,    0x3edab4cb
,    0x3f15d399
,    0x3eda8cee
,    0x3f15eeed
,    0x3eda6526
,    0x3f160a3c
,    0x3eda3d75
,    0x3f162586
,    0x3eda15d9
,    0x3f1640cb
,    0x3ed9ee52
,    0x3f165c0b
,    0x3ed9c6e1
,    0x3f167747
,    0x3ed99f85
,    0x3f16927d
,    0x3ed9783f
,    0x3f16adaf
,    0x3ed9510e
,    0x3f16c8db
,    0x3ed929f2
,    0x3f16e403
,    0x3ed902eb
,    0x3f16ff26
,    0x3ed8dbf9
,    0x3f171a44
,    0x3ed8b51c
,    0x3f17355d
,    0x3ed88e54
,    0x3f175071
,    0x3ed867a0
,    0x3f176b80
,    0x3ed84102
,    0x3f17868b
,    0x3ed81a78
,    0x3f17a191
,    0x3ed7f403
,    0x3f17bc92
,    0x3ed7cda2
,    0x3f17d78e
,    0x3ed7a755
,    0x3f17f285
,    0x3ed7811d
,    0x3f180d77
,    0x3ed75af9
,    0x3f182865
,    0x3ed734ea
,    0x3f18434e
,    0x3ed70eef
,    0x3f185e32
,    0x3ed6e907
,    0x3f187912
,    0x3ed6c334
,    0x3f1893ed
,    0x3ed69d75
,    0x3f18aec3
,    0x3ed677c9
,    0x3f18c994
,    0x3ed65231
,    0x3f18e461
,    0x3ed62cad
,    0x3f18ff29
,    0x3ed6073d
,    0x3f1919ec
,    0x3ed5e1e1
,    0x3f1934aa
,    0x3ed5bc97
,    0x3f194f64
,    0x3ed59762
,    0x3f196a1a
,    0x3ed57240
,    0x3f1984ca
,    0x3ed54d31
,    0x3f199f76
,    0x3ed52835
,    0x3f19ba1e
,    0x3ed5034d
,    0x3f19d4c0
,    0x3ed4de77
,    0x3f19ef5e
,    0x3ed4b9b5
,    0x3f1a09f8
,    0x3ed49506
,    0x3f1a248d
,    0x3ed4706a
,    0x3f1a3f1d
,    0x3ed44be1
,    0x3f1a59a9
,    0x3ed4276a
,    0x3f1a7430
,    0x3ed40306
,    0x3f1a8eb3
,    0x3ed3deb5
,    0x3f1aa931
,    0x3ed3ba77
,    0x3f1ac3aa
,    0x3ed3964b
,    0x3f1ade20
,    0x3ed37232
,    0x3f1af890
,    0x3ed34e2b
,    0x3f1b12fc
,    0x3ed32a36
,    0x3f1b2d64
,    0x3ed30654
,    0x3f1b47c7
,    0x3ed2e284
,    0x3f1b6225
,    0x3ed2bec7
,    0x3f1b7c7f
,    0x3ed29b1b
,    0x3f1b96d5
,    0x3ed27782
,    0x3f1bb126
,    0x3ed253fa
,    0x3f1bcb73
,    0x3ed23085
,    0x3f1be5bb
,    0x3ed20d22
,    0x3f1bffff
,    0x3ed1e9d0
,    0x3f1c1a3e
,    0x3ed1c690
,    0x3f1c3479
,    0x3ed1a362
,    0x3f1c4eb0
,    0x3ed18045
,    0x3f1c68e2
,    0x3ed15d3b
,    0x3f1c8310
,    0x3ed13a41
,    0x3f1c9d3a
,    0x3ed1175a
,    0x3f1cb75f
,    0x3ed0f483
,    0x3f1cd17f
,    0x3ed0d1be
,    0x3f1ceb9c
,    0x3ed0af0b
,    0x3f1d05b4
,    0x3ed08c68
,    0x3f1d1fc8
,    0x3ed069d7
,    0x3f1d39d7
,    0x3ed04757
,    0x3f1d53e2
,    0x3ed024e9
,    0x3f1d6de9
,    0x3ed0028b
,    0x3f1d87eb
,    0x3ecfe03e
,    0x3f1da1e9
,    0x3ecfbe02
,    0x3f1dbbe3
,    0x3ecf9bd7
,    0x3f1dd5d9
,    0x3ecf79bd
,    0x3f1defca
,    0x3ecf57b4
,    0x3f1e09b7
,    0x3ecf35bb
,    0x3f1e23a0
,    0x3ecf13d3
,    0x3f1e3d85
,    0x3ecef1fc
,    0x3f1e5765
,    0x3eced035
,    0x3f1e7141
,    0x3eceae7f
,    0x3f1e8b19
,    0x3ece8cd9
,    0x3f1ea4ed
,    0x3ece6b44
,    0x3f1ebebc
,    0x3ece49bf
,    0x3f1ed888
,    0x3ece284a
,    0x3f1ef24f
,    0x3ece06e6
,    0x3f1f0c12
,    0x3ecde592
,    0x3f1f25d1
,    0x3ecdc44e
,    0x3f1f3f8b
,    0x3ecda31a
,    0x3f1f5942
,    0x3ecd81f6
,    0x3f1f72f4
,    0x3ecd60e2
,    0x3f1f8ca2
,    0x3ecd3fde
,    0x3f1fa64c
,    0x3ecd1eea
,    0x3f1fbff2
,    0x3eccfe06
,    0x3f1fd994
,    0x3eccdd32
,    0x3f1ff332
,    0x3eccbc6d
,    0x3f200ccb
,    0x3ecc9bb8
,    0x3f202661
,    0x3ecc7b13
,    0x3f203ff2
,    0x3ecc5a7d
,    0x3f205980
,    0x3ecc39f7
,    0x3f207309
,    0x3ecc1981
,    0x3f208c8e
,    0x3ecbf91a
,    0x3f20a60f
,    0x3ecbd8c2
,    0x3f20bf8c
,    0x3ecbb879
,    0x3f20d905
,    0x3ecb9840
,    0x3f20f27a
,    0x3ecb7817
,    0x3f210beb
,    0x3ecb57fc
,    0x3f212558
,    0x3ecb37f1
,    0x3f213ec1
,    0x3ecb17f5
,    0x3f215826
,    0x3ecaf807
,    0x3f217187
,    0x3ecad829
,    0x3f218ae4
,    0x3ecab85a
,    0x3f21a43e
,    0x3eca989a
,    0x3f21bd93
,    0x3eca78e9
,    0x3f21d6e4
,    0x3eca5946
,    0x3f21f031
,    0x3eca39b3
,    0x3f22097a
,    0x3eca1a2e
,    0x3f2222bf
,    0x3ec9fab8
,    0x3f223c01
,    0x3ec9db51
,    0x3f22553e
,    0x3ec9bbf8
,    0x3f226e77
,    0x3ec99cae
,    0x3f2287ad
,    0x3ec97d72
,    0x3f22a0df
,    0x3ec95e45
,    0x3f22ba0c
,    0x3ec93f26
,    0x3f22d336
,    0x3ec92016
,    0x3f22ec5c
,    0x3ec90114
,    0x3f23057e
,    0x3ec8e220
,    0x3f231e9c
,    0x3ec8c33b
,    0x3f2337b7
,    0x3ec8a464
,    0x3f2350cd
,    0x3ec8859b
,    0x3f2369e0
,    0x3ec866e0
,    0x3f2382ef
,    0x3ec84833
,    0x3f239bfa
,    0x3ec82995
,    0x3f23b501
,    0x3ec80b04
,    0x3f23ce04
,    0x3ec7ec82
,    0x3f23e703
,    0x3ec7ce0d
,    0x3f23ffff
,    0x3ec7afa6
,    0x3f2418f7
,    0x3ec7914d
,    0x3f2431eb
,    0x3ec77302
,    0x3f244adb
,    0x3ec754c5
,    0x3f2463c8
,    0x3ec73696
,    0x3f247cb0
,    0x3ec71874
,    0x3f249595
,    0x3ec6fa60
,    0x3f24ae77
,    0x3ec6dc59
,    0x3f24c754
,    0x3ec6be60
,    0x3f24e02e
,    0x3ec6a075
,    0x3f24f904
,    0x3ec68297
,    0x3f2511d6
,    0x3ec664c6
,    0x3f252aa4
,    0x3ec64703
,    0x3f25436f
,    0x3ec6294e
,    0x3f255c36
,    0x3ec60ba5
,    0x3f2574f9
,    0x3ec5ee0a
,    0x3f258db9
,    0x3ec5d07c
,    0x3f25a675
,    0x3ec5b2fc
,    0x3f25bf2d
,    0x3ec59588
,    0x3f25d7e2
,    0x3ec57822
,    0x3f25f092
,    0x3ec55ac9
,    0x3f260940
,    0x3ec53d7d
,    0x3f2621e9
,    0x3ec5203e
,    0x3f263a8f
,    0x3ec5030c
,    0x3f265331
,    0x3ec4e5e7
,    0x3f266bd0
,    0x3ec4c8cf
,    0x3f26846b
,    0x3ec4abc4
,    0x3f269d02
,    0x3ec48ec5
,    0x3f26b596
,    0x3ec471d4
,    0x3f26ce26
,    0x3ec454ef
,    0x3f26e6b2
,    0x3ec43817
,    0x3f26ff3b
,    0x3ec41b4b
,    0x3f2717c0
,    0x3ec3fe8c
,    0x3f273042
,    0x3ec3e1da
,    0x3f2748c0
,    0x3ec3c535
,    0x3f27613a
,    0x3ec3a89c
,    0x3f2779b1
,    0x3ec38c0f
,    0x3f279224
,    0x3ec36f8f
,    0x3f27aa94
,    0x3ec3531c
,    0x3f27c300
,    0x3ec336b5
,    0x3f27db69
,    0x3ec31a5a
,    0x3f27f3ce
,    0x3ec2fe0c
,    0x3f280c2f
,    0x3ec2e1ca
,    0x3f28248d
,    0x3ec2c594
,    0x3f283ce8
,    0x3ec2a96a
,    0x3f28553f
,    0x3ec28d4d
,    0x3f286d92
,    0x3ec2713c
,    0x3f2885e2
,    0x3ec25536
,    0x3f289e2f
,    0x3ec2393d
,    0x3f28b677
,    0x3ec21d51
,    0x3f28cebd
,    0x3ec20170
,    0x3f28e6ff
,    0x3ec1e59b
,    0x3f28ff3d
,    0x3ec1c9d2
,    0x3f291778
,    0x3ec1ae15
,    0x3f292fb0
,    0x3ec19264
,    0x3f2947e4
,    0x3ec176bf
,    0x3f296014
,    0x3ec15b25
,    0x3f297841
,    0x3ec13f98
,    0x3f29906b
,    0x3ec12416
,    0x3f29a891
,    0x3ec108a0
,    0x3f29c0b4
,    0x3ec0ed35
,    0x3f29d8d3
,    0x3ec0d1d7
,    0x3f29f0ef
,    0x3ec0b683
,    0x3f2a0908
,    0x3ec09b3c
,    0x3f2a211d
,    0x3ec08000
,    0x3f2a392f
,    0x3ec064d0
,    0x3f2a513d
,    0x3ec049ab
,    0x3f2a6948
,    0x3ec02e91
,    0x3f2a814f
,    0x3ec01383
,    0x3f2a9954
,    0x3ebff881
,    0x3f2ab154
,    0x3ebfdd8a
,    0x3f2ac952
,    0x3ebfc29e
,    0x3f2ae14c
,    0x3ebfa7bd
,    0x3f2af942
,    0x3ebf8ce8
,    0x3f2b1136
,    0x3ebf721e
,    0x3f2b2926
,    0x3ebf5760
,    0x3f2b4112
,    0x3ebf3cac
,    0x3f2b58fb
,    0x3ebf2204
,    0x3f2b70e1
,    0x3ebf0767
,    0x3f2b88c4
,    0x3ebeecd4
,    0x3f2ba0a3
,    0x3ebed24d
,    0x3f2bb87f
,    0x3ebeb7d1
,    0x3f2bd058
,    0x3ebe9d60
,    0x3f2be82d
,    0x3ebe82fb
,    0x3f2bffff
,    0x3ebe689f
,    0x3f2c17ce
,    0x3ebe4e4f
,    0x3f2c2f99
,    0x3ebe340a
,    0x3f2c4761
,    0x3ebe19d0
,    0x3f2c5f26
,    0x3ebdffa0
,    0x3f2c76e8
,    0x3ebde57c
,    0x3f2c8ea6
,    0x3ebdcb62
,    0x3f2ca661
,    0x3ebdb153
,    0x3f2cbe19
,    0x3ebd974e
,    0x3f2cd5ce
,    0x3ebd7d54
,    0x3f2ced7f
,    0x3ebd6365
,    0x3f2d052d
,    0x3ebd4981
,    0x3f2d1cd8
,    0x3ebd2fa7
,    0x3f2d347f
,    0x3ebd15d8
,    0x3f2d4c24
,    0x3ebcfc13
,    0x3f2d63c5
,    0x3ebce259
,    0x3f2d7b63
,    0x3ebcc8a9
,    0x3f2d92fd
,    0x3ebcaf04
,    0x3f2daa95
,    0x3ebc9569
,    0x3f2dc229
,    0x3ebc7bd9
,    0x3f2dd9ba
,    0x3ebc6253
,    0x3f2df148
,    0x3ebc48d7
,    0x3f2e08d3
,    0x3ebc2f66
,    0x3f2e205a
,    0x3ebc15ff
,    0x3f2e37df
,    0x3ebbfca2
,    0x3f2e4f60
,    0x3ebbe350
,    0x3f2e66de
,    0x3ebbca08
,    0x3f2e7e59
,    0x3ebbb0ca
,    0x3f2e95d0
,    0x3ebb9796
,    0x3f2ead45
,    0x3ebb7e6c
,    0x3f2ec4b6
,    0x3ebb654c
,    0x3f2edc25
,    0x3ebb4c37
,    0x3f2ef390
,    0x3ebb332b
,    0x3f2f0af8
,    0x3ebb1a2a
,    0x3f2f225c
,    0x3ebb0133
,    0x3f2f39be
,    0x3ebae845
,    0x3f2f511d
,    0x3ebacf62
,    0x3f2f6878
,    0x3ebab688
,    0x3f2f7fd1
,    0x3eba9db9
,    0x3f2f9726
,    0x3eba84f3
,    0x3f2fae78
,    0x3eba6c37
,    0x3f2fc5c7
,    0x3eba5385
,    0x3f2fdd13
,    0x3eba3add
,    0x3f2ff45c
,    0x3eba223e
,    0x3f300ba2
,    0x3eba09a9
,    0x3f3022e5
,    0x3eb9f11e
,    0x3f303a24
,    0x3eb9d89d
,    0x3f305161
,    0x3eb9c025
,    0x3f30689a
,    0x3eb9a7b7
,    0x3f307fd1
,    0x3eb98f53
,    0x3f309704
,    0x3eb976f8
,    0x3f30ae35
,    0x3eb95ea7
,    0x3f30c562
,    0x3eb94660
,    0x3f30dc8c
,    0x3eb92e21
,    0x3f30f3b4
,    0x3eb915ed
,    0x3f310ad8
,    0x3eb8fdc2
,    0x3f3121f9
,    0x3eb8e5a0
,    0x3f313917
,    0x3eb8cd88
,    0x3f315033
,    0x3eb8b579
,    0x3f31674b
,    0x3eb89d74
,    0x3f317e60
,    0x3eb88578
,    0x3f319572
,    0x3eb86d85
,    0x3f31ac81
,    0x3eb8559c
,    0x3f31c38d
,    0x3eb83dbc
,    0x3f31da97
,    0x3eb825e5
,    0x3f31f19d
,    0x3eb80e17
,    0x3f3208a0
,    0x3eb7f653
,    0x3f321fa0
,    0x3eb7de98
,    0x3f32369e
,    0x3eb7c6e6
,    0x3f324d98
,    0x3eb7af3d
,    0x3f32648f
,    0x3eb7979d
,    0x3f327b84
,    0x3eb78007
,    0x3f329275
,    0x3eb76879
,    0x3f32a964
,    0x3eb750f5
,    0x3f32c04f
,    0x3eb73979
,    0x3f32d738
,    0x3eb72207
,    0x3f32ee1e
,    0x3eb70a9d
,    0x3f330501
,    0x3eb6f33d
,    0x3f331be0
,    0x3eb6dbe6
,    0x3f3332bd
,    0x3eb6c497
,    0x3f334997
,    0x3eb6ad51
,    0x3f33606f
,    0x3eb69614
,    0x3f337743
,    0x3eb67ee1
,    0x3f338e14
,    0x3eb667b5
,    0x3f33a4e3
,    0x3eb65093
,    0x3f33bbae
,    0x3eb6397a
,    0x3f33d277
,    0x3eb62269
,    0x3f33e93c
,    0x3eb60b61
,    0x3f33ffff
,    0x3eb5f462
,    0x3f3416bf
,    0x3eb5dd6b
,    0x3f342d7c
,    0x3eb5c67d
,    0x3f344437
,    0x3eb5af98
,    0x3f345aee
,    0x3eb598bc
,    0x3f3471a3
,    0x3eb581e8
,    0x3f348854
,    0x3eb56b1d
,    0x3f349f03
,    0x3eb5545a
,    0x3f34b5af
,    0x3eb53da0
,    0x3f34cc58
,    0x3eb526ee
,    0x3f34e2fe
,    0x3eb51045
,    0x3f34f9a2
,    0x3eb4ee58
,    0x3f351b90
,    0x3eb4c139
,    0x3f3548c6
,    0x3eb4943b
,    0x3f3575f0
,    0x3eb4675f
,    0x3f35a310
,    0x3eb43aa4
,    0x3f35d024
,    0x3eb40e0b
,    0x3f35fd2d
,    0x3eb3e192
,    0x3f362a2b
,    0x3eb3b53a
,    0x3f36571e
,    0x3eb38903
,    0x3f368406
,    0x3eb35ced
,    0x3f36b0e2
,    0x3eb330f7
,    0x3f36ddb4
,    0x3eb30521
,    0x3f370a7b
,    0x3eb2d96c
,    0x3f373737
,    0x3eb2add6
,    0x3f3763e8
,    0x3eb28260
,    0x3f37908e
,    0x3eb2570a
,    0x3f37bd29
,    0x3eb22bd4
,    0x3f37e9b9
,    0x3eb200bc
,    0x3f38163f
,    0x3eb1d5c4
,    0x3f3842ba
,    0x3eb1aaeb
,    0x3f386f2a
,    0x3eb18031
,    0x3f389b8f
,    0x3eb15596
,    0x3f38c7ea
,    0x3eb12b19
,    0x3f38f43a
,    0x3eb100bb
,    0x3f39207f
,    0x3eb0d67b
,    0x3f394cba
,    0x3eb0ac59
,    0x3f3978eb
,    0x3eb08255
,    0x3f39a510
,    0x3eb05870
,    0x3f39d12c
,    0x3eb02ea8
,    0x3f39fd3d
,    0x3eb004fd
,    0x3f3a2943
,    0x3eafdb71
,    0x3f3a553f
,    0x3eafb201
,    0x3f3a8131
,    0x3eaf88af
,    0x3f3aad18
,    0x3eaf5f7a
,    0x3f3ad8f5
,    0x3eaf3662
,    0x3f3b04c8
,    0x3eaf0d66
,    0x3f3b3090
,    0x3eaee488
,    0x3f3b5c4f
,    0x3eaebbc6
,    0x3f3b8803
,    0x3eae9320
,    0x3f3bb3ad
,    0x3eae6a97
,    0x3f3bdf4c
,    0x3eae422a
,    0x3f3c0ae2
,    0x3eae19d9
,    0x3f3c366d
,    0x3eadf1a4
,    0x3f3c61ef
,    0x3eadc98b
,    0x3f3c8d66
,    0x3eada18d
,    0x3f3cb8d4
,    0x3ead79ab
,    0x3f3ce437
,    0x3ead51e4
,    0x3f3d0f91
,    0x3ead2a39
,    0x3f3d3ae0
,    0x3ead02a9
,    0x3f3d6626
,    0x3eacdb34
,    0x3f3d9161
,    0x3eacb3da
,    0x3f3dbc93
,    0x3eac8c9a
,    0x3f3de7bb
,    0x3eac6576
,    0x3f3e12da
,    0x3eac3e6c
,    0x3f3e3dee
,    0x3eac177c
,    0x3f3e68f9
,    0x3eabf0a7
,    0x3f3e93fa
,    0x3eabc9ec
,    0x3f3ebef1
,    0x3eaba34c
,    0x3f3ee9df
,    0x3eab7cc5
,    0x3f3f14c3
,    0x3eab5658
,    0x3f3f3f9d
,    0x3eab3005
,    0x3f3f6a6e
,    0x3eab09cc
,    0x3f3f9535
,    0x3eaae3ac
,    0x3f3fbff3
,    0x3eaabda5
,    0x3f3feaa7
,    0x3eaa97b8
,    0x3f401552
,    0x3eaa71e5
,    0x3f403ff3
,    0x3eaa4c2a
,    0x3f406a8b
,    0x3eaa2688
,    0x3f409519
,    0x3eaa00ff
,    0x3f40bf9e
,    0x3ea9db8f
,    0x3f40ea1a
,    0x3ea9b638
,    0x3f41148c
,    0x3ea990f9
,    0x3f413ef5
,    0x3ea96bd3
,    0x3f416954
,    0x3ea946c5
,    0x3f4193ab
,    0x3ea921d0
,    0x3f41bdf8
,    0x3ea8fcf2
,    0x3f41e83c
,    0x3ea8d82d
,    0x3f421276
,    0x3ea8b380
,    0x3f423ca8
,    0x3ea88eea
,    0x3f4266d0
,    0x3ea86a6c
,    0x3f4290ef
,    0x3ea84606
,    0x3f42bb05
,    0x3ea821b8
,    0x3f42e512
,    0x3ea7fd81
,    0x3f430f16
,    0x3ea7d961
,    0x3f433911
,    0x3ea7b558
,    0x3f436303
,    0x3ea79167
,    0x3f438cec
,    0x3ea76d8d
,    0x3f43b6cc
,    0x3ea749ca
,    0x3f43e0a2
,    0x3ea7261d
,    0x3f440a70
,    0x3ea70288
,    0x3f443436
,    0x3ea6df09
,    0x3f445df2
,    0x3ea6bba0
,    0x3f4487a5
,    0x3ea6984f
,    0x3f44b150
,    0x3ea67513
,    0x3f44daf1
,    0x3ea651ee
,    0x3f45048a
,    0x3ea62edf
,    0x3f452e1a
,    0x3ea60be7
,    0x3f4557a2
,    0x3ea5e904
,    0x3f458120
,    0x3ea5c637
,    0x3f45aa96
,    0x3ea5a380
,    0x3f45d403
,    0x3ea580df
,    0x3f45fd68
,    0x3ea55e54
,    0x3f4626c4
,    0x3ea53bde
,    0x3f465017
,    0x3ea5197e
,    0x3f467962
,    0x3ea4f733
,    0x3f46a2a4
,    0x3ea4d4fd
,    0x3f46cbdd
,    0x3ea4b2dd
,    0x3f46f50e
,    0x3ea490d2
,    0x3f471e37
,    0x3ea46edc
,    0x3f474757
,    0x3ea44cfb
,    0x3f47706e
,    0x3ea42b2f
,    0x3f47997d
,    0x3ea40977
,    0x3f47c284
,    0x3ea3e7d5
,    0x3f47eb82
,    0x3ea3c647
,    0x3f481478
,    0x3ea3a4cd
,    0x3f483d65
,    0x3ea38368
,    0x3f48664a
,    0x3ea36218
,    0x3f488f27
,    0x3ea340dc
,    0x3f48b7fb
,    0x3ea31fb4
,    0x3f48e0c7
,    0x3ea2fea0
,    0x3f49098b
,    0x3ea2dda1
,    0x3f493247
,    0x3ea2bcb5
,    0x3f495afa
,    0x3ea29bdd
,    0x3f4983a5
,    0x3ea27b1a
,    0x3f49ac48
,    0x3ea25a69
,    0x3f49d4e3
,    0x3ea239cd
,    0x3f49fd75
,    0x3ea21944
,    0x3f4a25ff
,    0x3ea1f8cf
,    0x3f4a4e82
,    0x3ea1d86d
,    0x3f4a76fc
,    0x3ea1b81f
,    0x3f4a9f6e
,    0x3ea197e4
,    0x3f4ac7d8
,    0x3ea177bc
,    0x3f4af03a
,    0x3ea157a8
,    0x3f4b1894
,    0x3ea137a6
,    0x3f4b40e6
,    0x3ea117b8
,    0x3f4b6930
,    0x3ea0f7dc
,    0x3f4b9172
,    0x3ea0d813
,    0x3f4bb9ac
,    0x3ea0b85e
,    0x3f4be1de
,    0x3ea098ba
,    0x3f4c0a08
,    0x3ea0792a
,    0x3f4c322a
,    0x3ea059ac
,    0x3f4c5a44
,    0x3ea03a41
,    0x3f4c8257
,    0x3ea01ae8
,    0x3f4caa62
,    0x3e9ffba1
,    0x3f4cd264
,    0x3e9fdc6d
,    0x3f4cfa5f
,    0x3e9fbd4b
,    0x3f4d2253
,    0x3e9f9e3b
,    0x3f4d4a3e
,    0x3e9f7f3d
,    0x3f4d7222
,    0x3e9f6051
,    0x3f4d99fe
,    0x3e9f4177
,    0x3f4dc1d2
,    0x3e9f22af
,    0x3f4de99e
,    0x3e9f03f9
,    0x3f4e1163
,    0x3e9ee555
,    0x3f4e3920
,    0x3e9ec6c2
,    0x3f4e60d6
,    0x3e9ea841
,    0x3f4e8884
,    0x3e9e89d2
,    0x3f4eb02a
,    0x3e9e6b74
,    0x3f4ed7c9
,    0x3e9e4d27
,    0x3f4eff60
,    0x3e9e2eec
,    0x3f4f26ef
,    0x3e9e10c2
,    0x3f4f4e77
,    0x3e9df2a9
,    0x3f4f75f8
,    0x3e9dd4a1
,    0x3f4f9d71
,    0x3e9db6ab
,    0x3f4fc4e2
,    0x3e9d98c6
,    0x3f4fec4c
,    0x3e9d7af1
,    0x3f5013ae
,    0x3e9d5d2e
,    0x3f503b09
,    0x3e9d3f7b
,    0x3f50625d
,    0x3e9d21d9
,    0x3f5089a9
,    0x3e9d0448
,    0x3f50b0ee
,    0x3e9ce6c8
,    0x3f50d82b
,    0x3e9cc958
,    0x3f50ff61
,    0x3e9cabf8
,    0x3f512690
,    0x3e9c8eaa
,    0x3f514db7
,    0x3e9c716b
,    0x3f5174d7
,    0x3e9c543d
,    0x3f519bf0
,    0x3e9c371f
,    0x3f51c302
,    0x3e9c1a12
,    0x3f51ea0c
,    0x3e9bfd14
,    0x3f52110f
,    0x3e9be027
,    0x3f52380a
,    0x3e9bc34a
,    0x3f525eff
,    0x3e9ba67d
,    0x3f5285ec
,    0x3e9b89c0
,    0x3f52acd2
,    0x3e9b6d13
,    0x3f52d3b1
,    0x3e9b5075
,    0x3f52fa88
,    0x3e9b33e8
,    0x3f532159
,    0x3e9b176a
,    0x3f534822
,    0x3e9afafb
,    0x3f536ee5
,    0x3e9ade9d
,    0x3f5395a0
,    0x3e9ac24e
,    0x3f53bc54
,    0x3e9aa60e
,    0x3f53e301
,    0x3e9a89de
,    0x3f5409a7
,    0x3e9a6dbd
,    0x3f543046
,    0x3e9a51ac
,    0x3f5456de
,    0x3e9a35aa
,    0x3f547d6f
,    0x3e9a19b7
,    0x3f54a3f9
,    0x3e99fdd3
,    0x3f54ca7c
,    0x3e99e1fe
,    0x3f54f0f8
,    0x3e99c639
,    0x3f55176d
,    0x3e99aa82
,    0x3f553ddb
,    0x3e998edb
,    0x3f556442
,    0x3e997342
,    0x3f558aa2
,    0x3e9957b8
,    0x3f55b0fc
,    0x3e993c3d
,    0x3f55d74e
,    0x3e9920d1
,    0x3f55fd9a
,    0x3e990574
,    0x3f5623df
,    0x3e98ea25
,    0x3f564a1d
,    0x3e98cee5
,    0x3f567054
,    0x3e98b3b3
,    0x3f569684
,    0x3e989890
,    0x3f56bcae
,    0x3e987d7b
,    0x3f56e2d0
,    0x3e986274
,    0x3f5708ec
,    0x3e98477c
,    0x3f572f02
,    0x3e982c93
,    0x3f575510
,    0x3e9811b7
,    0x3f577b18
,    0x3e97f6ea
,    0x3f57a119
,    0x3e97dc2b
,    0x3f57c713
,    0x3e97c17a
,    0x3f57ed07
,    0x3e97a6d7
,    0x3f5812f4
,    0x3e978c42
,    0x3f5838da
,    0x3e9771bb
,    0x3f585eba
,    0x3e975742
,    0x3f588493
,    0x3e973cd6
,    0x3f58aa66
,    0x3e972279
,    0x3f58d032
,    0x3e970829
,    0x3f58f5f7
,    0x3e96ede8
,    0x3f591bb6
,    0x3e96d3b3
,    0x3f59416e
,    0x3e96b98d
,    0x3f596720
,    0x3e969f74
,    0x3f598ccb
,    0x3e968568
,    0x3f59b26f
,    0x3e966b6a
,    0x3f59d80e
,    0x3e96517a
,    0x3f59fda5
,    0x3e963796
,    0x3f5a2336
,    0x3e961dc1
,    0x3f5a48c1
,    0x3e9603f8
,    0x3f5a6e45
,    0x3e95ea3d
,    0x3f5a93c3
,    0x3e95d08f
,    0x3f5ab93a
,    0x3e95b6ee
,    0x3f5adeab
,    0x3e959d5a
,    0x3f5b0416
,    0x3e9583d4
,    0x3f5b297a
,    0x3e956a5a
,    0x3f5b4ed8
,    0x3e9550ee
,    0x3f5b742f
,    0x3e95378e
,    0x3f5b9980
,    0x3e951e3c
,    0x3f5bbecb
,    0x3e9504f6
,    0x3f5be40f
,    0x3e94ebbd
,    0x3f5c094d
,    0x3e94d291
,    0x3f5c2e85
,    0x3e94b971
,    0x3f5c53b7
,    0x3e94a05f
,    0x3f5c78e2
,    0x3e948758
,    0x3f5c9e07
,    0x3e946e5f
,    0x3f5cc326
,    0x3e945572
,    0x3f5ce83e
,    0x3e943c92
,    0x3f5d0d50
,    0x3e9423be
,    0x3f5d325c
,    0x3e940af7
,    0x3f5d5762
,    0x3e93f23c
,    0x3f5d7c62
,    0x3e93d98d
,    0x3f5da15b
,    0x3e93c0eb
,    0x3f5dc64f
,    0x3e93a855
,    0x3f5deb3c
,    0x3e938fcc
,    0x3f5e1023
,    0x3e93774e
,    0x3f5e3504
,    0x3e935edd
,    0x3f5e59de
,    0x3e934678
,    0x3f5e7eb3
,    0x3e932e1f
,    0x3f5ea382
,    0x3e9315d2
,    0x3f5ec84a
,    0x3e92fd91
,    0x3f5eed0d
,    0x3e92e55c
,    0x3f5f11c9
,    0x3e92cd33
,    0x3f5f367f
,    0x3e92b516
,    0x3f5f5b30
,    0x3e929d04
,    0x3f5f7fda
,    0x3e9284ff
,    0x3f5fa47e
,    0x3e926d05
,    0x3f5fc91c
,    0x3e925518
,    0x3f5fedb5
,    0x3e923d35
,    0x3f601247
,    0x3e92255f
,    0x3f6036d3
,    0x3e920d94
,    0x3f605b5a
,    0x3e91f5d5
,    0x3f607fda
,    0x3e91de21
,    0x3f60a455
,    0x3e91c679
,    0x3f60c8c9
,    0x3e91aedd
,    0x3f60ed38
,    0x3e91974b
,    0x3f6111a1
,    0x3e917fc6
,    0x3f613603
,    0x3e91684b
,    0x3f615a60
,    0x3e9150dc
,    0x3f617eb8
,    0x3e913979
,    0x3f61a309
,    0x3e912220
,    0x3f61c754
,    0x3e910ad3
,    0x3f61eb9a
,    0x3e90f391
,    0x3f620fda
,    0x3e90dc5a
,    0x3f623414
,    0x3e90c52f
,    0x3f625848
,    0x3e90ae0e
,    0x3f627c76
,    0x3e9096f9
,    0x3f62a09f
,    0x3e907fee
,    0x3f62c4c2
,    0x3e9068ef
,    0x3f62e8df
,    0x3e9051fa
,    0x3f630cf6
,    0x3e903b11
,    0x3f633108
,    0x3e902432
,    0x3f635514
,    0x3e900d5e
,    0x3f63791a
,    0x3e8ff695
,    0x3f639d1b
,    0x3e8fdfd7
,    0x3f63c115
,    0x3e8fc924
,    0x3f63e50a
,    0x3e8fb27b
,    0x3f6408fa
,    0x3e8f9bdd
,    0x3f642ce4
,    0x3e8f854a
,    0x3f6450c8
,    0x3e8f6ec1
,    0x3f6474a6
,    0x3e8f5843
,    0x3f64987f
,    0x3e8f41d0
,    0x3f64bc53
,    0x3e8f2b67
,    0x3f64e020
,    0x3e8f1508
,    0x3f6503e8
,    0x3e8efeb5
,    0x3f6527ab
,    0x3e8ee86b
,    0x3f654b68
,    0x3e8ed22c
,    0x3f656f1f
,    0x3e8ebbf7
,    0x3f6592d1
,    0x3e8ea5cd
,    0x3f65b67d
,    0x3e8e8fad
,    0x3f65da24
,    0x3e8e7997
,    0x3f65fdc5
,    0x3e8e638b
,    0x3f662160
,    0x3e8e4d8a
,    0x3f6644f7
,    0x3e8e3793
,    0x3f666887
,    0x3e8e21a6
,    0x3f668c12
,    0x3e8e0bc3
,    0x3f66af98
,    0x3e8df5ea
,    0x3f66d318
,    0x3e8de01c
,    0x3f66f693
,    0x3e8dca57
,    0x3f671a08
,    0x3e8db49c
,    0x3f673d78
,    0x3e8d9eec
,    0x3f6760e3
,    0x3e8d8945
,    0x3f678448
,    0x3e8d73a8
,    0x3f67a7a7
,    0x3e8d5e15
,    0x3f67cb01
,    0x3e8d488c
,    0x3f67ee56
,    0x3e8d330d
,    0x3f6811a6
,    0x3e8d1d98
,    0x3f6834f0
,    0x3e8d082c
,    0x3f685835
,    0x3e8cf2ca
,    0x3f687b74
,    0x3e8cdd72
,    0x3f689eae
,    0x3e8cc823
,    0x3f68c1e3
,    0x3e8cb2df
,    0x3f68e512
,    0x3e8c9da3
,    0x3f69083c
,    0x3e8c8872
,    0x3f692b61
,    0x3e8c734a
,    0x3f694e80
,    0x3e8c5e2b
,    0x3f69719a
,    0x3e8c4916
,    0x3f6994af
,    0x3e8c340b
,    0x3f69b7bf
,    0x3e8c1f09
,    0x3f69dac9
,    0x3e8c0a10
,    0x3f69fdcf
,    0x3e8bf521
,    0x3f6a20ce
,    0x3e8be03b
,    0x3f6a43c9
,    0x3e8bcb5f
,    0x3f6a66bf
,    0x3e8bb68c
,    0x3f6a89af
,    0x3e8ba1c2
,    0x3f6aac9a
,    0x3e8b8d01
,    0x3f6acf80
,    0x3e8b784a
,    0x3f6af260
,    0x3e8b639c
,    0x3f6b153c
,    0x3e8b4ef7
,    0x3f6b3812
,    0x3e8b3a5b
,    0x3f6b5ae3
,    0x3e8b25c8
,    0x3f6b7daf
,    0x3e8b113f
,    0x3f6ba076
,    0x3e8afcbe
,    0x3f6bc338
,    0x3e8ae847
,    0x3f6be5f5
,    0x3e8ad3d9
,    0x3f6c08ac
,    0x3e8abf73
,    0x3f6c2b5f
,    0x3e8aab17
,    0x3f6c4e0c
,    0x3e8a96c3
,    0x3f6c70b4
,    0x3e8a8279
,    0x3f6c9357
,    0x3e8a6e37
,    0x3f6cb5f5
,    0x3e8a59ff
,    0x3f6cd88e
,    0x3e8a45cf
,    0x3f6cfb22
,    0x3e8a31a8
,    0x3f6d1db1
,    0x3e8a1d8a
,    0x3f6d403b
,    0x3e8a0974
,    0x3f6d62c0
,    0x3e89f567
,    0x3f6d8540
,    0x3e89e163
,    0x3f6da7bb
,    0x3e89cd68
,    0x3f6dca31
,    0x3e89b976
,    0x3f6deca1
,    0x3e89a58c
,    0x3f6e0f0d
,    0x3e8991aa
,    0x3f6e3174
,    0x3e897dd2
,    0x3f6e53d6
,    0x3e896a01
,    0x3f6e7633
,    0x3e89563a
,    0x3f6e988b
,    0x3e89427b
,    0x3f6ebade
,    0x3e892ec4
,    0x3f6edd2c
,    0x3e891b16
,    0x3f6eff76
,    0x3e890771
,    0x3f6f21ba
,    0x3e88f3d3
,    0x3f6f43f9
,    0x3e88e03f
,    0x3f6f6634
,    0x3e88ccb2
,    0x3f6f886a
,    0x3e88b92e
,    0x3f6faa9a
,    0x3e88a5b3
,    0x3f6fccc6
,    0x3e88923f
,    0x3f6feeed
,    0x3e887ed4
,    0x3f70110f
,    0x3e886b72
,    0x3f70332d
,    0x3e885817
,    0x3f705545
,    0x3e8844c5
,    0x3f707759
,    0x3e88317b
,    0x3f709967
,    0x3e881e39
,    0x3f70bb71
,    0x3e880aff
,    0x3f70dd76
,    0x3e87f7ce
,    0x3f70ff77
,    0x3e87e4a4
,    0x3f712172
,    0x3e87d183
,    0x3f714369
,    0x3e87be69
,    0x3f71655b
,    0x3e87ab58
,    0x3f718748
,    0x3e87984f
,    0x3f71a931
,    0x3e87854e
,    0x3f71cb15
,    0x3e877254
,    0x3f71ecf3
,    0x3e875f63
,    0x3f720ece
,    0x3e874c7a
,    0x3f7230a3
,    0x3e873998
,    0x3f725274
,    0x3e8726bf
,    0x3f727440
,    0x3e8713ed
,    0x3f729607
,    0x3e870123
,    0x3f72b7ca
,    0x3e86ee61
,    0x3f72d988
,    0x3e86dba7
,    0x3f72fb41
,    0x3e86c8f5
,    0x3f731cf6
,    0x3e86b64a
,    0x3f733ea6
,    0x3e86a3a7
,    0x3f736051
,    0x3e86910c
,    0x3f7381f7
,    0x3e867e79
,    0x3f73a399
,    0x3e866bed
,    0x3f73c537
,    0x3e865969
,    0x3f73e6cf
,    0x3e8646ec
,    0x3f740863
,    0x3e863478
,    0x3f7429f3
,    0x3e86220a
,    0x3f744b7e
,    0x3e860fa5
,    0x3f746d04
,    0x3e85fd47
,    0x3f748e85
,    0x3e85eaf0
,    0x3f74b003
,    0x3e85d8a1
,    0x3f74d17b
,    0x3e85c65a
,    0x3f74f2ef
,    0x3e85b41a
,    0x3f75145e
,    0x3e85a1e1
,    0x3f7535c9
,    0x3e858fb0
,    0x3f75572f
,    0x3e857d86
,    0x3f757891
,    0x3e856b64
,    0x3f7599ee
,    0x3e855949
,    0x3f75bb46
,    0x3e854735
,    0x3f75dc9b
,    0x3e853529
,    0x3f75fdea
,    0x3e852324
,    0x3f761f35
,    0x3e851127
,    0x3f76407c
,    0x3e84ff30
,    0x3f7661be
,    0x3e84ed41
,    0x3f7682fb
,    0x3e84db5a
,    0x3f76a434
,    0x3e84c979
,    0x3f76c569
,    0x3e84b7a0
,    0x3f76e699
,    0x3e84a5cd
,    0x3f7707c5
,    0x3e849402
,    0x3f7728ec
,    0x3e84823f
,    0x3f774a0f
,    0x3e847082
,    0x3f776b2d
,    0x3e845ecc
,    0x3f778c47
,    0x3e844d1e
,    0x3f77ad5c
,    0x3e843b76
,    0x3f77ce6e
,    0x3e8429d6
,    0x3f77ef7a
,    0x3e84183d
,    0x3f781082
,    0x3e8406aa
,    0x3f783186
,    0x3e83f51f
,    0x3f785286
,    0x3e83e39b
,    0x3f787381
,    0x3e83d21e
,    0x3f789478
,    0x3e83c0a7
,    0x3f78b56a
,    0x3e83af38
,    0x3f78d658
,    0x3e839dcf
,    0x3f78f742
,    0x3e838c6e
,    0x3f791827
,    0x3e837b13
,    0x3f793908
,    0x3e8369bf
,    0x3f7959e4
,    0x3e835872
,    0x3f797abd
,    0x3e83472c
,    0x3f799b91
,    0x3e8335ec
,    0x3f79bc60
,    0x3e8324b4
,    0x3f79dd2c
,    0x3e831382
,    0x3f79fdf3
,    0x3e830257
,    0x3f7a1eb5
,    0x3e82f132
,    0x3f7a3f74
,    0x3e82e015
,    0x3f7a602e
,    0x3e82cefe
,    0x3f7a80e4
,    0x3e82bdee
,    0x3f7aa195
,    0x3e82ace4
,    0x3f7ac243
,    0x3e829be1
,    0x3f7ae2ec
,    0x3e828ae5
,    0x3f7b0391
,    0x3e8279ef
,    0x3f7b2431
,    0x3e826900
,    0x3f7b44ce
,    0x3e825818
,    0x3f7b6566
,    0x3e824736
,    0x3f7b85fa
,    0x3e82365a
,    0x3f7ba689
,    0x3e822586
,    0x3f7bc715
,    0x3e8214b7
,    0x3f7be79c
,    0x3e8203ef
,    0x3f7c081f
,    0x3e81f32e
,    0x3f7c289e
,    0x3e81e273
,    0x3f7c4919
,    0x3e81d1bf
,    0x3f7c698f
,    0x3e81c111
,    0x3f7c8a02
,    0x3e81b06a
,    0x3f7caa70
,    0x3e819fc8
,    0x3f7ccada
,    0x3e818f2e
,    0x3f7ceb40
,    0x3e817e99
,    0x3f7d0ba2
,    0x3e816e0b
,    0x3f7d2bff
,    0x3e815d84
,    0x3f7d4c59
,    0x3e814d02
,    0x3f7d6cae
,    0x3e813c87
,    0x3f7d8cff
,    0x3e812c13
,    0x3f7dad4c
,    0x3e811ba4
,    0x3f7dcd95
,    0x3e810b3c
,    0x3f7dedda
,    0x3e80fada
,    0x3f7e0e1b
,    0x3e80ea7f
,    0x3f7e2e57
,    0x3e80da29
,    0x3f7e4e90
,    0x3e80c9da
,    0x3f7e6ec5
,    0x3e80b991
,    0x3f7e8ef5
,    0x3e80a94e
,    0x3f7eaf21
,    0x3e809911
,    0x3f7ecf4a
,    0x3e8088db
,    0x3f7eef6e
,    0x3e8078aa
,    0x3f7f0f8e
,    0x3e806880
,    0x3f7f2faa
,    0x3e80585b
,    0x3f7f4fc2
,    0x3e80483d
,    0x3f7f6fd6
,    0x3e803825
,    0x3f7f8fe6
,    0x3e802813
,    0x3f7faff2
,    0x3e801807
,    0x3f7fcffa
,    0x3e800801
,    0x3f7feffe
,};

// Total pairs: 1024
