idf_component_register(SRCS "ota_json.c" "blufi_example_main.c"
                            "blufi_security.c"
                            "blufi_init.c"
                    INCLUDE_DIRS ".")
target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
