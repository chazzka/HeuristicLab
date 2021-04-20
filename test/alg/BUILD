cc_test(
    name = "PSOtests",
    srcs = glob(["**/PSO_test.cc"]),
    deps = [
        "//src/alg:PSO",
        "//test/mock:utilsMock",
        "//test/mock:cec20Mock",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "PSONovAtests",
    srcs = glob(["**/PSONovA_test.cc"]),
    deps = [
        "//src/alg:PSONovA",
        "//src/utils:UtilsLib",
        "//test/mock:cec20Mock",
        "@googletest//:gtest_main",
    ],
)



