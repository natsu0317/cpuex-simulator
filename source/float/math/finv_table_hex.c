const uint32_t inv_table[2048] = {
    0x3f7fc010
,    0x3fffe007
,    0x3f7f4070
,    0x3fffa027
,    0x3f7ec12f
,    0x3fff6067
,    0x3f7e424d
,    0x3fff20c6
,    0x3f7dc3ca
,    0x3ffee146
,    0x3f7d45a6
,    0x3ffea1e4
,    0x3f7cc7df
,    0x3ffe62a3
,    0x3f7c4a76
,    0x3ffe2380
,    0x3f7bcd6a
,    0x3ffde47d
,    0x3f7b50bb
,    0x3ffda59a
,    0x3f7ad468
,    0x3ffd66d5
,    0x3f7a5872
,    0x3ffd282f
,    0x3f79dcd8
,    0x3ffce9a9
,    0x3f796199
,    0x3ffcab41
,    0x3f78e6b5
,    0x3ffc6cf8
,    0x3f786c2b
,    0x3ffc2ece
,    0x3f77f1fd
,    0x3ffbf0c2
,    0x3f777828
,    0x3ffbb2d5
,    0x3f76fead
,    0x3ffb7506
,    0x3f76858b
,    0x3ffb3755
,    0x3f760cc2
,    0x3ffaf9c3
,    0x3f759452
,    0x3ffabc4f
,    0x3f751c3b
,    0x3ffa7ef9
,    0x3f74a47b
,    0x3ffa41c1
,    0x3f742d13
,    0x3ffa04a6
,    0x3f73b603
,    0x3ff9c7aa
,    0x3f733f49
,    0x3ff98acb
,    0x3f72c8e6
,    0x3ff94e0a
,    0x3f7252d9
,    0x3ff91167
,    0x3f71dd23
,    0x3ff8d4e1
,    0x3f7167c2
,    0x3ff89878
,    0x3f70f2b6
,    0x3ff85c2d
,    0x3f707e00
,    0x3ff81fff
,    0x3f70099e
,    0x3ff7e3ee
,    0x3f6f9591
,    0x3ff7a7fa
,    0x3f6f21d8
,    0x3ff76c23
,    0x3f6eae72
,    0x3ff73069
,    0x3f6e3b60
,    0x3ff6f4cc
,    0x3f6dc8a1
,    0x3ff6b94b
,    0x3f6d5635
,    0x3ff67de7
,    0x3f6ce41b
,    0x3ff642a0
,    0x3f6c7254
,    0x3ff60775
,    0x3f6c00df
,    0x3ff5cc67
,    0x3f6b8fbb
,    0x3ff59175
,    0x3f6b1ee8
,    0x3ff5569f
,    0x3f6aae67
,    0x3ff51be5
,    0x3f6a3e36
,    0x3ff4e148
,    0x3f69ce55
,    0x3ff4a6c6
,    0x3f695ec4
,    0x3ff46c61
,    0x3f68ef84
,    0x3ff43217
,    0x3f688093
,    0x3ff3f7e9
,    0x3f6811f0
,    0x3ff3bdd7
,    0x3f67a39d
,    0x3ff383e0
,    0x3f673599
,    0x3ff34a05
,    0x3f66c7e2
,    0x3ff31045
,    0x3f665a7a
,    0x3ff2d6a1
,    0x3f65ed60
,    0x3ff29d18
,    0x3f658093
,    0x3ff263aa
,    0x3f651413
,    0x3ff22a57
,    0x3f64a7e0
,    0x3ff1f120
,    0x3f643bf9
,    0x3ff1b803
,    0x3f63d05f
,    0x3ff17f02
,    0x3f636511
,    0x3ff1461b
,    0x3f62fa0f
,    0x3ff10d4f
,    0x3f628f58
,    0x3ff0d49e
,    0x3f6224ec
,    0x3ff09c08
,    0x3f61bacb
,    0x3ff0638c
,    0x3f6150f5
,    0x3ff02b2a
,    0x3f60e769
,    0x3feff2e3
,    0x3f607e27
,    0x3fefbab6
,    0x3f601530
,    0x3fef82a4
,    0x3f5fac81
,    0x3fef4aac
,    0x3f5f441c
,    0x3fef12ce
,    0x3f5edc00
,    0x3feedb0a
,    0x3f5e742d
,    0x3feea360
,    0x3f5e0ca2
,    0x3fee6bd0
,    0x3f5da55f
,    0x3fee3459
,    0x3f5d3e65
,    0x3fedfcfd
,    0x3f5cd7b2
,    0x3fedc5ba
,    0x3f5c7147
,    0x3fed8e91
,    0x3f5c0b22
,    0x3fed5782
,    0x3f5ba545
,    0x3fed208b
,    0x3f5b3fae
,    0x3fece9af
,    0x3f5ada5e
,    0x3fecb2ec
,    0x3f5a7554
,    0x3fec7c42
,    0x3f5a1090
,    0x3fec45b1
,    0x3f59ac11
,    0x3fec0f39
,    0x3f5947d8
,    0x3febd8db
,    0x3f58e3e4
,    0x3feba295
,    0x3f588035
,    0x3feb6c69
,    0x3f581ccb
,    0x3feb3655
,    0x3f57b9a5
,    0x3feb005a
,    0x3f5756c3
,    0x3feaca78
,    0x3f56f425
,    0x3fea94af
,    0x3f5691cb
,    0x3fea5efe
,    0x3f562fb4
,    0x3fea2966
,    0x3f55cde0
,    0x3fe9f3e6
,    0x3f556c50
,    0x3fe9be7f
,    0x3f550b02
,    0x3fe98930
,    0x3f54a9f7
,    0x3fe953fa
,    0x3f54492d
,    0x3fe91edb
,    0x3f53e8a6
,    0x3fe8e9d5
,    0x3f538861
,    0x3fe8b4e7
,    0x3f53285d
,    0x3fe88011
,    0x3f52c89b
,    0x3fe84b53
,    0x3f526919
,    0x3fe816ac
,    0x3f5209d9
,    0x3fe7e21e
,    0x3f51aad9
,    0x3fe7ada7
,    0x3f514c19
,    0x3fe77949
,    0x3f50ed9a
,    0x3fe74501
,    0x3f508f5a
,    0x3fe710d2
,    0x3f50315b
,    0x3fe6dcba
,    0x3f4fd39a
,    0x3fe6a8b9
,    0x3f4f761a
,    0x3fe674d0
,    0x3f4f18d8
,    0x3fe640fe
,    0x3f4ebbd5
,    0x3fe60d43
,    0x3f4e5f10
,    0x3fe5d9a0
,    0x3f4e028a
,    0x3fe5a614
,    0x3f4da642
,    0x3fe5729f
,    0x3f4d4a38
,    0x3fe53f41
,    0x3f4cee6c
,    0x3fe50bfa
,    0x3f4c92dd
,    0x3fe4d8c9
,    0x3f4c378c
,    0x3fe4a5b0
,    0x3f4bdc77
,    0x3fe472ae
,    0x3f4b81a0
,    0x3fe43fc2
,    0x3f4b2705
,    0x3fe40ced
,    0x3f4acca7
,    0x3fe3da2e
,    0x3f4a7285
,    0x3fe3a787
,    0x3f4a189f
,    0x3fe374f5
,    0x3f49bef5
,    0x3fe3427a
,    0x3f496586
,    0x3fe31016
,    0x3f490c53
,    0x3fe2ddc7
,    0x3f48b35b
,    0x3fe2ab8f
,    0x3f485a9e
,    0x3fe2796e
,    0x3f48021c
,    0x3fe24762
,    0x3f47a9d4
,    0x3fe2156d
,    0x3f4751c7
,    0x3fe1e38d
,    0x3f46f9f4
,    0x3fe1b1c4
,    0x3f46a25b
,    0x3fe18010
,    0x3f464afc
,    0x3fe14e72
,    0x3f45f3d7
,    0x3fe11ceb
,    0x3f459cea
,    0x3fe0eb79
,    0x3f454637
,    0x3fe0ba1c
,    0x3f44efbd
,    0x3fe088d5
,    0x3f44997c
,    0x3fe057a4
,    0x3f444374
,    0x3fe02689
,    0x3f43eda4
,    0x3fdff583
,    0x3f43980c
,    0x3fdfc492
,    0x3f4342ac
,    0x3fdf93b6
,    0x3f42ed84
,    0x3fdf62f0
,    0x3f429894
,    0x3fdf3240
,    0x3f4243db
,    0x3fdf01a4
,    0x3f41ef59
,    0x3fded11e
,    0x3f419b0f
,    0x3fdea0ac
,    0x3f4146fb
,    0x3fde7050
,    0x3f40f31e
,    0x3fde4009
,    0x3f409f78
,    0x3fde0fd7
,    0x3f404c08
,    0x3fdddfb9
,    0x3f3ff8ce
,    0x3fddafb1
,    0x3f3fa5ca
,    0x3fdd7fbd
,    0x3f3f52fc
,    0x3fdd4fde
,    0x3f3f0064
,    0x3fdd2013
,    0x3f3eae01
,    0x3fdcf05d
,    0x3f3e5bd3
,    0x3fdcc0bc
,    0x3f3e09db
,    0x3fdc912f
,    0x3f3db817
,    0x3fdc61b7
,    0x3f3d6688
,    0x3fdc3253
,    0x3f3d152e
,    0x3fdc0304
,    0x3f3cc408
,    0x3fdbd3c9
,    0x3f3c7316
,    0x3fdba4a2
,    0x3f3c2258
,    0x3fdb758f
,    0x3f3bd1ce
,    0x3fdb4691
,    0x3f3b8178
,    0x3fdb17a6
,    0x3f3b3155
,    0x3fdae8d0
,    0x3f3ae165
,    0x3fdaba0e
,    0x3f3a91a9
,    0x3fda8b5f
,    0x3f3a4220
,    0x3fda5cc5
,    0x3f39f2c9
,    0x3fda2e3e
,    0x3f39a3a5
,    0x3fd9ffcb
,    0x3f3954b4
,    0x3fd9d16c
,    0x3f3905f5
,    0x3fd9a321
,    0x3f38b768
,    0x3fd974e9
,    0x3f38690d
,    0x3fd946c5
,    0x3f381ae4
,    0x3fd918b5
,    0x3f37ccec
,    0x3fd8eab8
,    0x3f377f26
,    0x3fd8bcce
,    0x3f373192
,    0x3fd88ef8
,    0x3f36e42e
,    0x3fd86135
,    0x3f3696fc
,    0x3fd83386
,    0x3f3649fa
,    0x3fd805ea
,    0x3f35fd29
,    0x3fd7d861
,    0x3f35b088
,    0x3fd7aaeb
,    0x3f356418
,    0x3fd77d89
,    0x3f3517d8
,    0x3fd75039
,    0x3f34cbc8
,    0x3fd722fd
,    0x3f347fe8
,    0x3fd6f5d3
,    0x3f343438
,    0x3fd6c8bd
,    0x3f33e8b7
,    0x3fd69bb9
,    0x3f339d66
,    0x3fd66ec8
,    0x3f335244
,    0x3fd641eb
,    0x3f330751
,    0x3fd6151f
,    0x3f32bc8d
,    0x3fd5e867
,    0x3f3271f8
,    0x3fd5bbc1
,    0x3f322791
,    0x3fd58f2e
,    0x3f31dd59
,    0x3fd562ad
,    0x3f319350
,    0x3fd5363f
,    0x3f314974
,    0x3fd509e4
,    0x3f30ffc7
,    0x3fd4dd9a
,    0x3f30b647
,    0x3fd4b164
,    0x3f306cf5
,    0x3fd4853f
,    0x3f3023d1
,    0x3fd4592d
,    0x3f2fdada
,    0x3fd42d2d
,    0x3f2f9211
,    0x3fd40140
,    0x3f2f4974
,    0x3fd3d564
,    0x3f2f0105
,    0x3fd3a99b
,    0x3f2eb8c2
,    0x3fd37de4
,    0x3f2e70ad
,    0x3fd3523f
,    0x3f2e28c3
,    0x3fd326ab
,    0x3f2de107
,    0x3fd2fb2a
,    0x3f2d9976
,    0x3fd2cfbb
,    0x3f2d5212
,    0x3fd2a45d
,    0x3f2d0ada
,    0x3fd27912
,    0x3f2cc3cd
,    0x3fd24dd8
,    0x3f2c7cec
,    0x3fd222b0
,    0x3f2c3637
,    0x3fd1f799
,    0x3f2befad
,    0x3fd1cc95
,    0x3f2ba94f
,    0x3fd1a1a2
,    0x3f2b631c
,    0x3fd176c0
,    0x3f2b1d14
,    0x3fd14bf0
,    0x3f2ad736
,    0x3fd12132
,    0x3f2a9184
,    0x3fd0f684
,    0x3f2a4bfc
,    0x3fd0cbe9
,    0x3f2a069e
,    0x3fd0a15e
,    0x3f29c16b
,    0x3fd076e5
,    0x3f297c62
,    0x3fd04c7e
,    0x3f293783
,    0x3fd02227
,    0x3f28f2cf
,    0x3fcff7e2
,    0x3f28ae43
,    0x3fcfcdae
,    0x3f2869e2
,    0x3fcfa38b
,    0x3f2825aa
,    0x3fcf7979
,    0x3f27e19c
,    0x3fcf4f78
,    0x3f279db7
,    0x3fcf2588
,    0x3f2759fb
,    0x3fcefba9
,    0x3f271668
,    0x3fced1db
,    0x3f26d2fe
,    0x3fcea81e
,    0x3f268fbc
,    0x3fce7e71
,    0x3f264ca4
,    0x3fce54d6
,    0x3f2609b3
,    0x3fce2b4b
,    0x3f25c6ec
,    0x3fce01d1
,    0x3f25844c
,    0x3fcdd868
,    0x3f2541d5
,    0x3fcdaf0f
,    0x3f24ff85
,    0x3fcd85c7
,    0x3f24bd5e
,    0x3fcd5c8f
,    0x3f247b5e
,    0x3fcd3368
,    0x3f243986
,    0x3fcd0a51
,    0x3f23f7d5
,    0x3fcce14b
,    0x3f23b64c
,    0x3fccb856
,    0x3f2374ea
,    0x3fcc8f70
,    0x3f2333af
,    0x3fcc669b
,    0x3f22f29c
,    0x3fcc3dd6
,    0x3f22b1af
,    0x3fcc1522
,    0x3f2270e9
,    0x3fcbec7d
,    0x3f223049
,    0x3fcbc3e9
,    0x3f21efd0
,    0x3fcb9b65
,    0x3f21af7e
,    0x3fcb72f1
,    0x3f216f51
,    0x3fcb4a8d
,    0x3f212f4b
,    0x3fcb223a
,    0x3f20ef6b
,    0x3fcaf9f6
,    0x3f20afb1
,    0x3fcad1c2
,    0x3f20701d
,    0x3fcaa99e
,    0x3f2030ae
,    0x3fca818a
,    0x3f1ff165
,    0x3fca5986
,    0x3f1fb242
,    0x3fca3191
,    0x3f1f7344
,    0x3fca09ac
,    0x3f1f346b
,    0x3fc9e1d7
,    0x3f1ef5b7
,    0x3fc9ba12
,    0x3f1eb728
,    0x3fc9925d
,    0x3f1e78be
,    0x3fc96ab7
,    0x3f1e3a79
,    0x3fc94320
,    0x3f1dfc59
,    0x3fc91b99
,    0x3f1dbe5d
,    0x3fc8f422
,    0x3f1d8086
,    0x3fc8ccba
,    0x3f1d42d3
,    0x3fc8a562
,    0x3f1d0544
,    0x3fc87e19
,    0x3f1cc7d9
,    0x3fc856df
,    0x3f1c8a93
,    0x3fc82fb5
,    0x3f1c4d70
,    0x3fc8089a
,    0x3f1c1071
,    0x3fc7e18e
,    0x3f1bd396
,    0x3fc7ba92
,    0x3f1b96de
,    0x3fc793a4
,    0x3f1b5a49
,    0x3fc76cc6
,    0x3f1b1dd9
,    0x3fc745f7
,    0x3f1ae18b
,    0x3fc71f37
,    0x3f1aa560
,    0x3fc6f886
,    0x3f1a6959
,    0x3fc6d1e4
,    0x3f1a2d74
,    0x3fc6ab52
,    0x3f19f1b2
,    0x3fc684ce
,    0x3f19b613
,    0x3fc65e59
,    0x3f197a97
,    0x3fc637f3
,    0x3f193f3d
,    0x3fc6119b
,    0x3f190405
,    0x3fc5eb53
,    0x3f18c8f0
,    0x3fc5c519
,    0x3f188dfd
,    0x3fc59eef
,    0x3f18532c
,    0x3fc578d2
,    0x3f18187d
,    0x3fc552c5
,    0x3f17ddf0
,    0x3fc52cc6
,    0x3f17a385
,    0x3fc506d6
,    0x3f17693b
,    0x3fc4e0f4
,    0x3f172f13
,    0x3fc4bb21
,    0x3f16f50d
,    0x3fc4955d
,    0x3f16bb27
,    0x3fc46fa7
,    0x3f168164
,    0x3fc449ff
,    0x3f1647c1
,    0x3fc42466
,    0x3f160e3f
,    0x3fc3fedb
,    0x3f15d4df
,    0x3fc3d95f
,    0x3f159b9f
,    0x3fc3b3f1
,    0x3f156280
,    0x3fc38e91
,    0x3f152982
,    0x3fc36940
,    0x3f14f0a4
,    0x3fc343fc
,    0x3f14b7e7
,    0x3fc31ec7
,    0x3f147f4a
,    0x3fc2f9a0
,    0x3f1446cd
,    0x3fc2d488
,    0x3f140e71
,    0x3fc2af7d
,    0x3f13d635
,    0x3fc28a80
,    0x3f139e19
,    0x3fc26592
,    0x3f13661c
,    0x3fc240b1
,    0x3f132e40
,    0x3fc21bdf
,    0x3f12f683
,    0x3fc1f71a
,    0x3f12bee6
,    0x3fc1d264
,    0x3f128769
,    0x3fc1adbb
,    0x3f12500b
,    0x3fc18920
,    0x3f1218cc
,    0x3fc16493
,    0x3f11e1ad
,    0x3fc14013
,    0x3f11aaac
,    0x3fc11ba2
,    0x3f1173cb
,    0x3fc0f73e
,    0x3f113d09
,    0x3fc0d2e8
,    0x3f110666
,    0x3fc0aea0
,    0x3f10cfe1
,    0x3fc08a65
,    0x3f10997c
,    0x3fc06638
,    0x3f106334
,    0x3fc04218
,    0x3f102d0c
,    0x3fc01e06
,    0x3f0ff702
,    0x3fbffa01
,    0x3f0fc116
,    0x3fbfd60a
,    0x3f0f8b48
,    0x3fbfb221
,    0x3f0f5599
,    0x3fbf8e45
,    0x3f0f2008
,    0x3fbf6a76
,    0x3f0eea95
,    0x3fbf46b5
,    0x3f0eb53f
,    0x3fbf2301
,    0x3f0e8008
,    0x3fbeff5a
,    0x3f0e4aee
,    0x3fbedbc1
,    0x3f0e15f2
,    0x3fbeb835
,    0x3f0de114
,    0x3fbe94b6
,    0x3f0dac53
,    0x3fbe7144
,    0x3f0d77af
,    0x3fbe4de0
,    0x3f0d4329
,    0x3fbe2a88
,    0x3f0d0ec0
,    0x3fbe073e
,    0x3f0cda74
,    0x3fbde401
,    0x3f0ca645
,    0x3fbdc0d1
,    0x3f0c7233
,    0x3fbd9dae
,    0x3f0c3e3e
,    0x3fbd7a98
,    0x3f0c0a66
,    0x3fbd578f
,    0x3f0bd6aa
,    0x3fbd3492
,    0x3f0ba30c
,    0x3fbd11a3
,    0x3f0b6f89
,    0x3fbceec1
,    0x3f0b3c24
,    0x3fbccbeb
,    0x3f0b08da
,    0x3fbca923
,    0x3f0ad5ad
,    0x3fbc8667
,    0x3f0aa29c
,    0x3fbc63b8
,    0x3f0a6fa8
,    0x3fbc4116
,    0x3f0a3ccf
,    0x3fbc1e80
,    0x3f0a0a13
,    0x3fbbfbf7
,    0x3f09d772
,    0x3fbbd97b
,    0x3f09a4ed
,    0x3fbbb70c
,    0x3f097284
,    0x3fbb94a9
,    0x3f094037
,    0x3fbb7252
,    0x3f090e05
,    0x3fbb5008
,    0x3f08dbef
,    0x3fbb2dcb
,    0x3f08a9f4
,    0x3fbb0b9b
,    0x3f087814
,    0x3fbae976
,    0x3f084650
,    0x3fbac75e
,    0x3f0814a7
,    0x3fbaa553
,    0x3f07e319
,    0x3fba8354
,    0x3f07b1a6
,    0x3fba6162
,    0x3f07804e
,    0x3fba3f7b
,    0x3f074f11
,    0x3fba1da1
,    0x3f071def
,    0x3fb9fbd4
,    0x3f06ece8
,    0x3fb9da12
,    0x3f06bbfb
,    0x3fb9b85d
,    0x3f068b29
,    0x3fb996b4
,    0x3f065a71
,    0x3fb97518
,    0x3f0629d4
,    0x3fb95387
,    0x3f05f951
,    0x3fb93203
,    0x3f05c8e9
,    0x3fb9108a
,    0x3f05989a
,    0x3fb8ef1e
,    0x3f056866
,    0x3fb8cdbe
,    0x3f05384c
,    0x3fb8ac6a
,    0x3f05084c
,    0x3fb88b22
,    0x3f04d866
,    0x3fb869e6
,    0x3f04a899
,    0x3fb848b6
,    0x3f0478e7
,    0x3fb82792
,    0x3f04494e
,    0x3fb80679
,    0x3f0419cf
,    0x3fb7e56d
,    0x3f03ea69
,    0x3fb7c46c
,    0x3f03bb1d
,    0x3fb7a378
,    0x3f038bea
,    0x3fb7828f
,    0x3f035cd1
,    0x3fb761b2
,    0x3f032dd1
,    0x3fb740e0
,    0x3f02feea
,    0x3fb7201b
,    0x3f02d01c
,    0x3fb6ff61
,    0x3f02a167
,    0x3fb6deb3
,    0x3f0272cc
,    0x3fb6be10
,    0x3f024449
,    0x3fb69d79
,    0x3f0215df
,    0x3fb67cee
,    0x3f01e78e
,    0x3fb65c6e
,    0x3f01b955
,    0x3fb63bfa
,    0x3f018b36
,    0x3fb61b92
,    0x3f015d2f
,    0x3fb5fb35
,    0x3f012f40
,    0x3fb5dae3
,    0x3f01016a
,    0x3fb5ba9d
,    0x3f00d3ac
,    0x3fb59a62
,    0x3f00a606
,    0x3fb57a33
,    0x3f007879
,    0x3fb55a0f
,    0x3f004b04
,    0x3fb539f7
,    0x3f001da7
,    0x3fb519ea
,    0x3effe0c4
,    0x3fb4f9e8
,    0x3eff866a
,    0x3fb4d9f2
,    0x3eff2c40
,    0x3fb4ba07
,    0x3efed245
,    0x3fb49a27
,    0x3efe787a
,    0x3fb47a52
,    0x3efe1edf
,    0x3fb45a88
,    0x3efdc573
,    0x3fb43aca
,    0x3efd6c36
,    0x3fb41b17
,    0x3efd1328
,    0x3fb3fb6f
,    0x3efcba49
,    0x3fb3dbd2
,    0x3efc6199
,    0x3fb3bc41
,    0x3efc0917
,    0x3fb39cba
,    0x3efbb0c4
,    0x3fb37d3e
,    0x3efb589f
,    0x3fb35dce
,    0x3efb00a9
,    0x3fb33e68
,    0x3efaa8e0
,    0x3fb31f0e
,    0x3efa5146
,    0x3fb2ffbe
,    0x3ef9f9da
,    0x3fb2e079
,    0x3ef9a29b
,    0x3fb2c13f
,    0x3ef94b8a
,    0x3fb2a211
,    0x3ef8f4a7
,    0x3fb282ed
,    0x3ef89df1
,    0x3fb263d3
,    0x3ef84768
,    0x3fb244c5
,    0x3ef7f10c
,    0x3fb225c2
,    0x3ef79ade
,    0x3fb206c9
,    0x3ef744dc
,    0x3fb1e7db
,    0x3ef6ef07
,    0x3fb1c8f8
,    0x3ef6995f
,    0x3fb1aa1f
,    0x3ef643e3
,    0x3fb18b51
,    0x3ef5ee94
,    0x3fb16c8e
,    0x3ef59971
,    0x3fb14dd6
,    0x3ef5447a
,    0x3fb12f28
,    0x3ef4efaf
,    0x3fb11084
,    0x3ef49b11
,    0x3fb0f1ec
,    0x3ef4469e
,    0x3fb0d35e
,    0x3ef3f256
,    0x3fb0b4da
,    0x3ef39e3b
,    0x3fb09661
,    0x3ef34a4a
,    0x3fb077f3
,    0x3ef2f686
,    0x3fb0598e
,    0x3ef2a2ec
,    0x3fb03b35
,    0x3ef24f7d
,    0x3fb01ce6
,    0x3ef1fc3a
,    0x3faffea1
,    0x3ef1a921
,    0x3fafe067
,    0x3ef15633
,    0x3fafc237
,    0x3ef10370
,    0x3fafa411
,    0x3ef0b0d7
,    0x3faf85f6
,    0x3ef05e69
,    0x3faf67e5
,    0x3ef00c25
,    0x3faf49de
,    0x3eefba0c
,    0x3faf2be2
,    0x3eef681c
,    0x3faf0df0
,    0x3eef1656
,    0x3faef008
,    0x3eeec4bb
,    0x3faed22a
,    0x3eee7349
,    0x3faeb457
,    0x3eee2200
,    0x3fae968d
,    0x3eedd0e1
,    0x3fae78ce
,    0x3eed7fec
,    0x3fae5b19
,    0x3eed2f20
,    0x3fae3d6e
,    0x3eecde7d
,    0x3fae1fcd
,    0x3eec8e03
,    0x3fae0236
,    0x3eec3db2
,    0x3fade4aa
,    0x3eebed8b
,    0x3fadc727
,    0x3eeb9d8b
,    0x3fada9ae
,    0x3eeb4db5
,    0x3fad8c40
,    0x3eeafe07
,    0x3fad6edb
,    0x3eeaae81
,    0x3fad5180
,    0x3eea5f24
,    0x3fad342f
,    0x3eea0fef
,    0x3fad16e8
,    0x3ee9c0e2
,    0x3facf9ab
,    0x3ee971fe
,    0x3facdc78
,    0x3ee92341
,    0x3facbf4f
,    0x3ee8d4ac
,    0x3faca22f
,    0x3ee8863e
,    0x3fac851a
,    0x3ee837f9
,    0x3fac680e
,    0x3ee7e9da
,    0x3fac4b0c
,    0x3ee79be3
,    0x3fac2e13
,    0x3ee74e14
,    0x3fac1125
,    0x3ee7006b
,    0x3fabf440
,    0x3ee6b2ea
,    0x3fabd765
,    0x3ee66590
,    0x3fabba93
,    0x3ee6185c
,    0x3fab9dcb
,    0x3ee5cb50
,    0x3fab810d
,    0x3ee57e6a
,    0x3fab6458
,    0x3ee531aa
,    0x3fab47ad
,    0x3ee4e511
,    0x3fab2b0c
,    0x3ee4989f
,    0x3fab0e74
,    0x3ee44c52
,    0x3faaf1e6
,    0x3ee4002c
,    0x3faad561
,    0x3ee3b42c
,    0x3faab8e6
,    0x3ee36852
,    0x3faa9c74
,    0x3ee31c9e
,    0x3faa800c
,    0x3ee2d110
,    0x3faa63ad
,    0x3ee285a7
,    0x3faa4757
,    0x3ee23a64
,    0x3faa2b0b
,    0x3ee1ef46
,    0x3faa0ec9
,    0x3ee1a44e
,    0x3fa9f28f
,    0x3ee1597b
,    0x3fa9d660
,    0x3ee10ecd
,    0x3fa9ba39
,    0x3ee0c444
,    0x3fa99e1c
,    0x3ee079e0
,    0x3fa98208
,    0x3ee02fa2
,    0x3fa965fd
,    0x3edfe588
,    0x3fa949fc
,    0x3edf9b92
,    0x3fa92e04
,    0x3edf51c2
,    0x3fa91215
,    0x3edf0816
,    0x3fa8f630
,    0x3edebe8e
,    0x3fa8da53
,    0x3ede752b
,    0x3fa8be80
,    0x3ede2beb
,    0x3fa8a2b6
,    0x3edde2d0
,    0x3fa886f5
,    0x3edd99da
,    0x3fa86b3d
,    0x3edd5107
,    0x3fa84f8e
,    0x3edd0858
,    0x3fa833e9
,    0x3edcbfcc
,    0x3fa8184c
,    0x3edc7765
,    0x3fa7fcb9
,    0x3edc2f21
,    0x3fa7e12f
,    0x3edbe700
,    0x3fa7c5ad
,    0x3edb9f03
,    0x3fa7aa35
,    0x3edb572a
,    0x3fa78ec5
,    0x3edb0f73
,    0x3fa7735f
,    0x3edac7e0
,    0x3fa75801
,    0x3eda806f
,    0x3fa73cad
,    0x3eda3922
,    0x3fa72161
,    0x3ed9f1f8
,    0x3fa7061f
,    0x3ed9aaf0
,    0x3fa6eae5
,    0x3ed9640b
,    0x3fa6cfb4
,    0x3ed91d49
,    0x3fa6b48c
,    0x3ed8d6a9
,    0x3fa6996d
,    0x3ed8902c
,    0x3fa67e56
,    0x3ed849d1
,    0x3fa66349
,    0x3ed80398
,    0x3fa64844
,    0x3ed7bd81
,    0x3fa62d48
,    0x3ed7778d
,    0x3fa61254
,    0x3ed731ba
,    0x3fa5f76a
,    0x3ed6ec0a
,    0x3fa5dc88
,    0x3ed6a67b
,    0x3fa5c1af
,    0x3ed6610e
,    0x3fa5a6de
,    0x3ed61bc3
,    0x3fa58c16
,    0x3ed5d699
,    0x3fa57157
,    0x3ed59191
,    0x3fa556a1
,    0x3ed54caa
,    0x3fa53bf3
,    0x3ed507e4
,    0x3fa5214e
,    0x3ed4c340
,    0x3fa506b1
,    0x3ed47ebd
,    0x3fa4ec1d
,    0x3ed43a5b
,    0x3fa4d191
,    0x3ed3f61a
,    0x3fa4b70e
,    0x3ed3b1f9
,    0x3fa49c94
,    0x3ed36dfa
,    0x3fa48222
,    0x3ed32a1b
,    0x3fa467b8
,    0x3ed2e65d
,    0x3fa44d57
,    0x3ed2a2c0
,    0x3fa432ff
,    0x3ed25f43
,    0x3fa418af
,    0x3ed21be7
,    0x3fa3fe67
,    0x3ed1d8ab
,    0x3fa3e428
,    0x3ed1958f
,    0x3fa3c9f1
,    0x3ed15293
,    0x3fa3afc2
,    0x3ed10fb7
,    0x3fa3959c
,    0x3ed0ccfc
,    0x3fa37b7e
,    0x3ed08a60
,    0x3fa36169
,    0x3ed047e4
,    0x3fa3475c
,    0x3ed00588
,    0x3fa32d57
,    0x3ecfc34c
,    0x3fa3135a
,    0x3ecf812f
,    0x3fa2f966
,    0x3ecf3f32
,    0x3fa2df7a
,    0x3ecefd54
,    0x3fa2c596
,    0x3ecebb96
,    0x3fa2abbb
,    0x3ece79f7
,    0x3fa291e7
,    0x3ece3877
,    0x3fa2781c
,    0x3ecdf716
,    0x3fa25e59
,    0x3ecdb5d5
,    0x3fa2449e
,    0x3ecd74b2
,    0x3fa22aec
,    0x3ecd33ae
,    0x3fa21141
,    0x3eccf2ca
,    0x3fa1f79f
,    0x3eccb204
,    0x3fa1de04
,    0x3ecc715c
,    0x3fa1c472
,    0x3ecc30d3
,    0x3fa1aae8
,    0x3ecbf069
,    0x3fa19166
,    0x3ecbb01d
,    0x3fa177ec
,    0x3ecb6ff0
,    0x3fa15e7a
,    0x3ecb2fe1
,    0x3fa14510
,    0x3ecaeff0
,    0x3fa12bae
,    0x3ecab01d
,    0x3fa11254
,    0x3eca7069
,    0x3fa0f902
,    0x3eca30d2
,    0x3fa0dfb8
,    0x3ec9f159
,    0x3fa0c676
,    0x3ec9b1ff
,    0x3fa0ad3c
,    0x3ec972c2
,    0x3fa09409
,    0x3ec933a2
,    0x3fa07adf
,    0x3ec8f4a1
,    0x3fa061bc
,    0x3ec8b5bd
,    0x3fa048a2
,    0x3ec876f6
,    0x3fa02f8f
,    0x3ec8384d
,    0x3fa01684
,    0x3ec7f9c1
,    0x3f9ffd81
,    0x3ec7bb53
,    0x3f9fe485
,    0x3ec77d02
,    0x3f9fcb92
,    0x3ec73ece
,    0x3f9fb2a6
,    0x3ec700b7
,    0x3f9f99c2
,    0x3ec6c2bd
,    0x3f9f80e6
,    0x3ec684e0
,    0x3f9f6812
,    0x3ec6471f
,    0x3f9f4f45
,    0x3ec6097c
,    0x3f9f3680
,    0x3ec5cbf5
,    0x3f9f1dc2
,    0x3ec58e8b
,    0x3f9f050d
,    0x3ec5513e
,    0x3f9eec5f
,    0x3ec5140d
,    0x3f9ed3b8
,    0x3ec4d6f8
,    0x3f9ebb1a
,    0x3ec49a00
,    0x3f9ea283
,    0x3ec45d25
,    0x3f9e89f3
,    0x3ec42065
,    0x3f9e716b
,    0x3ec3e3c2
,    0x3f9e58eb
,    0x3ec3a73a
,    0x3f9e4072
,    0x3ec36acf
,    0x3f9e2801
,    0x3ec32e80
,    0x3f9e0f98
,    0x3ec2f24d
,    0x3f9df736
,    0x3ec2b635
,    0x3f9ddedb
,    0x3ec27a39
,    0x3f9dc688
,    0x3ec23e59
,    0x3f9dae3d
,    0x3ec20295
,    0x3f9d95f9
,    0x3ec1c6ec
,    0x3f9d7dbc
,    0x3ec18b5f
,    0x3f9d6587
,    0x3ec14fed
,    0x3f9d4d59
,    0x3ec11496
,    0x3f9d3533
,    0x3ec0d95b
,    0x3f9d1d14
,    0x3ec09e3b
,    0x3f9d04fc
,    0x3ec06336
,    0x3f9cecec
,    0x3ec0284c
,    0x3f9cd4e4
,    0x3ebfed7e
,    0x3f9cbce2
,    0x3ebfb2ca
,    0x3f9ca4e8
,    0x3ebf7831
,    0x3f9c8cf6
,    0x3ebf3db3
,    0x3f9c750a
,    0x3ebf0350
,    0x3f9c5d26
,    0x3ebec908
,    0x3f9c4549
,    0x3ebe8eda
,    0x3f9c2d74
,    0x3ebe54c7
,    0x3f9c15a6
,    0x3ebe1ace
,    0x3f9bfddf
,    0x3ebde0f0
,    0x3f9be61f
,    0x3ebda72c
,    0x3f9bce66
,    0x3ebd6d83
,    0x3f9bb6b5
,    0x3ebd33f4
,    0x3f9b9f0b
,    0x3ebcfa7f
,    0x3f9b8768
,    0x3ebcc124
,    0x3f9b6fcc
,    0x3ebc87e4
,    0x3f9b5838
,    0x3ebc4ebd
,    0x3f9b40aa
,    0x3ebc15b0
,    0x3f9b2924
,    0x3ebbdcbe
,    0x3f9b11a5
,    0x3ebba3e5
,    0x3f9afa2d
,    0x3ebb6b26
,    0x3f9ae2bc
,    0x3ebb3280
,    0x3f9acb52
,    0x3ebaf9f5
,    0x3f9ab3ef
,    0x3ebac182
,    0x3f9a9c94
,    0x3eba892a
,    0x3f9a853f
,    0x3eba50eb
,    0x3f9a6df1
,    0x3eba18c5
,    0x3f9a56ab
,    0x3eb9e0b9
,    0x3f9a3f6b
,    0x3eb9a8c6
,    0x3f9a2833
,    0x3eb970ec
,    0x3f9a1101
,    0x3eb9392c
,    0x3f99f9d6
,    0x3eb90184
,    0x3f99e2b3
,    0x3eb8c9f6
,    0x3f99cb96
,    0x3eb89281
,    0x3f99b480
,    0x3eb85b24
,    0x3f999d71
,    0x3eb823e1
,    0x3f998669
,    0x3eb7ecb6
,    0x3f996f68
,    0x3eb7b5a4
,    0x3f99586e
,    0x3eb77eab
,    0x3f99417b
,    0x3eb747cb
,    0x3f992a8f
,    0x3eb71103
,    0x3f9913a9
,    0x3eb6da53
,    0x3f98fcca
,    0x3eb6a3bc
,    0x3f98e5f2
,    0x3eb66d3e
,    0x3f98cf21
,    0x3eb636d8
,    0x3f98b857
,    0x3eb6008a
,    0x3f98a194
,    0x3eb5ca55
,    0x3f988ad7
,    0x3eb59437
,    0x3f987421
,    0x3eb55e32
,    0x3f985d72
,    0x3eb52845
,    0x3f9846ca
,    0x3eb4f270
,    0x3f983028
,    0x3eb4bcb3
,    0x3f98198d
,    0x3eb4870e
,    0x3f9802f9
,    0x3eb45181
,    0x3f97ec6b
,    0x3eb41c0c
,    0x3f97d5e4
,    0x3eb3e6ae
,    0x3f97bf64
,    0x3eb3b168
,    0x3f97a8eb
,    0x3eb37c3a
,    0x3f979278
,    0x3eb34723
,    0x3f977c0c
,    0x3eb31224
,    0x3f9765a6
,    0x3eb2dd3c
,    0x3f974f47
,    0x3eb2a86c
,    0x3f9738ef
,    0x3eb273b3
,    0x3f97229d
,    0x3eb23f12
,    0x3f970c52
,    0x3eb20a87
,    0x3f96f60d
,    0x3eb1d614
,    0x3f96dfcf
,    0x3eb1a1b8
,    0x3f96c998
,    0x3eb16d74
,    0x3f96b367
,    0x3eb13946
,    0x3f969d3c
,    0x3eb1052f
,    0x3f968718
,    0x3eb0d12f
,    0x3f9670fb
,    0x3eb09d47
,    0x3f965ae4
,    0x3eb06975
,    0x3f9644d4
,    0x3eb035b9
,    0x3f962eca
,    0x3eb00215
,    0x3f9618c6
,    0x3eafce87
,    0x3f9602c9
,    0x3eaf9b10
,    0x3f95ecd3
,    0x3eaf67af
,    0x3f95d6e2
,    0x3eaf3465
,    0x3f95c0f9
,    0x3eaf0132
,    0x3f95ab15
,    0x3eaece15
,    0x3f959538
,    0x3eae9b0e
,    0x3f957f62
,    0x3eae681d
,    0x3f956992
,    0x3eae3543
,    0x3f9553c8
,    0x3eae027f
,    0x3f953e04
,    0x3eadcfd2
,    0x3f952847
,    0x3ead9d3a
,    0x3f951290
,    0x3ead6ab9
,    0x3f94fce0
,    0x3ead384d
,    0x3f94e736
,    0x3ead05f8
,    0x3f94d192
,    0x3eacd3b8
,    0x3f94bbf4
,    0x3eaca18e
,    0x3f94a65d
,    0x3eac6f7a
,    0x3f9490cc
,    0x3eac3d7c
,    0x3f947b41
,    0x3eac0b94
,    0x3f9465bc
,    0x3eabd9c1
,    0x3f94503e
,    0x3eaba804
,    0x3f943ac6
,    0x3eab765d
,    0x3f942554
,    0x3eab44cb
,    0x3f940fe8
,    0x3eab134e
,    0x3f93fa83
,    0x3eaae1e7
,    0x3f93e523
,    0x3eaab096
,    0x3f93cfca
,    0x3eaa7f59
,    0x3f93ba77
,    0x3eaa4e32
,    0x3f93a52a
,    0x3eaa1d20
,    0x3f938fe4
,    0x3ea9ec24
,    0x3f937aa3
,    0x3ea9bb3c
,    0x3f936569
,    0x3ea98a6a
,    0x3f935034
,    0x3ea959ad
,    0x3f933b06
,    0x3ea92905
,    0x3f9325de
,    0x3ea8f871
,    0x3f9310bc
,    0x3ea8c7f3
,    0x3f92fba0
,    0x3ea89789
,    0x3f92e68a
,    0x3ea86735
,    0x3f92d17a
,    0x3ea836f5
,    0x3f92bc70
,    0x3ea806c9
,    0x3f92a76c
,    0x3ea7d6b3
,    0x3f92926e
,    0x3ea7a6b1
,    0x3f927d77
,    0x3ea776c4
,    0x3f926885
,    0x3ea746eb
,    0x3f925399
,    0x3ea71726
,    0x3f923eb3
,    0x3ea6e777
,    0x3f9229d3
,    0x3ea6b7db
,    0x3f9214f9
,    0x3ea68854
,    0x3f920025
,    0x3ea658e1
,    0x3f91eb57
,    0x3ea62983
,    0x3f91d68f
,    0x3ea5fa39
,    0x3f91c1cd
,    0x3ea5cb02
,    0x3f91ad10
,    0x3ea59be0
,    0x3f91985a
,    0x3ea56cd3
,    0x3f9183a9
,    0x3ea53dd9
,    0x3f916eff
,    0x3ea50ef3
,    0x3f915a5a
,    0x3ea4e021
,    0x3f9145bb
,    0x3ea4b163
,    0x3f913122
,    0x3ea482b9
,    0x3f911c8f
,    0x3ea45423
,    0x3f910801
,    0x3ea425a0
,    0x3f90f37a
,    0x3ea3f731
,    0x3f90def8
,    0x3ea3c8d6
,    0x3f90ca7c
,    0x3ea39a8f
,    0x3f90b605
,    0x3ea36c5b
,    0x3f90a195
,    0x3ea33e3b
,    0x3f908d2a
,    0x3ea3102e
,    0x3f9078c5
,    0x3ea2e235
,    0x3f906466
,    0x3ea2b44f
,    0x3f90500d
,    0x3ea2867c
,    0x3f903bb9
,    0x3ea258bd
,    0x3f90276b
,    0x3ea22b11
,    0x3f901323
,    0x3ea1fd79
,    0x3f8ffee1
,    0x3ea1cff3
,    0x3f8feaa4
,    0x3ea1a281
,    0x3f8fd66d
,    0x3ea17522
,    0x3f8fc23b
,    0x3ea147d6
,    0x3f8fae0f
,    0x3ea11a9e
,    0x3f8f99e9
,    0x3ea0ed78
,    0x3f8f85c9
,    0x3ea0c065
,    0x3f8f71ae
,    0x3ea09365
,    0x3f8f5d99
,    0x3ea06678
,    0x3f8f4989
,    0x3ea0399d
,    0x3f8f357f
,    0x3ea00cd6
,    0x3f8f217a
,    0x3e9fe021
,    0x3f8f0d7c
,    0x3e9fb37f
,    0x3f8ef982
,    0x3e9f86f0
,    0x3f8ee58f
,    0x3e9f5a73
,    0x3f8ed1a1
,    0x3e9f2e09
,    0x3f8ebdb8
,    0x3e9f01b2
,    0x3f8ea9d5
,    0x3e9ed56d
,    0x3f8e95f8
,    0x3e9ea93b
,    0x3f8e8220
,    0x3e9e7d1a
,    0x3f8e6e4d
,    0x3e9e510d
,    0x3f8e5a81
,    0x3e9e2512
,    0x3f8e46b9
,    0x3e9df929
,    0x3f8e32f7
,    0x3e9dcd52
,    0x3f8e1f3b
,    0x3e9da18d
,    0x3f8e0b84
,    0x3e9d75db
,    0x3f8df7d2
,    0x3e9d4a3b
,    0x3f8de426
,    0x3e9d1ead
,    0x3f8dd080
,    0x3e9cf331
,    0x3f8dbcdf
,    0x3e9cc7c7
,    0x3f8da943
,    0x3e9c9c6f
,    0x3f8d95ad
,    0x3e9c712a
,    0x3f8d821c
,    0x3e9c45f6
,    0x3f8d6e91
,    0x3e9c1ad4
,    0x3f8d5b0b
,    0x3e9befc3
,    0x3f8d478a
,    0x3e9bc4c5
,    0x3f8d340f
,    0x3e9b99d8
,    0x3f8d2099
,    0x3e9b6efd
,    0x3f8d0d28
,    0x3e9b4434
,    0x3f8cf9bd
,    0x3e9b197d
,    0x3f8ce657
,    0x3e9aeed7
,    0x3f8cd2f7
,    0x3e9ac442
,    0x3f8cbf9b
,    0x3e9a99bf
,    0x3f8cac45
,    0x3e9a6f4e
,    0x3f8c98f5
,    0x3e9a44ee
,    0x3f8c85aa
,    0x3e9a1aa0
,    0x3f8c7264
,    0x3e99f063
,    0x3f8c5f23
,    0x3e99c637
,    0x3f8c4be8
,    0x3e999c1d
,    0x3f8c38b1
,    0x3e997214
,    0x3f8c2581
,    0x3e99481c
,    0x3f8c1255
,    0x3e991e35
,    0x3f8bff2e
,    0x3e98f460
,    0x3f8bec0d
,    0x3e98ca9c
,    0x3f8bd8f1
,    0x3e98a0e8
,    0x3f8bc5db
,    0x3e987746
,    0x3f8bb2c9
,    0x3e984db5
,    0x3f8b9fbd
,    0x3e982435
,    0x3f8b8cb6
,    0x3e97fac6
,    0x3f8b79b4
,    0x3e97d168
,    0x3f8b66b7
,    0x3e97a81a
,    0x3f8b53bf
,    0x3e977ede
,    0x3f8b40cd
,    0x3e9755b2
,    0x3f8b2de0
,    0x3e972c97
,    0x3f8b1af8
,    0x3e97038d
,    0x3f8b0815
,    0x3e96da93
,    0x3f8af537
,    0x3e96b1ab
,    0x3f8ae25e
,    0x3e9688d2
,    0x3f8acf8a
,    0x3e96600b
,    0x3f8abcbc
,    0x3e963754
,    0x3f8aa9f2
,    0x3e960ead
,    0x3f8a972e
,    0x3e95e617
,    0x3f8a846f
,    0x3e95bd92
,    0x3f8a71b4
,    0x3e95951c
,    0x3f8a5eff
,    0x3e956cb8
,    0x3f8a4c4f
,    0x3e954463
,    0x3f8a39a4
,    0x3e951c1f
,    0x3f8a26fe
,    0x3e94f3eb
,    0x3f8a145d
,    0x3e94cbc8
,    0x3f8a01c1
,    0x3e94a3b4
,    0x3f89ef2a
,    0x3e947bb1
,    0x3f89dc98
,    0x3e9453be
,    0x3f89ca0b
,    0x3e942bdb
,    0x3f89b783
,    0x3e940409
,    0x3f89a500
,    0x3e93dc46
,    0x3f899282
,    0x3e93b493
,    0x3f898009
,    0x3e938cf0
,    0x3f896d95
,    0x3e93655e
,    0x3f895b26
,    0x3e933ddb
,    0x3f8948bc
,    0x3e931668
,    0x3f893656
,    0x3e92ef04
,    0x3f8923f6
,    0x3e92c7b1
,    0x3f89119b
,    0x3e92a06d
,    0x3f88ff44
,    0x3e92793a
,    0x3f88ecf2
,    0x3e925215
,    0x3f88daa6
,    0x3e922b01
,    0x3f88c85e
,    0x3e9203fc
,    0x3f88b61b
,    0x3e91dd07
,    0x3f88a3dd
,    0x3e91b621
,    0x3f8891a4
,    0x3e918f4b
,    0x3f887f6f
,    0x3e916885
,    0x3f886d40
,    0x3e9141ce
,    0x3f885b15
,    0x3e911b26
,    0x3f8848ef
,    0x3e90f48e
,    0x3f8836cf
,    0x3e90ce05
,    0x3f8824b2
,    0x3e90a78c
,    0x3f88129b
,    0x3e908122
,    0x3f880088
,    0x3e905ac7
,    0x3f87ee7b
,    0x3e90347b
,    0x3f87dc72
,    0x3e900e3f
,    0x3f87ca6e
,    0x3e8fe812
,    0x3f87b86e
,    0x3e8fc1f4
,    0x3f87a674
,    0x3e8f9be5
,    0x3f87947e
,    0x3e8f75e5
,    0x3f87828d
,    0x3e8f4ff5
,    0x3f8770a0
,    0x3e8f2a13
,    0x3f875eb9
,    0x3e8f0441
,    0x3f874cd6
,    0x3e8ede7d
,    0x3f873af8
,    0x3e8eb8c8
,    0x3f87291e
,    0x3e8e9323
,    0x3f871749
,    0x3e8e6d8c
,    0x3f870579
,    0x3e8e4804
,    0x3f86f3ae
,    0x3e8e228b
,    0x3f86e1e7
,    0x3e8dfd20
,    0x3f86d025
,    0x3e8dd7c5
,    0x3f86be68
,    0x3e8db278
,    0x3f86acaf
,    0x3e8d8d3a
,    0x3f869afb
,    0x3e8d680a
,    0x3f86894c
,    0x3e8d42e9
,    0x3f8677a1
,    0x3e8d1dd7
,    0x3f8665fb
,    0x3e8cf8d4
,    0x3f86545a
,    0x3e8cd3de
,    0x3f8642bd
,    0x3e8caef8
,    0x3f863125
,    0x3e8c8a20
,    0x3f861f91
,    0x3e8c6556
,    0x3f860e02
,    0x3e8c409b
,    0x3f85fc78
,    0x3e8c1bee
,    0x3f85eaf2
,    0x3e8bf750
,    0x3f85d971
,    0x3e8bd2c0
,    0x3f85c7f4
,    0x3e8bae3e
,    0x3f85b67c
,    0x3e8b89cb
,    0x3f85a509
,    0x3e8b6566
,    0x3f85939a
,    0x3e8b410f
,    0x3f85822f
,    0x3e8b1cc6
,    0x3f8570ca
,    0x3e8af88c
,    0x3f855f68
,    0x3e8ad45f
,    0x3f854e0b
,    0x3e8ab041
,    0x3f853cb3
,    0x3e8a8c31
,    0x3f852b5f
,    0x3e8a682f
,    0x3f851a10
,    0x3e8a443a
,    0x3f8508c5
,    0x3e8a2054
,    0x3f84f77f
,    0x3e89fc7c
,    0x3f84e63d
,    0x3e89d8b2
,    0x3f84d500
,    0x3e89b4f6
,    0x3f84c3c7
,    0x3e899147
,    0x3f84b293
,    0x3e896da7
,    0x3f84a163
,    0x3e894a14
,    0x3f849037
,    0x3e89268f
,    0x3f847f10
,    0x3e890318
,    0x3f846dee
,    0x3e88dfaf
,    0x3f845ccf
,    0x3e88bc53
,    0x3f844bb6
,    0x3e889905
,    0x3f843aa0
,    0x3e8875c5
,    0x3f84298f
,    0x3e885293
,    0x3f841883
,    0x3e882f6e
,    0x3f84077b
,    0x3e880c56
,    0x3f83f677
,    0x3e87e94c
,    0x3f83e578
,    0x3e87c650
,    0x3f83d47d
,    0x3e87a361
,    0x3f83c386
,    0x3e878080
,    0x3f83b294
,    0x3e875dac
,    0x3f83a1a6
,    0x3e873ae5
,    0x3f8390bd
,    0x3e87182c
,    0x3f837fd7
,    0x3e86f580
,    0x3f836ef6
,    0x3e86d2e2
,    0x3f835e1a
,    0x3e86b051
,    0x3f834d42
,    0x3e868dcd
,    0x3f833c6e
,    0x3e866b57
,    0x3f832b9e
,    0x3e8648ed
,    0x3f831ad3
,    0x3e862691
,    0x3f830a0c
,    0x3e860442
,    0x3f82f949
,    0x3e85e200
,    0x3f82e88b
,    0x3e85bfcc
,    0x3f82d7d1
,    0x3e859da4
,    0x3f82c71b
,    0x3e857b8a
,    0x3f82b669
,    0x3e85597c
,    0x3f82a5bc
,    0x3e85377c
,    0x3f829513
,    0x3e851589
,    0x3f82846e
,    0x3e84f3a2
,    0x3f8273ce
,    0x3e84d1c9
,    0x3f826331
,    0x3e84affc
,    0x3f825299
,    0x3e848e3d
,    0x3f824205
,    0x3e846c8a
,    0x3f823176
,    0x3e844ae4
,    0x3f8220ea
,    0x3e84294b
,    0x3f821063
,    0x3e8407bf
,    0x3f81ffe0
,    0x3e83e63f
,    0x3f81ef61
,    0x3e83c4cc
,    0x3f81dee6
,    0x3e83a366
,    0x3f81ce70
,    0x3e83820d
,    0x3f81bdfd
,    0x3e8360c0
,    0x3f81ad8f
,    0x3e833f80
,    0x3f819d25
,    0x3e831e4c
,    0x3f818cbf
,    0x3e82fd25
,    0x3f817c5e
,    0x3e82dc0b
,    0x3f816c00
,    0x3e82bafd
,    0x3f815ba7
,    0x3e8299fc
,    0x3f814b51
,    0x3e827907
,    0x3f813b00
,    0x3e82581f
,    0x3f812ab3
,    0x3e823743
,    0x3f811a6a
,    0x3e821674
,    0x3f810a25
,    0x3e81f5b1
,    0x3f80f9e5
,    0x3e81d4fa
,    0x3f80e9a8
,    0x3e81b450
,    0x3f80d96f
,    0x3e8193b2
,    0x3f80c93b
,    0x3e817320
,    0x3f80b90a
,    0x3e81529b
,    0x3f80a8de
,    0x3e813221
,    0x3f8098b6
,    0x3e8111b4
,    0x3f808891
,    0x3e80f154
,    0x3f807871
,    0x3e80d0ff
,    0x3f806855
,    0x3e80b0b7
,    0x3f80583d
,    0x3e80907a
,    0x3f804829
,    0x3e80704a
,    0x3f803819
,    0x3e805026
,    0x3f80280d
,    0x3e80300e
,    0x3f801805
,    0x3e801002
,    0x3f800801
,};

// Total pairs: 1024
